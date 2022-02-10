#ifndef _BICYCLE_POINT_H_
#define _BICYCLE_POINT_H_

#include "Vector.h"
#include <type_traits>
#include <string>
#include <cmath>
#include <cassert>

namespace bm {

	template <int Len, typename T>
	struct Point;

	class _PointInternal {

		template <int Len, typename T>
		friend struct Point;

		#define BINARY_OPERATOR(OP, RETURN_T, OTHER_T, OTHER_ACCES) \
		auto operator OP(OTHER_T const& other) const { \
			T init_array[Len]; \
			for (int i = 0; i < Len; ++i) { \
				init_array[i] = this->vals[i] OP other OTHER_ACCES; \
			} \
			return RETURN_T(init_array); \
		}

		template <int Len, typename T>
		struct PointBase {

			explicit PointBase() : vals{ T() } { }

			explicit PointBase(T const (&data)[Len]) {
				for (int i = 0; i < Len; ++i) {
					vals[i] = T{ data[i] };
				}
			} 
			template <typename ...Ts>
			explicit PointBase(Ts ... args) : vals{ T(args)... } {
				static_assert(sizeof...(args) == Len, "Number of point constructor arguments should be equal to its length.");
			}

			#define TEMPLATE_POINT Point<Len, T>
			#define TEMPLATE_VECTOR Vector<Len, T>
			BINARY_OPERATOR(+, TEMPLATE_POINT, TEMPLATE_VECTOR, .at(i));
			BINARY_OPERATOR(-, TEMPLATE_POINT, TEMPLATE_VECTOR, .at(i));
			BINARY_OPERATOR(-, TEMPLATE_VECTOR, TEMPLATE_POINT, .vals[i]);

			T const& at(int index) const {
				assert(index >= 0 && index < Len);
				return vals[index];
			}

			T& operator[](int index) {
				assert(index >= 0 && index < Len);
				return vals[index];
			}

			bool operator==(Point<Len, T> const& other) const {
				for (int i = 0; i < Len; ++i) {
					if (this->vals[i] != other.vals[i]) return false;
				}
				return true;
			}

			bool operator!=(Point<Len, T> const& other) const {
				return !(*this == other);
			}

		protected:

			T vals[Len];

		};
	};

	#define POINT_ASSIGN_OPERATOR(Len) \
	Point & operator=(Point const& another) { \
		for (int i = 0; i < Len; ++i) { vals[i] = T(another.vals[i]); } \
		return *this; \
	}

	template <int Len = 3, typename T = float>
	struct Point : _PointInternal::PointBase<Len, T> {

		using _PointInternal::PointBase<Len, T>::PointBase;

		POINT_ASSIGN_OPERATOR(Len);

	};

	template <typename T>
	struct Point<4, T> : _PointInternal::PointBase<4, T> {

		using _PointInternal::PointBase<4, T>::PointBase;

		Point(Point const& other) : Point(other.x, other.y, other.z, other.w) {}

		POINT_ASSIGN_OPERATOR(4);

		Point<2, T> xy() const {
			return Point<2, T>(x, y);
		}

		Point<3, T> xyz() const {
			return Point<3, T>(x, y, z);
		}

		T& x = this->vals[0];
		T& y = this->vals[1];
		T& z = this->vals[2];
		T& w = this->vals[3];

	};

	template <typename T>
	struct Point<3, T> : _PointInternal::PointBase<3, T> {

		using _PointInternal::PointBase<3, T>::PointBase;

		Point(Point const& other) : Point(other.x, other.y, other.z) {}

		POINT_ASSIGN_OPERATOR(3);

		Point<2, T> xy() const {
			return Point<2, T>(x, y);
		}

		T& x = this->vals[0];
		T& y = this->vals[1];
		T& z = this->vals[2];

	};

	template <typename T>
	struct Point<2, T> : _PointInternal::PointBase<2, T> {

		using _PointInternal::PointBase<2, T>::PointBase;

		Point(Point const& other) : Point(other.x, other.y) {}

		POINT_ASSIGN_OPERATOR(2);

		T& x = this->vals[0];
		T& y = this->vals[1];

	};

	template <int Len, typename T>
	bool equals(Point<Len, T> const& point1, Point<Len, T> const& point2, T delta = T()) {
		if (&point1 == &point2)
			return true;

		for (int i = 0; i < Len; ++i) {
			T const& point1i = point1.at(i);
			T const& point2i = point2.at(i);
			if (
				!(point1i <= point2i + delta && point2i <= point1i + delta) &&
				!(point2i <= point1i + delta && point1i <= point2i + delta)
			) {
				return false;
			}
		}

		return true;
	}


	template <typename ToType, typename FromType, int Len>
	Point<Len, ToType> changeT(Point<Len, FromType> const& fromVec) {
		ToType initArray[Len] = { ToType() };
		for (int i = 0; i < Len; ++i) initArray[i] = ToType(fromVec.at(i));
		return Point<Len, ToType>(initArray);
	}

	using Point4f = Point<4, float>;
	using Point3f = Point<3, float>;
	using Point2f = Point<2, float>;

	using Point4d = Point<4, double>;
	using Point3d = Point<3, double>;
	using Point2d = Point<2, double>;

	using Point4i = Point<4, int>;
	using Point3i = Point<3, int>;
	using Point2i = Point<2, int>;

	using Point4ui = Vector<4, unsigned int>;
	using Point3ui = Vector<3, unsigned int>;
	using Point2ui = Vector<2, unsigned int>;
};


#endif _BICYCLE_POINT_H_