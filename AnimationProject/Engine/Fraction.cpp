#include "Fraction.h"



Fraction::Fraction()
{
}


Fraction::~Fraction()
{
}

void Fraction::Set(int Numerator, int Denominator)
{
	m_numerator = Numerator;
	m_denominator = Denominator;
	m_decimal = (float)m_numerator / (float)m_denominator;
}

float Fraction::GetDecimal()
{
	return m_decimal;
}