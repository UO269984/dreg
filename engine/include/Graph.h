#pragma once

#include "Vector2.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

enum class GraphType {
	LINEAR=0, BEZIER
};

struct GraphInitData {
	GraphType type;
	std::vector<Vector2> refs;
};

class Graph {
public:
	Graph();
	Graph(const Graph& original);
	~Graph();
	
	void loadLinear(const Vector2* refs, size_t refsCount);
	void loadBezier(const Vector2* refs, size_t refsCount, size_t samplesPerSegment);
	
	const GraphInitData* getInitData() const;
	const Vector2* getPoints(size_t* pointsCount) const;
	float getY(float x) const;
	
	static bool saveInitDataEnabled;
	static size_t defaultBezierSamples;

private:
	void saveInitData(GraphType graphType, const Vector2* refs, size_t refsCount);
	
	std::vector<Vector2> graphPoints;
	GraphInitData* initData = NULL;
	
	mutable Vector2 lastAccess = {INFINITY, 0};
};