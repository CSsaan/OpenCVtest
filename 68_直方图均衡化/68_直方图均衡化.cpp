#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

//------------------------------【函数声明 & 数值声明】-------------------------------
Mat HVconcat2(Mat image1, Mat image2, int IsHconcat); //图像拼接

Mat srcImage;
Mat equalizeHistImage;
//-----------------------------------【main函数】------------------------------------
int main()
{
	//加载图片
	srcImage = imread("../resources/68.jpg", 0); //以灰度图打开
	if (!srcImage.data) { return -1; }

	//直方图均衡化
	equalizeHist(srcImage, equalizeHistImage);

	//图像拼接
	Mat result = HVconcat2(srcImage, equalizeHistImage, 1);
	pyrDown(result, result);
	imshow("直方图均衡化", result);
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
Mat HVconcat2(Mat image1, Mat image2, int IsHconcat = 1)
{
	vector<Mat>vhImage;
	vhImage.push_back(image1);
	vhImage.push_back(image2);
	Mat result;
	1 == IsHconcat ? hconcat(vhImage, result) : vconcat(vhImage, result); //三元运算符
	return result;
}
