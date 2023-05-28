#include<typeinfo>
#include<iostream>
#include<unordered_map>
#include <algorithm>
#include <iterator>
#include "./error.h"
#include "./eval_env.h"
#include "./value.h"
using namespace std::literals;  // 使用 s 后缀


std::unordered_map<std::optional<std::string>, ValuePtr> EvalEnv::myMap;
using SpecialFormType = ValuePtr(const std::vector<ValuePtr>&, EvalEnv&);
extern const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS;

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
        if (auto name = v[0]->asSymbol()){
            std::string s = {}; 
            if (SPECIAL_FORMS.count(name.value()) > 0) {
                s = name.value();
                //std::cout << s << std::endl;
                SpecialFormType* deal_with = SPECIAL_FORMS.find(s)->second;
                return deal_with(v, *this);
            } else {
                ValuePtr proc = this->eval(v[0]);
                const PairValue* tempptr = dynamic_cast<PairValue*>(&(*expr));
                std::vector<ValuePtr> args = this->evalList(tempptr->right());
                v.clear();
                return this->apply(proc, args);
            }
        //} else{ // if (auto name = v[0]->asSymbol()) {
                //if (auto value = myMap[name]) {
                //    /*std::cout << "get symbol's value from recording"
                //              << std::endl;
                //    if (value == nullptr) std::cout << "error in search" << std::endl;*/
                //    return value;
                //} else {
                //    throw LispError("Variable " + *name + " not defined.");
                //    return ValuePtr(new NilValue());
                //}
            /*ValuePtr proc = this->eval(v[0]);
            const PairValue* tempptr =
                dynamic_cast<PairValue*>(&(*expr));
            std::vector<ValuePtr> args = this->evalList(tempptr->right());
            v.clear();
            return this->apply(proc, args);*/
        }else {
                throw LispError("undefined operate.");
                return ValuePtr(new NilValue());
        }
    } else if (typeid(*expr) == typeid(SymbolValue)) {
        if (auto value = myMap[expr->asSymbol()]) {
            return value;
        } else {
            //std::cout << expr->toString();
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

std::vector<ValuePtr> EvalEnv::evalList(ValuePtr expr) {
    std::vector<ValuePtr> result;
    std::ranges::transform(expr->toVector(), std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(v); });
    return result;
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr> args) {
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        // 调用内置过程
        const BuiltinProcValue* tempptr =
            dynamic_cast<BuiltinProcValue*>(&(*proc));
        return tempptr->procedure(args);
    } else {
        throw LispError("Unimplemented");
    }
}
