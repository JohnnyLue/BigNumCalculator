#include "BigNum.h"

BigNum::BigNum(){
	value = "0";
	denominator = "1";
	isInt = false;
	isStructuredByString = false;
	isOperator = false;
}

BigNum::BigNum(std::string s) {
    s = RemoveSpace(s);
    if ((s[0] == '+' || s[0] == '-' || s[0] == '*' || s[0] == '/' || s[0] == '^'|| s[0] =='!') && s.length() == 1) {
        value = s[0];
        denominator = "1";
        isInt = false;
        isStructuredByString = true;
        isOperator = true;
    }
    else {
        for (int i = 0; i < s.length();i++) {
            if (!isdigit(s[i])&&s[i]!='.') {
                if (i == 0 && s[i] == '-');
                else {
                    *this = PostFixCalculation(ToPostFix(s));
                    isInt = false;
                    isStructuredByString = true;
                    isOperator = false;
                    return;
                }
            }
        }
        size_t decimal = s.find('.');
        if (decimal == std::string::npos) {
            value = s;
            denominator = "1";
            isInt = true;
            isStructuredByString = true;
            isOperator = false;
        }
        else {
            value = s;
            value.erase(decimal, 1);
            denominator = "1";
            denominator.insert(denominator.end(), s.length() - decimal - 1, '0');
            if(value[0]=='0') value.erase(0,1);
            isInt = false;
            isStructuredByString = true;
            isOperator = false;
            *this = reduction(*this);
        }
    }
}

int BigNum::ErrorCode = 0;

std::unordered_map<std::string, BigNum> BigNum::args;
/*BigNum Calculation(std::string formula) {
	std::deque<BigNum> ProcessedFormula;
	formula = RemoveSpace(formula);
	std::string denominator;
	//例外-號處理 eg. (-a) 
	{
		size_t index = formula.find('(-');
		formula.replace(index+1, 1, "-1*");
	}
	//括號處理
	{
		while (1) {
			size_t lhs = formula.find('(');
			if (lhs != std::string::npos) {
				int LeftParentheses = 0;
				size_t rhs = formula.find(')');
				if (rhs == std::string::npos) return ("Missing Parentheses");
				else if (rhs == lhs + 1) formula.erase(lhs, 2);
				else {
					for (size_t i = lhs; i < formula.length(); i++) {
						if (formula[i] == '(') LeftParentheses++;
						if (formula[i] == ')') LeftParentheses--;
						if (LeftParentheses == 0) {
							rhs = i;
							break;
						}
					}
					std::string tmp = Calculation(formula.substr(lhs + 1, rhs - lhs - 1));
					if (tmp == "Missing Parentheses") return tmp;
					formula.replace(lhs, rhs - lhs + 1, tmp);
				}
			}
			else break;
		}
		if (formula.find(')') != std::string::npos) {
			return "Missing Parentheses";
		}
	}
	//變數取代回數字
	{
		size_t lhs = 0, rhs = 0;
		for (int i = 0; i < formula.length(); i++) {
			while (1) {
				i++;
				if (i == i < formula.length()) break;
				if (formula[i] == '+' || formula[i] == '-' || formula[i] == '*' || formula[i] == '/' || formula[i] == '^') rhs = i;
				std::string tmp = formula.substr(lhs, rhs - lhs);
				if(args[tmp].isStructuredByString) 
			}
		}
	}
	size_t factorial = formula.find('!');
}
*/
std::deque<BigNum> BigNum::ToPostFix(std::string formula){
    std::unordered_map<std::string, int> priority;
    priority["+"] = 1;
    priority["-"] = 1;
    priority["*"] = 2;
    priority["/"] = 2;
    priority["^"] = 3;
    priority["!"] = 4;
    std::unordered_map<char, int> priority_char;
    priority_char['+'] = 1;
    priority_char['-'] = 1;
    priority_char['*'] = 2;
    priority_char['/'] = 2;
    priority_char['^'] = 3;
    priority_char['!'] = 4;
    std::string tmp, tmp_push;
    std::deque<std::string>sk; //stack
    std::deque<BigNum> ret;
    sk.push_back("#");
    formula = RemoveSpace(formula);
    formula.insert(formula.begin(), '(');
    formula.push_back(')');
    //例外-號處理 eg. (-a) 
    {
        for (int i = 0; i < formula.length(); i++) {
            if (formula[i] == '(') {
                if (i + 1 < formula.length() && formula[i + 1] == '-') {
                    formula.replace(formula.begin() + i + 1, formula.begin() + i + 2, "0-");
                    i += 2;
                }
            }
        }
    }
    for (int i = 0; i < formula.length();) {
        if (isdigit(formula[i])) {
            //判斷字元是否是0~9的數字
            while (isdigit(formula[i]) || formula[i] == '.') {
                tmp += formula[i];
                i++;
            }
            if (formula[i] == '!') {
                size_t decimal = tmp.find('.');
                if (decimal == std::string::npos) {
                    std::string tmp_factorial = tmp;
                    while (tmp_factorial != "1") {
                        tmp_factorial = stringSub(tmp_factorial, "1");
                        if (tmp_factorial.find_first_not_of('0') < tmp_factorial.size())  tmp_factorial = tmp_factorial.substr(tmp_factorial.find_first_not_of('0'));
                        tmp = stringMuti(tmp, tmp_factorial);
                    }
                }
                else {
                    ErrorCode = 3;
                    return ret;
                }
                i++;
            }
            ret.push_back(tmp);
            tmp.clear();
        }
        else {
            if (formula[i] == '+' || formula[i] == '-' || formula[i] == '*' || formula[i] == '/' 
                || formula[i] == '^' ||formula[i]=='!') {
                if (priority_char[formula[i]] > priority[sk.back()]) {
                    tmp_push.clear();
                    tmp_push += formula[i];
                    sk.push_back(tmp_push);
                    i++;
                }
                else {
                    while (priority_char[formula[i]] <= priority[sk.back()]) {
                        ret.push_back(sk.back());
                        sk.pop_back();
                    }
                    tmp_push.clear();
                    tmp_push += formula[i];
                    sk.push_back(tmp_push);
                    i++;
                }
            }
            else if (formula[i] == '(') {
                tmp_push.clear();
                tmp_push += formula[i];
                sk.push_back(tmp_push);
                i++;
            }
            else if (formula[i] == ')') {
                while (sk.back() != "(")
                {
                    ret.push_back(sk.back());
                    sk.pop_back();
                }
                sk.pop_back();
                i++;
            }
            else { 
                tmp_push.clear();
                while (formula[i] != '+' && formula[i] != '-' && formula[i] != '*' && formula[i] != '/' 
                        && formula[i] != '^'&& formula[i] != '('&& formula[i] != ')') {
                    tmp_push += formula[i];
                    i++;
                    if (i == formula.length()) break;
                }
                if (args[tmp_push].isStructuredByString) ret.push_back(args[tmp_push]);
                else {
                    args.erase(tmp_push);
                    ErrorCode = 2;
                    return ret;
                }
                tmp_push.clear();
            }
        }
        if (i == formula.length())
        {
            while (sk.size() != 1)
            {
                ret.push_back(sk.back());
                sk.pop_back();
            }
        }
    }
    return ret;
}

BigNum BigNum::PostFixCalculation(std::deque<BigNum> st) {
    if (ErrorCode) {
        BigNum tmp;
        return tmp;
    }
    std::deque<BigNum> nums;
        while (st.size() != 0){
            if (!st.front().isOperator) {
                nums.push_back(st.front());
                st.pop_front();
            }
            else
            {
                char c = st.front().value[0];
                st.pop_front();
                BigNum num1 = nums.back();
                nums.pop_back();
                switch (c) {
                case '!': {
                    if (num1.denominator == "1") {
                        std::string tmp_factorial = num1.value;
                        while (tmp_factorial != "1") {
                            tmp_factorial = stringSub(tmp_factorial, "1");
                            if (tmp_factorial.find_first_not_of('0') < tmp_factorial.size())  tmp_factorial = tmp_factorial.substr(tmp_factorial.find_first_not_of('0'));
                            num1.value = stringMuti(num1.value, tmp_factorial);
                        }
                        nums.push_back(num1);
                    }
                    else {
                        ErrorCode = 3;
                    }
                    break;
                }
                case'+': {
                    BigNum num2 = nums.back();
                    nums.pop_back();
                    nums.push_back(num1 + num2);
                    break;
                }
                case'-': {
                    BigNum num2 = nums.back();
                    nums.pop_back();
                    nums.push_back(num2-num1);
                    if (nums.back().value[0] == '-' && nums.back().value[1] == '-') nums.back().value.erase(0, 2);
                    break;
                }
                case'*': {
                    BigNum num2 = nums.back();
                    nums.pop_back();
                    nums.push_back(num1 * num2);
                    break;
                }
                case'/': {
                    BigNum num2 = nums.back();
                    nums.pop_back();
                    nums.push_back(num2 / num1);
                    break;
                }
                case'^': {
                    BigNum num2 = nums.back();
                    nums.pop_back();
                    num1 = reduction(num1);
                    if (num1.value == "1" && num1.denominator == "1") nums.push_back(num2);
                    else if (num2.value == "0") {
                        num1.value = "1";
                        num1.denominator = "0";
                        nums.push_back(num1);
                    }
                    else if (num1.denominator == "1") {
                        BigNum tmp = num2;
                        while (num1.value != "1") {
                            num1.value = stringSub(num1.value, "1");
                            if (num1.value.find_first_not_of('0') < num1.value.size())  
                                num1.value = num1.value.substr(num1.value.find_first_not_of('0'));
                            tmp = tmp * num2;
                        }
                        nums.push_back(tmp);
                    }
                    else if (num1.denominator == "2") {
                        if (num1.value[0] != '-') {
                            num1.value = stringSub(num1.value, "1");
                            num1.value = stringDivi(num1.value, "2");
                            BigNum tmp = num2;
                            tmp.value = "1";
                            while (num1.value != "0") {
                                num1.value = stringSub(num1.value, "1");
                                if (num1.value.find_first_not_of('0') < num1.value.size())
                                    num1.value = num1.value.substr(num1.value.find_first_not_of('0'));
                                tmp = tmp * num2;
                            }
                            tmp = root(num2, BigNum("0.5")) * tmp;
                            nums.push_back(tmp);
                        }
                        else {
                            num1.value.erase(0, 1);
                            num1.value = stringSub(num1.value, "1");
                            num1.value = stringDivi(num1.value, "2");
                            BigNum tmp = num2;
                            tmp.value = "1";
                            while (num1.value != "0") {
                                num1.value = stringSub(num1.value, "1");
                                if (num1.value.find_first_not_of('0') < num1.value.size())
                                    num1.value = num1.value.substr(num1.value.find_first_not_of('0'));
                                tmp = tmp * num2;
                            }
                            if (tmp.value[0] == '-') {
                                tmp.value.erase(0, 1);
                                tmp.denominator.insert(tmp.denominator.begin(), '-');
                            }
                            std::string tmpstr = tmp.value;
                            tmp.value = tmp.denominator;
                            tmp.denominator = tmpstr;
                            tmp = root(num2, BigNum("-0.5")) * tmp;
                            nums.push_back(tmp);
                        }
                    }
                    else {
                        BigNum::ErrorCode = 4;
                    }
                    break;
                }
                }
            }
            if (ErrorCode) {
                BigNum tmp;
                return tmp;
            }
        }
        return nums[0];
}

BigNum BigNum::operator+(BigNum r) {
    using namespace std;
    bool negative = false;
    if (this->value.compare("0") == 0)
        return r;
    if (r.value.compare("0") == 0)
        return *this;
    if (this->value[0] == '-' && r.value[0] != '-')
    {
        this->value = this->value.substr(1);
        return *this-r;
    }
    if (this->value[0] != '-' && r.value[0] == '-')
    {
        r.value = r.value.substr(1);
        return  *this - r;
    }
    if (this->value[0] == '-' && r.value[0] == '-')
    {
        r.value = r.value.substr(1);
        this->value = this->value.substr(1);
        negative = true;
    }

    string con = stringMuti(this->denominator, r.denominator);//分母
    string tl, tr;//通分後分子
    tl = stringMuti(this->value, r.denominator);
    tr = stringMuti(r.value, this->denominator);
    int i = tl.size() - 1, j = tr.size() - 1;
    vector <int> ans(max(i, j) + 2);
    int cur = 0;
    for (; i >= 0 && j >= 0; i--, j--)
    {
        int temp;
        temp = tl[i] - 48 + tr[j] - 48;

        ans[cur] += temp;
        if (ans[cur] >= 10)
        {
            ans[cur + 1]++;
            ans[cur] -= 10;
        }
        cur++;
    }


    for (; i >= 0; i--)
    {
        ans[cur] += tl[i] - 48;
        if (ans[cur] >= 10)
        {
            ans[cur + 1]++;
            ans[cur] -= 10;
        }
        cur++;
    }


    for (; j >= 0; j--)
    {
        ans[cur] += tr[j] - 48;
        if (ans[cur] >= 10)
        {
            ans[cur + 1]++;
            ans[cur] -= 10;
        }
        cur++;
    }
    BigNum answer = *this;
    answer.value.clear();
    answer.denominator.clear();
    while (ans.size())
    {
        answer.value += ans.back() + 48;
        ans.pop_back();
    }

    answer.value = answer.value.substr(answer.value.find_first_not_of('0'));
    if (negative)
    {
        answer.value.insert(answer.value.begin(), '-');
    }
    answer.denominator = con;

    return reduction(answer);
}

BigNum BigNum::operator-(BigNum r) {
    using namespace std;
        bool negative = false;

        if (this->value.compare("0") == 0 || this->value.compare("-0") == 0)
        {
            BigNum answer;
            r.value.insert(r.value.begin(), '-');
            if (r.value[0] == '-' && r.value[1] == '-') r.value.erase(0, 2);
            answer.value = r.value;
            answer.denominator = r.denominator;
            return reduction(answer);
        }
        if (r.value.compare("0") == 0 || r.value.compare("-0") == 0)
            return reduction(*this);

        if (this->value[0] == '-' && r.value[0] != '-')
        {
            r.value.insert(r.value.begin(), '-');
            return (*this+r);
        }
        if (this->value[0] != '-' && r.value[0] == '-')
        {
            r.value = r.value.substr(1);
            return (*this + r);
        }
        if (this->value[0] == '-' && r.value[0] == '-')
        {
            r.value = r.value.substr(1);
            this->value = this->value.substr(1);
            return(r- *this);
        }

        string con = stringMuti(this->denominator, r.denominator);//分母
        string tl, tr;//通分後分子
        tl = stringMuti(this->value, r.denominator);
        tr = stringMuti(r.value, this->denominator);
        if (tl.size() < tr.size())
        {
            string temp = tl;
            tl = tr;
            tr = temp;
            negative = true;
        }
        else if (tl.size() == tr.size())
        {
            if (tl.compare(tr) < 0)
            {
                string temp = tl;
                tl = tr;
                tr = temp;
                negative = true;
            }
            else if (tl.compare(tr) == 0)
            {
                BigNum answer;
                answer.value = "0";
                answer.denominator = "1";
                return answer;
            }
        }
        //tl>tr

        int i = tl.size() - 1, j = tr.size() - 1;
        vector <int> ans(max(i, j) + 1);
        int cur = 0;
        for (; i >= 0 && j >= 0; i--, j--)
        {
            int temp;
            temp = (tl[i] - 48) - (tr[j] - 48);

            ans[cur] += temp;
            if (ans[cur] < 0)
            {
                ans[cur + 1]--;
                ans[cur] += 10;
            }
            cur++;
        }

        for (; i >= 0; i--)
        {
            ans[cur++] += tl[i] - 48;
        }

        BigNum answer = *this;
        answer.value.clear();
        answer.denominator.clear();
        while (ans.size())
        {
            answer.value += ans.back() + 48;
            ans.pop_back();
        }
        answer.value = answer.value.substr(answer.value.find_first_not_of('0'));
        if (negative)
        {
            string temp = "-";
            temp.append(answer.value);
            answer.value = temp;
        }
        answer.denominator = con;

        answer = reduction(answer);
        return answer;
    }

BigNum BigNum::operator*(BigNum r) {
    using namespace std;
    bool negative = false;
    if ((this->value[0] == '-') ^ (r.value[0] == '-'))
    {
        negative = true;
    }
    this->value = this->value.substr(this->value.find_first_not_of('-'));
    r.value = r.value.substr(r.value.find_first_not_of('-'));


    BigNum answer = *this;
    if (this->value.compare("0") == 0 || r.value.compare("0") == 0)
    {
        answer.value = "0";
        answer.denominator = "1";
        return answer;
    }

    answer.value = stringMuti(this->value, r.value);
    answer.denominator = stringMuti(this->denominator, r.denominator);
    if (negative)
    {
        answer.value.insert(answer.value.begin(), '-');
    }
    return reduction(answer);
}

BigNum BigNum::operator/(BigNum r) {
    using namespace std;
    bool negative = false;

    if (r.value.compare("0") == 0)
    {
        ErrorCode = 1;
        return r;
    }
    if (this->value.compare("0") == 0)
    {
        BigNum answer = *this;
        answer.value = "0";
        answer.denominator = "1";
        return answer;
    }

    if ((this->value[0] == '-') ^ (r.value[0] == '-'))
    {
        negative = true;
    }
    this->value = this->value.substr(this->value.find_first_not_of('-'));
    r.value = r.value.substr(r.value.find_first_not_of('-'));

    BigNum answer = *this;

    answer.value = stringMuti(this->value, r.denominator);
    answer.denominator = stringMuti(this->denominator, r.value);
    if (negative)
    {
        answer.value.insert(answer.value.begin(), '-');
    }
    return reduction(answer);;
}

BigNum BigNum::operator=(char c[]) {
    std::string s = c;
    BigNum tmp(s);
    *this = tmp;
    return *this;
}

BigNum BigNum::reduction(BigNum self) {
    using namespace std;
    if (self.value.compare("0") == 0)
    {
        self.denominator = "1";
        return self;
    }

    string Rvalue, Rdenominator;//方便運算
    Rvalue = self.value;
    Rdenominator = self.denominator;

    bool negative = false;
    if ((Rvalue[0] == '-') ^ (Rdenominator[0] == '-'))
    {
        negative = true;
    }
    Rvalue = Rvalue.substr(Rvalue.find_first_not_of('-'));
    Rdenominator = Rdenominator.substr(Rdenominator.find_first_not_of('-'));

    string r, c;
    if (Rvalue.compare(Rdenominator) == 0)
    {
        if (negative)
            self.value = "-1";
        else
            self.value = "1";

        self.denominator = "1";
        return self;
    }
    if (stringBigger(Rdenominator, Rvalue))//Rdenominator > Rvalue 
    {
        r = Rdenominator;
        c = Rvalue;
    }
    else {
        r = Rvalue;
        c = Rdenominator;
    }//r is the bigger one
    string temp;
    while (r.find_first_not_of('0') < r.size())
    {
        if (stringBigger(r, c))
        {
            temp = stringDivi(r, c);
            r = stringSub(r, stringMuti(c, temp));
            if (r.find_first_not_of('0') >= r.size())
                break;
            r = r.substr(r.find_first_not_of('0'));
        }
        else {
            temp = r;
            r = c;
            c = temp;
        }
    }
    BigNum answer = self;
    answer.value.clear();
    answer.denominator.clear();
    answer.value = stringDivi(Rvalue, c);
    if (negative && answer.value.compare("0") != 0)
    {
        answer.value.insert(answer.value.begin(), '-');
    }
    answer.denominator = stringDivi(Rdenominator, c);
    return answer;
}

 std::string toDecimal(BigNum self)
{
    bool negative = false;
    if (self.value[0] == '-')
    {
        negative = true;
        self.value = self.value.substr(1);
    }
    if (self.value.find_first_not_of('0') >= self.value.size())
    {
        std::string ans = "0.";
        for (int i = 0; i < 100; ++i)
            ans += '0';
        return ans;
    }

    std::string answer = stringDivi(self.value, self.denominator);
    if (answer.find_first_not_of('0') >= answer.size())
    {
        answer = "0.";
    }
    else {
        answer = answer.substr(answer.find_first_not_of('0'));
        self.value = stringSub(self.value, stringMuti(answer, self.denominator));
        //self.value = self.value.substr(self.value.find_first_not_of('0'));

        answer += '.';
    }

    std::string temp;
    for (int i = 0; i < 100; i++)
    {
        if (self.value.find_first_not_of('0') < self.value.size())
        {
            self.value = self.value.substr(self.value.find_first_not_of('0'));
            self.value += '0';
            temp = stringDivi(self.value, self.denominator)[0];//0<=&&<10
            answer += temp;
            self.value = stringSub(self.value, stringMuti(temp, self.denominator));

        }
        else {
            answer += '0';
        }

    }
    if (negative)
        answer.insert(answer.begin(), '-');

    return answer;
}

std::ostream& operator<< (std::ostream& os, const BigNum& num) {
    if (num.denominator=="1") os << num.value << "\n";
    else os << toDecimal(num) << "\n";
    return os;
}

std::istream& operator>> (std::istream& is, BigNum& num) {
    if (num.isInt) {
        std::string s;
        is >> s;
        size_t index = s.find('.');
        if (index != std::string::npos) s.erase(index);
        num.value = s;
    }
    else {
        std::string s;
        is >> s;
        BigNum tmp(s);
        num = tmp;
    }
    return is;
}

//num2 = 1/2 or -1/2
BigNum BigNum::root(BigNum num1, BigNum num2) {
    using namespace std;
    if (num2.value.compare("0") == 0 || num2.value.compare("-0") == 0)
    {
        BigNum ans;
        ans.value = "1";
        ans.denominator = "1";
        return ans;
    }
    if (num1.value[0] == '-')
    {
        cout << "error";
        return num2;
    }
    if (num2.value[0] == '-')
    {
        num2.value = num2.value.substr(1);
        string temp = num1.denominator;
        num1.denominator = num1.value;
        num1.value = temp;
        return root(num1, num2);
    }

    num1.value = stringMuti(num1.value, num1.denominator);
    for (int i = 0; i < 200; i++)
    {
        num1.value += '0';
    }
    string step = "1";
    for (int i = 0; i < num1.value.size() / 2 + 2; ++i)
    {
        step += '0';
    }
    string c = "1";
    for (int i = 0; i < num1.value.size() / 2 - 1; i++)
        c += '0';
    string temp, ans = stringDivi(num1.value, c);
    for (;;)
    {
        step = stringDivi(step, "2");
        if (step.compare("0") == 0)step = "1";
        if (stringBigger(ans, step))
        {
            temp = stringSub(ans, step);
            temp = temp.substr(temp.find_first_not_of('0'));
        }

        while (stringBigger(ans, step) && stringBigger(stringMuti(temp, temp), num1.value)) {
            ans = temp;
            if (stringBigger(ans, step))
                temp = stringSub(ans, step);
            else
                break;
            temp = temp.substr(temp.find_first_not_of('0'));
        }
        if (step.compare("1") == 0)break;
    }

    BigNum answer;
    answer.value = ans;
    answer.denominator = num1.denominator;
    answer.denominator.insert(answer.denominator.end(), 100, '0');

    return reduction(answer);
}

void BigNum::PrintArgs() {
    system("cls");
    std::cout << "**************************************************************************************************************\n";
    std::cout << "*                                               Your Variables                                               *\n";
    for (const auto it : BigNum::args) {
        std::string msg;
        int n = 2;
        n += it.first.length();
        n += it.second.value.length();
        if (it.second.denominator != "1") n += (it.second.denominator.length() + 5);
        msg += '*';
        msg += it.first;
        msg += " ";
        msg += toDecimal(it.second);
        msg += "*\n";
        msg.insert(msg.begin() + msg.length() - 2, 111 - msg.length(), ' ');
        std::cout << msg;
    }
    std::cout << "*                                                                                                            *\n";
    std::cout << "**************************************************************************************************************\n";
}