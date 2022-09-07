#ifndef Z_H
#define Z_H
#include "N.h"

class Z{
public:
    Z();//default construct 0
    Z(const std::string& str);
    Z(const char* cstr);//C-style string
    Z(const Z& other);
    Z(const N& a);
    template<typename Int>Z(const Int& a);
    Z& operator=(const Z& other);
    template<typename Int>Z& operator=(const Int& a);
    Z operator+(const Z& rhs) const;
    template<typename Int>Z operator+(const Int& a) const;
    Z operator-() const;
    Z operator-(const Z& rhs) const;
    template<typename Int>Z operator-(const Int& a) const;
    friend Z operator-(const N&a, const N&b);
    Z operator*(const Z& rhs) const;
    template<typename Int>Z operator*(const Int& a) const;
    Z operator/(const Z& rhs) const;//round toward 0;
    template<typename Int>Z operator/(const Int& a) const;
    Z operator%(const Z& rhs) const;//same sign with a
    template<typename Int>Z operator%(const Int& a) const;
    Z operator^(const N& rhs) const;
    template<typename Int>Z operator^(const Int& a) const;
    Z& operator+=(const Z& rhs);
    Z& operator-=(const Z& rhs);
    Z& operator*=(const Z& rhs);
    Z& operator/=(const Z& rhs);
    Z& operator%=(const Z& rhs);
    Z& operator^=(const N& rhs);
    Z& operator++();
    Z operator++(int);
    Z& operator--();
    Z operator--(int);
    N abs() const;
    N dist(const Z& rhs) const;//return |a-b|
    template<typename Int>N dist(const Int& rhs) const;
    bool operator<(const Z& rhs) const;
    template<typename Int>bool operator<(const Int& b) const;
    bool operator>(const Z& rhs) const;
    template<typename Int>bool operator>(const Int& b) const;
    bool operator<=(const Z& rhs) const;
    template<typename Int>bool operator<=(const Int& b) const;
    bool operator>=(const Z& rhs) const;
    template<typename Int>bool operator>=(const Int& b) const;
    bool operator==(const Z& rhs) const;
    template<typename Int>bool operator==(const Int& b) const;
    bool operator!=(const Z& rhs) const;
    template<typename Int>bool operator!=(const Int& b) const;
    std::string to_string() const;
    friend std::ostream& operator<<(std::ostream& os, const Z& a);
    friend std::istream& operator>>(std::istream& is, Z& a);
    operator N() const;
    operator bool() const;
private:
    N n;
    bool sign;
    Z(const N& a, bool s): n(a), sign(s){}
};

Z::Z(): sign(true){}
Z::Z(const std::string& str): sign(true){
    size_t i;
    for(i = 0; i < str.size(); ++i){
        if(str[i] == '-') sign = !sign;
        else if(str[i] == '+') continue;
        else break;
    }
    if(i > 0)n = str.substr(i);
    else n = str;
}
Z::Z(const char* cstr): Z(std::string(cstr)){}
Z::Z(const Z& other): n(other.n), sign(other.sign){}
Z::Z(const N& a): n(a), sign(true){}
template<typename Int>
Z::Z(const Int& a): sign(a >= 0){
    if(sign) n = a;
    else n = -a;
} 
Z& Z::operator=(const Z& other){
    n = other.n;
    sign = other.sign;
    return *this;
}
template<typename Int>
Z& Z::operator=(const Int& a){
    return *this = Z(a);
}
Z Z::operator+(const Z& rhs) const{
    if(sign == rhs.sign) return Z(n + rhs.n, sign);
    if(rhs.n > n) return Z(rhs.n.dist(n), !sign);
    return Z(n.dist(rhs.n), sign);
}
template<typename Int>
Z Z::operator+(const Int& a) const{
    return *this + Z(a);
}
Z Z::operator-() const{
    return Z(n, !sign);
}
Z Z::operator-(const Z& rhs) const{
    if(sign != rhs.sign) return Z(n + rhs.n, sign);
    if(rhs.n > n) return Z(rhs.n.dist(n), !sign);
    return Z(n.dist(rhs.n), sign);
}
template<typename Int>
Z Z::operator-(const Int& a) const{
    return *this - Z(a);
}
Z Z::operator*(const Z& rhs) const{
    return Z(n * rhs.n, sign == rhs.sign);
}
template<typename Int>
Z Z::operator*(const Int& a) const{
    return (*this) * Z(a);
}
Z Z::operator/(const Z& rhs) const{
    return Z(n / rhs.n, sign == rhs.sign);
}
template<typename Int>
Z Z::operator/(const Int& a) const{
    return *this / Z(a);
}
Z Z::operator%(const Z& rhs) const{
    return Z(n % rhs.n, sign);
}
template<typename Int>
Z Z::operator%(const Int& a) const{
    return *this % Z(a);
}
Z Z::operator^(const N& rhs) const{
    return Z(n ^ rhs, sign || rhs % 2 == 0);
}
template<typename Int>
Z Z::operator^(const Int& a) const{
    N b = a;
    return *this ^ b;
}
Z& Z::operator+=(const Z& rhs){
    if(sign == rhs.sign) n += rhs.n;
    else if(n >= rhs.n) n -= rhs.n;
    else {
        n = rhs.n.dist(n);
        sign = !sign;
    }
    return *this;
}
//template<typename Int>Z& operator+=(const Int& a) const;
Z& Z::operator-=(const Z& rhs){
    if(sign != rhs.sign) n += rhs.n;
    else if(n >= rhs.n) n -= rhs.n;
    else {
        n = rhs.n.dist(n);
        sign = !sign;
    }
    return *this;
}
//template<typename Int>Z& operator-=(const Int& a) const;
Z& Z::operator*=(const Z& rhs){
    return *this = *this * rhs;
}
//template<typename Int>Z& operator*=(const Int& a) const;
Z& Z::operator/=(const Z& rhs){
    return *this = *this / rhs;
}
//template<typename Int>Z& operator/=(const Int& a) const;
Z& Z::operator%=(const Z& rhs){
    return *this = *this % rhs;
}
//template<typename Int>Z& operator%=(const Int& a) const;
Z& Z::operator^=(const N& rhs){
    return *this = *this ^rhs;
}
//template<typename Int>Z& operator^=(const Int& a) const;
Z& Z::operator++(){
    return *this += 1;
}
Z Z::operator++(int){
    Z copy = *this;
    ++(*this);
    return copy;
}
Z& Z::operator--(){
    return *this -= 1;
}
Z Z::operator--(int){
    Z copy = *this;
    --(*this);
    return copy;
}

N Z::abs() const{
    return n;
}

N Z::dist(const Z& rhs) const{
    return (*this - rhs).n;
}
template<typename Int>
N Z::dist(const Int& rhs) const{
    return dist(Z(rhs));
}
bool Z::operator<(const Z& rhs) const{
    if(sign < rhs.sign) return n || rhs.n;
    else if(sign > rhs.sign) return false;
    else if(sign) return n < rhs.n;
    else return rhs.n < n;
}
template<typename Int>
bool Z::operator<(const Int& b) const{
    return *this < Z(b);
}
bool Z::operator>(const Z& rhs) const{
    if(sign > rhs.sign) return n || rhs.n;
    else if(sign < rhs.sign) return false;
    else if(sign) return n > rhs.n;
    else return rhs.n > n;
}
template<typename Int>
bool Z::operator>(const Int& b) const{
    return *this > Z(b);
}
bool Z::operator<=(const Z& rhs) const{
    if(sign < rhs.sign) return true;
    else if(sign > rhs.sign) return n == 0 && rhs.n == 0;
    else if(sign) return n <= rhs.n;
    else return rhs.n <= n;
}
template<typename Int>
bool Z::operator<=(const Int& b) const{
    return *this <= Z(b);
}
bool Z::operator>=(const Z& rhs) const{
    if(sign > rhs.sign) return true;
    else if(sign < rhs.sign) return n == 0 && rhs.n == 0;
    else if(sign) return n >= rhs.n;
    else return rhs.n >= n;
}
template<typename Int>
bool Z::operator>=(const Int& b) const{
    return *this >= Z(b);
}
bool Z::operator==(const Z& rhs) const{
    return n == rhs.n && (sign == rhs.sign || n == 0);
}
template<typename Int>
bool Z::operator==(const Int& b) const{
    return *this == Z(b);
}
/*
bool operator==(const N& a, const Z& b){
    if(b < 0)return false;
    return a == b.abs();
}*/
bool Z::operator!=(const Z& rhs) const{
    return n != rhs.n || (sign != rhs.sign && n);
}
template<typename Int>
bool Z::operator!=(const Int& b) const{
    return *this != Z(b);
}
std::string Z::to_string() const{
    if(sign || n == 0) return n.to_string();
    return '-' + n.to_string();
}
std::ostream& operator<<(std::ostream& os, const Z& a){
    return os << a.to_string();
}
std::istream& operator>>(std::istream& is, Z& a){
    std::string str;
    is >> str;
    a = Z(str);
    return is;
}

Z::operator N() const{
    if(!sign && n) throw NegativeValue();
    //std::cout << "operator used" << '\n';
    return n;
}

Z::operator bool() const{
    return n;
}

Z operator-(const N&a, const N&b){
    if(a < b) return Z(a.dist(b), false);
    return Z(a.dist(b), true);
}
/*
template<typename Int>
Z operator-(const N&a, const Int& b){
    if constexpr(std::is_integral<Int>::value)return a - N(b);
}
*/
#endif