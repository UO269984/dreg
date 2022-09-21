#include "TestVector3.h"

#define CHECK_VECTOR_EQ(v1, v2, areEqual) \
	CuAssertTrue((v1 == v2) == areEqual);\
	CuAssertTrue((v1 != v2) != areEqual);

void TestVector3::compare() {
	Vector3 zeroOne3 = {0, 1, 0};
	
	CHECK_VECTOR_EQ(zero1, zero2, true)
	CHECK_VECTOR_EQ(zeroOne1, zeroOne2, true)
	CHECK_VECTOR_EQ(v1a, v1b, true)
	CHECK_VECTOR_EQ(v1a, v1a, true)
	
	CHECK_VECTOR_EQ(zero1, zeroOne1, false)
	CHECK_VECTOR_EQ(zero1, v1a, false)
	CHECK_VECTOR_EQ(zeroOne1, zeroOne3, false)
	CHECK_VECTOR_EQ(v1a, v2, false)
}

void TestVector3::sum() {
	CuAssertTrue(zero1 + zero2 == zero1);
	CuAssertTrue(v1a + zero1 == v1a);
	CuAssertTrue(v1a + zeroOne1 == (Vector3{1, 2, 4}));
	CuAssertTrue(v1a + v2 == (Vector3{4, 4, 6}));
}

void TestVector3::subtraction() {
	CuAssertTrue(zero1 - zero2 == zero1);
	CuAssertTrue(v1a - zero1 == v1a);
	CuAssertTrue(zero1 - v1a == (Vector3{-1, -2, -3}));
	CuAssertTrue(v1a - zeroOne1 == (Vector3{1, 2, 2}));
	CuAssertTrue(v1a - v2 == (Vector3{-2, 0, 0}));
}

void TestVector3::mult() {
	CuAssertTrue(zero1 * 0 == zero1);
	CuAssertTrue(v1a * 0 == zero1);
	CuAssertTrue(v1a * 1 == v1a);
	CuAssertTrue(v1a * 2.5 == (Vector3{2.5, 5, 7.5}));
}

void TestVector3::increment() {
	Vector3 vec = zero1;
	vec += zero1;
	CuAssertTrue(vec == zero1);
	
	vec = v1a;
	vec += zero1;
	CuAssertTrue(vec == v1a);
	
	vec = v1a;
	vec += zeroOne1;
	CuAssertTrue(vec == (Vector3{1, 2, 4}));
	
	vec = v1a;
	vec += v2;
	CuAssertTrue(vec == (Vector3{4, 4, 6}));
}

void TestVector3::length() {
	CuAssertTrue(zero1.length() == 0);
	CuAssertTrue(zeroOne1.length() == 1);
	CuAssertDblEquals(0.502493781, (Vector3{0.45, 0.1, 0.2}.length()), 0.000001);
	CuAssertDblEquals(3.741657387, v1a.length(), 0.000001);
}