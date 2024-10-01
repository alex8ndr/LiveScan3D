#include "KinectCapture.h"
#include <iostream>

KinectCapture::KinectCapture() : align_to_color(RS2_STREAM_COLOR), is_pipeline_started(false) {}

KinectCapture::~KinectCapture() {
    if (is_pipeline_started) {
        try {
            pipe.stop();
            std::cout << "RealSense pipeline stopped." << std::endl;
        }
        catch (const rs2::error& e) {
            std::cerr << "Error stopping RealSense pipeline: " << e.what() << std::endl;
        }
    }
}

bool KinectCapture::Initialize() {
    try {
        cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
        cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_RGB8, 30);
        pipe.start(cfg);
        is_pipeline_started = true;
        return true;
    }
    catch (const rs2::error& e) {
        std::cerr << "RealSense error calling " << e.get_failed_function() << "("
            << e.get_failed_args() << "): " << e.what() << std::endl;
        return false;
    }
}

bool KinectCapture::AcquireFrame() {
    if (!is_pipeline_started) {
        std::cerr << "Pipeline not started. Call Initialize() first." << std::endl;
        return false;
    }

    try {
        rs2::frameset frames = pipe.wait_for_frames();
        frames = align_to_color.process(frames);

        rs2::depth_frame depth = frames.get_depth_frame();
        rs2::video_frame color = frames.get_color_frame();

        if (!depth || !color) {
            return false;
        }

        GetDepthFrame(depth);
        GetColorFrame(color);
        return true;
    }
    catch (const rs2::error& e) {
        std::cerr << "Error acquiring frame: " << e.what() << std::endl;
        return false;
    }
}

void KinectCapture::GetDepthFrame(const rs2::depth_frame& depthFrame) {
    nDepthFrameWidth = depthFrame.get_width();
    nDepthFrameHeight = depthFrame.get_height();
    pDepth = new UINT16[nDepthFrameWidth * nDepthFrameHeight];
    memcpy(pDepth, depthFrame.get_data(), nDepthFrameWidth * nDepthFrameHeight * sizeof(UINT16));
}

void KinectCapture::GetColorFrame(const rs2::video_frame& colorFrame) {
    nColorFrameWidth = colorFrame.get_width();
    nColorFrameHeight = colorFrame.get_height();
    pColorRGBX = new RGB[nColorFrameWidth * nColorFrameHeight];
    memcpy(pColorRGBX, colorFrame.get_data(), nColorFrameWidth * nColorFrameHeight * sizeof(RGB));
}

// Implement these methods based on RealSense SDK capabilities
void KinectCapture::MapDepthFrameToCameraSpace(Point3f* pCameraSpacePoints) {
    // Implementation using RealSense SDK
}

void KinectCapture::MapColorFrameToCameraSpace(Point3f* pCameraSpacePoints) {
    // Implementation using RealSense SDK
}

void KinectCapture::MapDepthFrameToColorSpace(Point2f* pColorSpacePoints) {
    // Implementation using RealSense SDK
}

void KinectCapture::MapColorFrameToDepthSpace(Point2f* pDepthSpacePoints) {
    // Implementation using RealSense SDK
}