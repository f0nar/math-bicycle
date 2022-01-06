#ifndef _BICYCLE_POLYNOMIC_FUNCTION_H_
#define _BICYCLE_POLYNOMIC_FUNCTION_H_

#include <cmath>
#include <string>
#include <initializer_list>

namespace bm {

	#define MAX(A, B) (A > B ? A : B)

	#define POL_FUNC_POW(N1, N2) (N1 + N2)


	template <typename T>
	struct Function {
		virtual std::string toString() const = 0;
		virtual T operator()(T const& arg) const = 0;
	};

	template <int N, typename T>
	struct PolynomicFunction : Function<T> {

		template <int N2, typename T2>
		friend struct PolynomicFunction;

		PolynomicFunction(T const (&coefficients)[N + 1]) {
			for (int i = 0; i <= N; ++i) { m_coefficients[i] = T(coefficients[i]); }
		}

		T operator()(T const& arg) const {
			T res = m_coefficients[0];
			for (int i = 1; i <= N; ++i) { res = std::fma(res, arg, m_coefficients[i]); }
			return res;
		}

		template <int N2>
		PolynomicFunction<POL_FUNC_POW(N, N2), T> operator*(PolynomicFunction<N2, T> const& other) const {
			int const newN = POL_FUNC_POW(N, N2);
			T res_arr[newN + 1] = { T() };

			for (int i = N; i >= 0; --i) {
				for (int j = N2; j >= 0; --j) {
					res_arr[i + j] += m_coefficients[i] * other.m_coefficients[j];
				}
			}

			return PolynomicFunction<newN, T>(res_arr);
		}

		template <int N2>
		PolynomicFunction<MAX(N, N2), T> operator+(PolynomicFunction<N2, T> const& other) const {
			constexpr int maxN = MAX(N, N2);
			T res_arr[maxN + 1] = { T() };

			if (N > N2) {
				int const dN = N - N2;
				for (int i = N2; i >= 0; --i) { res_arr[i + dN] = m_coefficients[i + dN] + other.m_coefficients[i]; }
				for (int i = 0; i < dN; ++i) { res_arr[i] = m_coefficients[i]; }
			}
			else if (N2 > N) {
				int const dN = N2 - N;
				for (int i = N; i >= 0; --i) { res_arr[i + dN] = m_coefficients[i] + other.m_coefficients[i + dN]; }
				for (int i = 0; i < dN; ++i) { res_arr[i] = other.m_coefficients[i]; }
			}
			else { 
				for (int i = 0; i <= maxN; ++i) { res_arr[i] = m_coefficients[i] + other.m_coefficients[i]; }
			}

			return PolynomicFunction<maxN, T>(res_arr);
		}

		template <int N2>
		PolynomicFunction<MAX(N, N2), T> operator-(PolynomicFunction<N2, T> const& other) const {
			constexpr int maxN = MAX(N, N2);
			T res_arr[maxN + 1] = { T() };

			if (N > N2) {
				int const dN = N - N2;
				for (int i = N2; i >= 0; --i) { res_arr[i + dN] = m_coefficients[i + dN] - other.m_coefficients[i]; }
				for (int i = 0; i < dN; ++i) { res_arr[i] = m_coefficients[i]; }
			}
			else if (N2 > N) {
				int const dN = N2 - N;
				for (int i = N; i >= 0; --i) { res_arr[i + dN] = m_coefficients[i] - other.m_coefficients[i + dN]; }
				for (int i = 0; i < dN; ++i) { res_arr[i] = -other.m_coefficients[i]; }
			}
			else {
				for (int i = 0; i < maxN; ++i) { res_arr[i] = m_coefficients[i] - other.m_coefficients[i]; }
			}

			return PolynomicFunction<maxN, T>(res_arr);
		}

		PolynomicFunction<N, T> operator*(T const &scale) const {
			T res_arr[N + 1] = { T() };
			for (int i = 0; i <= N; ++i) { res_arr[i] = m_coefficients[i] * scale; }
			return PolynomicFunction<N, T>(res_arr);
		}

		PolynomicFunction<N, T> operator+(T const& add) const {
			T res_arr[N + 1] = { T() };
			for (int i = 0; i < N; ++i) { res_arr[i] = m_coefficients[i]; }
			res_arr[N] = m_coefficients[N] + add;
			return PolynomicFunction<N, T>(res_arr);
		}

		PolynomicFunction<N, T> operator-(T const& sub) const {
			return operator+(-sub);
		}

		std::string toString() const {
			auto to_string_without_trailing_zeros = [](float num) {
				std::string num_str = std::to_string(num);
				auto last_not_triling_zero_pos = num_str.find_last_not_of('0');
				auto erase_start =
					last_not_triling_zero_pos < num_str.length() && num_str.at(last_not_triling_zero_pos) == '.' ?
					last_not_triling_zero_pos : last_not_triling_zero_pos + 1;
				num_str.erase(erase_start, std::string::npos);
				return num_str;
			};
			auto continue_polynom_string = [&to_string_without_trailing_zeros](std::string& src, T const& coef, int pow) {
				if (!coef) return;
				if (coef > 0) src += '+';
				src += to_string_without_trailing_zeros(coef);
				if (pow > 0) {
					src += "*X";
					if (pow > 1) src += "^" + to_string_without_trailing_zeros(pow);
				}
			};

			std::string resStr;
			for (int i = 0, in = N + 1; i < in; ++i) {
				continue_polynom_string(resStr, m_coefficients[i], N - i);
			}
			return resStr;
		}

	private:

		T m_coefficients[N + 1];
	};

	template <int N, typename T>
	PolynomicFunction<N, T> operator*(T const& scale, PolynomicFunction<N, T> const& other) {
		return other * scale;
	}

	template <typename T>
	struct X_ : PolynomicFunction<1, T> {
		X_() : PolynomicFunction<1, T>::PolynomicFunction({ 1, 0 }) { }
	};


	template <typename T>
	struct One_ : PolynomicFunction<0, T> {
		One_() : PolynomicFunction<0, T>::PolynomicFunction({ 1 }) { }
	};

	using Xf = X_<float>;
	using Xd = X_<double>;

	using Onef = One_<float>;
	using Oned = One_<double>;

}

#endif // !_BICYCLE_POLYNOMIC_FUNCTION_H_