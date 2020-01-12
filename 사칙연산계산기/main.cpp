#include "Stack.h"

using namespace std;

void Infix2Postfix(char* const input, char* output);
bool isOperator(char input);
bool Oper_Precedence(char oper1, char oper2);

#define MAX_BUF_SIZE	256

int main()
{
	char input[MAX_BUF_SIZE] = { '\0', };
	char output[MAX_BUF_SIZE] = { '\0', };

	cout << "����� ���� �Է��� �ּ���.(��: 3*8)" << endl;
	cin >> input;

	Infix2Postfix(input, output);

	cout << output;

	return 0;
}

void Infix2Postfix(char* const input, char* output)
{
	int inputSize = (int)strlen(input);
	CStack<char>* pStack = CStack<char>::CreateS(MAX_BUF_SIZE);

	for (int i = 0; i < inputSize; i++) {
		if (!isOperator(input[i])) {				// ������ �ΰ�?
			*output = input[i];
			output++;
		}
		else {
			// ���ؿ� �ִ� ������ ���� input[i] �� �켱������ ������
			// ���ؿ� �ִ� �����ڸ�  Pop �մϴ�.
			while (Oper_Precedence(pStack->getData(), input[i])) {
				*output = pStack->pop();
				output++;
			}
			pStack->push(input[i]);
		}
	}

	// ���� ������ ���
	while (!pStack->empty()) {
		*output = pStack->pop();
		output++;
	}

	CStack<char>::DeleteS(pStack);
}

bool isOperator(char input)
{
	return (input == '+' || input == '-' || input == '*' || input == '/') ? true : false;
}

bool Oper_Precedence(char oper1, char oper2)
{
	switch (oper1)
	{
	case '*':
	case '/':
		return true;
	case '+':
	case '-':
		if (oper2 == '+' || oper2 == '-')
			return true;

	default:
		break;
	}

	return false;
}