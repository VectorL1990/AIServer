#include <stdio.h>
#include <iostream>

#include <opencv4/opencv2/core.hpp>
#include "opencv4/opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <vector>
#include <string>

#include "CVUtils.h"

using namespace std;
using namespace cv;


int main()
{
    CVUtils* CVUtilsInstance = new CVUtils();
    CVUtilsInstance->TestImg1 = cv::imread("./CamTest.jpg");
    CVUtilsInstance->TestImg2 = cv::imread("./DepthTest.jpg");
    //CVUtilsInstance->ReadImage();
    //CVUtilsInstance->ShowImage();
    //CVUtilsInstance->StereoRectify();
    CVUtilsInstance->BilateralSplatBlur(CVUtilsInstance->TestImg1, CVUtilsInstance->TestImg2);
}
