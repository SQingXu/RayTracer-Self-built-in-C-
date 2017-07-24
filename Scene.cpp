#include "Scene.h"

Color::Color(){
    red = 0;
    green = 0;
    blue = 0;
}
Color::Color(float values[3]){
    red = values[0];
    green = values[1];
    blue = values[2];
}

Material::Material(BRDF b){
    conBRDF = b;
}
void Material::getBRDF(BRDF* brdf){
    *brdf = conBRDF;
    return;
}



Sampler::Sampler(int sizex, int sizey){
    this->pixelSizex = sizex;
    this->pixelSizey = sizey;
    this->x = 0;
    this->y = 0;
}
bool Sampler::getSample(Sample* sample){
    if(x >= pixelSizex || y >= pixelSizey){
        return false;
    }
    sample->x = (float)x/pixelSizex * 100.0f;
    sample->y = (float)y/pixelSizey * 100.0f;
    sample->xInReal = x;
    sample->yInReal = y;
    if(x == pixelSizex-1){
        x = 0;
        y++;
    }else{
        x++;
        //std::cout << "sam.x: " << sample->x << "\n";
    }
    return true;
}

    
void Camera::GenerateRay(Sample& sample, Ray* ray){
    float fovxy = pi*fov/180;
        
    vec3 unscaled_w = eye - center;
    vec3 w = glm::normalize(unscaled_w);
    vec3 unscaled_u = glm::cross(up,w);
    vec3 u = glm::normalize(unscaled_u);
    vec3 v = glm::cross(w,u);
//        std::cout << "W x: " << w[0] << " y: " << w[1] << " z: " << w[2] << "\n";
//        std::cout << "U x: " << u[0] << " y: " << u[1] << " z: " << u[2] << "\n";
//        std::cout << "V x: " << v[0] << " y: " << v[1] << " z: " << v[2] << "\n";
        

    float alpha = tan(fovxy/2) * (sample.x - (100.0/2))/(100.0/2);
//  std::cout << std::setprecision(15) << std::fixed << alpha << "\n" << "sample.x: " << sample.x << "\n";
    float beta = tan(fovxy/2) * ((100.0/2)-sample.y)/(100.0/2);
    ray->pos = eye;
    ray->dir = glm::normalize(alpha * u + beta * v - w);
    return;
}




Sphere::Sphere(vec3 c, float r){
    center = c;
    radius = r;
    shape = ShapeType::Sphere;
}

bool Sphere::intersect(Ray& ray, float* thit, LocalGeo* geo){
        //at^2 + bt + c = 0
    float a = glm::dot(ray.dir, ray.dir);
    float b = 2 * glm::dot(ray.dir, (ray.pos - center));
    float c = glm::dot(ray.pos - center, ray.pos - center) - (radius * radius);
    float mid_res = (b * b - 4 * a * c );
    if(mid_res > 0){
    //two results, take the smaller positive one;
        float res1 = (-b + sqrt(mid_res))/(2*a);
        float res2 = (-b - sqrt(mid_res))/(2*a);
        if(res1 > 0 && res2 > 0){
            if(res1 > res2){
                *thit = res2;
            }else{
                *thit = res1;
            }
        }else if(res1 > 0 || res2 > 0){
            if(res1 > 0){
                *thit = res1;
            }else{
                *thit = res2;
            }
        }else{
            //both result are 0 or negative
            return false; 
        }
        geo -> pos = ray.pos + (*thit) * ray.dir;
        geo -> normal = glm::normalize(geo->pos - this->center);
        //std::cout << "Sphere function intersect\n";
        return true;
        
    }
    else if(mid_res == 0){
        //two equal result
        float res = (-b + sqrt(mid_res))/(2*a);
        if(res <= 0){
            return false;
        }
        *thit = res;
        geo -> pos = ray.pos + (*thit) * ray.dir;
        geo -> normal = glm::normalize(geo->pos - this->center);
        //std::cout << "Sphere function intersect\n";
        return true;
    }
    else{
        //no intersection
        return false;
    }
}

bool Sphere::intersectP(Ray& ray){
    float a = glm::dot(ray.dir, ray.dir);
    float b = 2 * glm::dot(ray.dir, (ray.pos - center));
    float c = glm::dot(ray.pos - center, ray.pos - center) - (radius * radius);
    float mid_res = (b * b - 4 * a * c );
    if(mid_res > 0){
        float res1 = (-b + sqrt(mid_res))/(2*a);
        float res2 = (-b - sqrt(mid_res))/(2*a);
        if(res1 > 0 || res2 > 0){
            return true;
        }else{
            return false;
        }
    }else if(mid_res == 0){
        float res = (-b + sqrt(mid_res))/(2*a);
        if(res > 0){
            return true;
        }
        return false;
    }else{
        return false;
    }
}



Triangle::Triangle(vec3 x, vec3 y, vec3 z){
    a = x;
    b = y;
    c = z;
    shape = ShapeType::Triangle;
}

bool Triangle::intersect(Ray& ray, float* thit, LocalGeo* geo){
    vec3 normal = glm::normalize(glm::cross((c-a),(b-a)));
    if(glm::dot(ray.dir,normal) == 0){
        return false;
    }
    if(glm::dot(ray.dir, normal) < 0){
        normal = vec3(0,0,0) - normal; 
    }
        
    *thit = (glm::dot(a,normal) - glm::dot(ray.pos,normal))/glm::dot(ray.dir,normal);
    if(*thit <= 0){
        //std::cout << "less than zero" << "\n";
        return false;
    }
    geo -> normal = normal;
    geo -> pos = ray.pos + (*thit) * ray.dir;
    
    //determine if the intersection is in the triangle
    vec3 v1 = b-a;
    vec3 v2 = c-a;
    vec3 v3 = geo->pos - a;
    float x1 = v1[0];
    float x2 = v2[0];
    float x3 = v3[0];
    float y1 = v1[1];
    float y2 = v2[1];
    float y3 = v3[1];
    float z1 = v1[2];
    float z2 = v2[2];
    float z3 = v3[2];
    float beta;
    float theta;
    if((y2 * x1 - y1 * x2) == 0){
        if((y2 * z1 - y1 * z2) == 0){
            //use x and z
            beta = (z2 * x3 - x2 * z3)/(z2 * x1 - x2 * z1);
        }else{
            //use y and z
            beta = (z2 * y3 - y2 * z3)/(z2 * y1 - y2 * z1);
        }
    }else{
        //use x and y
        beta = (y2 * x3 - y3 * x2)/(y2 * x1 - y1 * x2);
    }
    if(beta == 0){
        //no other ways work
        return false;
    }

    if(x2 == 0){
        if(y2 == 0){
            //use z2
            theta = (z3 - z1 * beta) / z2;
        }else{
            //use y2
            theta = (y3 - y1 * beta) / y2;
        }
    }else{
        //use x2
        theta = (x3 - x1 * beta) / x2;
    }

    float alpha = 1 - beta - theta;
    if(beta <= 1 && beta >= 0 && theta <= 1 && theta >= 0 && alpha <= 1 && alpha >= 0){
        return true;
    }else{
        //std::cout << "Common: out of range theta: " << x2 << " beta: " << beta << " alpha: " << alpha << " \n";
        return false;
    }
}
    
bool Triangle::intersectP(Ray& ray){
    vec3 normal = glm::normalize(glm::cross((c-a),(b-a)));
    if(glm::dot(ray.dir,normal) == 0){
        return false;
    }

    if(glm::dot(ray.dir, normal) < 0){
        normal = vec3(0,0,0) - normal; 
    }
        
    float thit = (glm::dot(a,normal) - glm::dot(ray.pos,normal))/glm::dot(ray.dir,normal);
    if(thit <= 0){
        return false;
    }
    vec3 intersect = ray.pos + (thit) * ray.dir;
        
    //determine if the intersection is in the triangle
    vec3 v1 = b-a;
    vec3 v2 = c-a;
    vec3 v3 = intersect - a;
    float x1 = v1[0];
    float x2 = v2[0];
    float x3 = v3[0];
    float y1 = v1[1];
    float y2 = v2[1];
    float y3 = v3[1];
    float z1 = v1[2];
    float z2 = v2[2];
    float z3 = v3[2];
    float beta;
    float theta;
    if((y2 * x1 - y1 * x2) == 0){
        if((y2 * z1 - y1 * z2) == 0){
            //use x and z
            beta = (z2 * x3 - x2 * z3)/(z2 * x1 - x2 * z1);
        }else{
            //use y and z
            beta = (z2 * y3 - y2 * z3)/(z2 * y1 - y2 * z1);
        }
    }else{
        //use x and y
        beta = (y2 * x3 - y3 * x2)/(y2 * x1 - y1 * x2);
    }
    if(beta == 0){
        //no other ways work
        return false;
    }

    if(x2 == 0){
        if(y2 == 0){
            //use z2
            theta = (z3 - z1 * beta) / z2;
        }else{
            //use y2
            theta = (y3 - y1 * beta) / y2;
        }
    }else{
        //use x2
        theta = (x3 - x1 * beta) / x2;
    }
    float alpha = 1 - beta - theta;
    if(beta <= 1 && beta >= 0 && theta <= 1 && theta >= 0 && alpha <= 1 && alpha >= 0){
        return true;
    }else{
        return false;
    }
}





