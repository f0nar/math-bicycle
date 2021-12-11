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
	float coefficients[] = { 3.0f, 0.0f, -25.0f, 0.0f, 60.0f, 1.0f };
	float coefficients2[] = { 7.0f, -2.0f, -5.0f, 15.0f, 60.0f};

	PolynomicFunction polFunc(coefficients);
	XYPlot plot(width, height, ColorRGB(255, 255, 255));
	plot.setRange(xStart, xEnd);
	plot.addGrid(25, 25, 1, ColorRGB(200, 200, 200));
	plot.addCurve(polFunc, ColorRGB(128, 55, 0));
	plot.addCurve(PolynomicFunction(coefficients2), ColorRGB(0, 55, 128));

	float const step = (xEnd - xStart) / (targetsNumber + 1);
	float const halfStep = step / 2;
	for (int i = 0; i < targetsNumber; ++i) {
		float const x = xStart + halfStep + step * i;
		float const y = polFunc(x);
		plot.addTarget(x, y, ColorRGB(100, 100, 0));
	}

	plot.update();
	return plot.save("plot_test.jpg");
}
