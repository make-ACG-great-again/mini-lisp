#include<typeinfo>
#include<iostream>
#include<unordered_map>
#include <algorithm>
#include <iterator>
#include "./forms.h"
#include "./error.h"
#include "./eval_env.h"

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    std::optional<std::string> name = args[1]->asSymbol(); 
    if (size(args) == 3) {
        if (typeid(*args[1]) == typeid(PairValue)) {
            std::vector<std::string> temp;
            std::ranges::transform(
                args[1]->toVector(), std::back_inserter(temp),
                [](ValuePtr v) { return v->toString(); });
            std::vector<std::string> s{};
            if (temp.size() > 1)
                std::copy(temp.begin() + 1, temp.end(), std::back_inserter(s));
            env.myMap.insert(
                std::make_pair(temp[0], make_shared<LambdaValue>(s, args[2]->toVector())));
            return ValuePtr(new NilValue());
        }
        auto tem = env.eval(args[2]);
        if (auto temp = env.myMap[tem->asSymbol()]) tem = temp;
        //env.myMap.insert(std::make_pair(name , tem));
        // args.clear();
        env.myMap[name] = tem;
        return ValuePtr(new NilValue());
    } else {
        throw LispError("illegal define.");
        return ValuePtr(new NilValue());
    }
}

ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() == 1) return ValuePtr(new NilValue());
    if (args.size() == 2) return args[1];
    std::vector<ValuePtr> destination;
    std::copy(args.begin() + 1, args.end(),
              std::back_inserter(destination));
    return std::make_shared<PairValue>(args[1],
                                       quoteForm(destination, env));
}

ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 4) {
        throw LispError("illegal if.");
        return ValuePtr(new NilValue());
    }
    if (env.eval(args[1])->toString() == "#f" ||
        env.eval(args[1])->toString() == "false") {
        return env.eval(args[3]);
    } else
        return env.eval(args[2]);
}

ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& env){
    if (args.size() < 2) {
        return std::make_shared<BooleanValue>(true);
    }
    for (int i = 1; i < args.size(); i++) {
        if (args[i]->toString() == "#f" || args[i]->toString() == "false") {
            return std::make_shared<BooleanValue>(false);
        } else {
            if (i == args.size() - 1) return env.eval(args[args.size() - 1]);
            else
                env.eval(args[i]);
        }
    }   
}

ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() < 2) {
        return std::make_shared<BooleanValue>(false);
    }
    for (int i = 1; i < args.size(); i++) {
        if (args[i]->toString() != "#f" && args[i]->toString() != "false") {
            return env.eval(args[i]);
        } else {
            env.eval(args[i]);
        }
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 3) {
        return std::make_shared<BooleanValue>(false);
    }
    std::vector<std::string> temp;
    std::ranges::transform(args[1]->toVector(), std::back_inserter(temp),
                           [](ValuePtr v) { return v->toString(); });
    return std::make_shared<LambdaValue>(temp, args[2]->toVector());
}

const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS{
    {"define", (defineForm)}, {"quote", (quoteForm)}, {"if", ifForm},
    {"and", andForm},         {"or", orForm},         {"lambda", lambdaForm}};
