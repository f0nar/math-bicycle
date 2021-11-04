#include "Matrix.h"
#include "Vector.h"

namespace bm {

	// ================================ internal stuff ================================
	namespace {

		template <int Rows, int Cols, typename T, bool isSquare>
		T _det(Matrix<Rows, Cols, T> mat) {
			bool inverted = false;
			for (int i = 0; i < Rows - 1; ++i) {
				// ask Dmytro if I need to make mat[i][i] != T() comparison
				if (!mat[i][i]) {
					bool swaped = false;
					for (int j = i + 1; j < Rows; ++j) {
						if (mat[j][i]) {
							mat[j].swap(mat[i]);
							swaped = true;
							break;
						}
					}
					if (!swaped) {
						return mat[i][i];
					}
					else {
						inverted = !inverted;
					}
				}
				for (int j = i + 1; j < Rows; ++j) {
					if (mat[j][i]) {
						mat[j].addScaled(mat[i], -(mat[j][i] / mat[i][i]));
					}
				}
			}
			T det = mat[0][0];
			for (int i = 1; i < Rows; ++i) {
				det *= mat[i][i];
			}
			// unar minus sign
			// is it ok?
			return inverted ? -det : det;
		}


		template <int Rows, int Cols, typename T, bool isSquare>
		bm::Matrix<Cols, Rows, T> _invert(bm::Matrix<Rows, Cols, T> mat) {
			bm::Matrix<Rows, Cols, T> identity;
			for (int i = 0; i < Rows - 1; ++i) {
				if (!mat[i][i]) {
					for (int j = i + 1; j < Rows; ++j) {
						if (mat[j][i]) {
							mat[j].swap(mat[i]);
							identity[j].swap(identity[i]);
							break;
						}
					}
				}
				for (int j = i + 1; j < Rows; ++j) {
					if (mat[j][i]) {
						const T scale = -(mat[j][i] / mat[i][i]);
						mat[j].addScaled(mat[i], scale);
						identity[j].addScaled(identity[i], scale);
					}
				}
			}
			for (int i = Rows - 1; i > 0; --i) {
				for (int j = i - 1; j >= 0; --j) {
					if (mat[j][i]) {
						const T scale = -(mat[j][i] / mat[i][i]);
						// here we need to add only one element then all row
						mat[j].addScaled(mat[i], scale);
						identity[j].addScaled(identity[i], scale);
					}
				}
			}
			for (int i = 0; i < Rows; ++i) {
				const T scale = mat[i][i];
				mat[i][i] /= scale;
				identity[i].scale(scale, false);
			}

			return identity;
		}

		template <typename D>
		void fillWith(D* arr, int rows, int cols, D const& diagonalValue) {
			for (int i = 0; i < rows; ++i) {
				for (int j = 0; j < cols; ++j) {
					int const index = i * cols + j;
					arr[index] = i != j ? D() : D(diagonalValue);
				}
			}
		}

		template <typename D>
		void fillDefault(D* arr, int rows, int cols) {
			fillWith<D>(arr, rows, cols, D());
		}

		template <>
		// refactor that with smth like fillWith<COLS == ROWS, std::is_integral(T)>
		// is it posible to implement with functions?
		void fillDefault<double>(double* arr, int rows, int cols) {
			fillWith<double>(arr, rows, cols, 1.0);
		}

	};

	// ================================ Row ================================

	template <int LENGTH, typename T>
	Row<LENGTH, T>::Row(T* row_data)
		: m_row_data(row_data)
	{}

	template <int LENGTH, typename T>
	T& Row<LENGTH, T>::operator[](int i) {
		// TODO check range
		return m_row_data[i];
	}

	template <int LENGTH, typename T>
	T const& Row<LENGTH, T>::at(int i) const {
		// TODO check range
		return m_row_data[i];
	}

	template <int LENGTH, typename T>
	void Row<LENGTH, T>::scale(T const& s, bool multiply) {
		// ask Dmytro if *= / /= operator is better here
		if (multiply) for (int i = 0; i < LENGTH; ++i) m_row_data[i] = m_row_data[i] * s;
		else for (int i = 0; i < LENGTH; ++i) m_row_data[i] = m_row_data[i] / s;
	}

	template <int LENGTH, typename T>
	void Row<LENGTH, T>::add(Row<LENGTH, T> const& row){
		// ask Dmytro if += operator is better here
		for (int i = 0; i < LENGTH; ++i) m_row_data[i] = m_row_data[i] + row.at(i);
	}

	template <int LENGTH, typename T>
	void Row<LENGTH, T>::addScaled(Row<LENGTH, T> const& row, T const& s, bool multiply) {
		// ask Dmytro if += operator is better here
		if (multiply) for (int i = 0; i < LENGTH; ++i) m_row_data[i] = m_row_data[i] + row.at(i) * s;
		else for (int i = 0; i < LENGTH; ++i) m_row_data[i] = m_row_data[i] + row.at(i) / s;
	}

	template <int LENGTH, typename T>
	void Row<LENGTH, T>::swap(Row<LENGTH, T>& other) {
		for (int i = 0; i < LENGTH; ++i) {
			T& temp = m_row_data[i];
			m_row_data[i] = other[i];
			other[i] = temp;
		}
	}

	// ================================ Matrix ================================

	template <int Rows, int Cols, typename T, bool isSquare>
	Matrix<Rows, Cols, T, isSquare>::Matrix() {
		fillDefault<T>(m_data, Rows, Cols);
	};

	template <int Rows, int Cols, typename T, bool isSquare>
	Matrix<Rows, Cols, T, isSquare>::Matrix(T *data) {
		for (int i = 0; i < Rows; ++i) {
			for (int j = 0; j < Cols; ++j) {
				int const index = i * Cols + j;
				m_data[index] = T(data[index]);
			}
		}
	}

	template <int Rows, int Cols, typename T, bool isSquare>
	Row<Cols, T> Matrix<Rows, Cols, T, isSquare>::operator[](int i) {
		return row(i);
	}

	template <int Rows, int Cols, typename T, bool isSquare>
	Row<Cols, const T> const Matrix<Rows, Cols, T, isSquare>::at(int i) const {
		return Row<Cols, const T>(m_data + i * Cols);
	}

	template <int Rows, int Cols, typename T, bool isSquare>
	Matrix<Cols, Rows, T> Matrix<Rows, Cols, T, isSquare>::trans() const {
		T transposed_arr[Cols * Rows];
		for (int i = 0; i < Rows; ++i) {
			for (int j = 0; j < Cols; ++j) {
				int const index = i * Cols + j;
				int const transposed_index = j * Rows + i;
				transposed_arr[transposed_index] = m_data[index];
			}
		}
		return Matrix<Cols, Rows, T>(transposed_arr);
	}

	template <int Rows, int Cols, typename T, bool isSquare>
	Row<Cols, T> Matrix<Rows, Cols, T, isSquare>::row(int i) {
		return Row<Cols, T>(m_data + i * Cols);
	}

	template <int Rows, int Cols, typename T, bool isSquare>
	Row<Cols, const T> Matrix<Rows, Cols, T, isSquare>::row(int i) const {
		return Row<Cols, const T>(m_data + i * Cols);
	}


	template <int Rows, int Cols, typename T>
	Matrix<Cols, Rows, T, true> Matrix<Rows, Cols, T, true>::inv() const {
		// Does return optimization work here?
		return _invert<Cols, Rows, T>(*this);
	}

	template <int Rows, int Cols, typename T>
	T Matrix<Rows, Cols, T, true>::det() const {
		return _det<Rows, Cols, T>(*this);
	}

	template <typename T, int Rows, int Cols1, int Cols2>
	Matrix<Rows, Cols2, T> operator*(Matrix<Rows, Cols1, T> const& mat1, Matrix<Cols1, Cols2, T> const& mat2) {
		T res_arr[Rows * Cols2];
		for (int i = 0; i < Rows; ++i) {
			for (int j = 0; j < Cols2; ++j) {
				// it is dot product
				// do we need row here? is it better to create vector_row vector_col and work with that?
				int const res_index = i * Cols2 + j;
				res_arr[res_index] = mat1.at(i).at(0) * mat2.at(0).at(j);
				for (int k = 1; k < Cols1; ++k) {
					res_arr[res_index] = res_arr[res_index] +  mat1.at(i).at(k) * mat2.at(k).at(j);
				}
			}
		}
		return Matrix<Rows, Cols2, T>(res_arr);
	}

	template <typename T, int Rows, int Cols, typename SQRT>
	Vector<Rows, T, SQRT, (Rows == 3)> operator*(Matrix<Rows, Cols, T> const& mat, Vector<Cols, T, SQRT, (Cols == 3)> const& vec) {
		T res_arr[Cols];
		// implementation is almost same as matrix multiplication
		for (int i = 0; i < Rows; ++i) {
			res_arr[i] = mat.at(i).at(0) * vec.at(0);
			for (int j = 1; j < Cols; ++j) {
				res_arr[i] = res_arr[i] + mat.at(i).at(j) * vec.at(j);
			}
		}

		return Vector<Rows, T>(res_arr);
	}

};