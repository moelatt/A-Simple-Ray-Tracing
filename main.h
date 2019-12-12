#ifndef MAIN_H
#define MAIN_H
#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include  <GL/glut.h>
#endif
#include <cstdio>
#include<stdio.h>
#include <istream>
#include <fstream>
#include <cmath>
#include <string>
#include<string.h>
#include <iostream>
#include <fstream>
#include<sstream>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <ctime>
#include <algorithm>
using namespace std;
#define objNum 4 

int screen_width = 800, screen_height = 700, MainWindow; 
int sizes = screen_width * screen_height, k = 0;
bool incIr = false; float transparent = 0.5, ir = 1, kr = 2.5; char c;
void display();
void displayObject();
void keyBoard(unsigned char key, int x, int y);
void reshape(int width, int height);
void reDisplay();
void modifyValue();

struct vecRay{
	float x, y, z;
	vecRay(const float& a =0, const float& b =0, const float& c =0):x(a), y(b), z(c){}
	float norm() const { return sqrt(*this * *this); } 
	vecRay operator+(const vecRay& op) const{ 
		return vecRay(this->x + op.x, this->y + op.y, this->z + op.z); 
	} 
	vecRay operator-(const vecRay& op) const{ 
		return vecRay(this->x - op.x, this->y - op.y, this->z - op.z);
	} 
	vecRay operator-() const{ 
		return vecRay(-this->x, -this->y, -this->z); 
	}
	vecRay operator!() const{ 
		return *this * (1.0f / norm()); 
	}
	float   operator*(const vecRay& op) const{ 
		return this->x * op.x + this->y * op.y + this->z * op.z; 
	} 
	vecRay operator*(const float& op)	  const{ 
		return vecRay(op * this->x, op * this->y, op * this->z);
	}
	vecRay operator^(const vecRay& op) const{ 
		return vecRay(this->y * op.z - this->z * op.y, this->z * op.x - this->x * op.z, this->x * op.y - this->y * op.x ); 
	}
}; 
struct Ray {
	vecRay position, direction;
	Ray(const vecRay& pos, const vecRay& dir):position(pos), direction(dir) {}
	vecRay point(float t){
		return this->position + this->direction * t;
	}
};
struct Viewing {
	vecRay position, direction, up, right; 
	int pixelwidth, pixelheight;
	float aspectRatio;
	Ray GetRay(const float& x, const float& y){
		return Ray(
		this->position,  
			this->right * (2 * x / this->pixelwidth - 1) * this->aspectRatio 
			+ this->up * (2 * y / this->pixelheight - 1) + this->direction);
	}
	Viewing(): position(vecRay(0, 0, 0)), direction(vecRay(1, 0, 0)), up(vecRay(0, 0, 1)), pixelwidth(100),
	pixelheight(100), aspectRatio(1) {}
	void setDirection(vecRay dir) {
		this->direction = dir;
		this->right = !(up ^ direction);
	}
	void PixelSize(int pixelwidth, int pixelheight) {	
		this->pixelwidth = pixelwidth;
		this->pixelheight = pixelheight;
		this->aspectRatio = (float)this->pixelwidth / (float)this->pixelheight;
	}
};
struct RGB {
	float r, g, b;
	RGB(const float x =0, const float y =0, const float z =0) { this->r = x; this->g = y; this->b = z; }
	float& operator[] (const int n){
		if(n == 0){ return this->r;} 
		else if(n == 1){return this->g;}
		else if(n == 2){return this->b;}
	}
	RGB operator+(RGB n){
		return RGB(this->r + n.r, this->g + n.g, this->b + n.b);
	}
	RGB operator*(float n){
		return RGB(n * this->r, n * this->g, n* this->b);
	}
	RGB operator*(RGB& n){
		return RGB(this->r * n.r, this->g * n.g, this->b * n.b);
	}
	RGB operator!(){
		float s = fmax(fmax(this->r, this->g), this->b);
		return RGB(this->r/s, this->g/s, this->b/s);
	}
	RGB operator+=(RGB n){
		this->r += n.r, this->g += n.g ,this->b += n.b;
		return *this;
	}
	bool operator==(RGB n){
		return (this->r == n.r) && (this->g == n.g) && (this->b == n.b);
	}
	RGB gc(float rg){
		return RGB(pow(this->r, rg), pow(this->g, rg), pow(this->b, rg));
	} 
};
struct PhongLighting {
	bool transparent, reflective;
	float I_refl, I_refr, k_diff, k_spec, shininess, transparency, refindex;
	RGB natrualColor, AmbientColor; vecRay position;
	virtual float intersect(Ray& ray) = 0;
	virtual vecRay surface_normal(vecRay&) = 0;
	virtual RGB altcol(vecRay& pos) { return natrualColor; }
	PhongLighting(vecRay pos): transparent(false), position(pos), reflective(false),
		I_refl(0.9), I_refr(0.9), k_diff(1), k_spec(2.0), shininess(30), transparency(0.1),
		refindex(1.0), natrualColor(RGB(1, 1, 1)), AmbientColor(RGB(0.05, 0.05, 0.05))  {}
};
struct light {
	vecRay position; RGB color; float intensity;
	light(vecRay p, RGB c, float i):position(p), color(c), intensity(i){}
};

struct objectPlane :PhongLighting {
	vecRay n;
	float intersect(Ray& ray){
		float k = ray.direction * n;
		if (k == 0) return 0;
		return fmax(0,((position - ray.position) * n) * (1 / k));
	}
	vecRay surface_normal(vecRay&){
		return n; 
	}
	RGB col(vecRay& pos) { return natrualColor; }
	objectPlane(vecRay pos, vecRay n_) : PhongLighting(pos) {
		n = !n_; 
	}
};
struct sphereGraph :PhongLighting {
	float radius;
	sphereGraph(float rad, vecRay pos) : PhongLighting(pos), radius (rad){}
	sphereGraph(float rad, vecRay pos, RGB nat, RGB amb) : PhongLighting(pos), radius (rad){}
	RGB col(vecRay& pos) { return natrualColor; }
	float intersect(Ray& ray){
		vecRay &first = ray.position, &second = ray.direction, &third = position;
		float v1 = second * second, v2 = 2 * (second * (first - third)),
		c = (first - third) * (first - third) - radius * radius;
		if (v2 * v2 - 4 * v1 * c < 0 || v1 < 0) {
			return 0; 
		}
		float tran1 = (-v2 + sqrt(v2 * v2 - 4 * v1 * c))/(2 * v1),
		tran2 = (-v2 - sqrt(v2 * v2 - 4 * v1 * c))/(2 * v1);
		if (tran1 > tran2) {
			if(tran2 > 0){
				return tran2;
			} 
			else{return tran1;}
		}
		if(tran1 > 0){
			return tran1;
		} 
		else{return tran2;}
	}
	vecRay surface_normal(vecRay& n){
		return !(n - position);
	}
};
#endif