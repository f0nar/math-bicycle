#include <iostream>
#include "src/Vector.h"
#include "src/Matrix.h"

using std::cout;
using std::endl;

template <int Length, typename T>
std::ostream& operator<<(std::ostream& out, bm::Vector<Length, T> const& vec);

template <int Length, typename T>
std::ostream& operator<<(std::ostream& out, bm::Row<Length, T> const& vec);

template <int Rows, int Cols, typename T>
std::ostream& operator<<(std::ostream& out, bm::Matrix<Rows, Cols, T> const& mat);

int main()
{
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
