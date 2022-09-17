#include "Vector3.h"

#define _USE_MATH_DEFINES
#include <cmath>

Vector3 Vector3::operator +(const Vector3& vec) const {
	return {x + vec.x, y + vec.y, z + vec.z};
}

Vector3 Vector3::operator -(const Vector3& vec) const {
	return {x - vec.x, y - vec.y, z - vec.z};
}

Vector3 Vector3::operator *(float mul) const {
	return {x * mul, y * mul, z * mul};
}

Vector3& Vector3::operator +=(const Vector3& vec) {
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

bool Vector3::operator ==(const Vector3& vec) const {
	return x == vec.x && y == vec.y && z == vec.z;
}

bool Vector3::operator !=(const Vector3& vec) const {
	return x != vec.x || y != vec.y || z != vec.z;
}

float Vector3::length() const {
	return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::rotateEuler(float x, float y, float z) const {
	float rollSin = sin(x * (M_PI / 180.0f));
	float rollCos = cos(x * (M_PI / 180.0f));
	float pitchSin = sin(y * (M_PI / 180.0f));
	float pitchCos = cos(y * (M_PI / 180.0f));
	float yawSin = sin(z * (M_PI / 180.0f));
	float yawCos = cos(z * (M_PI / 180.0f));
	
	float rotMatrix[9] = {
		yawCos * pitchCos, yawCos * pitchSin * rollSin - yawSin * rollCos, yawCos * pitchSin * rollCos + yawSin * rollSin,
		yawSin * pitchCos, yawSin * pitchSin * rollSin + yawCos * rollCos, yawSin * pitchSin * rollCos - yawCos * rollSin,
		-pitchSin, pitchCos * rollSin, pitchCos * rollCos
	};
	return rotate(rotMatrix);
}

Vector3 Vector3::rotate(const float* rotMatrix) const {
	Vector3 rotated;
	rotated.x = rotMatrix[0] * x + rotMatrix[1] * y + rotMatrix[2] * z;
	rotated.y = rotMatrix[3] * x + rotMatrix[4] * y + rotMatrix[5] * z;
	rotated.z = rotMatrix[6] * x + rotMatrix[7] * y + rotMatrix[8] * z;
	
	return rotated;
}