#include "MyVector.hpp"
#include <iostream>

using namespace P6;

MyVector::MyVector() : x(0), y(0), z(0) {

}

MyVector::MyVector(const float _x,
	const float _y,
	const float _z) : x(_x), y(_y), z(_z) {

}

void MyVector::operator += (const MyVector v) {
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
}

void MyVector::operator -=(const MyVector v){
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
}

void MyVector::operator *=(const float v){
	this->x *= v;
	this->y *= v;
	this->z *= v;
}

float MyVector::magnitude(){
	return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}

glm::vec3 MyVector::direction(){
	return ((1 / this->magnitude()) * glm::vec3(this->x,this->y,this->z));
}

glm::vec3 MyVector::componentProduct(const glm::vec3 v){
	return glm::vec3(this->x * v.x,
					this->y * v.y,
					this->z * v.z);
}

float MyVector::scalarProduct(const glm::vec3 v){
	return ((this->x * v.x) + (this->y * v.y) +	(this->z * v.z));
}

glm::vec3 MyVector::vectorProduct(const glm::vec3 v){
	return glm::vec3((this->y * v.z) - (this->z * v.y),
					(this->z * v.x) - (this->x * v.z),
					(this->x * v.y) - (this->y * v.x));
}
