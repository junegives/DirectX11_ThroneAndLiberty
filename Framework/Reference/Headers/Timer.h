#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CTimer
{
public:
	CTimer();
	~CTimer() = default;

public:
	HRESULT Initialize(void);
	_float ComputeTimeDelta(void);

public:
	/*------------------------------
	_fOffset : ���ο� ��� ����ġ
	_fDuration : ���ο� ����� ����� �ð�
	--------------------------------*/
	void SetTimeOffset(_float _fOffset , _float _fDuration) { 
		m_fTimeOffset = _fOffset;
		m_fSlowDuration = _fDuration;
		m_fSlowAccTime = 0.f;
		m_IsSlowMotion = true;
	}

private:
	/*ī�޶� DeltaTime ����ġ(SlowMotion�� �ʿ��� �� ���)*/
	_float m_fTimeOffset = 1.f;
	_float m_fSlowDuration = 0.f;
	_float m_fSlowAccTime = 0.f;
	_bool m_IsSlowMotion = false;

private:
	LARGE_INTEGER m_FrameTime;
	LARGE_INTEGER m_FixTime;
	LARGE_INTEGER m_LastTime;
	LARGE_INTEGER m_CpuTick;

	_float m_fTimeDelta = 0.f;

public:
	static shared_ptr<CTimer> Create(void);

};

END
