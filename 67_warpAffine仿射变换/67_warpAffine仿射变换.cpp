#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//------------------------------【函数声明 & 数值声明】-------------------------------
Mat HVconcat3(Mat image1, Mat image2, Mat image3, int IsHconcat);

Mat srcImage, dstImage_warp, dstImage_warp_rotate;
//-----------------------------------【main函数】------------------------------------
int main()
{
	//加载图片
	srcImage = imread("../resources/55.jpg");

	//两个三角形的6个顶点（3+3）
	Point2f srcTriangle[3];
	Point2f dstTriangle[3];

	//定义变换矩阵（仿射、旋转）
	Mat rotMat(2, 3, CV_32FC1);
	Mat warpMat(2, 3, CV_32FC1);

	/******************** 仿射变换 *************************/
	//设置类型相同

	//两个三角形对应顶点
	srcTriangle[0] = Point2f(0, 0);
	srcTriangle[1] = Point2f(static_cast<float>(srcImage.cols - 1), 0);
	srcTriangle[2] = Point2f(0, static_cast<float>(srcImage.rows - 1));
	dstTriangle[0] = Point2f(static_cast<float>(srcImage.cols * 0.0), static_cast<float>(srcImage.rows * 0.33));
	dstTriangle[1] = Point2f(static_cast<float>(srcImage.cols * 0.65), static_cast<float>(srcImage.rows * 0.35));
	dstTriangle[2] = Point2f(static_cast<float>(srcImage.cols * 0.15), static_cast<float>(srcImage.rows * 0.6));

	//计算变换矩阵
	warpMat = getAffineTransform(srcTriangle, dstTriangle);
	//进行仿射变换
	warpAffine(srcImage, dstImage_warp, warpMat, srcImage.size());

	/******************** 旋转变换 *************************/
	Point center = Point(srcImage.cols / 2, srcImage.rows / 2);
	double angle = -50.0; //顺时针旋转50度
	double scale = 0.6;   //放大0.6倍

	//计算旋转变换矩阵
	rotMat = getRotationMatrix2D(center, angle, scale);
	//进行仿射变换
	warpAffine(dstImage_warp, dstImage_warp_rotate, rotMat, srcImage.size());

	//拼接图片
	Mat result = HVconcat3(srcImage, dstImage_warp, dstImage_warp_rotate, 1);
	pyrDown(result, result);
	imshow("反射变换", result);
	waitKey(0);
	destroyAllWindows();
	return 0;
}


//-----------------------------------【自定义函数】------------------------------------
Mat HVconcat3(Mat image1, Mat image2, Mat image3, int IsHconcat = 1)
{
	vector<Mat>vhImage;
	vhImage.push_back(image1);
	vhImage.push_back(image2);
	vhImage.push_back(image3);
	Mat result;
	1 == IsHconcat ? hconcat(vhImage, result) : vconcat(vhImage, result); //三元运算符
	return result;
}