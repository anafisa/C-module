#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <Python.h>
#include <math.h>


/* ----| help functions |---- */

unsigned long long int fact(int n){
	unsigned long long int result = 1;
	while(n-1){
		result *= n;
		n -= 1;
	}

	return result;
}


int binomial(int n, int k){
	return n-k ? fact(n) / (fact(k) * fact(n-k)) : 1;
}


/* ----| module body |---- */

/* geometry probability */
static PyObject *geometric_prob(PyObject *self, PyObject *args) {

	double a, b, result;

	if(!PyArg_ParseTuple(args, "dd", &a, &b)){
		return NULL;
	}

	result = a/b;

	return Py_BuildValue("d", result);
}


/* conditional probability */

/* conditional probability */
static PyObject *conditional_prob(PyObject *self, PyObject *args) {
	double a, b, result;

	if(!PyArg_ParseTuple(args, "dd", &a, &b)){
		return NULL;
	}

	result = a/b;

	return Py_BuildValue("d", result);
}

/* complete probability */
static PyObject *complete_prob(PyObject *self, PyObject *args) {

	PyObject *ahi, *hi, *ahipo, *hipo;
	double result=0, k, p;
	int lenahi, lenhi;

	if(!PyArg_ParseTuple(args, "OO", &ahi, &hi)){
		return NULL;
	}

	lenahi = PySequence_Fast_GET_SIZE(ahi);
	lenhi = PySequence_Fast_GET_SIZE(hi);

	if(lenahi != lenhi){
		return NULL;  /* isklu4enie */
	}

	int i;
	for(i=0; i < lenahi; i++){
		ahipo = PySequence_Fast_GET_ITEM(ahi, i);
		hipo = PySequence_Fast_GET_ITEM(hi, i);
		k = PyFloat_AsDouble(ahipo);
		p = PyFloat_AsDouble(hipo);
		result += k * p;
	}

	return Py_BuildValue("d", result);
}

/* Bayes theorem */
static PyObject *bayes_theorem(PyObject *self, PyObject *args) {
	PyObject *ahi, *hi, *ahipo, *hipo;
	double complete_prob=0, k, p, result;
	int lenahi, lenhi, t;

	if(!PyArg_ParseTuple(args, "OOi", &ahi, &hi, &t)){
		return NULL;
	}

	lenahi = PySequence_Fast_GET_SIZE(ahi);
	lenhi = PySequence_Fast_GET_SIZE(hi);

	if(lenahi != lenhi){
		return NULL;  /* isklu4enie */
	}

	int i;
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


/* Bernoulli scheme */

/* simple */
static PyObject *bernoulli_simple(PyObject *self, PyObject *args) {
	
	double p, result = 0;
	int n, k; 

	if(!PyArg_ParseTuple(args, "dii", &p, &n, &k)){
		return NULL;
	}

	
	result = binomial(n, k) * pow(p, k) * pow(1-p, n-k);

	return Py_BuildValue("d", result);

}

/* interval */
static PyObject *bernoulli_interval(PyObject *self, PyObject *args) {
	
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

/* multiple tests */
static PyObject *bernoulli_multiple(PyObject *self, PyObject *args) {

	PyObject *eventsc, *probs, *kpo, *ppo;
	double p, result=1;
	int n=0, k, lene, lenp;
	
	if(!PyArg_ParseTuple(args, "OO", &eventsc, &probs)){
		return NULL;
	}

	lene = PySequence_Fast_GET_SIZE(eventsc);
	lenp = PySequence_Fast_GET_SIZE(probs);

	if(lene != lenp){
		return NULL;  /* isklu4enie */
	}
	
	int i;
	for(i=0; i < lene; i++){
		kpo = PySequence_Fast_GET_ITEM(eventsc, i);
		k = PyLong_AsLong(kpo);
		n += k;
	}
	
	for(i=0; i < lene; i++){
		kpo = PySequence_Fast_GET_ITEM(eventsc, i);
		ppo = PySequence_Fast_GET_ITEM(probs, i);
		if(!PyLong_Check(kpo)){
			return NULL;  /* isklu4enie */
		}
		k = PyLong_AsLong(kpo);
		p = PyFloat_AsDouble(ppo);
		result *= binomial(n, k) * pow(p, k);
		n -= k;
	}

	return Py_BuildValue("d", result);
}


static PyObject *local_moivre_laplace(PyObject *self, PyObject *args) {
	double p, q, x, result;
	int n, k;

	double my_pi = 3.14159;

	if(!PyArg_ParseTuple(args, "dii", &p, &n, &k)){
		return NULL;
	}

	q = 1-p;
	x = (k - n*p) / sqrt(n*p*q);
	result = pow(sqrt(2*my_pi*n*p*q), -1) * exp(-pow(x, 2)/2);

	return Py_BuildValue("d", result);
}



/* ----| module def |---- */


static PyMethodDef ProbStatMethods[] = {
	/* geometric probability */
	{
		"geometric_prob",
		geometric_prob,
		METH_VARARGS,
		"Takes 2 arguments a, b, where a is a satisfying area and b is the whole area. Returns the geometric probability"
	},

	/* conditional probability */
	{
		"conditional_prob",
		conditional_prob,
		METH_VARARGS,
		"Takes 2 arguments P(a, b) and P(b). Returns the probability event A, provided that event b occured."
	},

	/* complete probability */
	{
		"complete_prob",
		complete_prob,
		METH_VARARGS,
		"Takes 2 arguments, lists: { P(Hi) } and { P(A|Hi) }. Returns the complete probability."
	},

	/* Bayes theorem */
	{
		"bayes_theorem",
		bayes_theorem,
		METH_VARARGS,
		"Takes 3 arguments, lists: { P(Hi) } and { P(A|Hi) } and i. Returns the probability P(Hi|A)."
	},

	/* Bernoulli simple */
	{
		"bernoulli_simple",
		bernoulli_simple,
		METH_VARARGS,
		"Takes 3 arguments p, n, k. Returns the probability of an event with the probability p in a single test to occur k times in a series of n tests."
	},

	/* Bernoulli interval */
	{
		"bernoulli_interval",
		bernoulli_interval,
		METH_VARARGS,
		"Takes 4 arguments p, n, k1, k2. Returns the probability of an event with the probability p in a single test to occur from k1 to k2 times in a series of n tests."
	},

	/* Bernoulli multiple tests */
	{
		"bernoulli_multiple",
		bernoulli_multiple,
		METH_VARARGS,
		"Takes 2 lists as [k1, k2, ... ], [p1, p2, ... ]. Returns the probability to 1-st event with probability p1 to occur k1 times and 2-nd event with probability p2 to occur k2 times and so on in k1 + k2 + ... = n times."
	},

	/* Local theorem of de Moivre-Laplace */
	{
		"local_moivre_laplace",
		local_moivre_laplace,
		METH_VARARGS,
		"Takes 3 arguments p, n, k. Returns the probability of an event with the probability p in a single test to occur k times in a series of n tests. The probability is calculated by the local theorem of de Moivre-Laplace."
	},

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
