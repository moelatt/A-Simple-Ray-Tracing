#include "main.h"
using namespace std;

void phongLight(scene& objectScene, vecRay pos, int number, float r){
	for (int i = 0; i < number; i++) {
		float t = 2 * M_PI / number * i;
		light* lig = new light(pos + vecRay(r * cos(t), r * sin(t), 0), RGB(1, 1, 1) , 1.0 / number);
		objectScene.LightAdd(lig);
	}
}
void Sphere(scene& objectScene, vecRay pos, float r =0.5f, RGB col = RGB(0, 0, 1), bool ref =false) {
	sphereGraph* s = new sphereGraph(r, pos);
	s->natrualColor = col;
	s->reflective = ref;
	objectScene.ObjectAdd(s);
}
void plane(scene& objectScene, vecRay pos, vecRay dir, RGB col =RGB(0, 0.5, 0.5)){
	objectPlane* p = new objectPlane(pos, vecRay(0, 0, 1)^dir);
	p->natrualColor = col;
	p->k_spec = 0;
	objectScene.ObjectAdd(p);
}
void plane1(scene& objectScene, vecRay pos, RGB col = RGB(0, 1, 0),char inte = 'c'){
	switch (inte){ objectPlane* obj;
		case 'c': 
			obj = new objectPlane(pos, vecRay(0, 0, -1));
			obj->natrualColor = col;
			obj->k_spec = 0;
			objectScene.ObjectAdd(obj); break;
		case 'f': 
			obj = new objectPlane(pos, vecRay(0, 0, 1));
			obj->natrualColor = col;
			obj->k_spec = 0;
			objectScene.ObjectAdd(obj); break;
	default:break;
	}
}
void DrawObject(scene& objectScene, float ir, float kr, float transparent) {
	Sphere(objectScene, vecRay(1.5, 0.0, 1.2), 0.5, RGB(0.9, 0.5, 0), false);
	Sphere(objectScene, vecRay(1.2, 0.0, 0.0), 0.5, RGB(0.5, 0.8, 1), false);

	sphereGraph* sph1 = new sphereGraph(0.5, vecRay(1.5, 1.4, 1.2));
	sph1->natrualColor = RGB(1, 0.1, 0.1);
	sph1->reflective = true;
	sph1->I_refl = ir;
	sph1->k_spec = kr;	
	sph1->I_refr = 2;
	sph1->transparency = transparent;
	objectScene.ObjectAdd(sph1);

	sphereGraph* sph2 = new sphereGraph(0.5, vecRay(1.5, -1.4, 1.2));
	sph2->natrualColor = RGB(0.1, 1,0.1);
	sph2->reflective = true;
	sph2->I_refl = ir;
	sph1->I_refr = 2;
	sph2->k_spec = kr;
	sph2->transparency = transparent;
	objectScene.ObjectAdd(sph2);
	
	RGB gray(0.2,0.2,0.2), red(1, 0, 0);
	RGB green(0.3, 0.3, 0.3), blue(0.3, 0.0, 0.5);

	plane(objectScene, vecRay(3, 0, -0.5), vecRay(0, 1, 0), gray * 0.4);
	plane1(objectScene, vecRay(0, 0, +3.5), gray * 0.4, 'c');
	plane1(objectScene, vecRay(0, 0, -0.5), gray * 0.4, 'f');
	phongLight(objectScene, vecRay(-0.8, 0, 3), 1, 0.1);
}