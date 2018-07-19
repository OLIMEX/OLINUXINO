/*
 *
 * This file is part of pyA20.
 * spi.c is python SPI extension.
 *
 * Copyright (c) 2014 Stefan Mavrodiev @ OLIMEX LTD, <support@olimex.com>
 *
 * pyA20 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include "Python.h"

#if PY_MAJOR_VERSION >= 3
    #define PyInt_FromLong PyLong_FromLong
    #define PyInt_AsLong PyLong_AsLong
#endif


#include "spi_lib.h"
#include "../utilities/color.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

int fd;

#ifdef __DEBUG
#define debug(format, args...) printf("%s", CYAN); printf(format, args); printf("%s", ESC);
#else
#define debug(...)  ((void)0)
#endif


/**
 * Read n bytes from slave device
 * 
 * @param self
 * @param args number of bytes
 * @return tuple with read elements
 */
static PyObject* py_read(PyObject *self, PyObject* args){

    PyObject *rx_list = 0;
    PyObject *item = 0;
    uint8_t rx_len = 0;

    /* Parse arguments */
    if(!PyArg_ParseTuple(args, "i", &rx_len)){
        return NULL;
    }

    /* Alocate memmory for buffer*/
    uint8_t *rx_buffer = (uint8_t*)malloc(rx_len * sizeof(uint8_t));
    memset(rx_buffer, 0, sizeof(uint8_t)*rx_len);

    /* Read from device*/
    if(spi_read(fd, rx_buffer, rx_len) < 0){
        return PyErr_SetFromErrno(PyExc_IOError);
    }

    /* Make list */
    rx_list = PyList_New(rx_len);

    /* Populate list */
    uint8_t i;
    for(i = 0; i < rx_len; i++){
        item = PyInt_FromLong(rx_buffer[i]);
        PyList_SET_ITEM(rx_list, i, item);
    }

    /* Do cleanup*/
    free(rx_buffer);
    //Py_DECREF(item);

    /* Return list */
    return rx_list;
}

/**
 * Write bytes to slave device
 * 
 * @param self
 * @param args Tuple with data to write
 * @return none
 */
static PyObject* py_write(PyObject *self, PyObject* args){

    PyObject *tx_list;
    PyObject *item = 0;
    uint8_t tx_len = 0;

    /* Parse arguments */
    if(!PyArg_ParseTuple(args, "O!", &PyList_Type, &tx_list)){
        return NULL;
    }

    /* Get length of list */
    tx_len = PyList_Size(tx_list);

    /* Allocate memory for output buffer */
    uint8_t *tx_buffer = (uint8_t *)malloc(tx_len * sizeof(uint8_t));
    memset(tx_buffer, 0, sizeof(uint8_t));

    /* Populate output buffer */
    int i;
    for(i = 0; i < tx_len; i++){
        item = PyList_GetItem(tx_list, i);
        tx_buffer[i] = (uint8_t)PyInt_AsLong(item);
    }

    /* Send data */
    if(spi_write(fd, tx_buffer, tx_len) < 0){
        return PyErr_SetFromErrno(PyExc_IOError);
    }

    /* Do cleanup */
    free(tx_buffer);

    //Py_DECREF(item);
    Py_DECREF(tx_list);
    
    Py_RETURN_NONE;
}


/**
 * Do transfer of data to slave device
 * 
 * @param self
 * @param args tuple of data to read and 
 * @return list with read data
 */
static PyObject* py_xfer(PyObject* self, PyObject* args){

    PyObject *tx_list = 0;
    PyObject *rx_list = 0;
    PyObject *item = 0;

    uint8_t tx_len;
    uint8_t rx_len;

    /* Parse arguments */
    if(!PyArg_ParseTuple(args, "O!i", &PyList_Type, &tx_list, &rx_len)){
        return NULL;
    }

    /* Get length of output data */
    tx_len = PyList_Size(tx_list);

    /* Allocate memory for sending */
    uint8_t* tx_buffer;
    uint8_t* rx_buffer;
    
    tx_buffer = (uint8_t *)malloc(tx_len * sizeof(uint8_t));
    rx_buffer = (uint8_t *)malloc(rx_len * sizeof(uint8_t));

    memset(rx_buffer, 0, sizeof(uint8_t)*rx_len);

    /* Populate both output buffer */
    int i;
    for(i = 0; i < tx_len; i++){
        item = PyList_GetItem(tx_list, i);
        tx_buffer[i] = (uint8_t)PyInt_AsLong(item);
    }

    /* Do the transaction */
    if(spi_xfer(fd, tx_buffer, tx_len, rx_buffer, rx_len) < 0){
        return PyErr_SetFromErrno(PyExc_IOError);
    }

    /* Make new list*/
    rx_list = PyList_New(rx_len);

    /* Populate the new list */
    for(i = 0; i < rx_len; i++){
        item = PyInt_FromLong(rx_buffer[i]);
        PyList_SET_ITEM(rx_list, i, item);
    }

    /* Do cleanup */
    free(tx_buffer);
    free(rx_buffer);

    Py_DECREF(tx_list);
    //Py_DECREF(item);

    return rx_list;
}

/**
 * Open SPI device with given configuration
 * 
 * @param self
 * @param args
 * @param kwargs
 * @return none
 */
static PyObject* py_open(PyObject* self, PyObject* args, PyObject* kwargs){

    int ret;
    char *device;
    spi_config_t config = {0};

    /* Set default values */
    config.mode = 0;
    config.bits_per_word = 8;
    config.speed = 10000000;
    config.delay = 0;

    /* Define keywords */
    static char *kwlist [] = {
        "device", "mode", "bits_per_word", "speed", "delay", NULL
    };

    /* Parse arguments */
    if(!PyArg_ParseTupleAndKeywords(
        args, kwargs, "s|iiii", kwlist,
        &device,
        &config.mode,
        &config.bits_per_word,
        &config.speed,
        &config.delay)){
        return NULL;
    }

    /* Open the device */
    ret = spi_open(device, config);

    if(ret < 0 ){
        return PyErr_SetFromErrno(PyExc_IOError);
    }else{
        fd = ret;
    }

    Py_RETURN_NONE;
}

/**
 * Close file descriptor for SPI bus
 * 
 * @param self
 * @param args
 * @return none
 */
static PyObject* py_close(PyObject* self, PyObject* args){

    int ret;

    ret = spi_close(fd);
    if(ret < 0){
        return PyErr_SetFromErrno(PyExc_IOError);
    }else{
        fd = 0;
    }

    Py_RETURN_NONE;
}
 PyMethodDef module_methods[] = {
    {"open", (PyCFunction)py_open, METH_VARARGS | METH_KEYWORDS, "Open file dssescriptor"},
    {"xfer", py_xfer, METH_VARARGS, "Transfer data"},
    {"write", py_write, METH_VARARGS, "Write data"},
    {"read", py_read, METH_VARARGS, "Read data"},
    {"close", py_close, METH_NOARGS, "Close file descriptor"},
    {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef module_def = {
    PyModuleDef_HEAD_INIT,
    "spi",
    NULL,
    -1,
    module_methods
};
#endif

PyMODINIT_FUNC
#if PY_MAJOR_VERSION >= 3
    PyInit_spi(void) {
#else
    initspi(void) {
#endif
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    PyObject* module = NULL;


#if PY_MAJOR_VERSION >= 3
    module = PyModule_Create(&module_def);
#else
    module = Py_InitModule("spi", module_methods);
#endif


#if PY_MAJOR_VERSION >= 3
        return module;
#else
        return;
#endif

}
