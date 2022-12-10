/*
Author: B11032004 ¤ý¤lºû B11032002 §f©vÀM
Date:2022/4/21
Last_Update:2022/05/05
Subject:Big Num Calculator
*/

#include "BigNum.h"
using namespace std;



int main() {
	string s;
	BigNum::PrintArgs();
	while (getline(cin, s)) {
		//¿é¤J «ü¥O°»´ú
		if (s.substr(0, 4) == "Set " || s.substr(0, 4) == "set ") {
			s.erase(0, 4);
			if (s.substr(0, 8) == "Integer " || s.substr(0, 8) == "integer ") {
				s.erase(0, 8);
				std::string argc;
				int i = 0;
				while (s[i] != '=') {
					argc += s[i];
					i++;
				}
				s.erase(s.begin(), s.begin() + i+1);
				size_t index = s.find('.');
				if (index != std::string::npos) s.erase(index);
				CheckArgsValid(argc);
				if (!s.empty()) {
					BigNum tmp(s);
					BigNum::args[argc] = tmp;
				}
				BigNum::PrintArgs();
			}
			else if (s.substr(0, 8) == "Decimal " || s.substr(0, 8) == "decimal ") {
				s.erase(0, 8);
				std::string argc;
				int i = 0;
				while (s[i] != '=') {
					argc += s[i];
					i++;
				}
				s.erase(s.begin(), s.begin() + i+1);
				CheckArgsValid(argc);
				if (!s.empty()) {
					BigNum tmp(s);
					BigNum::args[argc] = tmp;
				}
				BigNum::PrintArgs();
			}
			else {
				BigNum::PrintArgs();
				std::cout << "Invalid Syntax\n";
			}
		}
		else {
			//¤@¯ë¹Bºâ
			BigNum tmp(s);
			BigNum::PrintArgs();
			if(!tmp.ErrorCode)	std::cout << tmp <<"\n";
			else {
				switch (tmp.ErrorCode) {
				case 1:
					cout << "Error : Divide by Zero\n";
					break;
				case 2:
					cout << "Error : Invalid Input\n";
					break;
				case 3:
					cout << "Error : Decimal Factorial Operation\n";
					break; 
				case 4:
					cout << "Error : Invalid Power\n";
					break;
				}
				tmp.ErrorCode = 0;
			}
		}
	}
	
	Integer i = (std::string)"123";
	Decimal d = (std::string)"0.3 * 3";
	vector<BigNum*> nums;
	nums.push_back(&i);
	nums.push_back(&d);
	for (const auto& num : nums)
		cout << *num << endl;
	
}

