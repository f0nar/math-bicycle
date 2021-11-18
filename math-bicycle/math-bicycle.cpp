#include <string>
#include <iostream>
#include "src/Vector.h"
#include "src/utils/Image.h"

bool imageWriteTest() {
	std::string const image_file = "write_image_test_10x10.jpg";
	bm::Image<unsigned char> image_10x10(10, 10);
	for (int i = 0; i < 10; i += 2) {
		for (int j = 1; j < 10; j += 2) {
			auto& pixel = image_10x10.getPixel(i, j);
			pixel.x = pixel.y = pixel.z = 255;
		}
	}
	return image_10x10.save(image_file);
}

bool imageReadTest() {
	std::string const read_image_file = "write_image_test_10x10.jpg";
	std::string const write_image_file = "read_image_test_10x10.jpg";
	bm::Image<unsigned char> image_10x10(read_image_file);
	return image_10x10.save(write_image_file);
}

int main() {
	return imageWriteTest() && imageReadTest();
}
