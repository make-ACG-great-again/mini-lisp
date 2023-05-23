#ifndef VALUE_H
#define VALUE_H

#include<string>
#include<memory>
#include<vector>
#include<optional>

// 你的代码

//using ValuePtr = std::shared_ptr<Value>;

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
};

using ValuePtr = std::shared_ptr<Value>;

class BooleanValue : public Value {
	bool b;
public:
	BooleanValue(bool b);
	std::string toString()const;
    std::optional<std::string> asSymbol();
	
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
};

class StringValue : public Value {
public:
	std::string c;
	StringValue(std::string s);
	std::string toString()const;
    std::optional<std::string> asSymbol();
};

class NilValue : public Value {
public:
	NilValue();
	std::string toString()const;
    std::optional<std::string> asSymbol();
};

class SymbolValue : public Value {
	std::string c;
public:
	SymbolValue(std::string k);
	std::string toString()const;
    std::optional<std::string> asSymbol();
};

//template<typename T1, typename T2>
//using ValuePtr = std::shared_ptr<Value>;
class PairValue : public Value {
	//using ValuePtr = std::shared_ptr<Value>;
	ValuePtr t1;
	ValuePtr t2;
public:
	static bool kh;
    static bool out;
	PairValue(ValuePtr t11, ValuePtr t22);
	std::string toString()const;
    static std::vector<std::shared_ptr<Value>> v;
    std::vector<std::shared_ptr<Value>> toVector();
    std::optional<std::string> asSymbol();
    ValuePtr right()const{
        ValuePtr temp = this->t2;
        return temp;
    };
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
};

#endif

