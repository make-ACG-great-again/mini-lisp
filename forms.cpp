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

const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS{
    {"define", (defineForm)}, 
    {"quote", (quoteForm)}};
