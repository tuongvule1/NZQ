#ifndef Q_H
#define Q_H
#include "Z.h"

#include <unordered_map>//for baby-step-giant-step
#include <math.h>


class Q{
public:
    Q();//default construct 0
    Q(const std::string& str);
    Q(const char* cstr);//C-style string
    Q(const Q& other);
    Q(const Z& z);
    Q(const N& n);
    Q(const Z& z1, const Z& z2);
    template<typename Int>Q(const Int& a);
    Q& operator=(const Q& other);
    template<typename Int>Q& operator=(const Int& a);
    Q operator+(const Q& rhs) const;
    template<typename Int>Q operator+(const Int& a) const;
    Q operator-() const;
    Q operator-(const Q& rhs) const;
    template<typename Int>Q operator-(const Int& a) const;
    Q operator*(const Q& rhs) const;
    template<typename Int>Q operator*(const Int& a) const;
    Q operator/(const Q& rhs) const;
    template<typename Int>Q operator/(const Int& a) const;
    Q operator^(const Z& rhs) const;
    template<typename Int>Q operator^(const Int& a) const;
    Q& operator+=(const Q& rhs);
    Q& operator-=(const Q& rhs);
    Q& operator*=(const Q& rhs);
    Q& operator/=(const Q& rhs);
    Q& operator^=(const Z& rhs);
    Q& operator++();
    Q operator++(int);
    Q& operator--();
    Q operator--(int);
    Q abs() const;
    Q dist(const Q& rhs) const;//return |a-b|
    Z floor() const;
    Z ceil() const;
    Z numer() const;
    N denom() const;
    template<typename Int>Q dist(const Int& rhs) const;
    bool operator<(const Q& rhs) const;
    template<typename Int>bool operator<(const Int& b) const;
    bool operator>(const Q& rhs) const;
    template<typename Int>bool operator>(const Int& b) const;
    bool operator<=(const Q& rhs) const;
    template<typename Int>bool operator<=(const Int& b) const;
    bool operator>=(const Q& rhs) const;
    template<typename Int>bool operator>=(const Int& b) const;
    bool operator==(const Q& rhs) const;
    template<typename Int>bool operator==(const Int& b) const;
    bool operator!=(const Q& rhs) const;
    template<typename Int>bool operator!=(const Int& b) const;
    std::string to_string() const;
    std::string to_decimal() const;
    std::string to_decimal(Z precision) const;
    friend std::ostream& operator<<(std::ostream& os, const Q& a);
    friend std::istream& operator>>(std::istream& is, Q& a);
    operator Z() const;//round toward 0
    operator N() const;//round down or throw NegativeValue
    operator bool() const;
private:
    Z num;
    N den;
    void simplify();
    static const constexpr long double EPSILON = 0.0000000001;
};

N gcd(const N& a, const N& b);

Q::Q(): num(0), den(1){}//default construct 0
Q::Q(const std::string& str){
    int dotPos = -1;
    std::string a;
    for(size_t i = 0; i < str.size(); ++i){
        if(str[i] == '/'){
            num = str.substr(0, i);
            Z temp = str.substr(i + 1);
            if(temp > 0) den = temp;
            else{
                num = -num;
                den = -temp;
            }
            simplify();
            return;
        }
        else if(str[i] == '.'){
            dotPos = i;
            a = str.substr(0, dotPos);
        }
        else if(str[i] == '('){
            std::string b = str.substr(dotPos + 1, i - dotPos - 1);
            std::string c = str.substr(i + 1, str.size() - i - 2);
            std::string ab = a + b;
            std::string abc = ab + c;
            num = Z(abc) - Z(ab);
            den = (N(10)^(b.size()))*((N(10)^(c.size())) - N(1));
            simplify();
            return;
        }
    }
    if(dotPos == -1){
        num = str;
        den = 1;
        return;
    }
    std::string b = str.substr(dotPos + 1);
    num = a + b;
    den = N(10)^b.size();
    simplify();
}
Q::Q(const char* cstr): Q(std::string(cstr)){}
Q::Q(const Q& other): num(other.num), den(other.den){}
Q::Q(const Z& z): num(z), den(1){}
Q::Q(const N& n): num(n), den(1){}/*
Q::Q(const Z& z, const N& n): num(z), den(n){
    simplify();
}*/
Q::Q(const Z& z1, const Z& z2){
    if(z2 > 0) {
        num = z1;
        den = z2;
    } else {
        num = -z1;
        den = z2.abs();
    }
    simplify();
}
template<typename Int>
Q::Q(const Int& a){
    if constexpr(std::is_integral<Int>::value){
        num = a;
        den = 1;
        return;
    }
    else if constexpr(std::is_arithmetic<Int>::value){
        Int fracpart;
        Int intpart;
        Int trash;
        fracpart = modf(a, &intpart);
        Int b = fracpart;
        Int orgFrac = fracpart;
        N i = 1;
        long double ie = EPSILON;
        while(fabs(fracpart) >= ie){
            b += orgFrac;
            fracpart = modf(b, &trash);
            ++i;
            ie += EPSILON;
        }
        num = N(intpart)* i + b;
        den = i;
        simplify();
    }
    else throw NotANumber();
}
Q& Q::operator=(const Q& other){
    num = other.num;
    den = other.den;
    return *this;
}
template<typename Int>
Q& Q::operator=(const Int& a){
    return *this = Q(a);
}
Q Q::operator+(const Q& rhs) const{
    /*Q ratio(den , rhs.den);
    return Q(num * ratio.den + ratio.num * rhs.num, den * ratio.den);*/
    return Q(num * rhs.den + rhs.num * den, den * rhs.den);
}
template<typename Int>
Q Q::operator+(const Int& a) const{
    return *this + Q(a);
}
Q Q::operator-() const{
    Q res;
    res.num = -num;
    res.den = den;
    return res;
}
Q Q::operator-(const Q& rhs) const{
    /*Q ratio(den , rhs.den);
    return Q(num * ratio.den - ratio.num * rhs.num, den * ratio.den);*/
    return Q(num * rhs.den - rhs.num * den, den * rhs.den);
}
template<typename Int>
Q Q::operator-(const Int& a) const{
    return *this - Q(a);
}
//friend Q operator-(const Q&a, const Q&b);
Q Q::operator*(const Q& rhs) const{
    /*Q q1(num, rhs.den);
    Q q2(rhs.num, den);
    return Q(q1.num * q2.num, q1.den * q2.den);*/
    return Q(num * rhs.num, den * rhs.den);
}
template<typename Int>
Q Q::operator*(const Int& a) const{
    return *this * Q(a);
}
Q Q::operator/(const Q& rhs) const{
    /*Q q1(num, rhs.num);
    Q q2(Z(rhs.den), den);
    return Q(q1.num * q2.num, q1.den * q2.den);*/
    return Q(num * rhs.den, rhs.num * den);
}
template<typename Int>
Q Q::operator/(const Int& a) const{
    return *this / Q(a);
}
//Q operator%(const Q& rhs) const;//same sign with a
//template<typename Int>Q operator%(const Int& a) const;
Q Q::operator^(const Z& rhs) const{
    Q res;
    if(rhs >= 0){
        res.num = num ^ rhs;
        res.den = den ^ rhs;
    } else {
        if(!num) throw DivideBy0();
        if(num > 0 || rhs % 2 == 0){
            res.den = num ^ rhs.abs();
            res.num = den ^ rhs.abs();
        } else {
            res.den = num.abs() ^ rhs.abs();
            res.num = -Z(den ^ rhs.abs());
        }
    }
    return res;
}
template<typename Int>
Q Q::operator^(const Int& a) const{
    return *this ^ Z(a);
}
Q& Q::operator+=(const Q& rhs){
    return *this = *this + rhs;
}
//template<typename Int>Q& operator+=(const Int& a) const;
Q& Q::operator-=(const Q& rhs){
    return *this = *this - rhs;
}
//template<typename Int>Q& operator-=(const Int& a) const;
Q& Q::operator*=(const Q& rhs){
    return *this = *this * rhs;
}
//template<typename Int>Q& operator*=(const Int& a) const;
Q& Q::operator/=(const Q& rhs){
    return *this = *this/rhs;
}
//template<typename Int>Q& operator/=(const Int& a) const;
//Q& operator%=(const Q& rhs);
//template<typename Int>Q& operator%=(const Int& a) const;
Q& Q::operator^=(const Z& rhs){
    return *this = *this ^ rhs;
}
//template<typename Int>Q& operator^=(const Int& a) const;
Q& Q::operator++(){
    return *this += 1;
}
Q Q::operator++(int){
    Q copy = *this;
    ++(*this);
    return copy;
}
Q& Q::operator--(){
    return *this -= 1;
}
Q Q::operator--(int){
    Q copy = *this;
    --(*this);
    return copy;
}
Q Q::abs() const{
    Q res;
    res.num = num.abs();
    res.den = den;
    return res;
}
Q Q::dist(const Q& rhs) const{
    return (*this - rhs).abs();
}
template<typename Int>
Q Q::dist(const Int& rhs) const{
    return dist(Q(rhs));
}
Z Q::floor() const{
    if(den != 1 && num < 0)return num / den - 1;
    return num / den;
}
Z Q::ceil() const{
    if(num > 0 && den != 1) return num / den + 1;
    return num / den;
}
Z Q::numer() const{
    return num;
}
N Q::denom() const{
    return den;
}

bool Q::operator<(const Q& rhs) const{
    return num * rhs.den < rhs.num * den;
}
template<typename Int>
bool Q::operator<(const Int& b) const{
    return *this < Q(b);
}
bool Q::operator>(const Q& rhs) const{
    return num * rhs.den > rhs.num * den;
}
template<typename Int>
bool Q::operator>(const Int& b) const{
    return *this > Q(b);
}
bool Q::operator<=(const Q& rhs) const{
    return num * rhs.den <= rhs.num * den;
}
template<typename Int>
bool Q::operator<=(const Int& b) const{
    return *this <= Q(b);
}
bool Q::operator>=(const Q& rhs) const{
    return num * rhs.den >= rhs.num * den;
}
template<typename Int>
bool Q::operator>=(const Int& b) const{
    return *this >= Q(b);
}
bool Q::operator==(const Q& rhs) const{
    return num == rhs.num && den == rhs.den;
}
template<typename Int>
bool Q::operator==(const Int& b) const{
    return *this == Q(b);
}
bool Q::operator!=(const Q& rhs) const{
    return num != rhs.num || den != rhs.den;
}
template<typename Int>
bool Q::operator!=(const Int& b) const{
    return *this != Q(b);
}
std::string Q::to_string() const{
    if(den == 1) return num.to_string();
    return num.to_string() + "/" + den.to_string();
}
std::string Q::to_decimal() const{
    if(den == 1) return num.to_string();
    Z intPart = num / den;
    Q fracPart = *this - intPart;
    N den1 = den;
    N v2, v5;
    while(den1 % 2 == 0){
        den1/=2;
        ++v2;
    }
    while(den1 % 5 == 0){
        den1/=5;
        ++v5;
    }
    N nume = fracPart.num.abs();
    while(v2 < v5){
        ++v2;
        nume*=2;
    }
    while(v5 < v2){
        ++v5;
        nume*=5;
    }
    if(den1 == 1){
        std::string str = nume.to_string();
        while(v5 > str.size())str = "0" + str;
        return intPart.to_string() + "." + str;
    }
    N d;
    N tento = N(10) % den1;//improve?
    //BABY-STEP-GIANT-STEP
    std::unordered_map<N, N, N::hash> cache;
    N m = den1.sqrtc();
    //std::cout << "m = " << m << '\n';
    //vector<size_t> cache2();
    /*
    while(tento % den1 != 1){
        //std::cout << "here" << den1 << "\n";
        tento *= 10;
        ++d;
    }*/
    bool got = false;
    for(N i = 1; i < m; ++i){
        if(tento == 1){
            d = i;
            got = true;
            break;
        }
        cache[tento] = i;
        //std::cout << tento << ' ' << i << '\n';
        tento = (tento * 10) % den1;
    }
    if(!got){
        Z s = 0;
        Z old_s = 1;
        Z t = 1;
        Z old_t = 0;
        N r = tento;
        N old_r = den1;
        while(r){
            N quo = old_r / r;
            N temp = r;
            r = old_r - quo * r;
            old_r = temp;
            Z tempz = s;
            s = old_s - s * quo;
            old_s = tempz;
            tempz = t;
            t = old_t - t * quo;
            old_t = tempz;
        }
        //std::cout << den1 << ' ' << old_s << ' ' << tento << ' ' << old_t << '\n';
        while(old_t < 0) old_t += den1;
        //std::cout << "old_t " << old_t << '\n';
        N invm = old_t % den1;
        N invmto = invm;
        for(N j = 1; j < m; ++j){
            //std::cout << invmto << '\n';
            if(invmto == 1){
                d = m * j;
                got = true;
                break;
            }
            auto it = cache.find(invmto);
            if(it != cache.end()){
                d = it->second + m * j;
                got = true;
                break;
            }
            invmto = (invmto * invm) % den1;
        }
    }
    //if(!got) throw DivideBy0("buh");
    tento = N(10)^d; 
    N ni = tento - N(1);
    nume *= ni/den1;
    N b = nume / ni;
    N c = nume % ni;
    std::string bs = b.to_string();
    if(b == 0) bs = "";
    while(v5 > bs.size())bs = "0" + bs;
    std::string cs = c.to_string();
    while(d > cs.size()) cs = "0" + cs;
    return intPart.to_string() + '.' + bs + '(' + cs + ')';
}

std::string Q::to_decimal(Z precision) const{
    if(precision > 0){
        Z intPart = *this;
        Q fracPart = (*this - intPart).abs();
        std::string str = intPart.to_string() + '.';
        while(precision > 0){
            N deduce = precision;
            bool done = false;
            while(deduce > 0){
                try{
                    fracPart *= (N(10)^deduce);
                    N after = fracPart;
                    std::string af = after.to_string();
                    while(deduce > af.size())af = '0' + af;
                    str += af;
                    precision -= deduce;
                    fracPart -= after;
                    done = true;
                    break;
                } catch(const std::exception& e) {
                    /*fracPart *= 10;//improve? binary search?
                    N di = fracPart;
                    str += di.to_string();
                    fracPart -= di;
                    --precision;*/
                    deduce/= 2;
                }
            }
            if(!done){
                //std::cout << *this << ' ' << precision << '\n';
                //std::cout << deduce << '\n';
                //WHAT SHOULD BE THROWN????
                throw std::exception();
            }
        }
        return str;
    } else {
        N ab = precision.abs();
        Z res =  *this / (N(10)^ab);
        std::string str = res.to_string();
        for(;ab >0;--ab)str += '0';
        return str;
    }
}
std::ostream& operator<<(std::ostream& os, const Q& a){
    return os << a.to_string();
}
std::istream& operator>>(std::istream& is, Q& a){
    std::string str;
    is >> str;
    a = str;
    return is;
}
Q::operator Z() const{
    return num / den;
}
Q::operator N() const{
    return num / den;
}
Q::operator bool() const{
    return num;
}

N gcd(const N& a, const N& b){
    if(b == 0) return a;
    return gcd(b, a%b);
}

void Q::simplify(){
    if(den == 0) throw DivideBy0();
    N d = gcd(num.abs(), den);
    num/=d;
    den/=d;
}

#endif