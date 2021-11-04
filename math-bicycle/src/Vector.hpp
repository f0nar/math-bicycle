#include <stdexcept>
#include "Vector.h"

namespace bm {

	template <int LEN, typename T, typename SQRT, bool is3D>
	Vector<LEN, T, SQRT, is3D>::Vector(T *values, SQRT sqrt) : sqrt(sqrt) {
		for (int i = 0; i < LEN; ++i) {
			m_vals[i] = T(values[i]);
		}// foreach
	}

	template <int LEN, typename T, typename SQRT, bool is3D>
	T& Vector<LEN, T, SQRT, is3D>::operator[](int index) {
		if (index < 0 || index >= LEN) {
			throw std::out_of_range("Index out of range");
		}
		return m_vals[index];
	}

	template <int LEN, typename T, typename SQRT, bool is3D>
	T const& Vector<LEN, T, SQRT, is3D>::at(int index) const {
		if (index < 0 || index >= LEN) {
			throw std::out_of_range("Index out of range");
		}
		return m_vals[index];
	}

	template <int LEN, typename T, typename SQRT, bool is3D>
	int Vector<LEN, T, SQRT, is3D>::len() const {
		return LEN;
	}

	template <int LEN, typename T, typename SQRT, bool is3D>
	T Vector<LEN, T, SQRT, is3D>::norm() const {
		T powSum = m_vals[0] * m_vals[0];
		for (int i = 1; i < LEN; ++i) {
			powSum = powSum + m_vals[i] * m_vals[i];
		}// acumulate map i*i
		return sqrt(powSum);
	}

	template <int LEN, typename T, typename SQRT, bool is3D>
	void Vector<LEN, T, SQRT, is3D>::normalize() {
		T const n = norm();
		for (int i = 0; i < LEN; ++i) {
			m_vals[i] /= n;
		}// foreach
	}

	template <int LEN, typename T, typename SQRT, bool is3D>
	T Vector<LEN, T, SQRT, is3D>::dot(Vector<LEN, T, SQRT, (LEN == 3)> const& multV) const {
		T powSum = m_vals[0] * multV.at(0);
		for (int i = 1; i < LEN; ++i) {
			powSum = powSum + m_vals[i] * multV.at(i);
		}
		return powSum;
	}


	template <typename T, typename SQRT>
	Vector <3, T, SQRT, true> Vector<3, T, SQRT, true>::cross(Vector<3, T, SQRT, true> const& other) const {
	//std::enable_if<(LEN == 3), Vector < LEN, T, SQRT, true >>::type Vector<LEN, T, SQRT, true>::cross(Vector<LEN, T, SQRT, true> const& other) const {
		T res_arr[] = {
			this->at(1) * other.at(2) - this->at(2) * other.at(1),
			this->at(2) * other.at(0) - this->at(0) * other.at(2),
			this->at(0) * other.at(1) - this->at(1) * other.at(0)
		};

		return Vector<3, T, SQRT, true>(res_arr);
	}

	template <int LEN, typename T, typename SQRT, bool is3D>
	Vector<LEN, T, SQRT, is3D> operator+(Vector<LEN, T, SQRT, is3D> const& addV1, Vector<LEN, T, SQRT, is3D> const& addV2) {
		T vals[LEN];
		for (int i = 0; i < LEN; ++i) {
			vals[i] = addV1.at(i) + addV2.at(i);
		}
		return Vector<LEN, T, SQRT, is3D>(vals);
	}


	template <int LEN, typename T, typename SQRT, bool is3D>
	Vector<LEN, T, SQRT, is3D> operator-(Vector<LEN, T, SQRT, is3D> const& subV1, Vector<LEN, T, SQRT, is3D> const& subV2) {
		T vals[LEN];
		for (int i = 0; i < LEN; ++i) {
			vals[i] = subV1.at(i) - subV2.at(i);
		}
		return Vector<LEN, T, SQRT, is3D>(vals);
	}
};