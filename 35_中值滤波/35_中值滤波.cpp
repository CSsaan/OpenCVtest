#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//图像拼接函数声明
Mat HVconcat(Mat image1, Mat image2, int IsHconcat);
//TrackBar发生改变的回调函数
//void onChangeTrackBar(int, void*);

Mat srcImage;
int kzise = 5;

int main()
{
	//原图
	srcImage = imread("../resources/1cat.jpg");
	if (!srcImage.data) { cout << "加载图片失败！" << endl; return -1; }

	//新建滑动条窗口
	namedWindow("中值滤波");
	createTrackbar("kzise", "中值滤波", &kzise, 255, 0);

	while (1)
	{
		//中值滤波
		Mat medianImage;
		//ksize改为奇数
		int newksize = kzise;
		if (newksize % 2 != 1)
		{
			newksize += 1;
		}
		medianBlur(srcImage, medianImage, newksize);

		//图像拼接(1:横向拼接，other:纵向拼接)
		Mat result = HVconcat(srcImage, medianImage, 1);
		//下采样缩小
		pyrDown(result, result);

		//显示图
		imshow("中值滤波", result);
		waitKey(30);
	}
	
	destroyAllWindows();
	return 0;
}

/**
 * @brief 滑动条回调函数，中值滤波以及显示
 */
//void onChangeTrackBar(int, void*)
//{
//	//中值滤波
//	Mat medianImage;
//	//ksize改为奇数
//	int newksize = kzise;
//	if (newksize % 2 != 1)
//	{
//		newksize += 1;
//	}
//	medianBlur(srcImage, medianImage, newksize);
//
//	//图像拼接(1:横向拼接，other:纵向拼接)
//	Mat result = HVconcat(srcImage, medianImage, 1);
//	//下采样缩小
//	pyrDown(result, result);
//
//	//显示图
//	imshow("中值滤波", result);
//	waitKey(0);
//}

/**
 * @brief 两张图像横向/纵向拼接
 * @param image1 第一张待拼接图像
 * @param image2 第二张待拼接图像
 * @param IsHconcat 1:横向拼接，other:纵向拼接
 * @return Mat 拼接结果
 */
Mat HVconcat(Mat image1, Mat image2, int IsHconcat = 1)
{
	vector<Mat>vhImage;
	vhImage.push_back(image1);
	vhImage.push_back(image2);
	Mat result;
	1 == IsHconcat ? hconcat(vhImage, result) : vconcat(vhImage, result); //三元运算符
	return result;
}
