#include "CVUtils.h"

void CVUtils::ReadImage()
{
    TestImg1 = cv::imread("./CamTest.jpg");
    TestImg2 = cv::imread("./DepthTest.jpg");
}

void CVUtils::ShowImage()
{
    namedWindow("TestWindow", WINDOW_AUTOSIZE);
    cv::imshow("opencvtest", TestImg1);
    cv::waitKey(0);
}

ImageFrame* CVUtils::SelectKeyFrame()
{
    for (int i = 0; i < ImageFrameList.size(); i++)
    {
        
    }
    return nullptr;
    
}

float CVUtils::CalculateMinCost(float BaseLineDist, float overlapFraction, float TrackError)
{
    if (cv::abs(BaseLineDist) <= MinBaseLineDist)
    {
        return ErrorCostArg;
    }
    else
    {
        return OmegaB/BaseLineDist + OmegaA*(1.0f - overlapFraction) + OmegaE*TrackError;
    }
}

void CVUtils::StereoRectify()
{
    /*
    Mat GrayImage1, GrayImage2, TempImage1, TempImage2, Canvas;
    TestImg1 = imread("TestImage1.jpg", IMREAD_COLOR);
    cv::cvtColor(TestImg1, GrayImage1, COLOR_BGR2GRAY);
    TestImg2 = imread("TestImage2.jpg", IMREAD_COLOR);
    cv::cvtColor(TestImg2, GrayImage2, COLOR_BGR2GRAY);
    double OriginImgW = TestImg1.cols;
    double OriginImgH = TestImg1.rows;
    double ResizeImgW = OriginImgW / 6.0;
    double ResizeImgH = OriginImgH / 6.0;
    //cv::resize(GrayImage1, TempImage1, Size(ResizeImgW, ResizeImgH), 0, 0, INTER_LINEAR);
    //cv::resize(GrayImage2, TempImage2, Size(ResizeImgW, ResizeImgH), 0, 0, INTER_LINEAR);
    Mat Result(OriginImgH, OriginImgW*2 + 1, GrayImage1.type());

    GrayImage1.colRange(0, OriginImgW).copyTo(Result.colRange(0, OriginImgW));
    GrayImage2.colRange(0, OriginImgW).copyTo(Result.colRange(OriginImgW + 1, Result.cols));

    Mat cameraMatrixL = (Mat_<double>(3, 3) << 682.55880, 0, 384.13666,
        0, 682.24569, 311.19558,
        0, 0, 1);
    Mat distCoeffL = (Mat_<double>(5, 1) << -0.51614, 0.36098, 0.00523, -0.00225, 0.00000);

    Mat cameraMatrixR = (Mat_<double>(3, 3) << 685.03817, 0, 397.39092,
        0, 682.54282, 272.04875,
        0, 0, 1);
    Mat distCoeffR = (Mat_<double>(5, 1) << -0.46640, 0.22148, 0.00947, -0.00242, 0.00000);

    const int imageWidth = 256;                             //摄像头的分辨率  
    const int imageHeight = 256;
    Size imageSize = Size(imageWidth, imageHeight);
    Rect validRoi[2];

    Mat T = (Mat_<double>(3, 1) << -61.34485, 2.89570, -4.76870);//T平移向量
    Mat rec = (Mat_<double>(3, 1) << -0.00306, -0.03207, 0.00206);//rec旋转向量
    Mat R = Mat::eye(3,3,CV_64F);//R 旋转矩阵
    */

    // Distortion matrix size is (5,1) which contains 3 axial and 2 tangent distortion coefficients
    // Camera matrix is in form of [fx, 0, cx, 0, fy, cy, 0, 0, 1]
    // Camera's relative matrixes R and T are needed to be initialized before running rectification
    cv::stereoRectify(CameraMatrix1, DistortionVector1, CameraMatrix2, DistortionVector2, OriginImg1.size(),
            CameraRelativeRotationMatrix, CameraRelativeTranslationMatrix, RectifyRotationMatrix1, RectifyRotationMatrix2,
            RectifyProjecMatrix1, RectifyProjecMatrix2, DisparityMatrix);
    
    Mat Map1X, Map1Y, Map2X, Map2Y;
    Mat ImgU1, ImgU2, ImgRectify, Img1Teste, Img2Test2;
    initUndistortRectifyMap(CameraMatrix1, DistortionVector1, RectifyRotationMatrix1, RectifyProjecMatrix1, RectifyImg1.size(), CV_32FC1, Map1X, Map1Y);
    initUndistortRectifyMap(CameraMatrix2, DistortionVector2, RectifyRotationMatrix2, RectifyProjecMatrix2, RectifyImg2.size(), CV_32FC1, Map2X, Map2Y);

    cv::remap(OriginImg1, RectifyImg2, Map1X, Map1Y, INTER_LINEAR);
    cv::remap(OriginImg2, RectifyImg2, Map2X, Map2Y, INTER_LINEAR);

    Mat ResultCanvas(RectifyImg1.rows, RectifyImg1.cols + RectifyImg2.cols, RectifyImg1.type());
    RectifyImg1.colRange(0, RectifyImg1.cols - 1).copyTo(ResultCanvas.colRange(0, RectifyImg1.cols - 1));
    RectifyImg2.colRange(0, RectifyImg2.cols - 1).copyTo(ResultCanvas.colRange(0, RectifyImg2.cols - 1));
    imshow("Recitification Image", ResultCanvas);
    cv::waitKey(0);

}

void CVUtils::BilateralSplatBlur(Mat3f ReferenceImg, Mat3f TargetImg)
{
    
    InputImage TmpImg(ReferenceImg);
    Mat3f mat_bg_in = TmpImg.get_Image(IMG_YUV);
    BilateralGridSolver = new BilateralGrid(mat_bg_in);
    BilateralGridSolver->construct_SliceMatrix_for_depth();
    BilateralGridSolver->construct_BlurMatrix();
    BilateralGridSolver->calc_Bistochastic();
    BilateralGridSolver->construct_AMatrix_step1();
    BilateralGridSolver->set_DepthImage(TargetImg);
    BilateralGridSolver->construct_AMatrix_step2_for_depth();
    BilateralGridSolver->execute_ICCG_for_depth(ICCG_LOOP_MAX, ICCG_EPS);
    BilateralGridSolver->show_Image(BG_DEPTHSUPERRESOLUTED);

}