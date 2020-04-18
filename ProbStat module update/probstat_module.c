#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <Python.h>
#include <math.h>


/* ----| help functions |---- */

int fact(int n){
	return n-1 ? n * fact(n-1) : n;
}


int binomial(int n, int k){
	return fact(n) / (fact(k) * fact(n-k));
}


/* ----| module body |---- */

/* geometry probability */
static PyObject *geometry(PyObject *self, PyObject *args) {

	double a, b, result;

	if(!PyArg_ParseTuple(args, "dd", &a, &b)){
		return NULL;
	}

	result = a/b;

	return Py_BuildValue("d", result);
}


/* conditional probability */
static PyObject *conditional(PyObject *self, PyObject *args) {

}


/* Bernoulli scheme */

/* simple */
static PyObject *bernoulliSimple(PyObject *self, PyObject *args) {
	
	double p, result;
	int n, k; 

	if(!PyArg_ParseTuple(args, "dii", &p, &n, &k)){
		return NULL;
	}

	
	result = binomial(n, k) * pow(p, k) * pow(1-p, n-k);

	return Py_BuildValue("d", result);

}

/* interval */
static PyObject *bernoulliInterval(PyObject *self, PyObject *args) {
	
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

/* multiple tests */ /*
static PyObject *bernoulliMultiple(PyObject *self, PyObject *args) {
	
	double p, result=1;
	int k, n=0, l;

	int i=0;
	while(args[i] != '\0'){
		if(!i%2){
			if(!PyArg_ParseTuple(args[i], "i", &k)){
				return NULL;
			}
			n += k;
		}
		i++;
	}

	if(!i%2){
		// исключение: последовательность должна быть четной длины

	l = i;
	for(i=0; i < l; i+=2){
		if(!PyArg_ParseTuple(args[i], "i", &k)){
				return NULL;
			}
		if(!PyArg_ParseTuple(args[i+1], "d", &p)){
				return NULL;
			}
		result *= binomial(n, k) * pow(p, k);
		n -= k;
	}

	return Py_BuildValue("d", result);
} */



/* ----| module def |---- */


static PyMethodDef ProbStatMethods[] = {
	/* geometry probability */
	{
		"geometry",
		geometry,
		METH_VARARGS,
		"Takes 2 arguments a, b, where a is a satisfying area and b is the whole area. Returns the geometric probability"
	},

	/* conditional probability */
	{
		"conditional",
		conditional,
		METH_VARARGS,
		"plus"
	
	},

	/* Bernoulli simple */
	{
		"bernoulliSimple",
		bernoulliSimple,
		METH_VARARGS,
		"Takes 3 arguments p, n, k. Returns the probability of an event with the probability p in a single test to occur k times in a series of n tests."
	
	},

	/* Bernoulli interval */
	{
		"bernoulliInterval",
		bernoulliInterval,
		METH_VARARGS,
		"Takes 4 arguments p, n, k1, k2. Returns the probability of an event with the probability p in a single test to occur from k1 to k2 times in a series of n tests."
	
	},

	/* Bernoulli multiple tests */
	/*{
		"bernoulliMultiple",
		bernoulliMultiple,
		METH_VARARGS,
		"Takes an even-length sequence as a1, p1, a2, p2, ... . Returns the probability to 1-st event with probability p1 to occur k1 times and 2-nd event with probability p2 to occur k2 times and so on in k1 + k2 + ... = n times."
	},*/

	{NULL, NULL, 0, NULL}
};


static struct PyModuleDef probstatmodule = {
	PyModuleDef_HEAD_INIT,
	"ProbStat",
	"Student assistant in the world of probability theory and mathematical statistics",
	-1,
	ProbStatMethods
};


PyMODINIT_FUNC PyInit_ProbStat(void) {
	return PyModule_Create(&probstatmodule);
}
