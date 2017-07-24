#pragma once

#ifdef MAINPROGRAM
#define EXTERN
#else
#define EXTERN extern
#endif

#include "Scene.h"
#include "Light.h"
#include "Primitive.h"

EXTERN vec3* vectors;
EXTERN int vector_num;
EXTERN Camera main_cam;
EXTERN int sizeX;
EXTERN int sizeY;

//Material
EXTERN BRDF brdf;

//Light
const int max_light = 10;
EXTERN Light* lights[max_light];
EXTERN int numused_light;

//GeoPrimitive
EXTERN std::vector<GeometricPrimitive*> geoPrimitives;

