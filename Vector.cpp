#include "Vector.h"
#include <iostream>


Vector::Vector(float _x, float _y){
	x = _x;
	y = _y;
}


Vector::~Vector(){
}

void Vector::changeCoords(float _x, float _y){
	x = _x;
	y = _y;
}

void Vector::rotate(float tetha){
	float aux = x;
	x = x * cos(tetha) - y * sin(tetha);
	y = aux * sin(tetha) + y * cos(tetha);
}

void Vector::nullify(){
	x = -x;
	y = -y;
}

void Vector::reset(){
	x = 300;
	y = 300;
}

void Vector::operator=(Vector v){
	x = v.x;
	y = v.y;
}

void Vector::showVector(){
	std::cout << x << " " << y << std::endl;
}

Vector Vector::operator+(Vector v){
	return Vector(x + v.x, y + v.y);
}

Vector Vector::operator-(Vector v){
	return Vector(x - v.x, y - v.y);
}

Vector Vector::operator/(int a){
	return Vector(x / a, y / a);
}