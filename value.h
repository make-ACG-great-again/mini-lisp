#ifndef VALUE_H
#define VALUE_H

#include<string>
#include<memory>
#include<vector>
#include<optional>
#include "./error.h"
//#include "./eval_env.h"

// 你的代码

//using ValuePtr = std::shared_ptr<Value>;
class EvalEnv;

class Value {
public:
	virtual std::string toString()const = 0;
    virtual std::vector<std::shared_ptr<Value>> toVector() {
        std::vector<std::shared_ptr<Value>> t;
        return t;
    };
    virtual std::optional<std::string> asSymbol() {
        return std::string{"error:unoverrided"};
    };
    virtual bool isNumber() {
        return false;
    };
    virtual double asNumber() {
        return 0;
    };
    virtual bool operator==(std::shared_ptr<Value> temp) {
        return false;
    };
};

using ValuePtr = std::shared_ptr<Value>;

class BooleanValue : public Value {
	bool b;
public:
	BooleanValue(bool b);
	std::string toString()const;
    std::optional<std::string> asSymbol();
    bool operator==(std::shared_ptr<Value> temp);
};

class NumericValue : public Value {
	double d;
public:
	NumericValue(double n);
	std::string toString()const;
    std::optional<std::string> asSymbol();
    bool isNumber() {
        return true;
    };
    double asNumber() {
        return d;
    };
    bool operator==(std::shared_ptr<Value> temp);
};

class StringValue : public Value {
public:
    std::string c;
public:
	StringValue(std::string s);
	std::string toString()const;
    //std::optional<std::string> asSymbol();
    bool operator==(std::shared_ptr<Value> temp);
};

class NilValue : public Value {
public:
	NilValue();
	std::string toString()const;
    std::optional<std::string> asSymbol();
    std::vector<std::shared_ptr<Value>> toVector();
    bool operator==(std::shared_ptr<Value> temp);
};

class SymbolValue : public Value {
	std::string c;
public:
	SymbolValue(std::string k);
	std::string toString()const;
    std::optional<std::string> asSymbol();
    bool operator==(std::shared_ptr<Value> temp);
};

//template<typename T1, typename T2>
//using ValuePtr = std::shared_ptr<Value>;
class PairValue : public Value {
	//using ValuePtr = std::shared_ptr<Value>;
	ValuePtr t1;
	ValuePtr t2;
public:
	PairValue(ValuePtr t11, ValuePtr t22);
	std::string toString()const;
    std::vector<std::shared_ptr<Value>> toVector();
    //std::optional<std::string> asSymbol();
    ValuePtr left() const {
        ValuePtr temp = this->t1;
        return temp;
    };
    ValuePtr right()const{
        ValuePtr temp = this->t2;
        return temp;
    };
    bool operator==(std::shared_ptr<Value> temp);
};

//typedef ValuePtr (*FuncPtr)(const std::vector<ValuePtr>&);

using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr>&);

class BuiltinProcValue : public Value {
    // [...]

public:
    BuiltinFuncType* procedure;
    BuiltinProcValue(BuiltinFuncType* p) : procedure(p){};
    // 直接返回 #<procedure> 就可以，我们不做更多要求。
    std::string toString() const override;
    bool operator==(std::shared_ptr<Value> temp);
};

class LambdaValue : public Value {
    std::vector<std::string> params;
    std::vector<ValuePtr> body;
    // [...]
public:
    std::shared_ptr<EvalEnv> environment;
    ValuePtr apply(const std::vector<ValuePtr>& args) const;
    LambdaValue(const std::vector<std::string>& params,const std::vector<ValuePtr>& body, std::shared_ptr<EvalEnv> env)
        : params{params}, body{body}, environment{env} {};
    std::string toString() const override;  // 如前所述，返回 #<procedure> 即可
    bool operator==(std::shared_ptr<Value> temp);
};


#endif

