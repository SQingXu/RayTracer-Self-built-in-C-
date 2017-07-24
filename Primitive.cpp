#include "Primitive.h"

bool GeometricPrimitive::intersect(Ray &ray, float* thit, Intersection* in){
	LocalGeo lgeo;
	if(!shape->intersect(ray, thit, &lgeo)){
		return false; 
	}
	in->primitive = this;
	in->localGeo = lgeo;
	return true;
}

bool GeometricPrimitive::intersectP(Ray &ray){
	return shape->intersectP(ray);
}

void GeometricPrimitive::getBRDF(LocalGeo &geo, BRDF* brdf){
	mat->getBRDF(brdf);
	return;
}

AggregatePrimitive::AggregatePrimitive(std::vector<GeometricPrimitive*> list){
	this->primitives = list;
	std::cout << "Size of primitives: " << this->primitives.size() << "\n";
}

bool AggregatePrimitive::intersect(Ray &ray, float* thit, Intersection* in){
	float least_t;
	int location;
	LocalGeo final_localGeo;
	bool hit = false;
	int hit_num = 0;
	for(int i = 0; i < primitives.size(); i++){
		float res_t;
		LocalGeo localGeo;
		GeometricPrimitive* geoprim = primitives[i];
		if(geoprim->shape->intersect(ray, &res_t, &localGeo)){
			hit = true;
			if(res_t < least_t || hit_num == 0){
				least_t = res_t;
				final_localGeo = localGeo;
				location = i;
			}
			hit_num++;
		}
		
	}
	//std::cout << "No seg fault here\n";
	if(hit){
		in->primitive = dynamic_cast<Primitive*>(primitives[location]);
		in->localGeo = final_localGeo;
	} 
	return hit;
}

bool AggregatePrimitive::intersectP(Ray &ray){
	bool hit = false;
	for(int i = 0; i < primitives.size(); i++){
		GeometricPrimitive* geoprim = primitives[i];
		if(geoprim->shape->intersectP(ray)){
			hit = true;
			break;
		}
	}
	return hit;
}

void AggregatePrimitive::getBRDF(LocalGeo &geo, BRDF* brdf){
	exit(1);
}




