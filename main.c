#include "base.h"
#include "graph.h"
#include "util.h"

static u8 r192() {
	return 3 * (rand() & 0x3fff) >> 8;
}

int main() {
	u8 i = 0, c = 0;
	srand(12345);
#if 1
	while (1) {
		cls(0);
		for (i = 0; i < 10; i += !c) {
			putchar(c != '\n' ? c : ' ');
			c++;
		}
		cls(1);
		for (i = 0; i < 10; i++) {
			setGAttr(c++ & 1);
			line(rand() & 0xff, r192(), rand() & 0xff, r192());
			box(rand() & 0xff, r192(), rand() & 0xff, r192());
			boxfill(rand() & 0xff, r192(), rand() & 0xff, r192());
			circle(rand() & 0xff, r192(), rand() & 0x3f);
		}
	}
#else
	cls(0);
	cursorOn();
	puts("KEYBOARD TEST");
	while (1) {
		u8 c = keyDown(1);
		if (c) printf("\'%c\' 0x%02x %d\n", (int)c, (int)c, (int)c);
	}
#endif
	return 0;
}
