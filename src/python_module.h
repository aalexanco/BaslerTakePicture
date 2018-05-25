#ifndef _PYTHON_MODULE_H_
#define _PYTHON_MODULE_H_

#include <Python.h>
#include <boost/python.hpp>
#include <opencv2/opencv.hpp>

// Camera struct to be exported to Python
struct Camera {
    PyObject *captureImage();
};

// AcquirePythonInterpreterLock instances must
// be used in a multithreaded programs in order
// to be able to call python code from different
// threads.
//
// More details: https://bit.ly/2HmUlBT
struct AcquirePythonInterpreterLock {
  PyGILState_STATE state;
  AcquirePythonInterpreterLock() {
    state = PyGILState_Ensure();
  }

  ~AcquirePythonInterpreterLock() {
    PyGILState_Release(state);
  }
};

// ReleasePythonInterpreterLock instances must
// be used in a multithreaded programs in order
// to be able to call C++ code from python regardless
// the thread python code is running.
//
// More details: https://bit.ly/2HmUlBT
struct ReleasePythonInterpreterLock {
  PyThreadState *state;
  ReleasePythonInterpreterLock() {
    state = PyEval_SaveThread();
  }
  ~ReleasePythonInterpreterLock() {
    PyEval_RestoreThread(state);
  }
};

void savePythonInterpreterThread();


PyObject *toPyObject(cv::Mat mat);


#endif // _PYTHON_MODULE_H_
