#include<opencv2/opencv.hpp>
#include<iostream>
#include<cmath>
using namespace cv;
using namespace std;

//------------------------------【函数声明 & 数值声明】-------------------------------
void callback0(int, void*); //归一化回调函数
void callback1(int, void*); //对数变换回调函数
Mat HVconcat2(Mat image1, Mat image2, int IsHconcat);

Mat srcImage;
int method;
//-----------------------------------【main函数】------------------------------------
int main()
{
	//加载原图
	srcImage = imread("../resources/89.png");
	if (srcImage.empty()) { cout << "载入图片失败！" << endl; }

	//归一化
	//// namedWindow("0:L1归一化|1:INF归一化|2:L2归一化|3:MinMax归一化");
	//createTrackbar("method", "0:L1归一化|1:INF归一化|2:L2归一化|3:MinMax归一化", &method, 1, callback0);
	//对数变换
	namedWindow("对数变换");
	createTrackbar("method", "对数变换", &method, 1, callback1);

	waitKey(0);
	destroyAllWindows();

	return 0;
}

//-----------------------------------【自定义函数】------------------------------------
/**
 * @brief 滑动条回调函数(图像数组归一化)
 */
void callback0(int, void*)
{
	Mat resultImage;
	int nowMethod;
	switch (method)
	{
	case(0):
		nowMethod = NORM_INF;
		break;
	case(1):
		nowMethod = NORM_MINMAX;
		break;
	default:
		nowMethod = NORM_MINMAX;
		break;
	}
	normalize(srcImage, resultImage, 255.0, 0.0, nowMethod);
	Mat result = HVconcat2(srcImage, resultImage, 1); //图像拼接
	//下采样缩小
	/*pyrDown(result, result);*/
	//显示频谱幅值图
	imshow("0:L1归一化|1:INF归一化|2:L2归一化|3:MinMax归一化", result);
}

/**
 * @brief 滑动条回调函数(对数变换)
 */
void callback1(int, void*)
{
	Mat grayImage, logImage;
	cvtColor(srcImage, grayImage, COLOR_RGB2GRAY); //先转灰度
	double minValue, maxValue;    // 最大值，最小值
	cv::Point  minIdx, maxIdx;    // 最小值坐标，最大值坐标
	minMaxLoc(grayImage, &minValue, &maxValue, &minIdx, &maxIdx);
	float c = 255.0 / log(1.0 + maxValue);
	logImage = grayImage.clone();
	for (int i = 0; i < grayImage.rows; i++)
	{
		for (int j = 0; j < grayImage.cols; j++)
		{
			logImage.at<float>(i, j, 0) = c * log(grayImage.at<float>(i, j, 0) + 1.0);
			cout << i << "," << j << endl;
		}
	}
	imshow("对数变换", logImage);
	Mat result = HVconcat2(grayImage, logImage, 1); //图像拼接
	//下采样缩小
	pyrDown(result, result);
	//显示频谱幅值图

}

/**
 * @brief 2张图像横向/纵向拼接
 * @param imageN 第N张待拼接图像
 * @param IsHconcat 1:横向拼接，other:纵向拼接
 * @return Mat 拼接结果
 */
Mat HVconcat2(Mat image1, Mat image2, int IsHconcat = 1)
{
	vector<Mat>vhImage;
	vhImage.push_back(image1);
	vhImage.push_back(image2);
	Mat result;
	1 == IsHconcat ? hconcat(vhImage, result) : vconcat(vhImage, result); //三元运算符
	return result;
}

