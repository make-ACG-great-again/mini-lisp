#ifndef BUILTINS_H
#define BUILTINS_H

#include<unordered_map>
#include<vector>
#include "./value.h"

using ValuePtr = std::shared_ptr<Value>;
using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr>&);

ValuePtr my_add(const std::vector<ValuePtr>& params);
ValuePtr my_print(const std::vector<ValuePtr>& params);

class builtin {
public:
    static std::unordered_map<std::string, BuiltinFuncType*> load_list;
    static std::vector<std::string> loadReference;
    static std::vector<std::string> load() {
        std::string add = "+";
        load_list[add] = &my_add;
        loadReference.push_back(add);
        std::string print = "print";
        load_list[print] = &my_print;
        loadReference.push_back(print);
        return loadReference;
    }
};

#endif
