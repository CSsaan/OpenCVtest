#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//------------------------------【函数声明 & 数值声明】-------------------------------
Mat HVconcat3(Mat image1, Mat image2, Mat image3, int IsHconcat);
void callback(int, void*); //滑动条回调函数

int thresholdValue = 200; //二值化阈值
Mat srcImage, grayImage, thresholdImage, dstImage;

//-----------------------------------【main函数】------------------------------------
int main()
{
	//加载图片
	srcImage = imread("../resources/71.jpg");
	if (!srcImage.data) { cout << "图片加载失败！" << endl; return -1; }
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);//加载为灰度格式
	//滑动条
	namedWindow("最小外接矩形边界");
	createTrackbar("thresholdValue", "最小外接矩形边界", &thresholdValue, 255, callback);

	waitKey(0);
	destroyAllWindows();
	return 0;
}


//-----------------------------------【自定义函数】------------------------------------
/**
 * @brief 滑动条回调函数
 */
void callback(int, void*)
{
	//图像二值化
	threshold(grayImage, thresholdImage, thresholdValue, 255, THRESH_BINARY_INV);
	cvtColor(thresholdImage, thresholdImage, COLOR_GRAY2BGR);//二值化后转为BGR

	//检测凸包
	vector<vector<Point>> contours;//边缘轮廓
	vector<Vec4i> hierarchy;//凸包点集
	findContours(thresholdImage, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));
	//绘制凸包
	vector<vector<Point>>hull(contours.size()); //顶点
	dstImage = Mat::zeros(srcImage.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		convexHull(contours[i], hull[i], false, true);
		drawContours(dstImage, contours, i, Scalar(0, 255, 0), 2, 8, hierarchy); //绘制边轮廓
		drawContours(dstImage, hull, i, Scalar(255, 0, 0), 2, 8, Mat());//根据顶点绘制凸包
	}

	//寻找最小外接矩形
	RotatedRect box = minAreaRect(thresholdImage); //格式为((cx,cy),(w,h),angle)
	//将RotateRect转换为4个顶点格式
	Point2f vertex[4];
	box.points(vertex); //box的四个顶点 -> vertex[4]

	//绘制最小外接矩形
	srcImage.copyTo(dstImage); //复制一份原图，用于绘制线条
	for (int i = 0; i < 4; i++)
	{
		line(dstImage, vertex[i], vertex[(i + 1) % 4], Scalar(100, 200, 200), 2, LINE_AA);
	}


	//图像拼接(1:横向拼接，other:纵向拼接)
	Mat result = HVconcat3(srcImage, thresholdImage, dstImage, 1);

	//下采样缩小
	pyrDown(result, result);
	//显示频谱幅值图
	imshow("最小外接矩形边界", result);
}

/**
 * @brief 3张图像横向/纵向拼接
 * @param imageN 第N张待拼接图像
 * @param IsHconcat 1:横向拼接，other:纵向拼接
 * @return Mat 拼接结果
 */
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

