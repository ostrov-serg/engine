
#pragma once

#define LINK_SIZE				2
#define MATCH_LIMIT				10000000
#define MATCH_LIMIT_RECURSION	MATCH_LIMIT
#define MAX_NAME_COUNT			10000
#define MAX_NAME_SIZE			32
#define NEWLINE					10
#define PARENS_NEST_LIMIT		250

#define REGEX_CASELESS           0x00000001  
#define REGEX_MULTILINE          0x00000002  
#define REGEX_DOTALL             0x00000004  
#define REGEX_EXTENDED           0x00000008  
#define REGEX_ANCHORED           0x00000010  
#define REGEX_DOLLAR_ENDONLY     0x00000020  
#define REGEX_EXTRA              0x00000040  
#define REGEX_NOTBOL             0x00000080  
#define REGEX_NOTEOL             0x00000100  
#define REGEX_UNGREEDY           0x00000200  
#define REGEX_NOTEMPTY           0x00000400  
#define REGEX_UTF8               0x00000800  
#define REGEX_UTF16              0x00000800  
#define REGEX_UTF32              0x00000800  
#define REGEX_NO_AUTO_CAPTURE    0x00001000  
#define REGEX_NO_UTF8_CHECK      0x00002000  
#define REGEX_NO_UTF16_CHECK     0x00002000  
#define REGEX_NO_UTF32_CHECK     0x00002000  
#define REGEX_AUTO_CALLOUT       0x00004000  
#define REGEX_PARTIAL_SOFT       0x00008000  
#define REGEX_PARTIAL            0x00008000  
#define REGEX_NEVER_UTF          0x00010000  
#define REGEX_DFA_SHORTEST       0x00010000  
#define REGEX_NO_AUTO_POSSESS    0x00020000  
#define REGEX_DFA_RESTART        0x00020000  
#define REGEX_FIRSTLINE          0x00040000  
#define REGEX_DUPNAMES           0x00080000  
#define REGEX_NEWLINE_CR         0x00100000  
#define REGEX_NEWLINE_LF         0x00200000  
#define REGEX_NEWLINE_CRLF       0x00300000  
#define REGEX_NEWLINE_ANY        0x00400000  
#define REGEX_NEWLINE_ANYCRLF    0x00500000  
#define REGEX_BSR_ANYCRLF        0x00800000  
#define REGEX_BSR_UNICODE        0x01000000  
#define REGEX_JAVASCRIPT_COMPAT  0x02000000  
#define REGEX_NO_START_OPTIMIZE  0x04000000  
#define REGEX_NO_START_OPTIMISE  0x04000000  
#define REGEX_PARTIAL_HARD       0x08000000  
#define REGEX_NOTEMPTY_ATSTART   0x10000000  
#define REGEX_UCP                0x20000000  

#define REGEX_ERROR_NOMATCH          (-1)
#define REGEX_ERROR_NULL             (-2)
#define REGEX_ERROR_BADOPTION        (-3)
#define REGEX_ERROR_BADMAGIC         (-4)
#define REGEX_ERROR_UNKNOWN_OPCODE   (-5)
#define REGEX_ERROR_UNKNOWN_NODE     (-5)  
#define REGEX_ERROR_NOMEMORY         (-6)
#define REGEX_ERROR_NOSUBSTRING      (-7)
#define REGEX_ERROR_MATCHLIMIT       (-8)
#define REGEX_ERROR_CALLOUT          (-9)  
#define REGEX_ERROR_BADUTF8         (-10)  
#define REGEX_ERROR_BADUTF16        (-10)  
#define REGEX_ERROR_BADUTF32        (-10)  
#define REGEX_ERROR_BADUTF8_OFFSET  (-11)  
#define REGEX_ERROR_BADUTF16_OFFSET (-11)  
#define REGEX_ERROR_PARTIAL         (-12)
#define REGEX_ERROR_BADPARTIAL      (-13)
#define REGEX_ERROR_INTERNAL        (-14)
#define REGEX_ERROR_BADCOUNT        (-15)
#define REGEX_ERROR_DFA_UITEM       (-16)
#define REGEX_ERROR_DFA_UCOND       (-17)
#define REGEX_ERROR_DFA_UMLIMIT     (-18)
#define REGEX_ERROR_DFA_WSSIZE      (-19)
#define REGEX_ERROR_DFA_RECURSE     (-20)
#define REGEX_ERROR_RECURSIONLIMIT  (-21)
#define REGEX_ERROR_NULLWSLIMIT     (-22)  
#define REGEX_ERROR_BADNEWLINE      (-23)
#define REGEX_ERROR_BADOFFSET       (-24)
#define REGEX_ERROR_SHORTUTF8       (-25)
#define REGEX_ERROR_SHORTUTF16      (-25)  
#define REGEX_ERROR_RECURSELOOP     (-26)
#define REGEX_ERROR_JIT_STACKLIMIT  (-27)
#define REGEX_ERROR_BADMODE         (-28)
#define REGEX_ERROR_BADENDIANNESS   (-29)
#define REGEX_ERROR_DFA_BADRESTART  (-30)
#define REGEX_ERROR_JIT_BADOPTION   (-31)
#define REGEX_ERROR_BADLENGTH       (-32)
#define REGEX_ERROR_UNSET           (-33)

struct real_regex16;
typedef struct real_regex16 regex16;
