#include<iostream>
#include "./builtins.h"
#include "./value.h"
#include "./error.h"

ValuePtr my_add(const std::vector<ValuePtr>& params) {
    int result = 0;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result += i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
};

ValuePtr my_print(const std::vector<ValuePtr>& params){
    for (auto i : params) {
        std::cout << i->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
};

std::unordered_map<std::string, BuiltinFuncType*> builtin::load_list;
std::vector<std::string> builtin::loadReference;
