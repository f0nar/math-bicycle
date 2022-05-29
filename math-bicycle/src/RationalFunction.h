#ifndef _BICYCLE_RATIONAL_FUNCTION_H_
#define _BICYCLE_RATIONAL_FUNCTION_H_

#include <array>
#include "Vector.h"
#include "Matrix.h"
#include "PolynomicFunction.h"

namespace bm {


	#define BINARY_OPERATOR(OP, RETURN_T, OTHER_T, NEW_NUMERATOR, NEW_DENOMINATOR) \
	RETURN_T operator OP(OTHER_T const& other) const { \
		return RETURN_T( \
			NEW_NUMERATOR, \
			NEW_DENOMINATOR \
		);\
	}

	template <typename T, int NUMERATOR, int DENOMINATOR = 0>
	struct RationalFunction {

		template <typename T2, int NUMERATOR2, int DENOMINATOR2>
		friend struct RationalFunction;

		RationalFunction(
			T const (&numerator)[NUMERATOR + 1],
			T const (&denominator)[DENOMINATOR + 1]
		) : m_numerator(numerator), m_denominator(denominator) { }

		RationalFunction(
			PolynomicFunction<NUMERATOR, T> const& numerator,
			PolynomicFunction<DENOMINATOR, T> const& denominator = One_<T>()
		) : m_numerator(numerator), m_denominator(denominator) { }

		T operator()(T const& parameter) const {
			return m_numerator(parameter) / m_denominator(parameter);
		}

		#define RATIONAL_FUNCTION_MULTIPLICATION_RES_T RationalFunction<T, POL_FUNC_POW(NUMERATOR, NUMERATOR_2), POL_FUNC_POW(DENOMINATOR, DENOMINATOR_2)>
		#define RATIONAL_FUNCTION_DIVISION_RES_T RationalFunction<T, POL_FUNC_POW(NUMERATOR, DENOMINATOR_2), POL_FUNC_POW(DENOMINATOR, NUMERATOR_2)>
		#define RATIONAL_FUNCTION_ADDITION_RES_T RationalFunction<T, MAX(POL_FUNC_POW(NUMERATOR, DENOMINATOR_2), POL_FUNC_POW(NUMERATOR_2, DENOMINATOR)), POL_FUNC_POW(DENOMINATOR_2, DENOMINATOR)>
		#define SCALAR_MULTIPLICATION_RES_T RationalFunction<T, NUMERATOR, DENOMINATOR>
		#define POLYNOMIC_FUNCTION_MULTIPLICATION_RES_T RationalFunction<T, POL_FUNC_POW(NUMERATOR, POL_FUNC_NUMERATOR), DENOMINATOR>
		#define POLYNOMIC_FUNCTION_DIVISION_RES_T RationalFunction<T, NUMERATOR, POL_FUNC_POW(POL_FUNC_NUMERATOR, DENOMINATOR)>
		#define POLYNOMIC_FUNCTION_ADDITION_RES_T RationalFunction<T, MAX(NUMERATOR, POL_FUNC_POW(POL_FUNC_NUMERATOR, DENOMINATOR)), DENOMINATOR>
		#define RATIONAL_FUNCTION_PARAMETER_T RationalFunction <T, NUMERATOR_2, DENOMINATOR_2>
		#define POLYNOMIC_FUNCTION_PARAMETER_T PolynomicFunction<POL_FUNC_NUMERATOR, T>

		template <int NUMERATOR_2, int DENOMINATOR_2>
		BINARY_OPERATOR(*, RATIONAL_FUNCTION_MULTIPLICATION_RES_T, RATIONAL_FUNCTION_PARAMETER_T, m_numerator * other.m_numerator, m_denominator * other.m_denominator);

		template <int NUMERATOR_2, int DENOMINATOR_2>
		BINARY_OPERATOR(/, RATIONAL_FUNCTION_DIVISION_RES_T, RATIONAL_FUNCTION_PARAMETER_T, m_numerator * other.m_denominator, m_denominator * other.m_numerator);

		template <int NUMERATOR_2, int DENOMINATOR_2>
		BINARY_OPERATOR(+, RATIONAL_FUNCTION_ADDITION_RES_T, RATIONAL_FUNCTION_PARAMETER_T, m_numerator * other.m_denominator + other.m_numerator * m_denominator, m_denominator * other.m_denominator);

		template <int NUMERATOR_2, int DENOMINATOR_2>
		BINARY_OPERATOR(-, RATIONAL_FUNCTION_ADDITION_RES_T, RATIONAL_FUNCTION_PARAMETER_T, m_numerator * other.m_denominator - other.m_numerator * m_denominator, m_denominator * other.m_denominator);

		template <int POL_FUNC_NUMERATOR>
		BINARY_OPERATOR(*, POLYNOMIC_FUNCTION_MULTIPLICATION_RES_T, POLYNOMIC_FUNCTION_PARAMETER_T, m_numerator * other, m_denominator);

		template <int POL_FUNC_NUMERATOR>
		BINARY_OPERATOR(/, POLYNOMIC_FUNCTION_DIVISION_RES_T, POLYNOMIC_FUNCTION_PARAMETER_T, m_numerator, m_denominator * other);

		template <int POL_FUNC_NUMERATOR>
		BINARY_OPERATOR(+, POLYNOMIC_FUNCTION_ADDITION_RES_T, POLYNOMIC_FUNCTION_PARAMETER_T, m_numerator + other * m_denominator, m_denominator);

		template <int POL_FUNC_NUMERATOR>
		BINARY_OPERATOR(-, POLYNOMIC_FUNCTION_ADDITION_RES_T, POLYNOMIC_FUNCTION_PARAMETER_T, m_numerator - other * m_denominator, m_denominator);

		BINARY_OPERATOR(*, SCALAR_MULTIPLICATION_RES_T, T, m_numerator * other, m_denominator);

		BINARY_OPERATOR(/, SCALAR_MULTIPLICATION_RES_T, T, m_numerator, m_denominator * other);

		#undef RATIONAL_FUNCTION_MULTIPLICATION_RES_T
		#undef RATIONAL_FUNCTION_DIVISION_RES_T
		#undef RATIONAL_FUNCTION_ADDITION_RES_T
		#undef RATIONAL_FUNCTION_SCALAR_MULTIPLICATION_RES_T
		#undef POLYNOMIC_FUNCTION_MULTIPLICATION_RES_T
		#undef POLYNOMIC_FUNCTION_DIVISION_RES_T
		#undef RATIONAL_FUNCTION_PARAMETER_T
		#undef POLYNOMIC_FUNCTION_PARAMETER_T
		#undef POLYNOMIC_FUNCTION_ADDITION_RES_T

		std::string toString() const {
			std::string const numeratorString = m_numerator.toString();
			std::string const denominatorString = m_denominator.toString();
			int const dividorLength = std::max(numeratorString.size(), denominatorString.size());
			std::string const dividorString = std::string(dividorLength, '-');
			return 
				numeratorString + "\n" +
				dividorString + "\n" +
				denominatorString;
		}

	private:

		PolynomicFunction<NUMERATOR, T> m_numerator;
		PolynomicFunction<DENOMINATOR, T> m_denominator;

	};

	template <typename T, int NUMERATOR, int DENOMINATOR>
	RationalFunction<T, NUMERATOR, DENOMINATOR> operator/(PolynomicFunction<NUMERATOR, T> const& numerator, PolynomicFunction<DENOMINATOR, T> const& denominator) {
		return RationalFunction<T, NUMERATOR, DENOMINATOR>(
			numerator,
			denominator
		);
	}

	template <typename T, int NUMERATOR, int DENOMINATOR>
	RationalFunction<T, NUMERATOR, DENOMINATOR> fitToRat(std::array<Vector<2, T>, (NUMERATOR + DENOMINATOR + 1)> const& data) {
		int const Dimension = NUMERATOR + DENOMINATOR + 1;
		Matrix<Dimension, Dimension, T> coefMat;
		Vector<Dimension, T> resVec;
		for (int i = 0; i < Dimension; ++i) {
			int j = 0;
			for (; j < NUMERATOR; ++j) {
				coefMat[i][j] = std::pow(data[i].x, NUMERATOR - j - 1);
			}
			for (; j < Dimension; ++j) {
				coefMat[i][j] = -std::pow(data[i].x, DENOMINATOR - (j - NUMERATOR)) * data[i].y;
			}
			resVec[i] = -std::pow(data[i].x, NUMERATOR);
		}
		Vector<Dimension, T> resCoef = coefMat.inv() * resVec;
		T numCoef[NUMERATOR + 1] = { T { 1 } };
		T denomCoef[DENOMINATOR + 1];
		for (int i = 0; i < NUMERATOR; ++i) {
			numCoef[i + 1] = resCoef.at(i);
		}
		for (int i = 0; i <= DENOMINATOR; ++i) {
			denomCoef[i] = resCoef.at(NUMERATOR + i);
		}
		return RationalFunction<T, NUMERATOR, DENOMINATOR>(numCoef, denomCoef);
	}


	#undef BINARY_OPERATOR

}

#endif // !_BICYCLE_RATIONAL_FUNCTION_H_