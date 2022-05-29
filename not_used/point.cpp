#include <sstream>
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "solution.hpp"

using namespace std;


extern const unsigned int screenW;
extern const unsigned int screenH;

extern float lorenz_max_x, lorenz_max_y, lorenz_max_z;
extern float lorenz_min_x, lorenz_min_y, lorenz_min_z;

extern const int n_of_p;
std::array<Point3D, number_of_points> lorenzSolution;

extern const float delta_t;

extern glm::vec3 color_c1, color_c2;


static struct Point3D
{
	glm::vec3 Position;
	glm::vec3 Color;

	Point3D()
		: Position(glm::vec3(0.0f, 0.0f, 0.0f))
		, Color(glm::vec3(0.0f, 0.0f, 0.0f))
	{
	}

	Point3D(glm::vec3 position, glm::vec3 color)
		: Position(position)
		, Color(color)
	{
	}
};


void LoadMaxMin(){
	float maximox = lorenzSolution[0].Position.z;
	float maximoy = lorenzSolution[0].Position.z;
	float maximoz = lorenzSolution[0].Position.z;

	float minimox = lorenzSolution[0].Position.z;
	float minimoy = lorenzSolution[0].Position.z;
	float minimoz = lorenzSolution[0].Position.z;
	for (size_t i = 1; i < n_of_p; i++){
		maximox = std::max(maximox, lorenzSolution[i].Position.x);
		maximoy = std::max(maximoy, lorenzSolution[i].Position.y);
		maximoz = std::max(maximoz, lorenzSolution[i].Position.z);

		minimox = std::min(minimox, lorenzSolution[i].Position.x);
		minimoy = std::min(minimoy, lorenzSolution[i].Position.y);
		minimoz = std::min(minimoz, lorenzSolution[i].Position.z);
	}

	lorenz_max_x = maximox;
	lorenz_max_y = maximoy;
	lorenz_max_z = maximoz;

	lorenz_min_x = minimox;
	lorenz_min_y = minimoy;
	lorenz_min_z = minimoz;	
}

//
//	*** Função para resolver o sistema de equações ***
//

std::array<Point3D, number_of_points> computeLorenz(float alpha, float beta, float rho, float proporcion)
{
	// Posição inicial do sistema

	float x = 0.01f;
	float y = 0.0f;
	float z = 0.0f;
	float dx;
	float dy;
	float dz;
	float vr = color_c2[0] - color_c1[0];
	float vg = color_c2[1] - color_c1[1];
	float vb = color_c2[2] - color_c1[2];
	float r, g, b;


	//std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	for (size_t i = 0; i < n_of_p; ++i)
	{
		dx = (alpha * (y - x)) * delta_t;
		dy = (x * (rho - z) - y) * delta_t;
		dz = (x * y - beta * z) * delta_t;

		x += dx;
		y += dy;
		z += dz;

		r = std::min(std::max(color_c1[0] + vr * (float)i / ((float)number_of_points * proporcion), 0.0f), 1.0f);
		g = std::min(std::max(color_c1[1] + vg * (float)i / ((float)number_of_points * proporcion), 0.0f), 1.0f);
		b = std::min(std::max(color_c1[2] + vb * (float)i / ((float)number_of_points * proporcion), 0.0f), 1.0f);
		
		lorenzSolution[i] = Point3D(glm::vec3(x, y, z), glm::vec3(r, g, b));
	}

	return lorenzSolution;
}
