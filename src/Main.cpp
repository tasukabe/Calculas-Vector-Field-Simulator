#include "Representator.hpp"

int main()
{
	CVFR app;
	while (!app.isClosed())
	{
		app.Update();
		app.Render();
	}
}