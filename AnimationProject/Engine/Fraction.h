#pragma once
class Fraction
{
	int m_numerator;
	int m_denominator;
	float m_decimal;
public:
	Fraction();
	~Fraction();
	void Set(int Numerator, int Denominator);
	float GetDecimal();
	inline int GetNumerator() { return m_numerator; };
	inline int GetDenominator() { return m_denominator; };
};