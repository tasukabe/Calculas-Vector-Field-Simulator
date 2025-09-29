#pragma once

#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "PolarQuard.hpp"
//#include "ParsingEquation.hpp"


/// <summary>
/// CVFR - Calculas Vector Field Representator
/// 
/// ******************************************************************
/// *** Note, Raylib generally uses Z as the distance from camera. ***
/// *** So, <x,y,z> would be redirected to <x, z, y>(At rendering).***
///	*** But it won't effect simulation at all.					   ***
/// ******************************************************************
/// 
/// </summary>

class CVFR
{
public:

	CVFR();
	~CVFR();

	void Update();
	void Render();

	const bool isClosed() const;
private:

	Camera3D camera;

	Model arrowModel;

	std::vector<std::vector<std::vector<PolarQuard>>> VectorFieldAngles;
	int SimulationX, SimulationY, SimulationZ;
	float maxIntencity, minIntencity;
	Color maxColor, minColor;
	
	void InitVectorFieldAnglesArray();

	bool isBaked = false;
	void CalculateAndRegisterPolarQuards();

	Vector3 fv(Vector3 Point);
};

