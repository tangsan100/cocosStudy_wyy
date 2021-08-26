#pragma once
#include "Common.h"
#include <string>

using namespace GT;
class Image
{
private:
	int width;
	int height;

	RGBA* data;
	bool alphaBlend;
	float alpha;

public:
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	void setAlphaBlend(bool isBlend) { alphaBlend = isBlend; }
	bool getAlphaBlend() { return alphaBlend; }
	void setAlpha(float alpha) { this->alpha = alpha; }
	float getAlpha() { return this->alpha; }
public:
	Image(int width = 0, int height = 0, byte* buffer = nullptr);
	~Image();
	RGBA getColor(int x, int y) const; 
	RGBA getColorByUV(float x, float y) const;

public:
	static Image* loadImage(std::string filename);

	// 图片缩放
	static Image* zoomImageSimple(Image* img, float zoomX, float zoomY);

	// 双线性插值算法
	static Image* zoomImage(Image* img, float zoomX, float zoomY);
};

