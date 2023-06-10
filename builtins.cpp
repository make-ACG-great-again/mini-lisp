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
    double a = params[0]->asNumber();
    double b = params[1]->asNumber();
    double result = std::fmod(a, b);
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
    int temp = std::floor(params[0]->asNumber() / params[1]->asNumber());
    double result = params[0]->asNumber() - params[1]->asNumber() * temp;
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
    double temp = params[0]->asNumber();
    if (temp != int(temp)) throw LispError("wrong type of param.");
    return std::make_shared<BooleanValue>(!(int(temp) % 2));
};

ValuePtr my_odd(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("wrong num of arguments.");
    };
    if (!params[0]->isNumber()) {
        throw LispError("Cannot put a non-numeric value.");
    }
    double temp = params[0]->asNumber();
    if (temp != int(temp)) throw LispError("wrong type of param.");
    return std::make_shared<BooleanValue>(int(temp) % 2);
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

ValuePtr my_eq(const std::vector<ValuePtr>& params){
    if (params.size() != 2) {
        throw LispError("wrong num of arguments.");
    }
    if (typeid(*params[0]) == typeid(BooleanValue) ||
        typeid(*params[0]) == typeid(NumericValue) ||
        typeid(*params[0]) == typeid(SymbolValue) ||
        typeid(*params[0]) == typeid(NilValue)) {
        if (params[0]->toString() == params[1]->toString())
            return std::make_shared<BooleanValue>(true);
        else
            return std::make_shared<BooleanValue>(false);
    } else {
        if (params[0] == params[1])
            return std::make_shared<BooleanValue>(true);
        else
            return std::make_shared<BooleanValue>(false);
    }
};

ValuePtr my_equal_for_content(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("wrong num of arguments.");
    }
    if (typeid(params[0]) != typeid(params[1]))
        return std::make_shared<BooleanValue>(false);
    if (*params[0] == params[1])
        return std::make_shared<BooleanValue>(true);
    else
        return std::make_shared<BooleanValue>(false);
};

ValuePtr my_not(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("wrong num of arguments.");
    }
    if (params[0]->toString() == "#f" || params[0]->toString() == "false")
        return std::make_shared<BooleanValue>(true);
    else
        return std::make_shared<BooleanValue>(false);
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
    if (params.size() == 1)
        if (params[0]->isNumber())
            n = params[0]->asNumber();
        else
            throw LispError("Cannot exit with a non-numeric value.");
    else
        n = 0;
    std::cout << "Program terminated with exit(" << n << ')';
    std::exit(n);
};

ValuePtr my_newline(const std::vector<ValuePtr>& params) {
    std::cout << std::endl;
    return std::make_shared<NilValue>();
};

ValuePtr my_error(const std::vector<ValuePtr>& params) {
    double n = 0;
    if (params.size() > 1) throw LispError("wrong num of arguments.");
    if (params.size() == 1) {
        throw LispError(params[0]->toString());
    } else
        throw LispError("");
};

ValuePtr my_apply(const std::vector<ValuePtr>& params){
    if (params.size() != 2) throw LispError("wrong num of arguments.");
    ValuePtr proc = params[0];
    ValuePtr list = params[1];
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        const BuiltinProcValue* tempptr =
            dynamic_cast<BuiltinProcValue*>(&(*proc));
        return tempptr->procedure(list->toVector());
    } else if (typeid(*proc) == typeid(LambdaValue)) {
        const LambdaValue* tempptr = dynamic_cast<LambdaValue*>(&(*proc));
        return tempptr->apply(list->toVector());
    } else {
        throw LispError("wrong type of proc.");
    }
};

ValuePtr my_displayln(const std::vector<ValuePtr>& params){
    if (params.size() != 1)
        throw LispError("wrong num of arguments.");
    my_display(std::vector<ValuePtr>{params[0]});
    my_newline(std::vector<ValuePtr>{(ValuePtr(new NilValue))});
    return ValuePtr(new NilValue);
};

ValuePtr my_eval(const std::vector<ValuePtr>& params){
    if (params.size() != 1)
        throw LispError("wrong num of arguments.");
    ValuePtr expr = params[0];
    if (EvalEnv::eval_environment.size() == 0)
        throw LispError("Cannot find eval-environment.");
    else {
        EvalEnv& temp_env = *EvalEnv::eval_environment.back();
        EvalEnv::eval_environment.pop_back();
        return temp_env.eval(expr);
    }
};

//类型检查库
ValuePtr my_atom(const std::vector<ValuePtr>& params){
    if (params.size() != 1) throw LispError("wrong num of arguments.");
    if (typeid(*params[0]) == typeid(BooleanValue) ||
        typeid(*params[0]) == typeid(NumericValue) ||
        typeid(*params[0]) == typeid(StringValue) ||
        typeid(*params[0]) == typeid(NilValue))
        return std::make_shared<BooleanValue>(true);
    else if (typeid(*params[0]) == typeid(SymbolValue)) {
        return std::make_shared<BooleanValue>(true);
    } else
        return std::make_shared<BooleanValue>(false);
};

ValuePtr my_boolean(const std::vector<ValuePtr>& params){
    if (params.size() != 1) throw LispError("wrong num of arguments.");
    if (typeid(*params[0]) == typeid(BooleanValue))
        return std::make_shared<BooleanValue>(true);
    else
        return std::make_shared<BooleanValue>(false);
};

ValuePtr my_integer(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) throw LispError("wrong num of arguments.");
    if (typeid(*params[0]) == typeid(NumericValue)) {
        if (params[0]->asNumber() == int(params[0]->asNumber()))
            return std::make_shared<BooleanValue>(true);
        else
            return std::make_shared<BooleanValue>(false);
    }else return std::make_shared<BooleanValue>(false);
};

ValuePtr my_list(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) throw LispError("wrong num of arguments.");
    if (typeid(*params[0]) == typeid(NilValue))
        return std::make_shared<BooleanValue>(true);
    else if (typeid(*params[0]) == typeid(PairValue)) {
        ValuePtr temp = params[0];
        while (typeid(*temp) == typeid(PairValue)) {
            PairValue* temporary = dynamic_cast<PairValue*>(&(*temp));
            temp = temporary->right();
        };
        if (typeid(*temp) != typeid(NilValue))
            return std::make_shared<BooleanValue>(false);
        else
            return std::make_shared<BooleanValue>(true);
    } else return std::make_shared<BooleanValue>(false);
};

ValuePtr my_number(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) throw LispError("wrong num of arguments.");
    if (typeid(*params[0]) == typeid(NumericValue))
        return std::make_shared<BooleanValue>(true);
    else
        return std::make_shared<BooleanValue>(false);
};

ValuePtr my_null(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) throw LispError("wrong num of arguments.");
    if (typeid(*params[0]) == typeid(NilValue))
        return std::make_shared<BooleanValue>(true);
    else
        return std::make_shared<BooleanValue>(false);
};

ValuePtr my_pair(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) throw LispError("wrong num of arguments.");
    if (typeid(*params[0]) == typeid(PairValue))
        return std::make_shared<BooleanValue>(true);
    else
        return std::make_shared<BooleanValue>(false);
};

ValuePtr my_procedure(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) throw LispError("wrong num of arguments.");
    if (typeid(*params[0]) == typeid(BuiltinProcValue) ||
        typeid(*params[0]) == typeid(LambdaValue))
        return std::make_shared<BooleanValue>(true);
    else
        return std::make_shared<BooleanValue>(false);
};

ValuePtr my_string(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) throw LispError("wrong num of arguments.");
    if (typeid(*params[0]) == typeid(StringValue))
        return std::make_shared<BooleanValue>(true);
    else
        return std::make_shared<BooleanValue>(false);
};

ValuePtr my_symbol(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) throw LispError("wrong num of arguments.");
    if (typeid(*params[0]) == typeid(SymbolValue))
        return std::make_shared<BooleanValue>(true);
    else
        return std::make_shared<BooleanValue>(false);
};

//对子与列表操作库
ValuePtr my_car(const std::vector<ValuePtr>& params){
    if (params.size() != 1) throw LispError("wrong num of arguments.");
    if (typeid(*params[0]) != typeid(PairValue))
        throw LispError("wrong type of arguments.");
    const PairValue* temp = dynamic_cast<PairValue*>(&(*params[0]));
    return temp->left();
};

ValuePtr my_cdr(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) throw LispError("wrong num of arguments.");
    if (typeid(*params[0]) != typeid(PairValue))
        throw LispError("wrong type of arguments.");
    const PairValue* temp = dynamic_cast<PairValue*>(&(*params[0]));
    return temp->right();
};

ValuePtr my_cons(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) throw LispError("wrong num of arguments.");
    return ValuePtr(new PairValue(params[0], params[1]));
};

ValuePtr my_length(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) throw LispError("wrong num of arguments.");
    if (typeid(*params[0]) == typeid(NilValue))
        return std::make_shared<NumericValue>(0);
    if (typeid(*params[0]) != typeid(PairValue))
        throw LispError("wrong type of arguments.");
    int len = 1;
    ValuePtr temp = params[0];
    while (typeid(*temp) == typeid(PairValue)) {
        PairValue* temporary = dynamic_cast<PairValue*>(&(*temp));
        temp = temporary->right();
        len++;
    };
    if (typeid(*temp) == typeid(NilValue)) len--;
    return std::make_shared<NumericValue>(len);
};

ValuePtr my_list_make(const std::vector<ValuePtr>& params) {
    if (params.size() == 0) return ValuePtr(new NilValue);
    int num = params.size();
    ValuePtr temp = ValuePtr(new PairValue(params[num - 1], ValuePtr(new NilValue)));
    if (params.size() == 1) return temp;
    ValuePtr temporary;
    for (int i = num - 2; i >= 0; i--) {
        temporary = ValuePtr(new PairValue(params[i], temp));
        temp = temporary;
    }
    return temp;
};

ValuePtr my_append(const std::vector<ValuePtr>& params) {
    if (params.size() == 0) return ValuePtr(new NilValue);
    int num = params.size();
    std::vector<ValuePtr> temp;
    for (int i = 0; i < num; i++) {
        std::vector<ValuePtr> temporary = params[i]->toVector();
        temp.insert(temp.end(), temporary.begin() , temporary.end());
    };
    return my_list_make(temp);
};

ValuePtr my_map(const std::vector<ValuePtr>& params){
    if (params.size() != 2) throw LispError("wrong num of arguments.");
    ValuePtr proc = params[0];
    ValuePtr list = params[1];
    if (typeid(*list) == typeid(NilValue)) return ValuePtr(new NilValue);
    if (typeid(*list) != typeid(PairValue))
        throw LispError("wrong type of list.");
    std::vector<ValuePtr> lists = list->toVector();
    std::vector<ValuePtr> temp;
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        const BuiltinProcValue* tempptr =
            dynamic_cast<BuiltinProcValue*>(&(*proc));
        for (int i = 0; i < lists.size(); i++)
            temp.push_back(tempptr->procedure(std::vector<ValuePtr>{lists[i]}));
        return my_list_make(temp);
    } else if (typeid(*proc) == typeid(LambdaValue)) {
        const LambdaValue* tempptr = dynamic_cast<LambdaValue*>(&(*proc));
        for (int i = 0; i < lists.size(); i++)
            temp.push_back(tempptr->apply(std::vector<ValuePtr>{lists[i]}));
        return my_list_make(temp);
    } else {
        throw LispError("wrong type of proc.");
    }
};

ValuePtr my_filter(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) throw LispError("wrong num of arguments.");
    ValuePtr proc = params[0];
    ValuePtr list = params[1];
    if (typeid(*list) == typeid(NilValue)) return ValuePtr(new NilValue);
    if (typeid(*list) != typeid(PairValue))
        throw LispError("wrong type of list.");
    std::vector<ValuePtr> lists = list->toVector();
    std::vector<ValuePtr> temp;
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        const BuiltinProcValue* tempptr =
            dynamic_cast<BuiltinProcValue*>(&(*proc));
        for (int i = 0; i < lists.size(); i++)
            if (tempptr->procedure(std::vector<ValuePtr>{lists[i]})->toString() != "#f")
                temp.push_back(lists[i]);
        return my_list_make(temp);
    } else if (typeid(*proc) == typeid(LambdaValue)) {
        const LambdaValue* tempptr = dynamic_cast<LambdaValue*>(&(*proc));
        for (int i = 0; i < lists.size(); i++)
            if (tempptr->apply(std::vector<ValuePtr>{lists[i]})->toString() != "#f")
                temp.push_back(lists[i]);
        return my_list_make(temp);
    } else {
        throw LispError("wrong type of proc.");
    }
};

ValuePtr my_reduce(const std::vector<ValuePtr>& params){
    if (params.size() != 2) throw LispError("wrong num of arguments.");
    ValuePtr proc = params[0];
    ValuePtr list = params[1];
    if (typeid(*list) != typeid(PairValue))
        throw LispError("wrong type of list.");
    std::vector<ValuePtr> lists = list->toVector();
    if (lists.size() == 1) return lists[0];
    std::vector<ValuePtr> temp{my_car(std::vector<ValuePtr>{list}),
                               my_reduce(std::vector<ValuePtr>{proc, my_cdr(std::vector<ValuePtr>{list})})};
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        const BuiltinProcValue* tempptr =
            dynamic_cast<BuiltinProcValue*>(&(*proc));
        return tempptr->procedure(temp);
    } else if (typeid(*proc) == typeid(LambdaValue)) {
        const LambdaValue* tempptr = dynamic_cast<LambdaValue*>(&(*proc));
        return tempptr->apply(temp);
    } else {
        throw LispError("wrong type of proc.");
    }
};

std::unordered_map<std::string, BuiltinFuncType*> builtin::load_list;
std::vector<std::string> builtin::loadReference;
