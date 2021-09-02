#pragma once
#include <vector>
#include "Common.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
		int			size; //尺寸
		int			stride; // 步长
		Data_Type	type;
		byte		*data;
		DataElement() {
			size = -1;
			stride = -1;
			type = GT_FLOAT;
			data = nullptr;
		}


	};

	enum GT_MATRIX_MODE{
		GT_MATRIX_VIEW = 0,
		GT_MATRIX_PROJECTION,
		GT_MATRIX_MODEL
	};

	struct StateMent
	{
		const Image* texture;
		bool isBindTexture;

		DataElement vertexData; // 顶点数据
		DataElement colorData;	// 颜色数据
		DataElement texCoorData;// 纹理数据


		std::vector<glm::mat4>	matrixVec; // 矩阵栈
		GT_MATRIX_MODE			matrixMode; // 模式
		glm::mat4				modelMatrix; // 模型矩阵
		glm::mat4				viewMatrix;	// v 变换矩阵
		glm::mat4				projectionMatrix;	// p变换矩阵
		StateMent() {
			texture = nullptr;
			isBindTexture = false;
			matrixMode = GT_MATRIX_VIEW;
			viewMatrix = glm::mat4(1.0f);
			modelMatrix = glm::mat4(1.0f);
			projectionMatrix = glm::mat4(1.0f);
		}
	};
	

	class Canvas
	{
	private:
		int wWidth;
		int wHeight;
		RGBA* buffer;
		float* zBuffer;

		StateMent statement;
		/*bool isBindTexture;
		const Image* texture;*/

	public:

		int getWidth() { return wWidth; }
		int getHeight() { return wHeight; }

		Canvas(int width, int height, void* buffer);
		~Canvas();

		void drawPoint(Point pt);

		void clear();

		void brensenhamLine(Point p1, Point p2);

		RGBA getPointColor(int x,int y);
		RGBA getScalingColor(RGBA color0, RGBA color1, float scale);

		// 计算UV值
		floatV2 lerpUV(floatV2 uv1, floatV2 uv2, float scale);
		float zLerp(float z1, float z2, float scale);

		// 画三角形
		void drawTriangle(Point pt1, Point pt2, Point pt3);

		// 画平底三角形
		void drawTriangleFlat(Point flat1, Point flat2, Point pt);

		// 通用的任意三角形
		void drawTriangleCommon(Point pt1, Point pt2, Point pt3);

		// 是否在矩形区域内
		bool isInRect(Point pt, Rect rect);

		// 是否在三角形内部
		bool isInTriangle(Point pt, std::vector<Point> pts);

		//在屏幕上输出一张图片
		void drawImage(int x, int y, Image* img);

		// 绑定纹理
		void bindTexture(const Image* img);

		// 状态传入：顶点数据
		void gtVertexPointer(int size, Data_Type type, int stride, byte* data);
		// 状态传入: 颜色数据
		void gtColorPointer(int size, Data_Type type, int stride, byte* data);
		// 状态传入：纹理数据
		void gtTexCoorPointer(int size, Data_Type type, int stride, byte* data);
		// 开始绘制
		void gtDrawArray(DRAW_MODE mode,int first,int count);

		// 设置变模式 
		void gtMatrixMode(GT_MATRIX_MODE mode);

		// 赋值矩阵
		void gtLoadMatrix(glm::mat4 matrix);
		//	变成单位阵
		void gtLoadIdentity();
		// 左乘一个矩阵
		void gtMultiMatrix(glm::mat4 matrix);

		// 顶点变换
		void gtVertexTransform(Point &pt);

		// 矩阵压栈
		void pushMatrix();

		// 矩阵弹栈
		void popMatrix();
	};
}



