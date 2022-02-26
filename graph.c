#include "graph.h"

static u8 gattr;

void setGAttr(u8 a) {
	gattr = a ? 0xff : 0;
}

void point(u8 x, u8 y) {
	static const u8 m[] = { 0x80, 0x40, 0x20, 0x10, 8, 4, 2, 1 };
	if (y < 192) {
		u8 *p = GBASE + (y << 5) + (x >> 3);
		if (gattr) *p |= m[x & 7];
		else *p &= ~m[x & 7];
	}
}

void line(u8 x0, u8 y0, u8 x1, u8 y1) {
	u8 x = x0, y = y0, dx, dy;
	s8 sx, sy;
	s16 d;
	if (x0 < x1) {
		dx = x1 - x0;
		sx = 1;
	}
	else {
		dx = x0 - x1;
		sx = -1;
	}
	if (y0 < y1) {
		dy = y1 - y0;
		sy = 1;
	}
	else {
		dy = y0 - y1;
		sy = -1;
	}
	if (dx > dy) {
		d = dx >> 1;
		while (1) {
			point(x, y);
			d += dy;
			if (d >= dx) {
				d -= dx;
				y += sy;
			}
			if (x == x1) return;
			x += sx;
		}
	}
	else {
		d = dy >> 1;
		while (1) {
			point(x, y);
			d += dx;
			if (d >= dy) {
				d -= dy;
				x += sx;
			}
			if (y == y1) return;
			y += sy;
		}
	}
}

void boxfill(u8 x0, u8 y0, u8 x1, u8 y1) {
	static const u8 ml[] = { 0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01 };
	static const u8 mr[] = { 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff };
	u8 m0, m1, o0, o1, y;
	if (x0 > x1) {
		u8 t = x0;
		x0 = x1;
		x1 = t;
	}
	if (y0 > y1) {
		u8 t = y0;
		y0 = y1;
		y1 = t;
	}
	if (y0 > 191) y0 = 191;
	if (y1 > 191) y1 = 191;
	m0 = ml[x0 & 7];
	m1 = mr[x1 & 7];
	o0 = x0 >> 3;
	o1 = x1 >> 3;
	for (y = y0; y <= y1; y++) {
		u8 *p = GBASE + (y << 5) + o0;
		u8 d, m, o = o0;
		if (o < o1) {
			d = *p & ~m0;
			if (gattr) d |= m0;
			*p++ = d;
			while (++o < o1) *p++ = gattr;
			m = m1;
		}
		else m = m0 & m1;
		d = *p & ~m;
		if (gattr) d |= m;
		*p = d;
	}
}

void box(u8 x0, u8 y0, u8 x1, u8 y1) {
	boxfill(x0, y0, x1, y0);
	boxfill(x0, y1, x1, y1);
	boxfill(x0, y0, x0, y1);
	boxfill(x1, y0, x1, y1);
}

void circle(u8 x0, u8 y0, u8 r) {
	u8 x, y;
	s16 f;
	if (!r) return;
	x = r;
	y = 0;
	f = (-r << 1) + 3;
	while (x >= y) {
		point(x0 + x, y0 + y);
		point(x0 - x, y0 + y);
		point(x0 + x, y0 - y);
		point(x0 - x, y0 - y);
		point(x0 + y, y0 + x);
		point(x0 - y, y0 + x);
		point(x0 + y, y0 - x);
		point(x0 - y, y0 - x);
		if (f >= 0) {
			x--;
			f -= x << 2;
		}
		y++;
		f += (y << 2) + 2;
	}
}
