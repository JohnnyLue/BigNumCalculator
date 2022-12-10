#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <deque>
#include <math.h>
#include <algorithm>
#define Integer BigNum
#define Decimal BigNum

static std::string RemoveSpace(std::string);
static std::string stringMuti(std::string, std::string);
static std::string stringSub(std::string, std::string);
static std::string stringDivi(std::string, std::string);
static void CheckArgsValid(std::string&);

static bool stringBigger(std::string, std::string);

std::string RemoveSpace(std::string s) {
	if (s.empty()) return s;
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == ' ') {
			s.erase(s.begin() + i);
			i--;
		}
	}
	return s;
}

//std::string multiplication
std::string stringMuti(std::string num1, std::string num2) {
    if (num1.find_first_not_of('0') >= num1.size() || num2.find_first_not_of('0') >= num2.size())
    {
        return "0";
    }

    std::vector<int> value(num1.length() + num2.length());
    int len = num1.length() + num2.length();

    for (int i = num1.length() - 1; i >= 0; i--)
    {
        for (int j = num2.length() - 1; j >= 0; j--)
        {
            int indenumerator = num2.length() - 1 - i + num1.length() - 1 - j;
            value[indenumerator] += (num1[i] - '0') * (num2[j] - '0');
        }
    }
    for (int i = 0; i < len - 1; i++)
    {
        value[i + 1] += value[i] / 10;
        value[i] = value[i] % 10;
    }
    int indenumerator = len - 1;
    while (value[indenumerator] == 0)
    {
        indenumerator--;
    }
    std::string s;
    while (indenumerator >= 0) {
        s += (value[indenumerator--] + 48);
    }
    //cout << s;
    return s;
}

// num1 >= num2
bool stringBigger(std::string num1, std::string num2)
{
    if (num1.find_first_not_of('0') >= num1.size() && num2.find_first_not_of('0') >= num1.size())
    {
        return true;
    }
    if (num1.find_first_not_of('0') >= num1.size() && num2.find_first_not_of('0') < num1.size())
    {
        return false;
    }
    if (num1.find_first_not_of('0') < num1.size() && num2.find_first_not_of('0') >= num1.size())
    {
        return true;
    }

    num1 = num1.substr(num1.find_first_not_of('0'));
    num2 = num2.substr(num2.find_first_not_of('0'));

    if (num1.size() < num2.size())
        return false;
    if (num1.size() == num2.size())
    {
        for (int i = 0; i < num1.size(); i++)
        {
            if (num1[i] > num2[i])
                break;
            else if (num1[i] < num2[i])
                return false;
        }
    }
    return true;
}

std::string stringSub(std::string num1, std::string num2)
{
    if (num1.compare(num2) == 0)
    {
        std::string ans;
        for (int i = 0; i < num1.size(); i++)
        {
            ans += '0';
        }
        return ans;
    }

    int i, j;

    std::vector<int>ans(num1.size());

    for (int i = 0; i < num1.size(); i++)
    {
        ans[i] = num1[i] - 48;
    }

    for (i = num1.size() - 1, j = num2.size() - 1; i >= 0 && j >= 0; i--, j--) // 從低位開始做減法
    {
        ans[i] -= (num2[j] - 48);         // 相減 
        int temp = i;
        while (ans[temp] < 0)           // 若是否需要借位
        {   // 借位 
            ans[temp] += 10;
            ans[temp - 1]--;
            temp--;
            if (temp < 0)
                break;
        }
    }

    std::string answer;
    for (auto i : ans)
        answer += i + 48;

    return answer;
}

//must:num1>0,num2>0
std::string stringDivi(std::string num1, std::string num2)
{
    std::string ans;
    if (num1.compare(num2) == 0)
        return "1";
    std::string temp, muti;
    for (int i = num2.size() - 1; i < num1.size(); ++i)
    {
        for (int b = 9; b >= 0; b--)
        {
            if (b == 0)
            {
                ans += '0';
                break;
            }
            temp = " ";
            temp[0] = b + 48;

            muti = stringMuti(num2, temp);

            if (stringBigger(num1.substr(0, i + 1), muti))
            {
                std::string fixpos = "1";
                for (int j = 0; j < num1.size() - i - 1; j++)
                {
                    fixpos += '0';
                }
                muti = stringMuti(muti, fixpos);

                num1 = stringSub(num1, muti);
                ans += b + 48;
                break;
            }
        }
    }
    if (ans.find_first_not_of('0') >= ans.size())
        return "0";
    ans = ans.substr(ans.find_first_not_of('0'));
    return ans;
}



void CheckArgsValid(std::string &s) {
    if (s == "Set" || s == "set" || s == "Integer" || s == "integer" || s == "Decimal" || s == "decimal") {
        std::cout << "Error : Reserved Word can't be used as Variable Name";
        return;
    }
    for (int i = 0; i < s.length(); i++) {
        if (s[i] > 'z' || s[i] < 'A') {
            if (s[i] >= '0' && s[i] <= '9');
            else {
                std::cout << "Error : Syntax Error. Only A-Z and a-z are allowed.";
                s.clear();
                break;
            }
        }
        else if (s[i] > 'Z' && s[i] < 'a') {
            std::cout << "Error : Syntax Error. Only A-Z and a-z are allowed.";
            s.clear();
            break;
        }
    }
}

