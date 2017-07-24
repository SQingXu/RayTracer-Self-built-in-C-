#pragma once
#include "Scene.h"

enum LightType{Point, Directional};
class Light{
public:
	LightType type;
	vec3 lightPos;
	Color lightCol;
	virtual void generateLightRay(LocalGeo&, Ray*, Color*) = 0;
};

class PointLight:public Light{
public:
	PointLight(vec3, Color);
	void generateLightRay(LocalGeo&, Ray*, Color*);
};

class DirectionalLight:public Light{
public:
	DirectionalLight(vec3, Color);
	void generateLightRay(LocalGeo&, Ray*, Color*);
};