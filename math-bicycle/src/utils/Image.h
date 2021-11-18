
#include <string>
#include <cassert>
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

		Image (int w, int h) : m_width(w), m_height(h), m_pixels(new Vector<3, T>[w*h]) { }

		Image (std::string const& path, bool *status = nullptr) : m_pixels(nullptr) {
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
			if (status) {
				*status = m_pixels != nullptr;
			}
		}

		int getWidth() {
			return m_width;
		}

		int getHeight() {
			return m_height;
		}

		Vector<3, T> &getPixel(int x, int y) {
			assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
			return m_pixels[y * m_width + x];
		}

		Vector<3, T> const &getPixel(int x, int y) const {
			assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
			return m_pixels[h * m_width + x];
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