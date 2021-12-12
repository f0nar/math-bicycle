#ifndef _BICYCLE_POLYNOMIC_FUNCTION_H_
#define _BICYCLE_POLYNOMIC_FUNCTION_H_

#include <cmath>
#include <initializer_list>

namespace bm {

	template <int N, typename T>
	struct PolynomicFunction {

		template <int N2, typename T2>
		friend struct PolynomicFunction;

		PolynomicFunction(T(&coefficients)[N]) {
			for (int i = 0; i < N; ++i) { m_coefficients[i] = T(coefficients[i]); }
		}

		T operator()(T const& arg) const {
			T res = m_coefficients[0];
			for (int i = 1; i < N; ++i) { res = std::fma(res, arg, m_coefficients[i]); }
			return res;
		}

		template <int N2>
		PolynomicFunction<(N + N2 - 1), T> operator*(PolynomicFunction<N2, T> const& other) const {
			T res_arr[N + N2 - 1] = { T() };

			for (int i = N - 1; i >= 0; --i) {
				for (int j = N2 - 1; j >= 0; --j) {
					res_arr[i + j] += m_coefficients[i] * other.m_coefficients[j];
				}
			}

			return PolynomicFunction<N + N2 - 1, T>(res_arr);
		}

		template <int N2>
		PolynomicFunction<(N > N2 ? N : N2), T> operator+(PolynomicFunction<N2, T> const& other) const {
			constexpr int maxN = N > N2 ? N : N2;
			T res_arr[maxN] = { T() };

			if (N > N2) {
				int const dN = N - N2;
				for (int i = N2 - 1; i >= 0; --i) { res_arr[i + dN] = m_coefficients[i + dN] + other.m_coefficients[i]; }
				for (int i = 0; i < dN; ++i) { res_arr[i] = m_coefficients[i]; }
			}
			else if (N2 > N) {
				int const dN = N2 - N;
				for (int i = N - 1; i >= 0; --i) { res_arr[i + dN] = m_coefficients[i] + other.m_coefficients[i + dN]; }
				for (int i = 0; i < dN; ++i) { res_arr[i] = other.m_coefficients[i]; }
			}
			else { 
				for (int i = 0; i < maxN; ++i) { res_arr[i] = m_coefficients[i] + other.m_coefficients[i]; }
			}

			return PolynomicFunction<maxN, T>(res_arr);
		}

		template <int N2>
		PolynomicFunction<(N > N2 ? N : N2), T> operator-(PolynomicFunction<N2, T> const& other) const {
			constexpr int maxN = N > N2 ? N : N2;
			T res_arr[maxN] = { T() };

			if (N > N2) {
				int const dN = N - N2;
				for (int i = N2 - 1; i >= 0; --i) { res_arr[i + dN] = m_coefficients[i + dN] - other.m_coefficients[i]; }
				for (int i = 0; i < dN; ++i) { res_arr[i] = m_coefficients[i]; }
			}
			else if (N2 > N) {
				int const dN = N2 - N;
				for (int i = N - 1; i >= 0; --i) { res_arr[i + dN] = m_coefficients[i] - other.m_coefficients[i + dN]; }
				for (int i = 0; i < dN; ++i) { res_arr[i] = -other.m_coefficients[i]; }
			}
			else {
				for (int i = 0; i < maxN; ++i) { res_arr[i] = m_coefficients[i] - other.m_coefficients[i]; }
			}

			return PolynomicFunction<maxN, T>(res_arr);
		}

		PolynomicFunction<N, T> operator*(T const &scale) const {
			T res_arr[N] = { T() };
			for (int i = 0; i < N; ++i) { res_arr[i] = m_coefficients[i] * scale; }
			return PolynomicFunction<N, T>(res_arr);
		}

		PolynomicFunction<N, T> operator+(T const& add) const {
			T res_arr[N] = { T() };
			for (int i = 0; i < N - 1; ++i) { res_arr[i] = m_coefficients[i]; }
			res_arr[N - 1] = m_coefficients[N - 1] + add;
			return PolynomicFunction<N, T>(res_arr);
		}

		PolynomicFunction<N, T> operator-(T const& sub) const {
			return operator+(-sub);
		}

	private:

		T m_coefficients[N];
	};

	template <int N, typename T>
	PolynomicFunction<N, T> operator*(T const& scale, PolynomicFunction<N, T> const& other) {
		return other * scale;
	}

	template <typename T = float>
	PolynomicFunction<2, T> getXfunc(T const& k = 1.0f, T const& b = 0.0f) {
		T arr[] = { k, b };
		return PolynomicFunction<2, T>(arr);
	}

}

#endif // !_BICYCLE_POLYNOMIC_FUNCTION_H_