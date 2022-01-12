#ifndef _BICYCLE_MATRIX_H_
#define _BICYCLE_MATRIX_H_

#include <type_traits>
#include "Vector.h"

namespace bm {

	template <int Rows, int Cols, typename T>
	struct Matrix;

	template <int Rows, int Cols, typename T, typename IsSquare>
	struct MatrixSpec;

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

		void swap(bm::Row<Len, T>& other) {
			for (int i = 0; i < Len; ++i) {
				T temp = m_row_data[i];
				m_row_data[i] = other[i];
				other[i] = temp;
			}
		}

	private:
		T* m_row_data;
	};

	class _MatrixInternal {

		template <int Rows, int Cols, typename T>
		friend struct Matrix;

		template <int Rows, int Cols, typename T, typename IsArithmeticSquare = void>
		struct InitMatrixDefault
		{
			void init(T(&matrix_array)[Rows * Cols]) { }
		};

		template <int Rows, int Cols, typename T>
		struct InitMatrixDefault<Rows, Cols, T, std::enable_if_t<(Rows == Cols && std::is_arithmetic<T>::value)>>
		{
			void init(T(&matrix_array)[Rows * Cols]) {
				T const diagonal_value = static_cast<T>(1);
				for (int i = 0; i < Rows; ++i) {
					int const array_index = i * Rows + i;
					matrix_array[array_index] = diagonal_value;
				}
			}
		};

		template <int Rows, int Cols, typename T>
		struct MatrixBase {

			MatrixBase() {
				InitMatrixDefault<Rows, Cols, T> data_initializer;
				data_initializer.init(m_vals);
			}

			MatrixBase(T const (&data)[Rows * Cols]) {
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

			T const& at(int i, int j) const {
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
				Matrix<Cols, Rows, T> identity, this_copy(m_vals);
				for (int i = 0; i < Rows - 1; ++i) {
					if (!this_copy[i][i]) {
						for (int j = i + 1; j < Rows; ++j) {
							if (this_copy[j][i]) {
								this_copy[j].swap(this_copy[i]);
								identity[j].swap(identity[i]);
								break;
							}
						}
					}
					for (int j = i + 1; j < Rows; ++j) {
						if (this_copy[j][i]) {
							const T scale = -(this_copy[j][i] / this_copy[i][i]);
							this_copy[j].addScaled(this_copy[i], scale);
							identity[j].addScaled(identity[i], scale);
						}
					}
				}
				for (int i = Rows - 1; i > 0; --i) {
					for (int j = i - 1; j >= 0; --j) {
						if (this_copy[j][i]) {
							const T scale = -(this_copy[j][i] / this_copy[i][i]);
							// here we need to add only one element then all row
							this_copy[j].addScaled(this_copy[i], scale);
							identity[j].addScaled(identity[i], scale);
						}
					}
				}
				for (int i = 0; i < Rows; ++i) {
					const T scale = this_copy[i][i];
					this_copy[i][i] /= scale;
					identity[i].scale(scale, false);
				}

				return identity;
			}

			T det() const {
				Matrix<Rows, Cols, T> this_copy(m_vals);
				bool inverted = false;
				for (int i = 0; i < Rows - 1; ++i) {
					// ask Dmytro if I need to make mat[i][i] != T() comparison
					if (!this_copy[i][i]) {
						bool swaped = false;
						for (int j = i + 1; j < Rows; ++j) {
							if (this_copy[j][i]) {
								this_copy[j].swap(this_copy[i]);
								swaped = true;
								break;
							}
						}
						if (!swaped) {
							return this_copy[i][i];
						}
						else {
							inverted = !inverted;
						}
					}
					for (int j = i + 1; j < Rows; ++j) {
						if (this_copy[j][i]) {
							this_copy[j].addScaled(this_copy[i], -(this_copy[j][i] / this_copy[i][i]));
						}
					}
				}
				T det = this_copy[0][0];
				for (int i = 1; i < Rows; ++i) {
					det *= this_copy[i][i];
				}
				// unar minus sign
				// is it ok?
				return inverted ? -det : det;
			}
		};

	};

	template <int Rows, int Cols, typename T>
	struct Matrix : _MatrixInternal::MatrixSpec<Rows, Cols, T> {

		using _MatrixInternal::MatrixSpec<Rows, Cols, T>::MatrixSpec;

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

	template <int Rows, int Cols, typename T>
	bool equals(Matrix<Rows, Cols, T> const& mat1, Matrix<Rows, Cols, T> const& mat2, T const &delta = T()) {
		if (&mat1 == &mat2)
			return true;

		for (int i = 0; i < Rows; ++i) {
			for (int j = 0; j < Cols; ++j) {
				T const& mat1ij = mat1.at(i, j);
				T const& mat2ij = mat2.at(i, j);
				if (
					!(mat1ij <= mat2ij + delta && mat2ij <= mat1ij + delta) &&
					!(mat2ij <= mat1ij + delta && mat1ij <= mat2ij + delta)) {
					return false;
				}
			}
		}

		return true;
	}

	using Matrix4f = Matrix<4, 4, float>;
	using Matrix3f = Matrix<3, 3, float>;
	using Matrix2f = Matrix<2, 2, float>;

	using Matrix4d = Matrix<4, 4, double>;
	using Matrix3d = Matrix<3, 3, double>;
	using Matrix2d = Matrix<2, 2, double>;

	using Matrix4i = Matrix<4, 4, int>;
	using Matrix3i = Matrix<3, 3, int>;
	using Matrix2i = Matrix<2, 2, int>;

	using Matrix4ui = Matrix<4, 4, unsigned int>;
	using Matrix3ui = Matrix<3, 3, unsigned int>;
	using Matrix2ui = Matrix<2, 2, unsigned int>;
}

#endif // _BICYCLE_MATRIX_H_