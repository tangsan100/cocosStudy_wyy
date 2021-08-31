#pragma once
#define MIN(x,y)	(((x) < (y))? (x) : (y))
#define MAX(x,y)	(((x) > (y))? (x) : (y))
#define PI 3.141592654
#define DEG2RAD(i) i/180.00*PI


namespace GT{
	

	


	struct Rect {
		int left;
		int right;
		int bottom;
		int top;
		Rect(int l, int r, int b, int t) :left(l), right(r), bottom(b), top(t) {

		}
	};

	template<typename T>
	struct V2
	{
		T x;
		T y;

		V2(T x, T y) :x(x), y(y) {

		}

		V2() {
			x = -1;
			y = -1;
		}
	};

	typedef V2<float> floatV2;
	typedef V2<int> intV2;
	typedef unsigned char byte;
	typedef unsigned int uint;



	struct RGBA
	{
		byte m_b;
		byte m_g;
		byte m_r;
		byte m_a;

		RGBA(byte r = 255, byte g = 255, byte b = 255, byte a = 255) :m_r(r), m_g(g), m_b(b), m_a(a) {

		}

	};


	struct Point {
		float _x;		// x 坐标
		float _y;		// y 坐标
		float _z;
		RGBA _color; // 颜色值
		floatV2 uv;  // uv 坐标

		Point(float x = 0, float y = 0,float z=0, RGBA color = RGBA(255, 255, 255, 255),floatV2 uv =floatV2(0,0)) :
			_x(x), _y(y),_z(z), _color(color),uv(uv) {

		}
	};
}