#pragma once

struct Vector3 {
	float x;
	float y;
	float z;
	
	Vector3 operator +(Vector3 vec) const;
	Vector3 operator -(Vector3 vec) const;
	Vector3 operator *(float mul) const;
	Vector3& operator +=(const Vector3& vector);
	
	float length() const;
	Vector3 rotateEuler(float x, float y, float z) const;
	Vector3 rotate(float* rotMatrix) const;
};