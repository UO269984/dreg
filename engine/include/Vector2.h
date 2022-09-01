#pragma once

struct Vector2 {
	float x;
	float y;
	
	#ifdef __cplusplus
	Vector2() {}
	Vector2(float x, float y) : x(x), y(y) {}
	#endif
};

#ifndef __cplusplus
typedef struct Vector2 Vector2;
#endif