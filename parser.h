#ifndef PARSER_H
#define PARSER_H
#include <memory>
#include "./value.h"
#include "./error.h"
#include "./token.h"
#include "./tokenizer.h"

class Parser {
public:
    std::deque<TokenPtr> tokens;
    Parser(std::deque<TokenPtr> tokens);
    ValuePtr parse();
    ValuePtr parseTails();
};

#endif
