#ifndef _BICYCLE_MATRIX_H_
#define _BICYCLE_MATRIX_H_

namespace bm {

	template <int LEN, typename T, typename SQRT, bool is3D>
	class Vector;

	// how to instantiate Row for const T? It doesnt require scale, add, swap, ...
	// Can I do next: template <int LENGTH, typename T> class Row <LENGTH, const T> ?
	// Maybe we dont need Row class at all, it is vector. Ask Dmytro
	template <int LENGTH, typename T>
	class Row {
	public:
		Row(T* row_data);

		T& operator[](int i);
		T const& at(int i) const;

		void scale(T const& s, bool multiply = true);
		void add(Row<LENGTH, T> const& row);
		void addScaled(Row<LENGTH, T> const& row, T const& s, bool multiply = true);
		void swap(bm::Row<LENGTH, T>& row);

	private:
		T* m_row_data;
	};


	template <int Rows = 3, int Cols = 3, typename T = double, bool isSquare = (Rows == Cols)>
	class Matrix {
	public:

		Matrix(T *data);
		Matrix();
	
		Matrix<Cols, Rows, T, (Cols == Rows)> trans() const;
		Row<Cols, T> operator[](int i);
		Row<Cols, const T> const at(int i) const;

	protected:
		Row<Cols, T> row(int i);
		Row<Cols, const T> row(int i) const;

		T m_data[Rows * Cols];
		 
	};

	template <int Rows, int Cols, typename T>
	class Matrix<Rows, Cols, T, true> : public Matrix<Rows, Cols, T, false> {
	public:

		Matrix() : Matrix<Rows, Cols, T, false>() {}
		Matrix(T* data) : Matrix<Rows, Cols, T, false>(data) {}

		Matrix<Cols, Rows, T, true> inv() const;

		T det() const;

	};

	template <typename T, int Rows, int Cols, typename SQRT>
	Vector<Rows, T, SQRT, (Rows == 3)> operator*(Matrix<Rows, Cols, T> const &mat, Vector<Cols, T, SQRT, (Cols == 3)> const& vec);

	template <typename T, int Rows, int Cols1, int Cols2>
	Matrix<Rows, Cols2, T> operator*(Matrix<Rows, Cols1, T> const& mat1, Matrix<Cols1, Cols2, T> const& mat2);

}

#include "Matrix.hpp"

#endif // _BICYCLE_MATRIX_H_