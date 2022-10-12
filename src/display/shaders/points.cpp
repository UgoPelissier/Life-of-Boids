#include "points.hpp"

mat3x3 points::vertex_transform_2d(float width, float height) {
	return mat3x3{ {{{2.f / width, 0.f, 0.f}}, {{0.f, -2.f / height, 0.f}}, {{-1.f, 1.f, 1.f}}} };
}