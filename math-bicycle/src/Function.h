#ifndef _BICYCLE_FUNCTION_H_
#define _BICYCLE_FUNCTION_H_

#include <cmath>
#include <string>

namespace bm {

    template <typename InnerFunc, typename Func>
    class Function {

        InnerFunc inner_;
        Func f_;
        std::string name_;

        using RetT = decltype(std::declval<InnerFunc>()(0));

    public:

        Function(InnerFunc inner, Func f, std::string const& funcName)
            : inner_(inner), f_(f), name_(funcName) {}

        RetT operator()(RetT x) {
            return f_(inner_(x));
        }

        std::string  toString() const {
            return name_ + '(' + inner_.toString() + ')';
        }

    };

    #define DEFINE_FUNC(FUNC) \
    template <typename InnerFunc> \
    auto FUNC(InnerFunc f) { \
        using  RetT = decltype(std::declval<InnerFunc>()(0)); \
        RetT (*pF) (RetT) = std::FUNC; \
        return Function<InnerFunc, decltype(pF)>(f, pF, #FUNC); \
    }

    DEFINE_FUNC(sin);
    DEFINE_FUNC(cos); 
    DEFINE_FUNC(exp);
    DEFINE_FUNC(sqrt);
    DEFINE_FUNC(log);
    DEFINE_FUNC(log10);
    DEFINE_FUNC(sinh);
    DEFINE_FUNC(cosh);

}

#endif