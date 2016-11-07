
#include "stdafx.h"
#include "ssh_str.h"

namespace ssh
{
	static ssh_ccs gName(nullptr);
	static int countArray(1);
	static String znamed[10];
	static int zindex(0);

	#define	IT_isfunction(it)		((((it) >= '6') && ((it) <= '9')) || ((it) == '_'))

	#define DIT_ptr64				0x04
	#define DIT_restrict			0x08
	#define DIT_near				0x00
	#define DIT_member				0x10
	#define DIT_based				0x0c
	#define	DIT_const				0x01
	#define DIT_modelmask			0x0c
	#define	BDT_const				0x01
	#define	BDT_volatile			0x02
	#define	DT_void					'X'
	#define RDT_array				'Y'
	#define	PoDT_void				'X'
	#define	PDT_reference			'A'
	#define	PDT_volatileReference	'B'
	#define PDT_extend				'$'
	#define PDT_ex_other			'B'
	#define PDT_ex_qualified		'C'
	#define	BDT_pointer				'P'
	#define	BDT_union				'T'
	#define	BDT_struct				'U'
	#define	BDT_class				'V'
	#define	BDT_enum				'W'
	#define	BDT_extend				'_'
	#define	BT_void					'0'
	#define	BT_nearptr				'2'

	String getPrimaryDataType(const String& superType);
	String getPtrRefDataType(const String& superType, bool isPtr);

	static String getZName()
	{
		int	zNameIndex(*gName - '0');
		if((zNameIndex >= 0) && (zNameIndex <= 9)) { gName++; return znamed[zNameIndex]; }
		if(*gName == '?') throw 1;
		String zName(gName, strchr(gName, '@') - gName);
		gName += (zName.length() + 1);
		if(zindex < 10) znamed[++zindex] = zName;
		return zName;
	}

	static String getScope()
	{
		String scope;
		while(*gName && *gName != '@')
		{
			if(!scope.is_empty()) scope = L"::" + scope;
			if(*gName == '?')
			{
				if(*++gName == '$') { gName--; scope = getZName() + scope; }
				else throw 1;
			}
			else scope = getZName() + scope;
		}
		if(*gName == '@') return scope;
		throw 1;
	}

	static String getScopedName()
	{
		String name(getZName());
		if(*gName && *gName != '@') name = getScope() + L"::" + name;
		if(*gName++ == '@') return name;
		throw 2;
	}

	static String getECSUDataType()
	{
		String ecsuDataType;
		static ssh_cws enumTypes[] = {L"char ",L"ssh_b ",L"short ",L"ssh_w ",L"int ",L"ssh_i ",L"long ",L"ssh_d "};
		switch(*gName++)
		{
			case BDT_union: ecsuDataType = L"union "; break;
			case BDT_struct: ecsuDataType = L"struct "; break;
			case BDT_class: ecsuDataType = L"class "; break;
			case BDT_enum: if(*gName >= '0' && *gName <= '7') { ecsuDataType = String(L"enum ") + enumTypes[*gName++ - '0']; break; }
			default: throw 1;
		}
		return ecsuDataType + getScopedName();
	}
	
	static int getDimension()
	{
		if(!*gName) throw 1;
		if((*gName >= '0') && (*gName <= '9')) return (int)(*gName++ - '0' + 1);
		int dim(0);
		while(*gName != '@')
		{
			if((*gName >= 'A') && (*gName <= 'P')) dim = (dim << 4) + (*gName - 'A'); else throw 1;
			gName++;
		}
		if(*gName++ != '@') throw 1;
		return dim;
	}
	
	static String getArrayType(const String& superType)
	{
		int noDimensions(getDimension());
		while(noDimensions--) countArray *= getDimension();
		return getPrimaryDataType(superType.is_empty() ? superType : L'(' + superType + L')');
	}

	static String getDataIndirectType(const String& superType, const String* cvType = nullptr, wchar_t prType = 0)
	{
		if(*gName)
		{
			ssh_i ditCode;;
			while(ditCode = (*gName - ((*gName >= 'A') ? (ssh_i)'A' : (ssh_i)('0' - 26))), ditCode == DIT_ptr64 || ditCode == DIT_restrict) gName++;
			gName++;
			if(ditCode <= DIT_member)
			{
				String ditType(prType, prType != 0);
				if(ditCode & DIT_member)
				{
					if(prType != '\0')
					{
						ditType = L"::" + ditType;
						if(*gName) ditType = getScope() + ditType; else throw 1;
					}
					else if(*gName) getScope();
					if(!*gName || *gName++ != '@') throw 1;
				}
				if((ditCode & DIT_modelmask) == DIT_based)
				{
					switch(*gName++)
					{
						case BT_void: break;
						case BT_nearptr: getScopedName(); break;
						default: throw 1;
					}
				}
				if(ditCode & DIT_const) ditType = L"const " + ditType;
				if(!superType.is_empty())
				{
					if(cvType)
					{
						if(cvType->is_empty()) ditType += L' ' + superType; else ditType += L' ' + *cvType + L' ' + superType;
					}
				}
				else if(cvType && !cvType->is_empty()) ditType += L' ' + *cvType;
				return ditType;
			}
		}
		throw 1;
	}

	static String getPtrRefType(const String& cvType, const String& superType, bool isPtr)
	{
		if(!*gName) throw 1;
		if(IT_isfunction(*gName)) throw 1;
		return getPtrRefDataType(getDataIndirectType(superType, &cvType, (isPtr ? L'*' : L'&')), isPtr);
	}

	static String getBasicDataType(const String& superType)
	{
		static ssh_cws dataTypes[] = {L"signed char", L"char", L"ssh_b", L"short", L"ssh_w", L"int", L"ssh_i", L"long", L"ssh_d", L"segment", L"float", L"double", L"long double"};
		static ssh_cws dataTypesEx[] = {L"char", L"ssh_b", L"short", L"ssh_w", L"int", L"ssh_i", L"ssh_l", L"ssh_u", L"__int128", L"__int128", L"bool"};

		if(!*gName) throw 1;
		ssh_b bdtCode(*gName++), bdtCodeEx;
		int pCvCode(-1);
		String basicDataType;
		switch(bdtCode)
		{
			case BDT_pointer:
			case (BDT_pointer | BDT_const):
			case (BDT_pointer | BDT_volatile):
			case (BDT_pointer | BDT_const | BDT_volatile): pCvCode = (bdtCode & (BDT_const | BDT_volatile)); break;
			case BDT_extend:
				bdtCodeEx = *gName++;
				if(bdtCodeEx == 'W')
					basicDataType = L"ssh_ws";
				else if(bdtCodeEx >= 'D' && bdtCodeEx <= 'N')
					basicDataType = dataTypesEx[bdtCodeEx - 'D'];
				else throw 1;
				break;
			default:
				if(bdtCode >= 'C' && bdtCode < 'P' && bdtCode != 'L')
					basicDataType = dataTypes[bdtCode - 'C'];
				else
				{
					gName--;
					basicDataType = getECSUDataType();
					if(basicDataType.is_empty()) return basicDataType;
				}
				break;
		}
		if(pCvCode == -1)
		{
			if(!superType.is_empty()) basicDataType += L' ' + superType;
			return	basicDataType;
		}
		String cvType;
		if(superType.is_empty() && pCvCode & BDT_const) cvType = L"const";
		return getPtrRefType(cvType, superType, true);
	}
	
	static String getPtrRefDataType(const String& superType, bool isPtr)
	{
		if(!*gName) throw 1;
		if(isPtr && *gName == PoDT_void) { gName++; return L"void" + (superType.is_empty() ? L'\0' : L' ' + superType); }
		if(*gName == RDT_array) { gName++; return getArrayType(superType); }
		return getBasicDataType(superType);
	}

	static String getPrimaryDataType(const String& superType)
	{
		String cvType;
		switch(*gName)
		{
			case 0: throw 1;
			case PDT_volatileReference:
			case PDT_reference: gName++; return getPtrRefType(cvType, superType, false);
			case PDT_extend:
				if(gName[1] != PDT_extend) throw 1;
				gName += 2;
				switch(*gName++)
				{
					case PDT_ex_other: return getPtrRefDataType(superType, true);
					case PDT_ex_qualified: return getBasicDataType(getDataIndirectType(superType));
					default: throw 1;
				}
		}
		return getBasicDataType(superType);
	}

	int SSH ssh_ext_count_array() { return countArray; }

	String SSH ssh_ext_exec(ssh_ccs str)
	{
		try
		{
			gName = str + 1;
			countArray = 1;
			zindex = 0;
			String superType;
			switch(*gName)
			{
				case 0: throw 1;
				case DT_void: return L"void";
				case '?': gName++; return getPrimaryDataType(getDataIndirectType(superType));
			}
			return getPrimaryDataType(superType);
		}
		catch(...) {}
		return L"";
	}

	ssh_u SSH ssh_ext_exec_hash(ssh_ccs str)
	{
		return ssh_hash(ssh_ext_exec(str));
	}
}
