#ifndef BUILTINS_H
#define BUILTINS_H

#include<unordered_map>
#include<vector>
#include "./value.h"

using ValuePtr = std::shared_ptr<Value>;
using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr>&);
//算术运算库
ValuePtr my_add(const std::vector<ValuePtr>& params);
ValuePtr my_minus(const std::vector<ValuePtr>& params);
ValuePtr my_multiply(const std::vector<ValuePtr>& params);
ValuePtr my_division(const std::vector<ValuePtr>& params);
ValuePtr my_abs(const std::vector<ValuePtr>& params);
ValuePtr my_expt(const std::vector<ValuePtr>& params);
ValuePtr my_quotient(const std::vector<ValuePtr>& params);
ValuePtr my_remainder(const std::vector<ValuePtr>& params);
ValuePtr my_modulo(const std::vector<ValuePtr>& params);
//比较库
ValuePtr my_equal(const std::vector<ValuePtr>& params);
ValuePtr my_greater(const std::vector<ValuePtr>& params);
ValuePtr my_less(const std::vector<ValuePtr>& params);
ValuePtr my_no_greater(const std::vector<ValuePtr>& params);
ValuePtr my_no_less(const std::vector<ValuePtr>& params);
ValuePtr my_even(const std::vector<ValuePtr>& params);
ValuePtr my_odd(const std::vector<ValuePtr>& params);
ValuePtr my_zero(const std::vector<ValuePtr>& params);
//核心库
ValuePtr my_print(const std::vector<ValuePtr>& params);
ValuePtr my_display(const std::vector<ValuePtr>& params);
ValuePtr my_exit(const std::vector<ValuePtr>& params);
ValuePtr my_newline(const std::vector<ValuePtr>& params);
//类型检查库
ValuePtr my_atom(const std::vector<ValuePtr>& params);

class builtin {
public:
    static std::unordered_map<std::string, BuiltinFuncType*> load_list;
    static std::vector<std::string> loadReference;
    static std::vector<std::string> load() {
        //算术运算库
        std::string add = "+";
        load_list[add] = &my_add;
        loadReference.push_back(add);
        std::string minus = "-";
        load_list[minus] = &my_minus;
        loadReference.push_back(minus);
        std::string multiply = "*";
        load_list[multiply] = &my_multiply;
        loadReference.push_back(multiply);
        std::string division = "/";
        load_list[division] = &my_division;
        loadReference.push_back(division);
        std::string abs = "abs";
        load_list[abs] = &my_abs;
        loadReference.push_back(abs);
        std::string expt = "expt";
        load_list[expt] = &my_expt;
        loadReference.push_back(expt);
        std::string quotient = "quotient";
        load_list[quotient] = &my_quotient;
        loadReference.push_back(quotient);
        std::string remainder = "remainder";
        load_list[remainder] = &my_remainder;
        loadReference.push_back(remainder);
        std::string modulo = "modulo";
        load_list[modulo] = &my_modulo;
        loadReference.push_back(modulo);
        //比较库
        std::string equal = "=";
        load_list[equal] = &my_equal;
        loadReference.push_back(equal);
        std::string greater = ">";
        load_list[greater] = &my_greater;
        loadReference.push_back(greater);
        std::string less = "<";
        load_list[less] = &my_less;
        loadReference.push_back(less);
        std::string no_greater = "<=";
        load_list[no_greater] = &my_no_greater;
        loadReference.push_back(no_greater);
        std::string no_less = ">=";
        load_list[no_less] = &my_no_less;
        loadReference.push_back(no_less);
        std::string even = "even?";
        load_list[even] = &my_even;
        loadReference.push_back(even);
        std::string odd = "odd?";
        load_list[odd] = &my_odd;
        loadReference.push_back(odd);
        std::string zero = "zero?";
        load_list[zero] = &my_zero;
        loadReference.push_back(zero);
        //核心库
        std::string print = "print";
        load_list[print] = &my_print;
        loadReference.push_back(print);
        std::string display = "display";
        load_list[display] = &my_display;
        loadReference.push_back(display);
        std::string exit = "exit";
        load_list[exit] = &my_exit;
        loadReference.push_back(exit);
        std::string newline = "newline";
        load_list[newline] = &my_newline;
        loadReference.push_back(newline);
        //类型检查库
        std::string atom = "atom?";
        load_list[atom] = &my_atom;
        loadReference.push_back(atom);
        return loadReference;
    }
};

#endif
