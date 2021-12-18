#include <gtest/gtest.h>
#include "../src/Vector.h"

using namespace bm;

float const precission = 1e-3f, precission_div_2 = precission / 2.0f;

TEST(VectorTest, DataAccessTest) {
	int const Len = 3;
	float init_array[] = { 1.1f, 2.2f, 3.3f };
	Vector<Len, float> vec3f(init_array);

	for (int i = 0; i < Len; ++i) {
		EXPECT_EQ(vec3f[i], init_array[i]);
		EXPECT_EQ(vec3f.at(i), init_array[i]);
	}
}

TEST(VectorTest, DotProductTest) {
	int const Len = 3;
	float init_array1[] = { 1.1f, 2.2f, 3.3f };
	float init_array2[] = { 3.3f, 4.4f, 5.5f };
	Vector<Len, float> vec3f1(init_array1), vec3f2(init_array2);
	auto get_dot = [](float(&arr1)[3], float(&arr2)[3]) {
		float res = 0;
		for (int i = 0; i < 3; ++i) {
			res += arr1[i] * arr2[i];
		}
		return res;
	};

	EXPECT_NEAR(vec3f1.dot(vec3f2), get_dot(init_array1, init_array2), precission);
}

TEST(VectorTest, EqualityTest) {
	int const Len = 3;
	float init_array[] = { 1.1f, 2.2f, 3.3f };
	float distorted_array[Len];

	for (int i = 0; i < Len; ++i) {
		float const offset = static_cast<float>(rand()) / RAND_MAX * precission - precission_div_2;
		distorted_array[i] = init_array[i] + offset;
	}

	Vector<Len, float> vec3f(init_array), distorted_vec3f(distorted_array);

	EXPECT_TRUE(equals(vec3f, distorted_vec3f, precission));

	distorted_vec3f[Len - 1] = vec3f[Len - 1] + precission * 10;


	EXPECT_FALSE(equals(vec3f, distorted_vec3f, precission));
}

TEST(VectorTest, ArithmeticOperationsTests) {
	int const Len = 3;
	float init_array[] = { 1.1f, 2.2f, 3.3f };
	float changing_array[] = { 3.3f, 4.4f, 5.5f };
	float const scale = 12.12f;

	Vector<Len, float> vec3f(init_array), changing_vec3f(changing_array);

	// ====================== vector addition ======================
	Vector<Len, float> vector_addition_res;
	
	for (int i = 0; i < Len; ++i) {
		vector_addition_res[i] = init_array[i] + changing_array[i];
	}

	EXPECT_TRUE(equals(vec3f + changing_vec3f, vector_addition_res, precission));

	// ====================== vector substraction ======================
	Vector<Len, float> vector_substraction_res;

	for (int i = 0; i < Len; ++i) {
		vector_substraction_res[i] = init_array[i] - changing_array[i];
	}

	EXPECT_TRUE(equals(vec3f - changing_vec3f, vector_substraction_res, precission));

	// ====================== vector multiplication ======================
	Vector<Len, float> vector_multiplication_res;

	for (int i = 0; i < Len; ++i) {
		vector_multiplication_res[i] = init_array[i] * changing_array[i];
	}

	EXPECT_TRUE(equals(vec3f * changing_vec3f, vector_multiplication_res, precission));

	// ====================== vector division ======================
	Vector<Len, float> vector_division_res;

	for (int i = 0; i < Len; ++i) {
		vector_division_res[i] = init_array[i] / changing_array[i];
	}

	EXPECT_TRUE(equals(vec3f / changing_vec3f, vector_division_res, precission));

	// ====================== scalar multiplication ======================
	Vector<Len, float> scalar_multiplication_res;

	for (int i = 0; i < Len; ++i) {
		scalar_multiplication_res[i] = init_array[i] * scale;
	}

	EXPECT_TRUE(equals(vec3f * scale, scalar_multiplication_res, precission));

	// ====================== scalar division ======================
	Vector<Len, float> scalar_division_res;

	for (int i = 0; i < Len; ++i) {
		scalar_division_res[i] = init_array[i] / scale;
	}

	EXPECT_TRUE(equals(vec3f / scale, scalar_division_res, precission));
}

TEST(VectorTest, CrossProductTest) {
	int const Len = 3;
	float init_array1[] = { 1.1f, 2.2f, 0.0f };
	float init_array2[] = { 0.0f, 4.4f, 5.5f };
	Vector<Len, float> vec3f1(init_array1), vec3f2(init_array2);
	auto get_cross = [](float(&arr1)[3], float(&arr2)[3]) {
		return Vector<3, float>(
			arr1[1] * arr2[2] - arr1[2] * arr2[1],
			arr1[2] * arr2[0] - arr1[0] * arr2[2],
			arr1[0] * arr2[1] - arr1[1] * arr2[0]
		);
	};

	EXPECT_TRUE(equals(vec3f1.cross(vec3f2), get_cross(init_array1, init_array2), precission));
}