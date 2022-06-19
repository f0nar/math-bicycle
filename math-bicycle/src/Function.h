#ifndef _BICYCLE_FUNCTION_H_
#define _BICYCLE_FUNCTION_H_

#include <cmath>
#include <string>

namespace bm {

    #define MULTIPLIER Multiplier
    #define ADDER Adder
    #define DIVIDOR Dividor

    #define DEFINE_FUNC_OPERATOR(NAME, OP) \
    template <typename LeftFunc, typename RightFunc> \
    class NAME { \
        LeftFunc left_; \
        RightFunc right_; \
        using ValT = decltype(std::declval<LeftFunc>()(0) OP std::declval<RightFunc>()(0)); \
    public: \
        NAME(LeftFunc const& left, RightFunc const& right) : left_(left), right_(right) {} \
        auto operator()(ValT const& arg) const { return left_(arg) OP right_(arg); } \
        template <typename RightFunc> auto operator *(RightFunc const& right) const { return MULTIPLIER <NAME, RightFunc>(*this, right); } \
        template <typename RightFunc> auto operator +(RightFunc const& right) const { return ADDER <NAME, RightFunc>(*this, right); } \
        template <typename RightFunc> auto operator /(RightFunc const& right) const { return DIVIDOR <NAME, RightFunc>(*this, right); } \
        std::string toString() const { return '(' + left_.toString() + ')' + #OP + '(' + right_.toString() + ')'; } \
    };

    DEFINE_FUNC_OPERATOR(MULTIPLIER, *);
    DEFINE_FUNC_OPERATOR(ADDER, +);
    DEFINE_FUNC_OPERATOR(DIVIDOR, /);

    template <typename InnerFunc, typename Func>
    class Function {

        InnerFunc inner_;
        Func f_;
        std::string name_;

        using RetT = decltype(std::declval<InnerFunc>()(0));

    public:

        Function(InnerFunc inner, Func f, std::string const& funcName)
            : inner_(inner), f_(f), name_(funcName) {}

        RetT operator()(RetT x) const {
            return f_(inner_(x));
        }

        std::string  toString() const {
            return name_ + '(' + inner_.toString() + ')';
        }

        template <typename RightFunc>
        auto operator*(RightFunc const &right) const {
            return Multiplier<Function, RightFunc>(*this, right);
        }

        template <typename RightFunc>
        auto operator+(RightFunc const& right) const {
            return Adder<Function, RightFunc>(*this, right);
        }

        template <typename RightFunc>
        auto operator/(RightFunc const& right) const {
            return Dividor<Function, RightFunc>(*this, right);
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