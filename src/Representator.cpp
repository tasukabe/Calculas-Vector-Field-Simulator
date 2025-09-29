#include "Representator.hpp"

CVFR::CVFR()
	: camera{0}, maxColor(BLUE), minColor(YELLOW)
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "Calculas Vector Field Representator");
	SetTargetFPS(90);

	camera.position = Vector3{ 0.0f, 5.0f, 10.0f };
	camera.target = Vector3{ 0.f, 5.f, 0.f };
	camera.up = Vector3{ 0.f, 1.f, 0.f };
	camera.fovy = 100.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	arrowModel = LoadModel("Resources/Arrow.obj");

	InitVectorFieldAnglesArray();
}

CVFR::~CVFR()
{
	UnloadModel(arrowModel);
	CloseWindow();
}

const bool CVFR::isClosed() const
{
	return WindowShouldClose();
}

void CVFR::Update()
{
	UpdateCamera(&camera, CAMERA_THIRD_PERSON);
	if (IsWindowResized())
	{
		// You can get the new window dimensions
		int newWidth = GetScreenWidth();
		int newHeight = GetScreenHeight();
		//TraceLog(LOG_INFO, "Window resized to: %dx%d", newWidth, newHeight);
		// You might need to update your rendering logic or element positions here
		// to adapt to the new window size.
	}

	//arrowModel.transform = MatrixRotateXYZ(Vector3{ 0.f, static_cast<float>(GetTime()), 0.f});
	if (!isBaked) {
		isBaked = true;
		
		TraceLog(LOG_INFO, "Starting Baking...");
		
		CalculateAndRegisterPolarQuards();
		TraceLog(LOG_INFO, "Ending Baking...");
	}
}

void CVFR::Render()
{
	BeginDrawing();
	ClearBackground(BLACK);

	BeginMode3D(camera);

	/*DrawCube(Vector3{ 0.0f, 0.0f, 0.0f }, 3.0f, 3.0f, 3.0f, RED);
	DrawCubeWires(Vector3{ 0.0f, 0.0f, 0.0f }, 3.0f, 3.0f, 3.0f, MAROON);*/
	float x = 0, y = 0, z = 0;
		//TraceLog(LOG_INFO, "Started Drawing...");
	for (auto& vz : VectorFieldAngles)
	{
		for (auto& vy : vz)
		{
			for (auto& v : vy)
			{
				float colorGradeRatio = (v.intencity - minIntencity) / (maxIntencity - minIntencity);
				Color arrowColor = {
					(char)((float)(maxColor.r - minColor.r) * colorGradeRatio),
					(char)((float)(maxColor.g - minColor.g) * colorGradeRatio),
					(char)((float)(maxColor.b - minColor.b) * colorGradeRatio),
					255
				};

				arrowModel.transform = MatrixMultiply(MatrixRotateXYZ(Vector3{ v.alpha, v.beta, v.gamma}), MatrixScale(v.intencity / (4*maxIntencity), v.intencity / (4 * maxIntencity), v.intencity / (4 * maxIntencity)));
				//arrowModel.transform = MatrixRotateXYZ(Vector3{ v.alpha, v.gamma, v.beta });

				DrawModel(arrowModel, Vector3{x- SimulationX / 2, y- SimulationY / 2, z-SimulationZ/2}, 1, arrowColor);

				x+= 1.f;
			}
			x = 0.f;
			y+= 1.f;
		}
		x = 0.f;
		y = 0.f;
		z+= 1.f;
	}

	//TraceLog(LOG_INFO, "Ended Drawing...");

	DrawGrid(15, 1.0f);

	EndMode3D();

	EndDrawing();
}


void CVFR::InitVectorFieldAnglesArray()
{
	SimulationX = 20;
	SimulationY = 20;
	SimulationZ = 20;

	for (int k = 0; k < SimulationZ; k++)
	{
		VectorFieldAngles.push_back(std::vector<std::vector<PolarQuard>>());
		for (int j = 0; j < SimulationY; j++)
		{
			VectorFieldAngles[k].push_back(std::vector<PolarQuard>());
			
		}
	}
}


void CVFR::CalculateAndRegisterPolarQuards()
{
	PolarQuard p;

	for (int k = 0; k < SimulationZ; k++) {
		for (int j = 0; j < SimulationY; j++) {
			for (int i = 0; i < SimulationX; i++) {
				Vector3 initial = Vector3{(float)i-SimulationX/2, (float)j- SimulationY / 2, (float)k- SimulationZ / 2 };
				Vector3 last = fv(initial);
				Vector3 arrow = {
					last.x - initial.x,
					last.y - initial.y,
					last.z - initial.z,
				};

				p.alpha = acos(arrow.x / Vector3Length(arrow));
				p.beta  = acos(arrow.y / Vector3Length(arrow));
				p.gamma = acos(arrow.z / Vector3Length(arrow));
				p.intencity = Vector3Length(arrow);

				if (p.intencity > maxIntencity) { maxIntencity = p.intencity; }
				else if (p.intencity < minIntencity) { minIntencity = p.intencity; }

				//VectorFieldAngles[(5 + k)][(5 + j)][(5 + i)] = p;
				TraceLog(LOG_INFO, "Intencity At <%f, %f, %f>: %f", arrow.x, arrow.y, arrow.z, p.intencity);
				VectorFieldAngles[k][j].push_back(p);

			}
		}
	}
}



Vector3 CVFR::fv(Vector3 Point)
{
	return Vector3{
		Point.x - 3 * Point.y,
		3 * Point.y - Point.z,
		-4 * Point.z - 2 * Point.x
	};
}

