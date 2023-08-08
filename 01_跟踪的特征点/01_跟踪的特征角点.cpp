#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

vector<Point2f> features;	// 初始化跟踪点的位置
int maxCount = 500;	// 检测的最大特征数
double qLevel = 0.01;	// 特征检测的等级
double minDist = 10.0;	// 两特征点之间的最小距离

int main()
{
	// 载入原图  
	Mat srcImage = imread("../resources/1cat.jpg");

	// 先转灰度
	Mat gray, featuresImage;
	cvtColor(srcImage, gray, COLOR_BGR2GRAY);
	// 获取特征点
	srcImage.copyTo(featuresImage);
	goodFeaturesToTrack(gray, features, maxCount, qLevel, minDist);
	// 绘制所以特征角点
	for (size_t i = 0; i < features.size(); i++)
	{
		circle(featuresImage, features[i], 3, Scalar(0, 255, 0), -1);
	}

	//图像拼接
	vector<Mat>vImage;
	vImage.push_back(srcImage);
	vImage.push_back(featuresImage);
	Mat result;
	hconcat(vImage, result);

	//下采样缩小
	pyrDown(result, result);
	//显示效果图 
	imshow("特征点检测", result);

	waitKey(0);
	destroyAllWindows();
	return 0;
}