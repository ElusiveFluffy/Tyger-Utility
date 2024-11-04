#pragma once

struct PtrVector3 {
	float* X, * Y, * Z;
};
struct Vector3 {
	float X, Y, Z;

	Vector3 operator+(Vector3 const rhs) const {
		return { X + rhs.X,
				 Y + rhs.Y,
				 Z + rhs.Z };
	};
	Vector3 operator-(Vector3 const rhs) const {
		return { X - rhs.X,
				 Y - rhs.Y,
				 Z - rhs.Z };
	};
};