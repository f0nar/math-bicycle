#include <cmath>
#include <gtest/gtest.h>
#include "../src/PolynomicFunction.h"

bm::Xf const X;
float const precission = 1e-3f, precission_div_2 = precission / 2.0f;

template <int Pow, typename T> 
T pol(T const(&coefficients)[Pow + 1], T const& x) {
	T res = coefficients[Pow];
	for (int i = 0; i < Pow; ++i) { res += coefficients[i] * std::pow(x, Pow - i); }
	return res;
}

TEST(PolynomicFunctionTest, ResTest) {
	int const N = 4;
	float const coefficients[N] = { -55.8f, -63.25f, 17.39f, 20.f };
	float const args[N] = { 5.f, 8.f, 3.3f, 11.f };
	bm::PolynomicFunction<N - 1, float> f(coefficients);
	for (int i = 0; i < N; ++i) { EXPECT_NEAR(f(args[i]), pol<N - 1>(coefficients, args[i]), precission); }
}

TEST(PolynomicFunctionTest, AddScalar) {
	int const N = 4;
	float const coefficients[N] = { -55.8f, -63.25f, 17.39f, 20.f };
	float const scalar = 93.7f;
	auto f = bm::PolynomicFunction<N - 1, float>(coefficients) + scalar;
	EXPECT_NEAR(f(0.f), coefficients[N - 1] + scalar, precission);
}

TEST(PolynomicFunctionTest, MultiplyScalar) {
	int const N = 4;
	float const coefficients[N] = { -55.8f, -63.25f, 17.39f, 20.f };
	float const args[N] = { 5.f, 8.f, 3.3f, 11.f };
	float const scalar = 93.7f;
	float res_coefficients[N];
	for (int i = 0; i < N; ++i) { res_coefficients[i] = coefficients[i] * scalar; }

	bm::PolynomicFunction<N - 1, float> f(coefficients);
	bm::PolynomicFunction<N - 1, float> res_array_f(res_coefficients);
	bm::PolynomicFunction<N - 1, float> scaled_f = f * scalar;

	for (int i = 0; i < N; ++i) { EXPECT_NEAR(scaled_f(args[i]), res_array_f(args[i]), precission); }
}

TEST(PolynomicFunctionTest, AddPolynomicFunction) {
	int const N1 = 4;
	int const N2 = 5;
	int const MaxN = std::max(N1, N2);
	float const coefficients1[N1] = {	   -55.8f, -63.25f,  17.39f, 20.f };
	float const coefficients2[N2] = { 15.f, 73.1f, -63.25f, -333.2f, 20.f };
	float const args[N1] = { 5.f, 8.f, 3.3f, 11.f };
	float res_coefficients[MaxN];
	const int dN = N2 - N1;
	for (int i = 0; i < N1; ++i) { res_coefficients[i + dN] = coefficients1[i] + coefficients2[i + dN]; }
	for (int i = 0; i < dN; ++i) { res_coefficients[i] = coefficients2[i]; }

	bm::PolynomicFunction<N1 - 1, float> f1(coefficients1);
	bm::PolynomicFunction<N2 - 1, float> f2(coefficients2);
	bm::PolynomicFunction<MaxN - 1, float> res_array_f(res_coefficients), res_f = f1 + f2;;

	for (int i = 0; i < N1; ++i) {
		EXPECT_NEAR(res_f(args[i]), res_array_f(args[i]), precission);
	}
}

TEST(PolynomicFunctionTest, MultiplicationTest) {
	int const N = 4;
	float zeros[N] = { 0.f, 1.f, -1.f, 3.f };
	auto f = (X - zeros[0]) * (X - zeros[1]) * (X - zeros[2]) * (X - zeros[3]);
	for (int i = 0; i < N; ++i) { EXPECT_NEAR(f(zeros[i]), 0.f, precission); }
}