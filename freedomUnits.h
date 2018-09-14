/*
 * Author:	Rain Mitchell Copyright (C) Noah Mitchell
 * 			rainmitch.personal@gmail.com
 * Version 0.0.1
 * Date: 09/14/2018
 * License:
 * 		1. I am not liable for whatever you do with this. If you find some way to break something using this code,
 * 		it's your fault. I don't even know how you could do that!
 * 		2. Don't claim that you own/wrote this or pretend to be me, that's just weird.
 * 		3. If you copy this code or use it in any way, you must include this comment.
 * 		4. You may modify this code, just write below this comment block what you changed.
*/


#ifndef FREEDOM_UNITS_H
#define FREEDOM_UNITS_H

#include <limits>
#include <stdint.h>
#include <iostream>
#include <cmath>

template<typename T>
class Imperial
{
public:
	Imperial (float inchInput) : Imperial ((double)inchInput)
	{
	}
	
	Imperial (double inchInput)
	{
		feet = (T)inchInput / 12;
		inch = (T)inchInput % 12;
		float remainder = inchInput - (T)inchInput;
		
		numerator = (uint8_t)(remainder * 64);
	}
	
	Imperial (T feet=0, uint8_t inches=0, float numInput=0)
	{
		uint8_t num = (uint8_t)(numInput * 64);
		
		if (num > 64) throw "Inch Fractional Overflow: numerator too big.";
		
		this->feet = feet;
		uint32_t tmp_inches = inches;
		if (num / 64 == 1)
		{
			tmp_inches += 1;
			numerator = 0;
		}
		else
		{
			numerator = num;
		}
		
		if (tmp_inches >= 12)
		{
			this->feet += tmp_inches / 12;
			tmp_inches = tmp_inches % 12;
		}
		this->inch = tmp_inches;
	}
	
	float inches ()
	{
		return (feet*12.0f) + (inch) + ((float)numerator / 64.0);
	}
	
	float yards ()
	{
		return inches () / 36;
	}
	
	float centimeters ()
	{
		return inches () * 2.54f;
	}
	
	float meters ()
	{
		return centimeters () / 100;
	}
	
	
	Imperial<T> operator+ (Imperial<T> b)
	{
		Imperial<T> c;
		c.feet = feet + b.feet;
		c.inch = inch + b.inch;
		c.numerator = numerator + b.numerator;
		c.simplify ();
		
		return c;
	}
	
	Imperial<T> operator- (Imperial<T> b)
	{
		if (inches () < b.inches ()) throw "Subtraction results in an underflow. You may not represent negative measurements directly.";
		
		Imperial<T> c;
		c.feet = feet - b.feet;
		c.inch = inch - b.inch;
		c.numerator = numerator - b.numerator;
		c.simplify ();
		
		return c;
	}
	
	Imperial<T> operator* (Imperial<T> b)
	{
		Imperial<T> c = Imperial<T> (inches () * b.inches ());
		
		return c;
	}
	
	Imperial<T> operator/ (Imperial<T> b)
	{
		Imperial<T> c = Imperial<T> (inches () / b.inches ());
		
		return c;
	}
	
	void simplify ()
	{
		if (numerator / 64 >= 1)
		{
			inch += numerator % 64;
			numerator = numerator % 64;
		}
		
		if (inch >= 12)
		{
			inch = 0;
			feet++;
		}
		else if (inch < 0)
		{
			feet += inch / 12;
			inch = inch % 12;
			
			feet--;
			inch = 12 + inch;
		}
	}
	
	T feet;
	struct
	{
		int16_t inch: 5, numerator: 11;
	};
};

std::string simplify (uint8_t numerator)
{
	int count = 6 - log2l (numerator);
	
	uint8_t new_numerator = numerator * pow (2, -count);
	uint8_t new_denominator = 64 * pow (2, -count);
	
	return std::to_string (new_numerator) + "/" + std::to_string (new_denominator);
}

template<typename T>
std::ostream &operator<< (std::ostream &os, Imperial<T> &imperial)
{
	os << imperial.feet << '\'' << imperial.inch;
	if (imperial.numerator != 0)
	{
		os << " " << simplify (imperial.numerator);
	}
	return os << '\"';
}

typedef Imperial<uint16_t> ishort;
typedef Imperial<uint32_t> ilow;
typedef Imperial<uint64_t> ihigh;

#endif
