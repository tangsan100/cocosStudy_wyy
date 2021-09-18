#pragma once
#include "Base.h"
#include <string>
class AImage
{
private:
	int width;
	int height;

	RGBA* data;

public:
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	RGBA* getData() const { return data; }

	
public:
	AImage(int width = 0, int height = 0, byte* buffer = nullptr);
	~AImage();
	RGBA getColor(int x, int y) const { return data[width*y + x]; }

public:
	static AImage* loadImage(std::string filename, bool reverse = true);

};

