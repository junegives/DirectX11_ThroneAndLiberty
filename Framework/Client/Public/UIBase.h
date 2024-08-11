#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "VIInstancing.h"

BEGIN(Engine)

class CGameObject;
class CVIInstancePoint;
class CVIInstanceUI;

END

BEGIN(Client)

class CUIBase abstract : public Engine::CGameObject
{
	using Super = CGameObject;

public:
	struct UIDesc
	{
		string strTag;
		UI_LAYER eUILayer{ UI_LAYER_END };
		UI_TYPE eUIType{ UI_END };
		UI_TEX_TYPE eUITexType{ UI_TEX_END };
		UI_SLOT_TYPE eUISlotType{ SLOT_END };
		_bool isMasked{ false };
		_bool isNoised{ false };
		_uint iShaderPassIdx{ 0 };
		_float fZOrder{ 0.f };
		_float fDiscardAlpha{ 0.f };
	};

public:
	CUIBase();
	virtual ~CUIBase();

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _deltaTime) override;
	virtual void Tick(_float _deltaTime) override;
	virtual void LateTick(_float _deltaTime) override;
	virtual HRESULT Render() override;

public: /* Get & Set Function */
	string& GetUITag() { return m_UIDesc.strTag; } // UI�� Tag�� ��ȯ, UIManager���� UI�� ã�ų� �� �� ���δ�
	void SetUITag(const string& _strTag) { m_UIDesc.strTag = _strTag; }

	_float2 GetUIPos() { return _float2(m_fX, m_fY); } // UI�� ��ġ�� ��ȯ
	void SetUIPos(_float _fPosX, _float _fPosY); // UI�� ��ġ�� ����

	_float2 GetUISize() { return _float2(m_fSizeX, m_fSizeY); } // UI�� ũ�⸦ ��ȯ
	void SetBufferSize(_float _fSizeX, _float _fSizeY); // UI�� ũ��(Scale)�� ����

	_bool IsFinished(); // UI�� �ִϸ��̼��� ��������? (�̱���)

	void SetIsSelected(_bool _isSelected) { m_isSelected = _isSelected; }
	_bool IsSelected() { return m_isSelected; } // �� UI�� ������ �Ǿ� �ִ���?

	_bool IsMouseOn(); // ���콺�� UI�ȿ� �ִ��� üũ
	_int IsInstanceMouseOn();
	_bool IsInstanceMouseOn(_uint _iIndex);

	void SetZOrder(_float _fZOrder) { m_UIDesc.fZOrder = _fZOrder; }
	_float GetZOrder() { return m_UIDesc.fZOrder; } // UI�� ZOrder�� ��ȯ
	_float GetSelectedZOrder() { return m_fSelectedZOrder; } // UI�� ���õǾ� ���� ��� �̰� ��ȯ(������ ���� �տ� �ְ�)

	void SetDiscardAlpha(_float _fDiscardAlpha) { m_UIDesc.fDiscardAlpha = _fDiscardAlpha; }

	void SetBindWorldMat(_float4x4 _worldMat) { m_BindWorldMat = _worldMat; } // ������� ���� �Ⱦ���(���� ���� ���ɼ� ����)

	void SetTexIndex(_uint _texIndex) { m_iTexIndex = _texIndex; } // Texture�� ���� �� ���� ��� ��Ȳ�� ���� �ٲ��� �� �ִ�
	_uint GetTexIndex() { return m_iTexIndex; } // ���� TextureIndex�� ��ȯ

	_bool IsActivated() { return m_isActivated; } // UI�� Ȱ��ȭ �Ǿ� �ִ��� üũ

	UI_TYPE GetUIType() { return m_UIDesc.eUIType; } // UI�� Ÿ���� ��ȯ
	UI_SLOT_TYPE GetSlotType() { return m_UIDesc.eUISlotType; }

	shared_ptr<CGameObject> GetUIOwner(); // �� UI�� ���� ��ü�� ��ȯ
	void SetUIOwner(shared_ptr<CGameObject> _pGameObject) { m_pOwner = _pGameObject; }

	_bool IsChild(); // �ڽİ�ü���� üũ

	unordered_map<string, weak_ptr<CUIBase>>& GetChildrenList() { return m_pUIChildren; }

	void SwapTextures(vector<string>& _swapTexTags) { m_strTexKeys.swap(_swapTexTags); }

	vector<string>& GetTexKeys() { return m_strTexKeys; }

	void SetIsWorldUI(_bool _isWorldUI) { m_isWorldUI = _isWorldUI; }

	void SetNonResetRender(_bool _isRender);
	void SetIsRender(_bool _isRender);
	_bool IsRender() { return m_isRender; }

	void SetIsPickable(_bool _isPickable) { m_isPickable = _isPickable; }
	_bool IsPickable() { return m_isPickable; }

	void SetIsLoadingDone(_bool _isLoadingDone) { m_isLoadingDone = _isLoadingDone; }
	_bool IsLoadingDone() { return m_isLoadingDone; }

public:
	void AddUIChild(const string& _strUITag, shared_ptr<CUIBase> _pUIChild); // UI�� �ڽ� UI�� �߰�
	void AddUIParent(shared_ptr<CUIBase> _uiParent); // �� UI�� �θ� UI�� ����

	void RemoveUIChild(const string& _strUIChildTag); // Ư�� �ڽ� UI�� ����(�̱���)
	void RemoveAllUIChild(); // �ڽ� UI���� ���� ����(�̱���)
	void RemoveUIParent(); // �θ� UI�� ����(�̱���)

	void SaveAllChildren(vector<shared_ptr<CUIBase>>& _outAllChildren);

public:
	void RotateUI(_float _xValue, _float _yValue, _float _zValue); // UI�� ȸ�� -> Z�ุ ����

public:
	void ActivateUI(); // Ư�� ���ǿ����� UI�� �۵��ؾ� �� ��� Ȱ��
	void DeactivateUI(); // Ư�� ���ǿ����� UI�� �۵��ؾ� �� ��� Ȱ��

	/* ���İ��� Ȱ���� UI�� ��Ÿ���ų� ������� ȿ���� �ְ� ���� ��� */
	void ResetInstanceData(); // ���İ��� �ʱ�ȭ

public: /* Slider */
	void ConvertMousePosToUV(_float _mouseX, _float _mouseY);

public:
	void PrepareUVs(_float _fX, _float _fY); // �ؽ��� �̹����� ��Ʋ���� ��� UV�� ���缭 �����صд�

protected:
	void KeyInput();

protected:
	UIDesc m_UIDesc{};
	_uint m_iTexIndex{ 0 };

protected:
	vector<string> m_strTexKeys;
	string m_strMaskKey;
	string m_strNoiseKey;
	shared_ptr<CVIInstancePoint> m_pBuffer{ nullptr };
	shared_ptr<CVIInstanceUI> m_pInstanceBuffer{ nullptr };

protected:
	weak_ptr<CUIBase> m_pUIParent;
	unordered_map<string, weak_ptr<CUIBase>> m_pUIChildren;

	//_float3 m_vLocalPos{}; // �θ� �������� �� ��� ��ġ

protected: /* �� UI�� �����ϴ� ��ü */
	weak_ptr<CGameObject> m_pOwner;

protected:
	vector<_float4> m_UVs;
	_uint m_iUVIndex{ 0 };

protected:
	_bool m_isActivated{ false };
	_bool m_isSelected{ false };

protected:
	_float m_fX{ 0.f }, m_fY{ 0.f }, m_fSizeX{ 0.f }, m_fSizeY{ 0.f };
	_float4x4 m_ViewMat{}, m_ProjMat{}, m_BindWorldMat{};
	_float4x4 m_RotationMat{};

protected: /* ZOrder */
	_float m_fSelectedZOrder{ 0.0f };

protected: /* �ڽ� UI�� �� ��ġ ���� */
	_float3 m_vDir{};
	_float m_fDist{ 0.f };

	_float3 m_vPrevParentPos{ FLT_MAX, FLT_MAX, FLT_MAX };

protected: /* Button */
	function<void(void)> m_activeFunc;

protected: /* Slider */
	_float2 m_uvMouse{ 0.5f, 0.5f };
	_float m_fUVRadius{ 0.f };

protected:
	_float m_fTimeAcc{ 0.f };

protected:
	_bool m_isHovered{ false };
	_bool m_isRender{ true };
	_bool m_isPickable{ false };

protected: /* Set World UI */
	_bool m_isWorldUI{ false };
	_bool m_isOnce{ true };

protected: /* �ν��Ͻ� �� */
	vector<_float2> m_vUIPos;
	vector<_float2> m_vUISize;

protected:
	_bool m_isLoadingDone{ true };

};

END
