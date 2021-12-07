#ifndef _BICYCLE_XYPLOT_H_
#define _BICYCLE_XYPLOT_H_

#include <memory>
#include <cmath>
#include "./Image.h"

namespace bm {

	using uchar = unsigned char;

	struct XYPlot : public Image<uchar> {

		XYPlot(int w, int h)
			: Image<uchar>::Image(w, h, ColorRGB(255, 255, 255)), m_xEnd(w), m_yEnd(h)
		{ }

		XYPlot(int w, int h, ColorRGB const& color)
			: Image<uchar>::Image(w, h, color), m_xEnd(w), m_yEnd(h)
		{ }

		XYPlot(std::string const& path) : Image<uchar>::Image(path) {
			m_xEnd = m_width;
			m_yEnd = m_height;
		}

		template <typename Func>
		void plot(Func func, float x0, float xn, ColorRGB const& color) {
			int values = std::round(std::sqrt(m_height * m_height + m_width * m_width) / 10);
			std::unique_ptr<float[]> results(new float[values]);

			float minRes = results[0] = func(x0);
			float maxRes = minRes;
			float const step = (xn - x0) / (values - 1);

			for (int i = 1; i < values; ++i) {
				float resI = results[i] = func(x0 + i * step);
				if (resI < minRes) minRes = resI;
				else if (maxRes < resI) maxRes = resI;
			}

			auto const ampl = maxRes - minRes;
			float const xScale = (m_width - 1.0f) / (values - 1.0f);
			float const yScale = (m_height - 1.0f) / ampl;

			for (int i = 0; i < values - 1; ++i) {
				int x0 = std::round(i * xScale);
				int xn = std::round((i + 1) * xScale);
				int y0 = m_height - 1 - std::round((results[i] - minRes) * yScale);
				int yn = m_height - 1 - std::round((results[(i + 1)] - minRes) * yScale);
				drawLine(x0, xn, y0, yn, color);
			}

			m_xStart = x0;
			m_xEnd = xn;
			m_yStart = minRes;
			m_yEnd = maxRes;
			m_xScale = (m_width - 1.0f) / (xn - x0);
			m_yScale = (m_height - 1.0f) / ampl;
		}

		void drawTarget(float x, float y, ColorRGB const& color) {
			if (x >= m_xStart && x <= m_xEnd && y >= m_yStart && y <= m_yEnd) {
				int const xCenter = std::round((x - m_xStart) * m_xScale);
				int const yCenter = m_height - 1 - std::round((y - m_yStart) * m_yScale);
				int const len = 5;
				int const lenDiv2 = len / 2;
				drawHorizontalLine(xCenter - lenDiv2, yCenter, len, color);
				drawVerticalLine(xCenter, yCenter - lenDiv2, len, color);
				drawLine(xCenter - lenDiv2, xCenter + lenDiv2, yCenter - lenDiv2, yCenter + lenDiv2, color);
				drawLine(xCenter - lenDiv2, xCenter + lenDiv2, yCenter + lenDiv2, yCenter - lenDiv2, color);
			}
		}

		float getXStart() const {
			return m_xStart;
		}

		float getYStart() const {
			return m_yStart;
		}

		float getXEnd() const {
			return m_xEnd;
		}

		float getYEnd() const {
			return m_yEnd;
		}

		float getXScale() const {
			return m_xScale;
		}

		float getYScale()  const {
			return m_yScale;
		}

	private:

		float m_xStart = 0.0f;
		float m_xEnd = 0.0f;
		float m_yStart = 0.0f;
		float m_yEnd = 0.0f;
		float m_xScale = 1.0f;
		float m_yScale = 1.0f;

	};

}

#endif // !_BICYCLE_XYPLOT_H_