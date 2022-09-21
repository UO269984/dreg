#include "Graph.h"

#include "dreg.h"

#include <cstdio>

bool Graph::saveInitDataEnabled = false;
size_t Graph::defaultBezierSamples = 25;

Graph::Graph() {
	if (saveInitDataEnabled)
		initData = new GraphInitData();
}

Graph::Graph(const Graph& original) : graphPoints(original.graphPoints) {
	if (original.initData != NULL)
		initData = new GraphInitData {original.initData->type, original.initData->refs};
}

Graph::~Graph() {
	if (initData != NULL)
		delete initData;
}

void Graph::saveInitData(GraphType graphType, const Vector2* refs, size_t refsCount) {
	if (initData != NULL) {	
		initData->type = graphType;
		
		initData->refs.clear();
		initData->refs.resize(refsCount);
		for (size_t i = 0; i < refsCount; i++)
			initData->refs[i] = refs[i];
	}
}

bool Graph::loadLinear(const Vector2* refs, size_t refsCount) {
	if (refsCount == 0) {
		printFunc("Error loading linear graph, references count: 0");
		return false;
	}
	
	saveInitData(GraphType::LINEAR, refs, refsCount);
	lastAccess = {INFINITY, 0};
	
	graphPoints.clear();
	graphPoints.resize(refsCount);
	
	graphPoints[0] = refs[0];
	Vector2* lastPoint = graphPoints.data();
	
	for (size_t i = 1; i < refsCount; i++) {
		if (refs[i].x > lastPoint->x)
			*(++lastPoint) = refs[i];
	}
	graphPoints.resize(lastPoint - graphPoints.data() + 1);
	return true;
}

bool Graph::loadBezier(const Vector2* refs, size_t refsCount) {
	return loadBezier(refs, refsCount, Graph::defaultBezierSamples);
}

bool Graph::loadBezier(const Vector2* refs, size_t refsCount, size_t samplesPerSegment) {
	if (refsCount < 4 || (refsCount - 4) % 3 != 0) {
		char aux[60];
		snprintf(aux, 60, "Error loading bezier graph, references count: %zu", refsCount);
		printFunc(aux);
		
		return false;
	}
	
	saveInitData(GraphType::BEZIER, refs, refsCount);
	lastAccess = {INFINITY, 0};
	
	float sampleInterval = 1 / (float) samplesPerSegment;
	size_t segmentsCount = ((refsCount - 1) / 3);
	
	size_t newGraphPointsCount = samplesPerSegment * segmentsCount;
	graphPoints.clear();
	graphPoints.resize(newGraphPointsCount);
	
	size_t iPoint = 0;
	for (size_t segment = 0; segment < refsCount - 1; segment += 3) {
		
		float t = sampleInterval;
		for (size_t i = 0; i < samplesPerSegment; i++) {
			if (i == 0) {
				if (iPoint == 0 || graphPoints[iPoint].x > graphPoints[iPoint - 1].x)
					graphPoints[iPoint++] = refs[segment]; //if t == 0 the point is P0
			}
			
			else {
				float t_3 = pow(t, 3);
				float t_2 = pow(t, 2);
				
				float p0Mul = (-3 * t + 3 * t_2 - t_3 + 1);
				float p1Mul = (3 * t - 6 * t_2 + 3 * t_3);
				float p2Mul = (3 * t_2 - 3 * t_3);
				
				graphPoints[iPoint].x = refs[segment].x * p0Mul +
					refs[segment + 1].x * p1Mul +
					refs[segment + 2].x * p2Mul +
					refs[segment + 3].x * t_3;
				
				if (graphPoints[iPoint].x > graphPoints[iPoint - 1].x) {
					graphPoints[iPoint++].y = refs[segment].y * p0Mul +
						refs[segment + 1].y * p1Mul +
						refs[segment + 2].y * p2Mul +
						refs[segment + 3].y * t_3;
				}
			}
			t += sampleInterval;
		}
	}
	graphPoints.resize(iPoint);
	return true;
}

const GraphInitData* Graph::getInitData() const {
	return initData;
}

const Vector2* Graph::getPoints(size_t* pointsCount) const {
	*pointsCount = graphPoints.size();
	return graphPoints.data();
}

float Graph::getY(float x) const {
	if (lastAccess.x == x)
		return lastAccess.y;
	
	size_t min = 0;
	size_t max = graphPoints.size() - 1;
	
	if (x < graphPoints[min].x)
		return graphPoints[min].y;
	
	else if (x > graphPoints[max].x)
		return graphPoints[max].y;
	
	while (max - min > 1) {
		size_t half = (max + min) / 2;
		(graphPoints[half].x > x ? max : min) = half;
	}
	
	const Vector2* p1 = graphPoints.data() + min;
	const Vector2* p2 = p1 + 1;
	
	float m = (p2->y - p1->y) / (p2->x - p1->x);
	lastAccess = {x, (x - p1->x) * m + p1->y};
	return lastAccess.y;
}