#include "./parser.h" 
#include "./error.h"

Parser::Parser(std::deque<TokenPtr> tokens) : tokens{std::move(tokens)} {};
    
bool Parser::uncomplete = 0;
std::deque<TokenPtr> Parser::unprocessed{};

ValuePtr Parser::parse() {
    if (tokens.empty()) {
        throw SyntaxError("Unimplemented");
        return nullptr;
    }
    TokenPtr token = std::move(*tokens.begin());
    tokens.pop_front();
    if (token->getType() == TokenType::NUMERIC_LITERAL) {
        auto value = static_cast<NumericLiteralToken&>(*token).getValue();
        unprocessed.push_back(std::move(token));
        return std::make_shared<NumericValue>(value);
    }
    if (token->getType() == TokenType::BOOLEAN_LITERAL) {
        auto value = static_cast<BooleanLiteralToken&>(*token).getValue();
        unprocessed.push_back(std::move(token));
        return std::make_shared<BooleanValue>(value);
    }
    if (token->getType() == TokenType::STRING_LITERAL) {
        auto value = static_cast<StringLiteralToken&>(*token).getValue();
        unprocessed.push_back(std::move(token));
        return std::make_shared<StringValue>(value);
    }
    if (token->getType() == TokenType::IDENTIFIER) {
        auto value = static_cast<IdentifierToken&>(*token).getName();
        unprocessed.push_back(std::move(token));
        return std::make_shared<SymbolValue>(value);
    }
    if (token->getType() == TokenType::LEFT_PAREN) {
        unprocessed.push_back(std::move(token));
        return this->parseTails();
    }
    if (token->getType() == TokenType::QUOTE) {
        unprocessed.push_back(std::move(token));
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("quote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    }
    if (token->getType() == TokenType::QUASIQUOTE) {
        unprocessed.push_back(std::move(token));
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("quasiquote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    }
    if (token->getType() == TokenType::UNQUOTE) {
        unprocessed.push_back(std::move(token));
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("unquote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    }
    throw SyntaxError("Unimplemented_parse");
}

ValuePtr Parser::parseTails() {
    if (tokens.empty()) {
        throw uncomplete_error("short_no')'");
        return nullptr;
    }
    if ((*tokens.begin())->getType() == TokenType::RIGHT_PAREN) {
        unprocessed.push_back(std::move(*tokens.begin()));
        tokens.pop_front();
        return std::make_shared<NilValue>();
    }
    auto car = this->parse();
    if (tokens.empty()) {
        throw uncomplete_error("uncomplete");
        return nullptr;
    }
    if ((*tokens.begin())->getType() == TokenType::DOT) {
        unprocessed.push_back(std::move(*tokens.begin()));
        tokens.pop_front();
        auto cdr = this->parse();
        if (tokens.empty()) {
            throw uncomplete_error("short_no')'after'.'");
            return nullptr;
        }
        if ((*tokens.begin())->getType() == TokenType::RIGHT_PAREN) {
            unprocessed.push_back(std::move(*tokens.begin()));
            tokens.pop_front();
        } else {
            throw uncomplete_error("unexpexcted");
            return nullptr;
        }
        return std::make_shared<PairValue>(car, cdr);
    } else {
        auto cdr = this->parseTails();
        if (cdr == nullptr) {
            throw uncomplete_error("wrong");
            return nullptr;
        }
        return std::make_shared<PairValue>(car, cdr);
    }
}
