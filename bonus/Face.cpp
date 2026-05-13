#include "Face.h"
#include <cmath>
#include <vector>
#include <climits>

// 1. Basic Geometry & Utilities

static void PlotCirclePoints(HDC hdc, int xc, int yc, int x, int y, COLORREF c) {
	SetPixel(hdc, xc + x, yc + y, c); SetPixel(hdc, xc - x, yc + y, c);
	SetPixel(hdc, xc + x, yc - y, c); SetPixel(hdc, xc - x, yc - y, c);
	SetPixel(hdc, xc + y, yc + x, c); SetPixel(hdc, xc - y, yc + x, c);
	SetPixel(hdc, xc + y, yc - x, c); SetPixel(hdc, xc - y, yc - x, c);
}

// Bilerp interpolation helper for filled shapes
static COLORREF Bilerp(COLORREF c1, COLORREF c2, COLORREF c3, COLORREF c4, float tx, float ty) {
	auto Lerp = [](BYTE a, BYTE b, float t) { return (BYTE)(a + t * (b - a)); };
	BYTE r = Lerp(Lerp(GetRValue(c1), GetRValue(c2), tx), Lerp(GetRValue(c3), GetRValue(c4), tx), ty);
	BYTE g = Lerp(Lerp(GetGValue(c1), GetGValue(c2), tx), Lerp(GetGValue(c3), GetGValue(c4), tx), ty);
	BYTE b = Lerp(Lerp(GetBValue(c1), GetBValue(c2), tx), Lerp(GetBValue(c3), GetBValue(c4), tx), ty);
	return RGB(r, g, b);
}

static void SimpleMidpointCircle(HDC hdc, int xc, int yc, int R, COLORREF c) {
	int x = 0, y = R;
	int d = 1 - R;
	PlotCirclePoints(hdc, xc, yc, x, y, c);
	while (x < y) {
		if (d < 0) d += 2 * x + 3;
		else { d += 2 * (x - y) + 5; y--; }
		x++;
		PlotCirclePoints(hdc, xc, yc, x, y, c);
	}
}

// Renders nose segment via parametric tracking
static void DrawParametricLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
	double dt = 1.0 / (std::max(std::abs(x2 - x1), std::abs(y2 - y1)) + 1);
	for (double t = 0.0; t <= 1.0; t += dt) {
		double x = x1 + t * (x2 - x1);
		double y = y1 + t * (y2 - y1);
		SetPixel(hdc, (int)std::round(x), (int)std::round(y), c);
	}
}

// 2. Filling and Advanced Geometry

struct Point2D {
	int x, y;
};

static void FloodFill4(HDC hdc, int startX, int startY, COLORREF oldColor, COLORREF newColor,
	int bx1 = 0, int by1 = 0, int bx2 = 4000, int by2 = 4000) {

	if (oldColor == newColor) return;

	COLORREF initialColor = GetPixel(hdc, startX, startY);
	if (initialColor != oldColor || initialColor == CLR_INVALID) return;

	std::vector<Point2D> pixelStack;
	pixelStack.push_back({ startX, startY });

	while (!pixelStack.empty()) {

		Point2D pt = pixelStack.back();
		pixelStack.pop_back();

		if (pt.x < bx1 || pt.x > bx2 || pt.y < by1 || pt.y > by2) continue;

		COLORREF currentColor = GetPixel(hdc, pt.x, pt.y);

		if (currentColor == oldColor && currentColor != CLR_INVALID) {
			SetPixel(hdc, pt.x, pt.y, newColor);

			pixelStack.push_back({ pt.x + 1, pt.y });
			pixelStack.push_back({ pt.x - 1, pt.y });
			pixelStack.push_back({ pt.x, pt.y + 1 });
			pixelStack.push_back({ pt.x, pt.y - 1 });
		}
	}
}

// Specialized function for the D-shaped happy smile boundary
static void DrawDShape(HDC hdc, int xc, int yc, int radius, COLORREF c) {
	int eyeOffset = radius / 3;
	POINT mStart = { xc - eyeOffset, yc + eyeOffset };
	POINT mEnd = { xc + eyeOffset, yc + eyeOffset };

	POINT T0 = { radius / 2, radius };
	POINT T1 = { radius / 2, -radius };

	// Set boundary points on Hermite curve
	for (double t = 0.0; t <= 1.0; t += 0.0005) {
		double t2 = t * t; double t3 = t2 * t;
		double h0 = 2 * t3 - 3 * t2 + 1; double h1 = t3 - 2 * t2 + t; double h2 = -2 * t3 + 3 * t2; double h3 = t3 - t2;
		SetPixel(hdc, (int)std::round(h0 * mStart.x + h1 * T0.x + h2 * mEnd.x + h3 * T1.x),
			(int)std::round(h0 * mStart.y + h1 * T0.y + h2 * mEnd.y + h3 * T1.y), c);
	}
	// Set boundary points on flat top line
	double dt = 1.0 / (std::max(std::abs(mEnd.x - mStart.x), std::abs(mEnd.y - mStart.y)) + 1);
	for (double t = 0.0; t <= 1.0; t += dt) {
		SetPixel(hdc, (int)std::round(mStart.x + t * (mEnd.x - mStart.x)), (int)std::round(mStart.y + t * (mEnd.y - mStart.y)), c);
	}
}

// Specialized function for the filled sad mouth boundary
static void DrawSadMouthShape(HDC hdc, int xc, int yc, int radius, COLORREF c) {
	int eyeOffset = radius / 3;
	POINT mStart = { xc - eyeOffset, yc + (int)(1.3 * eyeOffset) };
	POINT mEnd = { xc + eyeOffset, yc + (int)(1.3 * eyeOffset) };

	// Upward curve setup
	POINT T0 = { radius / 2, -radius / 2 };
	POINT T1 = { radius / 2, radius / 2 };

	// Set boundary points on Hermite curve
	for (double t = 0.0; t <= 1.0; t += 0.0005) {
		double t2 = t * t; double t3 = t2 * t;
		double h0 = 2 * t3 - 3 * t2 + 1; double h1 = t3 - 2 * t2 + t; double h2 = -2 * t3 + 3 * t2; double h3 = t3 - t2;
		SetPixel(hdc, (int)std::round(h0 * mStart.x + h1 * T0.x + h2 * mEnd.x + h3 * T1.x),
			(int)std::round(h0 * mStart.y + h1 * T0.y + h2 * mEnd.y + h3 * T1.y), c);
	}
	// Set boundary points on flat base line
	double dt = 1.0 / (std::max(std::abs(mEnd.x - mStart.x), std::abs(mEnd.y - mStart.y)) + 1);
	for (double t = 0.0; t <= 1.0; t += dt) {
		SetPixel(hdc, (int)std::round(mStart.x + t * (mEnd.x - mStart.x)), (int)std::round(mStart.y + t * (mEnd.y - mStart.y)), c);
	}
}

static void ConvexFill(HDC hdc, POINT* p, int n, COLORREF c1, COLORREF c2, COLORREF c3, COLORREF c4) {
	int minY = p[0].y, maxY = p[0].y;
	for (int i = 1; i < n; i++) {
		if (p[i].y < minY) minY = p[i].y;
		if (p[i].y > maxY) maxY = p[i].y;
	}
	std::vector<int> minX(maxY - minY + 1, INT_MAX), maxX(maxY - minY + 1, INT_MIN);
	for (int i = 0; i < n; i++) {
		POINT p1 = p[i], p2 = p[(i + 1) % n];
		if (p1.y == p2.y) continue;
		if (p1.y > p2.y) std::swap(p1, p2);
		for (int y = p1.y; y <= p2.y; y++) {
			int x = p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
			minX[y - minY] = std::min(minX[y - minY], x);
			maxX[y - minY] = std::max(maxX[y - minY], x);
		}
	}
	for (int y = minY; y <= maxY; y++) {
		for (int x = minX[y - minY]; x <= maxX[y - minY]; x++) {
			float tx = (float)(x - minX[y - minY]) / (maxX[y - minY] - minX[y - minY] + 1e-6f);
			float ty = (float)(y - minY) / (maxY - minY + 1);
			SetPixel(hdc, x, y, Bilerp(c1, c2, c3, c4, tx, ty));
		}
	}
}

static void DrawTriangle(HDC hdc, int xc, int yc, int radius, COLORREF c) {
	int eyeOffset = radius / 3;
	int baseHalf = radius / 12;
	POINT tP0 = { xc, yc - (eyeOffset / 2) };
	POINT tP1 = { xc - baseHalf, yc + (eyeOffset / 2) };
	POINT tP2 = { xc + baseHalf, yc + (eyeOffset / 2) };
	POINT triPoints[] = { tP0, tP1, tP2 };

	COLORREF cFill = c;
	ConvexFill(hdc, triPoints, 3, cFill, cFill, cFill, cFill);

	DrawParametricLine(hdc, tP0.x, tP0.y, tP1.x, tP1.y, c);
	DrawParametricLine(hdc, tP1.x, tP1.y, tP2.x, tP2.y, c);
	DrawParametricLine(hdc, tP2.x, tP2.y, tP0.x, tP0.y, c);
}

// Draws detailed eyes with white background and black pupils
static void DrawDetailedEyes(HDC hdc, int xc, int yc, int radius, COLORREF c, COLORREF bgColor) {
	int eyeOffset = radius / 3;
	int outerRadius = radius / 6;
	int innerRadius = radius / 15;

	int lx = xc - eyeOffset, ly = yc - eyeOffset;
	int rx = xc + eyeOffset, ry = yc - eyeOffset;

	SimpleMidpointCircle(hdc, lx, ly, outerRadius, c);
	SimpleMidpointCircle(hdc, rx, ry, outerRadius, c);

	FloodFill4(hdc, lx, ly, bgColor, RGB(255, 255, 255),
		lx - outerRadius, ly - outerRadius, lx + outerRadius, ly + outerRadius);
	FloodFill4(hdc, rx, ry, bgColor, RGB(255, 255, 255),
		rx - outerRadius, ry - outerRadius, rx + outerRadius, ry + outerRadius);

	SimpleMidpointCircle(hdc, lx, ly, innerRadius, c);
	SimpleMidpointCircle(hdc, rx, ry, innerRadius, c);

	FloodFill4(hdc, lx, ly, RGB(255, 255, 255), c,
		lx - innerRadius, ly - innerRadius, lx + innerRadius, ly + innerRadius);
	FloodFill4(hdc, rx, ry, RGB(255, 255, 255), c,
		rx - innerRadius, ry - innerRadius, rx + innerRadius, ry + innerRadius);
}

// 3. Main Face Rendering Functions

void DrawHappyFace(HDC hdc, int xc, int yc, int radius, COLORREF c, COLORREF bgColor) {
	SimpleMidpointCircle(hdc, xc, yc, radius, c);
	DrawDetailedEyes(hdc, xc, yc, radius, c, bgColor);
	DrawTriangle(hdc, xc, yc, radius, c);

	DrawDShape(hdc, xc, yc, radius, c);
	int eyeOffset = radius / 3;
	FloodFill4(hdc, xc, yc + (int)(1.2 * eyeOffset), bgColor, c,
		xc - radius, yc - radius, xc + radius, yc + radius);
}

void DrawSadFace(HDC hdc, int xc, int yc, int radius, COLORREF c, COLORREF bgColor) {
	SimpleMidpointCircle(hdc, xc, yc, radius, c);
	DrawDetailedEyes(hdc, xc, yc, radius, c, bgColor);
	DrawTriangle(hdc, xc, yc, radius, c);

	DrawSadMouthShape(hdc, xc, yc, radius, c);
	int eyeOffset = radius / 3;
	FloodFill4(hdc, xc, yc + (int)(1.15 * eyeOffset), bgColor, c,
		xc - radius, yc - radius, xc + radius, yc + radius);
}