#include <string>
#include "src/Vector.h"
#include "src/utils/Image.h"

bool imageTest() {
	std::string const image_file = "test_image_10x10.jpg";
	bm::Image<10, 10, unsigned char> image_10x10;
	for (int i = 0; i < 10; i += 2) {
		for (int j = 1; j < 10; j += 2) {
			auto& pixel = image_10x10.getPixel(i, j);
			pixel.x = pixel.y = pixel.z = 255;
		}
	}
	return image_10x10.save(image_file);
}

int main() {
	return imageTest();
}
