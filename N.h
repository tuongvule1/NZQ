#ifndef N_H
#define N_H
#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <type_traits>

//compiles with C++17
//custom exception types
class NegativeValue: public std::exception{
    public:
    const char * what () const noexcept override {
        return msg.c_str();
    }
    const std::string msg;
    NegativeValue(){}
    NegativeValue(const std::string &msg) : msg(msg) {}
};
class NotANumber: public std::exception{
    public:
    const char * what () const noexcept override {
        return msg.c_str();
    }
    const std::string msg;
    NotANumber(){}
    NotANumber(const std::string &msg) : msg(msg) {}
};
class DivideBy0: public std::exception{
    public:
    const char * what () const noexcept override {
        return msg.c_str();
    }
    const std::string msg;
    DivideBy0(){}
    DivideBy0(const std::string &msg) : msg(msg) {}
};


class N{
public:
    
    N();//default construct 0
    N(const std::string& str);
    N(const char* cstr);//C-style string
    N(const N& other);
    template<typename Int>N(const Int& a);
    /*
    template <typename Int,
              std::enable_if_t<std::is_integral<Int>::value, bool> = true 
    >
    N(const Int& a);//constructor from integral type
    template <typename Float,
              std::enable_if_t<std::is_floating_point<Float>::value, bool> = true
    >
    N(const Float& a);//constructor from floating point type
    template <typename Other,
              std::enable_if_t<!std::is_arithmetic<Other>::value, bool> = true
    >
    N(const Other& a);//contructor from other types*/
    //N(const long long& a);
    N& operator=(const N& other);
    template<typename Int>N& operator=(const Int& a);
    N operator+(const N& rhs) const;
    template<typename Int>N operator+(const Int& a) const;
    N operator*(const N& rhs) const;
    template<typename Int>N operator*(const Int& a) const;
    N operator/(const N& rhs) const;
    template<typename Int>N operator/(const Int& a) const;
    N operator%(const N& rhs) const;
    template<typename Int>N operator%(const Int& a) const;
    N operator^(const N& rhs) const;
    template<typename Int>N operator^(const Int& a) const;
    N& operator+=(const N& rhs);
    N& operator-=(const N& rhs);
    N& operator*=(const N& rhs);
    N& operator/=(const N& rhs);
    N& operator%=(const N& rhs);
    N& operator^=(const N& rhs);
    N& operator++();
    N operator++(int);
    N& operator--();
    N operator--(int);
    N dist(const N& rhs) const;//return |a-b|
    N factorial() const;
    N sqrtf() const;//floor of square root
    N sqrtc() const;//ceil of square root
    template<typename Int>N dist(const Int& rhs) const;
    bool operator<(const N& rhs) const;
    template<typename Int>bool operator<(const Int& b) const;
    bool operator>(const N& rhs) const;
    template<typename Int>bool operator>(const Int& b) const;
    bool operator<=(const N& rhs) const;
    template<typename Int>bool operator<=(const Int& b) const;
    bool operator>=(const N& rhs) const;
    template<typename Int>bool operator>=(const Int& b) const;
    bool operator==(const N& rhs) const;
    template<typename Int>bool operator==(const Int& b) const;
    bool operator!=(const N& rhs) const;
    template<typename Int>bool operator!=(const Int& b) const;
    std::string to_string() const;
    friend std::ostream& operator<<(std::ostream& os, const N& a);
    friend std::istream& operator>>(std::istream& is, N& a);
    operator bool() const;
    class hash;
    /*void printVec(){
        //debuging
        for(size_t i = 0; i < vec.size(); ++i){
            std::cout << vec[i] << ' ';
        }
        std::cout << '\n';
    }*/
private:
    std::vector<size_t> vec;
    N(const std::vector<size_t>& v): vec(v){}
    static const size_t DIGITS = log10(SIZE_MAX) / 2;
    static const size_t BASE = pow(10, DIGITS);
    static void addVec(const std::vector<size_t>& lon, const std::vector<size_t>& sho,
     std::vector<size_t>& res, size_t k);
    static void minusVec(const std::vector<size_t>& lon, const std::vector<size_t>& sho,
     std::vector<size_t>& res, size_t k);
    static size_t divide(std::vector<size_t>& dividend, size_t k, const std::vector<size_t>& divisor);
    static void trimVec(std::vector<size_t>& v);
};

N::N(){}//default construct 0
N::N(const std::string& str){
    size_t len = str.size();
    size_t vecLen = len/DIGITS;
    if(len % DIGITS) ++vecLen;
    vec.resize(vecLen);
    for(size_t i = 0; i < len; ++i){
        size_t j = len - i - 1;
        if(str[j] > '9' || str[j] < '0') throw NotANumber();
        vec[i/DIGITS] += pow(10, i % DIGITS) * (str[j] - '0'); 
    }
    trimVec(vec);
}
N::N(const char* cstr): N(std::string(cstr)){}
N::N(const N& other){
    vec = other.vec;
}

/*
template <typename Int,
            std::enable_if_t<std::is_integral<Int>::value, bool> = true
>
N::N(const Int& a){
    if(a < 0) throw NegativeValue();
    auto b = a;
    while(b > 0){
        vec.push_back(b % BASE);
        b = b/BASE;
    }
    //trimVec(vec);
}

template <typename Float,
            std::enable_if_t<std::is_floating_point<Float>::value, bool> = true
>
N::N(const Float& a){
    if(a < 0) throw NegativeValue();
    size_t b = static_cast<size_t>(a);
    while(b > 0){
        vec.push_back(b % BASE);
        b = b/BASE;
    }
}
template<typename Other, 
std::enable_if_t<!std::is_arithmetic<Other>::value, bool> = true
>
N::N(const Other& a){
    N temp = a;
    *this = temp;
    return;
}*/

template<typename Int> 
N::N(const Int& a){
    if constexpr(!std::is_arithmetic<Int>::value){
        N temp = a;
        *this = temp;
    } 
    else {
        if(a < 0) throw NegativeValue();
        if constexpr(!std::is_integral<Int>::value){
            size_t b = static_cast<size_t>(a);
            while(b > 0){
                vec.push_back(b % BASE);
                b = b/BASE;
            }
        } else {
            auto b = a;
            while(b > 0){
                vec.push_back(b % BASE);
                b = b/BASE;
            }
        }  
    }
    //trimVec(vec);
}

/*
template<typename Int> 
N::N(const Int& a){
    if(a < 0) throw NegativeValue();
    size_t b = static_cast<size_t>(a);
    while(b > 0){
        vec.push_back(b % BASE);
        b = b/BASE;
    }
    //trimVec(vec);
}*/

N& N::operator=(const N& other){
    vec = other.vec;
    return *this;
}
template<typename Int>
N& N::operator=(const Int& a){
    return (*this) = N(a);
}
N N::operator+(const N& rhs) const{
    N res;
    if(vec.size() < rhs.vec.size()) addVec(rhs.vec, vec, res.vec, 0u);
    else addVec(vec, rhs.vec, res.vec, 0u);
    return res;
}
template<typename Int>
N N::operator+(const Int& a) const{
    return *this + N(a);
}
N N::operator*(const N& rhs) const{
    N prod;
    if(!vec.size() || !rhs.vec.size()) return prod;
    prod.vec.resize(vec.size() + rhs.vec.size());
    for(size_t i = 0; i < vec.size(); ++i){
        for(size_t j = 0; j < rhs.vec.size(); ++j){
            N prodAdd = vec[i] * rhs.vec[j]; 
            addVec(prod.vec, prodAdd.vec, prod.vec, i + j);
        }
    }
    while(!prod.vec.back())prod.vec.pop_back();
    return prod;
}
template<typename Int>
N N::operator*(const Int& a) const{
    return (*this) * N(a);
}
N N::operator/(const N& rhs) const{
    std::vector<size_t> vec2 = vec;
    N res;
    size_t len = vec.size();
    res.vec.resize(len);
    for(size_t i = len - rhs.vec.size() + 1; i-- > 0;){
        size_t div = divide(vec2, i, rhs.vec);
        res.vec[i] = div;
    }
    trimVec(res.vec);
    return res;
}
template<typename Int>N N::operator/(const Int& a) const{
    return (*this) / N(a);
}
N N::operator%(const N& rhs) const{
    std::vector<size_t> vec2 = vec;
    N res;
    size_t len = vec.size();
    res.vec.resize(len);
    for(size_t i = len - rhs.vec.size() + 1; i-- > 0;){
        size_t div = divide(vec2, i, rhs.vec);
        res.vec[i] = div;
    }
    return N(vec2);
}
template<typename Int>N N::operator%(const Int& a) const{
    return (*this) % N(a);
}
N N::operator^(const N& rhs) const{
    if(!rhs.vec.size()) return 1;
    if(*this == 0 || *this == 1) return *this;
    std::vector<N> powers;
    std::vector<N> res;
    powers.push_back(1);
    res.push_back(*this);
    powers.push_back(2);
    while(powers.back() <= rhs){
        powers.push_back(powers.back() + powers.back());
        res.push_back(res.back() * res.back());
    }
    N finalRes = 1;
    N remaining = rhs;
    size_t s = powers.size() - 2;
    while(remaining > 0){
        while(powers[s] > remaining)--s;
        finalRes *= res[s];
        remaining -= powers[s];
    }
    return finalRes;
}
template<typename Int>
N N::operator^(const Int& a) const{
    return (*this) ^ N(a);
}

N& N::operator+=(const N& rhs){
    if(vec.size() < rhs.vec.size())addVec(rhs.vec, vec, vec, 0u);
    else addVec(vec, rhs.vec, vec, 0u);
    return *this;
}
N& N::operator-=(const N& rhs){
    minusVec(vec, rhs.vec, vec, 0u);
    return *this;
}
N& N::operator*=(const N& rhs){
    *this = (*this) * rhs;
    return *this;
}
N& N::operator/=(const N& rhs){
    *this = (*this) / rhs;
    return *this;
}
N& N::operator%=(const N& rhs){
    *this = (*this) % rhs;
    return *this;
}
N& N::operator^=(const N& rhs){
    *this = (*this)^rhs;
    return *this;
}
N& N::operator++(){
    return (*this) += N(1);
}
N N::operator++(int){
    N copy = *this;
    ++(*this);
    return copy;
}
N& N::operator--(){
    return (*this)-= N(1);
}
N N::operator--(int){
    N copy = *this;
    --(*this);
    return copy;
}
N N::dist(const N& rhs) const{
    N res;
    if(*this < rhs) minusVec(rhs.vec, vec, res.vec, 0u);
    else minusVec(vec, rhs.vec, res.vec, 0u);
    return res;
}
template<typename Int>
N N::dist(const Int& b) const{
    return dist(N(b));
}
N N::factorial() const{
    N fact = 1;
    for(N i = 1; i <= *this; ++i){
        fact *= i;
    }
    return fact;
}

N N::sqrtf() const{
    /*N res;
    N temp = *this;
    size_t len = vec.size();
    res.vec.resize((len + 1) / 2);
    size_t pos = res.vec.size() - 1;
    for(size_t i = len + len % 2 - 2; i > 0; i-= 2){
        //size_t cur = temp.vec[i];
        //if(i + 1 < temp.vec.size()) cur += temp.vec[i+1] * BASE;
        size_t low = 0;
        size_t high = static_cast<size_t>(std::sqrt(cur)) + 1;
        while(){
            size_t mid = (low + high)/2;
            size_t sq = mid * mid;
        }
    }*/
    if(!vec.size()) return 0;
    N res = 1;
    while(true){
        N temp = *this / res;
        if(res <= temp){
            if(temp <= res + 1) return res;
        }
        else if(temp + 1 == res){
            return temp;
        }
        res = (res + temp)/2;
    }
}

N N::sqrtc() const{
    if(!vec.size()) return 0;
    N res = 1;
    N minusOne;
    std::vector<size_t> one = {1u};
    minusVec(vec, one, minusOne.vec, 0u);
    while(true){
        N temp = minusOne / res;
        if(res <= temp){
            if(temp <= res + 1) return res + 1;
        }
        else if(temp + 1 == res){
            return res;
        }
        res = (res + temp)/2;
    }
}

bool N::operator<(const N& rhs) const{
    if(vec.size() < rhs.vec.size())return true;
    if(rhs.vec.size() < vec.size())return false;
    for(size_t i = vec.size(); i-- >0;){
        if(vec[i] < rhs.vec[i]) return true;
        if(vec[i] > rhs.vec[i]) return false;
    }
    return false;
}
template<typename Int>
bool N::operator<(const Int& b) const{
    return (*this) < N(b);
}
bool N::operator>(const N& rhs) const{
    if(vec.size() < rhs.vec.size())return false;
    if(rhs.vec.size() < vec.size())return true;
    for(size_t i = vec.size(); i-- >0;){
        if(vec[i] < rhs.vec[i]) return false;
        if(vec[i] > rhs.vec[i]) return true;
    }
    return false;
}
template<typename Int>
bool N::operator>(const Int& b) const{
    return (*this) > N(b);
}
bool N::operator<=(const N& rhs) const{
    if(vec.size() < rhs.vec.size())return true;
    if(rhs.vec.size() < vec.size())return false;
    for(size_t i = vec.size(); i-- >0;){
        if(vec[i] < rhs.vec[i]) return true;
        if(vec[i] > rhs.vec[i]) return false;
    }
    return true;
}
template<typename Int>
bool N::operator<=(const Int& b) const{
    return (*this) <= N(b);
}
bool N::operator>=(const N& rhs) const{
    if(vec.size() < rhs.vec.size())return false;
    if(rhs.vec.size() < vec.size())return true;
    for(size_t i = vec.size(); i-- >0;){
        if(vec[i] < rhs.vec[i]) return false;
        if(vec[i] > rhs.vec[i]) return true;
    }
    return true;
}
template<typename Int>
bool N::operator>=(const Int& b) const{
    return (*this)>=N(b);
}
bool N::operator==(const N& rhs) const{
    return vec == rhs.vec;
}
template<typename Int>
bool N::operator==(const Int& b) const{
    //std::cout << "comparing\n";
    return (*this) == N(b);
}
bool N::operator!=(const N& rhs) const{
    return !(*this == rhs);
}
template<typename Int>
bool N::operator!=(const Int& b) const{
    return *this != N(b);
}
std::string N::to_string() const{
    std::string res = "";
    if(!vec.size()) return "0";
    for(size_t i = vec.size(); i-- > 0;){
        std::string s = std::to_string(vec[i]);
        if(i != vec.size() - 1) while(s.size() < DIGITS) s = '0' + s;
        res += s;
    }
    return res;
}
std::ostream& operator<<(std::ostream& os, const N& a){
    return os << a.to_string();
}
std::istream& operator>>(std::istream& is, N& a){
    std::string str;
    is >> str;
    a = N(str);
    return is;
}
N::operator bool() const{
    return vec.size();
}


void N::addVec(const std::vector<size_t>& lon,
 const std::vector<size_t>& sho, std::vector<size_t>& res, size_t k){
    if(&lon != &res){
        res.resize(lon.size());
        for(size_t i = 0; i < k; ++i){
            res[i] = lon[i];
        }
    }
    bool re = false;
    for(size_t i = k; i < lon.size(); ++i){
        size_t num1 = re ? 1u + lon[i]: lon[i];
        size_t num2 = 0u;
        size_t j = i - k;
        bool in = j < sho.size();
        if(in) num2 = sho[j];
        size_t remaining = BASE - num1;
        if(num2 < remaining){
            res[i] = num1 + num2;
            re = false;
        } else {
            res[i] = num2 - remaining;
            re = true;
        }
        if(!re && !in){
            if(&lon != &res){
                for(size_t j = i + 1; j < lon.size(); ++j){
                    res[j] = lon[j];
                }
            }
            return;
        }
    }
    if(re) res.push_back(1u);
}

size_t N::divide(std::vector<size_t>& dividend, size_t k, const std::vector<size_t>& divisor){
    if(divisor.size() == 0u) throw DivideBy0("Divide "+ N(dividend).to_string()+ " by 0");
    N dvd;
    size_t len = dividend.size() - k;
    if(len < divisor.size()) return 0u;
    dvd.vec.resize(len);
    for(size_t i = 0; i < len; ++i){
        dvd.vec[i] = dividend[i + k];
    }
    size_t low;
    size_t high;
    if(len == divisor.size()){
        high = dividend.back()/divisor.back();
        low = dividend.back()/(divisor.back() + 1u);
    } else {
        size_t dvdp = dividend.back() * BASE + dividend[dividend.size() - 2];
        high = dvdp / divisor.back();
        low = dvdp / (divisor.back() + 1u);
    }
    ++high;//make high exclusive
    while(low + 1u < high){
        size_t mid = (low + high)/2;
        N tried = N(divisor) * N(mid);
        if(tried > dvd) high = mid;
        else if(tried < dvd) low = mid;
        else {
            minusVec(dividend, tried.vec, dividend, k);
            trimVec(dividend);
            return mid;
        }
    }
    N tried = N(divisor) * N(low);
    minusVec(dividend, tried.vec, dividend, k);
    trimVec(dividend);
    return low;
}

void N::minusVec(const std::vector<size_t>& lon,
 const std::vector<size_t>& sho, std::vector<size_t>& res, size_t k){
    if(&lon != &res){
        res.resize(lon.size());
        for(size_t i = 0; i < k; ++i){
            res[i] = lon[i];
        }
    }
    bool re = false;
    for(size_t i = k; i < lon.size(); ++i){
        size_t num1 = lon[i];
        size_t num2 = re? 1u: 0u;
        size_t j = i - k;
        bool in = j < sho.size();
        if(in) num2 += sho[j];
        if(num1 >= num2){
            res[i] = num1 - num2;
            re = false;
        } else {
            res[i] = BASE - num2 + num1;
            re = true;
        }
        if(!re && !in){
            if(&lon != &res){
                for(size_t j = i + 1; j < lon.size(); ++j){
                    res[j] = lon[j];
                }
                trimVec(res);
            }
            return;
        }
    }
    if(re) throw NegativeValue();
    trimVec(res);
}

void N::trimVec(std::vector<size_t>& v){
    while(v.size()){
        if(v.back()) break;
        v.pop_back();
    }
}

class N::hash{
    public:
    size_t operator()(const N& n) const{
        size_t len = n.vec.size();
        if(len == 0) return 0;
        else if(len == 1) return n.vec[0];
        else return n.vec[1] * BASE + n.vec[0];
    }
};



#endif