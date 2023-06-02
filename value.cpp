#include<iomanip>
#include <sstream>
#include<vector>
#include "./value.h"
#include "./eval_env.h"

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

bool BooleanValue::operator==(std::shared_ptr<Value> temp) {
    if (typeid(*temp) != typeid(BooleanValue)) {
        return false;
    }
    BooleanValue* t = dynamic_cast<BooleanValue*>(&*temp);
    if (t->b == this->b)
        return true;
    else
        return false;
};

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

bool NumericValue::operator==(std::shared_ptr<Value> temp) {
    if (typeid(*temp) != typeid(NumericValue)) {
        return false;
    }
    NumericValue* t = dynamic_cast<NumericValue*>(&*temp);
    if (t->d == this->d)
        return true;
    else
        return false;
};

StringValue::StringValue(std::string s) : c{ s } {};
std::string StringValue::toString() const {
    std::ostringstream oss;
    oss << std::quoted(c);
    std::string quotedStr = oss.str();
    return quotedStr;
}

//std::optional<std::string> StringValue::asSymbol() {
//    std::ostringstream oss;
//    oss << std::quoted(c);
//    std::string quotedStr = oss.str();
//    return quotedStr;
//}

bool StringValue::operator==(std::shared_ptr<Value> temp) {
    if (typeid(*temp) != typeid(StringValue)) {
        return false;
    }
    StringValue* t = dynamic_cast<StringValue*>(&*temp);
    if (t->c == this->c)
        return true;
    else
        return false;
};

NilValue::NilValue() {};
std::string NilValue::toString()const {
	return std::string{ "()" };
}

std::optional<std::string> NilValue::asSymbol() {
    return std::string{"()"};
}

bool NilValue::operator==(std::shared_ptr<Value> temp) {
    if (typeid(*temp) != typeid(NilValue)) {
        return false;
    }
    else return true;
};

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

bool SymbolValue::operator==(std::shared_ptr<Value> temp) {
    if (typeid(*temp) != typeid(SymbolValue)) {
        return false;
    }
    SymbolValue* t = dynamic_cast<SymbolValue*>(&*temp);
    if (t->c == this->c)
        return true;
    else
        return false;
};

bool PairValue::kh = 1;
bool PairValue::out = 1;
std::vector<std::shared_ptr<Value>> PairValue::v = {};

PairValue::PairValue(ValuePtr t11, ValuePtr t22) : t1{ t11 }, t2{ t22 } {};
std::string PairValue::toString() const {
    bool t = kh;
    kh = 1;
	std::string s1 = t1->toString();
    kh = t;
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
    //if (typeid(*t1) == typeid(PairValue)) {
    //    //v.push_back();
    //} else 
    if (typeid(*t1) == typeid(NilValue)) {
        
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

//std::optional<std::string> PairValue::asSymbol() {
//    std::optional<std::string> s = this->toString();
//    return s;
//}

bool PairValue::operator==(std::shared_ptr<Value> temp) {
    if (typeid(*temp) != typeid(PairValue)) {
        return false;
    }
    PairValue* t = dynamic_cast<PairValue*>(&*temp);
    if (*t->t1 == this->t1 && *t->t2 == this->t2)
        return true;
    else
        return false;
};

std::string BuiltinProcValue::toString() const {
    return "#<procedure>";
};

bool BuiltinProcValue::operator==(std::shared_ptr<Value> temp) {
    if (typeid(*temp) != typeid(BuiltinProcValue)) {
        return false;
    }
    BuiltinProcValue* t = dynamic_cast<BuiltinProcValue*>(&*temp);
    if (t->procedure == this->procedure)
        return true;
    else
        return false;
};

std::string LambdaValue::toString() const {
    return "#<procedure>";
};

ValuePtr LambdaValue::apply(const std::vector<ValuePtr>& args) const {
    std::shared_ptr<EvalEnv> calculate = EvalEnv::createGlobal();
    if (params.size() != args.size())
        throw LispError("wrong num of arguments.");
    for (int i = 0; i < params.size(); i++)
        calculate->defineBinding(params[i], args[i]);
    calculate->parent = this->environment;
    for (int i = 0; i < body.size(); i++) {
        if (i < body.size() - 1) calculate->eval(body[i]);
        else
            return calculate->eval(body[i]);
    };
};

bool LambdaValue::operator==(std::shared_ptr<Value> temp) {
    if (typeid(*temp) != typeid(LambdaValue)) {
        return false;
    }
    LambdaValue* t = dynamic_cast<LambdaValue*>(&*temp);
    std::vector<Value> result;
    if (t->params == this->params) {
        if (t->body.size() == this->body.size()) {
            for (int i = 0; i < this->body.size(); i++) {
                if (*t->body[i] == this->body[i])
                    continue;
                else
                    return false;
            }
            return true;
        } else return false;
    }else
        return false;
};
