#pragma once

struct Vector3 {
	float x;
	float y;
	float z;
	
	#ifdef __cplusplus
	Vector3 operator +(const Vector3& vec) const;
	Vector3 operator -(const Vector3& vec) const;
	Vector3 operator *(float mul) const;
	Vector3& operator +=(const Vector3& vector);
	bool operator ==(const Vector3& vector) const;
	bool operator !=(const Vector3& vector) const;
	
	float length() const;
	Vector3 rotateEuler(float x, float y, float z) const;
	Vector3 rotate(const float* rotMatrix) const;
	#endif
};

#ifndef __cplusplus
typedef struct Vector3 Vector3;
#endif