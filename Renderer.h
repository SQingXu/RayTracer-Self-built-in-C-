#pragma once
#include "variable.h"
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <GLUT/glut.h>
#include <iomanip>
#include <limits>

class Tracer{
public:
	AggregatePrimitive* aggPrim;
	Tracer(std::vector<GeometricPrimitive*>);
	Color shading(LocalGeo&, BRDF&, Ray&, Color&);
	void trace(Ray&, int, Color*);

};

class Film{
public:
	Film(int, int);
	void commit(Sample&, Color&);
	void writeImage();
private:
	int x;
	int y;
	Color* colorArray;
};