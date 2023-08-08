#include <opencv.hpp>
using namespace cv;

int main()
{
	Mat Kernel = getGaussianKernel(49, -1);
	std::cout << "==C风格==\n" << cv::format(Kernel, cv::Formatter::FMT_NUMPY) << std::endl;
	/*for (int i = 0; i < Kernel.cols; i++)
	{
		for (int j = 0; j < Kernel.rows; j++)
		{

		}
	}*/
	return 0;
}