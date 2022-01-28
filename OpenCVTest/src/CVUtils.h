#pragma once

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>
#include <vector>
#include <string>

#include "ImageFrame.h"
#include "InputImage.h"
#include "BilateralGrid.h"

using namespace cv;
using namespace cv::ximgproc;
using namespace std;

class CVUtils
{
public:
    void ReadImage();

    void ShowImage();

    void RectifyRemap(Mat LeftImageCameraMatrix, Mat LeftDistortMatrix, Mat RightImageCameraMatrix, Mat RightDistortMatrix);

    ImageFrame* SelectKeyFrame();

    float CalculateMinCost(float BaseLineDist, float overlapFraction, float TrackError);

    void StereoRectify();

    void BilateralSplatBlur(Mat3f ReferenceImg, Mat3f TargetImg);

    float OmegaA = 0.8f;
    float OmegaB = 0.4f;
    float OmegaE = 0.5f;
    float MinBaseLineDist = 4.0f;
    float ErrorCostArg = 1.0f;

    Mat CameraMatrix1;
    Mat CameraMatrix2;
    Mat DistortionVector1;
    Mat DistortionVector2;
    Mat OriginImg1;
    Mat OriginImg2;
    Mat RectifyImg1;
    Mat RectifyImg2;
    Mat CameraRelativeRotationMatrix;
    Mat CameraRelativeTranslationMatrix;
    Mat RectifyRotationMatrix1;
    Mat RectifyRotationMatrix2;
    Mat RectifyProjecMatrix1;
    Mat RectifyProjecMatrix2;
    Mat DisparityMatrix;
    Mat TestImg1;
    Mat TestImg2;

    vector<ImageFrame*> ImageFrameList;

    BilateralGrid* BilateralGridSolver;
};

/*
float CVUtils::OmegaB = 0.4f;
float CVUtils::OmegaA = 0.8f;
float CVUtils::OmegaE = 0.5f;
float CVUtils::MinBaseLineDist = 4.0f;
float CVUtils::ErrorCostArg = 1.0f;
*/

