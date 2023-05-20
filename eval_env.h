#ifndef EVAL_ENV_H
#define EVAL_ENV_H
#include<unordered_map>
#include "./value.h"

using ValuePtr = std::shared_ptr<Value>;

class EvalEnv {
public:
    ValuePtr eval(ValuePtr expr);
    static std::unordered_map<std::optional<std::string>, ValuePtr> myMap;
};
#endif
