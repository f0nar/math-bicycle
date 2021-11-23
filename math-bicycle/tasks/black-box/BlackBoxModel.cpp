#include "BlackBox.h"
#include "BlackBoxModel.h"

int main() {
	BlackBox<3> bb;
	BlackBoxModel<3> bbm(bb);

	RandomInBlackBox<3> rbb;
	BlackBoxModel<3> rbbm(rbb);
}