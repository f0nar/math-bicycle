#ifndef _BICYCLE_MATRIX_H_
#define _BICYCLE_MATRIX_H_

#include <type_traits>
#include "Vector.h"

namespace bm {

	template <int Rows, int Cols, typename T>
	struct Matrix;

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
		Matrix<Cols, Rows, T> _invert(Matrix<Rows, Cols, T> mat) {
			Matrix<Rows, Cols, T> identity;
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

	};

	// how to instantiate Row for const T? It doesnt require scale, add, swap, ...
	// Can I do next: template <int Len, typename T> class Row <Len, const T> ?
	// Maybe we dont need Row class at all, it is vector. Ask Dmytro
	template <int Len, typename T>
	class Row {
	public:
		Row(T* row_data) : m_row_data(row_data) { }

		T& operator[](int i) {
			return m_row_data[i];
		}

		T const& at(int i) const {
			return m_row_data[i];
		}

		void scale(T const& s, bool multiply = true) {
			if (multiply) for (int i = 0; i < Len; ++i) m_row_data[i] = m_row_data[i] * s;
			else for (int i = 0; i < Len; ++i) m_row_data[i] = m_row_data[i] / s;
		}

		void add(Row<Len, T> const& row) {
			for (int i = 0; i < Len; ++i) m_row_data[i] = m_row_data[i] + row.at(i);
		}

		void addScaled(Row<Len, T> const& row, T const& s, bool multiply = true) {
			if (multiply) for (int i = 0; i < Len; ++i) m_row_data[i] = m_row_data[i] + row.at(i) * s;
			else for (int i = 0; i < Len; ++i) m_row_data[i] = m_row_data[i] + row.at(i) / s;
		}

		void swap(bm::Row<Len, T>& row) {
			for (int i = 0; i < Len; ++i) {
				T& temp = m_row_data[i];
				m_row_data[i] = other[i];
				other[i] = temp;
			}
		}

	private:
		T* m_row_data;
	};

	template <int Rows, int Cols, typename T>
	struct MatrixBase {

		MatrixBase() { }

		MatrixBase(T(&data)[Rows * Cols]) {
			for (int i = 0; i < Rows; ++i) {
				int const index = i * Cols;
				for (int j = 0; j < Cols; ++j) {
					m_vals[index + j] = T(data[index + j]);
				}
			}
		}

		Row<Cols, T> operator[](int i) {
			return row(i);
		}

		Row<Cols, const T> const at(int i) const {
			return row(i);
		}

		Row<Cols, T> row(int i) {
			return Row<Cols, T>(m_vals + i * Cols);
		}

		Row<Cols, const T> row(int i) const {
			return Row<Cols, const T>(m_vals + i * Cols);
		}

		T& at(int i, int j) {
			return m_vals[i * Cols + j];
		}

		T const & at(int i, int j) const {
			return m_vals[i * Cols + j];
		}

	protected:

		T m_vals[Rows * Cols] = { T() };

	};

	template <int Rows, int Cols, typename T, typename Square = void>
	struct MatrixSpec : MatrixBase<Rows, Cols, T> {
		using MatrixBase<Rows, Cols, T>::MatrixBase;
	};

	template <int Rows, int Cols, typename T>
	struct MatrixSpec<Rows, Cols, T, std::enable_if_t<(Rows == Cols)>> : MatrixBase<Rows, Cols, T> {

		using MatrixBase<Rows, Cols, T>::MatrixBase;

		Matrix<Cols, Rows, T> inv() const {
			return _invert<Cols, Rows, T, true>(*this);
		}

		T det() const {
			return _det<Rows, Cols, T, true>(*this);
		}
	};

	template <int Rows, int Cols, typename T>
	struct Matrix : MatrixSpec<Rows, Cols, T> {
		using MatrixSpec<Rows, Cols, T>::MatrixSpec;

		Matrix<Cols, Rows, T> trans() const {
			Matrix<Cols, Rows, T> resMat;
			for (int i = 0; i < Rows; ++i) {
				for (int j = 0; j < Cols; ++j) {
					resMat.at(j, i) = at(i, j);
				}
			}
			return resMat;
		}

		template <int OtherCols>
		Matrix<Rows, OtherCols, T> operator*(Matrix<Cols, OtherCols, T> const& other) const {
			Matrix<Rows, OtherCols, T> resMat;
			for (int i = 0; i < Rows; ++i) {
				for (int j = 0; j < OtherCols; ++j) {
					resMat.at(i, j) = at(i, 0) * other.at(0, j);
					for (int k = 1; k < Cols; ++k) {
						resMat.at(i, j) = resMat.at(i, j) + at(i, k) * other.at(k, j);
					}
				}
			}
			return resMat;
		}

		Matrix operator+(Matrix const& other) const {
			Matrix resMat;
			for (int i = 0; i < Rows; ++i) {
				for (int j = 0; j < Cols; ++j) {
					resMat.at(i, j) = at(i, j) + other.at(i, j);
				}
			}
			return resMat;
		}

		Matrix operator-(Matrix const& other) const {
			Matrix resMat;
			for (int i = 0; i < Rows; ++i) {
				for (int j = 0; j < Cols; ++j) {
					resMat.at(i, j) = at(i, j) - other.at(i, j);
				}
			}
			return resMat;
		}

		Matrix operator*(T scale) const {
			Matrix resMat;
			for (int i = 0; i < Rows; ++i) {
				for (int j = 0; j < Cols; ++j) {
					resMat.at(i, j) = at(i, j) * scale;
				}
			}
			return resMat;
		}

		Matrix operator/(T scale) const {
			Matrix resMat;
			for (int i = 0; i < Rows; ++i) {
				for (int j = 0; j < Cols; ++j) {
					resMat.at(i, j) = at(i, j) / scale;
				}
			}
			return resMat;
		}

		Vector<Rows, T> operator*(Vector<Cols, T> const& vec) const {
			Vector<Rows, T> resVec;
			for (int i = 0; i < Rows; ++i) {
				resVec[i] = at(i, 0) * vec.at(0);
				for (int j = 1; j < Cols; ++j) {
					resVec[i] = resVec[i] + at(i, j) * vec.at(j);
				}
			}

			return resVec;
		}
	};
}

#endif // _BICYCLE_MATRIX_H_