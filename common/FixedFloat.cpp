#include "FixedFloat.h"

FixedFloat FixedFloat::zero_ = FixedFloat(0.0);
FixedFloat FixedFloat::invalid_ = FixedFloat("2147483648.999999999");


void FixedFloat::set_value( double value )
{
	unsigned int integer_value = static_cast<unsigned int>(std::abs(value));
	LONGLONG tmp_value = (std::abs(value) - integer_value) * MAX_DECIMAL_POINT_MOD_VALUE + integer_value * MAX_DECIMAL_POINT_MOD_VALUE;
	if (value < 0)
		value_ = tmp_value * -1;
	else
		value_ = tmp_value;
}

void	FixedFloat::set_value(char sign, unsigned int integer_value, unsigned int decimal_value, unsigned short decimal_point)
{
	LONGLONG tmp_value = integer_value * MAX_DECIMAL_POINT_MOD_VALUE;
	tmp_value += decimal_value * pow(10, MAX_DECIMAL_POINT - decimal_point);	
	if ((sign == '-'))
		value_ = tmp_value * -1;
	else
		value_ = tmp_value;
}

void	FixedFloat::set_value(int value, unsigned char decimal_point)
{
	LONGLONG tmp_value = static_cast<unsigned int>(std::abs(value)) * MAX_DECIMAL_POINT_MOD_VALUE;

	if (decimal_point > 0) {
		int divider = pow(10, (double)decimal_point);
		tmp_value /= divider;
	}

	if (value < 0)
		value_ = tmp_value * -1;
	else
		value_ = tmp_value;
}

void	FixedFloat::set_value(const char value[], unsigned int value_len, unsigned char decimal_point, char sign)
{
	LONGLONG tmp_value = 0;	

	if (sign == '-') {
		sign = FP_MINUS;
	}

	for (int i = 0; i < value_len; ++i) {
		if (value[i] >= '0' && value[i] <= '9') {
			tmp_value *= 10;
			tmp_value += value[i] - '0';			
		}		
		else if (value[i] == '+') {
			sign = FP_PLUS;							
		}
		else if (value[i] == '-') {
			sign = FP_MINUS;							
		}		
		else if (value[i] == ' ' || value[i] == '\t') {
			continue;
		}
		else {
			break;		
		}
	}

	tmp_value *= pow(10, MAX_DECIMAL_POINT - decimal_point);
	if (sign != FP_PLUS)
		value_ = tmp_value * -1;
	else
		value_ = tmp_value;
}

void	FixedFloat::set_value(const std::string& value)
{
	LONGLONG integer_value = 0;	
	unsigned int decimal_value = 0;
	int e_value = 0;
	int e_sign = FP_PLUS, sign = FP_PLUS;
	int decimal_point = 0;
	LONGLONG tmp_value = 0;
	
	bool point_found = false;
	bool e_found = false;	
	
	for (int i = 0; i < value.size(); ++i) {
		if (value[i] >= '0' && value[i] <= '9') {
			if (e_found == false) {
				if (point_found == false) {
					integer_value *= 10;
					integer_value += value[i] - '0';
				}
				else {
					decimal_value *= 10;
					decimal_value += value[i] - '0';
					decimal_point++;
				}
			}
			else {
				e_value *= 10;
				e_value += value[i] - '0';
			}
		}
		else if (value[i] == '.') {
			point_found = true;			
		}
		else if (value[i] == '+') {
			if (e_found == false) {
				sign = FP_PLUS;				
			}
			else {
				e_sign = FP_PLUS;
			}
		}
		else if (value[i] == '-') {
			if (e_found == false) {
				sign = FP_MINUS;				
			}
			else {
				e_sign = FP_MINUS;
			}
		}
		else if (value[i] == 'e' || value[i] == 'E') {
			if (e_found == false) {
				e_found = true;
			}
			else {
				break;
			}
		}
		else if (value[i] == ' ' || value[i] == '\t') {
			continue;
		}
		else {
			break;		
		}
	}

	if (e_found == true) {
		if (e_sign == FP_MINUS) {
			e_value *= -1;
		}
		tmp_value = integer_value * MAX_DECIMAL_POINT_MOD_VALUE;
		tmp_value += decimal_value * pow(10, MAX_DECIMAL_POINT - decimal_point);
		tmp_value *= pow(10, e_value);
	}
	else {
		decimal_value *= pow(10, MAX_DECIMAL_POINT - decimal_point);
		tmp_value = integer_value * MAX_DECIMAL_POINT_MOD_VALUE + decimal_value;
	}

	if (sign == FP_MINUS)
		value_ = tmp_value * -1;
	else
		value_ = tmp_value;
}

void FixedFloat::set_value(LONGLONG value )
{
	value_ = value;
}


int FixedFloat::get_value_int() const
{
	return value_ / MAX_DECIMAL_POINT_MOD_VALUE;
}

double FixedFloat::get_value_double() const
{
	return value_ / 1000000000.0;
}

const LONGLONG& FixedFloat::get_value_no_decimal_numbers() const
{
	return value_;
}

FixedFloat FixedFloat::operator+( const FixedFloat& rhs ) const
{
	return (FixedFloat(this->get_value_no_decimal_numbers() + rhs.get_value_no_decimal_numbers()));
}

FixedFloat FixedFloat::operator-( const FixedFloat& rhs ) const
{
	return (FixedFloat(this->get_value_no_decimal_numbers() - rhs.get_value_no_decimal_numbers()));
}

FixedFloat FixedFloat::operator*( const FixedFloat& rhs ) const
{
	LONGLONG ai = value_ / MAX_DECIMAL_POINT_MOD_VALUE;
	LONGLONG ad = abs(value_) % MAX_DECIMAL_POINT_MOD_VALUE;
	if (get_sign() == FP_MINUS)
		ad *= -1;

	LONGLONG bi = rhs.value_ / MAX_DECIMAL_POINT_MOD_VALUE;
	LONGLONG bd = abs(rhs.value_) % MAX_DECIMAL_POINT_MOD_VALUE;
	if (rhs.get_sign() == FP_MINUS)
		bd *= -1;

	LONGLONG res = ad * bd / MAX_DECIMAL_POINT_MOD_VALUE;
	res += ai * bd;
	res += bi * ad;
	res += ai * bi * MAX_DECIMAL_POINT_MOD_VALUE;
	return FixedFloat(res);
}

FixedFloat FixedFloat::operator/( const FixedFloat& rhs ) const
{
	long double a = this->get_value_no_decimal_numbers();
	long double b = rhs.get_value_no_decimal_numbers();
	if (b == 0.0) {
		throw std::runtime_error("zero divide exception");
	}
	long double tmp_res = a / b;
	LONGLONG ti = static_cast<LONGLONG>(tmp_res);
	LONGLONG td = (tmp_res - ti) * static_cast<long double>(MAX_DECIMAL_POINT_MOD_VALUE);
	LONGLONG res = ti * MAX_DECIMAL_POINT_MOD_VALUE + td;

	return FixedFloat(res);
}

FixedFloat FixedFloat::operator+(FixedFloat&& rhs ) const
{
	return (FixedFloat(this->get_value_no_decimal_numbers() + rhs.get_value_no_decimal_numbers()));
}

FixedFloat FixedFloat::operator-(FixedFloat&& rhs ) const
{
	return (FixedFloat(this->get_value_no_decimal_numbers() - rhs.get_value_no_decimal_numbers()));
}

FixedFloat FixedFloat::operator*(FixedFloat&& rhs ) const
{
	LONGLONG ai = value_ / MAX_DECIMAL_POINT_MOD_VALUE;
	LONGLONG ad = abs(value_) % MAX_DECIMAL_POINT_MOD_VALUE;
	if (get_sign() == FP_MINUS)
		ad *= -1;

	LONGLONG bi = rhs.value_ / MAX_DECIMAL_POINT_MOD_VALUE;
	LONGLONG bd = abs(rhs.value_) % MAX_DECIMAL_POINT_MOD_VALUE;
	if (rhs.get_sign() == FP_MINUS)
		bd *= -1;

	LONGLONG res = ad * bd / MAX_DECIMAL_POINT_MOD_VALUE;
	res += ai * bd;
	res += bi * ad;
	res += ai * bi * MAX_DECIMAL_POINT_MOD_VALUE;
	return FixedFloat(res);
}

FixedFloat FixedFloat::operator/(FixedFloat&& rhs ) const
{
	long double a = this->get_value_no_decimal_numbers();
	long double b = rhs.get_value_no_decimal_numbers();
	if (b == 0.0) {
		throw std::runtime_error("zero divide exception");
	}
	long double tmp_res = a / b;
	LONGLONG ti = static_cast<LONGLONG>(tmp_res);
	LONGLONG td = (tmp_res - ti) * static_cast<long double>(MAX_DECIMAL_POINT_MOD_VALUE);
	LONGLONG res = ti * MAX_DECIMAL_POINT_MOD_VALUE + td;

	return FixedFloat(res);
}

FixedFloat& FixedFloat::operator+=( const FixedFloat& rhs )
{
	*this = FixedFloat(this->get_value_no_decimal_numbers() + rhs.get_value_no_decimal_numbers());
	return *this;
}

FixedFloat& FixedFloat::operator-=( const FixedFloat& rhs )
{
	*this = FixedFloat(this->get_value_no_decimal_numbers() - rhs.get_value_no_decimal_numbers());
	return *this;
}

FixedFloat& FixedFloat::operator*=( const FixedFloat& rhs )
{
	*this = this->operator*(rhs); 
	return *this;
}

FixedFloat& FixedFloat::operator/=( const FixedFloat& rhs )
{
	long double a = this->get_value_no_decimal_numbers();
	long double b = rhs.get_value_no_decimal_numbers();

	if (b == 0.0) {
		throw std::runtime_error("zero divide exception");
	}

	long double tmp_res = a / b;
	LONGLONG ti = static_cast<LONGLONG>(tmp_res);
	LONGLONG td = (tmp_res - ti) * static_cast<long double>(MAX_DECIMAL_POINT_MOD_VALUE);
	LONGLONG res = ti * MAX_DECIMAL_POINT_MOD_VALUE + td;

	*this = FixedFloat(res);
	return *this;
}

FixedFloat& FixedFloat::operator+=( FixedFloat&& rhs )
{
	*this = FixedFloat(this->get_value_no_decimal_numbers() + rhs.get_value_no_decimal_numbers());
	return *this;
}

FixedFloat& FixedFloat::operator-=( FixedFloat&& rhs )
{
	*this = FixedFloat(this->get_value_no_decimal_numbers() - rhs.get_value_no_decimal_numbers());
	return *this;
}

FixedFloat& FixedFloat::operator*=( FixedFloat&& rhs )
{
	*this = this->operator*(rhs);
	return *this;
}

FixedFloat& FixedFloat::operator/=( FixedFloat&& rhs )
{
	long double a = this->get_value_no_decimal_numbers();
	long double b = rhs.get_value_no_decimal_numbers();

	if (b == 0.0) {
		throw std::runtime_error("zero divide exception");
	}

	long double tmp_res = a / b;
	LONGLONG ti = static_cast<LONGLONG>(tmp_res);
	LONGLONG td = (tmp_res - ti) * static_cast<long double>(MAX_DECIMAL_POINT_MOD_VALUE);
	LONGLONG res = ti * MAX_DECIMAL_POINT_MOD_VALUE + td;

	*this = FixedFloat(res);
	return *this;
}

bool FixedFloat::operator<( const FixedFloat& rhs ) const
{
	return this->get_value_no_decimal_numbers() < rhs.get_value_no_decimal_numbers();
}

bool FixedFloat::operator==( const FixedFloat& rhs ) const
{
	return this->get_value_no_decimal_numbers() == rhs.get_value_no_decimal_numbers();
}

bool FixedFloat::operator<=( const FixedFloat& rhs ) const
{
	return this->get_value_no_decimal_numbers() <= rhs.get_value_no_decimal_numbers();
}

bool FixedFloat::operator>=( const FixedFloat& rhs ) const
{
	return this->get_value_no_decimal_numbers() >= rhs.get_value_no_decimal_numbers();
}

bool FixedFloat::operator>( const FixedFloat& rhs ) const
{
	return this->get_value_no_decimal_numbers() > rhs.get_value_no_decimal_numbers();
}

bool FixedFloat::operator!=( const FixedFloat& rhs ) const
{
	return !this->operator==(rhs);
}


bool FixedFloat::operator<( FixedFloat&& rhs ) const
{
	return this->get_value_no_decimal_numbers() < rhs.get_value_no_decimal_numbers();
}

bool FixedFloat::operator==( FixedFloat&& rhs ) const
{
	return this->get_value_no_decimal_numbers() == rhs.get_value_no_decimal_numbers();
}

bool FixedFloat::operator<=( FixedFloat&& rhs ) const
{
	return this->get_value_no_decimal_numbers() <= rhs.get_value_no_decimal_numbers();
}

bool FixedFloat::operator>=( FixedFloat&& rhs ) const
{
	return this->get_value_no_decimal_numbers() >= rhs.get_value_no_decimal_numbers();
}

bool FixedFloat::operator>( FixedFloat&& rhs ) const
{
	return this->get_value_no_decimal_numbers() > rhs.get_value_no_decimal_numbers();
}

bool FixedFloat::operator!=( FixedFloat&& rhs ) const
{
	return !this->operator==(rhs);
}


const FixedFloat& FixedFloat::zero()
{
	return zero_;
}

const FixedFloat& FixedFloat::invalid()
{
	return invalid_;
}

unsigned int FixedFloat::get_value_decimal_int( unsigned int decimal_point) const
{
	return (std::abs(value_) % MAX_DECIMAL_POINT_MOD_VALUE) / pow(10, MAX_DECIMAL_POINT - decimal_point);
}

double FixedFloat::get_value_decimal() const
{
	return (value_ % MAX_DECIMAL_POINT_MOD_VALUE) / (double)MAX_DECIMAL_POINT_MOD_VALUE;
}

std::string FixedFloat::to_string(unsigned short decimal_point) const 
{
	if (decimal_point > 9)
		decimal_point = 9;

	unsigned int decimal_value, integer_value;
	static const int MAX_FIXED_FLOAT_STR_LEN = 21;
	char tmp_buf[MAX_FIXED_FLOAT_STR_LEN + 1] = { 0, };
	int int_start_idx = MAX_FIXED_FLOAT_STR_LEN - 1, i, str_start_idx = 0;
	unsigned int tmp_val, div_val;
	ULONGLONG mod_val;

	integer_value = std::abs(value_) / MAX_DECIMAL_POINT_MOD_VALUE;
	decimal_value = std::abs(value_) % MAX_DECIMAL_POINT_MOD_VALUE;

	if (decimal_point != 0) {
		mod_val = 1000000000;

		for (i = 0; i < decimal_point; ++i) {
			tmp_val = decimal_value % mod_val;
			mod_val /= 10;
			char n = '0' + tmp_val / mod_val;
			tmp_buf[MAX_FIXED_FLOAT_STR_LEN - decimal_point + i] = n;
		}
		tmp_buf[MAX_FIXED_FLOAT_STR_LEN - decimal_point - 1] = '.';
		int_start_idx = MAX_FIXED_FLOAT_STR_LEN - decimal_point - 2;
	}

	mod_val = 10;
	for (i = 0; i < 10; ++i) {
		tmp_val = integer_value % mod_val;
		div_val = mod_val / 10;
		char n = '0' + tmp_val / div_val;
		tmp_buf[int_start_idx - i] = n;

		if (integer_value < mod_val)
			break;
		else {
			mod_val *= 10;
		}
	}

	str_start_idx = int_start_idx - i;
	if (value_ < 0) {
		--str_start_idx;
		tmp_buf[str_start_idx] = '-';
	}

	return std::string(tmp_buf + str_start_idx);
}


bool operator<(const char* lhs, const FixedFloat& rhs)
{
	return FixedFloat(lhs) < rhs;
}

bool operator>(const char* lhs, const FixedFloat& rhs)
{
	return FixedFloat(lhs) > rhs;
}

bool operator<=(const char* lhs, const FixedFloat& rhs)
{
	return FixedFloat(lhs) <= rhs;
}

bool operator>=(const char* lhs, const FixedFloat& rhs)
{
	return FixedFloat(lhs) >= rhs;
}

bool operator==(const char* lhs, const FixedFloat& rhs)
{
	return FixedFloat(lhs) == rhs;
}		

bool operator!=(const char* lhs, const FixedFloat& rhs)
{
	return !operator==(lhs, rhs);
}		

bool operator>(const char* lhs, FixedFloat&& rhs)
{
	return FixedFloat(lhs) > rhs;
}

bool operator<=(const char* lhs, FixedFloat&& rhs)
{
	return FixedFloat(lhs) <= rhs;
}

bool operator>=(const char* lhs, FixedFloat&& rhs)
{
	return FixedFloat(lhs) >= rhs;
}

bool operator==(const char* lhs, FixedFloat&& rhs)
{
	return FixedFloat(lhs) == rhs;
}		

bool operator!=(const char* lhs, FixedFloat&& rhs)
{
	return !operator==(lhs, rhs);
}		

FixedFloat operator+(const char* lhs, const FixedFloat& rhs) {
	return FixedFloat(lhs) + rhs;
}

FixedFloat operator-(const char* lhs, const FixedFloat& rhs) {
	return FixedFloat(lhs) - rhs;
}

FixedFloat operator*(const char* lhs, const FixedFloat& rhs) {
	return FixedFloat(lhs) * rhs;
}

FixedFloat operator/(const char* lhs, const FixedFloat& rhs) {
	return FixedFloat(lhs) / rhs;
}
