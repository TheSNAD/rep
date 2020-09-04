#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <cassert>

using namespace std;

bool operation(char op){
    return op == '+' || op == '-' || op == '*' || op == '/';
}

int priority(char op){
	if (op < 0)
		return 3;
	else {
		if(op == '+' || op == '-')
			return 1;
		else
			if(op == '*' || op == '/' )
				return 2;
			else
				return -1; // ( || )
	}
}

void action(vector<double> &value, char op){
    if(op < 0){
        double unitar = value.back();
        value.pop_back();
        if (-op == '-')
        	value.push_back(-unitar);
    }
    else{
        double right = value.back();
        value.pop_back();
        double left = value.back();
        value.pop_back();
        if (op=='+') value.push_back(left+right);
        else if(op=='-') value.push_back(left-right);
        else if(op=='*') value.push_back(left*right);
        else if(op=='/') value.push_back(left/right);
    }
}

double calc(string &expr){
    bool unary = true;
    vector<double> value;
    vector<char> op;

    for(int i = 0; i < expr.size(); i++){
            if(expr[i] == '('){
                op.push_back('(');
                unary = true;
            }
            else if(expr[i] == ')'){
                while(op.back() != '('){
                    action(value, op.back());
                    op.pop_back();
                }
                op.pop_back();
                unary = false;
            }
            else if(operation(expr[i])){
                char inst = expr[i];
                if(unary == true) //unary operator
                	inst = -inst;
                while(!op.empty() && priority(op.back()) >= priority(inst)){
                    action(value, op.back());
                    op.pop_back();
                }
                op.push_back(inst);
                unary = true;
            }
            else if(isdigit(expr[i])){	// number corvertation
                string number;
                int dot = 1;
                while(i < expr.size() && (isdigit(expr[i]) || expr[i] == ',' || expr[i] == '.'))
                	if(expr[i] == ',') {
                		number += '.';
                		i++;
                	} else {
                	number += expr[i++];
                }
                i--;
                value.push_back(strtod(number.c_str(), NULL));
                unary = false;
            }
            else {
                string err;
                while(i < expr.size() && !isdigit(expr[i]) && !operation(expr[i]))
                	err += expr[i++];
                cout << "wrong expr part: " << err << endl;
                exit(1);
            }
        }
    while(!op.empty()){
        action(value, op.back());
        op.pop_back();
    }
    return value.back();
}

double Roundf(double e) {
	int z = 1;
	if (e < 0)
		z = -1;
	e = z*int(abs(e)*100 + 0.5)/100.0;
	return e;
}

void DelIndent(string& expr) {
	expr.erase(remove_if(begin(expr), end(expr), [l = locale{}](auto ch) { return isspace(ch, l); }), end(expr));
}

void IndentTest() {
	string s = " -  1       * 2";
	string s2 = "   1       + (10 *     1   .2)  +    2";
	string s3 = " 1       *             1  ,   2";

	DelIndent(s);
	DelIndent(s2);
	DelIndent(s3);

	assert(Roundf(calc(s)) == -2);
	assert(Roundf(calc(s2)) == 15);
	assert(Roundf(calc(s3)) == 1.2);

	return;
}

void RoundingTest() {
	string s = "-1 * 2.666";
	string s2 = "2.455 + 2.455 + 2.455 + 2.455 + 2.455 + 2.455";
	string s3 = "-1.127 + 1,002";

	DelIndent(s);
	DelIndent(s2);
	DelIndent(s3);

	assert(Roundf(calc(s)) == -2.67);
	assert(Roundf(calc(s2)) == 14.73);
	assert(Roundf(calc(s3)) == -0.13);

	return;
}

void BracketsTest() {
	string s = "1*(2-2*(11-9))";
	string s2 = "(1-3/(1+2*(2-1)))";
	string s3 = "(40-(2+5*7)) - (2*8/4)*(10-9)";

	DelIndent(s);
	DelIndent(s2);
	DelIndent(s3);

	assert(Roundf(calc(s)) == -2);
	assert(Roundf(calc(s2)) == 0);
	assert(Roundf(calc(s3)) == -1);

	return;
}

void TestRunner() {
	IndentTest();
	RoundingTest();
	BracketsTest();

	return;
}

int main() {
	TestRunner();

    string expr;

    getline(cin, expr);
    expr.erase(remove_if(begin(expr), end(expr), [l = locale{}](auto ch) { return isspace(ch, l); }), end(expr));

    double res = Roundf(calc(expr));
    cout << res;

    return 0;
}