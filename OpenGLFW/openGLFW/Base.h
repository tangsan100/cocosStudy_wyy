#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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

template<typename T>
struct V3
{
	T x;
	T y;
	T z;

	V3(T x, T y, T z) :x(x), y(y), z(z) {

	}

	V3() {
		x = -1;
		y = -1;
		z = -1;
	}
};

typedef V2<float> floatV2;
typedef V2<int> intV2;
typedef V3<float> fV3;
typedef V3<int> iV3;
typedef unsigned char byte;
typedef unsigned int uint;


struct RGBA
{
	byte r;
	byte g;
	byte b;
	byte a;

	RGBA(byte r = 255, byte g = 255, byte b = 255, byte a = 255) :r(r), g(g), b(b), a(a) {

	}

};