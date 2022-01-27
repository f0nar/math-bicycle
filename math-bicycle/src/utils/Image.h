#ifndef _BICYCLE_IMAGE_H_
#define _BICYCLE_IMAGE_H_

#include <algorithm>
#include <cassert>
#include <cmath>
#include <string>

#include "../Color.h"
#include "../Point.h"
#include "../Vector.h"
#include "./Fonts.h"

#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../third/stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../third/stb_image.h"


namespace bm {

	template <typename T>
	class Image {
	public:

		Image(int w, int h) : m_width(w), m_height(h), m_pixels(new ColorRGB_<T>[w * h]) {}

		Image (int w, int h, ColorRGB_<T> const& color) : Image(w, h) {
			if (isValid()) fill(color);
		}

		Image (std::string const& path) : m_pixels(nullptr) {
			unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, nullptr, 3);
			if (data) {
				m_pixels = new ColorRGB_<T>[m_width * m_height];
				int const pixels_number = m_width * m_height;
				for (int i = 0; i < pixels_number; ++i) {
					int const index_to_read = i * 3;
					ColorRGB_<T>& pixel = m_pixels[i];
					pixel.x = static_cast<unsigned char>(data[index_to_read]);
					pixel.y = static_cast<unsigned char>(data[index_to_read + 1]);
					pixel.z = static_cast<unsigned char>(data[index_to_read + 2]);
				}
				free(data);
			}
		}

		int getWidth() const {
			return m_width;
		}

		int getHeight() const {
			return m_height;
		}

		bool isValid() const {
			return m_pixels != nullptr;
		}

		ColorRGB_<T> &getPixel(int x, int y) {
			assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
			return m_pixels[y * m_width + x];
		}

		ColorRGB_<T>& getPixel(Point2i const &pos) {
			getPixel(pos.x, pos.y);
		}

		ColorRGB_<T> const &getPixel(int x, int y) const {
			assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
			return m_pixels[h * m_width + x];
		}


		ColorRGB_<T> const& getPixel(Point2i const& pos) const {
			getPixel(pos.x, pos.y);
		}

		void drawPixel(int x, int y, ColorRGB_<T> const& color) {
			getPixel(x, y) = color;
		}

		void drawLine(int x0, int xn, int y0, int yn, ColorRGB_<T> const& color) {
			const int deltaX = abs(xn - x0);
			const int deltaY = abs(yn - y0);
			const int signX = x0 < xn ? 1 : -1;
			const int signY = y0 < yn ? 1 : -1;
			int error = deltaX - deltaY;
			drawPixel(xn, yn, color);
			while (x0 != xn || y0 != yn)
			{
				drawPixel(x0, y0, color);
				int error2 = error * 2;
				if (error2 > -deltaY)
				{
					error -= deltaY;
					x0 += signX;
				}
				if (error2 < deltaX)
				{
					error += deltaX;
					y0 += signY;
				}
			}

		}

		void drawLine(Point2i const& from, Point2i const& to, ColorRGB_<T> const& color) {
			drawLine(from.x, to.x, from.y, to.y, color);
		}

		void drawHorizontalLine(int x0, int y0, int len, ColorRGB_<T> const& color) {
			for (int i = 0; i < len; ++i) { drawPixel(x0 + i, y0, color); }
		}

		void drawHorizontalLine(Point2i const& from, int len, ColorRGB_<T> const& color) {
			drawHorizontalLine(from.x, from.y, len, color);
		}

		void drawVerticalLine(int x0, int y0, int len, ColorRGB_<T> const& color) {
			for (int i = 0; i < len; ++i) { drawPixel(x0, y0 + i, color); }
		}

		void drawVerticalLine(Point2i const& from, int len, ColorRGB_<T> const& color) {
			drawVerticalLine(from.x, from.y, len, color);
		}

		void drawHorizontalGrid(int y0, int yStep, int yWidth, ColorRGB_<T> const& color) {
			for (int y = y0; y < m_height; y += yStep + yWidth) {
				for (int x = 0; x < m_width; ++x) {
					for (
						int y0 = y, yn = std::min(y + yWidth, m_height); y0 < yn; ++y0) {
						m_pixels[y0 * m_width + x] = color;
					}
				}
			}
		}

		void drawVerticalGrid(int x0, int xStep, int xWidth, ColorRGB_<T> const& color) {
			for (int x = x0; x < m_width; x += xStep + xWidth) {
				for (int y = 0; y < m_height; ++y) {
					for (
						int x0 = x, xn = std::min(x + xWidth, m_width);
						x0 < xn; ++x0)
					{
						m_pixels[y * m_width + x0] = color;
					}
				}
			}
		}

		void drawGrid(int start, int step, int width, ColorRGB_<T> const& color) {
			drawVerticalGrid(start, step, width, color);
			drawHorizontalGrid(start, step, width, color);
		}

		void drawGrid(int x0, int xStep, int xWidth, ColorRGB_<T> const& vColor, int y0, int yStep, int yWidth, ColorRGB_<T> const& hColor) {
			drawVerticalGrid(x0, xStep, xWidth, vColor);
			drawHorizontalGrid(y0, yStep, yWidth, hColor);
		}

		void drawKey(int x, int y, char key, ColorRGB_<T> const& color) {
			int set;
			char* bitmap = font8x8_basic[key];
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					set = bitmap[j] & 1 << i;
					if (set) { drawPixel(x + i, y + j, color); }
				}
			}
		}

		void drawKey(Point2i const& leftCornerPoint, char key, ColorRGB_<T> const& color) {
			drawKey(leftCornerPoint.x, leftCornerPoint.y, key, color);
		}

		void drawRectangle(int x0, int y0, int width, int height, ColorRGB_<T> const& borderColor, ColorRGB_<T> const& fillColor) {
			drawVerticalLine(x0, y0, height, borderColor);
			drawVerticalLine(x0 + width - 1, y0, height, borderColor);
			drawHorizontalLine(x0 + 1, y0, width - 2, borderColor);
			drawHorizontalLine(x0 + 1, y0 + height - 1, width - 2, borderColor);
			drawRectangle(x0 + 1, y0 + 1, width - 2, height - 2, fillColor);
		}

		void drawRectangle(Point2i const& leftCornerPoint, int width, int height, ColorRGB_<T> const& borderColor, ColorRGB_<T> const& fillColor) {
			drawRectangle(leftCornerPoint.x, leftCornerPoint.y, width, height, borderColor, fillColor);
		}

		void drawRectangle(int x0, int y0, int width, int height, ColorRGB_<T> const& fillColor) {
			for (int xi = x0, xn = x0 + width; xi < xn; ++xi) {
				drawVerticalLine(xi, y0, height, fillColor);
			}
		}

		void drawRectangle(Point2i const& leftCornerPoint, int width, int height, ColorRGB_<T> const& fillColor) {
			drawRectangle(leftCornerPoint.x, leftCornerPoint.y, width, height, fillColor);
		}
		
		void drawString(int x, int y, std::string const& str, ColorRGB_<T> const& color) {
			for (int i = 0; i < str.size(); ++i) {
				drawKey(x, y, str[i], color);
				x += 8;
			}
		}

		void drawString(Point2i const& leftCornerPoint, std::string const& str, ColorRGB_<T> const& color) {
			drawString(leftCornerPoint.x, leftCornerPoint.y, str, color);
		}

		void fill(ColorRGB_<T> const &color) {
			for (int i = 0; i < m_width; ++i) {
				for (int j = 0; j < m_height; ++j) {
					m_pixels[j * m_width + i] = color;
				}
			}
		}

		int save(std::string const &fileName) {
			int const pixels_number = m_width * m_height;
			unsigned char *image_array = new unsigned char [m_width * m_height * 3];
			for (int i = 0; i < pixels_number; ++i) {
				int const index_to_write = i * 3;
				ColorRGB_<T> const& pixel = m_pixels[i];
				image_array[index_to_write]     = static_cast<unsigned char>(pixel.x);
				image_array[index_to_write + 1] = static_cast<unsigned char>(pixel.y);
				image_array[index_to_write + 2] = static_cast<unsigned char>(pixel.z);
			}
			int const write_res =  stbi_write_jpg(fileName.c_str(), m_width, m_height, 3, image_array, 95);
			delete[] image_array;
			return write_res;
		}

		~Image() { delete[] m_pixels; }

	protected:

		int m_width;
		int m_height;
		ColorRGB_<T> *m_pixels;

	};

}

#endif // !_BICYCLE_IMAGE_H_