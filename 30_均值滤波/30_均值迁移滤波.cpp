#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//图像拼接函数声明
Mat HVconcat(Mat image1, Mat image2, int IsHconcat);
//TrackBar发生改变的回调函数
void onChangeTrackBar(int, void*);

Mat srcImage;
int sp = 10;
int sr = 10;

int main()
{
	//原图
	srcImage = imread("../resources/36.jpg");
	if (!srcImage.data) { cout << "加载图片失败！" << endl; return -1; }

	//新建滑动条窗口
	namedWindow("均值迁移滤波");
	createTrackbar("物理空间半径", "均值迁移滤波", &sp, 50, onChangeTrackBar);
	createTrackbar("色彩空间半径", "均值迁移滤波", &sr, 50, onChangeTrackBar);

	waitKey(0); //在main里waitKey
	destroyAllWindows();
	return 0;
}

/**
 * @brief 滑动条回调函数，双边滤波以及显示
 */
void onChangeTrackBar(int, void*)
{
	//均值迁移滤波
	Mat pyrMeanShiftImage;
	pyrMeanShiftFiltering(srcImage, pyrMeanShiftImage, sp, sr);

	//图像拼接(1:横向拼接，other:纵向拼接)
	Mat result = HVconcat(srcImage, pyrMeanShiftImage, 1);
	//下采样缩小
	//pyrDown(result, result);

	//显示图
	imshow("均值迁移滤波", result);
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
