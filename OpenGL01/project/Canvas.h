#pragma once
#include <vector>
#include "Common.h"

class Image;

namespace GT {

	enum Data_Type
	{
		GT_FLOAT=0,
		GT_INT = 1
	};

	enum DRAW_MODE
	{
		GT_LINE = 1,
		GT_TRIANGLE = 2
	};

	struct DataElement
	{
		int			size; //�ߴ�
		int			stride; // ����
		Data_Type	type;
		byte		*data;
		DataElement() {
			size = -1;
			stride = -1;
			type = GT_FLOAT;
			data = nullptr;
		}


	};
	struct StateMent
	{
		const Image* texture;
		bool isBindTexture;

		DataElement vertexData; // ��������
		DataElement colorData;	// ��ɫ����
		DataElement texCoorData;// ��������

		StateMent() {
			texture = nullptr;
			isBindTexture = false;
		}
	};
	

	class Canvas
	{
	private:
		int wWidth;
		int wHeight;
		RGBA* buffer;

		StateMent statement;
		/*bool isBindTexture;
		const Image* texture;*/

	public:

		int getWidth() { return wWidth; }
		int getHeight() { return wHeight; }

		Canvas(int width, int height, void* buffer);

		void drawPoint(int x,int y, RGBA rgb);

		void clear();

		void brensenhamLine(Point p1, Point p2);

		RGBA getPointColor(int x,int y);
		RGBA getScalingColor(RGBA color0, RGBA color1, float scale);

		// ����UVֵ
		floatV2 lerpUV(floatV2 uv1, floatV2 uv2, float scale);

		// ��������
		void drawTriangle(Point pt1, Point pt2, Point pt3);

		// ��ƽ��������
		void drawTriangleFlat(Point flat1, Point flat2, Point pt);

		// ͨ�õ�����������
		void drawTriangleCommon(Point pt1, Point pt2, Point pt3);

		// �Ƿ��ھ���������
		bool isInRect(Point pt, Rect rect);

		// �Ƿ����������ڲ�
		bool isInTriangle(Point pt, std::vector<Point> pts);

		//����Ļ�����һ��ͼƬ
		void drawImage(int x, int y, Image* img);

		// ������
		void bindTexture(const Image* img);

		// ״̬���룺��������
		void gtVertexPointer(int size, Data_Type type, int stride, byte* data);
		// ״̬����: ��ɫ����
		void gtColorPointer(int size, Data_Type type, int stride, byte* data);
		// ״̬���룺��������
		void gtTexCoorPointer(int size, Data_Type type, int stride, byte* data);
		// ��ʼ����
		void gtDrawArray(DRAW_MODE mode,int first,int count);
	};
}



