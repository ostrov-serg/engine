
#pragma once

namespace ssh
{
	// ��������� �� ���������� sshREGX

	extern __regx_compile ssh_regx_compile;
	extern __regx_exec ssh_regx_exec;
	extern __regx_free ssh_regx_free;

	class SSH regx
	{
	public:
		// ����������� �� ���������
		regx() : result(0), re(nullptr) { memset(patterns, 0, sizeof(patterns)); memset(vector, 0, sizeof(vector)); }
		// ���������������� �����������
		regx(ssh_cws* pattern, ssh_u count) : regx()
		{
			ssh_u idx(0);
			while(idx < count) set_pattern(idx, pattern[idx]), idx++;
		}
		// ����������
		virtual ~regx()
		{
			if(ssh_regx_free)
			{
				if(re) ssh_regx_free(re);
				for(ssh_u i = 0; i < 32; i++)
					if(patterns[i]) ssh_regx_free(patterns[i]);
			}
		}
		// ��������� ������� � �������
		bool set_pattern(ssh_u idx, ssh_cws pattern)
		{
			if(idx < 32) return ((patterns[idx] = compile(pattern)) != nullptr);
			return false;
		}
		// ����� ���������� ��� ���������� ��������
		ssh_l match(ssh_cws subject, ssh_l idx, ssh_u idx_ptrn = -1)
		{
			subj = subject;
			return (result = ssh_regx_exec((idx_ptrn == -1 ? re : patterns[idx_ptrn]), subject, wcslen(subject), idx, 0, vector, 256));
		}
		// ����� ���������� � ����������� ��������
		ssh_l match(ssh_cws subject, ssh_cws pattern, ssh_l idx)
		{
			return ((re = compile(pattern)) ? match(subject, idx) : 0);
		}
		// ������� ��������� �� ����������� ��������� ��������
		String substr(ssh_l idx)
		{
			return subj.substr(vec(idx, 0), len(idx));
		}
		// �������� � ����������� ��������
		bool replace(String& subject, const String& pattern, ssh_cws repl, ssh_l idx)
		{
			return ((re = compile(pattern)) ? replace(subject, repl, idx, -1) : false);
		}
		// �������� ��� ���������� ��������
		bool replace(String& subject, ssh_cws repl, ssh_l idx, ssh_u idx_ptrn)
		{
			ssh_l nWcs(wcslen(repl));
			while(match(subject, idx_ptrn, idx) > 0)
			{
				idx = vector[0];
				subject.remove(idx, vector[1] - idx);
				subject.insert(idx, repl);
				idx += nWcs;
			}
			return true;
		}
		// ������� ���������� ��������� ����������
		ssh_l count() const { return result; }
		// ������� ������ � ������� ����������
		ssh_l vec(ssh_l idx, ssh_l offs) const { return (idx < result ? vector[idx * 2 + offs] : -1); }
		// ������� ����� � ������� ����������
		ssh_l len(ssh_l idx) const { return (idx < result ? (vector[idx * 2 + 1] - vector[idx * 2]) : 0); }
	protected:
		// �������������
		regex16* compile(ssh_cws pattern)
		{
			result = 0;
			if(re && ssh_regx_free) { ssh_regx_free(re); re = nullptr; }
			return (ssh_regx_compile ? ssh_regx_compile(pattern, 0) : nullptr);
		}
		// ������� ������
		String subj;
		// ��������� �������
		ssh_l vector[256];
		// ����� ���������
		ssh_l result;
		// ��������� �������
		regex16* re;
		// ����������������� ��������
		regex16* patterns[32];
	};
}
