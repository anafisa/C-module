#include <Python.h>
#include <math.h>

static PyObject *py_probability(PyObject *self, PyObject *args) {
    double x, y, result;

    if (!PyArg_ParseTuple(args,"dd", &x, &y)) {
        return NULL;
    }
    result = x/y;
    return Py_BuildValue("d", result);
}

static PyObject *py_expectation(PyObject *self, PyObject *args) {
    PyObject *pylist1, *pylist2;
    double  double_item1, double_item2, len1, len2;
    double result = 0;
    int i;

    if (!PyArg_ParseTuple(args,"OO",&pylist1, &pylist2)) {
        return NULL;
    }
    len1 = PySequence_Fast_GET_SIZE(pylist1);
    len2 = PySequence_Fast_GET_SIZE(pylist2);
    if (len1 != len2)
        return NULL;

    for(i=0; i<len1; i++) {
        PyObject *item1 = PySequence_Fast_GET_ITEM(pylist1,i);
        PyObject *item2 = PySequence_Fast_GET_ITEM(pylist2,i);
        double_item1 = PyFloat_AsDouble(item1);
        double_item2 = PyFloat_AsDouble(item2);
        result += double_item1 * double_item2;
    }

    return Py_BuildValue("d", result);
}

static PyObject *py_dispersion(PyObject *self, PyObject *args) {
    PyObject *pylist1, *pylist2;
    double double_item1, double_item2, len1, len2;
    double result1 = 0, result2 = 0;
    int i;

    if (!PyArg_ParseTuple(args, "OO", &pylist1, &pylist2)) {
        return NULL;
    }
    len1 = PySequence_Fast_GET_SIZE(pylist1);
    len2 = PySequence_Fast_GET_SIZE(pylist2);
    if (len1 != len2)
        return NULL;

    for (i = 0; i < len1; i++) {
        PyObject *item1 = PySequence_Fast_GET_ITEM(pylist1, i);
        PyObject *item2 = PySequence_Fast_GET_ITEM(pylist2, i);
        double_item1 = PyFloat_AsDouble(item1);
        double_item2 = PyFloat_AsDouble(item2);
        result1 += double_item1 * double_item2;
        result2 += pow(double_item1,2) * double_item2;
    }

    return Py_BuildValue("d", result2 - pow(result1,2));
}



static PyMethodDef ownmod_methods[] = {
        {
                "probability", // название функции внутри python
                py_probability, // функция C
                      METH_VARARGS, // макрос, поясняющий, что функция у нас с аргументами
                         "calculate probability" // документация для функции внутри python
        },
        {
                "expectation", // название функции внутри python
                py_expectation, // функция C
                    METH_VARARGS, // макрос, поясняющий, что функция у нас с аргументами
                "calculate expectation" // документация для функции внутри python
        },
        {
                "dispersion", // название функции внутри python
                py_dispersion, // функция C
                METH_VARARGS, // макрос, поясняющий, что функция у нас с аргументами
                "calculate dispersion" // документация для функции внутри python
        },
        { NULL, NULL, 0, NULL } // так называемый sentiel. Сколько бы элементов структуры у вас не было, этот нулевой элемент должен быть всегда, и при этом быть последним
};


static PyModuleDef simple_module = {
        /* Описываем наш модуль */
        PyModuleDef_HEAD_INIT, // обязательный макрос
        "probstat", // my_plus.__name__
        "amazing documentation", // my_plus.__doc__
        -1,
        ownmod_methods // сюда передаем методы модуля
};

// в названии функции обязательно должен быть префикс PyInit
PyMODINIT_FUNC PyInit_probstat(void) {
    PyObject* m;
// создаем модуль
    m = PyModule_Create(&simple_module);
// если все корректно, то эта проверка не проходит
    if (m == NULL)
        return NULL;
    return m;
}