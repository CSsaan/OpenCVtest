#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//图像拼接函数声明
Mat HVconcat(Mat image1, Mat image2, int IsHconcat);
//TrackBar发生改变的回调函数
void onChangeTrackBar(int, void*);

Mat srcImage; //原图
int gaussianSize = 5; //高斯核大小

int main()
{
	//加载原图
	srcImage = imread("../resources/36.jpg");
	if (!srcImage.data) { cout << "加载图片失败！" << endl; return -1; }

	//新建滑动条窗口
	namedWindow("高斯滤波");
	createTrackbar("kernel size", "高斯滤波", &gaussianSize, 100, onChangeTrackBar, 0);

	waitKey(0);
	destroyAllWindows();
	return 0;
}

/**
 * @brief 滑动条回调函数，方框滤波以及显示
 */
void onChangeTrackBar(int, void*)
{
	//方框滤波
	Mat gaussianImage;
	//高斯核改为奇数
	int newgaussianSize = gaussianSize;
	if (gaussianSize % 2 != 1)
	{
		newgaussianSize = gaussianSize + 1;
	}
	GaussianBlur(srcImage, gaussianImage, Size(newgaussianSize, newgaussianSize), 0, 0);
	//boxFilter(srcImage, boxImage, -1, Size(boxSize, boxSize));

	//图像拼接(1:横向拼接，other:纵向拼接)
	Mat result = HVconcat(srcImage, gaussianImage, 1);
	//下采样缩小
	//pyrDown(result, result);

	//显示频谱幅值图
	imshow("高斯滤波", result);
}

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
