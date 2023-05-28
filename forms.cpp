#include "./forms.h"
#include "./error.h"
#include "./eval_env.h"

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    std::optional<std::string> name = args[1]->asSymbol(); 
    if (size(args) == 3) {
        auto tem = env.eval(args[2]);
        if (auto temp = env.myMap[tem->asSymbol()]) tem = temp;
        env.myMap.insert(std::make_pair(name , tem));
        // args.clear();
        return ValuePtr(new NilValue());
    } else {
        throw LispError("illegal define.");
        return ValuePtr(new NilValue());
    }
}

ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
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
    if (args[1]->toString() == "#f") {
        return env.eval(args[3]);
    } else
        return env.eval(args[2]);
}

ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& env){
    if (args.size() < 2) {
        return std::make_shared<BooleanValue>(false);
    }
    for (int i = 1; i < args.size(); i++) {
        if (args[i]->toString() == "#f") {
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
        if (args[i]->toString() != "#f") {
            return env.eval(args[i]);
        } else {
            env.eval(args[i]);
        }
    }
    return std::make_shared<BooleanValue>(false);
}

const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS{
    {"define", (defineForm)}, 
    {"quote", (quoteForm)},
    {"if", ifForm},
    {"and" , andForm},
    {"or" , orForm}};
