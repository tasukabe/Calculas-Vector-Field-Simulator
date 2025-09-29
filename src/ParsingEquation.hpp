#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <cmath>
#include <vector>
#include <array>

typedef struct vec3
{
	float x;
	float y;
	float z;
}vec3;

class P_Equation
{
public:
	P_Equation();

	void SetEquation(const std::string& equation);

	void GetValue(const vec3& v);

private:
	static constexpr std::size_t NumberOfOperations = 13;
	std::array<std::string, NumberOfOperations> Operations;

	std::unordered_map<std::string, std::function<float(float, float)>>
		OperationDictionary;


	// The higher the index the higher the priority.
	std::vector<std::vector<std::string>> Tokens;
	std::vector<char> TokenSeperators;

	std::string MapedEquation;

	float x, y, z, t;
	std::vector<float> constants;
	const float pi = 3.14159;
	const float e  = 2.71828;

	enum class MapingValues {
		x,
		y,
		z,
		t,
		constant,
		pi,
		e,
		seperator_start,
		seperator_end,
		operators,
		functional_operators,
		undefined
	};

private:

	void InitTokenizer();
	void TokenizeEquation(const std::string& equation);

};

// TARGET: parser "x^2 * y^3 * z * sin(x)"

/*
	Operations: + - * / ^ 
	Functional Operators: tanX sinX cosX sinhX coshX tanhX
	Seperators: ( )
*/
