#include "cvcapture.h"

//#include <QImage>
//#include <QDebug>


namespace mmicv {

CvCapture::CvCapture()
    : m_width(0)
    , m_height(0)
    , m_oneShot(true)
    , camera(nullptr)
    , m_grabbing(false)
    , m_fromFile(false)
    , m_camRunning(false)
{
        startCamera();
}

CvCapture::~CvCapture()
{
    if (!m_fromFile && m_camRunning)
        stopCamera();
}

void CvCapture::startCamera()
{
    PylonInitialize();

    try {
        camera = new CInstantCamera(CTlFactory::GetInstance().CreateFirstDevice());
        cout << "Using device " << camera->GetDeviceInfo().GetModelName() << endl;
        camera->Open();
        m_camRunning = true;
    }

    catch (GenICam::GenericException &e) {
        m_fromFile = true;
        m_camRunning = false;
        //Error handling
        cerr << "An exception occurred." << endl << e.GetDescription() << endl;
    }
}

void CvCapture::stopCamera()
{
    if (camera != nullptr) {
        delete camera;
        camera = nullptr;
    }
    PylonTerminate();
}

cv::Mat CvCapture::cvMat() const
{
    cv::Mat mat;
    mat.create(m_height, m_width, CV_8UC3);
    memcpy(mat.data, m_data, mat.total() * mat.elemSize());
    cvtColor(mat, mat, CV_BGR2RGB);
    return mat;
}


void CvCapture::grabFromFile()
{
    //qDebug() << __PRETTY_FUNCTION__;
    // cv::Mat mat = fromFile(m_file.toStdString());
    // m_width = mat.size().width;
    // m_height = mat.size().height;
    // m_data = mat.data;
    // emit dataChanged();
}

bool CvCapture::grabFromCamera()
{
    if (m_fromFile)
        return false;

    camera->StartGrabbing();
    CPylonImage image;
    CImageFormatConverter fc;
    fc.OutputPixelFormat = PixelType_Mono8;
    while(camera->IsGrabbing()){
        CGrabResultPtr ptrGrabResult;
        camera->RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
        if (ptrGrabResult.IsValid() && ptrGrabResult->GrabSucceeded()) {
            fc.Convert(image, ptrGrabResult);
            m_width = ptrGrabResult->GetWidth();
            m_height = ptrGrabResult->GetHeight();
            m_data = image.GetBuffer();
            if (oneShot())
                camera->StopGrabbing();
        } else {
            return false;
        }
    }

    return true;
}

void CvCapture::stop()
{
    if (!m_fromFile && camera && camera->IsGrabbing()) {
        camera->StopGrabbing();
    }
}

void CvCapture::setResData(const std::string &data)
{
    if(m_resData == data)
    {
        return;
    }
    
    m_resData = data;
    if (!m_resData.empty()) {
        Pylon::CFeaturePersistence::LoadFromString(Pylon::String_t(m_resData.c_str()),
                                         &camera->GetNodeMap(), true);
    }
}

void CvCapture::setResDataByPath(const std::string &data)
{
    Pylon::CFeaturePersistence::Load(Pylon::String_t(data.c_str()),
     &camera->GetNodeMap(), true);
}


std::vector<cv::Mat> CvCapture::getImage(int shots)
{
    if (!m_camRunning || camera == nullptr)
        throw std::runtime_error("Camera failed to initialize");

    std::vector<cv::Mat> mats;
    camera->StartGrabbing(shots);
    CPylonImage image;
    CImageFormatConverter fc;
    fc.OutputPixelFormat = PixelType_RGB8packed;
    CGrabResultPtr ptrGrabResult;
    int width = 0;
    int height= 0;

    while(camera->IsGrabbing()){
        camera->RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
        if (ptrGrabResult->GrabSucceeded()) {
            fc.Convert(image, ptrGrabResult);
            width = ptrGrabResult->GetWidth();
            height = ptrGrabResult->GetHeight();
            cv::Mat mat;
            mat.create(height, width, CV_8UC3);
            memcpy(mat.data, image.GetBuffer(), mat.total() * mat.elemSize());
            cvtColor(mat, mat, CV_BGR2RGB);
            mats.push_back(mat);
        }
    }

    return mats;
}

cv::Mat CvCapture::fromFile(const std::string &path)
{
    m_image = cv::imread(path);
    return m_image;
}

} // namespace mmicv
