#pragma once
#include "Scene.h"

class Intersection;

class Primitive{
public:
	virtual bool intersect(Ray &ray, float* thit, Intersection* in) = 0;
	virtual bool intersectP(Ray &ray) = 0;
	virtual void getBRDF(LocalGeo &geo, BRDF* brdf) = 0;

};

class Intersection{
public:
	LocalGeo localGeo;
	Primitive* primitive;
};

class GeometricPrimitive:public Primitive{
public:
	Shape* shape;
	Material* mat;
	Transformation objToWorld, worldToObj;
	bool intersect(Ray &ray, float* thit, Intersection* in);
	bool intersectP(Ray &ray);
	void getBRDF(LocalGeo &geo, BRDF* brdf);
};

class AggregatePrimitive:public Primitive{
public:
	std::vector<GeometricPrimitive*> primitives;
	AggregatePrimitive(std::vector<GeometricPrimitive*> list);
	bool intersect(Ray &ray, float* thit, Intersection* in);
	bool intersectP(Ray &ray);
	void getBRDF(LocalGeo &geo, BRDF* brdf);
};


