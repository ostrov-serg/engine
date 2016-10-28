
#pragma once

#define TOTAL_KEYWORDS 329
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 45
#define MIN_HASH_VALUE 11
#define MAX_HASH_VALUE 849

const struct alias* aliases_lookup(const ssh_ws* str, unsigned int len);

struct alias
{
	int name;
	unsigned int encoding_index;
};

struct stringpool_t
{
	ssh_ws stringpool_str11[sizeof(L"CN")];
	ssh_ws stringpool_str15[sizeof(L"R8")];
	ssh_ws stringpool_str17[sizeof(L"866")];
	ssh_ws stringpool_str25[sizeof(L"862")];
	ssh_ws stringpool_str26[sizeof(L"CP1361")];
	ssh_ws stringpool_str27[sizeof(L"CP866")];
	ssh_ws stringpool_str28[sizeof(L"CP1251")];
	ssh_ws stringpool_str30[sizeof(L"CP1256")];
	ssh_ws stringpool_str32[sizeof(L"CP1255")];
	ssh_ws stringpool_str33[sizeof(L"CP1133")];
	ssh_ws stringpool_str34[sizeof(L"ASCII")];
	ssh_ws stringpool_str35[sizeof(L"CP862")];
	ssh_ws stringpool_str36[sizeof(L"CP1253")];
	ssh_ws stringpool_str37[sizeof(L"CHAR")];
	ssh_ws stringpool_str38[sizeof(L"CP1252")];
	ssh_ws stringpool_str39[sizeof(L"CP936")];
	ssh_ws stringpool_str40[sizeof(L"CP1258")];
	ssh_ws stringpool_str42[sizeof(L"C99")];
	ssh_ws stringpool_str47[sizeof(L"CP932")];
	ssh_ws stringpool_str49[sizeof(L"ISO-IR-6")];
	ssh_ws stringpool_str54[sizeof(L"CP819")];
	ssh_ws stringpool_str56[sizeof(L"ISO-IR-166")];
	ssh_ws stringpool_str58[sizeof(L"ISO-IR-165")];
	ssh_ws stringpool_str60[sizeof(L"ISO-IR-126")];
	ssh_ws stringpool_str64[sizeof(L"ISO-IR-58")];
	ssh_ws stringpool_str65[sizeof(L"ISO-IR-226")];
	ssh_ws stringpool_str66[sizeof(L"ISO8859-1")];
	ssh_ws stringpool_str68[sizeof(L"ISO8859-6")];
	ssh_ws stringpool_str69[sizeof(L"ISO-IR-138")];
	ssh_ws stringpool_str70[sizeof(L"ISO8859-5")];
	ssh_ws stringpool_str71[sizeof(L"ISO8859-16")];
	ssh_ws stringpool_str73[sizeof(L"ISO8859-15")];
	ssh_ws stringpool_str74[sizeof(L"ISO8859-3")];
	ssh_ws stringpool_str76[sizeof(L"ISO8859-2")];
	ssh_ws stringpool_str77[sizeof(L"ISO8859-13")];
	ssh_ws stringpool_str78[sizeof(L"ISO8859-8")];
	ssh_ws stringpool_str79[sizeof(L"ISO-8859-1")];
	ssh_ws stringpool_str80[sizeof(L"GB2312")];
	ssh_ws stringpool_str81[sizeof(L"ISO-8859-6")];
	ssh_ws stringpool_str82[sizeof(L"EUCCN")];
	ssh_ws stringpool_str83[sizeof(L"ISO-8859-5")];
	ssh_ws stringpool_str84[sizeof(L"ISO-8859-16")];
	ssh_ws stringpool_str85[sizeof(L"ISO-IR-159")];
	ssh_ws stringpool_str86[sizeof(L"ISO-8859-15")];
	ssh_ws stringpool_str87[sizeof(L"ISO-8859-3")];
	ssh_ws stringpool_str89[sizeof(L"ISO-8859-2")];
	ssh_ws stringpool_str90[sizeof(L"ISO-8859-13")];
	ssh_ws stringpool_str91[sizeof(L"ISO-8859-8")];
	ssh_ws stringpool_str92[sizeof(L"ISO-IR-101")];
	ssh_ws stringpool_str93[sizeof(L"850")];
	ssh_ws stringpool_str95[sizeof(L"EUC-CN")];
	ssh_ws stringpool_str96[sizeof(L"ISO8859-9")];
	ssh_ws stringpool_str98[sizeof(L"ISO-IR-199")];
	ssh_ws stringpool_str99[sizeof(L"CSASCII")];
	ssh_ws stringpool_str100[sizeof(L"ISO646-CN")];
	ssh_ws stringpool_str104[sizeof(L"CP850")];
	ssh_ws stringpool_str105[sizeof(L"ISO-IR-203")];
	ssh_ws stringpool_str106[sizeof(L"CP1250")];
	ssh_ws stringpool_str107[sizeof(L"HZ")];
	ssh_ws stringpool_str109[sizeof(L"ISO-8859-9")];
	ssh_ws stringpool_str113[sizeof(L"CP950")];
	ssh_ws stringpool_str114[sizeof(L"ISO-2022-CN")];
	ssh_ws stringpool_str116[sizeof(L"ISO_8859-1")];
	ssh_ws stringpool_str117[sizeof(L"CP949")];
	ssh_ws stringpool_str118[sizeof(L"ISO_8859-6")];
	ssh_ws stringpool_str119[sizeof(L"ISO-IR-148")];
	ssh_ws stringpool_str120[sizeof(L"ISO_8859-5")];
	ssh_ws stringpool_str121[sizeof(L"ISO_8859-16")];
	ssh_ws stringpool_str122[sizeof(L"ISO-IR-109")];
	ssh_ws stringpool_str123[sizeof(L"ISO_8859-15")];
	ssh_ws stringpool_str124[sizeof(L"ISO_8859-3")];
	ssh_ws stringpool_str125[sizeof(L"ISO_8859-16:2001")];
	ssh_ws stringpool_str126[sizeof(L"ISO_8859-2")];
	ssh_ws stringpool_str127[sizeof(L"ISO_8859-13")];
	ssh_ws stringpool_str128[sizeof(L"ISO_8859-8")];
	ssh_ws stringpool_str131[sizeof(L"ISO-IR-110")];
	ssh_ws stringpool_str132[sizeof(L"ISO_8859-15:1998")];
	ssh_ws stringpool_str134[sizeof(L"ISO-2022-CN-EXT")];
	ssh_ws stringpool_str136[sizeof(L"CP1254")];
	ssh_ws stringpool_str137[sizeof(L"ISO-IR-149")];
	ssh_ws stringpool_str139[sizeof(L"L1")];
	ssh_ws stringpool_str140[sizeof(L"L6")];
	ssh_ws stringpool_str141[sizeof(L"L5")];
	ssh_ws stringpool_str143[sizeof(L"L3")];
	ssh_ws stringpool_str144[sizeof(L"L2")];
	ssh_ws stringpool_str145[sizeof(L"L8")];
	ssh_ws stringpool_str146[sizeof(L"ISO_8859-9")];
	ssh_ws stringpool_str147[sizeof(L"ISO8859-10")];
	ssh_ws stringpool_str153[sizeof(L"CSISO2022CN")];
	ssh_ws stringpool_str155[sizeof(L"ISO-IR-179")];
	ssh_ws stringpool_str156[sizeof(L"UHC")];
	ssh_ws stringpool_str158[sizeof(L"ISO-IR-14")];
	ssh_ws stringpool_str160[sizeof(L"ISO-8859-10")];
	ssh_ws stringpool_str167[sizeof(L"CP367")];
	ssh_ws stringpool_str168[sizeof(L"ISO_8859-10:1992")];
	ssh_ws stringpool_str170[sizeof(L"ISO-IR-100")];
	ssh_ws stringpool_str171[sizeof(L"LATIN1")];
	ssh_ws stringpool_str172[sizeof(L"CP1257")];
	ssh_ws stringpool_str173[sizeof(L"LATIN6")];
	ssh_ws stringpool_str174[sizeof(L"ISO8859-4")];
	ssh_ws stringpool_str175[sizeof(L"LATIN5")];
	ssh_ws stringpool_str176[sizeof(L"TIS620")];
	ssh_ws stringpool_str177[sizeof(L"ISO8859-14")];
	ssh_ws stringpool_str178[sizeof(L"ELOT_928")];
	ssh_ws stringpool_str179[sizeof(L"LATIN3")];
	ssh_ws stringpool_str180[sizeof(L"SJIS")];
	ssh_ws stringpool_str181[sizeof(L"LATIN2")];
	ssh_ws stringpool_str183[sizeof(L"LATIN8")];
	ssh_ws stringpool_str184[sizeof(L"ISO_8859-14:1998")];
	ssh_ws stringpool_str185[sizeof(L"GEORGIAN-ACADEMY")];
	ssh_ws stringpool_str186[sizeof(L"MAC")];
	ssh_ws stringpool_str187[sizeof(L"ISO-8859-4")];
	ssh_ws stringpool_str189[sizeof(L"TIS-620")];
	ssh_ws stringpool_str190[sizeof(L"ISO-8859-14")];
	ssh_ws stringpool_str191[sizeof(L"GB18030")];
	ssh_ws stringpool_str192[sizeof(L"X0212")];
	ssh_ws stringpool_str193[sizeof(L"L4")];
	ssh_ws stringpool_str196[sizeof(L"ISO-IR-57")];
	ssh_ws stringpool_str197[sizeof(L"ISO_8859-10")];
	ssh_ws stringpool_str198[sizeof(L"IBM866")];
	ssh_ws stringpool_str199[sizeof(L"ISO-IR-157")];
	ssh_ws stringpool_str200[sizeof(L"ISO-IR-87")];
	ssh_ws stringpool_str202[sizeof(L"ISO-IR-127")];
	ssh_ws stringpool_str203[sizeof(L"US")];
	ssh_ws stringpool_str204[sizeof(L"CP874")];
	ssh_ws stringpool_str206[sizeof(L"IBM862")];
	ssh_ws stringpool_str207[sizeof(L"MS936")];
	ssh_ws stringpool_str210[sizeof(L"ISO8859-7")];
	ssh_ws stringpool_str211[sizeof(L"L7")];
	ssh_ws stringpool_str214[sizeof(L"LATIN-9")];
	ssh_ws stringpool_str215[sizeof(L"ISO-IR-144")];
	ssh_ws stringpool_str220[sizeof(L"L10")];
	ssh_ws stringpool_str221[sizeof(L"X0201")];
	ssh_ws stringpool_str222[sizeof(L"ROMAN8")];
	ssh_ws stringpool_str223[sizeof(L"ISO-8859-7")];
	ssh_ws stringpool_str224[sizeof(L"ISO_8859-4")];
	ssh_ws stringpool_str225[sizeof(L"IBM819")];
	ssh_ws stringpool_str226[sizeof(L"ARABIC")];
	ssh_ws stringpool_str227[sizeof(L"ISO_8859-14")];
	ssh_ws stringpool_str228[sizeof(L"GB_2312-80")];
	ssh_ws stringpool_str229[sizeof(L"BIG5")];
	ssh_ws stringpool_str231[sizeof(L"TIS620-0")];
	ssh_ws stringpool_str232[sizeof(L"UCS-2")];
	ssh_ws stringpool_str233[sizeof(L"X0208")];
	ssh_ws stringpool_str238[sizeof(L"CSBIG5")];
	ssh_ws stringpool_str239[sizeof(L"CSKOI8R")];
	ssh_ws stringpool_str241[sizeof(L"GB_1988-80")];
	ssh_ws stringpool_str242[sizeof(L"BIG-5")];
	ssh_ws stringpool_str243[sizeof(L"KOI8-R")];
	ssh_ws stringpool_str244[sizeof(L"IBM-CP1133")];
	ssh_ws stringpool_str249[sizeof(L"JP")];
	ssh_ws stringpool_str250[sizeof(L"US-ASCII")];
	ssh_ws stringpool_str251[sizeof(L"CN-BIG5")];
	ssh_ws stringpool_str252[sizeof(L"LATIN10")];
	ssh_ws stringpool_str253[sizeof(L"CHINESE")];
	ssh_ws stringpool_str255[sizeof(L"CSUNICODE11")];
	ssh_ws stringpool_str257[sizeof(L"ISO-CELTIC")];
	ssh_ws stringpool_str259[sizeof(L"CSGB2312")];
	ssh_ws stringpool_str260[sizeof(L"ISO_8859-7")];
	ssh_ws stringpool_str261[sizeof(L"CSISOLATIN1")];
	ssh_ws stringpool_str263[sizeof(L"CSISOLATIN6")];
	ssh_ws stringpool_str265[sizeof(L"CSISOLATIN5")];
	ssh_ws stringpool_str266[sizeof(L"TIS620.2533-1")];
	ssh_ws stringpool_str267[sizeof(L"MACCROATIAN")];
	ssh_ws stringpool_str269[sizeof(L"CSISOLATIN3")];
	ssh_ws stringpool_str270[sizeof(L"UNICODE-1-1")];
	ssh_ws stringpool_str271[sizeof(L"CSISOLATIN2")];
	ssh_ws stringpool_str273[sizeof(L"KOI8-T")];
	ssh_ws stringpool_str274[sizeof(L"CSISOLATINCYRILLIC")];
	ssh_ws stringpool_str275[sizeof(L"IBM850")];
	ssh_ws stringpool_str276[sizeof(L"MS-ANSI")];
	ssh_ws stringpool_str278[sizeof(L"TIS620.2529-1")];
	ssh_ws stringpool_str279[sizeof(L"LATIN4")];
	ssh_ws stringpool_str280[sizeof(L"GEORGIAN-PS")];
	ssh_ws stringpool_str284[sizeof(L"EUCKR")];
	ssh_ws stringpool_str285[sizeof(L"CSISOLATINARABIC")];
	ssh_ws stringpool_str290[sizeof(L"ECMA-118")];
	ssh_ws stringpool_str292[sizeof(L"UTF-16")];
	ssh_ws stringpool_str295[sizeof(L"ARMSCII-8")];
	ssh_ws stringpool_str297[sizeof(L"EUC-KR")];
	ssh_ws stringpool_str298[sizeof(L"ISO-10646-UCS-2")];
	ssh_ws stringpool_str299[sizeof(L"UTF-8")];
	ssh_ws stringpool_str301[sizeof(L"KOREAN")];
	ssh_ws stringpool_str302[sizeof(L"CYRILLIC")];
	ssh_ws stringpool_str304[sizeof(L"UTF-32")];
	ssh_ws stringpool_str305[sizeof(L"TIS620.2533-0")];
	ssh_ws stringpool_str306[sizeof(L"CSUNICODE")];
	ssh_ws stringpool_str310[sizeof(L"ISO_8859-5:1988")];
	ssh_ws stringpool_str312[sizeof(L"ISO_8859-3:1988")];
	ssh_ws stringpool_str314[sizeof(L"ISO_8859-8:1988")];
	ssh_ws stringpool_str315[sizeof(L"LATIN7")];
	ssh_ws stringpool_str316[sizeof(L"ISO-2022-KR")];
	ssh_ws stringpool_str319[sizeof(L"KSC_5601")];
	ssh_ws stringpool_str327[sizeof(L"MACTHAI")];
	ssh_ws stringpool_str329[sizeof(L"CSUCS4")];
	ssh_ws stringpool_str330[sizeof(L"UCS-4")];
	ssh_ws stringpool_str331[sizeof(L"CSUNICODE11UTF7")];
	ssh_ws stringpool_str332[sizeof(L"ISO_8859-9:1989")];
	ssh_ws stringpool_str333[sizeof(L"CN-GB-ISOIR165")];
	ssh_ws stringpool_str336[sizeof(L"EUCJP")];
	ssh_ws stringpool_str338[sizeof(L"IBM367")];
	ssh_ws stringpool_str339[sizeof(L"HP-ROMAN8")];
	ssh_ws stringpool_str344[sizeof(L"ASMO-708")];
	ssh_ws stringpool_str346[sizeof(L"ISO646-US")];
	ssh_ws stringpool_str347[sizeof(L"ISO-10646-UCS-4")];
	ssh_ws stringpool_str348[sizeof(L"UNICODE-1-1-UTF-7")];
	ssh_ws stringpool_str349[sizeof(L"EUC-JP")];
	ssh_ws stringpool_str350[sizeof(L"WCHAR_T")];
	ssh_ws stringpool_str351[sizeof(L"EUCTW")];
	ssh_ws stringpool_str352[sizeof(L"ISO-2022-JP-1")];
	ssh_ws stringpool_str353[sizeof(L"CSHPROMAN8")];
	ssh_ws stringpool_str354[sizeof(L"ISO646-JP")];
	ssh_ws stringpool_str355[sizeof(L"CSISO2022KR")];
	ssh_ws stringpool_str356[sizeof(L"TCVN")];
	ssh_ws stringpool_str357[sizeof(L"ISO-2022-JP-2")];
	ssh_ws stringpool_str362[sizeof(L"ISO_8859-4:1988")];
	ssh_ws stringpool_str364[sizeof(L"EUC-TW")];
	ssh_ws stringpool_str365[sizeof(L"CSISO58GB231280")];
	ssh_ws stringpool_str367[sizeof(L"MS-EE")];
	ssh_ws stringpool_str368[sizeof(L"ISO-2022-JP")];
	ssh_ws stringpool_str369[sizeof(L"CSISOLATIN4")];
	ssh_ws stringpool_str372[sizeof(L"CSPC862LATINHEBREW")];
	ssh_ws stringpool_str373[sizeof(L"NEXTSTEP")];
	ssh_ws stringpool_str374[sizeof(L"ISO_8859-1:1987")];
	ssh_ws stringpool_str375[sizeof(L"ISO_8859-6:1987")];
	ssh_ws stringpool_str377[sizeof(L"CSIBM866")];
	ssh_ws stringpool_str379[sizeof(L"ISO_8859-2:1987")];
	ssh_ws stringpool_str380[sizeof(L"HZ-GB-2312")];
	ssh_ws stringpool_str383[sizeof(L"WINDOWS-1251")];
	ssh_ws stringpool_str384[sizeof(L"WINDOWS-1256")];
	ssh_ws stringpool_str385[sizeof(L"WINDOWS-1255")];
	ssh_ws stringpool_str386[sizeof(L"ECMA-114")];
	ssh_ws stringpool_str387[sizeof(L"WINDOWS-1253")];
	ssh_ws stringpool_str388[sizeof(L"WINDOWS-1252")];
	ssh_ws stringpool_str389[sizeof(L"WINDOWS-1258")];
	ssh_ws stringpool_str390[sizeof(L"GREEK8")];
	ssh_ws stringpool_str392[sizeof(L"MACROMAN")];
	ssh_ws stringpool_str393[sizeof(L"JIS_C6226-1983")];
	ssh_ws stringpool_str395[sizeof(L"CSISO2022JP2")];
	ssh_ws stringpool_str396[sizeof(L"WINDOWS-936")];
	ssh_ws stringpool_str397[sizeof(L"JIS0208")];
	ssh_ws stringpool_str399[sizeof(L"VISCII")];
	ssh_ws stringpool_str402[sizeof(L"CSISO57GB1988")];
	ssh_ws stringpool_str403[sizeof(L"KS_C_5601-1989")];
	ssh_ws stringpool_str407[sizeof(L"CSISO2022JP")];
	ssh_ws stringpool_str408[sizeof(L"CSVISCII")];
	ssh_ws stringpool_str411[sizeof(L"CN-GB")];
	ssh_ws stringpool_str412[sizeof(L"MACARABIC")];
	ssh_ws stringpool_str422[sizeof(L"WINDOWS-1250")];
	ssh_ws stringpool_str428[sizeof(L"MACROMANIA")];
	ssh_ws stringpool_str429[sizeof(L"CSKSC56011987")];
	ssh_ws stringpool_str430[sizeof(L"JIS_C6220-1969-RO")];
	ssh_ws stringpool_str431[sizeof(L"UTF-7")];
	ssh_ws stringpool_str434[sizeof(L"CSEUCKR")];
	ssh_ws stringpool_str436[sizeof(L"CSISO14JISC6220RO")];
	ssh_ws stringpool_str437[sizeof(L"WINDOWS-1254")];
	ssh_ws stringpool_str438[sizeof(L"CSISO159JISX02121990")];
	ssh_ws stringpool_str446[sizeof(L"ISO_8859-7:1987")];
	ssh_ws stringpool_str447[sizeof(L"MACICELAND")];
	ssh_ws stringpool_str455[sizeof(L"WINDOWS-1257")];
	ssh_ws stringpool_str458[sizeof(L"GBK")];
	ssh_ws stringpool_str460[sizeof(L"KS_C_5601-1987")];
	ssh_ws stringpool_str461[sizeof(L"TCVN5712-1")];
	ssh_ws stringpool_str463[sizeof(L"TCVN-5712")];
	ssh_ws stringpool_str471[sizeof(L"UCS-2-INTERNAL")];
	ssh_ws stringpool_str473[sizeof(L"MACINTOSH")];
	ssh_ws stringpool_str478[sizeof(L"UNICODELITTLE")];
	ssh_ws stringpool_str480[sizeof(L"UCS-2LE")];
	ssh_ws stringpool_str483[sizeof(L"ANSI_X3.4-1986")];
	ssh_ws stringpool_str485[sizeof(L"MS-CYRL")];
	ssh_ws stringpool_str488[sizeof(L"ANSI_X3.4-1968")];
	ssh_ws stringpool_str493[sizeof(L"CSISOLATINHEBREW")];
	ssh_ws stringpool_str496[sizeof(L"MACCYRILLIC")];
	ssh_ws stringpool_str498[sizeof(L"CSMACINTOSH")];
	ssh_ws stringpool_str501[sizeof(L"CSEUCTW")];
	ssh_ws stringpool_str503[sizeof(L"UNICODEBIG")];
	ssh_ws stringpool_str510[sizeof(L"UCS-2-SWAPPED")];
	ssh_ws stringpool_str511[sizeof(L"CSISOLATINGREEK")];
	ssh_ws stringpool_str517[sizeof(L"UCS-2BE")];
	ssh_ws stringpool_str519[sizeof(L"KOI8-U")];
	ssh_ws stringpool_str520[sizeof(L"UCS-4-INTERNAL")];
	ssh_ws stringpool_str521[sizeof(L"VISCII1.1-1")];
	ssh_ws stringpool_str525[sizeof(L"KOI8-RU")];
	ssh_ws stringpool_str529[sizeof(L"UCS-4LE")];
	ssh_ws stringpool_str533[sizeof(L"MS-HEBR")];
	ssh_ws stringpool_str537[sizeof(L"EXTENDED_UNIX_CODE_PACKED_FORMAT_FOR_JAPANESE")];
	ssh_ws stringpool_str544[sizeof(L"UTF-16LE")];
	ssh_ws stringpool_str547[sizeof(L"MULELAO-1")];
	ssh_ws stringpool_str552[sizeof(L"UTF-32LE")];
	ssh_ws stringpool_str558[sizeof(L"MACCENTRALEUROPE")];
	ssh_ws stringpool_str559[sizeof(L"UCS-4-SWAPPED")];
	ssh_ws stringpool_str561[sizeof(L"WINDOWS-874")];
	ssh_ws stringpool_str563[sizeof(L"ISO_646.IRV:1991")];
	ssh_ws stringpool_str566[sizeof(L"UCS-4BE")];
	ssh_ws stringpool_str569[sizeof(L"SHIFT-JIS")];
	ssh_ws stringpool_str571[sizeof(L"JIS_X0212")];
	ssh_ws stringpool_str577[sizeof(L"MS-ARAB")];
	ssh_ws stringpool_str578[sizeof(L"GREEK")];
	ssh_ws stringpool_str581[sizeof(L"UTF-16BE")];
	ssh_ws stringpool_str587[sizeof(L"JISX0201-1976")];
	ssh_ws stringpool_str589[sizeof(L"UTF-32BE")];
	ssh_ws stringpool_str591[sizeof(L"JAVA")];
	ssh_ws stringpool_str600[sizeof(L"JIS_X0201")];
	ssh_ws stringpool_str604[sizeof(L"HEBREW")];
	ssh_ws stringpool_str606[sizeof(L"SHIFT_JIS")];
	ssh_ws stringpool_str612[sizeof(L"JIS_X0208")];
	ssh_ws stringpool_str623[sizeof(L"CSISO87JISX0208")];
	ssh_ws stringpool_str624[sizeof(L"JIS_X0212-1990")];
	ssh_ws stringpool_str629[sizeof(L"JIS_X0208-1983")];
	ssh_ws stringpool_str651[sizeof(L"TCVN5712-1:1993")];
	ssh_ws stringpool_str663[sizeof(L"CSSHIFTJIS")];
	ssh_ws stringpool_str664[sizeof(L"JIS_X0208-1990")];
	ssh_ws stringpool_str683[sizeof(L"MACUKRAINE")];
	ssh_ws stringpool_str688[sizeof(L"MS_KANJI")];
	ssh_ws stringpool_str689[sizeof(L"CSHALFWIDTHKATAKANA")];
	ssh_ws stringpool_str694[sizeof(L"JOHAB")];
	ssh_ws stringpool_str708[sizeof(L"CSPC850MULTILINGUAL")];
	ssh_ws stringpool_str713[sizeof(L"JIS_X0212.1990-0")];
	ssh_ws stringpool_str714[sizeof(L"BIG5HKSCS")];
	ssh_ws stringpool_str727[sizeof(L"BIG5-HKSCS")];
	ssh_ws stringpool_str764[sizeof(L"MACGREEK")];
	ssh_ws stringpool_str770[sizeof(L"MS-TURK")];
	ssh_ws stringpool_str771[sizeof(L"MS-GREEK")];
	ssh_ws stringpool_str791[sizeof(L"BIGFIVE")];
	ssh_ws stringpool_str804[sizeof(L"BIG-FIVE")];
	ssh_ws stringpool_str821[sizeof(L"MACTURKISH")];
	ssh_ws stringpool_str843[sizeof(L"WINBALTRIM")];
	ssh_ws stringpool_str844[sizeof(L"MACHEBREW")];
	ssh_ws stringpool_str849[sizeof(L"CSEUCPKDFMTJAPANESE")];
};
static const struct stringpool_t stringpool_contents =
{
	L"CN",
	L"R8",
	L"866",
	L"862",
	L"CP1361",
	L"CP866",
	L"CP1251",
	L"CP1256",
	L"CP1255",
	L"CP1133",
	L"ASCII",
	L"CP862",
	L"CP1253",
	L"CHAR",
	L"CP1252",
	L"CP936",
	L"CP1258",
	L"C99",
	L"CP932",
	L"ISO-IR-6",
	L"CP819",
	L"ISO-IR-166",
	L"ISO-IR-165",
	L"ISO-IR-126",
	L"ISO-IR-58",
	L"ISO-IR-226",
	L"ISO8859-1",
	L"ISO8859-6",
	L"ISO-IR-138",
	L"ISO8859-5",
	L"ISO8859-16",
	L"ISO8859-15",
	L"ISO8859-3",
	L"ISO8859-2",
	L"ISO8859-13",
	L"ISO8859-8",
	L"ISO-8859-1",
	L"GB2312",
	L"ISO-8859-6",
	L"EUCCN",
	L"ISO-8859-5",
	L"ISO-8859-16",
	L"ISO-IR-159",
	L"ISO-8859-15",
	L"ISO-8859-3",
	L"ISO-8859-2",
	L"ISO-8859-13",
	L"ISO-8859-8",
	L"ISO-IR-101",
	L"850",
	L"EUC-CN",
	L"ISO8859-9",
	L"ISO-IR-199",
	L"CSASCII",
	L"ISO646-CN",
	L"CP850",
	L"ISO-IR-203",
	L"CP1250",
	L"HZ",
	L"ISO-8859-9",
	L"CP950",
	L"ISO-2022-CN",
	L"ISO_8859-1",
	L"CP949",
	L"ISO_8859-6",
	L"ISO-IR-148",
	L"ISO_8859-5",
	L"ISO_8859-16",
	L"ISO-IR-109",
	L"ISO_8859-15",
	L"ISO_8859-3",
	L"ISO_8859-16:2001",
	L"ISO_8859-2",
	L"ISO_8859-13",
	L"ISO_8859-8",
	L"ISO-IR-110",
	L"ISO_8859-15:1998",
	L"ISO-2022-CN-EXT",
	L"CP1254",
	L"ISO-IR-149",
	L"L1",
	L"L6",
	L"L5",
	L"L3",
	L"L2",
	L"L8",
	L"ISO_8859-9",
	L"ISO8859-10",
	L"CSISO2022CN",
	L"ISO-IR-179",
	L"UHC",
	L"ISO-IR-14",
	L"ISO-8859-10",
	L"CP367",
	L"ISO_8859-10:1992",
	L"ISO-IR-100",
	L"LATIN1",
	L"CP1257",
	L"LATIN6",
	L"ISO8859-4",
	L"LATIN5",
	L"TIS620",
	L"ISO8859-14",
	L"ELOT_928",
	L"LATIN3",
	L"SJIS",
	L"LATIN2",
	L"LATIN8",
	L"ISO_8859-14:1998",
	L"GEORGIAN-ACADEMY",
	L"MAC",
	L"ISO-8859-4",
	L"TIS-620",
	L"ISO-8859-14",
	L"GB18030",
	L"X0212",
	L"L4",
	L"ISO-IR-57",
	L"ISO_8859-10",
	L"IBM866",
	L"ISO-IR-157",
	L"ISO-IR-87",
	L"ISO-IR-127",
	L"US",
	L"CP874",
	L"IBM862",
	L"MS936",
	L"ISO8859-7",
	L"L7",
	L"LATIN-9",
	L"ISO-IR-144",
	L"L10",
	L"X0201",
	L"ROMAN8",
	L"ISO-8859-7",
	L"ISO_8859-4",
	L"IBM819",
	L"ARABIC",
	L"ISO_8859-14",
	L"GB_2312-80",
	L"BIG5",
	L"TIS620-0",
	L"UCS-2",
	L"X0208",
	L"CSBIG5",
	L"CSKOI8R",
	L"GB_1988-80",
	L"BIG-5",
	L"KOI8-R",
	L"IBM-CP1133",
	L"JP",
	L"US-ASCII",
	L"CN-BIG5",
	L"LATIN10",
	L"CHINESE",
	L"CSUNICODE11",
	L"ISO-CELTIC",
	L"CSGB2312",
	L"ISO_8859-7",
	L"CSISOLATIN1",
	L"CSISOLATIN6",
	L"CSISOLATIN5",
	L"TIS620.2533-1",
	L"MACCROATIAN",
	L"CSISOLATIN3",
	L"UNICODE-1-1",
	L"CSISOLATIN2",
	L"KOI8-T",
	L"CSISOLATINCYRILLIC",
	L"IBM850",
	L"MS-ANSI",
	L"TIS620.2529-1",
	L"LATIN4",
	L"GEORGIAN-PS",
	L"EUCKR",
	L"CSISOLATINARABIC",
	L"ECMA-118",
	L"UTF-16",
	L"ARMSCII-8",
	L"EUC-KR",
	L"ISO-10646-UCS-2",
	L"UTF-8",
	L"KOREAN",
	L"CYRILLIC",
	L"UTF-32",
	L"TIS620.2533-0",
	L"CSUNICODE",
	L"ISO_8859-5:1988",
	L"ISO_8859-3:1988",
	L"ISO_8859-8:1988",
	L"LATIN7",
	L"ISO-2022-KR",
	L"KSC_5601",
	L"MACTHAI",
	L"CSUCS4",
	L"UCS-4",
	L"CSUNICODE11UTF7",
	L"ISO_8859-9:1989",
	L"CN-GB-ISOIR165",
	L"EUCJP",
	L"IBM367",
	L"HP-ROMAN8",
	L"ASMO-708",
	L"ISO646-US",
	L"ISO-10646-UCS-4",
	L"UNICODE-1-1-UTF-7",
	L"EUC-JP",
	L"WCHAR_T",
	L"EUCTW",
	L"ISO-2022-JP-1",
	L"CSHPROMAN8",
	L"ISO646-JP",
	L"CSISO2022KR",
	L"TCVN",
	L"ISO-2022-JP-2",
	L"ISO_8859-4:1988",
	L"EUC-TW",
	L"CSISO58GB231280",
	L"MS-EE",
	L"ISO-2022-JP",
	L"CSISOLATIN4",
	L"CSPC862LATINHEBREW",
	L"NEXTSTEP",
	L"ISO_8859-1:1987",
	L"ISO_8859-6:1987",
	L"CSIBM866",
	L"ISO_8859-2:1987",
	L"HZ-GB-2312",
	L"WINDOWS-1251",
	L"WINDOWS-1256",
	L"WINDOWS-1255",
	L"ECMA-114",
	L"WINDOWS-1253",
	L"WINDOWS-1252",
	L"WINDOWS-1258",
	L"GREEK8",
	L"MACROMAN",
	L"JIS_C6226-1983",
	L"CSISO2022JP2",
	L"WINDOWS-936",
	L"JIS0208",
	L"VISCII",
	L"CSISO57GB1988",
	L"KS_C_5601-1989",
	L"CSISO2022JP",
	L"CSVISCII",
	L"CN-GB",
	L"MACARABIC",
	L"WINDOWS-1250",
	L"MACROMANIA",
	L"CSKSC56011987",
	L"JIS_C6220-1969-RO",
	L"UTF-7",
	L"CSEUCKR",
	L"CSISO14JISC6220RO",
	L"WINDOWS-1254",
	L"CSISO159JISX02121990",
	L"ISO_8859-7:1987",
	L"MACICELAND",
	L"WINDOWS-1257",
	L"GBK",
	L"KS_C_5601-1987",
	L"TCVN5712-1",
	L"TCVN-5712",
	L"UCS-2-INTERNAL",
	L"MACINTOSH",
	L"UNICODELITTLE",
	L"UCS-2LE",
	L"ANSI_X3.4-1986",
	L"MS-CYRL",
	L"ANSI_X3.4-1968",
	L"CSISOLATINHEBREW",
	L"MACCYRILLIC",
	L"CSMACINTOSH",
	L"CSEUCTW",
	L"UNICODEBIG",
	L"UCS-2-SWAPPED",
	L"CSISOLATINGREEK",
	L"UCS-2BE",
	L"KOI8-U",
	L"UCS-4-INTERNAL",
	L"VISCII1.1-1",
	L"KOI8-RU",
	L"UCS-4LE",
	L"MS-HEBR",
	L"EXTENDED_UNIX_CODE_PACKED_FORMAT_FOR_JAPANESE",
	L"UTF-16LE",
	L"MULELAO-1",
	L"UTF-32LE",
	L"MACCENTRALEUROPE",
	L"UCS-4-SWAPPED",
	L"WINDOWS-874",
	L"ISO_646.IRV:1991",
	L"UCS-4BE",
	L"SHIFT-JIS",
	L"JIS_X0212",
	L"MS-ARAB",
	L"GREEK",
	L"UTF-16BE",
	L"JISX0201-1976",
	L"UTF-32BE",
	L"JAVA",
	L"JIS_X0201",
	L"HEBREW",
	L"SHIFT_JIS",
	L"JIS_X0208",
	L"CSISO87JISX0208",
	L"JIS_X0212-1990",
	L"JIS_X0208-1983",
	L"TCVN5712-1:1993",
	L"CSSHIFTJIS",
	L"JIS_X0208-1990",
	L"MACUKRAINE",
	L"MS_KANJI",
	L"CSHALFWIDTHKATAKANA",
	L"JOHAB",
	L"CSPC850MULTILINGUAL",
	L"JIS_X0212.1990-0",
	L"BIG5HKSCS",
	L"BIG5-HKSCS",
	L"MACGREEK",
	L"MS-TURK",
	L"MS-GREEK",
	L"BIGFIVE",
	L"BIG-FIVE",
	L"MACTURKISH",
	L"WINBALTRIM",
	L"MACHEBREW",
	L"CSEUCPKDFMTJAPANESEL"
};
#define stringpool ((const ssh_ws*)&stringpool_contents)

static const struct alias aliases[] =
{
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str11, ei_iso646_cn},
	{-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str15, ei_hp_roman8},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str17, ei_cp866},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str25, ei_cp862},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str26, ei_johab},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str27, ei_cp866},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str28, ei_cp1251},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str30, ei_cp1256},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str32, ei_cp1255},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str33, ei_cp1133},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str34, ei_ascii},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str35, ei_cp862},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str36, ei_cp1253},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str37, ei_local_char},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str38, ei_cp1252},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str39, ei_ces_gbk},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str40, ei_cp1258},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str42, ei_c99},
	{-1}, {-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str47, ei_cp932},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str49, ei_ascii},
	{-1}, {-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str54, ei_iso8859_1},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str56, ei_tis620},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str58, ei_isoir165},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str60, ei_iso8859_7},
	{-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str64, ei_gb2312},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str65, ei_iso8859_16},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str66, ei_iso8859_1},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str68, ei_iso8859_6},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str69, ei_iso8859_8},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str70, ei_iso8859_5},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str71, ei_iso8859_16},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str73, ei_iso8859_15},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str74, ei_iso8859_3},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str76, ei_iso8859_2},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str77, ei_iso8859_13},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str78, ei_iso8859_8},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str79, ei_iso8859_1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str80, ei_euc_cn},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str81, ei_iso8859_6},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str82, ei_euc_cn},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str83, ei_iso8859_5},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str84, ei_iso8859_16},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str85, ei_jisx0212},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str86, ei_iso8859_15},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str87, ei_iso8859_3},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str89, ei_iso8859_2},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str90, ei_iso8859_13},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str91, ei_iso8859_8},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str92, ei_iso8859_2},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str93, ei_cp850},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str95, ei_euc_cn},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str96, ei_iso8859_9},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str98, ei_iso8859_14},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str99, ei_ascii},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str100, ei_iso646_cn},
	{-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str104, ei_cp850},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str105, ei_iso8859_15},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str106, ei_cp1250},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str107, ei_hz},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str109, ei_iso8859_9},
	{-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str113, ei_cp950},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str114, ei_iso2022_cn},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str116, ei_iso8859_1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str117, ei_cp949},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str118, ei_iso8859_6},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str119, ei_iso8859_9},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str120, ei_iso8859_5},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str121, ei_iso8859_16},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str122, ei_iso8859_3},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str123, ei_iso8859_15},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str124, ei_iso8859_3},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str125, ei_iso8859_16},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str126, ei_iso8859_2},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str127, ei_iso8859_13},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str128, ei_iso8859_8},
	{-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str131, ei_iso8859_4},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str132, ei_iso8859_15},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str134, ei_iso2022_cn_ext},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str136, ei_cp1254},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str137, ei_ksc5601},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str139, ei_iso8859_1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str140, ei_iso8859_10},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str141, ei_iso8859_9},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str143, ei_iso8859_3},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str144, ei_iso8859_2},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str145, ei_iso8859_14},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str146, ei_iso8859_9},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str147, ei_iso8859_10},
	{-1}, {-1}, {-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str153, ei_iso2022_cn},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str155, ei_iso8859_13},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str156, ei_cp949},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str158, ei_iso646_jp},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str160, ei_iso8859_10},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str167, ei_ascii},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str168, ei_iso8859_10},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str170, ei_iso8859_1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str171, ei_iso8859_1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str172, ei_cp1257},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str173, ei_iso8859_10},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str174, ei_iso8859_4},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str175, ei_iso8859_9},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str176, ei_tis620},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str177, ei_iso8859_14},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str178, ei_iso8859_7},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str179, ei_iso8859_3},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str180, ei_sjis},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str181, ei_iso8859_2},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str183, ei_iso8859_14},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str184, ei_iso8859_14},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str185, ei_georgian_academy},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str186, ei_mac_roman},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str187, ei_iso8859_4},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str189, ei_tis620},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str190, ei_iso8859_14},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str191, ei_gb18030},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str192, ei_jisx0212},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str193, ei_iso8859_4},
	{-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str196, ei_iso646_cn},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str197, ei_iso8859_10},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str198, ei_cp866},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str199, ei_iso8859_10},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str200, ei_jisx0208},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str202, ei_iso8859_6},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str203, ei_ascii},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str204, ei_cp874},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str206, ei_cp862},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str207, ei_ces_gbk},
	{-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str210, ei_iso8859_7},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str211, ei_iso8859_13},
	{-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str214, ei_iso8859_15},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str215, ei_iso8859_5},
	{-1}, {-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str220, ei_iso8859_16},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str221, ei_jisx0201},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str222, ei_hp_roman8},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str223, ei_iso8859_7},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str224, ei_iso8859_4},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str225, ei_iso8859_1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str226, ei_iso8859_6},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str227, ei_iso8859_14},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str228, ei_gb2312},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str229, ei_ces_big5},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str231, ei_tis620},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str232, ei_ucs2},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str233, ei_jisx0208},
	{-1}, {-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str238, ei_ces_big5},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str239, ei_koi8_r},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str241, ei_iso646_cn},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str242, ei_ces_big5},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str243, ei_koi8_r},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str244, ei_cp1133},
	{-1}, {-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str249, ei_iso646_jp},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str250, ei_ascii},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str251, ei_ces_big5},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str252, ei_iso8859_16},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str253, ei_gb2312},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str255, ei_ucs2be},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str257, ei_iso8859_14},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str259, ei_euc_cn},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str260, ei_iso8859_7},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str261, ei_iso8859_1},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str263, ei_iso8859_10},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str265, ei_iso8859_9},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str266, ei_tis620},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str267, ei_mac_croatian},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str269, ei_iso8859_3},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str270, ei_ucs2be},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str271, ei_iso8859_2},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str273, ei_koi8_t},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str274, ei_iso8859_5},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str275, ei_cp850},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str276, ei_cp1252},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str278, ei_tis620},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str279, ei_iso8859_4},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str280, ei_georgian_ps},
	{-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str284, ei_euc_kr},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str285, ei_iso8859_6},
	{-1}, {-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str290, ei_iso8859_7},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str292, ei_utf16},
	{-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str295, ei_armscii_8},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str297, ei_euc_kr},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str298, ei_ucs2},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str299, ei_utf8},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str301, ei_ksc5601},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str302, ei_iso8859_5},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str304, ei_utf32},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str305, ei_tis620},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str306, ei_ucs2},
	{-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str310, ei_iso8859_5},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str312, ei_iso8859_3},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str314, ei_iso8859_8},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str315, ei_iso8859_13},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str316, ei_iso2022_kr},
	{-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str319, ei_ksc5601},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str327, ei_mac_thai},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str329, ei_ucs4},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str330, ei_ucs4},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str331, ei_utf7},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str332, ei_iso8859_9},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str333, ei_isoir165},
	{-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str336, ei_euc_jp},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str338, ei_ascii},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str339, ei_hp_roman8},
	{-1}, {-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str344, ei_iso8859_6},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str346, ei_ascii},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str347, ei_ucs4},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str348, ei_utf7},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str349, ei_euc_jp},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str350, ei_local_wchar_t},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str351, ei_euc_tw},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str352, ei_iso2022_jp1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str353, ei_hp_roman8},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str354, ei_iso646_jp},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str355, ei_iso2022_kr},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str356, ei_tcvn},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str357, ei_iso2022_jp2},
	{-1}, {-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str362, ei_iso8859_4},
	{-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str364, ei_euc_tw},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str365, ei_gb2312},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str367, ei_cp1250},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str368, ei_iso2022_jp},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str369, ei_iso8859_4},
	{-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str372, ei_cp862},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str373, ei_nextstep},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str374, ei_iso8859_1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str375, ei_iso8859_6},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str377, ei_cp866},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str379, ei_iso8859_2},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str380, ei_hz},
	{-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str383, ei_cp1251},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str384, ei_cp1256},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str385, ei_cp1255},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str386, ei_iso8859_6},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str387, ei_cp1253},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str388, ei_cp1252},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str389, ei_cp1258},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str390, ei_iso8859_7},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str392, ei_mac_roman},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str393, ei_jisx0208},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str395, ei_iso2022_jp2},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str396, ei_ces_gbk},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str397, ei_jisx0208},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str399, ei_viscii},
	{-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str402, ei_iso646_cn},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str403, ei_ksc5601},
	{-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str407, ei_iso2022_jp},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str408, ei_viscii},
	{-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str411, ei_euc_cn},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str412, ei_mac_arabic},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str422, ei_cp1250},
	{-1}, {-1}, {-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str428, ei_mac_romania},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str429, ei_ksc5601},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str430, ei_iso646_jp},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str431, ei_utf7},
	{-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str434, ei_euc_kr},
	{-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str436, ei_iso646_jp},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str437, ei_cp1254},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str438, ei_jisx0212},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},

	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str446, ei_iso8859_7},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str447, ei_mac_iceland},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str455, ei_cp1257},
	{-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str458, ei_ces_gbk},
	{-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str460, ei_ksc5601},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str461, ei_tcvn},
	{-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str463, ei_tcvn},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str471, ei_ucs2internal},
	{-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str473, ei_mac_roman},
	{-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str478, ei_ucs2le},
	{-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str480, ei_ucs2le},
	{-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str483, ei_ascii},
	{-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str485, ei_cp1251},
	{-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str488, ei_ascii},
	{-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str493, ei_iso8859_8},
	{-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str496, ei_mac_cyrillic},
	{-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str498, ei_mac_roman},
	{-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str501, ei_euc_tw},
	{-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str503, ei_ucs2be},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str510, ei_ucs2swapped},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str511, ei_iso8859_7},
	{-1}, {-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str517, ei_ucs2be},
	{-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str519, ei_koi8_u},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str520, ei_ucs4internal},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str521, ei_viscii},
	{-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str525, ei_koi8_ru},
	{-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str529, ei_ucs4le},
	{-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str533, ei_cp1255},
	{-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str537, ei_euc_jp},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str544, ei_utf16le},
	{-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str547, ei_mulelao},
	{-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str552, ei_utf32le},
	{-1}, {-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str558, ei_mac_centraleurope},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str559, ei_ucs4swapped},
	{-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str561, ei_cp874},
	{-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str563, ei_ascii},
	{-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str566, ei_ucs4be},
	{-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str569, ei_sjis},
	{-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str571, ei_jisx0212},
	{-1}, {-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str577, ei_cp1256},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str578, ei_iso8859_7},
	{-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str581, ei_utf16be},
	{-1}, {-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str587, ei_jisx0201},
	{-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str589, ei_utf32be},
	{-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str591, ei_java},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str600, ei_jisx0201},
	{-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str604, ei_iso8859_8},
	{-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str606, ei_sjis},
	{-1}, {-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str612, ei_jisx0208},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str623, ei_jisx0208},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str624, ei_jisx0212},
	{-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str629, ei_jisx0208},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str651, ei_tcvn},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str663, ei_sjis},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str664, ei_jisx0208},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str683, ei_mac_ukraine},
	{-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str688, ei_sjis},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str689, ei_jisx0201},
	{-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str694, ei_johab},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str708, ei_cp850},
	{-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str713, ei_jisx0212},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str714, ei_big5hkscs},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str727, ei_big5hkscs},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str764, ei_mac_greek},
	{-1}, {-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str770, ei_cp1254},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str771, ei_cp1253},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str791, ei_ces_big5},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str804, ei_ces_big5},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str821, ei_mac_turkish},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str843, ei_cp1257},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str844, ei_mac_hebrew},
	{-1}, {-1}, {-1}, {-1},
	{(int)(long long)&((struct stringpool_t *)0)->stringpool_str849, ei_euc_jp}
};
