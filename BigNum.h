#pragma once
#include "utility.h"

class BigNum  {
public:
	BigNum();
	BigNum(std::string);
	std::string value;
	bool isInt;
	bool isStructuredByString;
	bool isOperator;
	static int ErrorCode;
	/*
	Error Code List:
	1. Divide by Zero
	2. Invalid Input
	3. Decimal Factorial Operation
	4. Power Invalid
	*/
	std::string denominator;
	BigNum operator+(BigNum);
	BigNum operator-(BigNum);
	BigNum operator*(BigNum);
	BigNum operator/(BigNum);
	BigNum operator=(char[]);
	friend std::ostream& operator<< (std::ostream&, const BigNum&);
	friend std::istream& operator>> (std::istream&, BigNum&);
	static std::unordered_map<std::string, BigNum> args; //�ܼƦC��
	static BigNum PostFixCalculation(std::deque<BigNum>);	//��m�B��
	static std::deque<BigNum> ToPostFix(std::string);	//�ഫ ��m�B��l
	static BigNum reduction(BigNum);	//����
	static BigNum root(BigNum,BigNum);	//�}�ڸ�
	static void PrintArgs();
};

