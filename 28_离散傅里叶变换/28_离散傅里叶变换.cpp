#include <opencv2/core/core.hpp>
/**
	离散傅里叶变换率是表征图像中灰度变化剧烈程度的指标。

	频谱图上的各点与图像上的各点并不存在一一对应关系。
	傅里叶频谱图上所看到的明亮不一的亮点实际上是图像上某一点与邻域点差异的强弱，
	即梯度的大小，也即该点频率的大小.
*/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdarg.h>

using namespace cv;
using namespace std;

//声明拼接矩阵
Mat HVconcat(Mat image1, Mat image2, int IsHconcat);

int main()
{
	Mat srcImage = imread("../resources/1cat.jpg", 0); // 以灰度打开图片
	if (!srcImage.data) { return -1; }

	//延扩到傅里叶变换的最佳尺寸
	int h = getOptimalDFTSize(srcImage.rows);
	int w = getOptimalDFTSize(srcImage.cols);

	//添加的边缘为0padding
	Mat padded;
	copyMakeBorder(srcImage, padded, 0, h - srcImage.rows, 0, w - srcImage.cols, BORDER_CONSTANT, Scalar::all(0));

	//将padded与zero（傅里叶变换结果的实部、虚部）通道合并
	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat complexI;
	merge(planes, 2, complexI);

	//DFT变换
	dft(complexI, complexI);

	//将复数转换为幅值，即=> log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
	split(complexI, planes); // 将多通道数组complexI分离成几个单通道数组，planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]);// 计算二维矢量的幅值（实部、虚部、output）
	Mat magnitudeImage = planes[0];

	magnitudeImage += Scalar::all(1);
	log(magnitudeImage, magnitudeImage);//求自然对数

	//剪切和重分布幅度图象限     
	magnitudeImage = magnitudeImage(Rect(0, 0, magnitudeImage.cols & -2, magnitudeImage.rows & -2)); //若有奇数行或奇数列，进行频谱裁剪 
	//重新排列傅立叶图像中的象限，使得原点位于图像中心  
	int cx = magnitudeImage.cols / 2;
	int cy = magnitudeImage.rows / 2;
	Mat q0(magnitudeImage, Rect(0, 0, cx, cy));   // ROI区域的左上
	Mat q1(magnitudeImage, Rect(cx, 0, cx, cy));  // ROI区域的右上
	Mat q2(magnitudeImage, Rect(0, cy, cx, cy));  // ROI区域的左下
	Mat q3(magnitudeImage, Rect(cx, cy, cx, cy)); // ROI区域的右下
	//交换象限（左上与右下进行交换）
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	//交换象限（右上与左下进行交换）
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	//归一化，用0到1之间的浮点值将矩阵变换为可视的图像格式
	normalize(magnitudeImage, magnitudeImage, 0, 1, NORM_MINMAX);

	//图像拼接(1:横向拼接，other:纵向拼接)
	cvtColor(padded, padded, COLOR_GRAY2BGR);
	cvtColor(magnitudeImage, magnitudeImage, COLOR_GRAY2BGR);
	//Mat result = HVconcat(padded, magnitudeImage, 1);
	//下采样缩小
	//pyrDown(result, result);

	//显示频谱幅值图
	imshow("原始图片", padded);
	imshow("频谱幅值", magnitudeImage);
	waitKey(0);
	destroyAllWindows();
	return 0;
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
	1 == IsHconcat ? hconcat(vhImage, result) : vconcat(vhImage, result);
	return result;
}