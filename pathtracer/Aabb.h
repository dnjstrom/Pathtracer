#ifndef _chag_Aabb_h
#define _chag_Aabb_h

#include "float3.h"
#include "ray.h"


namespace chag
{

	/**
	* An aabb defined by the min and max extrema.
	*/
	class Aabb
	{
	public:
		float3 min;
		float3 max;

		/**
		*/
		const float3 getCentre() const { return (min + max) * 0.5f; }

		/**
		*/
		const float3 getHalfSize() const { return (max - min) * 0.5f; }

		/**
		*/
		float getVolume() const { float3 d = max - min; return d.x * d.y * d.z; }

		/**
		*/
		float getArea() const { float3 d = max - min; return d.x*d.y*2.0f + d.x*d.z*2.0f + d.z*d.y*2.0f; }

	};



	/**
	*/
	inline Aabb combine(const Aabb &a, const Aabb &b)
	{
		Aabb result = { min(a.min, b.min), max(a.max, b.max) };
		return result;
	}

	/**
	*
	*/
	inline Aabb combine(const Aabb &a, const float3 &pt)
	{
		Aabb result = { min(a.min, pt), max(a.max, pt) };
		return result;
	}
	/**
	* creates an aabb that has min = FLT_MAX and max = -FLT_MAX.
	*/
	Aabb make_inverse_extreme_aabb();


	/**
	*/
	Aabb make_aabb(const float3 &min, const float3 &max);

	/**
	*/
	inline Aabb make_aabb(const float3 &position, const float radius)
	{
		Aabb result = { position - radius, position + radius };
		return result;
	}

	/**
	*/
	Aabb make_aabb(const float3 *positions, const size_t numPositions);

	/**
	*/
	inline bool overlaps(const Aabb &a, const Aabb &b)
	{
		return a.max.x > b.min.x && a.min.x < b.max.x
			&&   a.max.y > b.min.y && a.min.y < b.max.y
			&&   a.max.z > b.min.z && a.min.z < b.max.z;

	}

	/**
	* Intersect with a ray (from pbrt)
	*/

	inline bool intersect(const Aabb &a, const ray &r, float *hitt0 = NULL, float *hitt1 = NULL)
	{
		float t0 = r.mint, t1 = r.maxt;
		for(int i=0; i<3; i++){
			float invRayDir = 1.0f / r.d[i];
			float tNear = (a.min[i] - r.o[i]) * invRayDir; 
			float tFar =  (a.max[i] - r.o[i]) * invRayDir; 
			if(tNear > tFar) { //swap(tNear, tFar); 
				float temp = tNear; 
				tNear = tFar; 
				tFar = temp; 
			}
			t0 = tNear > t0 ? tNear : t0; 
			t1 = tFar < t1 ? tFar : t1; 
			if(t0 > t1) return false; 
		}
		if(hitt0) *hitt0 = t0;
		if(hitt1) *hitt1 = t1;
		return true; 
	}

} // namespace chag

#endif // _chag_Aabb_h
