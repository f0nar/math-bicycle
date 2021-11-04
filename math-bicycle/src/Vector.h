#ifndef _BICYCLE_VECTOR_H_
#define _BICYCLE_VECTOR_H_

#include <cmath>
#include <type_traits>

namespace bm {

	template <int LEN = 3, typename T = double, typename SQRT = T(*)(T), bool is3D = (LEN == 3)>
	class Vector {
	public:
		// add check if LEN < 1

		Vector(T *values, SQRT sqrt = std::sqrt);

		T& operator[](int index);
		T const& at(int index) const;

		int len() const;
		T norm() const;
		void normalize();

		// looks bad
		T dot(Vector<LEN, T, SQRT, (LEN == 3)> const& multV) const;

	protected:
		// try to change to std::array
		T m_vals[LEN];
		SQRT sqrt;

	};

	template <typename T, typename SQRT>
	class Vector<3, T, SQRT, true> : public Vector<3, T, SQRT, false> {
	public:

		Vector(T* values, SQRT sqrt = std::sqrt) : Vector<3, T, SQRT, false>(values, sqrt) {}

		// try to use enble_if here like std::enable_if_t< (LEN == 3), Vector <LEN, T, SQRT, true> >
		Vector <3, T, SQRT, true> cross(Vector<3, T, SQRT, true> const& other) const;

	};

	template <int LEN, typename T, typename SQRT, bool is3D>
	Vector<LEN, T, SQRT, is3D> operator+(Vector<LEN, T, SQRT, is3D> const& addV1, Vector<LEN, T, SQRT, is3D> const& addV2);

	template <int LEN, typename T, typename SQRT, bool is3D>
	Vector<LEN, T, SQRT, is3D> operator-(Vector<LEN, T, SQRT, is3D> const& subV1, Vector<LEN, T, SQRT, is3D> const& subV2);

}

#include "Vector.hpp"

#endif //_BICYCLE_VECTOR_H_