#include "base.h"

void memcpy(char *, const char *, int);
void memset(char *, int, int);

#define XN		32
#define YN		24
#define CSIZE	0x300
#define GSIZE	0x1800

static u8 cursX, cursY, cursActive, printer;

static void writeChar(u8 c) {
	CBASE[(cursY << 5) + cursX] = c;
}

static void cursDraw() {
	if (cursActive) writeChar(0xf9);
}

static void cursErase() {
	if (cursActive) writeChar(0);
}

static void newLine() {
	cursErase();
	cursX = 0;
	if (cursY < YN - 1) cursY++;
	else {
		memcpy/*memmove*/(CBASE, CBASE + XN, XN * (YN - 1));
		memset(CBASE + XN * (YN - 1), 0, XN);
	}
	cursDraw();
}

void putchar(u8 c) {
	if (printer) PRINTER = c;
	else if (c == '\n') newLine();
	else {
		writeChar(c);
		if (++cursX >= XN) newLine();
		else cursDraw();
	}
}

void puts_n(const u8 *s) {
	u8 *p = (u8 *)s;
	while (*p) putchar(*p++);
}

void puts(const u8 *s) {
	puts_n(s);
	putchar('\n');
}

void printf(const u8 *format, ...) {
	u16 *ap = (u16 *)&format;
	u8 *p = (u8 *)format;
#ifdef __chibicc__
	ap += 2;
#endif
#ifdef STDARG_REV
#define next_ap	(--ap)
#else
#define next_ap	(++ap)
#endif
	while (*p) {
		u8 c[7];
		u8 a, b, f, i, l, n, r, t, u;
		u16 v;
		u8 *q;
		switch (t = *p++) {
		case '%':
			b = ' ';
			l = n = 0;
			switch (t = *p++) {
				case '%': putchar('%'); continue;
				case '0': b = '0'; t = *p++; break;
				case '-': l = 1; t = *p++; break;
				case 'c': putchar(*next_ap); continue;
			}
			if (t >= '1' && t <= '9')
				for (n = t - '0'; (t = *p++) >= '0' && t <= '9';)
					n = 10 * n + t - '0';
			if (t == 's')
				for (q = *(u8 **)next_ap, i = 0; q[i]; i++)
					;
			else {
				if (u = t == 'u') t = *p++;
				switch (t) {
					case 'd': r = 10; break;
					case 'o': r = 8; u = 1; break;
					case 'x': r = 16; u = 1; a = 0x27; break;
					case 'X': r = 16; u = 1; a = 7; break;
					default: continue;
				}
				v = *next_ap;
				if (f = !u && (s16)v < 0) v = -v;
				c[i = sizeof(c) - 1] = 0;
				do {
					u16 d = v / r, t1 = v - d * r + '0';
					c[--i] = t1 > '9' ? t1 + a : t1;
					v = d;
				} while (v);
				if (f) c[--i] = '-';
				q = &c[i];
			}
			i = sizeof(c) - 1 - i;
			if (l) n -= i;
			else while (n-- > i) putchar(b);
			while (*q) putchar(*q++);
			if (l) while (n--) putchar(' ');
			break;
		default:
			putchar(t);
			break;
		}
	}
#undef next_ap
}

void cls(u8 graph) {
	if (graph) {
		memset(GBASE, 0, GSIZE);
		DISP = 0xc0;
	}
	else {
		cursX = cursY = 0;
		memset(CBASE, 0, CSIZE);
		DISP = 0;
	}
}

void locate(u8 x, u8 y) {
	cursErase();
	cursX = x < XN - 1 ? x : XN - 1;
	cursY = y < YN - 1 ? y : YN - 1;
	cursDraw();
}

void cursorOn(void) {
	u8 l = cursActive;
	cursActive = 1;
	if (!l) cursDraw();
}

void cursorOff(void) {
	if (cursActive) cursErase();
	cursActive = 0;
}

void setPrinter(u8 f) {
	printer = f;
}

#define PORTN	13
#define REP1	10
#define REP2	1

u8 keyDown(u8 repeat) {
	static const u8 nom[] = "zaq1xsw2cde3vfr4bgt5nhy6mju7,ki8.lo9/;p0 '\\- ][= \n\b";
	static const u8 sft[] = "ZAQ!XSW@CDE#VFR$BGT%NHY^MJU&<KI*>LO(?:P) \"|_ }{+ \n\b";
	static u8 last[PORTN], cur[PORTN];
	static u8 port, mask, repport, repmask, repsft, rep;
	u8 i, d = 0;
	if (!(cur[repport] & repmask)) repmask = 0;
	for (i = 0; i < 4 * PORTN && !d; i++) {
		if (!mask) {
			KEY = port;
			last[port] = cur[port];
			cur[port] = ~KEY;
			mask = 1;
		}
		if (cur[port] & mask) {
			s8 f = !(last[port] & mask) ? -1 :
				repeat && port == repport && mask == repmask && !--rep;
			if (f) {
				u8 m = mask;
				if (f == -1) repsft = cur[port] & 0xa0;
				for (i = port << 2; !(m & 1); i++, m >>= 1)
					;
				d = (repsft ? sft : nom)[i];
				if (f == -1) {
					repport = port;
					repmask = mask;
					rep = REP1;
				}
				else rep = REP2;
			}
		}
		mask <<= 1;
		if (mask & 0x10) {
			mask = 0;
			port++;
			if (port >= PORTN) port = 0;
		}
	}
	return d;
}
