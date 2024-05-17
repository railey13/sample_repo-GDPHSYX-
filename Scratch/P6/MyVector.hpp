#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace P6 {

	class MyVector {
	public:
		float x, y, z;

	public:
		MyVector();
		MyVector(const float _x,
			const float _y,
			const float _z);

	public:
		void operator += (const MyVector v);
		void operator -= (const MyVector v);
		void operator *= (const float v);
	public:
		float magnitude();
		glm::vec3 direction();
		glm::vec3 componentProduct(const glm::vec3 v);
		float scalarProduct(const glm::vec3 v);
		glm::vec3 vectorProduct(const glm::vec3 v);

	public:
		explicit operator glm::vec3() const { return glm::vec3(x, y, z); }
	};
}
