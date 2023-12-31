#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	//载入原图  
	Mat srcImage = imread("../resources/1.jpg");

	//进行腐蚀操作 
	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	Mat dstImage;
	dilate(srcImage, dstImage, element);
	//erode(srcImage, dstImage, element);

	//图像拼接
	vector<Mat>vImage;
	vImage.push_back(srcImage);
	vImage.push_back(dstImage);
	Mat result;
	hconcat(vImage, result);

	//下采样缩小
	pyrDown(result, result);
	//显示效果图 
	imshow("【效果图】腐蚀操作", result);

	waitKey(0);
	destroyAllWindows();
	return 0;
}