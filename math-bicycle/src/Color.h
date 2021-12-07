#ifndef _BICYCLE_COLOR_H_
#define _BICYCLE_COLOR_H_

#include "Vector.h"

namespace bm {

	template <typename T = unsigned char>
	using ColorRGB_ = Vector<3, T>;

	using ColorRGB = ColorRGB_<unsigned char>;


};

#endif // !_BICYCLE_COLOR_H_