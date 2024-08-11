#pragma once

#include "Tool_Defines.h"
#include "GameObject.h"
#include "VIInstancing.h"

BEGIN(Engine)

class CGameObject;
class CTexture;
class CVIInstancing;
class CVIInstancePoint;
class CVIInstanceUI;

END

BEGIN(Tool_UI)

class CToolUIBase abstract : public CGameObject
{
	using Super = CGameObject;

public:
	struct ToolUIDesc
	{
		string m_strTag;
		ETOOL_UI_LAYER m_eUILayer{ TOOL_UI_LAYER_END };
		ETOOL_UI_TYPE m_eUIType{ TOOL_UI_END };
		ETOOL_UI_TEX_TYPE m_eUITexType{ TOOL_UI_TEX_END };
		ETOOL_SLOT_TYPE m_eUISlotType{ TOOL_SLOT_END };
 		_bool m_isMasked{ false };
		_bool isNoised{ false };
		_uint iShaderPassIdx{ 0 };
		_float fZOrder{ 0.1f };
		_float fDiscardAlpha{ 0.f };
	};

public:
	CToolUIBase();
	virtual ~CToolUIBase();

public:
	virtual HRESULT Initialize(CTransform::TRANSFORM_DESC* _pTransformDesc) override;
	virtual void PriorityTick(_float _deltaTime) override;
	virtual void Tick(_float _deltaTime) override;
	virtual void LateTick(_float _deltaTime) override;
	virtual HRESULT Render() override;

public: /* Get & Set Function */
	string& GetUITag() { return m_ToolUIDesc.m_strTag; }
	void SetUITag(const string& _strTag) { m_ToolUIDesc.m_strTag = _strTag; }

	_float2 GetUIPos() { return _float2(m_fX, m_fY); }
	void SetUIPos(_float _fPosX, _float _fPosY); // UI�� ��ġ�� ����
	_float2 GetUISize() { return _float2(m_fSizeX, m_fSizeY); }
	void SetBufferSize(_float _fSizeX, _float _fSizeY); // UI�� ũ��(Scale)�� ����

	void SetColor(_float4 _vColor);
	_float4 GetColor() { return m_InstanceDesc.vColor; }

	_bool IsFinished();

	void SetIsSelected(_bool _isSelected) { m_isSelected = _isSelected; }
	_bool IsSelected() { return m_isSelected; } // �� UI�� ������ �Ǿ� �ִ���?

	_bool IsMouseOn(); // ���콺�� UI�ȿ� �ִ��� üũ

	void SetZOrder(_float _fZOrder) { m_ToolUIDesc.fZOrder = _fZOrder; }
	_float GetZOrder() { return m_ToolUIDesc.fZOrder; }
	_float GetSelectedZOrder() { return m_fSelectedZOrder; }

	void SetDiscardAlpha(_float _fDiscardAlpha) { m_ToolUIDesc.fDiscardAlpha = _fDiscardAlpha; }
	_float GetDiscardAlpha() { return m_ToolUIDesc.fDiscardAlpha; }

	void SetBindWorldMat(_float4x4 _worldMat) { m_BindWorldMat = _worldMat; }

	void SetTexIndex(_uint _texIndex) { m_iTexIndex = _texIndex; }

	_bool IsActivated() { return m_isActivated; } // UI�� Ȱ��ȭ �Ǿ� �ִ��� üũ

	ETOOL_UI_TYPE GetUIType() { return m_ToolUIDesc.m_eUIType; } // UI�� Ÿ���� ��ȯ
	ETOOL_SLOT_TYPE GetSlotType() { return m_ToolUIDesc.m_eUISlotType; } // UIŸ���� ������ ��� ������ Ÿ���� ��ȯ

	shared_ptr<CGameObject> GetUIOwner();
	void SetUIOwner(shared_ptr<CGameObject> _pGameObject) { m_pOwner = _pGameObject; }

	_bool IsChild(); // �ڽİ�ü���� üũ

	unordered_map<string, weak_ptr<CToolUIBase>>& GetChildrenList() { return m_pUIChildren; }
	vector<weak_ptr<CToolUIBase>>& GetChildrenListVec() { return m_pUIChildrenVec; }

	void SwapTextures(vector<string>& _swapTexTags) { m_strTexKeys.swap(_swapTexTags); }
	
	/* �Ľ̿� */
	CVIInstancing::InstanceDesc& GetInstanceDesc() { return m_InstanceDesc; }
	ToolUIDesc& GetToolUIDesc() { return m_ToolUIDesc; }
	unordered_map<string, weak_ptr<CToolUIBase>>& GetUIChildren() { return m_pUIChildren; }
	vector<weak_ptr<CToolUIBase>>& GetUIChildrenVec() { return m_pUIChildrenVec; }
	shared_ptr<CToolUIBase> GetUIParent();
	vector<string>& GetTexKeys() { return m_strTexKeys; }
	string& GetMaskKey() { return m_strMaskKey; }
	string& GetNoiseKey() { return m_strNoiseKey; }
	vector<_float4>& GetAtlasUVs() { return m_UVs; }
	_uint GetShaderPass() { return m_iShaderPass; }

	void SetShaderPassIndex(_uint _passIndex);

	vector<_float2>& GetInstanceUIPos() { return m_vUIPos; }
	vector<_float2>& GetInstanceUISize() { return m_vUISize; }

public:
	void AddUIChild(const string& _strUITag, shared_ptr<CToolUIBase> _pUIChild); // UI�� �ڽ� UI�� �߰�
	void AddUIChild(shared_ptr<CToolUIBase> _pUIChild);
	void AddUIParent(shared_ptr<CToolUIBase> _uiParent); // �� UI�� �θ� UI�� ����

	void RemoveUIChild(const string& _strUIChildTag);
	void RemoveUIChildrenVec();
	void RemoveAllUIChild();
	void RemoveUIParent();

public:
	void RotateUI(_float _xValue, _float _yValue, _float _zValue);

public:
	void ActivateUI(); // Ư�� ���ǿ����� UI�� �۵��ؾ� �� ��� Ȱ��
	void DeactivateUI(); // Ư�� ���ǿ����� UI�� �۵��ؾ� �� ��� Ȱ��

	/* ���İ��� Ȱ���� UI�� ��Ÿ���ų� ������� ȿ���� �ְ� ���� ��� */
	void ResetInstanceData(); // ���İ��� �ʱ�ȭ

public: /* Slider */
	void ConvertMousePosToUV(_float _mouseX, _float _mouseY);

public:
	void PrepareUVs(_float _fX, _float _fY);

protected:
	void KeyInput();

protected:
	/*string m_strTag;
	ETOOL_UI_LAYER m_eUILayer{ TOOL_UI_LAYER_END };
	ETOOL_UI_TYPE m_eUIType{ TOOL_UI_END };
	ETOOL_UI_TEX_TYPE m_eUITexType{ TOOL_UI_TEX_END };
	_bool m_isMasked{ false };*/
	_uint m_iTexIndex{ 0 };

protected:
	vector<string> m_strTexKeys;
	string m_strMaskKey;
	string m_strNoiseKey;
	shared_ptr<CVIInstancePoint> m_pBuffer{ nullptr };

protected:
	weak_ptr<CToolUIBase> m_pUIParent;
	unordered_map<string, weak_ptr<CToolUIBase>> m_pUIChildren;
	vector<weak_ptr<CToolUIBase>> m_pUIChildrenVec;

	//_float3 m_vLocalPos{}; // �θ� �������� �� ��� ��ġ

protected:
	weak_ptr<CGameObject> m_pOwner;

protected:
	vector<_float4> m_UVs;
	_uint m_iUVIndex{ 0 };

protected:
	_bool m_isActivated{ false };
	_bool m_isSelected{ false };

protected:
	_float m_fX{ 0.f }, m_fY{ 0.f }, m_fSizeX{ 0.f }, m_fSizeY{ 0.f };

	_float m_fSelectedZOrder{ 0.f };
	_float4x4 m_ViewMat{}, m_ProjMat{}, m_BindWorldMat{};
	_float4x4 m_RotationMat{};

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

protected: /* �Ľ� �� */
	CVIInstancing::InstanceDesc m_InstanceDesc{};
	ToolUIDesc m_ToolUIDesc{};
	//InstanceUI m_InstanceUIDesc{};

protected: /* �ν��Ͻ� �� */
	shared_ptr<CVIInstanceUI> m_pInstanceBuffer{ nullptr };
	vector<_float2> m_vUIPos;
	vector<_float2> m_vUISize;

};

END
