typedef unsigned int ucs4_t;
typedef unsigned int state_t;
typedef struct conv_struct * conv_t;

struct mbtowc_funcs
{
	int(*xxx_mbtowc) (conv_t conv, ucs4_t *pwc, unsigned char const *s, int n);
	int(*xxx_flushwc) (conv_t conv, ucs4_t *pwc);
};

#define RET_ILSEQ      -1
#define RET_TOOFEW(n)  (-2-(n))

struct wctomb_funcs
{
	int(*xxx_wctomb) (conv_t conv, unsigned char *r, ucs4_t wc, int n);
	int(*xxx_reset) (conv_t conv, unsigned char *r, int n);
};

#define RET_ILUNI      -1
#define RET_TOOSMALL   -2

struct conv_struct
{
	int iindex;
	struct mbtowc_funcs ifuncs;
	state_t istate;
	int oindex;
	struct wctomb_funcs ofuncs;
	int oflags;
	state_t ostate;
	int transliterate;
	int discard_ilseq;
};

/*
 * Include all the converters.
 */

#include "ascii.h"

 /* General multi-byte encodings */
#include "utf8.h"
#include "ucs2.h"
#include "ucs2be.h"
#include "ucs2le.h"
#include "ucs4.h"
#include "ucs4be.h"
#include "ucs4le.h"
#include "utf16.h"
#include "utf16be.h"
#include "utf16le.h"
#include "utf32.h"
#include "utf32be.h"
#include "utf32le.h"
#include "utf7.h"
#include "ucs2internal.h"
#include "ucs2swapped.h"
#include "ucs4internal.h"
#include "ucs4swapped.h"
#include "c99.h"
#include "java.h"

/* 8-bit encodings */
#include "iso8859_1.h"
#include "iso8859_2.h"
#include "iso8859_3.h"
#include "iso8859_4.h"
#include "iso8859_5.h"
#include "iso8859_6.h"
#include "iso8859_7.h"
#include "iso8859_8.h"
#include "iso8859_9.h"
#include "iso8859_10.h"
#include "iso8859_13.h"
#include "iso8859_14.h"
#include "iso8859_15.h"
#include "iso8859_16.h"
#include "koi8_r.h"
#include "koi8_u.h"
#include "koi8_ru.h"
#include "cp1250.h"
#include "cp1251.h"
#include "cp1252.h"
#include "cp1253.h"
#include "cp1254.h"
#include "cp1255.h"
#include "cp1256.h"
#include "cp1257.h"
#include "cp1258.h"
#include "cp850.h"
#include "cp862.h"
#include "cp866.h"
#include "mac_roman.h"
#include "mac_centraleurope.h"
#include "mac_iceland.h"
#include "mac_croatian.h"
#include "mac_romania.h"
#include "mac_cyrillic.h"
#include "mac_ukraine.h"
#include "mac_greek.h"
#include "mac_turkish.h"
#include "mac_hebrew.h"
#include "mac_arabic.h"
#include "mac_thai.h"
#include "hp_roman8.h"
#include "nextstep.h"
#include "armscii_8.h"
#include "georgian_academy.h"
#include "georgian_ps.h"
#include "koi8_t.h"
#include "mulelao.h"
#include "cp1133.h"
#include "tis620.h"
#include "cp874.h"
#include "viscii.h"
#include "tcvn.h"

/* CJK character sets [CCS = coded character set] [CJKV.INF chapter 3] */

typedef struct
{
	unsigned short indx; /* index into big table */
	unsigned short used; /* bitmask of used entries */
} Summary16;

#include "iso646_jp.h"
#include "jisx0201.h"
#include "jisx0208.h"
#include "jisx0212.h"

#include "iso646_cn.h"
#include "gb2312.h"
#include "isoir165.h"
/*#include "gb12345.h"*/
#include "gbk.h"
#include "cns11643.h"
#include "big5.h"

#include "ksc5601.h"
#include "johab_hangul.h"

/* CJK encodings [CES = character encoding scheme] [CJKV.INF chapter 4] */

#include "euc_jp.h"
#include "sjis.h"
#include "cp932.h"
#include "iso2022_jp.h"
#include "iso2022_jp1.h"
#include "iso2022_jp2.h"

#include "euc_cn.h"
#include "ces_gbk.h"
#include "gb18030.h"
#include "iso2022_cn.h"
#include "iso2022_cnext.h"
#include "hz.h"
#include "euc_tw.h"
#include "ces_big5.h"
#include "cp950.h"
#include "big5hkscs.h"

#include "euc_kr.h"
#include "cp949.h"
#include "johab.h"
#include "iso2022_kr.h"

/* Encodings used by system dependent locales. */

#ifdef USE_AIX
#include "cp856.h"
#include "cp922.h"
#include "cp943.h"
#include "cp1046.h"
#include "cp1124.h"
#include "cp1129.h"
#include "cp1161.h"
#include "cp1162.h"
#include "cp1163.h"
#endif

#ifdef USE_OSF1
#include "dec_kanji.h"
#include "dec_hanyu.h"
#endif

#ifdef USE_DOS
#include "cp437.h"
#include "cp737.h"
#include "cp775.h"
#include "cp852.h"
#include "cp853.h"
#include "cp855.h"
#include "cp857.h"
#include "cp858.h"
#include "cp860.h"
#include "cp861.h"
#include "cp863.h"
#include "cp864.h"
#include "cp865.h"
#include "cp869.h"
#include "cp1125.h"
#endif

#ifdef USE_EXTRA
#include "euc_jisx0213.h"
#include "shift_jisx0213.h"
#include "iso2022_jp3.h"
#include "tds565.h"
#include "riscos1.h"
#endif

