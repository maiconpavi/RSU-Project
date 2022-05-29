#pragma once
#ifndef SOLUTION_HPP
#	define SOLUTION_HPP

#	include <array>
#	include <glm/glm.hpp>
#	include <glm/gtc/matrix_transform.hpp>

// #ifndef precision
const float solution_precision = 2.0f;
const int number_of_points = 25000 * (int)solution_precision;
const float deltat = 0.01f / solution_precision;
// #endif

extern unsigned int screenW;
extern unsigned int screenH;

extern float lorenz_max_x, lorenz_max_y, lorenz_max_z;
extern float lorenz_min_x, lorenz_min_y, lorenz_min_z;

extern glm::vec3 color_c1, color_c2;
extern float z_co_temp;

struct Point3D
{
	glm::vec3 Position;
	glm::vec3 Color;

	Point3D()
		: Position(glm::vec3(0.0f, 0.0f, 0.0f))
		, Color(glm::vec3(0.0f, 0.0f, 0.0f))
	{ }

	Point3D(glm::vec3 position, glm::vec3 color)
		: Position(position)
		, Color(color)
	{ }
};

void LoadMaxMin();

std::array<Point3D, number_of_points> computeLorenz(float alpha = 10.0f,
													float beta = 8.0f / 3.0f,
													float rho = 28.0f,
													float proporcion = 1.5f);

#endif
