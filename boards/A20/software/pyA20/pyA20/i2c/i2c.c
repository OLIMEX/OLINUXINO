/*
 * 
 * This file is part of pyA20.
 * i2c.c is python I2C extension.
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

#include "i2c_lib.h"

/* Define some global variables */
char bus[30];
int address;
int fd;

/**
 * Initialize module by giving bus name.
 * 
 * @param self
 * @param args string with the name of the i2c bus
 * @return none
 */
static PyObject* py_init(PyObject* self, PyObject* args) {

    char *temp;

    /* Parse arguments */
    if (!PyArg_ParseTuple(args, "s", &temp)) {
        return NULL;
    }

    /* Copy name to the global variable */
    strcpy(bus, temp);

    Py_RETURN_NONE;
}

/**
 * Read n bytes from i2c-bus
 * 
 * @param self
 * @param args number of bytes to read
 * @return tuple containing the read bytes
 */
static PyObject* py_read(PyObject* self, PyObject* args) {

    int bytes;
    int i;

    PyObject *pylist = 0;
    PyObject *item = 0;

    /* Parse arguments */
    if (!PyArg_ParseTuple(args, "i", &bytes)) {
        return NULL;
    }

    /* If bytes are <= 0 this could cause segmentation fault */
    if (bytes <= 0) {
        PyErr_SetString(PyExc_ValueError, "i2c read: invalid number of bytes to read");
        return NULL;
    }

    /* Allocate memory for the temp buffer */
    uint8_t *read_buffer = (uint8_t *) malloc(bytes * sizeof (uint8_t));


    /* Read bytes and check return status */
    if (i2c_read(fd, read_buffer, bytes) < 0) {
        Py_DECREF(pylist);
//        Py_DECREF(item);
        free(read_buffer);
        return PyErr_SetFromErrno(PyExc_IOError);
    }

    /* Make new list with size n bytes */
    pylist = PyList_New(bytes);

    /* Fill list with data from read_buffer */
    if (pylist != NULL) {
        for (i = 0; i < bytes; i++) {
            item = PyInt_FromLong(read_buffer[i]);
            PyList_SET_ITEM(pylist, i, item);
        }
    }

    /* Cleanup */
    free(read_buffer);
//    Py_DECREF(item);

    /* Return list */
    return pylist;
}

/**
 * Open I2C device
 * 
 * @param self
 * @param args
 * @return 
 */
static PyObject* py_open(PyObject* self, PyObject* args) {

    /* Parse arguments */
    if (!PyArg_ParseTuple(args, "i", &address)) {
        return NULL;
    }
    
    /* Open slave device*/
    fd = i2c_open(bus, address);
    if (fd < 0) {
        return PyErr_SetFromErrno(PyExc_IOError);
    }

    Py_RETURN_NONE;
}

/**
 * Write bytes to I2C slave device
 * 
 * @param self
 * @param args bytes to send
 * @return none
 */
static PyObject* py_write(PyObject* self, PyObject* args) {

    PyObject *list = 0;
    PyObject *item = 0;

    /* Parse arguments */
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &list)) {
        return NULL;
    }

    /* Get length of the list */
    uint8_t n = PyList_Size(list);


    /* Allocate memory */
    uint8_t *buffer = (uint8_t*)malloc(n * sizeof(uint8_t));
    
    /* Populate output buffer */
    uint8_t i;
    for (i = 0; i < n; i++) {
        item = PyList_GetItem(list, i);
        buffer[i] = (unsigned char) PyInt_AsLong(item);
    }

    /* Send data */
    if (i2c_send(fd, buffer, n) < 0) {
        return PyErr_SetFromErrno(PyExc_IOError);
    }
    
    /* Do cleanup */
    free(buffer);
    Py_DECREF(list);
//    Py_DECREF(item);
    
    Py_RETURN_NONE;
}

/**
 * Close communication with I2C slave device
 * 
 * @param self
 * @param args none
 * @return none
 */
static PyObject* py_close(PyObject* self, PyObject *args) {
    
    /* Close file descriptor */
    if (i2c_close(fd) < 0) {
        return PyErr_SetFromErrno(PyExc_IOError);
    }
    Py_RETURN_NONE;
}

static PyMethodDef module_methods[] = {
    {"init", py_init, METH_VARARGS, "Initialize module"},
    {"open", py_open, METH_VARARGS, "Open file descriptor"},
    {"close", py_close, METH_NOARGS, "Close file descriptor"},
    {"read", py_read, METH_VARARGS, "Read n bytes from I2C bus"},
    {"write", py_write, METH_VARARGS, "Write n bytes to I2C bus"},
    {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef module_def = {
    PyModuleDef_HEAD_INIT,
    "i2c",
    NULL,
    -1,
    module_methods
};
#endif

PyMODINIT_FUNC
#if PY_MAJOR_VERSION >= 3
    PyInit_i2c(void) {
#else
    initi2c(void) {
#endif

    PyObject* module = NULL;

#if PY_MAJOR_VERSION >= 3
    module = PyModule_Create(&module_def);
#else
    module = Py_InitModule("i2c", module_methods);
#endif


    if (module == NULL)
#if PY_MAJOR_VERSION >= 3
        return NULL;
#else
        return;
#endif

    #if PY_MAJOR_VERSION >= 3
        return module;
    #else
        return;
    #endif

}



