#include <array>

#include "src/Color.h"
#include "src/PolynomicFunction.h"
#include "src/utils/XYPlot.h"

using namespace bm;

int main() {
	int const width = 512;
	int const height = 512;
	float const xStart = -2.5;
	float const xEnd = 2.5;
	int const targetsNumber = 5;
	float const step = (xEnd - xStart) / (targetsNumber + 1);
	float const halfStep = step / 2;
	Xf X;

	auto polFunc1 = 3.0f * X * X * X * X * X - 25.0f * X * X * X + 60.0f * X + 1.0f;
	// auto polFunc2 = 7.0f * X * X * X * X - 2.0f * X * X * X + -5.0f * X * X + 15.0f;
	// auto polFunc3 = 9.0f * X * (X - xStart) * (X - xEnd);

	XYPlot plot(width, height, ColorRGB(255, 255, 255));
	plot.setRange(xStart, xEnd);
	plot.enableXAxis(true);
	plot.enableYAxis(true);
	plot.enableCurveNamesTable(true);
	plot.addGrid(0.5, ColorRGB(200, 200, 200), GridType::Vertical);
	plot.addGrid(10, ColorRGB(200, 200, 200), GridType::Horizontal);
	plot.addCurve(polFunc1.toString(), polFunc1, ColorRGB(128, 55, 0));
	// plot.addCurve(polFunc2.toString(), polFunc2, ColorRGB(0, 55, 128));
	// plot.addCurve(polFunc3.toString(), polFunc3, ColorRGB(0, 128, 128));

	std::array<Vector2f, targetsNumber> poly_points;
	for (int i = 0; i < targetsNumber; ++i) {
		auto& point = poly_points[i];
		point.x = xStart + halfStep + step * i;
		point.y = polFunc1(point.x);
		plot.addTarget(point.x, point.y, ColorRGB(100, 100, 0));
	}
	auto const& recoveredFunc = fitPoly(poly_points);
	plot.addCurve(recoveredFunc.toString(), recoveredFunc, ColorRGB(0, 55, 128));

	plot.update();
	return plot.save("plot_test.jpg");
}
