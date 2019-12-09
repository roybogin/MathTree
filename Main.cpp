#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <regex>
using namespace std;

struct NodeValue
{
	double value;
	bool isNum;

	NodeValue& operator=(const double& other)
	{
		value = other;
		isNum = true;
		return *this;
	}

	NodeValue& operator=(const char& other)
	{
		value = other;
		isNum = false;
		return *this;
	}

	
};


ostream& operator<<(ostream& os, const NodeValue& nd)
{
	if (nd.isNum)
	{
		os << nd.value;
	}
	else
	{
		os << (char)nd.value;
	}
	return os;
}

class BinMathTree
{
private:
	BinMathTree* left;
	BinMathTree* right;
	NodeValue value;
public:
	BinMathTree(double value)
	{
		this->value = value;
		this->value.isNum = true;
		left = NULL;
		right = NULL;
	}

	BinMathTree(char value)
	{
		this->value = value;
		this->value.isNum = false;
		left = NULL;
		right = NULL;
	}


	BinMathTree(double value, BinMathTree* left, BinMathTree* right)
	{
		this->value = value;
		this->value.isNum = true;
		this->left = left;
		this->right = right;
	}

	BinMathTree(char value, BinMathTree* left, BinMathTree* right)
	{
		this->value = value;
		this->value.isNum = false;
		this->left = left;
		this->right = right;
	}

	BinMathTree* getLeft()
	{
		return left;
	}

	BinMathTree* getRight()
	{
		return right;
	}

	bool isLeaf()
	{
		return (left == NULL && right == NULL);
	}

	NodeValue getValue()
	{
		return value;
	}
};

bool okayInBrackets(string eq, int idx)
{
	if (regex_match(eq, regex("\\([^()]*\\)")))
		return true;
	int cnt = 0;
	for (int i = 0; i < idx; i++)
	{
		if (eq[i] == '(')
			cnt++;
		if (eq[i] == ')')
			cnt--;
	}
	return cnt == 0;
}

BinMathTree* toTree(string eq)
{
	eq = regex_replace(eq, regex("\\s"), "");
	eq = regex_replace(eq, regex("e"), to_string(M_E));
	eq = regex_replace(eq, regex("pi"), to_string(M_PI));

	if (eq.find_first_not_of("0123456789.()") == eq.npos)
	{
		eq = regex_replace(eq, regex("\\("), "");
		eq = regex_replace(eq, regex("\\)"), "");
		return new BinMathTree(stod(eq));
	}
	string order[] = { "+-", "*/", "^" };
	int i = 0;
	int loc = eq.find_last_of(order[0]);

	while (loc == eq.npos || !okayInBrackets(eq, loc))
	{
		if (loc == eq.npos)
		{
			i++;
			loc = eq.find_last_of(order[i]);
		}
		else
		{
			loc = eq.find_last_of(order[i], loc - 1);
		}
	}
	return new BinMathTree(eq[loc], toTree(eq.substr(0, loc)), toTree(eq.substr(loc + 1)));
}

double eval(BinMathTree* tree)
{
	if (tree->isLeaf())
	{
		return tree->getValue().value;
	}
	char operand = tree->getValue().value;
	switch (operand)
	{
	case '+':
		return (eval(tree->getLeft()) + eval(tree->getRight()));
		break;
	case '-':
		return (eval(tree->getLeft()) - eval(tree->getRight()));
		break;
	case '*':
		return (eval(tree->getLeft()) * eval(tree->getRight()));
		break;
	case '/':
		return (eval(tree->getLeft()) / eval(tree->getRight()));
		break;
	case '^':
		return (pow(eval(tree->getLeft()), eval(tree->getRight())));
		break;
	}
}



int main()
{
	string eq;
	cout << "Enter problem:" << endl;
	cin >> eq;
	cout << eq << " = ";
	BinMathTree* tree = toTree(eq);
	cout << eval(tree);
	return 0;
}