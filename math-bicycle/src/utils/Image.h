
#include <string>
#include <cassert>
#include "../Vector.h"
#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../third/stb_image_write.h"

namespace bm {

	template <int Width, int Height, typename T>
	class Image {
	public:

		int getWidth() {
			return Width;
		}

		int getHeight() {
			return Height;
		}

		Vector<3, T> &getPixel(int x, int y) {
			assert(x >= 0 && x < Width && y >= 0 && y < Height);
			return m_pixels[y * Width + x];
		}

		Vector<3, T> const &getPixel(int x, int y) const {
			assert(x >= 0 && x < Width&& y >= 0 && y < Height);
			return m_pixels[h * Width + x];
		}

		bool save(std::string const &fileName) {
			int const pixels_number = Width * Height;
			unsigned char image_array[Width * Height * 3];
			for (int i = 0; i < pixels_number; ++i) {
				int const index_to_write = i * 3;
				Vector<3, T> const& pixel = m_pixels[i];
				image_array[index_to_write]     = static_cast<unsigned char>(pixel.x);
				image_array[index_to_write + 1] = static_cast<unsigned char>(pixel.y);
				image_array[index_to_write + 2] = static_cast<unsigned char>(pixel.z);
			}
			return stbi_write_jpg(fileName.c_str(), Width, Height, 3, image_array, 95);
		}

	private:
		Vector<3, T> m_pixels[Width * Height];
	};

}