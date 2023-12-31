#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main()
{
	// 视频地址
	string dir = "../resources/06_播放视频.avi";
	// 捕获视频
	VideoCapture capture(dir);

	// 循环所有帧
	while (1)
	{
		Mat frame;
		capture >> frame;
		// 判断结束
		if (frame.empty())
		{
			cout << "视频播放结束。" << endl;
			break;
		}
		// 显示
		imshow(dir, frame);
		waitKey(30); // 计算两帧间隔
	}
	destroyAllWindows();
	return 0;
}