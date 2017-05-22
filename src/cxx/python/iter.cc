/*
** Copyright (C) 2011 Fargier Sylvain <fargier.sylvain@free.fr>
**
** This software is provided 'as-is', without any express or implied
** warranty.  In no event will the authors be held liable for any damages
** arising from the use of this software.
**
** Permission is granted to anyone to use this software for any purpose,
** including commercial applications, and to alter it and redistribute it
** freely, subject to the following restrictions:
**
** 1. The origin of this software must not be misrepresented; you must not
**    claim that you wrote the original software. If you use this software
**    in a product, an acknowledgment in the product documentation would be
**    appreciated but is not required.
** 2. Altered source versions must be plainly marked as such, and must not be
**    misrepresented as being the original software.
** 3. This notice may not be removed or altered from any source distribution.
**
** iter.cc
**
**        Created on: Wed 02 Feb 2011 11:12:14 PM CET
**            Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#include <Python.h>

typedef struct {
  PyObject_HEAD
  long int m;
  long int i;
} spam_MyIter;

PyObject* spam_MyIter_iter(PyObject *self)
{
  Py_INCREF(self);
  return self;
}

PyObject* spam_MyIter_iternext(PyObject *self)
{
  spam_MyIter *p = (spam_MyIter *)self;
  if (p->i < p->m) {
    PyObject *tmp = Py_BuildValue("l", p->i);
    (p->i)++;
    return tmp;
  } else {
    /* Raising of standard StopIteration exception with empty value. */
    PyErr_SetNone(PyExc_StopIteration);
    return NULL;
  }
}

static PyTypeObject spam_MyIterType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "spam._MyIter",            /*tp_name*/
    sizeof(spam_MyIter),       /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    0,                         /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_ITER,
      /* tp_flags: Py_TPFLAGS_HAVE_ITER tells python to
         use tp_iter and tp_iternext fields. */
    "Internal myiter iterator object.",           /* tp_doc */
    0,  /* tp_traverse */
    0,  /* tp_clear */
    0,  /* tp_richcompare */
    0,  /* tp_weaklistoffset */
    spam_MyIter_iter,  /* tp_iter: __iter__() method */
    spam_MyIter_iternext  /* tp_iternext: next() method */
};

static PyObject *
spam_myiter(PyObject *self, PyObject *args)
{
  long int m;
  spam_MyIter *p;

  if (!PyArg_ParseTuple(args, "l", &m))  return NULL;

  /* I don't need python callable __init__() method for this iterator,
     so I'll simply allocate it as PyObject and initialize it by hand. */

  p = PyObject_New(spam_MyIter, &spam_MyIterType);
  if (!p) return NULL;

  /* I'm not sure if it's strictly necessary. */
  if (!PyObject_Init((PyObject *)p, &spam_MyIterType)) {
    Py_DECREF(p);
    return NULL;
  }

  p->m = m;
  p->i = 0;
  return (PyObject *)p;
}

static PyObject *
spam_tester(PyObject *self, PyObject *args)
{
  PyObject *obj, *iter, *item;

  if (!PyArg_ParseTuple(args, "O:tester", &obj)) return NULL;

  iter = PyObject_GetIter(obj);
  if (!iter) {
    printf("Not an iterable\n");
    return NULL;
  }

printf("iter : %s\n", obj->ob_type->tp_name);
  while ((item = PyIter_Next(iter))) {
    printf("item: %s\n", item->ob_type->tp_name);
    Py_DECREF(item);
  }
  Py_DECREF(iter);

  PyObject *items = PyDict_Items(obj);
  iter = PyObject_GetIter(items);

  while ((item = PyIter_Next(iter))) {

    PyObject *item2 = PyTuple_GetItem(item, 1);
    //PyTuple_SetItem(item, 1, PyString_FromString("pwet"));

    Py_DECREF(item);
  }
  Py_DECREF(iter);


  Py_DECREF(items);
  Py_DECREF(obj); // is it required?

  return NULL;
}

static PyMethodDef SpamMethods[] = {
    {"myiter",  spam_myiter, METH_VARARGS, "Iterate from i=0 while i<m."},
    {"tester",  spam_tester, METH_VARARGS, "Test method."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
inititer(void)
{
  PyObject* m;

  spam_MyIterType.tp_new = PyType_GenericNew;
  if (PyType_Ready(&spam_MyIterType) < 0)  return;

  m = Py_InitModule("iter", SpamMethods);

  Py_INCREF(&spam_MyIterType);
  PyModule_AddObject(m, "_MyIter", (PyObject *)&spam_MyIterType);
}
