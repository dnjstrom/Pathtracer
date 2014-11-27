#include "BvhNode.h"
#include "Scene.h"

using namespace std;
using namespace chag;


bool BvhNode::compX(Triangle t, Triangle T)
{
	float tmin = min(t.n0.x, min(t.n1.x, t.n2.x));
	float Tmin = min(T.n0.x, min(T.n1.x, T.n2.x));
	return (tmin < Tmin);
}

bool BvhNode::compY(Triangle t, Triangle T)
{
	float tmin = min(t.n0.y, min(t.n1.y, t.n2.y));
	float Tmin = min(T.n0.y, min(T.n1.y, T.n2.y));
	return (tmin < Tmin);
}

bool BvhNode::compZ(Triangle t, Triangle T)
{
	float tmin = min(t.n0.z, min(t.n1.z, t.n2.z));
	float Tmin = min(T.n0.z, min(T.n1.z, T.n2.z));
	return (tmin < Tmin);
}

BvhNode::BvhNode(vector<Triangle> tri)
{
	triangles = tri;

	// Find minimal bounding box
	float x, X, y, Y, z, Z = 0;
	for (unsigned int i = 0; i < triangles.size(); i++)
	{
		Triangle t = triangles[i];
		x = min(t.n0.x, min(t.n1.x, min(t.n2.x, x))); // min x coordinate
		X = max(t.n0.x, max(t.n1.x, max(t.n2.x, X))); // max x coordinate
		y = min(t.n0.y, min(t.n1.y, min(t.n2.y, y))); // min y coordinate
		Y = max(t.n0.y, max(t.n1.y, max(t.n2.y, Y))); // max y coordinate
		z = min(t.n0.z, min(t.n1.z, min(t.n2.z, z))); // min z coordinate
		Z = max(t.n0.z, max(t.n1.z, max(t.n2.z, Z))); // max z coordinate
	}
	bounds.min = make_vector(x, y, z);
	bounds.max = make_vector(X, Y, Z);

	// If this is a leaf node, don't recurse any further.
	if (triangles.size() <= 1)
	{
		return;
	}

	// Find longest axis
	float dx = X - x;
	float dy = Y - y;
	float dz = Z - z;

	if (dz > dx && dz > dy)
	{
		// Split along z axis
		sort(triangles.begin, triangles.end, compZ);
		float cutoff = (z + Z) / 2;
		vector<Triangle> leftTri;
		vector<Triangle> rightTri;
		for (unsigned int i = 0; i < triangles.size(); i++)
		{
			Triangle t = triangles[i];
			float tmin = min(t.n0.z, min(t.n1.z, t.n2.z));
			if (tmin <= cutoff)
			{
				leftTri.push_back(t);
			}
			else
			{
				rightTri.push_back(t);
			}
		}
		left = new BvhNode(leftTri);
		right = new BvhNode(rightTri);
	}
	else if (dy > dx && dy > dz)
	{
		// Split along y axis
		sort(triangles.begin, triangles.end, compY);
		float cutoff = (y + Y) / 2;
		vector<Triangle> leftTri;
		vector<Triangle> rightTri;
		for (unsigned int i = 0; i < triangles.size(); i++)
		{
			Triangle t = triangles[i];
			float tmin = min(t.n0.y, min(t.n1.y, t.n2.y));
			if (tmin <= cutoff)
			{
				leftTri.push_back(t);
			}
			else
			{
				rightTri.push_back(t);
			}
		}
		left = new BvhNode(leftTri);
		right = new BvhNode(rightTri);
	}
	else
	{
		// Split along x axis
		sort(triangles.begin, triangles.end, compX);
		float cutoff = (x + X) / 2;
		vector<Triangle> leftTri;
		vector<Triangle> rightTri;
		for (unsigned int i = 0; i < triangles.size(); i++)
		{
			Triangle t = triangles[i];
			float tmin = min(t.n0.x, min(t.n1.x, t.n2.x));
			if (tmin <= cutoff)
			{
				leftTri.push_back(t);
			}
			else
			{
				rightTri.push_back(t);
			}
		}
		left = new BvhNode(leftTri);
		right = new BvhNode(rightTri);
	}
}


BvhNode::~BvhNode()
{
}
