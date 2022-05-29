cmake_minimum_required (VERSION 3.8)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable (
	"mathbicycle"
	"math-bicycle.cpp"
	"src/Matrix.h"
	"src/Vector.h"
	"src/Color.h"
	"src/PolynomicFunction.h"
	"src/utils/Image.h"
	"src/utils/XYPlot.h"
	"third/stb_image_write.h"
	"third/stb_image.h"
	"src/utils/Fonts.h"
	"src/Point.h"
	"src/RationalFunction.h"
 "src/ExpotentialFunction.h")

add_executable(
  "math_bicycle_test"
  "tests/Matrix_test.cc"
  "tests/Vector_test.cc"
  "tests/PolynomicFunction_test.cc"
  "tests/RationalFunction_test.cc"
 "src/ExpotentialFunction.h")

target_link_libraries(
  "math_bicycle_test"
  "gtest_main"
)

add_subdirectory ("tasks")

include(GoogleTest)
gtest_discover_tests(math_bicycle_test)
