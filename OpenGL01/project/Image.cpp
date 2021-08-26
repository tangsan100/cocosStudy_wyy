#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

Image::Image(int width, int height, byte* buffer) {
	this->width = width;
	this->height = height;

	if (buffer) {
		data = new RGBA[width*height];

		std::memcpy(data, buffer, width*height * sizeof(RGBA));
	}

	this->alphaBlend = false;
	this->alpha = 1;
}

Image::~Image() {
	if (data)
	{
		delete[] data;
	}
}


RGBA Image::getColor(int x, int y) const
{
	if (x >= width || x < 0|| y >= height || y < 0)
	{
		return RGBA(0, 0, 0);
	}
	return data[y*width + x];
}

RGBA Image::getColorByUV(float u, float v) const {
	int x = u * width;
	int y = v * height;

	x = x % width;
	y = y % height;

	return data[y*width + x];
}

Image* Image::loadImage(std::string filename) {
	int picType = 0;
	int picWidth = 0;
	int picHeight = 0;

	stbi_set_flip_vertically_on_load(true);

	byte* buffer = stbi_load(filename.c_str(), &picWidth, &picHeight, &picType, STBI_rgb_alpha);

	Image* img = nullptr;
	if (buffer)
	{
		for (int i = 0; i < picWidth* picHeight * sizeof(RGBA); i += sizeof(RGBA))
		{
			byte tmp = buffer[i];
			buffer[i] = buffer[i + 2];
			buffer[i + 2] = tmp;
		}

		/*picWidth = picWidth < maxWidth ? picWidth : maxWidth;
		picHeight = picHeight < MaxHeight ? picHeight : MaxHeight;*/

		img = new Image(picWidth, picHeight, buffer);
	}
	stbi_image_free(buffer);

	return img;
}

/*
* 图片的简单缩放
*/
Image* Image::zoomImageSimple(Image* img, float zoomX, float zoomY) {
	int width = img->getWidth()*zoomX;
	int height = img->getHeight()*zoomY;


	byte* data = new byte[width*height * sizeof(RGBA)];

	for (int i =0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			int x = i/zoomX;
			int y = j/zoomY;

			x = x < img->getWidth() ? x : (img->getWidth() - 1);
			y = y < img->getHeight() ? y : (img->getHeight() - 1);

			RGBA color = img->getColor(x, y);
			memcpy(data + (j*width + i) * sizeof(RGBA), &color, sizeof(RGBA));
		}
	}

	Image* image = new Image(width, height, data);

	delete[] data;

	return image;
}

Image* Image::zoomImage(Image* img, float zoomX, float zoomY) {
	int width = img->getWidth()*zoomX;
	int height = img->getHeight()*zoomY;


	byte* data = new byte[width*height * sizeof(RGBA)];


	// 定义四个点坐标，和权重值
	int x1, x2, y1, y2;
	float dx1, dx2, dy1, dy2;


	for (int i = 0; i < width; ++i)
	{

		// x 坐标
		float x = i / zoomX;
		x1 = int(x);
		if (x1 >= img->getWidth() -1)
		{
			x1 = img->getWidth() - 1;
			x2 = x1;
		}
		else
			x2 = x1 + 1;

		dx1 = x - x1;
		dx2 = 1.0 - dx1;
		for (int j = 0; j < height; ++j)
		{
			

			// y 坐标
			float y = j / zoomY;
			y1 = int(y);
			if (y1 >= img->getHeight()-1)
			{
				y1 = img->getHeight() - 1;
				y2 = y1;
			}
			else
				y2 = y1;

			dy1 = y - y1;
			dy2 = 1.0 - dy1;

			RGBA color;
			color.m_r = img->getColor(x1, y1).m_r*dx2*dy2 +
						img->getColor(x2,y1).m_r*dx1*dy2 +
						img->getColor(x1,y2).m_r*dx2*dy1 +
						img->getColor(x2,y2).m_r*dx1*dy1;

			color.m_g = img->getColor(x1, y1).m_g*dx2*dy2 +
						img->getColor(x2, y1).m_g*dx1*dy2 +
						img->getColor(x1, y2).m_g*dx2*dy1 +
						img->getColor(x2, y2).m_g*dx1*dy1;

			color.m_b = img->getColor(x1, y1).m_b*dx2*dy2 +
				img->getColor(x2, y1).m_b*dx1*dy2 +
				img->getColor(x1, y2).m_b*dx2*dy1 +
				img->getColor(x2, y2).m_b*dx1*dy1;

			color.m_a = img->getColor(x1, y1).m_a*dx2*dy2 +
				img->getColor(x2, y1).m_a*dx1*dy2 +
				img->getColor(x1, y2).m_a*dx2*dy1 +
				img->getColor(x2, y2).m_a*dx1*dy1;

			memcpy(data + (j*width + i) * sizeof(RGBA), &color, sizeof(RGBA));
		}
	}

	Image* image = new Image(width, height, data);

	delete[] data;

	return image;
}