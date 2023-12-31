#include <opencv2\imgproc\types_c.h>
#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;

//------------------------------【函数声明 & 数值声明】-------------------------------
vector<vector<Point3f>> objPoints; //存放所有世界空间3D角点坐标
vector<Point3f> objWorldPoints; //转换世界空间3D角点坐标
vector<vector<Point2f>> imgPoints; //存放所有图像空间2D角点坐标
vector<Point2f > imgSinglePoints; //每张图片2D角点坐标
String img_path = "D:/OpenCVProject/OpenCVtest/86_相机校正/Test/*.jpg";  //所有畸变图像
vector<String> img_paths;
String toProcessImg_path = "D:/OpenCVProject/OpenCVtest/86_相机校正/Test/1.jpg"; //待校准的图片

Size chessBoardSize = Size(6, 9); //棋盘网格数
bool showCorners = true; //显示检测的角点
TermCriteria criteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.001); //提取亚像元迭代终止条件
Mat srcImage, grayImage, toProcessImage, ProcessedImage; // 读取每张畸变图
Mat cameraMatrix, distCoeffs, R, T; //畸变参数

//-----------------------------------【main函数】------------------------------------
int main()
{
	glob(img_path, img_paths);//读取指定文件夹下图像

	//转世界坐标系3D
	for (int i = 0; i < chessBoardSize.height; i++)
	{
		for (int j = 0; j < chessBoardSize.width; j++)
		{
			objWorldPoints.push_back(Point3f(j, i, 0));
		}
	}

	// 获取每张图所有3D、2D角点
	for (int i = 0; i < img_paths.size(); i++)
	{
		srcImage = imread(img_paths[i]); //每张畸变原图
		cvtColor(srcImage, grayImage, COLOR_BGR2GRAY); //转灰度
		bool findSuccess = findChessboardCorners(grayImage, chessBoardSize, imgSinglePoints,
			CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE); //检测棋盘角点

		//显示角点
		if (findSuccess && showCorners)
		{
			cornerSubPix(grayImage, imgSinglePoints, Size(11, 11), Size(-1, -1), criteria);  //提取亚像元角点
			drawChessboardCorners(srcImage, chessBoardSize, imgSinglePoints, findSuccess); //绘制角点
			//保存所有2d、3d点
			objPoints.push_back(objWorldPoints);
			imgPoints.push_back(imgSinglePoints);
		}
	}

	//计算畸变参数
	calibrateCamera(objPoints, imgPoints, srcImage.size(), cameraMatrix, distCoeffs, R, T);
	cout << "cameraMatrix:" << endl << cameraMatrix << endl;
	cout << "distCoeffs:" << endl << distCoeffs << endl;
	cout << "Rotation vector:" << endl << R << endl;
	cout << "Translation vector:" << endl << T << endl;

	//畸变校正
	toProcessImage = imread(toProcessImg_path);
	undistort(toProcessImage, ProcessedImage, cameraMatrix, distCoeffs);//畸变校正

	imwrite("D:/OpenCVProject/OpenCVtest/86_相机校正/ProcessedImage.jpg", ProcessedImage);
	imwrite("D:/OpenCVProject/OpenCVtest/86_相机校正/drawChessboardCorners.jpg", srcImage);
	cout << "处理完成。" << endl;
	return 0;
}

