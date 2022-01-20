#ifndef _BICYCLE_XYPLOT_H_
#define _BICYCLE_XYPLOT_H_

#include <memory>
#include <cmath>
#include <functional>
#include <vector>
#include <map>
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

		void addCurve(std::string const &name, XYPlotCurve const& curve, ColorRGB const &color) {
			m_curvesMap.emplace(name, XYPlotCurveData(curve, color));
		}

		void addTarget(float x, float y, ColorRGB const& color) {
			m_targets.emplace_back(x, y, color);
		}

		void addGrid(float step, ColorRGB const& color, GridType gridType = GridType::Both) {
			m_grids.emplace_back(step, color, gridType);
		}

		void setRange(float x0, float xn) {
			m_xStart = x0;
			m_xEnd = xn;
		}

		void enableXAxis(bool enable) {
			m_enableXAxis = enable;
		}

		void enableYAxis(bool enable) {
			m_enableYAxis = enable;
		}

		void enableCurveNamesTable(bool enable) {
			m_enableCurveNamesTable = enable;
		}

		void update() {
			fill(m_bgColor);

			int values = std::round(std::sqrt(m_height * m_height + m_width * m_width) / 10);
			std::vector<std::unique_ptr<float[]>> results;
			float minRes = INFINITY;
			float maxRes = -INFINITY;
			float const step = (m_xEnd - m_xStart) / (values - 1);

			for (auto const& [name, curveData] : m_curvesMap) {
				auto& resultsPtr = results.emplace_back(new float[values]);
				for (int j = 0; j < values; ++j) {
					float resI = resultsPtr[j] = curveData.func(m_xStart + j * step);
					if (resI < minRes) minRes = resI;
					else if (maxRes < resI) maxRes = resI;
				}
			}

			m_yStart = minRes;
			m_yEnd = maxRes;
			m_xScale = (m_width - 1.0f) / (m_xEnd - m_xStart);
			m_yScale = (m_height - 1.0f) / (m_yEnd - m_yStart);
			{
				Matrix3f imageToWorld({
					1.0f / m_xScale,  0,		       m_xStart,
					0,		         -1.0f / m_yScale, m_yEnd,
					0,		          0,		       1
				});
				m_worldToImage = imageToWorld.inv();
			}

			drawGrids();
			drawAxises();

			{
				int i = 0;
				float xStep = (m_xEnd - m_xStart) / (values - 1);
				for (auto const& [name, curveData] : m_curvesMap) {
					auto& resultsPtr = results[i];
					for (int j = 0; j < values - 1; ++j) {
						Vector3f fromWorld(j * xStep, resultsPtr[j], 1);
						Vector3f toWorld((j + 1) * xStep, resultsPtr[(j + 1)], 1);
						drawLine(toImage(fromWorld), toImage(toWorld), curveData.color);
					}
					++i;
				}
			}

			drawTargets();
			if (m_enableCurveNamesTable) drawCurveNames();
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
			XYPlotGridData(float step, ColorRGB const &color, GridType type)
				: step(step), color(color), type(type) { }

			float step;
			ColorRGB color;
			GridType type;
		};

		void drawTargets() {
			for (const auto& target : m_targets) {
				drawTarget(target.x, target.y, target.color);
			}
		}

		void drawGrids() {
			auto imageZeroPoint = getImageZeroPoint();
			for (const auto& grid : m_grids) {
				if (grid.type != GridType::Vertical) {
					int const stepInPixels =  std::round(grid.step * m_yScale);
					int const startInPixels = imageZeroPoint.x % stepInPixels;
					drawHorizontalGrid(startInPixels, stepInPixels - 1, 1, grid.color);
				}
				if (grid.type != GridType::Horizontal) {
					int const stepInPixels = std::round(grid.step * m_xScale);
					int const startInPixels = imageZeroPoint.y % stepInPixels;
					drawVerticalGrid(startInPixels, stepInPixels - 1, 1, grid.color);
				}
			}
		}

		void drawCurveNames() {
			int const curveNumber = m_curvesMap.size();
			int const charSize = 8;
			int const verDistanceBetweenLabels = 2;
			int const horDistanceBetweenLabels = 2;
			int const curveMarkerLen = 10;
			int const rectangleOutterOffset = 10;
			int const rectangleInnerOffset = 5;
			int const borderWidth = 1;

			int maxNameLength = 0;
			for (auto const& [name, curveData] : m_curvesMap) {
				if (name.size() > maxNameLength) maxNameLength = name.size();
			}

			int const rectangleWidth = 2 * borderWidth + 2 * rectangleInnerOffset + curveMarkerLen + horDistanceBetweenLabels + maxNameLength * charSize;
			int const rectangleHeight = 2 * borderWidth + 2 * rectangleInnerOffset + charSize * curveNumber + verDistanceBetweenLabels * (curveNumber - 1);
			int const rectangleLeftTopX = rectangleOutterOffset;
			int const rectangleLeftTopY = m_height - rectangleInnerOffset - rectangleHeight;

			drawRectangle(rectangleLeftTopX, rectangleLeftTopY, rectangleWidth, rectangleHeight, ColorRGB(0, 0, 0), ColorRGB(255, 255, 255));

			int i = 0;
			int const lineX = rectangleOutterOffset + borderWidth + rectangleInnerOffset;
			for (auto const& [name, curveData] : m_curvesMap) {
				int const lineY = rectangleLeftTopY + borderWidth + rectangleInnerOffset + i * (charSize + verDistanceBetweenLabels);
				drawHorizontalLine(lineX, lineY + charSize / 2, curveMarkerLen, curveData.color);
				drawString(lineX + curveMarkerLen + horDistanceBetweenLabels, lineY, name, ColorRGB(0, 0, 0));
				++i;
			}
		}

		void drawAxises() {
			auto imageZeroPoint = getImageZeroPoint();
			if (m_enableYAxis && m_xStart < 0 && m_xEnd > 0) {
				drawVerticalLine(imageZeroPoint.x, 0, m_height, ColorRGB(0, 0, 0));
			}
			if (m_enableXAxis && m_yStart < 0 && m_yEnd > 0) {
				drawHorizontalLine(0, imageZeroPoint.y, m_width, ColorRGB(0, 0, 0));
			}
		}

		void drawTarget(float x, float y, ColorRGB const& color) {
			int const len = 9;
			int const lenDiv2 = len / 2;
			auto targetCenterPoint = toImage(Vector3f(x, y, 1));
			drawHorizontalLine(targetCenterPoint.x - lenDiv2, targetCenterPoint.y, len, color);
			drawVerticalLine(targetCenterPoint.x, targetCenterPoint.y - lenDiv2, len, color);
			drawLine(targetCenterPoint - lenDiv2, targetCenterPoint + lenDiv2, color);
			drawLine(targetCenterPoint + Vector2i(-lenDiv2, lenDiv2), targetCenterPoint + Vector2i(lenDiv2, -lenDiv2), color);
		}

	private:

		Vector2i getImageZeroPoint() const {
			return toImage(Vector3f(0, 0, 1));
		}

		Vector2i toImage(Vector2f const& worldVector) const {
			return toImage(Vector3f(worldVector.x, worldVector.y, 1));
		}

		Vector2i toImage(Vector3f const& worldVector) const {
			auto imageVector2f = (m_worldToImage * worldVector).xy();
			return Vector2i(std::round(imageVector2f.x), std::round(imageVector2f.y));
		}

		bool m_enableXAxis = false;
		bool m_enableYAxis = false;
		bool m_enableCurveNamesTable = false;

		float m_xStart = -1.0f;
		float m_xEnd = 1.0f;
		float m_yStart = -1.0f;
		float m_yEnd = 1.0f;
		float m_xScale = 1.0f;
		float m_yScale = 1.0f;

		Matrix3f m_worldToImage;

		ColorRGB m_bgColor = ColorRGB(255, 255, 255);

		std::map<std::string, XYPlotCurveData> m_curvesMap;
		std::vector<XYPlotTargetData> m_targets;
		std::vector<XYPlotGridData> m_grids;
	};

}

#endif // !_BICYCLE_XYPLOT_H_