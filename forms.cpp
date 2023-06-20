#include <typeinfo>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <fstream>

#include "./tokenizer.h"
#include "./parser.h"
#include "./eval_env.h"
#include "./builtins.h"
#include "./value.h"
#include "./forms.h"
#include "./error.h"
#include "./eval_env.h"

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() < 2) {
        throw LispError("illegal define.");
        return std::make_shared<NilValue>();
    }
    std::string name = args[1]->asSymbol().value(); 
    if (typeid(*args[1]) == typeid(PairValue)) {
        std::vector<std::string> temp;
        std::ranges::transform(args[1]->toVector(), std::back_inserter(temp),
                               [](ValuePtr v) { return v->toString(); });
        std::vector<std::string> s{};
        if (temp.size() > 1)
            std::copy(temp.begin() + 1, temp.end(), std::back_inserter(s));
        if (args.size() < 3) {
            throw LispError("illegal if.");
            return std::make_shared<NilValue>();
        }
        std::vector<ValuePtr> temporary;
        temporary.insert(temporary.end(), args.begin() + 2, args.end());
        env.defineBinding(temp[0], make_shared<LambdaValue>(s, temporary, env.shared_from_this()));
        return std::make_shared<NilValue>();
    }
    if (size(args) == 3) {
        if (typeid(*args[1]) != typeid(SymbolValue))
            throw LispError("illegal define.");
        auto tem = env.eval(args[2]);
        if (env.lookupBinding(tem) != nullptr) tem = env.lookupBinding(tem);
        //env.myMap.insert(std::make_pair(name , tem));
        // args.clear();
        env.defineBinding(name, tem);
        return std::make_shared<NilValue>();
    } else {
        throw LispError("illegal define.");
        return std::make_shared<NilValue>();
    }
}

ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() == 1) return ValuePtr(std::make_shared<NilValue>());
    if (args.size() == 2) return args[1];
    std::vector<ValuePtr> destination;
    std::copy(args.begin() + 1, args.end(),
              std::back_inserter(destination));
    return std::make_shared<PairValue>(args[1],
                                       quoteForm(destination, env));
}

ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() == 3) {
        auto temp = env.eval(args[1]);
        if (temp->toString() == "#f" || temp->toString() == "false") {
            return std::make_shared<NilValue>();
        } else
            return env.eval(args[2]);
    }
    if (args.size() != 4) {
        throw LispError("illegal if.");
        return std::make_shared<NilValue>();
    }
    auto temp = env.eval(args[1]);
    if (temp->toString() == "#f" || temp->toString() == "false") {
        return env.eval(args[3]);
    } else
        return env.eval(args[2]);
}

ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& env){
    if (args.size() < 2) {
        return std::make_shared<BooleanValue>(true);
    }
    for (int i = 1; i < args.size(); i++) {
        auto temp = env.eval(args[i]);
        if (temp->toString() == "#f" || temp->toString() == "false") {
            return std::make_shared<BooleanValue>(false);
        } else {
            if (i == args.size() - 1)
                return temp;
            else
                continue;
        }
    }   
}

ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() < 2) {
        return std::make_shared<BooleanValue>(false);
    }
    for (int i = 1; i < args.size(); i++) {
        auto temp = env.eval(args[i]);
        if (temp->toString() != "#f" && temp->toString() != "false") {
            return temp;
        } else {
            continue;
        }
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() < 3) {
        throw LispError("illegal if.");
        return std::make_shared<NilValue>();
    }
    std::vector<ValuePtr> temporary;
    temporary.insert(temporary.end() , args.begin() + 2, args.end());
    std::vector<std::string> temp;
    std::ranges::transform(args[1]->toVector(), std::back_inserter(temp),
                           [](ValuePtr v) { return v->toString(); });
    return std::make_shared<LambdaValue>(temp, temporary , env.shared_from_this());
}

ValuePtr condForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() <= 1) return std::make_shared<NilValue>();
    int num = 0;
    for (auto i : args) {
        if (num == 0) {
            num++;
            continue;
        }
        std::vector<ValuePtr> list = i->toVector();
        if (list.size() == 0) throw LispError("wrong num of arguments.");
        auto condition = list[0];
        if (condition->asSymbol() == "else") {
            if (num != args.size() - 1) throw LispError("'else' must the last one.");
            else if (list.size() <= 1) throw LispError("'else' must be followed by others.");
            else {
                for (int j = 1; j < list.size(); j++) {
                    if (j < list.size() - 1)
                        env.eval(list[j]);
                    else
                        return env.eval(list[list.size() - 1]);
                }
            }
        } else if (env.eval(condition)->asSymbol() == "#f") {
            num++;
            continue;
        } else {
            if (list.size() == 1) return env.eval(condition);
            for (int j = 1; j < list.size(); j++) {
                if (j < list.size() - 1)
                    env.eval(list[j]);
                else
                    return env.eval(list[list.size() - 1]);
            }
        }
        num++;
    };
    return std::make_shared<NilValue>();
};

ValuePtr beginForm(const std::vector<ValuePtr>& args, EvalEnv& env){
    if (args.size() <= 1) throw LispError("wrong num of arguments.");
    for (int i = 1; i < args.size(); i++) {
        if (i < args.size() - 1)
            env.eval(args[i]);
        else
            return env.eval(args[i]);
    };
};

ValuePtr letForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() <= 2) throw LispError("wrong num of arguments.");
    auto arguments = args[1]->toVector();
    std::vector<std::string> argument;
    std::vector<ValuePtr> value;
    for (auto i : arguments) {
        auto temp = i->toVector();
        if (temp.size() != 2)
            throw LispError("wrong num of arguments.");
        else {
            argument.push_back(temp[0]->toString());
            value.push_back(env.eval(temp[1]));
        }
    }
    std::vector<ValuePtr> body;
    body.insert(body.end(), args.begin() + 2, args.end());
    LambdaValue temp = LambdaValue(argument, body, env.shared_from_this());
    return temp.apply(value);
};

ValuePtr quasiquoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() <= 1) return std::make_shared<NilValue>();
    auto temporary = args[1]->toVector();
    std::vector<ValuePtr> destination;
    for (auto i : temporary) {
        if (typeid(*i) != typeid(PairValue)) {
            destination.push_back(i);
        }else if (i->toVector()[0]->asSymbol().value() == "unquote") {
            if (i->toVector().size() != 2)
                throw LispError("'unquote' must be followed by others.");
            std::vector<ValuePtr> temp = i->toVector()[1]->toVector();
            destination.push_back(env.eval(my_list_make(temp)));
        } else destination.push_back(i);
    }
    return my_list_make(destination);
}

ValuePtr importForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 2) throw LispError("illegal import.");
    std::string place = args[1]->toString();
    if (place.length() <= 2) {
        throw LispError("illegal import.");
    }
    std::string address = {"C:\\Users\\LCM\\Desktop\\import\\"};
    std::string source = args[1]->toString();
    address.append(source.substr(1, source.length() - 2));
    address.append(".txt");
    //std::cout << address << std::endl;
    std::ifstream get_in(address, std::ios::in);
    if (!get_in) {
        throw LispError("unfound.");
    }
    std::string line_T;
    std::deque<ValuePtr> parsers_import{};
    while (std::getline(get_in, line_T)) {
        try {
            if (line_T == "") continue;
            if (line_T[0] == ';' && line_T[1] == ';') 
                continue;
            if (!Parser::uncomplete) {
                Parser::unprocessed.clear();
                auto tokens = Tokenizer::tokenize(line_T);
                Parser parser(std::move(tokens));
                while (!parser.tokens.empty()) {
                    auto value = parser.parse();
                    Parser::uncomplete = 0;
                    Parser::unprocessed.clear();
                    parsers_import.push_back(std::move(value));
                }
                while (!parsers_import.empty()) {
                    ValuePtr value = std::move(parsers_import.front());
                    parsers_import.pop_front();
                    auto result = env.eval(std::move(value));
                }
                Parser::uncomplete = 0;
            } else {
                auto tokens = Tokenizer::tokenize(line_T);
                while (!tokens.empty()) {
                    Parser::unprocessed.push_back(std::move(tokens.front()));
                    tokens.pop_front();
                }
                Parser parser(std::move(Parser::unprocessed));
                Parser::unprocessed.clear();
                while (!parser.tokens.empty()) {
                    auto value = parser.parse();
                    Parser::uncomplete = 0;
                    Parser::unprocessed.clear();
                    parsers_import.push_back(std::move(value));
                }
                while (!parsers_import.empty()) {
                    ValuePtr value = std::move(parsers_import.front());
                    parsers_import.pop_front();
                    auto result = env.eval(std::move(value));
                }
                Parser::uncomplete = 0;
                Parser::unprocessed.clear();
            }
        } catch (uncomplete_error& c) {
            Parser::uncomplete = 1;
        } catch (std::runtime_error& e) {
            std::cout << "wrong" << std::endl;
            throw LispError("failed while import.");
        }
    }
    if (Parser::uncomplete) throw LispError("failed while import.");
    return std::make_shared<NilValue>();
}

const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS{
    {"define", (defineForm)}, {"quote", (quoteForm)}, {"if", ifForm},
    {"and", andForm},         {"or", orForm},         {"lambda", lambdaForm},
    {"cond", condForm},       {"begin", beginForm}, {"let", letForm},
    {"quasiquote", quasiquoteForm}, {"import", importForm}};
