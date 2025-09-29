#include "ParsingEquation.hpp"

P_Equation::P_Equation()
{
	auto Add   = [](float a, float b) { return a + b; };
	auto Sub   = [](float a, float b) { return a - b; };
	auto Multi = [](float a, float b) { return a * b; };
	auto Divid = [](float a, float b) { return 0 == b ? -1 : a/b ; };
	auto Pow   = [](float a, float b) { return powf(a, b); };
	
	auto SIN  = [](float, float x) { return sinf(x); };
	auto COS  = [](float, float x) { return cosf(x); };
	auto TAN  = [](float, float x) { return tanf(x); };
	auto SINH = [](float, float x) { return sinhf(x); };
	auto COSH = [](float, float x) { return coshf(x); };
	auto TANH = [](float, float x) { return tanhf(x); };

	auto PI = [=](float, float) { return pi; };
	auto E  = [=](float, float) { return e; };


	Operations[0] = "+";
	Operations[1] = "-";
	Operations[2] = "*";
	Operations[3] = "/";
	Operations[4] = "^";
	Operations[5] = "sin";
	Operations[6] = "cos";
	Operations[7] = "tan";
	Operations[8] = "sinh";
	Operations[9] = "cosh";
	Operations[10] = "tanh";
	Operations[11] = "pi";
	Operations[12] = "e";
	

	OperationDictionary[Operations[0]]	= Add;
	OperationDictionary[Operations[1]]	= Sub;
	OperationDictionary[Operations[2]]	= Multi;
	OperationDictionary[Operations[3]]	= Divid;
	OperationDictionary[Operations[4]]	= Pow;
	OperationDictionary[Operations[5]]	= SIN;
	OperationDictionary[Operations[6]]	= COS;
	OperationDictionary[Operations[7]]  = TAN;
	OperationDictionary[Operations[8]]  = SINH;
	OperationDictionary[Operations[9]]  = COSH;
	OperationDictionary[Operations[10]] = TANH;
	OperationDictionary[Operations[11]] = PI;
	OperationDictionary[Operations[12]] = E;

	InitTokenizer();

}

void P_Equation::SetEquation(const std::string& equation)
{
	MapingValues previous;

	TokenizeEquation(equation);

	//for (auto c : equation)
	//{
	///*	if (c == ' ') { continue; }
	//	if (c == 'x') { previous = MapingValues::x; continue; }
	//	if (c == 'y') { previous = MapingValues::y; continue; }
	//	if (c == 'z') { previous = MapingValues::z; continue; }
	//	if (c == 't') { previous = MapingValues::t; continue; }
	//	if (c == 'p') { previous = MapingValues::pi; continue; }
	//	if (c == 'e') { previous = MapingValues::e; continue; }
	//	if (c == '(') { previous = MapingValues::seperator_start; continue; }
	//	if (c == ')') { previous = MapingValues::seperator_end; continue; }
	//	if (c == '+' || c == '-' || c == '*' || c == '/') { previous = MapingValues::seperator_end; continue; }
	//	if (c == 's' || c == 'c' || c == 't' ) { previous = MapingValues::seperator_end; continue; }
	//*/	
	//}
}

void P_Equation::GetValue(const vec3& v)
{

}


void P_Equation::InitTokenizer()
{
	Tokens.reserve(4);
	TokenSeperators.push_back(' ');
	TokenSeperators.push_back('(');
}

void P_Equation::TokenizeEquation(const std::string& equation)
{

#define PUSH_TOKEN													\
	if(currentToken != "") {										\
		Tokens[currentTokenListIndex].push_back(currentToken);		\
		currentToken = "";											\
	}

#define PUSH_TOKEN_n												\
	Tokens[currentTokenListIndex].push_back(currentToken);			\
	currentToken = "";

#define ADD_NEW_TOKEN_LIST											\
	Tokens.push_back(std::vector<std::string>());					\
	currentTokenListIndex++;
	
#define SHOULD_GO if (go) { go = false; continue; }
	
	/*
	* If equation has { ' ', '(', OPERATORS, SPECIAL_CONSTANTS }, then evaluate new token.
	* '~' means no token in other words
 	*/

	int currentTokenListIndex = 0;	// Local Token list Index
	std::string currentToken = "";

	bool go = false;

	for (auto c : equation)
	{
		if (c == ' ')
		{
			PUSH_TOKEN
			continue;
		}
		if(c == '(')
		{
			PUSH_TOKEN
			auto token_c = *(Tokens[currentTokenListIndex].end()-1)->end();

			if (token_c != '*' || token_c != '/' || token_c != '+' || token_c != '-')
			{
				currentToken = '*';
				PUSH_TOKEN
			}
			ADD_NEW_TOKEN_LIST
			continue;
		}
		if (c == ')')
		{
			// then go low
			currentTokenListIndex--;
			continue;
		}

		if (std::isdigit(*(currentToken.end()-1)) != std::isdigit(c))
		{
			PUSH_TOKEN
				currentToken += c;

		}

		//If operator Appeares.
		for (auto o = Operations.begin(); o < Operations.begin() + 5; o++)
		{
			if (c == (*o)[0])
			{
				PUSH_TOKEN
				currentToken += c;
				PUSH_TOKEN
				go = true;
				break;
			}
		}
		SHOULD_GO

		//If Functional Operator OR Special Constants appeares.
		for (auto fo = Operations.begin() + 5; fo < Operations.end(); fo++)
		{
			if (currentToken == *fo)
			{
				PUSH_TOKEN_n
					go = true;
				break;
			}
		}
		SHOULD_GO

		//IF NOWHERE TO GO...
		currentToken += c;
	}
}

