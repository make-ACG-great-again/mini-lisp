#include <iostream>
#include <string>
#include <fstream>
#include <vector>
//#include <conio.h>

#include "./tokenizer.h"
#include "./parser.h"
#include "./eval_env.h"
#include "./builtins.h"
#include "./value.h"
#include "./rjsj_test.hpp"
//#include "./PDCurses-3.9/PDCurses-3.9/curses.h";
//
//#ifdef _WIN32
//#define NOMINMAX
//#include <Windows.h>
//#endif

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

std::vector<std::string::iterator> findAllOccurrences(std::string& str, char ch) {
    std::vector<std::string::iterator> occurrences;
    auto it = std::find(str.begin(), str.end(), ch);
    while (it != str.end()) {
        occurrences.push_back(it);
        it = std::find_if(std::next(it), str.end(),
                          [ch](char c) { return c == ch; });
    }
    //std::cout << "find" << std::endl;
    return occurrences;
}

//std::string getline() {
//    std::string input;
//    char ch;
//    std::string temp;
//    std::vector<char> cut_off{'(', ')', '\'', '`', ',', ';', '\n'};
//    while (true) {
//        // 检查是否有键盘输入
//        if (_kbhit()) {
//            // 获取键盘输入
//            ch = _getch();
//            if (std::find(cut_off.begin(), cut_off.end(), ch) != cut_off.end()) {
//                auto temp_env = EvalEnv::createGlobal();
//                auto temp_Val = std::make_shared<StringValue>(temp);
//                if (temp_env->lookupBinding(temp_Val) && !temp.empty()) {
//                    for (int i = 0; i < temp.size(); i++) {
//                        std::cout << '\b';
//                    }
//                    std::cout << "\033[33m" << temp << "\033[0m";
//                }
//            }
//            if (ch == '\n') {
//                std::cout << std::endl;
//                return input;
//            }
//            std::cout << ch; 
//            input += ch;
//        }
//
//        // 处理其他逻辑
//        // ...
//    }
//}

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
    static auto env_s = EvalEnv::createGlobal();  // 求值
    Parser::uncomplete = 0;
    std::deque<ValuePtr> parsers_REPL{};
    while (true) {
        try {
            if (!Parser::uncomplete) {
                Parser::unprocessed.clear();
                std::cout << ">>> ";
                std::string line;
                std::getline(std::cin, line);
                if (std::cin.eof()) {
                    std::exit(0);
                }
                auto tokens = Tokenizer::tokenize(line);
                // for (auto& token : tokens) { //词法分析器
                //     std::cout << *token << std::endl; //词法分析器
                // } //词法分析器
                Parser parser(std::move(tokens));  // tokenptr 不支持复制
                while (!parser.tokens.empty()) {
                    auto value = parser.parse();
                    // std::cout << value->toString() << std::endl;  //
                    // 输出外部表示
                    // //语法分析
                    Parser::uncomplete = 0;
                    Parser::unprocessed.clear();
                    parsers_REPL.push_back(std::move(value));
                }
                while (!parsers_REPL.empty()) {
                    ValuePtr value = std::move(parsers_REPL.front());
                    parsers_REPL.pop_front();
                    auto result = env_s->eval(std::move(value));  // 求值
                    // if (result == nullptr) std::cout << "error" << std::endl;
                    std::cout << result->toString() << std::endl;  // 求值
                }
                Parser::uncomplete = 0;
            } else {
                std::cout << "...";
                std::string line;
                std::getline(std::cin, line);
                if (std::cin.eof()) {
                    std::exit(0);
                }
                auto tokens = Tokenizer::tokenize(line);
                //Parser::unprocessed.insert(Parser::unprocessed.end(), tokens.begin(), tokens.end());
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
                    parsers_REPL.push_back(std::move(value));
                }
                while (!parsers_REPL.empty()) {
                    ValuePtr value = std::move(parsers_REPL.front());
                    parsers_REPL.pop_front();
                    auto result = env_s->eval(std::move(value));
                    std::cout << result->toString() << std::endl;
                }
                Parser::uncomplete = 0;
                Parser::unprocessed.clear();
            }
        } catch (uncomplete_error& c) {
            Parser::uncomplete = 1;
        } catch (std::runtime_error& e) {
            std::cerr << "error: " << e.what() << std::endl;
            Parser::uncomplete = 0;
            Parser::unprocessed.clear();
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
    Parser::uncomplete = 0;
    std::deque<ValuePtr> parsers_text{};
    while (std::getline(get_in, line)) {
        try {
            if (line == "") continue;
            if (line[0] == ';' && line[1] == ';') continue;
            std::vector<std::string::iterator> temp = findAllOccurrences(line, '~');
            while (temp.size() % 2 != 0) {
                //std::cout << temp.size() << std::endl;
                std::string moreline;
                if (!std::getline(get_in, moreline))
                    throw LispError("unclosed ~");
                line += moreline;
                //std::cout << "moreline" << std::endl;
                temp = findAllOccurrences(line, '~');
            }
            if (temp.size() > 0) {
                int n = 0;
                while (n < temp.size()) {
                    auto head = temp[n];
                    n++;
                    auto tail = temp[n];
                    n++;
                    line.erase(head, tail);
                    *head = ' ';
                }
            }
            if (!Parser::uncomplete) {
                Parser::unprocessed.clear();
                auto tokens = Tokenizer::tokenize(line);
                Parser parser(std::move(tokens));
                while (!parser.tokens.empty()) {
                    auto value = parser.parse();
                    Parser::uncomplete = 0;
                    Parser::unprocessed.clear();
                    parsers_text.push_back(std::move(value));
                }
                while (!parsers_text.empty()) {
                    ValuePtr value = std::move(parsers_text.front());
                    parsers_text.pop_front();
                    auto result = env->eval(std::move(value));
                }
                Parser::uncomplete = 0;
            } else {
                auto tokens = Tokenizer::tokenize(line);
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
                    parsers_text.push_back(std::move(value));
                }
                while (!parsers_text.empty()) {
                    ValuePtr value = std::move(parsers_text.front());
                    parsers_text.pop_front();
                    auto result = env->eval(std::move(value));
                }
                Parser::uncomplete = 0;
                Parser::unprocessed.clear();
            }
        } catch (uncomplete_error& c) {
            Parser::uncomplete = 1;
        } catch (std::runtime_error& e) {
            std::cerr << "error: " << e.what() << std::endl;
            return 1;
        }
    }
    return 0;
}

int main(int argc, char** argv) {
    //SetConsoleOutputCP(CP_UTF8);
    if (argc == 1) {
        REPL();
    } else if(argc == 2){
        char* place = argv[1];
        std::ifstream get(place, std::ios::in);
        int reply = text(get);
        return reply;
    }
}
