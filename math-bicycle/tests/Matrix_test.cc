
#include <gtest/gtest.h>
#include "../src/Matrix.h"

using namespace bm;

float const precission = 1e-3f, precission_div_2 = precission / 2.0f;

TEST(MatrixTest, DataAccessTest) {
	int const Rows = 3;
	int const Cols = 3;
	float init_array[Rows * Cols] = {
		1.1f, 2.2f, 3.3f,
		4.4f, 5.5f, 6.6f,
		7.7f, 8.8f, 9.9f
	};
	Matrix<Rows, Cols, float> mat3f(init_array);
	for (int i = 0; i < Rows; ++i) {
		for (int j = 0; j < Cols; ++j) {
			int const array_index = i* Cols + j;
			EXPECT_EQ(mat3f[i][j],		 init_array[array_index]);
			EXPECT_EQ(mat3f.at(i).at(j), init_array[array_index]);
			EXPECT_EQ(mat3f.at(i, j),	 init_array[array_index]);
		}
	}
}

TEST(MatrixTest, TransposeOperationTest) {
	int const Rows = 4;
	int const Cols = 2;
	float init_array[Rows * Cols] = {
		1.1f, 2.2f,
		3.3f, 4.4f,
		5.5f, 6.6f,
		7.7f, 8.8f
	};
	Matrix<Rows, Cols, float> mat4_2f(init_array);
	Matrix<Cols, Rows, float> mat2_4f = mat4_2f.trans();
	for (int i = 0; i < Rows; ++i) {
		for (int j = 0; j < Cols; ++j) {
			int const array_index = i * Cols + j;
			EXPECT_EQ(mat4_2f.at(i, j), mat2_4f.at(j, i));
		}
	}
}

TEST(MatrixTest, EqualityTest) {
	int const Rows = 3;
	int const Cols = 3;
	float init_array[Rows * Cols] = {
		1.1f, 2.2f, 3.3f,
		4.4f, 5.5f, 6.6f,
		7.7f, 8.8f, 9.9f
	};
	float distorted_array[Rows * Cols];

	for (int i = 0; i < Rows; ++i) {
		for (int j = 0; j < Cols; ++j) {
			int const array_index = i * Cols + j;
			float const offset = static_cast<float>(rand()) / RAND_MAX * precission - precission_div_2;
			distorted_array[array_index] = init_array[array_index] + offset;
		}
	}

	Matrix<Rows, Cols, float> mat3f(init_array), distorted_mat3f(distorted_array);

	EXPECT_TRUE(equals(mat3f, distorted_mat3f, precission));

	distorted_mat3f.at(Rows - 1, Cols - 1) = mat3f.at(Rows - 1, Cols - 1) + precission * 10;

	EXPECT_FALSE(equals(mat3f, distorted_mat3f, precission));
}

TEST(MatrixTest, ArithmeticOperationsTest) {
	int const Rows = 3;
	int const Cols = 3;
	float init_array[Rows * Cols] = {
		1.1f, 2.2f, 3.3f,
		4.4f, 5.5f, 6.6f,
		7.7f, 8.8f, 9.9f
	};
	float changing_array[Rows * Cols] = {
		9.9f, 8.8f, 7.7f,
		6.6f, 5.5f, 4.4f,
		3.3f, 2.2f, 1.1f
	};
	float scale = 17.17f;

	Matrix<Rows, Cols, float> mat3f(init_array), changing_mat3f(changing_array);

	// ================= matrix addition ===================
	Matrix<Rows, Cols, float> addition_res;// , substruction_res, mul_scale_res, div_scale_res;

	for (int i = 0; i < Rows; ++i) {
		for (int j = 0; j < Cols; ++j) {
			int const  array_index = i * Cols + j;
			addition_res.at(i, j) = init_array[array_index] + changing_array[array_index];
		}
	}

	EXPECT_TRUE(equals(mat3f + changing_mat3f, addition_res, precission));

	// ================= matrix substraction ===================
	Matrix<Rows, Cols, float> substruction_res;// , mul_scale_res, div_scale_res;

	for (int i = 0; i < Rows; ++i) {
		for (int j = 0; j < Cols; ++j) {
			int const  array_index = i * Cols + j;
			substruction_res.at(i, j) = init_array[array_index] - changing_array[array_index];
		}
	}

	EXPECT_TRUE(equals(mat3f - changing_mat3f, substruction_res, precission));

	// ================= scalar multiplication ===================
	Matrix<Rows, Cols, float> mul_scale_res;// , div_scale_res;

	for (int i = 0; i < Rows; ++i) {
		for (int j = 0; j < Cols; ++j) {
			int const  array_index = i * Cols + j;
			mul_scale_res.at(i, j) = init_array[array_index] * scale;
		}
	}

	EXPECT_TRUE(equals(mat3f * scale, mul_scale_res, precission));

	// ================= scalar division ===================
	Matrix<Rows, Cols, float> div_scale_res;

	for (int i = 0; i < Rows; ++i) {
		for (int j = 0; j < Cols; ++j) {
			int const  array_index = i * Cols + j;
			div_scale_res.at(i, j) = init_array[array_index] / scale;
		}
	}

	EXPECT_TRUE(equals(mat3f / scale, div_scale_res, precission));
}

TEST(MatrixTest, InversionTest) {
	int const Dim = 3;
	float init_array[Dim * Dim] = {
		1.1f, 7.7f,   14.14f,
		4.4f, 22.22f, 6.6f,
		7.7f, 12.12f, 9.9f
	};

	Matrix<Dim, Dim, float> mat3f(init_array), identity_mat3f;
	Matrix<Dim, Dim, float> inverse_mat3f = mat3f.inv();

	EXPECT_TRUE(equals(mat3f * inverse_mat3f, identity_mat3f, precission));
	EXPECT_TRUE(equals(inverse_mat3f * mat3f, identity_mat3f, precission));
}

TEST(MatrixTest, DeterminantCalculationTest) {
	int const Dim = 3;
	float init_array1[Dim * Dim] = {
		1.1f, 7.7f,   14.14f,
		4.4f, 22.22f, 6.6f,
		7.7f, 12.12f, 9.9f
	};
	float init_array2[Dim * Dim] = {
		1.1f, 2.2f, 3.3f,
		4.4f, 5.5f, 6.6f,
		7.7f, 8.8f, 9.9f
	};
	auto mat3f_det = [](float(&matrix_array)[9]) {
		return
			matrix_array[0] * matrix_array[4] * matrix_array[8] +
			matrix_array[6] * matrix_array[1] * matrix_array[5] +
			matrix_array[3] * matrix_array[2] * matrix_array[7] -
			matrix_array[2] * matrix_array[4] * matrix_array[6] -
			matrix_array[3] * matrix_array[1] * matrix_array[8] -
			matrix_array[0] * matrix_array[5] * matrix_array[7];
	};
	auto equals = [](float a, float b, float precision) {
		return abs(a) - abs(b) <= precision;
	};

	Matrix<Dim, Dim, float> mat3f(init_array1), zero_det_mat3f(init_array2);

	EXPECT_TRUE(equals(mat3f.det(),			 mat3f_det(init_array1), precission));
	EXPECT_TRUE(equals(zero_det_mat3f.det(), mat3f_det(init_array2), precission));
}
