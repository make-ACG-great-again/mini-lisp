#include<iomanip>
#include <sstream>
#include<vector>
#include "./value.h"

BooleanValue::BooleanValue(bool b) {
	this->b = b;
}
std::string BooleanValue::toString() const {
	if (b) {
		return std::string{ "#t" };
	}
	else return std::string{ "#f" };
}

std::optional<std::string> BooleanValue::asSymbol() {
    if (b) {
        return std::string{"#t"};
    } else
        return std::string{"#f"};
}

NumericValue::NumericValue(double n) {
	d = n;
}
std::string NumericValue::toString() const {
	int m = d / 1;
	if (d == m) {
		/*char x = m + 48;
		std::string s{};
		s += x;*/
		std::string s = std::to_string(m);
		return s;
	}
	else return std::to_string(d);
}

std::optional<std::string> NumericValue::asSymbol() {
    int m = d / 1;
    if (d == m) {
        /*char x = m + 48;
        std::string s{};
        s += x;*/
        std::string s = std::to_string(m);
        return s;
    } else
        return std::to_string(d);
}

StringValue::StringValue(std::string s) : c{ s } {};
std::string StringValue::toString() const {
    std::ostringstream oss;
    oss << std::quoted(c);
    std::string quotedStr = oss.str();
    return quotedStr;
}

std::optional<std::string> StringValue::asSymbol() {
    std::ostringstream oss;
    oss << std::quoted(c);
    std::string quotedStr = oss.str();
    return quotedStr;
}

NilValue::NilValue() {};
std::string NilValue::toString()const {
	return std::string{ "()" };
}

std::optional<std::string> NilValue::asSymbol() {
    return std::string{"()"};
}

SymbolValue::SymbolValue(std::string k) :c{ k } {};
std::string SymbolValue::toString() const {
	/*std::string s;
	if (c.length() == 0)
		return s;
	for (int i = 0; i < c.length(); i++) {
		s += c[i];
	}*/
	return c;
}

std::optional<std::string> SymbolValue::asSymbol() {
    return c;
}

bool PairValue::kh = 1;
bool PairValue::out = 1;
std::vector<std::shared_ptr<Value>> PairValue::v = {};

PairValue::PairValue(ValuePtr t11, ValuePtr t22) : t1{ t11 }, t2{ t22 } {};
std::string PairValue::toString() const {
	std::string s1 = t1->toString();
	//std::string s2 = t2->toString();
	std::string s2;
	Value* temp = &*t2;
	bool temp_kh = kh;
	kh = 0;
	if (auto pair = dynamic_cast<const PairValue*>(temp)) {
		s2 = ' ' + pair->toString();
		if (temp_kh) {
			kh = 1;
			return std::string{ '(' + s1 + s2 + ')' };
		}
		else return std::string{ s1 + s2 };
	};
	if (auto pair = dynamic_cast<const NilValue*>(temp)) {
		if (temp_kh) {
			kh = 1;
			return std::string{ '(' + s1 + ')' };
		}
		else return std::string{ s1 };
	}
	else { s2 = " . " + t2->toString(); }
	if (temp_kh) {
		kh = 1;
		return std::string{ '(' + s1 + s2 + ')' };
	}
	else return std::string{ s1 + s2 };
}

std::vector<std::shared_ptr<Value>> PairValue::toVector() {
    bool tt = out;
    if (typeid(*t1) == typeid(PairValue)) {
        out = 0;
        t1->toVector();
    } else if (typeid(*t1) == typeid(NilValue)) {
        
    } else
        v.push_back(t1);
    if (typeid(*t2) == typeid(PairValue)) {
        out = 0;
        t2->toVector();
    } else if (typeid(*t2) == typeid(NilValue)) {
    } else
        v.push_back(t2);
    if (tt) {
        out = 1;
        std::vector<std::shared_ptr<Value>> temp2 = PairValue::v;
        v.clear();
        return temp2;
    }
    return v;
};

std::optional<std::string> PairValue::asSymbol() {
    std::optional<std::string> s = this->toString();
    return s;
}

std::string BuiltinProcValue::toString() const {
    return "#<procedure>";
};
