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
	static std::unordered_map<std::string, BigNum> args; //變數列表
	static BigNum PostFixCalculation(std::deque<BigNum>);	//後置運算
	static std::deque<BigNum> ToPostFix(std::string);	//轉換 後置運算子
	static BigNum reduction(BigNum);	//約分
	static BigNum root(BigNum,BigNum);	//開根號
	static void PrintArgs();
};

