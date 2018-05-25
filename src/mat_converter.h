/*
 * CVBoostConverter.hpp
 *
 *  Created on: Mar 20, 2014
 *      Author: Gregory Kramida
 *   Copyright: (c) 2014 Gregory Kramida
 *     License: MIT
 */

#ifndef CVBOOSTCONVERTER_HPP_
#define CVBOOSTCONVERTER_HPP_

#define PY_ARRAY_UNIQUE_SYMBOL matcvt_ARRAY_API
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#include <Python.h>
#include <numpy/ndarrayobject.h>
#include <opencv2/core/core.hpp>
#include <boost/python.hpp>
#include <cstdio>

namespace matcvt {

using namespace cv;

//===================   ERROR HANDLING     =========================================================

int failmsg(const char *fmt, ...);
PyObject* failmsgp(const char *fmt, ...);

//===================   THREADING     ==============================================================
class PyAllowThreads;
class PyEnsureGIL;

static size_t REFCOUNT_OFFSET = (size_t)&(((PyObject*)0)->ob_refcnt) +
    (0x12345678 != *(const size_t*)"\x78\x56\x34\x12\0\0\0\0\0")*sizeof(int);

static inline PyObject* pyObjectFromRefcount(const int* refcount)
{
    return (PyObject*)((size_t)refcount - REFCOUNT_OFFSET);
}

static inline int* refcountFromPyObject(const PyObject* obj)
{
    return (int*)((size_t)obj + REFCOUNT_OFFSET);
}

//===================   NUMPY ALLOCATOR FOR OPENCV     =============================================

class NumpyAllocator;

//===================   STANDALONE CONVERTER FUNCTIONS     =========================================

PyObject* fromMatToNDArray(const Mat& m);
Mat fromNDArrayToMat(PyObject* o);

//===================   BOOST CONVERTERS     =======================================================

struct matToNDArrayBoostConverter {
	static PyObject* convert(Mat const& m);
};


struct matFromNDArrayBoostConverter {

	matFromNDArrayBoostConverter();

	/// @brief Check if PyObject is an array and can be converted to OpenCV matrix.
	static void* convertible(PyObject* object);

	/// @brief Construct a Mat from an NDArray object.
	static void construct(PyObject* object,
			boost::python::converter::rvalue_from_python_stage1_data* data);
};
} // end namespace matcvt
#endif /* CVBOOSTCONVERTER_HPP_ */
