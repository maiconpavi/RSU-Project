#pragma once
#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#define precision 2.0f
#define number_of_points 25000*(int)precision
#define deltat 0.01f / precision

typedef struct Point3D point_t;
void LoadMaxMin();
std::array<point_t, number_of_points> computeLorenz(float alpha = 10.0f, float beta = 8.0f / 3.0f, float rho = 28.0f, float proporcion = 1.5f);

#endif