/*
 * pyA13_I2C.c
 *
 * Copyright 2013 Stefan Mavrodiev <support@olimex.com>
 *
 * This program is free software; you can redistribute it and/or modify
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
#include "i2c.h"

int bus;
int address;
int fd;


static PyObject* py_init(PyObject* self, PyObject* args){
    int a;
    
    if(!PyArg_ParseTuple(args, "i", &a)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse argument");
        return NULL;
    }
    
    bus = a;
    
    Py_RETURN_NONE;
}
static PyObject* py_read(PyObject* self, PyObject* args){
    
    int bytes;
    int i;
    PyObject *pylist, *item;
    
    if (!PyArg_ParseTuple( args, "i", &bytes)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse argument");
        return NULL;
    }
        
    unsigned char in[bytes];


    I2C_Read(&fd, in, bytes);
    
    pylist = PyList_New(bytes);
    
    if(pylist != NULL){
        for(i = 0; i < bytes; i++){
            item = PyInt_FromLong(in[i]);
            PyList_SET_ITEM(pylist, i, item);
        }
    }       
    return pylist;
}
static PyObject* py_open(PyObject* self, PyObject* args){
    
    if(!PyArg_ParseTuple(args, "i", &address)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse argument");
        return NULL;
    }
        
    if (I2C_Open(&fd, bus, address) < 0){
        PyErr_SetString(PyExc_IOError, "Failed to open bus");
        return NULL;
    }
        
    Py_RETURN_NONE;
}
static PyObject* py_write(PyObject* self, PyObject* args){
    
    PyObject *list;
    PyObject *item;
    int i,n;
    
    
    if(!PyArg_ParseTuple(args, "O!", &PyList_Type, &list)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse argument");
        return NULL;
    }
    
    n = PyList_Size(list);
    
    if(n < 0){
        PyErr_SetString(PyExc_ValueError, "Invalid number of byte to send");
        return NULL;
    }
        
    unsigned char a[n];
    
    for(i = 0; i < n; i++){
        item = PyList_GetItem(list, i);
        a[i] = (unsigned char)PyInt_AsLong(item);      
    }
    
    if(I2C_Send(&fd, a, n) < 0){
        PyErr_SetString(PyExc_IOError, "Failed to send data");
        return NULL;
    }
    
    Py_RETURN_NONE;
}
static PyObject* py_close(PyObject* self, PyObject *args){
    if(I2C_Close(&fd) < 0){
        PyErr_SetString(PyExc_IOError, "Failed to close bus");
        return NULL;
    }
    Py_RETURN_NONE;
}

PyMethodDef module_methods[] = {
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
    "I2C",
    NULL,
    -1,
    module_methods
};
#endif

PyMODINIT_FUNC initI2C(void) {
    PyObject* module = NULL;


#if PY_MAJOR_VERSION >= 3
    module = PyModule_Create(&module_methods);
#else
    module = Py_InitModule("I2C", module_methods);
#endif


    if(module == NULL)
#if PY_MAJOR_VERSION >= 3
        return module;
#else
        return;
#endif

}



