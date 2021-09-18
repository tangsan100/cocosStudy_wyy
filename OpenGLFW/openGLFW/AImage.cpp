#include "AImage.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

AImage::AImage(int width , int height, byte* buffer ) {
	this->width = width;
	this->height = height;

	if (buffer) {
		data = new RGBA[width*height];

		std::memcpy(data, buffer, width*height * sizeof(RGBA));
	}

}


AImage::~AImage() {
	delete [] data;
}


AImage* AImage::loadImage(std::string filename, bool reverse) {
	int picType = 0;
	int picWidth = 0;
	int picHeight = 0;

	stbi_set_flip_vertically_on_load(reverse);

	byte* buffer = stbi_load(filename.c_str(), &picWidth, &picHeight, &picType, STBI_rgb_alpha);

	AImage* img  = new AImage(picWidth, picHeight, buffer);

	//if (buffer)
	//{
	//	for (int i = 0; i < picWidth* picHeight * sizeof(RGBA); i += sizeof(RGBA))
	//	{
	//		byte tmp = buffer[i];
	//		buffer[i] = buffer[i + 2];
	//		buffer[i + 2] = tmp;
	//	}

	//	/*picWidth = picWidth < maxWidth ? picWidth : maxWidth;
	//	picHeight = picHeight < MaxHeight ? picHeight : MaxHeight;*/

	//	img = new AImage(picWidth, picHeight, buffer);
	//}

	
	stbi_image_free(buffer);

	return img;
}