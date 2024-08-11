#pragma once

#include "Tool_Defines.h"
#include "Engine_Defines.h"

namespace Tool_UI
{
	class CToolUIBase;

	class CToolUIMgr
	{
		DECLARE_SINGLETON(Tool_UI::CToolUIMgr)

	public:
		CToolUIMgr();
		~CToolUIMgr();

	public:
		HRESULT Initialize();
		void Tick(_float _fDeltaTime);

	public: /* Get & Set Function */
		void SetHUD(const string& _strUITag);

		void SetUISize(_float _fSizeX, _float _fSizeY);

		shared_ptr<CToolUIBase> GetLatestActivatedUI() { return m_pLatestActivatedUI.lock(); }

		/* ���� �ʿ� */
		function<void(void)> GetBindFunc(const string& _funcTag) { return m_bindFuncs[_funcTag]; }

		/* ���� */
		unordered_map<string, shared_ptr<CToolUIBase>>* GetAllUIs() { return &m_UIs; }
		vector<weak_ptr<CToolUIBase>>* GetAllActivatedUIs() { return &m_UIVec; }

	public:
		HRESULT AddUI(const string& _strUITag, shared_ptr<CToolUIBase> _pUI);
		HRESULT AddUIAfterSwapTag(const string& _strUITag, shared_ptr<CToolUIBase> _pUI);
		shared_ptr<CToolUIBase> FindUI(const string& _strUITag);

		HRESULT AddUIChild(const string& _strUITag, const string& _strChildTag);
		HRESULT AddUIParent(const string& _strUITag, const string& _strParentTag);
		HRESULT AddUIChild(const string& _strChildUITag);
		HRESULT AddUIParent(const string& _strParentTag);
		HRESULT AddUIInstanceChild(const string& _strUITag, const string& _strChildTag);
		HRESULT AddUIInstanceChild(const string& _strChildTag);

		HRESULT RemoveUIChild(const string& _strUITag, const string& _strChildUITag);

	public:
		void ActivateUI(const string& _strUITag);
		void DeactivateUI(const string& _strUITag);

		void SelectUI(const string& _strUITag);

		void DeleteSelectedUI(const string& _strUITag);

	public:
		void SortUIZOrder();
		void ResetUISelect();

	private:
		void KeyInput();

	private:
		HRESULT PrepareBindFuncs();
		void SortChildUIs(shared_ptr<CToolUIBase> _parentItemBG, shared_ptr<CToolUIBase> _childItemBG);

	public:
		static shared_ptr<CToolUIMgr> Create();

	private:
		unordered_map<string, shared_ptr<CToolUIBase>> m_UIs; // ��� UI�� ��Ƽ� ���� �����̳� -> ã�� �� ���� ������ ���� -> �������� �ȉ�
		vector<weak_ptr<CToolUIBase>> m_UIVec; // Ȱ��ȭ�� UI�鸸 ��Ƴ��� �����̳� -> Ȱ��ȭ �� UI�鸸 ���� �����ϱ� ���� ���� -> �߰��� ��������
		//list<weak_ptr<ToolUIBase>> _uiList; // Ȱ��ȭ�� UI�鸸 ��Ƴ��� �����̳� -> Ȱ��ȭ �� UI�鸸 ���� �����ϱ� ���� ���� -> �߰��� ��������

		shared_ptr<CToolUIBase> m_pHUD{ nullptr };
		weak_ptr<CToolUIBase> m_pLatestActivatedUI;

	private: /* Move UI */
		_float3 m_vDir{};
		_float m_fDist{ 0.f };

	private: /* Button */
		unordered_map<string, function<void(void)>> m_bindFuncs;

	private: /* Activate MouseSlot */
		_bool m_isMouseSlotActivated{ false };

	};
}
