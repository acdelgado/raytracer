
// Copyright (C) 2018 Ian Dunn
// For conditions of distribution and use, see the LICENSE file


#pragma once

#include <glm/glm.hpp>


class Light
{

public:

	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 genPosition(int radius)
	{
		const glm::vec3 n = position;
		glm::vec3 w;
		if(n.x == 0)
			w = glm::cross(n, glm::vec3(1,0,0));
		else
			w = glm::cross(n, glm::vec3(0,0,1));
		float theta = rand() / (float) (RAND_MAX) *3.14159;

		glm::vec3 k = glm::normalize(n);
		w = doScale(w, cos(theta)) + doScale(glm::cross(k, w), sin(theta));

		if(radius == 0)
		{
			w = doScale(w, radius / sqrt(glm::dot(w,w)));
		}
		else{
			float r = (rand() / (float) (RAND_MAX)) * radius;
			w = doScale(w, r / sqrt(glm::dot(w,w)));
		}

		w += doScale(n, 1 / glm::dot(n,n));

		return w + position;
	}

protected:
	glm::vec3 doScale(glm::vec3 u, float s)
	{
		u.x *= s;
		u.y *= s;
		u.z *= s;
		return u;
	}
};
