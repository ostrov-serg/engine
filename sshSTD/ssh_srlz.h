
#pragma once

// �����
#define SC_BIN			0x0001// ������ � bin
#define SC_OCT			0x0002// ������ � oct
#define SC_HEX			0x0003// ������ � hex
#define SC_FLGS			0x0004// �����
// �����������
#define SC_NODE			0x0008// ����
#define SC_PTR			0x0010// ���������
#define SC_FLT			0x0020// float, double
#define SC_BOOL			0x0040// bool
#define SC_SC			0x0080// �� ������
#define SC_STR			0x0100// ������


#define SCHEME_SPEC(cls, var, flg)	(std::is_pointer<SSH_PURE_TYPE1_D(cls::var)>() * SC_PTR) | (std::is_floating_point<SSH_PURE_TYPE2_D(cls::var)>() * SC_FLT) | \
									(SSH_IS_STRING_D(cls::var) * SC_STR) | (SSH_IS_BOOL_D(cls::var) * SC_BOOL) | (std::is_base_of<Serialize, decltype(cls::var)>() * SC_SC) | flg, \
									ssh_count_array<decltype(cls::var)>(), sizeof(decltype(cls::var))

#define SCHEME_BEGIN(cls)			static SCHEME cls##_scheme[] = {
#define SCHEME_END(cls)				{nullptr, nullptr, nullptr, 0, 0, 0, 0}}; return cls##_scheme;

#define SCHEME_VAR(cls, var, def, flgs, stk) {L#var, def, stk, offsetof(cls, var), SCHEME_SPEC(cls, var, flgs) },

#define SCHEME_NODE_BEGIN(cls, var) {L#var, nullptr, nullptr, offsetof(cls, var), SCHEME_SPEC(cls, var, SC_NODE) },
#define SCHEME_NODE_END()			{nullptr, nullptr, nullptr, 0, 0, 0, 0},


namespace ssh
{
	template<class _Ty> struct ssh_count_array : std::integral_constant<int, 1> {};
	template<class _Ty, size_t _Nx> struct ssh_count_array<_Ty[_Nx]> : std::integral_constant<int, _Nx> {};

	class SSH Serialize
	{
	public:
		struct SCHEME
		{
			// ��� (��� ������� - �������, ��� ��������� - ����)
			ssh_cws name;
			// �������� �� ���������
			ssh_cws def;
			// ��������� ��� �������������� ������ � ������������
			int* stk;
			// �������� ���������� ������������ ��������
			ssh_u offs;
			// ������������ ����
			ssh_w flags;
			// ���������� ��������� � ����������
			ssh_w count;
			// ������ ���� � ������
			ssh_w width;
		};

		Serialize() {}
		virtual ~Serialize() {}
		virtual SCHEME* get_scheme() const = 0;
		virtual void open(const buffer& buf, void* srlz, bool is_xml);
		virtual void save(const String& path, void* srlz, bool is_xml);
	protected:
		//virtual void readXml(HXML hp, ssh_l p_offs, ssh_l idx = 0);
		//virtual void writeXml(HXML h, ssh_l p_offs);
		virtual void writeBin(ssh_l p_offs);
		virtual void readBin(ssh_l p_offs);
	};
}