#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CLevel;

class CLevelMgr final
{
public:
	CLevelMgr();
	~CLevelMgr() = default;

public:
	HRESULT Initialize();
	void Tick(_float _fTimeDelta);

	/*����� or Ȯ�ο� ��Ʈ ��¿�*/
	HRESULT Render();
	
	/*Funtion.�ε� ������ ���� ��ȯ ��ȣ�� ������ �غ��ϱ� ����*/
	HRESULT OpenLevel(_uint _iLevelIndex, shared_ptr<CLevel> _pLevel);
	HRESULT OpenLevelForClient(_uint _iLevelIndex);
	/*Funtion.�ε��� ��ģ �� �������� ���� ��ȯ*/
	HRESULT OpenLevel2(_uint _iLevelIndex, shared_ptr<CLevel> _pLevel);
	HRESULT StartLevel(shared_ptr<class CLevel> _pNextLevel);

public:
	_uint GetCurrentLevel() { return m_iLevelIndex; }
	void SetLoadingLevel(shared_ptr<class CLevel> _pLoadingLevel) { m_pLoadingLevel = _pLoadingLevel; }
	bool isLoadingLevel() { return m_IsLoadingLevel; }

public:
	void LevelBGMOn();
	void LevelBGMOff();
	
	void SwitchingBGM();
	void SwitchingBGM2();
	void SwitchingBGM3();

private:
	_uint m_iLevelIndex = { 0 };
	shared_ptr<class CLevel> m_pCurrentLevel = { nullptr };

private:
	/*�ε� ������ ������ �ʰ� �����մϴ�*/
	shared_ptr<class CLevel> m_pLoadingLevel = { nullptr };

private:
	_bool m_IsLoadingLevel = false;

public:
	static shared_ptr<CLevelMgr> Create();

};

END