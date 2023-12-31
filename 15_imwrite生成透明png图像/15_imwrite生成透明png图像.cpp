#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//--------------------------------【createAlphaMat( )函数】--------------------------------
//		描述：创建带alpha通道的Mat
//-------------------------------------------------------------------------------------------------
void createAlphaMat(Mat& mat)
{
	for (int i = 0; i < mat.rows; ++i) {
		for (int j = 0; j < mat.cols; ++j) {
			Vec4b& rgba = mat.at<Vec4b>(i, j); // Mat4x4转成vec4
			rgba[0] = UCHAR_MAX;
			rgba[1] = saturate_cast<uchar>((float(mat.cols - j)) / ((float)mat.cols) * UCHAR_MAX);
			rgba[2] = saturate_cast<uchar>((float(mat.rows - i)) / ((float)mat.rows) * UCHAR_MAX);
			rgba[3] = saturate_cast<uchar>(0.2 * (rgba[1] + rgba[2]));
		}
	}
}


int main()
{
	Mat mat(480, 640, CV_8UC4);

	createAlphaMat(mat);

	vector<int> compression_params;
	compression_params.push_back(IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	// 显示
	try
	{
		imwrite("../resources/AlphaPNG.png", mat, compression_params);
		imshow("AlphaPNG.png", mat);
		waitKey(0);
	}
	catch (exception& e)
	{
		cout << "生成png失败:" << e.what() << endl;
		return -1;
	}
	destroyAllWindows();
	return 0;

}
