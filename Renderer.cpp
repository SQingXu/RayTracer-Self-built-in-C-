#define MAINPROGRAM
#include "Renderer.h"


using namespace std;

#define MAINPROGRAM


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

void printShape(GeometricPrimitive* geo){
	Shape* s = geo->shape;
	BRDF ppbrdf;
	LocalGeo lgeo;
	geo->getBRDF(lgeo, &ppbrdf);
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
	std::cout << "Mat ambient property r: " << ppbrdf.kambient.red << " g: " << ppbrdf.kambient.green << " b: " << ppbrdf.kambient.blue << "\n";
} 





Tracer::Tracer(std::vector<GeometricPrimitive*> primitives){
	this->aggPrim = new AggregatePrimitive(primitives);
}

Color Tracer::shading(LocalGeo& geo, BRDF& brdf, Ray& lray, Color& lcol){
	vec3 zero = vec3(0,0,0);
	vec3 light_dir = zero - lray.dir;
	float lDotn = glm::dot(geo.normal,light_dir);
	//std::cout << lDotn << "\n";
	//std::cout << "ldir  x: " << lray.dir[0] << " y: " << lray.dir[1] << " z: " << lray.dir[2] << "\n";

	if(lDotn < 0.0){
		lDotn = 0.0;
	}
    Color lambert = lcol * brdf.kdiffuse * lDotn;
    //std::cout << "lambert r: " << lambert.red << " g: " << lambert.green << " b: " << lambert.blue << "\n";
    vec3 halfvec = glm::normalize(glm::normalize(main_cam.eye) + light_dir);
    float nDoth = glm::dot(glm::normalize(geo.normal),halfvec);
    if(nDoth < 0.0){
		nDoth = 0.0;
	}
    Color phong = lcol * brdf.kspecular * nDoth;
    Color result = lambert + phong;
    return result;
}

void Tracer::trace(Ray& ray, int depth, Color* color){
	Intersection in;
	float thit;
	Ray lray_inTols;
	Color lcolor;
	BRDF pbrdf;
	if(depth > 5){
		color->red = 0;
		color->green = 0;
		color->blue = 0;
		return;
	}
	if(!aggPrim->intersect(ray,&thit,&in)){
		color->red = 0;
		color->green = 0;
		color->blue = 0;
		return;
	}
	//std::cout << "Intersect" << "\n";
	in.primitive->getBRDF(in.localGeo,&pbrdf);
	GeometricPrimitive* geoPrim = dynamic_cast<GeometricPrimitive*>(in.primitive);
	pbrdf.kreflection = 0;
	*color = *color + pbrdf.kambient;
	for(int i = 0; i < numused_light; i++){
		lights[i]->generateLightRay(in.localGeo, &lray_inTols, &lcolor);
		if(!aggPrim->intersectP(lray_inTols)){
			//std::cout << "Light ray counter" << "\n";
			Color shading = Tracer::shading(in.localGeo,pbrdf,lray_inTols,lcolor);
			//std::cout << "shading r: " << shading.red << " g: " << shading.green << " b: " << shading.blue << "\n";
			//std::cout << "brdf.kdiffuse r: " << pbrdf.kdiffuse.red << " g: " << pbrdf.kdiffuse.green << " b: " << pbrdf.kdiffuse.blue << "\n";
			*color = *color + shading;
		} 
	}
	if(pbrdf.kreflection > 0){
		vec3 reflect_dir = glm::reflect(ray.dir, in.localGeo.normal);
		vec3 reflect_pos = in.localGeo.pos;
		Ray reflectRay;
		reflectRay.pos = reflect_pos;
		reflectRay.dir = reflect_dir;
		Color* reflectColor = new Color();
		trace(reflectRay, depth+1, reflectColor);
		*color = *color + *reflectColor;
	}
}

Film::Film(int x, int y){
	this->x = x;
	this->y = y;
	colorArray = new Color[x*y];
	Color def = Color();
	//initialize all color with black
	for(int i = 0; i < y; i++){
		for(int j = 0; j < x; j++){
			colorArray[j + i * x] = def;
		}
	}
}

void Film::commit(Sample& sample, Color& color){
	// int x_pos = (int)((sample.x)/100.0f * x);
	// int y_pos = (int)((sample.y)/100.0f * y);
	//covert from float to int have problem, some int will be missed
	int x_pos = sample.xInReal;
	int y_pos = sample.yInReal;
	//std::cout<<"Commit in position x: " << x_pos << " y: " << y_pos << "\n";
	colorArray[y_pos * x + x_pos] = color;
}

void Film::writeImage(){
	FILE *fp = fopen("result2.ppm","wb");
	(void)fprintf(fp, "P6\n%d %d\n255\n",x,y);
	for(int i = 0; i < y; i++){
		for(int j = 0; j < x; j++){
			static unsigned char color[3];
			Color read = colorArray[i*x + j];
			//std::cout << "Write Color test red: " << read.red << " green: " << read.green << " blue: " << read.blue << "\n";
			color[0] = (unsigned char)(int)(read.red * 255);
			color[1] = (unsigned char)(int)(read.green * 255);
			color[2] = (unsigned char)(int)(read.blue * 255);
			
			(void)fwrite(color, 1, 3, fp);
		}
	}
	(void) fclose(fp);
	return;
}

int main(){
	//Film film = Film(640, 480);
	//film.writeImage();

	readfile("scene2.txt");
	std::cout << sizeX << "\n" << sizeY << "\n";
	Sampler sampler = Sampler(sizeX, sizeY);
	Tracer tracer = Tracer(geoPrimitives);
	Film film = Film(sizeX, sizeY);
	Sample sam;
	Ray ray;
	Color color;
	while(sampler.getSample(&sam)){
		color = Color();
		main_cam.GenerateRay(sam, &ray);
		tracer.trace(ray, 0, &color);
		// if(color.red >= 1.0f && color.green >= 1.0f && color.blue >= 1.0f){
		// 	std::cout << "Color test red: " << color.red << " green: " << color.green << " blue: " << color.blue << "\n";
		// }
		film.commit(sam, color);
	}
	film.writeImage();

	// for(int i = 0; i < geoPrimitives.size(); i++){
	// 	printShape(geoPrimitives[i]);
	// }
	return 0;
}

