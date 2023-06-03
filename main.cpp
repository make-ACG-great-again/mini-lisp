#include <iostream>
#include <string>
#include <fstream>

#include "./tokenizer.h"
#include "./parser.h"
#include "./eval_env.h"
#include "./builtins.h"
#include "./value.h"
#include "./rjsj_test.hpp"

struct TestCtx {
    //EvalEnv env;
    std::shared_ptr<EvalEnv> env = EvalEnv::createGlobal();
    std::string eval(std::string input) {
        auto tokens = Tokenizer::tokenize(input);
        Parser parser(std::move(tokens));
        auto value = parser.parse();
        auto result = env->eval(std::move(value));
        return result->toString();
    }
};

void REPL() {
    //RJSJ_TEST(TestCtx, Lv2, Lv2Only);
    //RJSJ_TEST(TestCtx, Lv2, Lv3);
    //RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4);
    //RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5);
    //RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5, Lv6);
    //RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5, Lv5Extra, Lv6, Lv7);
    //RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5, Lv5Extra, Lv6, Lv7, Lv7Lib);
    //RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5, Lv5Extra, Lv6, Lv7, Lv7Lib, Sicp);
    // [...]
    auto env = EvalEnv::createGlobal();  // 求值
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
            //std::cout << value->toString() << std::endl;  // 输出外部表示   //语法分析
            auto result = env->eval(std::move(value)); //求值
            //if (result == nullptr) std::cout << "error" << std::endl;
            std::cout << result->toString() << std::endl; //求值
        } catch (std::runtime_error& e) {
            std::cerr << "error: " << e.what() << std::endl;
        }
    }
}

int text(std::ifstream& get_in) {
    if (!get_in) {
        std::cout << "Failed to open file." << std::endl;
        return 1;
    }
    auto env = EvalEnv::createGlobal();
    std::string line;
    while (std::getline(get_in, line)) {
        try {
            auto tokens = Tokenizer::tokenize(line);
            Parser parser(std::move(tokens));
            auto value = parser.parse();
            auto result = env->eval(std::move(value));
        } catch (std::runtime_error& e) {
            std::cerr << "error: " << e.what() << std::endl;
            return 1;
        }
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        REPL();
    } else if(argc == 2){
        char* place = argv[1];
        std::ifstream get(place, std::ios::in);
        int reply = text(get);
        return reply;
    }
}
