#ifndef _BICYCLE_VECTOR_H_
#define _BICYCLE_VECTOR_H_

#include <type_traits>
#include <string>
#include <cmath>
#include <cassert>

namespace bm {

	template <int Len, typename T>
	struct Vector;

	class _VectorInternal {

		template <int Len, typename T>
		friend struct Vector;

		#define BINARY_OPERATOR(OP, OTHER_T, OTHER_ACCES) \
		auto operator OP(OTHER_T const& other) const { \
			Vector<Len, T> res; \
			for (int i = 0; i < Len; ++i) { \
				res.vals[i] = this->vals[i] OP other OTHER_ACCES; \
			} \
				return res; \
		}

		template <int Len, typename T>
		struct VectorBase {

			VectorBase() : vals{ T() } { }

			VectorBase(T const (&data)[Len]) {
				for (int i = 0; i < Len; ++i) {
					vals[i] = T{ data[i] };
				}
			}

			template <typename ...Ts>
			VectorBase(Ts ... args) : vals{ T(args)... } {
				static_assert(sizeof...(args) == Len, "Number of vector constructor arguments should be equal to its length.");
			}

			#define TEMPLATE_VECTOR Vector<Len, T>
			BINARY_OPERATOR(+, TEMPLATE_VECTOR, .vals[i]);
			BINARY_OPERATOR(-, TEMPLATE_VECTOR, .vals[i]);
			BINARY_OPERATOR(*, TEMPLATE_VECTOR, .vals[i]);
			BINARY_OPERATOR(/, TEMPLATE_VECTOR, .vals[i]);
			BINARY_OPERATOR(/, float);
			BINARY_OPERATOR(*, float);

			T const &at(int index) const {
				assert(index >= 0 && index < Len);
				return vals[index];
			}

			T& operator[](int index) {
				assert(index >= 0 && index < Len);
				return vals[index];
			}

			auto dot(Vector<Len, T> const &other) const {
				auto res = this->vals[0] * other.vals[0];
				for (int i = 1; i < Len; ++i) res = std::fma(this->vals[i], other.vals[i], res);
				return res;
			}

			bool operator==(Vector<Len, T> const& other) const {
				for (int i = 0; i < Len; ++i) {
					if (this->vals[i] != other.vals[i]) return false;
				}
				return true;
			}

			bool operator!=(Vector<Len, T> const& other) const {
				return !(*this == other);
			}

		protected:

			T vals[Len];

		};
	};

	#define VECTOR_ASSIGN_OPERATOR(Len) \
	Vector & operator=(Vector const& another) { \
		for (int i = 0; i < Len; ++i) { vals[i] = T(another.vals[i]); } \
		return *this; \
	}

	template <int Len = 3, typename T = float>
	struct Vector : _VectorInternal::VectorBase<Len, T> {

		using _VectorInternal::VectorBase<Len, T>::VectorBase;

		VECTOR_ASSIGN_OPERATOR(Len);

	};

	template <typename T>
	struct Vector<4, T> : _VectorInternal::VectorBase<4, T> {

		using _VectorInternal::VectorBase<4, T>::VectorBase;

		Vector(Vector const& other) : Vector(other.x, other.y, other.z, other.w) {}

		VECTOR_ASSIGN_OPERATOR(4);

		T& x = this->vals[0];
		T& y = this->vals[1];
		T& z = this->vals[2];
		T& w = this->vals[3];

	};

	template <typename T>
	struct Vector<3, T> : _VectorInternal::VectorBase<3, T> {

		using _VectorInternal::VectorBase<3, T>::VectorBase;

		Vector(Vector const& other) : Vector(other.x, other.y, other.z) {}

		VECTOR_ASSIGN_OPERATOR(3);

		Vector cross(Vector const& other) const {
			return Vector({
				this->at(1) * other.at(2) - this->at(2) * other.at(1),
				this->at(2) * other.at(0) - this->at(0) * other.at(2),
				this->at(0) * other.at(1) - this->at(1) * other.at(0)
			});
		}

		T& x = this->vals[0];
		T& y = this->vals[1];
		T& z = this->vals[2];

	};

	template <typename T>
	struct Vector<2, T> : _VectorInternal::VectorBase<2, T> {

		using _VectorInternal::VectorBase<2, T>::VectorBase;

		Vector(Vector const& other) : Vector(other.x, other.y) {}

		VECTOR_ASSIGN_OPERATOR(2);

		T& x = this->vals[0];
		T& y = this->vals[1];

	};

	template <int Len, typename T>
	bool equals(Vector<Len, T> const& vec1, Vector<Len, T> const& vec2, T delta = T()) {
		if (&vec1 == &vec2)
			return true;

		for (int i = 0; i < Len; ++i) {
			T const& vec1i = vec1.at(i);
			T const& vec2i = vec2.at(i);
			if (
				!(vec1i <= vec2i + delta && vec2i <= vec1i + delta) &&
				!(vec2i <= vec1i + delta && vec1i <= vec2i + delta)) {
				return false;
			}
		}

		return true;
	}

	using Vector4f = Vector<4, float>;
	using Vector3f = Vector<3, float>;
	using Vector2f = Vector<2, float>;

	using Vector4d = Vector<4, double>;
	using Vector3d = Vector<3, double>;
	using Vector2d = Vector<2, double>;

	using Vector4i = Vector<4, int>;
	using Vector3i = Vector<3, int>;
	using Vector2i = Vector<2, int>;

	using Vector4ui = Vector<4, unsigned int>;
	using Vector3ui = Vector<3, unsigned int>;
	using Vector2ui = Vector<2, unsigned int>;
};

#endif // !_BICYCLE_VECTOR_H_