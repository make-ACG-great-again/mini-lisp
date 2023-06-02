#ifndef BUILTINS_H
#define BUILTINS_H

#include<unordered_map>
#include<vector>
#include "./value.h"

using ValuePtr = std::shared_ptr<Value>;
using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr>&);

//算术运算库 - 9/9
ValuePtr my_add(const std::vector<ValuePtr>& params);
ValuePtr my_minus(const std::vector<ValuePtr>& params);
ValuePtr my_multiply(const std::vector<ValuePtr>& params);
ValuePtr my_division(const std::vector<ValuePtr>& params);
ValuePtr my_abs(const std::vector<ValuePtr>& params);
ValuePtr my_expt(const std::vector<ValuePtr>& params);
ValuePtr my_quotient(const std::vector<ValuePtr>& params);
ValuePtr my_remainder(const std::vector<ValuePtr>& params);
ValuePtr my_modulo(const std::vector<ValuePtr>& params);
//比较库 - 11/11
ValuePtr my_equal(const std::vector<ValuePtr>& params);
ValuePtr my_greater(const std::vector<ValuePtr>& params);
ValuePtr my_less(const std::vector<ValuePtr>& params);
ValuePtr my_no_greater(const std::vector<ValuePtr>& params);
ValuePtr my_no_less(const std::vector<ValuePtr>& params);
ValuePtr my_even(const std::vector<ValuePtr>& params);
ValuePtr my_odd(const std::vector<ValuePtr>& params);
ValuePtr my_zero(const std::vector<ValuePtr>& params);
ValuePtr my_eq(const std::vector<ValuePtr>& params);
ValuePtr my_equal_for_content(const std::vector<ValuePtr>& params);
ValuePtr my_not(const std::vector<ValuePtr>& params);
//核心库 - 4/8
ValuePtr my_print(const std::vector<ValuePtr>& params);
ValuePtr my_display(const std::vector<ValuePtr>& params);
ValuePtr my_exit(const std::vector<ValuePtr>& params);
ValuePtr my_newline(const std::vector<ValuePtr>& params);
//类型检查库 - 10/10
ValuePtr my_atom(const std::vector<ValuePtr>& params);
ValuePtr my_boolean(const std::vector<ValuePtr>& params);
ValuePtr my_integer(const std::vector<ValuePtr>& params);
ValuePtr my_list(const std::vector<ValuePtr>& params);
ValuePtr my_number(const std::vector<ValuePtr>& params);
ValuePtr my_null(const std::vector<ValuePtr>& params);
ValuePtr my_pair(const std::vector<ValuePtr>& params);
ValuePtr my_procedure(const std::vector<ValuePtr>& params);
ValuePtr my_string(const std::vector<ValuePtr>& params);
ValuePtr my_symbol(const std::vector<ValuePtr>& params);
//对子与列表操作库 - 9/9
ValuePtr my_car(const std::vector<ValuePtr>& params);
ValuePtr my_cdr(const std::vector<ValuePtr>& params);
ValuePtr my_cons(const std::vector<ValuePtr>& params);
ValuePtr my_length(const std::vector<ValuePtr>& params);
ValuePtr my_list_make(const std::vector<ValuePtr>& params);
ValuePtr my_append(const std::vector<ValuePtr>& params);
ValuePtr my_map(const std::vector<ValuePtr>& params);
ValuePtr my_filter(const std::vector<ValuePtr>& params);
ValuePtr my_reduce(const std::vector<ValuePtr>& params);

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
        std::string eq = "eq?";
        load_list[eq] = &my_eq;
        loadReference.push_back(eq);
        std::string equal_for_content = "equal?";
        load_list[equal_for_content] = &my_equal_for_content;
        loadReference.push_back(equal_for_content);
        std::string not_t = "not";
        load_list[not_t] = &my_not;
        loadReference.push_back(not_t);
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
        std::string boolean = "boolean?";
        load_list[boolean] = &my_boolean;
        loadReference.push_back(boolean);
        std::string integer = "integer?";
        load_list[integer] = &my_integer;
        loadReference.push_back(integer);
        std::string list = "list?";
        load_list[list] = &my_list;
        loadReference.push_back(list);
        std::string number = "number?";
        load_list[number] = &my_number;
        loadReference.push_back(number);
        std::string null = "null?";
        load_list[null] = &my_null;
        loadReference.push_back(null);
        std::string pair = "pair?";
        load_list[pair] = &my_pair;
        loadReference.push_back(pair);
        std::string procedure = "procedure?";
        load_list[procedure] = &my_procedure;
        loadReference.push_back(procedure);
        std::string string = "string?";
        load_list[string] = &my_string;
        loadReference.push_back(string);
        std::string symbol = "symbol?";
        load_list[symbol] = &my_symbol;
        loadReference.push_back(symbol);
        //对子与列表操作库
        std::string car = "car";
        load_list[car] = &my_car;
        loadReference.push_back(car);
        std::string cdr = "cdr";
        load_list[cdr] = &my_cdr;
        loadReference.push_back(cdr);
        std::string cons = "cons";
        load_list[cons] = &my_cons;
        loadReference.push_back(cons);
        std::string length = "length";
        load_list[length] = &my_length;
        loadReference.push_back(length);
        std::string list_make = "list";
        load_list[list_make] = &my_list_make;
        loadReference.push_back(list_make);
        std::string append = "append";
        load_list[append] = &my_append;
        loadReference.push_back(append);
        std::string map = "map";
        load_list[map] = &my_map;
        loadReference.push_back(map);
        std::string filter = "filter";
        load_list[filter] = &my_filter;
        loadReference.push_back(filter);
        std::string reduce = "reduce";
        load_list[reduce] = &my_reduce;
        loadReference.push_back(reduce);
        return loadReference;
    }
};

#endif
