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
