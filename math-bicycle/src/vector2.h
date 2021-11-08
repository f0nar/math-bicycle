#ifndef _Vector_2_H_
#define _Vector_2_H_

#include <type_traits>
#include <cmath>
#include <cassert>

namespace bm {

	template <int Len, typename isSpecial = void>
	struct Vector_;

#define BINARY_OPERATOR(OP, OTHER_T, OTHER_ACCES) \
	auto operator OP(OTHER_T const& other) const { \
		Vector_<Len> res; \
		for (int i = 0; i < Len; ++i) { \
			res.vals[i] = this->vals[i] OP other OTHER_ACCES; \
		} \
		return res; \
	}

	namespace {
		// TODO типізувати дані
		template <int Len>
		struct Vector_Base {

			Vector_Base() { }

			// TODO заборонити виклик з невідповідним розміром масиву
			// TODO генерувати конструктор відносно кількості параметрів
			// template <typename T>
			Vector_Base(float const (&data)[Len]) {
				for (int i = 0; i < Len; ++i) {
					vals[i] = data[i];
				}
			}

			BINARY_OPERATOR(+, Vector_<Len>, .vals[i]);
			BINARY_OPERATOR(-, Vector_<Len>, .vals[i]);
			BINARY_OPERATOR(*, Vector_<Len>, .vals[i]);
			BINARY_OPERATOR(/, Vector_<Len>, .vals[i]);
			BINARY_OPERATOR(/ , float);
			BINARY_OPERATOR(* , float);

			float at(int index) const {
				assert(index >= 0 && index < Len);
				return vals[index];
			}

			float dot() const {
				float res = this->vals[0] * this->vals[0];
				for (int i = 1; i < Len; ++i) res = std::fma(this->vals[i], this->vals[i], res);
				return res;
			}

			bool operator==(Vector_<Len> const& other) const {
				for (int i = 0; i < Len; ++i) {
					if (this->vals[i] != other.vals[i]) return false;
				}
				return true;
			}

			bool operator!=(Vector_<Len> const& other) const {
				return !(*this == other);
			}

			float vals[Len] = { 0 };

		};
	};

	template <int Len, typename isSpecial = void>
	struct Vector_ : Vector_Base<Len> {
		using Vector_Base<Len>::Vector_Base;
	};

	template <int Len>
	struct Vector_<Len, std::enable_if_t<(Len == 4)>> : Vector_Base<Len> {

		using Vector_Base<Len>::Vector_Base;

		Vector_(Vector_ const& other)
			: Vector_(other.x, other.y, other.z, other.w) {}

		Vector_(float x, float y, float z, float w)
			: Vector_Base<Len>({ x, y, z, w }) {}

		float& x = this->vals[0];
		float& y = this->vals[1];
		float& z = this->vals[2];
		float& w = this->vals[3];
	};

	template <int Len>
	struct Vector_<Len, std::enable_if_t<(Len == 3)>>
		: Vector_Base<Len>
	{

		using Vector_Base<Len>::Vector_Base;

		Vector_(Vector_ const& other)
			: Vector_(other.x, other.y, other.z) {}

		Vector_(float x, float y, float z)
			: Vector_Base<Len>({ x, y, z }) {}

		Vector_ cross(Vector_ const& other) const {
			return Vector_({
				this->at(1) * other.at(2) - this->at(2) * other.at(1),
				this->at(2) * other.at(0) - this->at(0) * other.at(2),
				this->at(0) * other.at(1) - this->at(1) * other.at(0)
			});
		}

		float& x = this->vals[0];
		float& y = this->vals[1];
		float& z = this->vals[2];
	};

	template <int Len>
	struct Vector_<Len, std::enable_if_t<(Len == 2)>> : Vector_Base<Len> {

		using Vector_Base<Len>::Vector_Base;

		Vector_(Vector_ const& other)
			: Vector_(other.x, other.y) {}

		Vector_(float x, float y)
			: Vector_Base<Len>({ x, y }) {}

		float& x = this->vals[0];
		float& y = this->vals[1];
	};

	using Vec3 = Vector_<3>;

};

#endif // !_Vector_2_H_