#include "main.h"

typedef std::vector<PhongLighting*> vecObj;
typedef std::vector<light*> vecL;
struct scene {
	vecObj objs; vecL li;
	int objValue,  lightValue;
	void ObjectAdd(PhongLighting* obj){
		objs.push_back(obj);
		this->objValue = this->objValue + 1;
	}
	void LightAdd(light* lig){
		li.push_back(lig);
		this->lightValue = this->lightValue + 1;	
	}
};
//==============================================================//
struct RayTracer {
	int maxDepth;
	RGB RayShadowRefl(scene&, Ray&, int, 
	const bool& isLight = false, 
	Ray dRay 	=Ray(vecRay(0, 0, 0), vecRay(0, 0, 0)), 
	light* lig 	=new light(vecRay(0, 0, 0), RGB(0, 0, 0), 0),
	PhongLighting* oldobj = 0); 
	RayTracer(int depth): maxDepth(depth){}
};
RGB PhongShade(PhongLighting* obj, vecRay& pos, vecRay& dir, vecRay& olddir, light* lig){
	float k_d = obj->k_diff, k_s = obj->k_spec, I_s = lig->intensity;
	vecRay shadeObj = !dir, norm = obj->surface_normal(pos), R = norm * 2 * fmax(0, shadeObj * norm) - shadeObj;
	float diff = k_d * I_s * fmax(0, shadeObj * norm),
	spec = k_s * I_s * pow(fmax(0, -!olddir * R), obj->shininess);
	return (obj->AmbientColor + (obj->altcol(pos) * diff) + (lig->color * spec)) * (1-obj->transparency);
}
RGB RayTracer::RayShadowRefl(scene& objectScene, Ray& ray, int depth, const bool& isLight, Ray dRay, light* lig, PhongLighting* oldobj) {
	RGB color = RGB(0, 0, 0); 
	float t = 0;
	PhongLighting* objp;   

	for (int i = 0; i < objectScene.objValue; i++) {
		if (objectScene.objs[i] == oldobj) continue; 
		float temp = objectScene.objs[i]->intersect(ray); 
		if ((temp < t || t == 0) && temp > 0) {
			t = temp;
			objp = objectScene.objs[i];
		}
	}
	if (isLight == true){
		if (t == 0 || t * ray.direction.norm() > (lig->position - ray.position).norm() 
			|| (oldobj != objp && objp->transparent)) {	
			float k_d = oldobj->k_diff, k_s = oldobj->k_spec, I_s = lig->intensity;
	
			vecRay shadeObj = !ray.direction, 
			norm = oldobj->surface_normal(ray.position),
			R = norm * 2 * (shadeObj * norm) - shadeObj;
	
			float diff = k_d * I_s * fmax(0, shadeObj * norm),
			spec = k_s * I_s * pow(fmax(0, -!dRay.direction * R), oldobj->shininess);
	
			return ( oldobj->AmbientColor + (oldobj->altcol(ray.position) * diff) 
				+ (lig->color * spec) ) * (1-oldobj->transparency) 
				* (objp->transparent ? objp->transparency : 1);
		}
		else {
			return oldobj->AmbientColor;
		}
	}
	if (t > 0 && !isLight) {
		vecRay pos = ray.direction * t + ray.position;
		for (int i = 0; i < objectScene.lightValue; i++) {	
			float eps = 0; 
			Ray lightRay = Ray(pos + objp->surface_normal(pos) * eps, objectScene.li[i]->position - pos);
			color = color + RayShadowRefl(objectScene, lightRay, depth + 1, true, ray, objectScene.li[i], objp);
		}
		if (depth < maxDepth) {
			vecRay norm = objp->surface_normal(pos);
			if (objp->reflective) {
				Ray refRay(pos, ray.direction - norm * (norm * ray.direction) * 2);
				color = color + (RayShadowRefl(objectScene, refRay, depth + 1, isLight, ray, lig, objp)
					 	* objp->I_refl);
			}
			if (objp->transparent) {
				float d = norm * ray.direction, r = objp->refindex,
				k = r * r * (1 - d * d);
				Ray refRay(pos,  ray.direction * r - norm * (r * d + sqrt(1 - k)));
				color = color + (RayShadowRefl(objectScene, refRay, depth + 1, isLight, ray, lig, objp) 
					* objp->I_refr);
			}
		}
	}
	else {
		color += RGB(0.6, 0.6, 1);
	}
	return color;
}
