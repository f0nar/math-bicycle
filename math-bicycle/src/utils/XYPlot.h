#ifndef _BICYCLE_XYPLOT_H_
#define _BICYCLE_XYPLOT_H_

#include <memory>
#include <cmath>
#include "./Image.h"

namespace bm {

	using uchar = unsigned char;

	struct XYPlot : public Image<uchar> {

		using Image<uchar>::Image;

		template <typename Func, typename ArgT, typename ResT = ArgT>
		void plot(Func func, ArgT x0, ArgT xn, int values, ColorRGB<uchar> const& color) {
			std::unique_ptr<ResT[]> results(new ResT[values]);

			ResT minRes = results[0] = func(x0);
			ResT maxRes = minRes;
			ArgT const step = (xn - x0) / (values - 1);

			for (int i = 1; i < values; ++i) {
				ResT resI = results[i] = func(x0 + i * step);
				if (results[i] < minRes) minRes = results[i];
				else if (maxRes < results[i]) maxRes = results[i];
			}

			auto const ampl = maxRes - minRes;
			float const xScale = getWidth() / values;
			float const yScale = (getHeight() - 1) / ampl;

			for (int i = 0; i < values - 1; ++i) {
				int x0 = std::round(i * xScale);
				int y0 = getHeight() - 1 - std::round((results[i] - minRes) * yScale);
				int xn = std::round((i + 1) * xScale);
				int yn = getHeight() - 1 - std::round((results[(i + 1)] - minRes) * yScale);
				drawLine(x0, xn, y0, yn, color);
			}
		}

	};

}

#endif // !_BICYCLE_XYPLOT_H_