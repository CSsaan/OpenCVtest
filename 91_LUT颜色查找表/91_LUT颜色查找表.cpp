#include<opencv2/opencv.hpp>
#include<iostream>
#include "math.h"

using namespace cv;
using namespace std;

/* 运行时可配置大小 */
#define vectorscope_width 256
#define vectorscope_height 256
/* 128 也是一个不错的选择，但 256 是最大值。 你和 V 正在使用这个分辨率 */
#define VECTORSCOPE_WIDTH_MAX 256
#define VECTORSCOPE_HEIGHT_MAX 256
static uint8_t* vectorscope = NULL;

void bzero32(void* buf, size_t len) { memset(buf, 0, len); }

static void
vectorscope_clear()
{
	if (vectorscope != NULL)
	{
		bzero32(vectorscope, vectorscope_width * vectorscope_height * sizeof(uint8_t));
	}
}

static void
vectorscope_init()
{
	if (vectorscope == NULL)
	{
		vectorscope = (uint8_t*)malloc(VECTORSCOPE_WIDTH_MAX * VECTORSCOPE_HEIGHT_MAX * sizeof(uint8_t));
		vectorscope_clear();
	}
}

/* 在给定位置绘制<计数>像素的助手。当<计数>大于 1 时不进行换行检查 */
static void
vectorscope_putpixels(uint8_t* bmp_buf, int x_pos, int y_pos, uint8_t color, uint8_t count)
{
	int pos = x_pos + y_pos * vectorscope_width;

	while (count--)
	{
		bmp_buf[pos++] = 255 - color;
		cout << "x_pos:" << x_pos << "  y_pos:" << y_pos << endl;
	}
}

/* 另一个在给定位置绘制色点的帮助程序。
<xc> 和 <yc> 指定示波器图形的中心。
<frac_x> 和 <frac_y> 以 1/2048 为单位，并指定相对点位置。
 */
static void
vectorscope_putblock(uint8_t* bmp_buf, int xc, int yc, uint8_t color, int32_t frac_x, int32_t frac_y)
{
	int x_pos = xc + (((int32_t)vectorscope_width * frac_x) >> 12);   // x_pos:109
	int y_pos = yc + ((-(int32_t)vectorscope_height * frac_y) >> 12); // y_pos:49

	vectorscope_putpixels(bmp_buf, x_pos + 0, y_pos - 4, color, 1);
	vectorscope_putpixels(bmp_buf, x_pos + 0, y_pos + 4, color, 1);

	vectorscope_putpixels(bmp_buf, x_pos - 3, y_pos - 3, color, 7);
	vectorscope_putpixels(bmp_buf, x_pos - 3, y_pos - 2, color, 7);
	vectorscope_putpixels(bmp_buf, x_pos - 3, y_pos - 1, color, 7);
	vectorscope_putpixels(bmp_buf, x_pos - 4, y_pos + 0, color, 9);
	vectorscope_putpixels(bmp_buf, x_pos - 3, y_pos + 1, color, 7);
	vectorscope_putpixels(bmp_buf, x_pos - 3, y_pos + 2, color, 7);
	vectorscope_putpixels(bmp_buf, x_pos - 3, y_pos + 3, color, 7);
}

/* draws the overlay: circle with color dots. */
static void vectorscope_paint(uint8_t* bmp_buf, uint32_t x_origin, uint32_t y_origin)
{
	//int r = vectorscope_height/2 - 1;
	int xc = x_origin + (vectorscope_width >> 1); // xc:128
	int yc = y_origin + (vectorscope_height >> 1);// yc:128

	/* red block at U=-14.7% V=61.5% => U=-304/2048th V=1259/2048th */
	vectorscope_putblock(bmp_buf, xc, yc, 8, -302, 1259);
	/* green block */
	vectorscope_putblock(bmp_buf, xc, yc, 7, -593, -1055);
	/* blue block */
	vectorscope_putblock(bmp_buf, xc, yc, 9, 895, -204);
	/* cyan block */
	vectorscope_putblock(bmp_buf, xc, yc, 5, 301, -1259);
	/* magenta block */
	vectorscope_putblock(bmp_buf, xc, yc, 14, 592, 1055);
	/* yellow block */
	vectorscope_putblock(bmp_buf, xc, yc, 15, -893, 204);
}

int main()
{
	vectorscope_init();
	vectorscope_clear();

	vectorscope_paint(vectorscope, 0, 0);

	//for (int i = 0; i < VECTORSCOPE_WIDTH_MAX * VECTORSCOPE_HEIGHT_MAX; i++)
	//{
	//	cout << vectorscope[i] << endl;
	//}

	return 0;
}