#include "Stack.h"

using namespace std;

// ����ǥ����� ����ǥ������� �����ϴ� �Լ�
void Infix2Postfix(char* const input, char* output);
int Infix2Postfix_bracket(char* const input, char* output);
int Infix2Postfix_multiple(char* const input, char* output);

// ���������� �Ǵ� �Լ�
// 'input' �� ������('+', '-', '*', '/')�̸� true �̰�, �ƴϸ� false
bool isOperator(char input);

bool isNumSeparaor(char input);

// ������ �켱 �Ǵ� �Լ�
// oper1 �� '*' �̰�, oper2 �� '*', '/', '+', '-' �̸� true,
// oper1 �� '/' �̰�, oper2 �� '*', '/', '+', '-' �̸� true,
// oper1 �� '+' �̰�, oper2 �� '+', '-' �̸� true,
// oper1 �� '-' �̰�, oper2 �� '+', '-' �̸� true,
// �� �ܿ��� false
bool Oper_Precedence(char oper1, char oper2);

// ���� ǥ����� ����ϴ� �Լ�
int evalPostfix(char *exp);
int evalPostfix_multiple(char *exp);

// ������ ���ϴ� �Լ�
int myPow(int base, int n);

int carlc(int num1, int num2, char oper);

#define MAX_BUF_SIZE	256
#define NUM2CHAR(X)		((X) + '0')
#define CHAR2NUM(X)		((X) - '0')

// ���ڰ� ������ - ����(' ')
#define NUM_SEPARATOR	' '

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

bool isNumSeparaor(char input)
{
	return input == NUM_SEPARATOR;
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
	CStack<int>* pStack = CStack<int>::CreateS(MAX_BUF_SIZE);

	for (int i = 0; i < length; i++) {
		if (!isOperator(exp[i])) {
			pStack->push(CHAR2NUM(exp[i]));
		}
		else {
			int num1 = pStack->pop();
			int num2 = pStack->pop();
			switch (exp[i])
			{
			case '+': pStack->push(num2 + num1); break;
			case '-': pStack->push(num2 - num1); break;
			case '*': pStack->push(num2 * num1); break;
			case '/': pStack->push(num2 / num1); break;
			}
		}
	}

	int res = pStack->pop();

	CStack<int>::DeleteS(pStack);

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

int Infix2Postfix_bracket(char* const input, char* output)
{
	static int call_num = 0;			// ��ȣ ī��Ʈ
	int length = (int)strlen(input);
	CStack<char>* pStack = CStack<char>::CreateS(MAX_BUF_SIZE);

	for (int i = 0; i < length; i++) {
		if (isLeftBracket(input[i])) {
			char output_temp[MAX_BUF_SIZE] = { 0, };
			call_num++;

			i += Infix2Postfix_bracket(&input[++i], output_temp);
			
			// output_temp �� output �� ����
			copy_buf(output, output_temp);
			
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
			return i;
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
	return (call_num == 0) ? 0 : -1;
}

int Infix2Postfix_multiple(char* const input, char* output)
{
	static int call_num = 0;			// ��ȣ ī��Ʈ
	int length = (int)strlen(input);
	CStack<char>* pStack = CStack<char>::CreateS(MAX_BUF_SIZE);

	for (int i = 0; i < length; i++) {
		if (isLeftBracket(input[i])) {
			char output_temp[MAX_BUF_SIZE] = { 0, };
			call_num++;

			i += Infix2Postfix_bracket(&input[++i], output_temp);

			// output_temp �� output �� ����
			copy_buf(output, output_temp);

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
			return i;
		}
		else {
			if (!isOperator(input[i])) {				// ������ �ΰ�?
				*output = input[i];
				output++;
			}
			else {
				// ���� ������
				*output = NUM_SEPARATOR;
				output++;

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
	return (call_num == 0) ? 0 : -1;
}

int makeNum(CStack<char>* pStack_num)
{
	int num = 0, num_index = 0;
	while (!pStack_num->empty()) {
		num += CHAR2NUM(pStack_num->pop()) * myPow(10, num_index++);
	}
	return num;
}

int evalPostfix_multiple(char *exp)
{
	int length = (int)strlen(exp);
	CStack<int>* pStack = CStack<int>::CreateS(MAX_BUF_SIZE);
	CStack<char>* pStack_num = CStack<char>::CreateS(MAX_BUF_SIZE);
	int num = 0, num_index = 0;

	for (int i = 0; i < length; i++) {
		if (isOperator(exp[i])) {
			num = makeNum(pStack_num);
			pStack->push(num);

			int num1 = pStack->pop();
			int num2 = pStack->pop();
			pStack->push(carlc(num2, num1, exp[i]));
		}
		else if (isNumSeparaor(exp[i])) {
			num = 0, num_index = 0;
			while (!pStack_num->empty()) {
				num += CHAR2NUM(pStack_num->pop()) * myPow(10, num_index++);
			}
			pStack->push(num);
		}
		else {
			pStack_num->push(exp[i]);
		}
	}

	int res = pStack->pop();

	CStack<int>::DeleteS(pStack);
	CStack<char>::DeleteS(pStack_num);

	return res;
}

int myPow(int base, int n)
{
	if (n == 0)
		return 1;
	else
		return base * myPow(base, n - 1);
}

int carlc(int num1, int num2, char oper)
{
	switch (oper)
	{
	case '+': return num1 + num2;
	case '-': return num1 - num2;
	case '*': return num1 * num2;
	case '/': return num1 / num2;
	}
	return -1;
}