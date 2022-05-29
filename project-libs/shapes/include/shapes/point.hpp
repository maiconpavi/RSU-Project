#pragma once
#ifndef POINT_HPP
#	define POINT_HPP

#	include <array>
#	include <glm/glm.hpp>
#	include <glm/gtc/matrix_transform.hpp>


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

#endif
