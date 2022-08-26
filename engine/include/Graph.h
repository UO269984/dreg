#pragma once

#include "Vector2.h"

#define _USE_MATH_DEFINES
#include <cmath>

class Graph {
public:
	Graph() {}
	~Graph();
	
	void loadLinear(Vector2* refs, size_t refsCount);
	void loadBezier(Vector2* refs, size_t refsCount, size_t samplesPerSegment);
	
	Vector2* getPoints(size_t* pointsCount) const;
	float getY(float x) const;

private:
	void deletePoints();
	
	Vector2* graphPoints = NULL;
	size_t graphPointsCount = 0;
	
	mutable Vector2 lastAccess = {INFINITY, 0};
};