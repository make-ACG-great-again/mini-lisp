#include<typeinfo>
#include<iostream>
#include<unordered_map>
#include "./error.h"
#include "./eval_env.h"
#include "./value.h"
using namespace std::literals;  // 使用 s 后缀

std::unordered_map<std::optional<std::string>, ValuePtr> EvalEnv::myMap;

ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (typeid(*expr) == typeid(BooleanValue) ||
        typeid(*expr) == typeid(NumericValue) || 
        typeid(*expr) == typeid(StringValue)) {
        //std::cout << "direct out" << std::endl;
        //if (expr == nullptr) std::cout << "error in direct out" << std::endl;
        return expr;
    } else if (typeid(*expr) == typeid(NilValue)) {
        throw LispError("Evaluating nil is prohibited.");
        return ValuePtr(new NilValue());
    } else if (typeid(*expr) == typeid(PairValue)) {
        std::vector<ValuePtr> v = expr->toVector();
        if (v[0]->asSymbol() == "define"s) {
            if (auto name = v[1]->asSymbol()) {
                auto tem = v[2];
                while (auto te = myMap[tem->asSymbol()]) {
                    tem = te;
                }
                myMap[name] = tem;
                //std::cout << v[1]->toString() << tem->toString() << std::endl;
                //if (name == std::nullopt) std::cout << "null";
                //std::cout <<"new symbol recorded" << std::endl;
                return ValuePtr(new NilValue());
            } else if (auto name = expr->asSymbol()) {
                if (auto value = myMap[name]) {
                    /*std::cout << "get symbol's value from recording"
                              << std::endl;
                    if (value == nullptr) std::cout << "error in search" << std::endl;*/
                    return value;
                } else {
                    throw LispError("Variable " + *name + " not defined.");
                    return ValuePtr(new NilValue());
                }
            }
            else {
                throw LispError("Malformed define.");
                return ValuePtr(new NilValue());
            }
        }
        v.clear();
    } else if (typeid(*expr) == typeid(SymbolValue)) {
        if (auto value = myMap[expr->asSymbol()]) {
            return value;
        } else {
            std::cout << expr->toString();
            throw LispError("Variable " + expr->toString() + " not defined.");
            return ValuePtr(new NilValue());
        }
    } else if (typeid(*expr) == typeid(BuiltinProcValue)) {
        return expr;
    }else {
        throw LispError("Unimplemented");
        return ValuePtr(new NilValue());
    }
}
