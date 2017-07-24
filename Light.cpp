#include "Light.h"

PointLight::PointLight(vec3 pos, Color col){
	this->lightPos = pos;
	this->lightCol = col;
	this->type = LightType::Point;
}

void PointLight::generateLightRay(LocalGeo& geo, Ray* lray, Color* lcol){
	vec3 source = geo.pos;
	vec3 dir = glm::normalize(this->lightPos - source);
	lray -> pos = source;
	lray -> dir = dir;
	*lcol = this->lightCol;
}

DirectionalLight::DirectionalLight(vec3 dir, Color col){
	this->lightPos = dir;
	this->lightCol = col;
	this->type = LightType::Directional;
}

void DirectionalLight::generateLightRay(LocalGeo& geo, Ray* lray, Color* lcol){
	vec3 source = geo.pos;
	vec3 zero = vec3(0,0,0);
	vec3 dir = glm::normalize(zero-(this->lightPos));
	lray -> pos = source;
	lray -> dir = dir;
	*lcol = this->lightCol;
}