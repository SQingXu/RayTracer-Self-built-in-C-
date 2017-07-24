#pragma once
#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include <GLUT/glut.h>
#include <memory>
#include <vector>
#include "glm-0.9.2.7/glm/glm.hpp"


enum class ShapeType {Sphere,Triangle};

typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
const float pi = 3.14159265;

class Ray{
public:
    vec3 pos;
    vec3 dir;
    float t_min, t_max;
};

class LocalGeo{
public:
    vec3 pos;
    vec3 normal;
};

class Transformation{
public:
	mat4 m;
	mat4 minvt;
	LocalGeo operator*(const LocalGeo &lgeo){
        LocalGeo localgeo;
        vec4 pos_4 = vec4(lgeo.pos,1.0f);
        vec4 norm_4 = vec4(lgeo.normal,1.0f);
        pos_4 = m * pos_4;
        norm_4 = minvt * norm_4;
        
        vec3 pos_3 = vec3(pos_4);
        vec3 norm_3 = vec3(norm_4);
        
        localgeo.pos = pos_3;
        localgeo.normal = norm_3;
        return localgeo;
    }
    
    Ray operator*(const Ray &ray){
        Ray res;
        vec4 pos_4 = vec4(ray.pos, 1.0f);
        vec4 dir_4 = vec4(ray.dir, 1.0f);
        
        pos_4 = m * pos_4;
        dir_4 = m * dir_4;
        
        vec3 pos_3 = vec3(pos_4);
        vec3 dir_3 = vec3(dir_4);
        
        res.pos = pos_3;
        res.dir = dir_3;
        return res;
    }
};

class Sample{
public:
    float x;
    float y;
    int xInReal;
    int yInReal;
};

class Color{
public:
    float red;
    float green;
    float blue;
    Color();
    Color(float values[3]);
    Color operator+(const Color &color2){
        Color color;
        color.red = this->red + color2.red;
        color.green = this->green + color2.green;
        color.blue = this->blue + color2.blue;
        return color;
    }
    
    Color operator-(const Color &color2){
        Color color;
        color.red = this->red - color2.red;
        color.green = this->green - color2.green;
        color.blue = this->blue - color2.blue;
        return color;
    }
    
    Color operator*(const float scale){
        Color color;
        color.red = this->red * scale;
        color.green = this->green * scale;
        color.blue = this->blue * scale;
        return color;
    }
    
    Color operator/(const float scale){
        Color color;
        color.red = this->red / scale;
        color.green = this->green / scale;
        color.blue = this->blue / scale;
        return color;
    }

    Color operator*(const Color &color2){
        Color color;
        color.red = this->red * color2.red;
        color.green = this->green * color2.green;
        color.blue = this->blue * color2.blue;
        return color;
    }
};

class BRDF{
public:
    Color kdiffuse;
    Color kspecular;
    Color kambient;
    float kreflection;
};

class Material{
public:
	BRDF conBRDF;
	Material(BRDF b);
	void getBRDF(BRDF*);
};

class Sampler{
public:
	int pixelSizex;
	int pixelSizey;
	Sampler(int sizex, int sizey);
	bool getSample(Sample*);
private:
	int x;
	int y;
};

class Camera{
public:
	vec3 eye; 
	vec3 center;
	vec3 up; 
	float fov;
	void GenerateRay(Sample& sample, Ray* ray);
};

class Shape{
public:
	ShapeType shape;
	virtual bool intersect(Ray& ray, float* thit, LocalGeo* geo) = 0;
	virtual bool intersectP(Ray& ray) = 0;
};

class Sphere:public Shape{
public:
	vec3 center;
	float radius;
	Sphere(vec3, float);
	bool intersect(Ray& ray, float* thit, LocalGeo* geo);
	bool intersectP(Ray& ray);
};
class Triangle:public Shape{
public:
	vec3 a;
	vec3 b;
	vec3 c;
	Triangle(vec3 x, vec3 y, vec3 z);
	bool intersect(Ray& ray, float* thit, LocalGeo* geo);
	bool intersectP(Ray& ray);
};

