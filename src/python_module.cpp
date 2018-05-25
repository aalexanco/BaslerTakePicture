#include "python_module.h"

#include "mat_converter.h"
#include "cvcapture.h"

#define LOG std::cerr << "[" << __PRETTY_FUNCTION__ << "] "

using std::endl;

PyObject *Camera::captureImage()
{
    try {
        LOG << "Capturing image..." << endl;
        mmicv::CvCapture captureObj;

        const auto &images = captureObj.getImage(1);
        if (images.size() == 0)
            return nullptr;

        cv::Mat mat = images[0];
        LOG << "Generating matrix [rows=" << mat.rows << "]" << endl;
        return matcvt::fromMatToNDArray(mat);
    }
    catch (std::exception &e) {
        LOG << "Error: Failed to capture image: " << e.what() << endl;
        return nullptr;
    }
}


void savePythonInterpreterThread()
{
    PyEval_SaveThread();
}

PyObject *toPyObject(cv::Mat mat)
{
    return matcvt::fromMatToNDArray(mat);
}

BOOST_PYTHON_MODULE(libtakepicture)
{
    using namespace boost::python;
    using namespace matcvt;

    Py_Initialize();
    import_array();

    to_python_converter<cv::Mat, matToNDArrayBoostConverter>();
    matFromNDArrayBoostConverter();

    class_<Camera>("Camera")
        .def("captureImage", &Camera::captureImage)
    ;

}
