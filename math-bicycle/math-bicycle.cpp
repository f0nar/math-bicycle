#include <iostream>
#include "src/Vector.h"
#include "src/Matrix.h"

#include <cassert>

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
	float
	mat3arr[] = {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0
	},
	mat3arr2[] = {
		1.0, 2.0, 3.0,
		4.0, 5.0, 6.0,
		7.0, 8.0, 9.0
	};
	bm::Matrix<3, 3, float> mat(mat3arr), mat2(mat3arr2);
	cout << mat + mat2 << endl;

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
