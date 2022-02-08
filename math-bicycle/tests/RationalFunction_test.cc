#include <gtest/gtest.h>
#include "../src/PolynomicFunction.h"
#include "../src/RationalFunction.h"

using bm::PolynomicFunction;
using bm::RationalFunction;

float const precission = 1e-3f, precission_div_2 = precission / 2.0f;

TEST(RationalFunctionTest, PolynomicFunctionsDivisionTest) {
	float const args[] = { 5.f, 8.f, 3.3f, 11.f };
	float const numerator_coefficients[] = { -55.8f, -63.25f,  17.39f, 20.f };
	float const denominator_coefficients[] = { 15.f, 73.1f, -63.25f, -333.2f, 20.f };
	PolynomicFunction<3, float> numerator(numerator_coefficients);
	PolynomicFunction<4, float> denominator(denominator_coefficients);
	RationalFunction rationalFunction = numerator / denominator;

	for (int i = 0; i < 4; ++i) { EXPECT_NEAR(rationalFunction(args[i]), numerator(args[i]) / denominator(args[i]), precission); }
}

TEST(RationalFunctionTest, RationalFunctionAdditionTest) {
	float const args[] = { 5.f, 8.f, 3.3f, 11.f };
	float const numerator_coefficients1[] = { -55.8f, -63.25f,  17.39f, 20.f };
	float const denominator_coefficients1[] = { 15.f, 73.1f, -63.25f, -333.2f, 20.f };
	float const numerator_coefficients2[] = { 35.8f, 73.f,  4.993f };
	float const denominator_coefficients2[] = { 144.39f, 1.f, 25.63f, -993.7f, 20.f, 29.56f };
	PolynomicFunction<3, float> numerator1(numerator_coefficients1);
	PolynomicFunction<2, float> numerator2(numerator_coefficients2);
	PolynomicFunction<4, float> denominator1(denominator_coefficients1);
	PolynomicFunction<5, float> denominator2(denominator_coefficients2);
	RationalFunction rationalFunction1(numerator1, denominator1);
	RationalFunction rationalFunction2(numerator2, denominator2);
	RationalFunction resultedRationalFunction = rationalFunction1 + rationalFunction2;

	auto resultedRationalFunctionsImitator = [&numerator1, &numerator2, &denominator1, &denominator2]
	(float arg) {
		return
			(numerator1(arg) * denominator2(arg) + numerator2(arg) * denominator1(arg)) /
			(denominator1(arg) * denominator2(arg));
	};

	for (int i = 0; i < 4; ++i) {
		auto const arg = args[i];
		EXPECT_NEAR(resultedRationalFunction(arg), resultedRationalFunctionsImitator(arg), precission);
	}
}

TEST(RationalFunctionTest, RationalFunctionSubstractionTest) {
	float const args[] = { 5.f, 8.f, 3.3f, 11.f };
	float const numerator_coefficients1[] = { -55.8f, -63.25f,  17.39f, 20.f };
	float const denominator_coefficients1[] = { 15.f, 73.1f, -63.25f, -333.2f, 20.f };
	float const numerator_coefficients2[] = { 35.8f, 73.f,  4.993f };
	float const denominator_coefficients2[] = { 144.39f, 1.f, 25.63f, -993.7f, 20.f, 29.56f };
	PolynomicFunction<3, float> numerator1(numerator_coefficients1);
	PolynomicFunction<2, float> numerator2(numerator_coefficients2);
	PolynomicFunction<4, float> denominator1(denominator_coefficients1);
	PolynomicFunction<5, float> denominator2(denominator_coefficients2);
	RationalFunction rationalFunction1(numerator1, denominator1);
	RationalFunction rationalFunction2(numerator2, denominator2);
	RationalFunction resultedRationalFunction = rationalFunction1 - rationalFunction2;

	auto resultedRationalFunctionsImitator = [&numerator1, &numerator2, &denominator1, &denominator2]
	(float arg) {
		return
			(numerator1(arg) * denominator2(arg) - numerator2(arg) * denominator1(arg)) /
			(denominator1(arg) * denominator2(arg));
	};

	for (int i = 0; i < 4; ++i) {
		auto const arg = args[i];
		EXPECT_NEAR(resultedRationalFunction(arg), resultedRationalFunctionsImitator(arg), precission);
	}
}

TEST(RationalFunctionTest, RationalFunctionMultiplicationTest) {
	float const args[] = { 5.f, 8.f, 3.3f, 11.f };
	float const numerator_coefficients1[] = { -55.8f, -63.25f,  17.39f, 20.f };
	float const denominator_coefficients1[] = { 15.f, 73.1f, -63.25f, -333.2f, 20.f };
	float const numerator_coefficients2[] = { 35.8f, 73.f,  4.993f };
	float const denominator_coefficients2[] = { 144.39f, 1.f, 25.63f, -993.7f, 20.f, 29.56f };
	PolynomicFunction<3, float> numerator1(numerator_coefficients1);
	PolynomicFunction<2, float> numerator2(numerator_coefficients2);
	PolynomicFunction<4, float> denominator1(denominator_coefficients1);
	PolynomicFunction<5, float> denominator2(denominator_coefficients2);
	RationalFunction rationalFunction1(numerator1, denominator1);
	RationalFunction rationalFunction2(numerator2, denominator2);
	RationalFunction resultedRationalFunction = rationalFunction1 * rationalFunction2;

	auto resultedRationalFunctionsImitator = [&numerator1, &numerator2, &denominator1, &denominator2]
	(float arg) {
		return
			(numerator1(arg) * numerator2(arg)) /
			(denominator1(arg) * denominator2(arg));
	};

	for (int i = 0; i < 4; ++i) {
		auto const arg = args[i];
		EXPECT_NEAR(resultedRationalFunction(arg), resultedRationalFunctionsImitator(arg), precission);
	}
}

TEST(RationalFunctionTest, RationalFunctionDivisionTest) {
	float const args[] = { 5.f, 8.f, 3.3f, 11.f };
	float const numerator_coefficients1[] = { -55.8f, -63.25f,  17.39f, 20.f };
	float const denominator_coefficients1[] = { 15.f, 73.1f, -63.25f, -333.2f, 20.f };
	float const numerator_coefficients2[] = { 35.8f, 73.f,  4.993f };
	float const denominator_coefficients2[] = { 144.39f, 1.f, 25.63f, -993.7f, 20.f, 29.56f };
	PolynomicFunction<3, float> numerator1(numerator_coefficients1);
	PolynomicFunction<2, float> numerator2(numerator_coefficients2);
	PolynomicFunction<4, float> denominator1(denominator_coefficients1);
	PolynomicFunction<5, float> denominator2(denominator_coefficients2);
	RationalFunction rationalFunction1(numerator1, denominator1);
	RationalFunction rationalFunction2(numerator2, denominator2);
	RationalFunction resultedRationalFunction = rationalFunction1 / rationalFunction2;

	auto resultedRationalFunctionsImitator = [&numerator1, &numerator2, &denominator1, &denominator2]
	(float arg) {
		return
			(numerator1(arg) * denominator2(arg)) /
			(denominator1(arg) * numerator2(arg));
	};

	for (int i = 0; i < 4; ++i) {
		auto const arg = args[i];
		EXPECT_NEAR(resultedRationalFunction(arg), resultedRationalFunctionsImitator(arg), precission);
	}
}

TEST(RationalFunctionTest, PolynomicFunctionDivisionTest) {
	float const args[] = { 5.f, 8.f, 3.3f, 11.f };
	float const numerator_coefficients[] = { -55.8f, -63.25f,  17.39f, 20.f };
	float const denominator_coefficients[] = { 15.f, 73.1f, -63.25f, -333.2f, 20.f };
	float const polynomic_coefficients[] = { 35.8f, 73.f,  4.993f };
	PolynomicFunction<3, float> numerator(numerator_coefficients);
	PolynomicFunction<4, float> denominator(denominator_coefficients);
	PolynomicFunction<2, float> polynomic(polynomic_coefficients);
	RationalFunction rationalFunction(numerator, denominator);
	RationalFunction resultedRationalFunction = rationalFunction / polynomic;

	auto resultedRationalFunctionsImitator = [&numerator, &denominator, &polynomic]
	(float arg) {
		return
			numerator(arg) /
			(denominator(arg) * polynomic(arg));
	};

	for (int i = 0; i < 4; ++i) {
		auto const arg = args[i];
		EXPECT_NEAR(resultedRationalFunction(arg), resultedRationalFunctionsImitator(arg), precission);
	}
}

TEST(RationalFunctionTest, PolynomicFunctionMultiplicationTest) {
	float const args[] = { 5.f, 8.f, 3.3f, 11.f };
	float const numerator_coefficients[] = { -55.8f, -63.25f,  17.39f, 20.f };
	float const denominator_coefficients[] = { 15.f, 73.1f, -63.25f, -333.2f, 20.f };
	float const polynomic_coefficients[] = { 35.8f, 73.f,  4.993f };
	PolynomicFunction<3, float> numerator(numerator_coefficients);
	PolynomicFunction<4, float> denominator(denominator_coefficients);
	PolynomicFunction<2, float> polynomic(polynomic_coefficients);
	RationalFunction rationalFunction(numerator, denominator);
	RationalFunction resultedRationalFunction = rationalFunction * polynomic;

	auto resultedRationalFunctionsImitator = [&numerator, &denominator, &polynomic]
	(float arg) {
		return
			(numerator(arg) * polynomic(arg)) /
			denominator(arg);
	};

	for (int i = 0; i < 4; ++i) {
		auto const arg = args[i];
		EXPECT_NEAR(resultedRationalFunction(arg), resultedRationalFunctionsImitator(arg), precission);
	}
}

TEST(RationalFunctionTest, PolynomicFunctionAdditionTest) {
	float const args[] = { 5.f, 8.f, 3.3f, 11.f };
	float const numerator_coefficients[] = { -55.8f, -63.25f,  17.39f, 20.f };
	float const denominator_coefficients[] = { 15.f, 73.1f, -63.25f, -333.2f, 20.f };
	float const polynomic_coefficients[] = { 35.8f, 73.f,  4.993f };
	PolynomicFunction<3, float> numerator(numerator_coefficients);
	PolynomicFunction<4, float> denominator(denominator_coefficients);
	PolynomicFunction<2, float> polynomic(polynomic_coefficients);
	RationalFunction rationalFunction(numerator, denominator);
	RationalFunction resultedRationalFunction = rationalFunction + polynomic;

	auto resultedRationalFunctionsImitator = [&numerator, &denominator, &polynomic]
	(float arg) {
		return
			(numerator(arg) +  polynomic(arg) * denominator(arg)) /
			denominator(arg);
	};

	for (int i = 0; i < 4; ++i) {
		auto const arg = args[i];
		EXPECT_NEAR(resultedRationalFunction(arg), resultedRationalFunctionsImitator(arg), precission);
	}
}

TEST(RationalFunctionTest, PolynomicFunctionSubstractionTest) {
	float const args[] = { 5.f, 8.f, 3.3f, 11.f };
	float const numerator_coefficients[] = { -55.8f, -63.25f,  17.39f, 20.f };
	float const denominator_coefficients[] = { 15.f, 73.1f, -63.25f, -333.2f, 20.f };
	float const polynomic_coefficients[] = { 35.8f, 73.f,  4.993f };
	PolynomicFunction<3, float> numerator(numerator_coefficients);
	PolynomicFunction<4, float> denominator(denominator_coefficients);
	PolynomicFunction<2, float> polynomic(polynomic_coefficients);
	RationalFunction rationalFunction(numerator, denominator);
	RationalFunction resultedRationalFunction = rationalFunction - polynomic;

	auto resultedRationalFunctionsImitator = [&numerator, &denominator, &polynomic]
	(float arg) {
		return
			(numerator(arg) - polynomic(arg) * denominator(arg)) /
			denominator(arg);
	};

	for (int i = 0; i < 4; ++i) {
		auto const arg = args[i];
		EXPECT_NEAR(resultedRationalFunction(arg), resultedRationalFunctionsImitator(arg), precission);
	}
}

TEST(RationalFunctionTest, ScalarMultiplicationTest) {
	float const args[] = { 5.f, 8.f, 3.3f, 11.f };
	float const numerator_coefficients[] = { -55.8f, -63.25f,  17.39f, 20.f };
	float const denominator_coefficients[] = { 15.f, 73.1f, -63.25f, -333.2f, 20.f };
	PolynomicFunction<3, float> numerator(numerator_coefficients);
	PolynomicFunction<4, float> denominator(denominator_coefficients);
	float const scalar = 18.32;
	RationalFunction rationalFunction(numerator, denominator);
	RationalFunction resultedRationalFunction = rationalFunction * scalar;

	auto resultedRationalFunctionsImitator = [&numerator, &denominator, scalar]
	(float arg) {
		return
			(numerator(arg) * scalar) /
			denominator(arg);
	};

	for (int i = 0; i < 4; ++i) {
		auto const arg = args[i];
		EXPECT_NEAR(resultedRationalFunction(arg), resultedRationalFunctionsImitator(arg), precission);
	}
}

TEST(RationalFunctionTest, ScalarDivisionTest) {
	float const args[] = { 5.f, 8.f, 3.3f, 11.f };
	float const numerator_coefficients[] = { -55.8f, -63.25f,  17.39f, 20.f };
	float const denominator_coefficients[] = { 15.f, 73.1f, -63.25f, -333.2f, 20.f };
	PolynomicFunction<3, float> numerator(numerator_coefficients);
	PolynomicFunction<4, float> denominator(denominator_coefficients);
	float const scalar = 18.32;
	RationalFunction rationalFunction(numerator, denominator);
	RationalFunction resultedRationalFunction = rationalFunction / scalar;

	auto resultedRationalFunctionsImitator = [&numerator, &denominator, scalar]
	(float arg) {
		return
			numerator(arg) /
			(denominator(arg) * scalar);
	};

	for (int i = 0; i < 4; ++i) {
		auto const arg = args[i];
		EXPECT_NEAR(resultedRationalFunction(arg), resultedRationalFunctionsImitator(arg), precission);
	}
}