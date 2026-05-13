#include <cmath>
#include <windows.h>
#include "../lines/LineDDA.h"

void FillCircleWithLines(HDC hdc, int xc, int yc, int R, int quarter, int color)
{
	if (R <= 0) return;

	double PI = 3.14159265359;
	double startRad = (quarter - 1) * PI / 2.0;
	double endRad = quarter * PI / 2.0;

	double dtheta = 0.1 / R;

	for (double rad = startRad; rad <= endRad; rad += dtheta)
	{
		int x = xc + (int)std::round(R * std::cos(rad));
		int y = yc + (int)std::round(R * std::sin(rad));

		DrawLineDDA(hdc, xc, yc, x, y, color);
	}
}