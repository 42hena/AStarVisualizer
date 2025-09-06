#pragma once

struct Weight
{
	float g = 0.0;
	float h = 0.0;
	float f = 0.0;

	// Operator 오버로딩
	bool operator<(const Weight& rhs) const{
		return f < rhs.f;
	}
};