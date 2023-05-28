#include<iostream>
#include<cmath>
#include<unordered_map>
#include <algorithm>
#include <iterator>
#include<typeinfo>
#include "./eval_env.h"
#include "./builtins.h"
#include "./value.h"
#include "./error.h"

using SpecialFormType = ValuePtr(const std::vector<ValuePtr>&, EvalEnv&);
extern const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS;

//算术运算库
ValuePtr my_add(const std::vector<ValuePtr>& params) {
    double result = 0;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result += i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
};

ValuePtr my_minus(const std::vector<ValuePtr>& params){
    double result = 0;
    if (params.size() == 1) {
        if (!(params[0]->isNumber()))
            throw LispError("Cannot put a non-numeric value.");
        result -= params[0]->asNumber();
    } else if (params.size() != 2) {
        throw LispError("wrong num of arguments.");
    } else {
        if (!params[0]->isNumber()) {
            throw LispError("Cannot put a non-numeric value.");
        }
        result += params[0]->asNumber();
        if (!params[1]->isNumber()) {
            throw LispError("Cannot minus a non-numeric value.");
        }
        result -= params[1]->asNumber();
    }
    return std::make_shared<NumericValue>(result);
};

ValuePtr my_multiply(const std::vector<ValuePtr>& params) {
    double result = 1;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("Cannot multiply a non-numeric value.");
        }
        result *= i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
};

ValuePtr my_division(const std::vector<ValuePtr>& params) {
    double result = 1;
    if (params.size() == 1) {
        if (!(params[0]->isNumber()))
            throw LispError("Cannot put a non-numeric value.");
        if (params[0]->asNumber() == 0)
            throw LispError("Cannot be divided by zero.");
        result /= params[0]->asNumber();
    }else if (params.size() != 2) {
        throw LispError("wrong num of arguments.");
    } else {
        if (!params[0]->isNumber()) {
            throw LispError("Cannot put a non-numeric value.");
        }
        result = params[0]->asNumber();
        if (!params[1]->isNumber()) {
            throw LispError("Cannot divide a non-numeric value.");
        }
        if (params[1]->asNumber() == 0)
            throw LispError("Cannot be divided by zero.");
        result /= params[1]->asNumber();
    }
    return std::make_shared<NumericValue>(result);
};

ValuePtr my_abs(const std::vector<ValuePtr>& params){
    if (params.size() != 1) {
        throw LispError("wrong num of arguments.");
    };
    if (!params[0]->isNumber()) {
        throw LispError("Cannot put a non-numeric value.");
    }
    double result = abs(params[0]->asNumber());
    return std::make_shared<NumericValue>(result);
};

ValuePtr my_expt(const std::vector<ValuePtr>& params){
    if (params.size() != 2) throw LispError("wrong num of arguments.");
    if (!params[0]->isNumber() || !params[1]->isNumber())
        throw LispError("Cannot put a non-numeric value.");
    if (params[0]->asNumber() == 0 && params[1]->asNumber() == 0)
        throw LispError("Cannot compute pow(0, 0).");
    double result = pow(params[0]->asNumber(), params[1]->asNumber());
    return std::make_shared<NumericValue>(result);
};

ValuePtr my_quotient(const std::vector<ValuePtr>& params){
    if (params.size() != 2) throw LispError("wrong num of arguments.");
    if (!params[0]->isNumber() || !params[1]->isNumber())
        throw LispError("Cannot put a non-numeric value.");
    double temp = params[0]->asNumber() / params[1]->asNumber();
    int result = temp / 1;
    return std::make_shared<NumericValue>(result);
};

ValuePtr my_remainder(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) throw LispError("wrong num of arguments.");
    if (!params[0]->isNumber() || !params[1]->isNumber())
        throw LispError("Cannot put a non-numeric value.");
    /*int a = params[0]->asNumber() * 10000;
    int b = params[1]->asNumber() * 10000;
    int temp = a % b;
    double result = temp / 10000;*/
    if (params[0]->asNumber() / 1 != 0 || params[1]->asNumber() / 1 != 0)
        throw LispError("Cannot put a non-integer value.");
    int result = int(params[0]->asNumber()) % int(params[1]->asNumber());
    return std::make_shared<NumericValue>(result);
};

ValuePtr my_modulo(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) throw LispError("wrong num of arguments.");
    if (!params[0]->isNumber() || !params[1]->isNumber())
        throw LispError("Cannot put a non-numeric value.");
    /*int a = params[0]->asNumber() * 10000;
    int b = params[1]->asNumber() * 10000;
    int temp = a % b;
    double result = temp / 10000;*/
    double a = params[0]->asNumber();
    double b = params[1]->asNumber();
    double result = std::fmod(a, b);
    return std::make_shared<NumericValue>(result);
};

//比较库
ValuePtr my_equal(const std::vector<ValuePtr>& params){
    if (params.size() != 2) {
        throw LispError("wrong num of arguments.");
    }
    if (!params[0]->isNumber()) {
        throw LispError("Cannot put a non-numeric value.");
    }
    if (!params[1]->isNumber()) {
        throw LispError("Cannot put a non-numeric value.");
    }
    return std::make_shared<BooleanValue>(params[0]->asNumber() == params[1]->asNumber());
};

ValuePtr my_greater(const std::vector<ValuePtr>& params){
    if (params.size() != 2) {
        throw LispError("wrong num of arguments.");
    }
    if (!params[0]->isNumber()) {
        throw LispError("Cannot put a non-numeric value.");
    }
    if (!params[1]->isNumber()) {
        throw LispError("Cannot put a non-numeric value.");
    }
    return std::make_shared<BooleanValue>(params[0]->asNumber() >
                                          params[1]->asNumber());
};

ValuePtr my_less(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("wrong num of arguments.");
    }
    if (!params[0]->isNumber()) {
        throw LispError("Cannot put a non-numeric value.");
    }
    if (!params[1]->isNumber()) {
        throw LispError("Cannot put a non-numeric value.");
    }
    return std::make_shared<BooleanValue>(params[0]->asNumber() <
                                          params[1]->asNumber());
};

ValuePtr my_no_greater(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("wrong num of arguments.");
    }
    if (!params[0]->isNumber()) {
        throw LispError("Cannot put a non-numeric value.");
    }
    if (!params[1]->isNumber()) {
        throw LispError("Cannot put a non-numeric value.");
    }
    return std::make_shared<BooleanValue>(params[0]->asNumber() <=
                                          params[1]->asNumber());
};

ValuePtr my_no_less(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("wrong num of arguments.");
    }
    if (!params[0]->isNumber()) {
        throw LispError("Cannot put a non-numeric value.");
    }
    if (!params[1]->isNumber()) {
        throw LispError("Cannot put a non-numeric value.");
    }
    return std::make_shared<BooleanValue>(params[0]->asNumber() >=
                                          params[1]->asNumber());
};

ValuePtr my_even(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("wrong num of arguments.");
    };
    if (!params[0]->isNumber()) {
        throw LispError("Cannot put a non-numeric value.");
    }
    return std::make_shared<BooleanValue>(!(params[0]->asNumber() / 2));
};

ValuePtr my_odd(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("wrong num of arguments.");
    };
    if (!params[0]->isNumber()) {
        throw LispError("Cannot put a non-numeric value.");
    }
    return std::make_shared<BooleanValue>(params[0]->asNumber() / 2);
};

ValuePtr my_zero(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("wrong num of arguments.");
    };
    if (!params[0]->isNumber()) {
        throw LispError("Cannot put a non-numeric value.");
    }
    return std::make_shared<BooleanValue>(params[0]->asNumber() == 0);
};

//核心库
ValuePtr my_print(const std::vector<ValuePtr>& params){
    for (auto i : params) {
        std::cout << i->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
};

ValuePtr my_display(const std::vector<ValuePtr>& params){
    for (auto i : params) {
        if (typeid(*i) == typeid(StringValue)) {
            const StringValue* temp = dynamic_cast<StringValue*>(&(*i));
            std::cout << temp->c;
        } else
            std::cout << i->toString();
    }
    return std::make_shared<NilValue>();
};

ValuePtr my_exit(const std::vector<ValuePtr>& params) {
    double n = 0;
    if (params.size() > 1) throw LispError("wrong num of arguments.");
    if (params.size() == 1 && params[0]->isNumber()) {
        n = params[0]->asNumber();
    } else
        throw LispError("Cannot exit with a non-numeric value.");
    std::cout << "Program terminated with exit(" << n << ')';
    std::exit(n);
};

ValuePtr my_newline(const std::vector<ValuePtr>& params) {
    std::cout << std::endl;
    return std::make_shared<NilValue>();
};

//类型检查库
ValuePtr my_atom(const std::vector<ValuePtr>& params){
    if (params.size() != 1) throw LispError("wrong num of arguments.");
    if (typeid(*params[0]) == typeid(BooleanValue) ||
        typeid(*params[0]) == typeid(NumericValue) ||
        typeid(*params[0]) == typeid(StringValue) ||
        typeid(*params[0]) == typeid(NilValue()))
        return std::make_shared<BooleanValue>(true);
    else if (typeid(*params[0]) == typeid(SymbolValue)) {
        if (auto value = EvalEnv::myMap[params[0]->asSymbol()]) return std::make_shared<BooleanValue>(true);
        else if (SPECIAL_FORMS.count(params[0]->asSymbol().value()) > 0)
            return std::make_shared<BooleanValue>(false);
        else {
            throw LispError("Variable " + params[0]->toString() +
                            " not defined.");
            return ValuePtr(new NilValue());
        }
    } else
        return std::make_shared<BooleanValue>(false);
};

std::unordered_map<std::string, BuiltinFuncType*> builtin::load_list;
std::vector<std::string> builtin::loadReference;
