/*
 *----------------------------------------------------------------------
 *    T2EX Software Package
 *
 *    Copyright 2012 by Ken Sakamura.
 *    This software is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2012/12/12.
 *
 *----------------------------------------------------------------------
 */
/*
 * This software package is available for use, modification, 
 * and redistribution in accordance with the terms of the attached 
 * T-License 2.0.
 * If you want to redistribute the source code, you need to attach 
 * the T-License 2.0 document.
 * There's no obligation to publish the content, and no obligation 
 * to disclose it to the T-Engine Forum if you have modified the 
 * software package.
 * You can also distribute the modified source code. In this case, 
 * please register the modification to T-Kernel traceability service.
 * People can know the history of modifications by the service, 
 * and can be sure that the version you have inherited some 
 * modification of a particular version or not.
 *
 *    http://trace.t-engine.org/tk/?lang=en
 *    http://trace.t-engine.org/tk/?lang=ja
 *
 * As per the provisions of the T-License 2.0, T-Engine Forum 
 * ensures that the the portion of the software that is copyrighted 
 * by Ken Sakamura or the T-Engine Forum does not infringe the 
 * copyrights of a third party.
 * However, it does not make any warranty other than this.
 * DISCLAIMER: T-Engine Forum and Ken Sakamura shall not be held
 * responsible for any consequences or damages caused directly or
 * indirectly by the use of this software package.
 *
 * Materials copyrighted by third parties are included in the 
 * software package. Please use them according to the individual 
 * copyright notice and license for these parts.
 */

/*
 *	@(#)fimp_fat_enc.c
 *
 */

#include "fimp_fat_enc.h"

/*
 *  ASCII lower case -> upper case conversion table
 */
EXPORT	const	UB	fatEncAsciiLowToUpTable[] = {
	/* 00-07: NUL SOH STX ETX EOT ENQ ACK BEL   */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	/* 08-0f: BS  HT  LF  VT  FF  CR  SO  SI    */
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	/* 10-17: DLE DC1 DC2 DC3 DC4 NAK SYN ETB   */
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	/* 18-1f: CAN EM  SUB ESC FS  GS  RS  US    */
	0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
	/* 20-27: Spa !   "   #   $   %   &   '     */
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
	/* 28-2f: (   )   *   +   ,   -   .   /     */
	0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
	/* 30-37: 0   1   2   3   4   5   6   7     */
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
	/* 38-3f: 8   9   :   ;   <   =   >   ?     */
	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
	/* 40-47: @   A   B   C   D   E   F   G     */
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
	/* 48-4f: H   I   J   K   L   M   N   O     */
	0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
	/* 50-57: P   Q   R   S   T   U   V   W     */
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
	/* 58-5f: X   Y   Z   [   \   ]   ^   _     */
	0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
	/* 60-67: `   a   b   c   d   e   f   g     */
	0x60, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
	/* 68-6f: h   i   j   k   l   m   n   o     */
	0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
	/* 70-77: p   q   r   s   t   u   v   w     */
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
	/* 78-7f: x   y   z   {   |   }   ~   DEL   */
	0x58, 0x59, 0x5a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,

	/* for fat_strncasecmp() */
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
	0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
	0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
	0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
	0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
	0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
	0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
	0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
	0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
};

/*
 *  ASCII upper case -> lower case conversion table
 */
EXPORT	const	UB	fatEncAsciiUpToLowTable[ASCII_MAX + 1] = {
	/* 00-07: NUL SOH STX ETX EOT ENQ ACK BEL   */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	/* 08-0f: BS  HT  LF  VT  FF  CR  SO  SI    */
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	/* 10-17: DLE DC1 DC2 DC3 DC4 NAK SYN ETB   */
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	/* 18-1f: CAN EM  SUB ESC FS  GS  RS  US    */
	0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
	/* 20-27: Spa !   "   #   $   %   &   '     */
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
	/* 28-2f: (   )   *   +   ,   -   .   /     */
	0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
	/* 30-37: 0   1   2   3   4   5   6   7     */
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
	/* 38-3f: 8   9   :   ;   <   =   >   ?     */
	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
	/* 40-47: @   A   B   C   D   E   F   G     */
	0x40, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
	/* 48-4f: H   I   J   K   L   M   N   O     */
	0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
	/* 50-57: P   Q   R   S   T   U   V   W     */
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
	/* 58-5f: X   Y   Z   [   \   ]   ^   _     */
	0x78, 0x79, 0x7a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
	/* 60-67: `   a   b   c   d   e   f   g     */
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
	/* 68-6f: h   i   j   k   l   m   n   o     */
	0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
	/* 70-77: p   q   r   s   t   u   v   w     */
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
	/* 78-7f: x   y   z   {   |   }   ~   DEL   */
	0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
};

/*
 *  ASCII character kind table
 */
#define LOW	ASCII_LOW
#define UP	ASCII_UP
#define NUM	ASCII_NUM
#define SYM	ASCII_SYM
#define CON	ASCII_CON
EXPORT	const	UB	fatEncAsciiIsLowTable[ASCII_MAX + 1] = {
	/* 00-07: NUL SOH STX ETX EOT ENQ ACK BEL   */
	CON, CON, CON, CON, CON, CON, CON, CON,
	/* 08-0f: BS  HT  LF  VT  FF  CR  SO  SI    */
	CON, CON, CON, CON, CON, CON, CON, CON,
	/* 10-17: DLE DC1 DC2 DC3 DC4 NAK SYN ETB   */
	CON, CON, CON, CON, CON, CON, CON, CON,
	/* 18-1f: CAN EM  SUB ESC FS  GS  RS  US    */
	CON, CON, CON, CON, CON, CON, CON, CON,
	/* 20-27: Spa !   "   #   $   %   &   '     */
	SYM, SYM, SYM, SYM, SYM, SYM, SYM, SYM,
	/* 28-2f: (   )   *   +   ,   -   .   /     */
	SYM, SYM, SYM, SYM, SYM, SYM, SYM, SYM,
	/* 30-37: 0   1   2   3   4   5   6   7     */
	NUM, NUM, NUM, NUM, NUM, NUM, NUM, NUM,
	/* 38-3f: 8   9   :   ;   <   =   >   ?     */
	NUM, NUM, SYM, SYM, SYM, SYM, SYM, SYM,
	/* 40-47: @   A   B   C   D   E   F   G     */
	SYM,  UP,  UP,	UP,  UP,  UP,  UP,  UP,
	/* 48-4f: H   I   J   K   L   M   N   O     */
	UP,   UP,  UP,	UP,  UP,  UP,  UP,  UP,
	/* 50-57: P   Q   R   S   T   U   V   W     */
	UP,   UP,  UP,	UP,  UP,  UP,  UP,  UP,
	/* 58-5f: X   Y   Z   [   \   ]   ^   _     */
	UP,   UP,  UP, SYM, SYM, SYM, SYM, SYM,
	/* 60-67: `   a   b   c   d   e   f   g     */
	SYM, LOW, LOW, LOW, LOW, LOW, LOW, LOW,
	/* 68-6f: h   i   j   k   l   m   n   o     */
	LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,
	/* 70-77: p   q   r   s   t   u   v   w     */
	LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,
	/* 78-7f: x   y   z   {   |   }   ~   DEL   */
	LOW, LOW, LOW, SYM, SYM, SYM, SYM, CON,
};

/*
 *  ASCII allowed character table
 */
#define DENY	(0x00)
#define SHRT	(0x01)		/* Allowed by Short name */
#define LONG	(0x02)		/* Allowed by Long name  */
#define BOTH	(0x03)
EXPORT	const	UB	fatEncAsciiAllowTable[ASCII_MAX + 1] = {
	/* 00-07: NUL SOH STX ETX EOT ENQ ACK BEL   */
	DENY, DENY, DENY, DENY, DENY, DENY, DENY, DENY,
	/* 08-0f: BS  HT  LF  VT  FF  CR  SO  SI    */
	DENY, DENY, DENY, DENY, DENY, DENY, DENY, DENY,
	/* 10-17: DLE DC1 DC2 DC3 DC4 NAK SYN ETB   */
	DENY, DENY, DENY, DENY, DENY, DENY, DENY, DENY,
	/* 18-1f: CAN EM  SUB ESC FS  GS  RS  US    */
	DENY, DENY, DENY, DENY, DENY, DENY, DENY, DENY,
	/* 20-27: Spa !   "   #   $   %   &   '     */
	LONG, BOTH, DENY, BOTH, BOTH, BOTH, BOTH, BOTH,
	/* 28-2f: (   )   *   +   ,   -   .   /     */
	BOTH, BOTH, DENY, LONG, LONG, BOTH, LONG, DENY,
	/* 30-37: DENY1   2   3   4   5   6   7     */
	BOTH, BOTH, BOTH, BOTH, BOTH, BOTH, BOTH, BOTH,
	/* 38-3f: 8   9   :   ;   <   =   >   ?     */
	BOTH, BOTH, DENY, LONG, DENY, LONG, DENY, DENY,
	/* 40-47: @   A   B   C   D   E   F   G     */
	BOTH, BOTH, BOTH, BOTH, BOTH, BOTH, BOTH, BOTH,
	/* 48-4f: H   I   J   K   L   M   N   O     */
	BOTH, BOTH, BOTH, BOTH, BOTH, BOTH, BOTH, BOTH,
	/* 50-57: P   Q   R   S   T   U   V   W     */
	BOTH, BOTH, BOTH, BOTH, BOTH, BOTH, BOTH, BOTH,
	/* 58-5f: X   Y   Z   [   \   ]   ^   _     */
	BOTH, BOTH, BOTH, LONG, DENY, LONG, BOTH, BOTH,
	/* 60-67: `   a   b   c   d   e   f   g     */
	BOTH, BOTH, BOTH, BOTH, BOTH, BOTH, BOTH, BOTH,
	/* 68-6f: h   i   j   k   l   m   n   o     */
	BOTH, BOTH, BOTH, BOTH, BOTH, BOTH, BOTH, BOTH,
	/* 70-77: p   q   r   s   t   u   v   w     */
	BOTH, BOTH, BOTH, BOTH, BOTH, BOTH, BOTH, BOTH,
	/* 78-7f: x   y   z   {   |   }   ~   DEL   */
	BOTH, BOTH, BOTH, BOTH, DENY, BOTH, BOTH, DENY,
	/* 78-7f: x   y   z   {   |   }   ~   DEL   */
};

/*
 *  Compare two strings ignoring different of capital letter.
 */
EXPORT	W	fat_strncasecmp(const UB *str1, const UB *str2, UW n)
{
	UB	c1, c2;

	if (n != 0) {
		do {
			c1 = fatEncAsciiLowToUpTable[*str1++];
			c2 = fatEncAsciiLowToUpTable[*str2++];
			if (c1 != c2) return (W)c1 - (W)c2;
			if (c1 == '\0') break;
		} while (--n != 0);
	}
	return 0;
}

/*
 *  UTF16 string length
 */
EXPORT	W	fat_strlen16(const UH *utf16str)
{
	W	len;

	for (len = 0; utf16str[len] != (UH)'\0'; len++);
	return len;
}

/*
 *  Convert Utf8 -> Unicode : returns the number of encoded bytes.
 */
EXPORT	W	fatEncUtf8ToUnicode(const UB utf8str[], UW *unicode)
{
#ifdef	FAT_ASCII_FN_ONLY

	if (utf8str[0] < 0x80) {
		*unicode = (UW)utf8str[0];
		return 1;
	}
	*unicode = 0;
	return 0;

#else	/* FAT_ASCII_FN_ONLY */

	UB	c0, c1, c2, c3, c4, c5;
	UW	uc;
	W	len;

	len = 0;
	uc = 0;

	/* 0xxx xxxx : 0 - 0x7f */
	c0 = utf8str[0];
	if ((c0 & 0x80) == 0x00) {
		uc = (UW)c0;
		len = 1;
		goto exit0;
	}

	/* 110yyy yx, 10xx xxxx : 0x80 - 0x7ff */
	if ((c1 = utf8str[1] ^ 0x80) > 0x3f) goto exit0;
	if ((c0 & 0xe0) == 0xc0) {
		uc = (((UW)c0 & 0x1f) << 6) | (UW)c1;
		if (uc > 0x7f) len = 2;
		goto exit0;
	}

	/* 1110yyyy, 10yxxx xx, 10xx xxxx : 0x800 - 0xffff */
	if ((c2 = utf8str[2] ^ 0x80) > 0x3f) goto exit0;
	if ((c0 & 0xf0) == 0xe0) {
		uc = (((UW)c0 & 0x0f) << 12) | ((UW)c1 << 6) | (UW)c2;
		if (uc > 0x7ff) len = 3;
		goto exit0;
	}
	/* 11110y yy, 10yy xxxx, 10xxxx xx, 10xx xxxx : 0x10000 - 0x1fffff */
	if ((c3 = utf8str[3] ^ 0x80) > 0x3f) goto exit0;
	if ((c0 & 0xf8) == 0xf0) {
		uc = (((UW)c0 & 0x07) << 18) | ((UW)c1 << 12) | ((UW)c2 << 6)
			| (UW)c3;
		if (uc > 0xffff) len = 4;
		goto exit0;
	}
	/* 111110yy, 10yyyx xx, 10xx xxxx, 10xxxx xx, 10xx xxxx :
		0x200000 - 0x3ffffff */
	if ((c4 = utf8str[4] ^ 0x80) > 0x3f) goto exit0;
	if ((c0 & 0xfc) == 0xf8) {
		uc = (((UW)c0 & 0x03) << 24) | ((UW)c1 << 18) | ((UW)c2 << 12)
			| ((UW)c3 << 6) | (UW)c4;
		if (uc > 0x1fffff) len = 5;
		goto exit0;
	}
	/* 1111110y 10yy yyxx 10xxxx xx 10xx xxxx 10xxxx xx 10xx xxxx :
		0x4000000 - 0x7fffffff */
	if ((c5 = utf8str[5] ^ 0x80) > 0x3f) goto exit0;
	if ((c0 & 0xfe) == 0xfc) {
		uc = (((UW)c0 & 0x01) << 30) | ((UW)c1 << 24) | ((UW)c2 << 18)
			| ((UW)c3 << 12) | ((UW)c4 << 6) | (UW)c5;
		if (uc > 0x3ffffff) len = 6;
		goto exit0;
	}
exit0:
	*unicode = (len == 0) ? 0 : uc;
	return len;
#endif	/* FAT_ASCII_FN_ONLY */
}

/*
 *  Convert Unicode -> Utf8
 */
EXPORT	W	fatEncUnicodeToUtf8(UW unicode, UB utf8a[UTF8_MAX])
{
#ifdef	FAT_ASCII_FN_ONLY

	utf8a[0] = (unicode <= 0x7f) ? (UB)unicode : '_';
	return 1;

#else	/* FAT_ASCII_FN_ONLY */

	W	len;

	/* 0xxx xxxx */
	if (unicode <= 0x7f) {
		utf8a[0] = (UB)(unicode & 0x7f);
		len = 1;
	}
	/* 110yyy yx, 10xx xxxx */
	else if (unicode <= 0x7ff) {
		utf8a[0] = ((unicode >> 6) & 0x1f) | 0xc0;
		utf8a[1] = (unicode & 0x3f) | 0x80;
		len = 2;
	}
	/* 1110yyyy, 10yxxx xx, 10xx xxxx */
	else if (unicode <= 0xffff) {
		utf8a[0] = ((unicode >> 12) & 0x1f) | 0xe0;
		utf8a[1] = ((unicode >> 6) & 0x3f) | 0x80;
		utf8a[2] = (unicode & 0x3f) | 0x80;
		len = 3;
	}
	/* 11110y yy, 10yy xxxx, 10xxxx xx, 10xx xxxx */
	else if (unicode <= 0x1fffff) {
		utf8a[0] = ((unicode >> 18) & 0x07) | 0xf0;
		utf8a[1] = ((unicode >> 12) & 0x3f) | 0x80;
		utf8a[2] = ((unicode >> 6) & 0x3f) | 0x80;
		utf8a[3] = (unicode & 0x3f) | 0x80;
		len = 4;
	}
	/* 111110yy, 10yyyx xx, 10xx xxxx, 10xxxx xx, 10xx xxxx */
	else if (unicode <= 0x3ffffff) {
		utf8a[0] = ((unicode >> 24) & 0x03) | 0xf8;
		utf8a[1] = ((unicode >> 18) & 0x3f) | 0x80;
		utf8a[2] = ((unicode >> 12) & 0x3f) | 0x80;
		utf8a[3] = ((unicode >> 6) & 0x3f) | 0x80;
		utf8a[4] = (unicode & 0x3f) | 0x80;
		len = 5;
	}
	/* 1111110y 10yy yyxx 10xxxx xx 10xx xxxx 10xxxx xx 10xx xxxx */
	else if (unicode <= 0x7fffffff) {
		utf8a[0] = ((unicode >> 30) & 0x01) | 0xfc;
		utf8a[1] = ((unicode >> 24) & 0x3f) | 0x80;
		utf8a[2] = ((unicode >> 18) & 0x3f) | 0x80;
		utf8a[3] = ((unicode >> 12) & 0x3f) | 0x80;
		utf8a[4] = ((unicode >> 6) & 0x3f) | 0x80;
		utf8a[5] = (unicode & 0x3f) | 0x80;
		len = 6;
	} else {
		len = 0;
	}
	return len;

#endif	/* FAT_ASCII_FN_ONLY */
}

/*
 *  Convert Utf8 string to UTF16 string.
 *	If success, return length of the utf16 string (not byte).
 *	Else, return -1.
 *	utf8len is the length of utf8str, not include '\0'.
 *	utf16buflen is the byte number of the utf16str, include '\0'.
 */
EXPORT	W	fatEncUtf8strToUtf16str(const UB *utf8str,
			W utf8len, UH *utf16str, W utf16buflen, BOOL toUpper)
{
	W	i8, i16;
	UW	unicode;

	for (utf16buflen--, i8 = i16 = 0;
		i8 < utf8len && i16 < utf16buflen && utf8str[i8] != '\0'; ) {

		i8 += fatEncUtf8ToUnicode(&utf8str[i8], &unicode);
		if (UnicodeIsAscii(unicode)) {
			if (AsciiIsLFN(unicode)) {
				utf16str[i16++] = (toUpper == FALSE) ?
					unicode : AsciiToUP(unicode);
			} else {
				i16 = -1;
				break;
			}
#ifndef	FAT_ASCII_FN_ONLY
		} else if (UnicodeIsNormal(unicode)) {
			utf16str[i16++] = (UH)unicode;
		} else if (UnicodeIsSurrogate(unicode)) {
			utf16str[i16++] = UnicodeToUtf16UpSurrogate(unicode);
			utf16str[i16++] = UnicodeToUtf16LowSurrogate(unicode);
#endif	/* ~ FAT_ASCII_FN_ONLY */
		} else {
			i16 = -1;
			break;
		}
	}
	if (i16 >= 0) utf16str[i16] = (UH)'\0';
	return i16;
}

/*
 *  Convert UTF16 string to Utf8 string.
 */
EXPORT	W	fatEncUtf16strToUtf8str(const UH *utf16str,
				W utf16len, UB *utf8str, W utf8buflen)
{
	W	i8, i16;
	UB	utf8a[UTF8_MAX];

	for (i8 = i16 = 0; i16 < utf16len && i8 < utf8buflen
					&& utf16str[i16] != (UH)'\0'; ) {
#ifdef	FAT_ASCII_FN_ONLY
		if (fatEncUnicodeToUtf8(utf16str[i16++], utf8a) > 0 &&
				i8 < utf8buflen) {
			utf8str[i8++] = utf8a[0];
		}
#else	/* FAT_ASCII_FN_ONLY */
		W	i, len;
		UW	unicode;
		
		if (Utf16IsUpSurrogate(utf16str[i16])
			&& Utf16IsLowSurrogate(utf16str[i16 + 1])) {
			unicode = Utf16ToUnicode(
					utf16str[i16], utf16str[i16 + 1]);
			i16 += 2;
		} else {
			unicode = utf16str[i16++];
		}
		len = fatEncUnicodeToUtf8(unicode, utf8a);
		for (i = 0; i < len && i8 < utf8buflen; i++, i8++) {
			utf8str[i8] = utf8a[i];
		}
#endif	/* FAT_ASCII_FN_ONLY */
	}
	if (i8 < utf8buflen) {
		utf8str[i8] = '\0';
	} else {
		utf8str[i8 - 1] = '\0';
	}
	return i8;
}

