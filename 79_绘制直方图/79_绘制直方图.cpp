#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//------------------------------【函数声明 & 数值声明】-------------------------------
Mat HVconcat3(Mat image1, Mat image2, Mat image3, int IsHconcat);

int thresholdValue = 200; //二值化阈值
Mat srcImage, HSVImage, thresholdImage, dstImage;

//-----------------------------------【main函数】------------------------------------
int main()
{
	//加载图片
	srcImage = imread("../resources/71.jpg");
	if (!srcImage.data) { cout << "图片加载失败！" << endl; return -1; }
	cvtColor(srcImage, HSVImage, COLOR_BGR2HSV);//加载为灰度格式
	//
	int channels[] = { 0,1 };
	MatND dstHist;
	int histSize[] = { 30,32 };
	// 定义色调的变化范围为0到179
	float hueRanges[] = { 0, 180 };
	//定义饱和度的变化范围为0（黑、白、灰）到255（纯光谱颜色）
	float saturationRanges[] = { 0, 256 };
	const float* ranges[] = { hueRanges, saturationRanges };

	//直方图计算
	calcHist(&HSVImage, 1, channels, Mat(), dstHist, 2, histSize, ranges, true, false);

	//绘制直方图
	double maxValue = 0.0;
	minMaxLoc(dstHist, 0, &maxValue, 0, 0);
	Mat histImg = Mat::zeros(32 * 10, 30 * 10, CV_8UC3);

	for (int hue = 0; hue < 30; hue++)
		for (int saturation = 0; saturation < 32; saturation++)
		{
			float binValue = dstHist.at<float>(hue, saturation);
			int intensity = cvRound((double)binValue * 255 / maxValue);
			rectangle(histImg, Point(hue * 10, saturation * 10),
				Point((hue + 1) * 10 - 1, (saturation + 1) * 10 - 1),
				Scalar::all(intensity), FILLED);
		}
	imshow("srcImage", srcImage);
	imshow("histImage", histImg);
	waitKey(0);
	destroyAllWindows();
	return 0;
}

//-----------------------------------【自定义函数】------------------------------------
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