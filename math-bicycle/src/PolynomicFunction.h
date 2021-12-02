#ifndef _BICYCLE_POLYNOMIC_FUNCTION_H_
#define _BICYCLE_POLYNOMIC_FUNCTION_H_

#include <cmath>
#include <initializer_list>

namespace bm {

	template <int N, typename T>
	struct PolynomicFunction {

		PolynomicFunction(std::initializer_list<T> il) : m_coefficients(il) { }

		PolynomicFunction(T(&coefficients)[N]) {
			for (int i = 0; i < N; ++i) { m_coefficients[i] = T(coefficients[i]); }
		}

		T operator()(T const& arg) const {
			T res = m_coefficients[N - 1];
			for (int i = 0; i < N - 1; ++i) {
				res += m_coefficients[i] * std::pow<T, int>(arg, N - i - 1);
			}
			return res;
		}

	private:

		T m_coefficients[N];
	};

}

#endif // !_BICYCLE_POLYNOMIC_FUNCTION_H_