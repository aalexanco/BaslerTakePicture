#ifndef CAPTURE_H
#define CAPTURE_H

#include <opencv2/opencv.hpp>
#include <pylon/PylonIncludes.h>
#include <iostream>

using namespace Pylon;
using namespace cv;
using namespace std;

namespace mmicv {

class CvCapture {

public:
    CvCapture();
    virtual ~CvCapture();

    void startCamera();
    void stopCamera();
    cv::Mat fromFile(const std::string &path);
    bool grabFromCamera();
    void grabFromFile();
    std::vector<cv::Mat> getImage(int shots = 1);
    cv::Mat cvMat() const;
    std::string resPath() const { return m_resPath; }
    void setResPath(const std::string &path) { m_resPath = path; }
    bool oneShot() const { return m_oneShot; }
    void setOneShot(bool oneShot) { m_oneShot = oneShot; }
    int width() const { return m_width; }
    int height() const { return m_height; }
    bool grabbing() const { return camera->IsGrabbing(); }
    void stop();
    void setResData(const std::string &data);
    void setResDataByPath(const std::string &data);
    std::vector<cv::Mat> captures() const { return m_captures; }
    bool camRunning() const { return m_camRunning; }

private:
    cv::Mat m_image;
    void *m_data;
    int m_width;
    int m_height;
    std::string m_resPath;
    bool m_oneShot;
    CInstantCamera *camera;
    bool m_grabbing;
    bool m_fromFile;
    std::string m_resData;
    bool m_camRunning;
    std::vector<cv::Mat> m_captures;
};

} // namespace mmicv
#endif //  CAPTURE_H
