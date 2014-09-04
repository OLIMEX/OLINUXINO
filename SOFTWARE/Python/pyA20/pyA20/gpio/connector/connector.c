/*
 *
 * This file is part of pyA20.
 * connector.c is python GPIO extension.
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
#include "../mapping.h"


 PyMethodDef module_methods[] = {
    {NULL, NULL, 0, NULL}
};


#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef module_def = {
    PyModuleDef_HEAD_INIT,
    "connector",
    NULL,
    -1,
    module_methods
};
#endif

PyMODINIT_FUNC
#if PY_MAJOR_VERSION >= 3
    PyInit_connector(void) {
#else
    initconnector(void) {
#endif
    PyObject* module = NULL;


#if PY_MAJOR_VERSION >= 3
    module = PyModule_Create(&module_def);
#else
    module = Py_InitModule("connector", module_methods);
#endif


    if (module == NULL)
#if PY_MAJOR_VERSION >= 3
        return module;
#else
        return;
#endif

    int i;
    int j;
    char name[20];
    for(i = 0; i < sizeof(gpio)/sizeof(gpio[0]); i++){
        for(j = 0; j < 40; j++){
            if(!strcmp(gpio[i].pins[j].name, "")){
                break;
            }else{
                sprintf(name, "%sp%d", gpio[i].connector_name, gpio[i].pins[j].pin_number);
                PyModule_AddObject(module, name, Py_BuildValue("i", gpio[i].pins[j].offset));
            }
        }
    }

    #if PY_MAJOR_VERSION >= 3
        return module;
    #else
        return;
    #endif
}