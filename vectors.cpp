
#include <iostream>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

void Constructors()
{
	std::cout << std::endl;

	//Float Vectors
	glm::vec2 PointZero{ 10.0f, 10.0f };
	glm::vec3 PointOne{ 10.0f, 0.0f, 0.0f };
	glm::vec4 PointTwo{ 10.0f, 0.0f, 0.0f, 0.0f };

	//Int Vector
	glm::ivec2 PointThree{ 3, 4 };

	//Boolean Vector
	glm::bvec3 PointFour{ false, true, false };

	//Double Precision Float Vector
	glm::dvec2 PointFive{ 10.0, 10.0 };

	std::cout << "Point: " << glm::to_string(PointZero) << std::endl;
	std::cout << "Point: " << glm::to_string(PointOne) << std::endl;
	std::cout << "Point: " << glm::to_string(PointTwo) << std::endl;
	std::cout << "Point: " << glm::to_string(PointThree) << std::endl;
	std::cout << "Point: " << glm::to_string(PointFour) << std::endl;
	std::cout << "Point: " << glm::to_string(PointFive) << std::endl;
}

void Components()
{
	std::cout << std::endl;

	glm::vec3 P{ 1, 2, 3 };
	std::cout << "X:" << P.x << " Y:" << P.y << " Z:" << P.z << std::endl;
	std::cout << "R:" << P.r << " G:" << P.g << " B:" << P.b << std::endl;
	std::cout << "0:" << P[0] << " 1:" << P[1] << " 2:" << P[2] << std::endl;
}

void Swizzle()
{
	std::cout << std::endl;

	glm::vec4 P{ 1, 2, 3, 4 };

	glm::vec3 Q = P.xxx;
	glm::vec3 R = P.xyx;
	glm::vec4 T = P.xyzw;
	glm::vec4 Y = P.xzzw;

	std::cout << glm::to_string(Y) << std::endl;
}

void Operations()
{
	std::cout << std::endl;

	glm::vec3 P0{ 10.0f, 10.0f, 0.0f };
	glm::vec3 P1{ 10.0f, 10.0f, 10.0f };

	glm::vec3 R;

	R = P0 + P1;
	std::cout << glm::to_string(R) << std::endl;
	
	R = P0 - P1;
	std::cout << glm::to_string(R) << std::endl;

	R = P0 * 5.0f;
	std::cout << glm::to_string(R) << std::endl;

	R = P0 * P1;
	std::cout << glm::to_string(R) << std::endl;

	float L = glm::length(P0);
	std::cout << L << std::endl;

	//Normalizar vetor
	glm::vec3 Norm = glm::normalize(P0);

	//Produto Escalar (Dot Product)
	float Dot = glm::dot(P0, P1);

	//Produto Vetorial (Cross Product)
	glm::vec3 Cross = glm::cross(P0, P1);

	//Distancia
	float Distance = glm::distance(P0, P1);

	//Refração
	glm::vec3 Refract = glm::refract(P0, Norm, 1.0f);

	//Reflexão
	glm::vec3 Reflect = glm::reflect(P0, Norm);
}

int main()
{
	Constructors();

	Components();

	Swizzle();

	Operations();

	return 0;
}