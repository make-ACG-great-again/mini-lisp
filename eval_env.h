#ifndef EVAL_ENV_H
#define EVAL_ENV_H
#include<unordered_map>
#include<vector>
#include "./value.h"
#include "./builtins.h"

using ValuePtr = std::shared_ptr<Value>;

extern std::unordered_map<std::string, BuiltinFuncType*> load_list;

class EvalEnv {
public:
    static std::unordered_map<std::optional<std::string>, ValuePtr> myMap;
    EvalEnv() {
        //ValuePtr temp =  std::make_shared<BuiltinProcValue>(&my_add);
        auto loadList = builtin::load();
        for (std::string i : loadList) {
            ValuePtr temp = std::make_shared<BuiltinProcValue>(builtin::load_list[i]);
            myMap[i] = temp;
        }
    }
    ValuePtr eval(ValuePtr expr);
    std::vector<ValuePtr> evalList(ValuePtr expr);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr> args);
};
#endif
