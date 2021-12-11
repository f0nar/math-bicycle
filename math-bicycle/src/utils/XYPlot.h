#ifndef _BICYCLE_XYPLOT_H_
#define _BICYCLE_XYPLOT_H_

#include <memory>
#include <cmath>
#include <functional>
#include <vector>
#include "./Image.h"

namespace bm {

	using uchar = unsigned char;

	using XYPlotCurve = std::function<float(float)>;

	enum class GridType {
		Vertical,
		Horizontal,
		Both
	};

	struct XYPlot : public Image<uchar> {

		XYPlot(int w, int h) : Image<uchar>::Image(w, h, ColorRGB(255, 255, 255)) { }

		XYPlot(int w, int h, ColorRGB const& color) : Image<uchar>::Image(w, h, color), m_bgColor(color) { }

		XYPlot(std::string const& path) : Image<uchar>::Image(path) { }

		void addCurve(XYPlotCurve const& plot, ColorRGB const &color) {
			m_curves.emplace_back(plot, color);
		}

		void addTarget(float x, float y, ColorRGB const& color) {
			m_targets.emplace_back(x, y, color);
		}

		void addGrid(float start, float step, float width, ColorRGB const& color, GridType gridType = GridType::Both) {
			m_grids.emplace_back(start, step, width, color, gridType);
		}

		void setRange(float x0, float xn) {
			m_xStart = x0;
			m_xEnd = xn;
		}

		void update() {
			drawGrids();
			drawCurves();
			drawTargets();
		}

	protected:

		struct XYPlotCurveData {
			XYPlotCurveData(XYPlotCurve const &func, ColorRGB const &color) : func(func), color(color) { }
			XYPlotCurve func;
			ColorRGB color;
		};

		struct XYPlotTargetData {
			XYPlotTargetData(float x, float y, ColorRGB const &color) : x(x), y(y), color(color) { }
			float x;
			float y;
			ColorRGB color;
		};

		struct XYPlotGridData {
			XYPlotGridData(float start, float step, float width, ColorRGB const &color, GridType type)
				: start(start), step(step), width(width), color(color), type(type) { }

			float start;
			float step;
			float width;
			ColorRGB color;
			GridType type;
		};

		void drawCurves() {
			int values = std::round(std::sqrt(m_height * m_height + m_width * m_width) / 10);
			std::vector<std::unique_ptr<float[]>> results;
			float minRes = INFINITY;
			float maxRes = -INFINITY;
			float const step = (m_xEnd - m_xStart) / (values - 1);

			for (int i = 0, curvesNumber = m_curves.size(); i < curvesNumber; ++i) {
				auto const& curve = m_curves[i];
				auto& resultsPtr = results.emplace_back(new float[values]);
				for (int j = 0; j < values; ++j) {
					float resI = resultsPtr[j] = curve.func(m_xStart + j * step);
					if (resI < minRes) minRes = resI;
					else if (maxRes < resI) maxRes = resI;
				}
			}

			auto const ampl = maxRes - minRes;
			float const xScale = (m_width - 1.0f) / (values - 1.0f);
			float const yScale = (m_height - 1.0f) / ampl;

			for (int i = 0, curvesNumber = m_curves.size(); i < curvesNumber; ++i) {
				auto const& curve = m_curves[i];
				auto& resultsPtr = results[i];
				for (int j = 0; j < values - 1; ++j) {
					int x0 = std::round(j * xScale);
					int xn = std::round((j + 1) * xScale);
					int y0 = m_height - 1 - std::round((resultsPtr[j] - minRes) * yScale);
					int yn = m_height - 1 - std::round((resultsPtr[(j + 1)] - minRes) * yScale);
					drawLine(x0, xn, y0, yn, curve.color);
				}
			}

			m_yStart = minRes;
			m_yEnd = maxRes;
			m_xScale = (m_width - 1.0f) / (m_xEnd - m_xStart);
			m_yScale = yScale;
		}

		void drawTargets() {
			for (const auto& target : m_targets) {
				drawTarget(target.x, target.y, target.color);
			}
		}

		void drawGrids() {
			for (const auto& grid : m_grids) {
				switch (grid.type) {
					case GridType::Vertical: { drawVerticalGrid(grid.start, grid.step, grid.width, grid.color); break; }
					case GridType::Horizontal: { drawHorizontalGrid(grid.start, grid.step, grid.width, grid.color); break; }
					case GridType::Both: {
						drawVerticalGrid(grid.start, grid.step, grid.width, grid.color);
						drawHorizontalGrid(grid.start, grid.step, grid.width, grid.color);
						break;
					}
				}
			}
		}

		void drawTarget(float x, float y, ColorRGB const& color) {
			int const xCenter = std::round((x - m_xStart) * m_xScale);
			int const yCenter = m_height - 1 - std::round((y - m_yStart) * m_yScale);
			int const len = 9;
			int const lenDiv2 = len / 2;
			drawHorizontalLine(xCenter - lenDiv2, yCenter, len, color);
			drawVerticalLine(xCenter, yCenter - lenDiv2, len, color);
			drawLine(xCenter - lenDiv2, xCenter + lenDiv2, yCenter - lenDiv2, yCenter + lenDiv2, color);
			drawLine(xCenter - lenDiv2, xCenter + lenDiv2, yCenter + lenDiv2, yCenter - lenDiv2, color);
		}

	private:

		float m_xStart = -1.0f;
		float m_xEnd = 1.0f;
		float m_yStart = -1.0f;
		float m_yEnd = 1.0f;
		float m_xScale = 1.0f;
		float m_yScale = 1.0f;

		ColorRGB m_bgColor = ColorRGB(255, 255, 255);

		std::vector<XYPlotCurveData> m_curves;
		std::vector<XYPlotTargetData> m_targets;
		std::vector<XYPlotGridData> m_grids;
	};

}

#endif // !_BICYCLE_XYPLOT_H_