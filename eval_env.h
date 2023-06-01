#ifndef EVAL_ENV_H
#define EVAL_ENV_H
#include<unordered_map>
#include<vector>
#include "./value.h"
#include "./builtins.h"

using ValuePtr = std::shared_ptr<Value>;

class EvalEnv : public std::enable_shared_from_this<EvalEnv> {
    EvalEnv() {
        // ValuePtr temp =  std::make_shared<BuiltinProcValue>(&my_add);
        auto loadList = builtin::load();
        for (std::string i : loadList) {
            ValuePtr temp =
                std::make_shared<BuiltinProcValue>(builtin::load_list[i]);
            this->defineBinding(i, temp);
        }
    }
public:
    static std::shared_ptr<EvalEnv> createGlobal() {
        return make_shared<EvalEnv>(EvalEnv());
    };
    std::shared_ptr<EvalEnv> parent;
    std::unordered_map<std::optional<std::string>, ValuePtr> myMap;
    ValuePtr eval(ValuePtr expr);
    std::vector<ValuePtr> evalList(ValuePtr expr);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr> args);
    ValuePtr lookupBinding(ValuePtr target);
    ValuePtr defineBinding(std::string target, ValuePtr content);
};
#endif
