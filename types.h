#ifndef _TYPES_H_
#define _TYPES_H_

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

#define CBASE	((u8 *)0x100)
#define GBASE	((u8 *)0x900)
#define PRINTER	(*(u8 *)0xe800)
#define KEY		(*(u8 *)0xeec0)
#define DISP	(*(u8 *)0xefe0)

#endif
