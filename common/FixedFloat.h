#pragma once
#include <string>
#include <stdexcept>
#include <cmath>
#include "CommonTypeDefs.h"

const unsigned int MAX_DECIMAL_POINT = 9;
const _int64 MAX_DECIMAL_POINT_MOD_VALUE = pow(10, MAX_DECIMAL_POINT);

struct FixedFloatHash;
class FixedFloat
{
	enum {FP_PLUS, FP_MINUS};
	public:	
		friend struct FixedFloatHash;
		FixedFloat():value_(0){}
		explicit FixedFloat(double value){set_value(value);}
		explicit FixedFloat(_int64 value){set_value(value);}
		FixedFloat(char sign, unsigned int integer_value, unsigned int decimal_value, unsigned short decimal_point = MAX_DECIMAL_POINT)
			{set_value(sign, integer_value, decimal_value, decimal_point);}
		FixedFloat(int value, unsigned char decimal_point) {set_value(value, decimal_point);}
		FixedFloat(const char value[], unsigned int value_len, unsigned char decimal_point, char sign = 0) {set_value(value, value_len, decimal_point, sign);}
		FixedFloat(const std::string& value){set_value(value);}

		FixedFloat& operator=(const FixedFloat& rhs) {
			if (this != &rhs) {
				this->value_ = rhs.value_;	
			}
			return *this;
		}

		FixedFloat& operator=(FixedFloat&& rhs) {
			this->value_ = rhs.value_;
			return *this;
		}

		FixedFloat(const FixedFloat& rhs):
			value_(rhs.value_){}

		FixedFloat(FixedFloat&& rhs):
			value_(rhs.value_){}

		template <typename T>
		FixedFloat& operator=(const T& rhs){
			*this = FixedFloat(static_cast<double>(rhs));
			return *this;
		}
		FixedFloat& operator=(const char* rhs){
			*this = FixedFloat(rhs);
			return *this;
		}
		template <typename T>
		FixedFloat operator+(const T& rhs) const{
			return *this + FixedFloat(static_cast<double>(rhs));
		}

		FixedFloat operator+(const char* rhs) const{
			return *this + FixedFloat(rhs);
		}

		template <typename T>
		FixedFloat operator-(const T& rhs) const{
			return *this - FixedFloat(static_cast<double>(rhs));
		}
		FixedFloat operator-(const char* rhs) const{
			return *this - FixedFloat(rhs);
		}
		template <typename T>
		FixedFloat operator*(const T& rhs) const{
			return this->operator*(FixedFloat(static_cast<double>(rhs)));
		}
		FixedFloat operator*(const char* rhs) const{
			return this->operator*(FixedFloat(rhs));
		}
		template <typename T>
		FixedFloat operator/(const T& rhs) const{
			return this->operator/(FixedFloat(static_cast<double>(rhs)));
		}
		FixedFloat operator/(const char* rhs) const{
			return this->operator/(FixedFloat(rhs));
		}

		template <typename T>
		FixedFloat& operator+=(const T& rhs){
			*this += FixedFloat(static_cast<double>(rhs));
			return *this;
		}
		template <typename T>
		FixedFloat& operator-=(const T& rhs){
			*this -= FixedFloat(static_cast<double>(rhs));
			return *this;
		}
		template <typename T>
		FixedFloat& operator*=(const T& rhs){
			return this->operator*=(FixedFloat(static_cast<double>(rhs)));
		}
		template <typename T>
		FixedFloat& operator/=(const T& rhs){
			return this->operator/=(FixedFloat(static_cast<double>(rhs)));
		}
		FixedFloat& operator+=(const char* rhs){
			*this += FixedFloat(rhs);
			return *this;
		}
		FixedFloat& operator-=(const char* rhs){
			*this -= FixedFloat(rhs);
			return *this;
		}
		FixedFloat& operator*=(const char* rhs){
			return this->operator*=(FixedFloat(rhs));
		}
		FixedFloat& operator/=(const char* rhs){
			return this->operator/=(FixedFloat(rhs));
		}


		FixedFloat operator+(const FixedFloat& rhs) const;
		FixedFloat operator-(const FixedFloat& rhs) const;
		FixedFloat operator*(const FixedFloat& rhs) const;
		FixedFloat operator/(const FixedFloat& rhs) const;

		FixedFloat operator+(FixedFloat&& rhs) const;
		FixedFloat operator-(FixedFloat&& rhs) const;
		FixedFloat operator*(FixedFloat&& rhs) const;
		FixedFloat operator/(FixedFloat&& rhs) const;

		FixedFloat& operator+=(const FixedFloat& rhs);
		FixedFloat& operator-=(const FixedFloat& rhs);
		FixedFloat& operator*=(const FixedFloat& rhs);
		FixedFloat& operator/=(const FixedFloat& rhs);
	
		FixedFloat& operator+=(FixedFloat&& rhs);
		FixedFloat& operator-=(FixedFloat&& rhs);
		FixedFloat& operator*=(FixedFloat&& rhs);
		FixedFloat& operator/=(FixedFloat&& rhs);
		
		bool operator<(const FixedFloat& rhs) const;
		bool operator>(const FixedFloat& rhs) const;
		bool operator<=(const FixedFloat& rhs) const;
		bool operator>=(const FixedFloat& rhs) const;
		bool operator==(const FixedFloat& rhs) const;		
		bool operator!=(const FixedFloat& rhs) const;		

		bool operator<(FixedFloat&& rhs) const;
		bool operator>(FixedFloat&& rhs) const;
		bool operator<=(FixedFloat&& rhs) const;
		bool operator>=(FixedFloat&& rhs) const;
		bool operator==(FixedFloat&& rhs) const;		
		bool operator!=(FixedFloat&& rhs) const;		

		template <typename T>
		friend FixedFloat operator+(const T& lhs, const FixedFloat& rhs);
		friend FixedFloat operator+(const char* lhs, const FixedFloat& rhs);

		template <typename T>
		friend FixedFloat operator-(const T& lhs, const FixedFloat& rhs);
		friend FixedFloat operator-(const char* lhs, const FixedFloat& rhs);

		template <typename T>
		friend FixedFloat operator*(const T& lhs, const FixedFloat& rhs);
		friend FixedFloat operator*(const char* lhs, const FixedFloat& rhs);

		template <typename T>
		friend FixedFloat operator/(const T& lhs, const FixedFloat& rhs);
		friend FixedFloat operator/(const char* lhs, const FixedFloat& rhs);

		template <typename T>
		friend bool operator<(const FixedFloat& lhs, const T& rhs);		

		template <typename T>
		friend bool operator>(const FixedFloat& lhs, const T& rhs);
		
		template <typename T>
		friend bool operator<=(const FixedFloat& lhs, const T& rhs);
		
		template <typename T>
		friend bool operator>=(const FixedFloat& lhs, const T& rhs);		

		template <typename T>
		friend bool operator==(const FixedFloat& lhs, const T& rhs);
		
		template <typename T>
		friend bool operator!=(const FixedFloat& lhs, const T& rhs);
		
		template <typename T>
		friend bool operator<(const T& lhs, const FixedFloat& rhs);
		
		template <typename T>
		friend bool operator>(const T& lhs, const FixedFloat& rhs);

		template <typename T>
		friend bool operator<=(const T& lhs, const FixedFloat& rhs);

		template <typename T>
		friend bool operator>=(const T& lhs, const FixedFloat& rhs);
		
		template <typename T>
		friend bool operator==(const T& lhs, const FixedFloat& rhs);

		template <typename T>
		friend bool operator!=(const T& lhs, const FixedFloat& rhs);
		
		friend bool operator<(const char* lhs, const FixedFloat& rhs);
		
		friend bool operator>(const char* lhs, const FixedFloat& rhs);

		friend bool operator<=(const char* lhs, const FixedFloat& rhs);

		friend bool operator>=(const char* lhs, const FixedFloat& rhs);
		
		friend bool operator==(const char* lhs, const FixedFloat& rhs);

		friend bool operator!=(const char* lhs, const FixedFloat& rhs);
		
		operator int() const {
			return get_value_int();
		}

		operator double() const{
			return get_value_double();
		}

		operator std::string() const {
			return std::string(to_string());
		}

		void	set_value(double value);
		void	set_value(_int64 value);
		void	set_value(char sign, unsigned int integer_value, unsigned int decimal_value, unsigned short decimal_point = MAX_DECIMAL_POINT);
		void	set_value(int value, unsigned char decimal_point);
		void	set_value(const char value[], unsigned int value_len, unsigned char decimal_point, char sign = 0);
		void	set_value(const std::string& value);

		int		get_value_int() const;
		unsigned int get_value_decimal_int(unsigned int decimal_point = 2) const;
		double	get_value_decimal() const;
		double	get_value_double() const;
		const _int64& get_value_no_decimal_numbers() const;

		unsigned int get_sign() const {return (value_ < 0)?FP_MINUS:FP_PLUS;}
		static const FixedFloat& zero();
		static const FixedFloat& invalid();

		std::string to_string(unsigned short decimal_point = 4) const;

	private:				
		static FixedFloat zero_;
		static FixedFloat invalid_;
		_int64				value_;
};
template <typename T>
bool operator<(const FixedFloat& lhs, const T& rhs)
{
	return lhs < FixedFloat(static_cast<double>(rhs));
}

template <typename T>
bool operator>(const FixedFloat& lhs, const T& rhs)
{
	return lhs > FixedFloat(static_cast<double>(rhs));
}

template <typename T>
bool operator<=(const FixedFloat& lhs, const T& rhs)
{
	return lhs <= FixedFloat(static_cast<double>(rhs));
}

template <typename T>
bool operator>=(const FixedFloat& lhs, const T& rhs)
{
	return lhs >= FixedFloat(static_cast<double>(rhs));
}

template <typename T>
bool operator==(const FixedFloat& lhs, const T& rhs)
{
	return lhs == FixedFloat(static_cast<double>(rhs));
}		

template <typename T>
bool operator!=(const FixedFloat& lhs, const T& rhs)
{
	return !operator==(lhs, rhs);
}		

template <typename T>
bool operator<(const T& lhs, const FixedFloat& rhs)
{
	return FixedFloat(static_cast<const double>(lhs)) < rhs;
}

template <typename T>
bool operator>(const T& lhs, const FixedFloat& rhs)
{
	return FixedFloat(static_cast<const double>(lhs)) > rhs;
}

template <typename T>
bool operator<=(const T& lhs, const FixedFloat& rhs)
{
	return FixedFloat(static_cast<const double>(lhs)) <= rhs;
}

template <typename T>
bool operator>=(const T& lhs, const FixedFloat& rhs)
{
	return FixedFloat(static_cast<const double>(lhs)) >= rhs;
}

template <typename T>
bool operator==(const T& lhs, const FixedFloat& rhs)
{
	return FixedFloat(static_cast<const double>(lhs)) == rhs;
}		

template <typename T>
bool operator!=(const T& lhs, const FixedFloat& rhs)
{
	return !operator==(lhs, rhs);
}		
////
template <typename T>
bool operator<(const FixedFloat& lhs, const char* rhs)
{
	return lhs < FixedFloat(rhs);
}

template <typename T>
bool operator>(const FixedFloat& lhs, const char* rhs)
{
	return lhs > FixedFloat(rhs);
}

template <typename T>
bool operator<=(const FixedFloat& lhs, const char* rhs)
{
	return lhs <= FixedFloat(rhs);
}

template <typename T>
bool operator>=(const FixedFloat& lhs, const char* rhs)
{
	return lhs >= FixedFloat(rhs);
}

template <typename T>
bool operator==(const FixedFloat& lhs, const char* rhs)
{
	return lhs == FixedFloat(rhs);
}		

template <typename T>
bool operator!=(const FixedFloat& lhs, const char* rhs)
{
	return !operator==(lhs, rhs);
}		

bool operator<(const char* lhs, const FixedFloat& rhs);
bool operator>(const char* lhs, const FixedFloat& rhs);
bool operator<=(const char* lhs, const FixedFloat& rhs);
bool operator>=(const char* lhs, const FixedFloat& rhs);
bool operator==(const char* lhs, const FixedFloat& rhs);
bool operator!=(const char* lhs, const FixedFloat& rhs);

////

bool operator>(const char* lhs, FixedFloat&& rhs);
bool operator<=(const char* lhs, FixedFloat&& rhs);
bool operator>=(const char* lhs, FixedFloat&& rhs);
bool operator==(const char* lhs, FixedFloat&& rhs);
bool operator!=(const char* lhs, FixedFloat&& rhs);

template <typename T>
FixedFloat operator+(const T& lhs, const FixedFloat& rhs) {
	return FixedFloat(static_cast<double>(lhs)) + rhs;
}

FixedFloat operator+(const char* lhs, const FixedFloat& rhs);

template <typename T>
FixedFloat operator-(const T& lhs, const FixedFloat& rhs) {
	return FixedFloat(static_cast<double>(lhs)) - rhs;
}

FixedFloat operator-(const char* lhs, const FixedFloat& rhs);

template <typename T>
FixedFloat operator*(const T& lhs, const FixedFloat& rhs) {
	return FixedFloat(static_cast<double>(lhs)) * rhs;
}

FixedFloat operator*(const char* lhs, const FixedFloat& rhs);

template <typename T>
FixedFloat operator/(const T& lhs, const FixedFloat& rhs) {
	return FixedFloat(static_cast<double>(lhs)) / rhs;
}

FixedFloat operator/(const char* lhs, const FixedFloat& rhs);

// 해쉬값 생성을 위해 사용 
struct FixedFloatHash
{
	std::size_t operator()(const FixedFloat& val) const
	{   
		std::hash<_int64> make_hash;
		return make_hash(val.get_value_no_decimal_numbers());
	}   
};

