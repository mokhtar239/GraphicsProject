#include <cmath>
#include <windows.h>

void FillCircleWithCircles(HDC hdc, int xc, int yc, int R, int quarter, int color)
{
	if (R <= 0) return;

	double PI = 3.14159265359;
	double startRad = (quarter - 1) * PI / 2.0;
	double endRad = quarter * PI / 2.0;

	for (double r = 0.5; r <= R; r += 0.5)
	{
		double dtheta = 0.5 / r;

		for (double rad = startRad; rad <= endRad; rad += dtheta)
		{
			int x = xc + (int)std::round(r * std::cos(rad));
			int y = yc + (int)std::round(r * std::sin(rad));
			SetPixel(hdc, x, y, color);
		}
	}
}