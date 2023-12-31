#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
using namespace cv;

//OpenCV3需额外加入：
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"


int main()
{
	// 数据属性
	int width = 512;
	int height = 512;
	Mat image = Mat::zeros(height, width,CV_8UC3);

	// 创建训练数据
	int labels[4] = { 1, -1, -1, -1 };
	Mat labelsMat(4, 1, CV_32SC1, labels);

	float trainingData[4][2] = { {501, 10}, {255, 10}, {501, 255}, {10, 501} };
	Mat trainingDataMat(4, 2, CV_32FC1, trainingData);

	// 设置SVM参数
	CvSVMParams params;
	params.KernelTypes

	// train

	// 显示决定区域

	// 显示训练数据

	// 显示支持向量



	return 0;
}