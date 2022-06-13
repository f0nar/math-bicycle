#ifndef _BICYCLE_FUNCTION_H_
#define _BICYCLE_FUNCTION_H_

#include <cmath>
#include <string>

namespace bm {

    template <typename T>
    struct Function {
        virtual std::string toString() const = 0;
        virtual T operator()(T const& arg) const = 0;
    };

    template <typename FuncT>
    class CompositeFunction : public Function<decltype(std::declval<FuncT>()(0))>
    {

        using ValT = decltype(std::declval<FuncT>()(0));
        Function<ValT> const &m_innerFunc;
        FuncT m_func;
        std::string m_funcName;

    public:

        CompositeFunction(Function<ValT> const& inner, FuncT f, std::string const &funcName)
            : m_innerFunc(inner), m_func(f), m_funcName(funcName) { }

        virtual ValT operator()(ValT const &x) const override {
            return m_func(m_innerFunc(x));
        }

        virtual std::string toString() const override {
            return m_funcName + '(' + m_innerFunc.toString() + ')';
        }

    };

    #define DEFINE_FUNC(FUNC) \
    template <typename T> \
    auto FUNC(Function<T> const &f) { \
        T (*pF) (T) = std::FUNC; \
        return CompositeFunction<decltype(pF)>(f, pF, #FUNC); \
    }

    DEFINE_FUNC(sin);
    DEFINE_FUNC(cos); 
    DEFINE_FUNC(exp);

}

#endif