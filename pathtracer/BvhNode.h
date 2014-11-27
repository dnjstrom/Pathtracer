#include "Aabb.h"
#include "Scene.h"

using namespace std;
using namespace chag;

#pragma once
class BvhNode
{
public:
	BvhNode(vector<Triangle>);
	~BvhNode();
	chag::Aabb bounds;
	BvhNode *left;
	BvhNode *right;
	vector<Triangle> triangles;
	bool compX(Triangle t, Triangle T);
	bool compY(Triangle t, Triangle T);
	bool compZ(Triangle t, Triangle T);
};

