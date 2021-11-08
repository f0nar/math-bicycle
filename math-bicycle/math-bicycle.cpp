#include <iostream>
#include "src/Vector.h"
#include "src/Matrix.h"
#include "src/vector2.h"

#include <cassert>

using std::cout;
using std::endl;

template <int Length, typename T>
std::ostream& operator<<(std::ostream& out, bm::Vector<Length, T> const& vec);

template <int Length, typename T>
std::ostream& operator<<(std::ostream& out, bm::Row<Length, T> const& vec);

template <int Rows, int Cols, typename T>
std::ostream& operator<<(std::ostream& out, bm::Matrix<Rows, Cols, T> const& mat);


#define TEST_BINARY_OPERATOR(OP, Len) \
{ \
	float arr1[Len], arr2[Len], arr3[Len]; \
	for (int i = 0; i < Len; ++i) { \
		arr1[i] = static_cast<float>(rand() % 100); \
		arr2[i] = static_cast<float>(rand() % 100 + 1); \
		arr3[i] = arr1[i] OP arr2[i]; \
	} \
	bm::Vector_<Len> vec1(arr1), vec2(arr2), vec3(arr3); \
	bm::Vector_Base<2> vecBase; \
	auto vecSum = vec1 OP vec2; \
	assert(vecSum == vec3); \
}

void foo() {

	//Vector<3> vec3(1, 2, 3), vec3another(3, 2, 1);
	//Vector<3> vec3sum = vec3 + vec3another;
	//vec3.x = 10;
	//cout << vec3.vals[0] << endl;
	//cout << vec3sum.x << ' ' << vec3sum.y << ' ' << vec3sum.z << endl;
	//Vector<4> vec4(1, 2, 3, 4), vec4_(4, 3, 2, 1);

	//cout << vec4.x << ' ' << vec4.y << ' ' << vec4.z << ' ' << vec4.w << endl;
	//cout << vec4Sum.vals[0] << ' ' << vec4Sum.vals[1] << ' ' << vec4Sum.vals[2] << ' ' << vec4Sum.vals[3] << endl;


	//Vector<10> vec10({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
}


// додати тести
// 
int main()
{
	bm::Vec3 vec1(1, 2, 0), vec2(0, 1, 0);
	auto prod = vec1 / 2;
	cout << prod.x << ' ' << prod.y << ' ' << prod.z << endl;
	TEST_BINARY_OPERATOR(+, 4);
	TEST_BINARY_OPERATOR(-, 4);
	TEST_BINARY_OPERATOR(/, 4);
	TEST_BINARY_OPERATOR(*, 4);
	/*
	double
	vec_elements_4[] = { 1., 2., 3., 5. },
	z_vec_elements[] = { 0., 0., 1. },
	y_vec_elements[] = { 0., 1., 0. };
	bm::Vector<4> vec_4(vec_elements_4);
	bm::Vector<3> z_vec(z_vec_elements), y_vec(y_vec_elements);

	double
	mat_elements[] = {
		1., 2., 3., 4.,
		5., 7., 7., 8.,
		9., 10., 12., 12.,
		13., 14., 15., 17.
	},
	mat_4_2_elemts[] = {
		1., 2.,
		3., 4.,
		5., 6.,
		7., 8.
	},
	mat_2_4_elemts[] = {
		1., 2., 3., 4.,
		5., 6., 7., 8.
	};
	bm::Matrix<4, 2> mat_4_2(mat_4_2_elemts);
	bm::Matrix<2, 4> mat_2_4(mat_2_4_elemts);
	bm::Matrix<4, 4> mat_4_4(mat_elements);

	cout
		<< "========== Vector demo ==========" << endl << endl

		<< "3d vector:" << endl
		<< z_vec << endl << endl

		<< "4d vector:" << endl
		<< vec_4 << endl << endl

		<< "dot product:" << endl
		<< vec_4.dot(vec_4) << endl << endl

		<< "cross product:" << endl
		<< z_vec.cross(y_vec) << endl << endl

		<< "sum of vectos:" << endl
		<< z_vec + y_vec << endl << endl

		<< "sub of vectos:" << endl
		<< z_vec - y_vec << endl << endl

		<< "========== Matrix demo ==========" << endl << endl

		<< "4x4 matrix:" << endl
		<< mat_4_4 << endl << endl

		<< "transposed matrix:" << endl
		<< mat_4_4.trans() << endl << endl

		<< "determinant is:" << endl
		<< mat_4_4.det() << endl << endl

		<< "inverted matrix:" << endl
		<< mat_4_4.inv() << endl << endl

		<< "matrix * inverted matrix:" << endl
		<< mat_4_4 * mat_4_4.inv() << endl << endl

		<< "inverted matrix * matrix:" << endl
		<< mat_4_4.inv() * mat_4_4 << endl << endl

		<< "matrix * vector:" << endl
		<< mat_4_4 * vec_4 << endl << endl;
	*/
 	return 0;
}

template <int Length, typename T>
std::ostream& operator<<(std::ostream& out, bm::Vector<Length, T> const& vec) {
	out << '[';
	for (int i = 0; i < Length - 1; ++i) {
		out << vec.at(i) << ", ";
	}
	out << vec.at(Length - 1) << "]";

	return out;
}

template <int Length, typename T>
std::ostream& operator<<(std::ostream& out, bm::Row<Length, T> const& vec) {
	out << '[';
	for (int i = 0; i < Length - 1; ++i) {
		out << vec.at(i) << ", ";
	}
	out << vec.at(Length - 1) << "]";

	return out;
}

template <int Rows, int Cols, typename T>
std::ostream& operator<<(std::ostream& out, bm::Matrix<Rows, Cols, T> const& mat) {
	out << '[' << endl;
	for (int i = 0; i < Rows - 1; i++) {
		out << "   " << mat.at(i) << ',' << endl;
	}
	out << "   " << mat.at(Rows - 1) << endl << "]";

	return out;
}
