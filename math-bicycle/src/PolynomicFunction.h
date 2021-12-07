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
			T res = m_coefficients[0];
			for (int i = 1; i < N; ++i) { res = std::fma(res, arg, m_coefficients[i]); }
			return res;
		}

	private:

		T m_coefficients[N];
	};

}

#endif // !_BICYCLE_POLYNOMIC_FUNCTION_H_