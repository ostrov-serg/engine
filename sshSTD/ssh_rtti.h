
#pragma once

namespace ssh
{
	class SSH RTTI
	{
	public:
		RTTI() : next(rootRTTI) { rootRTTI = this; }
		// ������� ������
		virtual RTTI* createRTTI() = 0;
		// ������� ��� ������
		const String& className() const { return name; }
		// �������
		static RTTI* createClass(const String& nm)
		{
			RTTI* rtti(rootRTTI);
			while(rtti && rtti->className() != nm) rtti = rtti->next;
			return (rtti ? rtti->createRTTI() : nullptr);
		}
	protected:
		// ��� ������
		String name;
		// ��������� � ������
		RTTI* next;
		// ������ ������
		static RTTI* rootRTTI;
	};

	#define SSH_RTTI_DECL(cls)			class RTTI_##cls : public RTTI \
										{ \
										public: \
											RTTI_##cls() {name = L#cls;} \
											virtual RTTI* createRTTI() override {return reinterpret_cast<RTTI*>(new cls());} \
										}; \
										static RTTI_##cls cls##_rtti;
	#define SSH_RTTI_IMPL(cls)			cls::RTTI_##cls cls::cls##_rtti;
}
