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


#define TEST_BINARY_OPERATOR(OP, Len) \
{ \
	float arr1[Len], arr2[Len], arr3[Len]; \
	for (int i = 0; i < Len; ++i) { \
		arr1[i] = static_cast<float>(rand() % 100); \
		arr2[i] = static_cast<float>(rand() % 100 + 1); \
		arr3[i] = arr1[i] OP arr2[i]; \
	} \
	bm::Vector<Len, float> vec1(arr1), vec2(arr2), vec3(arr3); \
	auto vecSum = vec1 OP vec2; \
	assert(vecSum == vec3); \
}

int main()
{
	//TEST_BINARY_OPERATOR(+, 4);
	//TEST_BINARY_OPERATOR(-, 4);
	//TEST_BINARY_OPERATOR(/, 4);
	//TEST_BINARY_OPERATOR(*, 4);

	float mat3arr[] = {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0
	};
	bm::Matrix<3, 3, float> mat(mat3arr);
	cout << mat.at(0) << endl;
	cout << mat.at(1) << endl;
	cout << mat.at(2) << endl;
	//bm::Matrix<3, 3, float> mat(mat3arr);

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
