#ifndef _BICYCLE_EXPOTENTIAL_FUNCTION_H_
#define _BICYCLE_EXPOTENTIAL_FUNCTION_H_

#include <cmath>

namespace bm {

	template <typename PowerFuncT>
	struct ExponentialFunction {

		using FuncT = decltype(std::declval<PowerFuncT>()(0));

		ExponentialFunction(PowerFuncT const& powerFunc)
			: m_powerFunc(powerFunc) { }

		FuncT operator()(FuncT const& arg) const {
			return std::exp(m_powerFunc(arg));
		}

	private:

		PowerFuncT const & m_powerFunc;

	};

	template <typename PowerFuncT>
	ExponentialFunction<PowerFuncT>
	exp(PowerFuncT const& powerFunc) {
		return ExponentialFunction<PowerFuncT>(powerFunc);
	}

}


#endif _BICYCLE_EXPOTENTIAL_FUNCTION_H_