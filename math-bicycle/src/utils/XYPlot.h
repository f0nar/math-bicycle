#ifndef _BICYCLE_XYPLOT_H_
#define _BICYCLE_XYPLOT_H_

#include <memory>
#include <cmath>
#include <functional>
#include <vector>
#include <map>
#include <limits>
#include <sstream>
#include "./Image.h"

namespace bm {

	using uchar = unsigned char;

	enum class GridType {
		Vertical,
		Horizontal,
		Both
	};

	template <typename T>
	struct XYPlot : public Image<uchar> {

		using XYPlotCurve = std::function<T(T)>;

		XYPlot(int w, int h) : Image<uchar>::Image(w, h, ColorRGB(255, 255, 255)) { }

		XYPlot(int w, int h, ColorRGB const& color) : Image<uchar>::Image(w, h, color), m_bgColor(color) { }

		XYPlot(std::string const& path) : Image<uchar>::Image(path) { }

		void addCurve(std::string const &name, XYPlotCurve const& curve, ColorRGB const &color) {
			m_curvesMap.emplace(name, XYPlotCurveData(curve, color));
		}

		void addTarget(T x, T y, ColorRGB const& color) {
			m_targets.emplace_back(x, y, color);
		}

		void addGrid(T step, ColorRGB const& color, GridType gridType = GridType::Both) {
			m_grids.emplace_back(step, color, gridType);
		}

		void setRange(T x0, T xn) {
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

		void setMaxY(T maxY) {
			m_yMax = maxY;
		}

		void setMinY(T minY) {
			m_yMin = minY;
		}

		void update() {
			fill(m_bgColor);

			int values = std::round(std::sqrt(m_height * m_height + m_width * m_width));
			std::vector<std::vector<T>> results;
			T minRes = INFINITY;
			T maxRes = -INFINITY;
			T const step = (m_xEnd - m_xStart) / (values - 1);

			for (auto const& [name, curveData] : m_curvesMap) {
				auto& resultsVector = results.emplace_back(values);
				for (int j = 0; j < values; ++j) {
					T resI = resultsVector[j] = curveData.func(m_xStart + j * step);
					if (resI < minRes) { minRes = resI; }
					else if (maxRes < resI) { maxRes = resI; }
				}
			}

			m_yStart = std::max(minRes, m_yMin);
			m_yEnd = std::min(maxRes, m_yMax);
			m_xScale = (m_width - 1.0) / (m_xEnd - m_xStart);
			m_yScale = (m_height - 1.0) / (m_yEnd - m_yStart);
			{
				Matrix<3, 3, T> imageToWorld({
					1.0 / m_xScale,  0,		       m_xStart,
					0,		         -1.0 / m_yScale, m_yEnd,
					0,		          0,		       1
				});
				m_worldToImage = imageToWorld.inv();
			}

			drawGrids();
			drawAxises();

			{
				int i = 0;
				T xStep = (m_xEnd - m_xStart) / (values - 1);
				for (auto const& [name, curveData] : m_curvesMap) {
					auto& resultsVec = results[i];
					for (int j = 0; j < values - 1; ++j) {
						T currRes = resultsVec[j];
						T nextRes = resultsVec[j + 1];
						if (!isnan(currRes) && !isnan(nextRes)) {
							Point<3, T> fromWorld(m_xStart + j * xStep, resultsVec[j], 1);
							Point<3, T> toWorld(m_xStart + (j + 1) * xStep, resultsVec[(j + 1)], 1);
							drawLine(toImage(fromWorld), toImage(toWorld), curveData.color);
						}
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
			XYPlotTargetData(T x, T y, ColorRGB const &color) : x(x), y(y), color(color) { }
			T x;
			T y;
			ColorRGB color;
		};

		struct XYPlotGridData {
			XYPlotGridData(T step, ColorRGB const &color, GridType type)
				: step(step), color(color), type(type) { }

			T step;
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
					int const startInPixels = imageZeroPoint.y % stepInPixels;
					drawHorizontalGrid(startInPixels, stepInPixels - 1, 1, grid.color);
				}
				if (grid.type != GridType::Horizontal) {
					int const stepInPixels = std::round(grid.step * m_xScale);
					int const startInPixels = imageZeroPoint.x % stepInPixels;
					drawVerticalGrid(startInPixels, stepInPixels - 1, 1, grid.color);
				}
			}
		}

		void drawCurveNames() {
			int const charSize = 8;
			int const verDistanceBetweenLabels = 2;
			int const horDistanceBetweenLabels = 2;
			int const curveMarkerLen = 10;
			int const rectangleOutterOffset = 10;
			int const rectangleInnerOffset = 5;
			int const borderWidth = 1;

			int maxNameLength = 0;
			int linesNumber = 0;
			std::map<std::string, std::vector<std::string>> nameLineMap;
			for (auto const& [name, curveData] : m_curvesMap) {
				std::stringstream nameStringStream(name);
				std::string line;
				std::vector<std::string> lines;
				while (std::getline(nameStringStream, line, '\n')) {
					lines.push_back(line);
					++linesNumber;
					if (line.size() > maxNameLength) maxNameLength = line.size();
				}
				nameLineMap[name] = lines;
			}

			int const rectangleWidth = 2 * borderWidth + 2 * rectangleInnerOffset + curveMarkerLen + horDistanceBetweenLabels + maxNameLength * charSize;
			int const rectangleHeight = 2 * borderWidth + 2 * rectangleInnerOffset + charSize * linesNumber + verDistanceBetweenLabels * (linesNumber - 1);
			int const rectangleLeftTopX = rectangleOutterOffset;
			int const rectangleLeftTopY = m_height - rectangleInnerOffset - rectangleHeight;

			drawRectangle(rectangleLeftTopX, rectangleLeftTopY, rectangleWidth, rectangleHeight, ColorRGB(0, 0, 0), ColorRGB(255, 255, 255));

			int namesDrawed = 0;
			int const lineX = rectangleOutterOffset + borderWidth + rectangleInnerOffset;
			for (auto const& [name, curveData] : m_curvesMap) {
				int linesDrawed = 0;
				const auto& lines = nameLineMap[name];
				const int curvemarkerLine = lines.size() / 2;
				int lineY = rectangleLeftTopY + borderWidth + rectangleInnerOffset + namesDrawed * (charSize + verDistanceBetweenLabels);
				for (;
					linesDrawed < curvemarkerLine;
					++linesDrawed, lineY += charSize + verDistanceBetweenLabels
				) {
					drawString(lineX + curveMarkerLen + horDistanceBetweenLabels, lineY, lines[linesDrawed], ColorRGB(0, 0, 0));
				}
				drawString(lineX + curveMarkerLen + horDistanceBetweenLabels, lineY, lines[linesDrawed], ColorRGB(0, 0, 0));
				drawHorizontalLine(lineX, lineY + charSize / 2, curveMarkerLen, curveData.color);
				for (;
					linesDrawed < lines.size();
					++linesDrawed, lineY += charSize + verDistanceBetweenLabels
				) {
					drawString(lineX + curveMarkerLen + horDistanceBetweenLabels, lineY, lines[linesDrawed], ColorRGB(0, 0, 0));
				}

				namesDrawed += linesDrawed;
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

		void drawTarget(T x, T y, ColorRGB const& color) {
			int const len = 9;
			int const lenDiv2 = len / 2;
			auto targetCenterPoint = toImage(Point<3, T>(x, y, 0));
			drawHorizontalLine(targetCenterPoint.x - lenDiv2, targetCenterPoint.y, len, color);
			drawVerticalLine(targetCenterPoint.x, targetCenterPoint.y - lenDiv2, len, color);
			drawLine(targetCenterPoint - Vector2i(lenDiv2), targetCenterPoint + Vector2i(lenDiv2), color);
			drawLine(targetCenterPoint + Vector2i(-lenDiv2, lenDiv2), targetCenterPoint + Vector2i(lenDiv2, -lenDiv2), color);
		}

	private:

		Point2i getImageZeroPoint() const {
			return toImage(Point<3, T>(0, 0, 1));
		}

		Point2i toImage(Point<2, T>const& worldPoint) const {
			return toImage(Point<3, T>(worldPoint.x, worldPoint.y, 1));
		}

		Point2i toImage(Point<3, T> const& worldPoint) const {
			auto imageVector = (m_worldToImage * worldPoint).xy();
			return Point2i(std::round(imageVector.x), std::round(imageVector.y));
		}

		bool m_enableXAxis = false;
		bool m_enableYAxis = false;
		bool m_enableCurveNamesTable = false;

		T m_xStart = -1.0f;
		T m_xEnd = 1.0f;
		T m_yStart = -1.0f;
		T m_yEnd = 1.0f;
		T m_xScale = 1.0f;
		T m_yScale = 1.0f;
		T m_yMax = std::numeric_limits<T>::max();
		T m_yMin = std::numeric_limits<T>::min();

		Matrix<3, 3, T> m_worldToImage;

		ColorRGB m_bgColor = ColorRGB(255, 255, 255);

		std::map<std::string, XYPlotCurveData> m_curvesMap;
		std::vector<XYPlotTargetData> m_targets;
		std::vector<XYPlotGridData> m_grids;
	};

}

#endif // !_BICYCLE_XYPLOT_H_