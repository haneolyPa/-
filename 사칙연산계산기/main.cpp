#include "Stack.h"

using namespace std;

// ����ǥ����� ����ǥ������� �����ϴ� �Լ�
void Infix2Postfix(char* const input, char* output);
bool Infix2Postfix_bracket(char* const input, char* output);

// ���������� �Ǵ� �Լ�
// 'input' �� ������('+', '-', '*', '/')�̸� true �̰�, �ƴϸ� false
bool isOperator(char input);

// ������ �켱 �Ǵ� �Լ�
// oper1 �� '*' �̰�, oper2 �� '*', '/', '+', '-' �̸� true,
// oper1 �� '/' �̰�, oper2 �� '*', '/', '+', '-' �̸� true,
// oper1 �� '+' �̰�, oper2 �� '+', '-' �̸� true,
// oper1 �� '-' �̰�, oper2 �� '+', '-' �̸� true,
// �� �ܿ��� false
bool Oper_Precedence(char oper1, char oper2);

// ���� ǥ����� ����ϴ� �Լ�
int evalPostfix(char *exp);

#define MAX_BUF_SIZE	256
#define NUM2CHAR(X)		(X) + '0'
#define CHAR2NUM(X)		(X) - '0'

int main()
{
	char input[MAX_BUF_SIZE] = { '\0', };
	char output[MAX_BUF_SIZE] = { '\0', };

	cout << "����� ���� �Է��� �ּ���.(��: 3*8)" << endl;
	cin >> input;

	Infix2Postfix(input, output);

	cout << endl << "���� ǥ������� ����� ����" << endl;
	cout << output << endl;

	cout << "��� ����� : " << endl;
	cout << evalPostfix(output) << endl;

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


int evalPostfix(char *exp)
{
	int length = (int)strlen(exp);
	CStack<char>* pStack = CStack<char>::CreateS(MAX_BUF_SIZE);

	for (int i = 0; i < length; i++) {
		if (!isOperator(exp[i])) {
			pStack->push(exp[i]);
		}
		else {
			int num1 = CHAR2NUM(pStack->pop());
			int num2 = CHAR2NUM(pStack->pop());
			switch (exp[i])
			{
			case '+': pStack->push(NUM2CHAR(num2 + num1)); break;
			case '-': pStack->push(NUM2CHAR(num2 - num1)); break;
			case '*': pStack->push(NUM2CHAR(num2 * num1)); break;
			case '/': pStack->push(NUM2CHAR(num2 / num1)); break;
			}
		}
	}

	int res = CHAR2NUM(pStack->pop());

	CStack<char>::DeleteS(pStack);

	return res;
}

bool isLeftBracket(char input)
{
	return input == '(' ? true : false;
}

bool isRightBracket(char input)
{
	return input == ')' ? true : false;
}

void copy_buf(char* input, char* output)
{
	int length = (int)strlen(output);
	for (int i = 0; i < length; i++) {
		input[i] = output[i];
	}
}

bool Infix2Postfix_bracket(char* const input, char* output)
{
	static int call_num = 0;			// ��ȣ ī��Ʈ
	int length = (int)strlen(input);
	CStack<char>* pStack = CStack<char>::CreateS(MAX_BUF_SIZE);

	for (int i = 0; i < length; i++) {
		if (isLeftBracket(input[i])) {
			char output_temp[MAX_BUF_SIZE] = { 0, };
			call_num++;
			Infix2Postfix_bracket(&input[++i], output_temp);
			// output_temp �� output �� ����
			copy_buf(output, output_temp);
			i += (int)(strlen(output_temp));
			output += strlen(output_temp);
		}
		else if (isRightBracket(input[i])) {
			call_num--;
			// ���� ������ ���
			while (!pStack->empty()) {
				*output = pStack->pop();
				output++;
			}

			CStack<char>::DeleteS(pStack);
			return true;
		}
		else {
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
	}

	// ���� ������ ���
	while (!pStack->empty()) {
		*output = pStack->pop();
		output++;
	}

	CStack<char>::DeleteS(pStack);
	return (call_num == 0) ? true : false;
}