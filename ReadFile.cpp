#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <deque>
#include <string>
#include <GLUT/glut.h>
#include <iomanip>
#include <limits>

using namespace std;


#define MAINPROGRAM
#include "variable.h"

bool readvalues(stringstream &s, const int numvals, float* values)
{
	for(int i = 0; i < numvals; i++){
		s >> values[i];
		if(s.fail()){
			cout << "Failed Reading values " << i << " while skip/n";
			return false;
		}
	}
	return true;
}

void readfile(const char* filename)
{
 string str, cmd;
 ifstream in;
 int vectorptr = 0;
 in.open(filename);

 if (in.is_open())
 {
 	getline(in,str);
 	while(in){
 		if((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')){
 			stringstream s(str);
 			s >> cmd;
 			int i;
 			bool validinput;
 			float values[10];

 			if (cmd == "maxverts"){
 				validinput = readvalues(s,1,values);
 				if(validinput){
 					vector_num = (int)values[0];
 					vectors = new vec3[vector_num];
 				}
 			}

 			else if(cmd == "vertex"){
 				validinput = readvalues(s,3,values);
 				if(validinput){
 					for(int i = 0; i < 3; i++){
 						vectors[vectorptr][i] = values[i];
 					}
 					vectorptr++;
 				}
 			}
 			else if(cmd == "camera"){
 				validinput = readvalues(s,10,values);
 				if(validinput){
 					for(int i = 0; i < 10; i++){
 						vec3 eye;
 						vec3 center;
 						vec3 up; 
 						float fov;
 						if(i < 3){
 							main_cam.eye[i] = values[i];
 						}else if(i < 6){
 							main_cam.center[i - 3] = values[i];
 						}else if(i < 9){
 							main_cam.up[i - 6] = values[i];
 						}else{
 							main_cam.fov = values[i];
 						}
 					}
 				}
 			}else if(cmd == "ambient"){
 				validinput = readvalues(s,3,values);
 				if(validinput){
 					brdf.kambient = Color(values);
 				}
 			}else if(cmd == "diffuse"){
 				validinput = readvalues(s,3,values);
 				if(validinput){
 					brdf.kdiffuse = Color(values);
 				}
 			}else if(cmd == "specular"){
 				validinput = readvalues(s,3,values);
 				if(validinput){
 					brdf.kspecular = Color(values); 
 				}
 			}else if(cmd == "point"){
 				validinput = readvalues(s,6,values);
 				if(validinput){
 					if(numused_light == max_light){
 						std::cout << "Max light reached\n" ;
 						continue;
 					}
 					vec3 pos;
 					float col_v[3];
 					for(int i = 0; i < 6; i ++){
 						if(i < 3){
 							//position of light
 							pos[i] = values[i];  
 						}else if(i < 6){
 							//color of light
 							col_v[i-3] = values[i];
 						}
 					}
 					Color col = Color(col_v);
 					PointLight* light = new PointLight(pos, col);
 					lights[numused_light] = light;
 					numused_light++;
 				}
 			}else if(cmd == "directional"){
 				validinput = readvalues(s,6,values);
 				if(validinput){
 					if(numused_light == max_light){
 						std::cout << "Max light reached\n" ;
 						continue;
 					}
 					vec3 pos;
 					float col_v[3];
 					for(int i = 0; i < 6; i ++){
 						if(i < 3){
 							//position of light
 							pos[i] = values[i];  
 						}else if(i < 6){
 							//color of light
 							col_v[i-3] = values[i];
 						}
 					}
 					Color col = Color(col_v);
 					DirectionalLight* light = new DirectionalLight(pos, col);
 					lights[numused_light] = light;
 					numused_light++;
 				}
 			}else if(cmd == "size"){
 				validinput = readvalues(s,2,values);
 				if(validinput){
 					sizeX = (int)values[0];
 					sizeY = (int)values[1];
 				}
 			}else if(cmd == "tri"){
 				validinput = readvalues(s,3,values);
 				if(validinput){
 					
 					vec3 a = vectors[(int)values[0]];
 					vec3 b = vectors[(int)values[1]]; 
 					vec3 c = vectors[(int)values[2]];
 					Triangle* tri = new Triangle(a,b,c);
 					Material* mat = new Material(brdf);
 					GeometricPrimitive* geoPrim = new GeometricPrimitive();
 					geoPrim->shape = dynamic_cast<Shape*>(tri);
 					geoPrim->mat = mat;
 					geoPrimitives.push_back(geoPrim);
 				}
 			}else if(cmd == "sphere"){
 				validinput = readvalues(s,4,values);
 				if(validinput){
 					vec3 center; 
 					float radius;
 					for(int i = 0; i < 3; i++){
 						center[i] = values[i]; 
 					}
 					radius = values[3];
 					Sphere* sphere = new Sphere(center,radius);
 					Material* mat = new Material(brdf);
 					GeometricPrimitive* geoPrim = new GeometricPrimitive();
 					geoPrim->shape = dynamic_cast<Shape*>(sphere);
 					geoPrim->mat = mat;
 					geoPrimitives.push_back(geoPrim);
 				}
 			}else{
 				std::cerr << "Unknown Command: " << cmd << "Skipping\n";
 			}
 		}
 		getline(in,str);
 	}

 }else{
 	std::cerr << "Unable to open input data file " << filename << "\n"; 
 }
}

void printShape(Shape* s){
	if(s->shape == ShapeType::Sphere){
		Sphere* sp = dynamic_cast<Sphere*>(s);
		Sphere sph = *sp;
		std::cout << "Sphere radius: " << sph.radius << "\n" << "    center x:" 
		<< sph.center[0] << " y: " << sph.center[1] << " z: " << sph.center[2] << "\n";	
	}else if(s->shape == ShapeType::Triangle){
		Triangle* tr = dynamic_cast<Triangle*>(s);
		Triangle tri = *tr;
		std::cout << "Triangle a x: " << tri.a[0] << " y: " << tri.a[1] << " z: " << tri.a[2] << "\n";
		std::cout << "         b x: " << tri.b[0] << " y: " << tri.b[1] << " z: " << tri.b[2] << "\n";
		std::cout << "         c x: " << tri.c[0] << " y: " << tri.c[1] << " z: " << tri.c[2] << "\n"; 
	}
} 

// int main(int argc, char* argv[])
// {
// 	readfile("scene2.txt");
// 	Sampler sam = Sampler(sizeX,sizeY);
// 	Sample* s1 = new Sample();
// 	Sample* s2 = new Sample();
// 	bool valid = sam.getSample(s1);
// 	if(!valid){
// 		std::cerr << "Error in sampling s1\n";
// 	}
// 	valid = sam.getSample(s2);
// 	if(!valid){
// 		std::cerr << "Error in sampling s2\n";
// 	}
// 	Ray* r1 = new Ray();
// 	Ray* r2 = new Ray();
// 	main_cam.GenerateRay(*s1, r1);
// 	main_cam.GenerateRay(*s2, r2);
// 	//std::cout << "s1.x: " << (*s1).x << " s2.x: " << (*s2).x <<"\n";
// 	// std::cout << std::setprecision(15) << std::fixed << "r1. dir x: " << r1->dir[0] << " y: " << r1->dir[1] << " z: "
// 	// << r1->dir[2] << "\n" << "  pos x: " << r1->pos[0] << " y: "<< r1->pos[1] 
// 	// << " z: " << r1->pos[2] << "\n";
// 	// std::cout << std::setprecision(15) << std::fixed << "r2. dir x: " << r2->dir[0] << " y: " << r2->dir[1] << " z: "
// 	// << r2->dir[2] << "\n" << "  pos x: " << r2->pos[0] << " y: "<< r2->pos[1] 
// 	// << " z: " << r2->pos[2] << "\n";
// 	GeometricPrimitive* prime = new GeometricPrimitive(); 
// 	std::cout << "Sample 1 x test" << (int)((s2->x)/100.0f*sizeX) << "\n";



// 	for(int i = 0; i < geoPrimitives.size(); i++){
// 		printShape(geoPrimitives[i]->shape);
// 	}




// 	return 0; 


// }