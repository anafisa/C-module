#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

/* ----| Module body |---- */

/* ----| helper-functions |---- */

unsigned long long int fact(int n){
    unsigned long long int result = 1;
    while (n-1) {
        result *= n;
        n -= 1;
    }
    return result;
}

int binomial(int n, int k){
    return n-k ? fact(n) / (fact(k) * fact(n-k)) : 1;
}

/* ----| Module methods |---- */

/* theoretical probability */
static PyObject *py_probability(PyObject *self, PyObject *args) {
    double x, y, result;

    if (!PyArg_ParseTuple(args,"dd", &x, &y)) {
        return NULL;
    }
    result = x/y;
    return Py_BuildValue("d", result);
}

/* geometric probability */
static PyObject *py_geometric_prob(PyObject *self, PyObject *args) {
    double x, y, result;

    if(!PyArg_ParseTuple(args, "dd", &x, &y)){
        return NULL;
    }
    result = x/y;
    return Py_BuildValue("d", result);
}

/* conditional probability */
static PyObject *py_conditional_prob(PyObject *self, PyObject *args) {
    double x, y, result;

    if(!PyArg_ParseTuple(args, "dd", &x, &y)){
        return NULL;
    }
    result = x/y;
    return Py_BuildValue("d", result);
}


/* law of total probability */
static PyObject *py_total_prob(PyObject *self, PyObject *args) {
    PyObject *ahi, *hi, *ahipo, *hipo;
    double result=0, k, p;
    int i, lenahi, lenhi;

    if(!PyArg_ParseTuple(args, "OO", &ahi, &hi)){
        return NULL;
    }
    lenahi = PySequence_Fast_GET_SIZE(ahi);
    lenhi = PySequence_Fast_GET_SIZE(hi);

    if(lenahi != lenhi){
        return NULL;
    }
    for(i=0; i < lenahi; i++){
        ahipo = PySequence_Fast_GET_ITEM(ahi, i);
        hipo = PySequence_Fast_GET_ITEM(hi, i);
        k = PyFloat_AsDouble(ahipo);
        p = PyFloat_AsDouble(hipo);
        result += k * p;
    }
    return Py_BuildValue("d", result);
}

/* independent events */
static PyObject *py_independent_events(PyObject *self, PyObject *args) {
    double x, y, z;

    if(!PyArg_ParseTuple(args, "ddd", &x, &y, &z)){
        return NULL;
    }
    return Py_BuildValue("d", x * y == z);
}

/* bayes rule */
static PyObject *py_bayes_rule(PyObject *self, PyObject *args) {
    PyObject *ahi, *hi, *ahipo, *hipo;
    double complete_prob=0, k, p, result;
    int lenahi, lenhi, t, i;

    if(!PyArg_ParseTuple(args, "OOi", &ahi, &hi, &t)){
        return NULL;
    }
    lenahi = PySequence_Fast_GET_SIZE(ahi);
    lenhi = PySequence_Fast_GET_SIZE(hi);

    if(lenahi != lenhi){
        return NULL;
    }
    for(i=0; i < lenahi; i++){
        ahipo = PySequence_Fast_GET_ITEM(ahi, i);
        hipo = PySequence_Fast_GET_ITEM(hi, i);
        k = PyFloat_AsDouble(ahipo);
        p = PyFloat_AsDouble(hipo);
        complete_prob += k * p;
    }
    ahipo = PySequence_Fast_GET_ITEM(ahi, t-1);
    hipo = PySequence_Fast_GET_ITEM(hi, t-1);
    k = PyFloat_AsDouble(ahipo);
    p = PyFloat_AsDouble(hipo);
    result = (p * k) / complete_prob;

    return Py_BuildValue("d", result);
}

/* simple bernoulli scheme */
static PyObject *py_bernoulli_simple(PyObject *self, PyObject *args) {
    double p, result = 0;
    int n, k;

    if(!PyArg_ParseTuple(args, "dii", &p, &n, &k)){
        return NULL;
    }
    result = binomial(n, k) * pow(p, k) * pow(1-p, n-k);

    return Py_BuildValue("d", result);
}

/* interval bernoulli scheme  */
static PyObject *py_bernoulli_interval(PyObject *self, PyObject *args) {
    double p, result=0;
    int n, k1, k2;

    if(!PyArg_ParseTuple(args, "diii", &p, &n, &k1, &k2)){
        return NULL;
    }
    for(k1; k1 < k2+1; k1++){
        result += binomial(n, k1) * pow(p, k1) * pow(1-p, n-k1);
    }
    return Py_BuildValue("d", result);
}

/* multiple tests bernoulli scheme */
static PyObject *py_bernoulli_multiple(PyObject *self, PyObject *args) {
    PyObject *eventsc, *probs, *kpo, *ppo;
    double p, result=1;
    int n=0, k, lene, lenp, i;

    if(!PyArg_ParseTuple(args, "OO", &eventsc, &probs)){
        return NULL;
    }
    lene = PySequence_Fast_GET_SIZE(eventsc);
    lenp = PySequence_Fast_GET_SIZE(probs);

    if(lene != lenp){
        return NULL;
    }
    for(i=0; i < lene; i++){
        kpo = PySequence_Fast_GET_ITEM(eventsc, i);
        k = PyLong_AsLong(kpo);
        n += k;
    }
    for(i=0; i < lene; i++){
        kpo = PySequence_Fast_GET_ITEM(eventsc, i);
        ppo = PySequence_Fast_GET_ITEM(probs, i);
        if(!PyLong_Check(kpo)){
            return NULL;
        }
        k = PyLong_AsLong(kpo);
        p = PyFloat_AsDouble(ppo);
        result *= binomial(n, k) * pow(p, k);
        n -= k;
    }
    return Py_BuildValue("d", result);
}

/* De Moivre–Laplace local theorem */
static PyObject *py_local_moivre_laplace(PyObject *self, PyObject *args) {
    double p, q, x, result;
    double my_pi = 3.14159;
    int n, k;

    if(!PyArg_ParseTuple(args, "dii", &p, &n, &k)){
        return NULL;
    }
    q = 1-p;
    x = (k - n*p) / sqrt(n*p*q);
    result = pow(sqrt(2*my_pi*n*p*q), -1) * exp(-pow(x, 2)/2);

    return Py_BuildValue("d", result);
}

/* expected value */
static PyObject *py_expected_value(PyObject *self, PyObject *args) {
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

/* variance */
static PyObject *py_variance(PyObject *self, PyObject *args) {
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

/* standard deviation */
static PyObject *py_standard_deviation(PyObject *self, PyObject *args) {
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
    return Py_BuildValue("d", pow(result2 - pow(result1,2), 0.5));
}


/* ----| Module def |---- */

static PyMethodDef ownmod_methods[] = {

        /* theoretical probability */
        {
                "probability",  // python name
                py_probability, // C name
                METH_VARARGS,
                "Takes 2 arguments: A - number of correct events , B - total number of events. Returns P(A)."
        },

        /* geometric probability */
        {
                "geometric_prob",
                py_geometric_prob,
                METH_VARARGS,
                "Takes 2 arguments: A - satisfying area, B - whole area. Returns P(A)."
        },

        /* conditional probability */
        {
                "conditional_prob",
                py_conditional_prob,
                METH_VARARGS,
                "Takes 2 arguments: P(AB) and P(b). Returns P(A|B)."
        },

        /* total probability */
        {
                "total_prob",
                py_total_prob,
                METH_VARARGS,
                "Takes 2 arguments: { P(Hi) }, { P(A|Hi) }. Returns P(A)."
        },

        /* independent events */
        {
                "independent_events",
                py_independent_events,
                METH_VARARGS,
                "Takes 3 arguments: P(A), P(B), P(AB). Returns whether events A and B are independent (0) or not (1)."
        },

        /* bayes theorem */
        {
                "bayes_rule",
                py_bayes_rule,
                METH_VARARGS,
                "Takes 3 arguments: [ P(Hi) ], [ P(A|Hi) ], i. Returns P(Hi|A)."
        },

        /* simple bernoulli scheme */
        {
                "bernoulli_simple",
                py_bernoulli_simple,
                METH_VARARGS,
                "Takes 3 arguments: p, n, k. Returns the probability of an event with the probability p in a single test to occur k times in a series of n tests."
        },

        /* interval bernoulli scheme */
        {
                "bernoulli_interval",
                py_bernoulli_interval,
                METH_VARARGS,
                "Takes 4 arguments: p, n, k1, k2. Returns the probability of an event with the probability p in a single test to occur from k1 to k2 times in a series of n tests."
        },

        /*  multiple bernoulli scheme*/
        {
                "bernoulli_multiple",
                py_bernoulli_multiple,
                METH_VARARGS,
                "Takes 2 arguments: [k1, k2, ... ], [p1, p2, ... ]. Returns the probability to 1-st event with probability p1 to occur k1 times and 2-nd event with probability p2 to occur k2 times and so on in k1 + k2 + ... = n times."
        },

        /* De Moivre–Laplace local theorem */
        {
                "local_moivre_laplace",
                py_local_moivre_laplace,
                METH_VARARGS,
                "Takes 3 arguments: p, n, k. Returns the probability of an event with the probability p in a single test to occur k times in a series of n tests. The probability is calculated by the local theorem of de Moivre-Laplace."
        },

        /* expected value */
        {
                "expected_value",
                py_expected_value,
                    METH_VARARGS,
                "Takes 2 arguments: [x1, x2, ...], [p1, p2,...]. Returns E(x)."
        },

        /* variance */
        {
                "variance",
                py_variance,
                METH_VARARGS,
                "Takes 2 arguments: [x1, x2, ...], [p1, p2,...]. Returns D(x)."
        },

        /* standard deviation */
        {
                "standard_deviation",
                py_standard_deviation,
                METH_VARARGS,
                "Takes 2 arguments: [x1, x2, ...], [p1, p2,...]. Returns standard deviation."
        },
        { NULL, NULL, 0, NULL }
};


static PyModuleDef simple_module = {
        PyModuleDef_HEAD_INIT,
        "probstat", // probstat.__name__
        "probstat - the main helper for students in probability & statistics.\nprobstat methods allow users to deal with complicated tasks and calculations in statistics and probability theory.", // probstat.__doc__
        -1,
        ownmod_methods
};


PyMODINIT_FUNC PyInit_probstat(void) {
    PyObject* m;

    m = PyModule_Create(&simple_module);
    if (m == NULL)
        return NULL;
    return m;
}