#pragma once

#include "UIBase.h"

BEGIN(Engine)

class CVIInstancing;

END

BEGIN(Client)

class CUIInstanceQuest : public CUIBase
{
	using Super = CUIBase;

public:
	CUIInstanceQuest();
	virtual ~CUIInstanceQuest();

public:
	virtual HRESULT Initialize(vector<string>& _strTextureTags, const string& _strMaskTextureTag, const string& _strNoiseTextureTag, CUIBase::UIDesc& _UIDesc, CVIInstancing::InstanceDesc& _InstanceDesc, vector<_float2>& _vUIPos, vector<_float2>& _vUISize);
	virtual void	PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void UpdateQuestContent(QUEST_TYPE _questType, const wstring& _questTitle, const wstring& _questContent, const string& _strFont = "Font_AdenL10", _float4 _vTitleFontColor = _float4(0.f, 0.f, 0.f, 0.f), _float4 _vContentFontColor = _float4(0.f, 0.f, 0.f, 0.f));
	void UpdateQuestContentNonAuto(QUEST_TYPE _questType, const wstring& _questTitle, const wstring& _questContent, const string& _strFont = "Font_AdenL10", _float4 _vTitleFontColor = _float4(0.f, 0.f, 0.f, 0.f), _float4 _vContentFontColor = _float4(0.f, 0.f, 0.f, 0.f));
	void AppendQuestContent(QUEST_TYPE _questType, const wstring& _questContent);

private:
	void UpdateTexture();
	void QuestKeyInput();

private:
	HRESULT AddComponent(vector<string>& _strTextureTags, const string& _strMaskTextureTag, const string& _strNoiseTextureTag, CVIInstancing::InstanceDesc& _InstanceDesc, vector<_float2>& _vUIPos, vector<_float2>& _vUISize);

public:
	static shared_ptr<CUIInstanceQuest> Create(vector<string>& _strTextureTags, const string& _strMaskTextureTag, const string& _strNoiseTextureTag, CUIBase::UIDesc& _UIDesc, CVIInstancing::InstanceDesc& _InstanceDesc, vector<_float2>& _vUIPos, vector<_float2>& _vUISize);

private:
	_float m_fTimeAcc{ 0.f };

private:
	_uint m_iInstanceRenderState{ 0 };

private:
	vector<_float2> m_vSlotUVRatio;

private:
	weak_ptr<CCustomFont> m_Font;

private:
	// �� �ٿ� ���� ���� 22
	// �� ���� �Ѱ� �� 42 -> \n�� �����ؾ� �ǹǷ�
	wstring m_strPanelText{ L"����Ʈ" }; // Pos6

	//wstring m_strMainQuestTitle{ L"���ױ� �Ƿ�" }; // 9
	//wstring m_strMainQuestContent{ L"ĳ���� ������ �ؾ� ���� �Ʒ����� �ǹ���\n���� Ȯ��" }; // 10

	//wstring m_strSubQuestTitle0{ L"���ױ� �Ƿ�" }; // 14
	//wstring m_strSubQuestContent0{ L"ĳ���� ������ �ؾ� ���� �Ʒ����� �ǹ���\n���� Ȯ��" }; // 15
	//wstring m_strSubQuestTitle1{ L"���ױ� �Ƿ�" }; // 17
	//wstring m_strSubQuestContent1{ L"ĳ���� ������ �ؾ� ���� �Ʒ����� �ǹ���\n���� Ȯ��" }; // 18

	wstring m_strMainQuestTitle{ L"" }; // 9
	wstring m_strMainQuestContent{ L"" }; // 10

	wstring m_strSubQuestTitle0{ L"" }; // 14
	wstring m_strSubQuestContent0{ L"" }; // 15
	wstring m_strSubQuestTitle1{ L"" }; // 17
	wstring m_strSubQuestContent1{ L"" }; // 18

	/* Font Tag */
	string m_strMainQuestFont{ "Font_AdenL10" };
	string m_strSubQuestFont0{ "Font_AdenL10" };
	string m_strSubQuestFont1{ "Font_AdenL10" };

	/* Font Color */
	_float4 m_vMainTitleFontColor{ 1.f, 1.f, 1.f, 1.f };
	_float4 m_vMainContentFontColor{ 1.f, 1.f, 1.f, 1.f };
	_float4 m_vSubTitleFontColor0{ 1.f, 1.f, 1.f, 1.f };
	_float4 m_vSubContentFontColor0{ 1.f, 1.f, 1.f, 1.f };
	_float4 m_vSubTitleFontColor1{ 1.f, 1.f, 1.f, 1.f };
	_float4 m_vSubContentFontColor1{ 1.f, 1.f, 1.f, 1.f };

};

END
