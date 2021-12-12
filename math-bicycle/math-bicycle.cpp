﻿#include "src/Color.h"
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
	auto X = getXfunc();

	auto polFunc1 = 3.0f * X * X * X * X * X - 25.0f * X * X * X + 60.0f * X + 1.0f;
	auto polFunc2 = 7.0f * X * X * X * X - 2.0f * X * X * X + -5.0f * X * X + 15.0f;
	XYPlot plot(width, height, ColorRGB(255, 255, 255));
	plot.setRange(xStart, xEnd);
	plot.addGrid(25, 25, 1, ColorRGB(200, 200, 200));
	plot.addCurve(polFunc1, ColorRGB(128, 55, 0));
	plot.addCurve(polFunc2, ColorRGB(0, 55, 128));

	for (int i = 0; i < targetsNumber; ++i) {
		float const x = xStart + halfStep + step * i;
		plot.addTarget(x, polFunc1(x), ColorRGB(100, 100, 0));
	}

	plot.update();
	return plot.save("plot_test.jpg");
}
