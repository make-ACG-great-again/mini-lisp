#include "./parser.h" 
#include "./error.h"

Parser::Parser(std::deque<TokenPtr> tokens): tokens{std::move(tokens)} {};

ValuePtr Parser::parse() {
    if (tokens.empty()) {
        throw SyntaxError("Unimplemented");
        return nullptr;
    }
    unprocessed.push_back(std::move(*tokens.begin()));
    tokens.pop_front();
    auto token = unprocessed.end();
    token--;
    if ((*token)->getType() == TokenType::NUMERIC_LITERAL) {
        auto value = static_cast<NumericLiteralToken&>(*(*token)).getValue();
        return std::make_shared<NumericValue>(value);
    }
    if ((*token)->getType() == TokenType::BOOLEAN_LITERAL) {
        auto value = static_cast<BooleanLiteralToken&>(*(*token)).getValue();
        return std::make_shared<BooleanValue>(value);
    }
    if ((*token)->getType() == TokenType::STRING_LITERAL) {
        auto value = static_cast<StringLiteralToken&>(*(*token)).getValue();
        return std::make_shared<StringValue>(value);
    }
    if ((*token)->getType() == TokenType::IDENTIFIER) {
        auto value = static_cast<IdentifierToken&>(*(*token)).getName();
        return std::make_shared<SymbolValue>(value);
    }
    if ((*token)->getType() == TokenType::LEFT_PAREN) {
        return this->parseTails();
    }
    if ((*token)->getType() == TokenType::QUOTE) {
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("quote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    }
    if ((*token)->getType() == TokenType::QUASIQUOTE) {
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("quasiquote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    }
    if ((*token)->getType() == TokenType::UNQUOTE) {
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("unquote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    }
    throw SyntaxError("Unimplemented_parse");
}

ValuePtr Parser::parseTails() {
    if (tokens.empty()) {
        throw uncomplete_error("Unimplemented_short_no')'");
        return nullptr;
    }
    if ((*tokens.begin())->getType() == TokenType::RIGHT_PAREN) {
        tokens.pop_front();
        return std::make_shared<NilValue>();
    }
    auto car = this->parse();
    if ((*tokens.begin())->getType() == TokenType::DOT) {
        tokens.pop_front();
        auto cdr = this->parse();
        if (tokens.empty()) {
            throw uncomplete_error("Unimplemented_short_no')'after'.'");
            return nullptr;
        }
        if ((*tokens.begin())->getType() == TokenType::RIGHT_PAREN) {
            tokens.pop_front();
        } else {
            throw SyntaxError("Unimplemented_unexpexcted");
            return nullptr;
        }
        return std::make_shared<PairValue>(car, cdr);
    } else {
        auto cdr = this->parseTails();
        if (cdr == nullptr) {
            throw SyntaxError("Unimplemented_wrong");
            return nullptr;
        }
        return std::make_shared<PairValue>(car, cdr);
    }
}
