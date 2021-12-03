#ifndef _BICYCLE_IMAGE_H_
#define _BICYCLE_IMAGE_H_

#include <string>
#include <cassert>
#include <algorithm>
#include <iostream>

#include "../Vector.h"

#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../third/stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../third/stb_image.h"


namespace bm {

	template <typename T>
	class Image {
	public:

		Image(int w, int h) : m_width(w), m_height(h), m_pixels(new Vector<3, T>[w * h]) {}

		Image (int w, int h, T r, T g, T b) : Image(w, h) {
			if (isValid()) fill(r, g, b);
		}

		Image (std::string const& path) : m_pixels(nullptr) {
			unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, nullptr, 3);
			if (data) {
				m_pixels = new Vector<3, T>[m_width * m_height];
				int const pixels_number = m_width * m_height;
				for (int i = 0; i < pixels_number; ++i) {
					int const index_to_read = i * 3;
					Vector<3, T>& pixel = m_pixels[i];
					pixel.x = static_cast<unsigned char>(data[index_to_read]);
					pixel.y = static_cast<unsigned char>(data[index_to_read + 1]);
					pixel.z = static_cast<unsigned char>(data[index_to_read + 2]);
				}
				free(data);
			}
		}

		int getWidth() {
			return m_width;
		}

		int getHeight() {
			return m_height;
		}

		bool isValid() {
			return m_pixels != nullptr;
		}

		Vector<3, T> &getPixel(int x, int y) {
			assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
			return m_pixels[y * m_width + x];
		}

		Vector<3, T> const &getPixel(int x, int y) const {
			assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
			return m_pixels[h * m_width + x];
		}

		void drawPixel(int x, int y, T r, T g, T b) {
			auto &pixel = getPixel(x, y);
			pixel.x = r;
			pixel.y = g;
			pixel.z = b;
		}

		void drawLine(int x0, int xn, int y0, int yn, T r, T g, T b) {
			int dx = abs(xn - x0);
			int dy = abs(yn - y0);
			int err = 0;
			if (dy <= dx) {
				int yDir = yn >= y0 ? 1 : -1;
				int dErr = dy + 1;
				int y = y0;
				if (xn < x0) std::swap(x0, xn);
				for (int x = x0; x <= xn; ++x) {
					drawPixel(x, y, r, g, b);
					err += dErr;
					if (err >= dx + 1) {
						y += yDir;
						err -= dx + 1;
					}
				}
			} else {
				int xDir = xn >= x0 ? 1 : -1;
				int dErr = dx + 1;
				int x = x0;
				if (yn < y0) std::swap(y0, yn);
				for (int y = y0; y < yn; ++y) {
					drawPixel(x, y, r, g, b);
					err += dErr;
					if (err >= dy + 1) {
						x += xDir;
						err -= dy + 1;
					}
				}
			}
		}

		void drawGrid(int x0, int xStep, int xWidth, int y0, int yStep, int yWidth, T r, T g, T b) {
			for (int x = x0; x < m_width; x += xStep + xWidth) {
				for (int y = 0; y < m_height; ++y) {
					for (
						int x0 = x, xn = std::min(x + xWidth, m_width);
						x0 < xn;
						++x0)
					{
						auto& pixel = m_pixels[y * m_width + x0];
						pixel[0] = r;
						pixel[1] = g;
						pixel[2] = b;
					}
				}
			}
			for (int y = y0; y < m_height; y += yStep + yWidth) {
				for (int x = 0; x < m_width; ++x) {
					for (
						int y0 = y, yn = std::min(y + yWidth, m_height); y0 < yn; ++y0) {
						auto& pixel = m_pixels[y0 * m_width + x];
						pixel[0] = r;
						pixel[1] = g;
						pixel[2] = b;
					}
				}
			}
		}

		void fill(T r, T g, T b) {
			for (int i = 0; i < m_width; ++i) {
				for (int j = 0; j < m_height; ++j) {
					auto& pixel = m_pixels[j * m_width + i];
					pixel[0] = r;
					pixel[1] = g;
					pixel[2] = b;
				}
			}
		}

		int save(std::string const &fileName) {
			int const pixels_number = m_width * m_height;
			unsigned char *image_array = new unsigned char [m_width * m_height * 3];
			for (int i = 0; i < pixels_number; ++i) {
				int const index_to_write = i * 3;
				Vector<3, T> const& pixel = m_pixels[i];
				image_array[index_to_write]     = static_cast<unsigned char>(pixel.x);
				image_array[index_to_write + 1] = static_cast<unsigned char>(pixel.y);
				image_array[index_to_write + 2] = static_cast<unsigned char>(pixel.z);
			}
			int const write_res =  stbi_write_jpg(fileName.c_str(), m_width, m_height, 3, image_array, 95);
			delete[] image_array;
			return write_res;
		}

		~Image() {
			delete[] m_pixels;
		}

	private:

		int m_width;
		int m_height;
		Vector<3, T> *m_pixels;
	};

}

#endif // !_BICYCLE_IMAGE_H_