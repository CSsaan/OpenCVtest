#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	// 原始图片
	Mat srcImage = imread("../resources/40ed.jpg");

	// blur混合模糊
	Mat blurImage;
	blur(srcImage, blurImage, Size(3, 3));

	//图像拼接
	Mat result;
	vector<Mat> hImage;
	hImage.push_back(srcImage);
	hImage.push_back(blurImage);
	hconcat(hImage, result);

	//下采样缩小
	//pyrDown(result, result);

	//显示
	imshow("blur模糊", result);
	waitKey(0);
	destroyAllWindows();
	return 0;
}