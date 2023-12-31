#include<opencv2/opencv.hpp>
#include<iostream>

using namespace cv;
using namespace std;

int main()
{
	string fL_video = "D:\\OpenCVProject\\OpenCVtest\\resources\\222.mp4"; // *视频文件路径*
	VideoCapture cap(fL_video);         // *openCV加载视频*
	Mat frame;                          // *初始化视频帧矩阵*
	cap.read(frame);                    // *加载视频帧*
	//*获取视频信息*
	double fps = cap.get(CAP_PROP_FPS);            // *视频fps*
	double frames = cap.get(CAP_PROP_FRAME_COUNT); // *视频总帧数*
	int height = cap.get(CAP_PROP_FRAME_HEIGHT);   // *视频帧高*
	int width = cap.get(CAP_PROP_FRAME_WIDTH);     // *视频帧宽*
	cout << "视频帧率：" << fps << endl;
	cout << "视频总帧数：" << frames << endl;
	cout << "视频帧高、宽：[" << height << "," << width << "]" << endl;

	//*视频写入*
	cv::VideoWriter out;
	out.open("D:\\OpenCVProject\\OpenCVtest\\resources\\0my_video.mp4", VideoWriter::fourcc('D', 'I', 'V', 'X'), fps, cv::Size(height, width), true);
	int frame_index = frames - 1;
	if (cap.isOpened())
	{
		while (frame_index != 0)
		{
			cap.set(CAP_PROP_POS_FRAMES, frame_index); //倒序 定位到帧
			cap.read(frame); //读取当前帧
			out.write(frame); //写入视频帧
			frame_index = frame_index - 1;
			cout << "[" << frames - frame_index << "/" << frames << "]" << endl;
		}
	}

	cap.release();
	out.release();

	return 0;
}

