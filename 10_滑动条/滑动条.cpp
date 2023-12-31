#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//TrackBar发生改变的回调函数
void onChangeTrackBar(int, void*);

int strengh = 7;
Mat srcImage;

int main()
{
	// 原始图片
	srcImage = imread("../resources/4blur.jpg");
	
	//新建滑动条窗口
	namedWindow("blur强度效果");
	createTrackbar("strengh", "blur强度效果", &strengh, 255, onChangeTrackBar, 0);
	
	waitKey(0); //在main里waitKey
	destroyAllWindows();
	return 0;
}

void onChangeTrackBar(int, void*)
{
	// blur混合模糊
	Mat blurImage;
	blur(srcImage, blurImage, Size(strengh, strengh));

	//图像拼接
	Mat result;
	vector<Mat> hImage;
	hImage.push_back(srcImage);
	hImage.push_back(blurImage);
	hconcat(hImage, result);

	//下采样缩小
	pyrDown(result, result);

	//显示
	imshow("blur强度效果", result);
}