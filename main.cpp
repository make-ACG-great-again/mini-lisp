#include <iostream>
#include <string>

#include "./tokenizer.h"
#include "./parser.h"
#include "./eval_env.h"
#include "./rjsj_test.hpp"
#include "./builtins.h"
#include "./value.h"

//struct TestCtx {
//    std::string eval(std::string input) {
//        auto tokens = Tokenizer::tokenize(input);
//        Parser parser(std::move(tokens));
//        auto value = parser.parse();
//        return value->toString();
//    }
//};

struct TestCtx {
    EvalEnv env;
    std::string eval(std::string input) {
        auto tokens = Tokenizer::tokenize(input);
        Parser parser(std::move(tokens));
        auto value = parser.parse();
        auto result = env.eval(std::move(value));
        return result->toString();
    }
};

int main() {
    //RJSJ_TEST(TestCtx, Lv2, Lv2Only);
    //RJSJ_TEST(TestCtx, Lv2, Lv3);
    //RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4);
    // [...]
    while (true) {
        try {
            std::cout << ">>> ";
            std::string line;
            std::getline(std::cin, line);
            if (std::cin.eof()) {
                std::exit(0);
            }
            auto tokens = Tokenizer::tokenize(line);
            //for (auto& token : tokens) { //词法分析器
            //    std::cout << *token << std::endl; //词法分析器
            //} //词法分析器
            Parser parser(std::move(tokens));  // tokenptr 不支持复制
            auto value = parser.parse();
            //std::cout << value->toString() << std::endl;  // 输出外部表示   //语法分析器
            EvalEnv env; //求值
            auto result = env.eval(std::move(value)); //求值
            //if (result == nullptr) std::cout << "error" << std::endl;
            std::cout << result->toString() << std::endl; //求值
        } catch (std::runtime_error& e) {
            std::cerr << "error: " << e.what() << std::endl;
        }
    }
}
