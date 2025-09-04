#include <assert.h>
#include "citrus.h"

int main() {
	for (int i = 0; i < 100; i++)
		assert(f(i) == i);
	return 0;
}
