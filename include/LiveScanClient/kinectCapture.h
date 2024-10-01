#pragma once

#include <librealsense2/rs.hpp>
#include "ICapture.h"
#include "utils.h"

class KinectCapture : public ICapture
{
public:
    KinectCapture();
    ~KinectCapture();
    bool Initialize();
    bool AcquireFrame();
    void MapDepthFrameToCameraSpace(Point3f* pCameraSpacePoints);
    void MapColorFrameToCameraSpace(Point3f* pCameraSpacePoints);
    void MapDepthFrameToColorSpace(Point2f* pColorSpacePoints);
    void MapColorFrameToDepthSpace(Point2f* pDepthSpacePoints);

private:
    rs2::pipeline pipe;
    rs2::config cfg;
    rs2::align align_to_color;
    bool is_pipeline_started;

    void GetDepthFrame(const rs2::depth_frame& depthFrame);
    void GetColorFrame(const rs2::video_frame& colorFrame);
};