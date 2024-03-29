#include<typeinfo>
#include<iostream>
#include<unordered_map>
#include <algorithm>
#include <iterator>
#include "./error.h"
#include "./eval_env.h"
#include "./value.h"
using namespace std::literals;  // 使用 s 后缀

using SpecialFormType = ValuePtr(const std::vector<ValuePtr>&, EvalEnv&);
extern const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS;
std::vector<std::shared_ptr<EvalEnv>> EvalEnv::eval_environment{};

ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (typeid(*expr) == typeid(BooleanValue) ||
        typeid(*expr) == typeid(NumericValue) || 
        typeid(*expr) == typeid(StringValue)) {
        //std::cout << "direct out" << std::endl;
        //if (expr == nullptr) std::cout << "error in direct out" << std::endl;
        return expr;
    } else if (typeid(*expr) == typeid(NilValue)) {
        throw LispError("Evaluating nil is prohibited.");
        return std::make_shared<NilValue>();
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
                if (v[0]->asSymbol() == "eval")
                    EvalEnv::eval_environment.push_back(
                        this->shared_from_this());
                v.clear();
                auto temp = this->apply(proc, args);
                return temp;
            }
        //} else{ // if (auto name = v[0]->asSymbol()) {
                //if (auto value = myMap[name]) {
                //    /*std::cout << "get symbol's value from recording"
                //              << std::endl;
                //    if (value == nullptr) std::cout << "error in search" << std::endl;*/
                //    return value;
                //} else {
                //    throw LispError("Variable " + *name + " not defined.");
                //    return std::make_shared<NilValue>();
                //}
            /*ValuePtr proc = this->eval(v[0]);
            const PairValue* tempptr =
                dynamic_cast<PairValue*>(&(*expr));
            std::vector<ValuePtr> args = this->evalList(tempptr->right());
            v.clear();
            return this->apply(proc, args);*/
        }else {
                throw LispError("undefined operate.");
                return std::make_shared<NilValue>();
        }
    } else if (typeid(*expr) == typeid(SymbolValue)) {
        if(this->lookupBinding(expr)) return this->lookupBinding(expr);
        else throw LispError("Variable " + expr->toString() + " not defined.");
    } else if (typeid(*expr) == typeid(BuiltinProcValue)) {
        return expr;
    } else if (typeid(*expr) == typeid(LambdaValue)) {
        return expr;
    } else {
        throw LispError("Unimplemented");
        return std::make_shared<NilValue>();
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
    } else if (typeid(*proc) == typeid(LambdaValue)) {
        const LambdaValue* tempptr =
            dynamic_cast<LambdaValue*>(&(*proc));
        return tempptr->apply(args);
    } else {
        throw LispError("Unimplemented");
    }
}

ValuePtr EvalEnv::lookupBinding(ValuePtr target) {
    if (this->myMap.count(target->asSymbol().value()) > 0)
        return this->myMap[target->asSymbol().value()];
    else if (this->parent == nullptr) {
        return nullptr;
    } else return this->parent->lookupBinding(target);
};

ValuePtr EvalEnv::defineBinding(std::string target, ValuePtr content){
    /*auto tem = this->eval(content);
    if (this->lookupBinding(tem) != nullptr) tem = this->lookupBinding(tem);*/
    this->myMap[target] = content;
    return std::make_shared<NilValue>();
};
