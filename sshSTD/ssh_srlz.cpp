
#include "stdafx.h"
#include "ssh_srlz.h"

namespace ssh
{
	static Serialize::SCHEME* _sc(nullptr);

	void ssh::Serialize::open(const Buffer & buf, bool is_xml)
	{
		_sc = get_scheme();
		if(is_xml)
		{
			Xml xml(buf);
			readXml(xml.root(), &xml, 0, 0);
		}
		else
		{
			readBin(buf.to<ssh_b*>(), 0);
		}
	}

	void ssh::Serialize::save(const String & path, bool is_xml)
	{
		_sc = get_scheme();
		if(is_xml)
		{
			Xml xml;
			writeXml(xml.root(), &xml, 0);
			xml.save(path);
		}
		else
		{
			File f(path, File::create_write);
			writeBin(&f, 0);
		}
	}

	void ssh::Serialize::readXml(HXML hp, Xml* xml, ssh_l p_offs, ssh_l idx)
	{
		/*
		SCHEME* sc;
		HXML h;
		String sval;
		// получить узел
		if(!(h = _xml->node(hp, (idx ? nullptr : _sc->name), idx)))
		{
			SSH_LOG(L"Ќе найден узел <%s, индекс: %i> xml!", _sc->name, idx);
			return;
		}
		ssh_u _ID(_sc++->ID);
		ssh_l i;
		while((sc = _sc++))
		{
			if(!sc->name) break;
			ssh_u flg(sc->flags);
			ssh_l offs(sc->offs + p_offs);
			ssh_l count(sc->count);
			ssh_u val, pos(0);
			ssh_ws* _ws;
			if((flg & SC_VAR))
			{
				sval = _xml->attr<String>(h, sc->name, sc->def);
				if((flg & SC_BASE64)) sval = ssh_base64(sval, true).to<ssh_ws>();
				_ws = sval.buffer();
			}
			for(i = 0; i < count; i++)
			{
				if(_ID)
				{
					if(!(flg & SC_OBJ) || sc->ID == -1) { if(sc->ID != -1) _sc--; return; }
					if(sc->ID != _ID) { _sc--; readXml(h, offs, i); if((i + 1) < count) _sc = sc + 1; }
				}
				else if((flg & SC_OBJ)) { _sc--; readXml(h, offs, i); if((i + 1) < count) _sc = sc + 1; }
				else if((flg & SC_NODE)) { Serialize* srlz((Serialize*)((ssh_b*)(this) + offs)); _sc = srlz->get_scheme(); srlz->readXml(h, 0, i); _sc = sc + 1; }
				if((flg & SC_VAR))
				{
					ssh_ws* _nws(nullptr);
					ssh_b* obj((ssh_b*)(this) + offs);
					if(*_ws && count > 1)
					{
						pos = (_ws - sval.buffer());
						if((_nws = wcschr(_ws, L','))) *_nws = 0;
					}
					switch(sc->hash)
					{
						case _hash_wcs:
						case _hash_ccs: break;
						case _hash_string: *(String*)obj = _ws; break;
						case _hash_char: *(ssh_cs*)obj = *ssh_cnv(cp_ansi, _ws, false).to<ssh_cs>(); break;
						case _hash_wchar: *(ssh_ws*)obj = *_ws; break;
						case _hash_float: *(float*)obj = sval.toNum<float>(pos, String::_flt); break;
						case _hash_double: *(double*)obj = sval.toNum<double>(pos, String::_dbl); break;
						default:
							if(sc->width <= 8)
							{
								switch(flg & 15)
								{
									case SC_ENUM:
									case SC_FLAGS: val = ssh_cnv_value(_ws, sc->stk, String(sc->def)); break;
									case SC_BOOL: val = (wcscmp(_ws, L"true") == 0); break;
									default: val = sval.toNum<ssh_u>(pos, (String::Radix)(flg & 3)); break;
								}
								memcpy(obj, &val, sc->width);
							}
							break;
					}
					if(_nws) _ws = _nws + 1;
					else if(*_ws) pos = sval.length(), _ws = (sval.buffer() + pos);
				}
				offs += sc->width;
			}
		}
		*/
	}

	void ssh::Serialize::writeXml(HXML hp, Xml* xml, ssh_l p_offs)
	{
		// создать узел
		HXML h(xml->add_node(hp, _sc->name, _sc->def));
		_sc++;
		while(_sc->name)
		{
			SCHEME* sc(_sc);
			ssh_u offs(sc->offs + p_offs);
			ssh_u opt(sc->opt);
			ssh_u count(sc->count);
			ssh_u width(sc->width / count);
			String val;
			while(count--)
			{
				if(opt & SC_NODE)
				{
					if(opt & SC_OBJ)
					{
						Serialize* srlz((Serialize*)((ssh_b*)(this) + offs));
						_sc = srlz->get_scheme();
						srlz->writeXml(h, xml, 0);
						if(count) _sc = sc - 1; else _sc = sc + 1;
					}
					else
					{
						writeXml(h, xml, offs);
						if(count) _sc = sc;
					}
				}
				else
				{
					if(!(opt & (SC_CV | SC_PTR)))
					{
						if(!val.is_empty() && !(opt & SC_LIT)) val += L',';
						ssh_b* obj((ssh_b*)(this) + offs);

						if(opt & SC_FLT) val += String(*(ssh_u*)obj, (sc->width == 4 ? Radix::_flt : Radix::_dbl));
						else if(opt & SC_BOOL) val += String(*(ssh_u*)obj, Radix::_bool);
						else if(opt & SC_STR) val += *(String*)obj;
						else if(opt & SC_LIT) { if(*obj) val += (width == 2 ? *(ssh_ws*)obj : *(ssh_cs*)obj); }
						else
						{
							ssh_u lval(0);
							memcpy(&lval, obj, width);
							val += (sc->stk ? ssh_implode2(lval, sc->stk, sc->def, !(opt & SC_FLGS)) : String(lval, (Radix)(opt & 3)));
						}
					}
				}
				offs += width;
			}
			if(!(opt & (SC_CV | SC_PTR | SC_NODE))) xml->add_attr(h, sc->name, (opt & SC_BASE64) ? ssh_base64(L"utf-16le", val) : val);
			_sc++;
		}
	}

	void ssh::Serialize::writeBin(File* f, ssh_l p_offs)
	{
		_sc++;
		while(_sc->name)
		{
			SCHEME* sc(_sc);
			ssh_u offs(sc->offs + p_offs);
			ssh_u opt(sc->opt);
			ssh_u count(sc->count);
			ssh_u width(sc->width / count);
			while(count--)
			{
				if(opt & SC_NODE)
				{
					if(opt & SC_OBJ)
					{
						Serialize* srlz((Serialize*)((ssh_b*)(this) + offs));
						_sc = srlz->get_scheme();
						srlz->writeBin(f, 0);
						if(count) _sc = sc - 1; else _sc = sc + 1;
					}
					else
					{
						writeBin(f, offs);
						if(count) _sc = sc;
					}
				}
				else
				{
					if(!(opt & (SC_CV | SC_PTR)))
					{
						ssh_b* obj((ssh_b*)(this) + offs);
						if(opt & SC_STR) f->write(((void*)*(ssh_u*)obj), ((*(String*)obj).length() + 1) * 2); else f->write(obj, width);
					}
				}
				offs += width;
			}
			_sc++;
		}
	}

	void ssh::Serialize::readBin(ssh_b* buf, ssh_l p_offs)
	{
		/*
		SCHEME* sc;
		ssh_u _ID(_sc++->ID);
		while((sc = _sc++))
		{
			if(!sc->name) break;
			ssh_u flg(sc->flags);
			ssh_l offs(sc->offs + p_offs);
			ssh_l count(sc->count);
			while(count--)
			{
				if(_ID)
				{
					if(!(flg & SC_OBJ) || sc->ID == -1) { if(sc->ID != -1) _sc--; return; }
					if(sc->ID != _ID) { _sc--; readBin(offs); if(count) _sc = sc + 1; }
				}
				else if((flg & SC_OBJ)) { _sc--; readBin(offs); if(count) _sc = sc + 1; }
				else if((flg & SC_NODE)) { Serialize* srlz((Serialize*)((ssh_b*)(this) + offs)); _sc = srlz->get_scheme(); srlz->readBin(0); _sc = sc + 1; }
				if((flg & SC_VAR))
				{
					ssh_b* obj((ssh_b*)(this) + offs);
					switch(sc->hash)
					{
						case _hash_wcs:
						case _hash_ccs: break;
						case _hash_string: *(String*)obj = (ssh_ws*)_buf; _buf += (wcslen((ssh_ws*)_buf) * 2 + 2); break;
						default: memcpy(obj, _buf, sc->width); _buf += sc->width; break;
					}
				}
				offs += sc->width;
			}
		}
		*/
	}
}
