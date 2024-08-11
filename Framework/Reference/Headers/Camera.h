#pragma once

#include "GameObject.h"


BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	enum EAXIS { PITCH, YAW, ROLL, AXIS_END };

public:
	struct CAMERA_DESC
	{
		_float3		vEye = { 0.0f,0.0f,0.0f }, vAt = { 0.0f,0.0f,1.0f };
		_float		fFovy = 0.0f;
		_float		fNear = 0.1f;
		_float		fFar = 1000.0f;
		_float		fAspect = 0.0f;
	};

public:
	CCamera();
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize(CAMERA_DESC* _pCamDes, CTransform::TRANSFORM_DESC* _TransDesc = nullptr);
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_float3 GetEye() { return m_pTransformCom->GetState(CTransform::STATE_POSITION); }
	_float3 GetAt() { return m_vCamAt; }
	void LerpEyeAt(float _fTime, float _fWeight, const _float3& _vStartEye, const _float3& _vStartAt, const _float3& _vEndEye, const _float3& _vEndAt);
	void LerpFovy(float _fFovy, float _fTime);
	virtual void SetEyeAt(const _float3& _vEye, const _float3& _vAt);
	void SetFovEyeAt(float _fFov, const _float3& _vEye, const _float3& _vAt);
	void SwitchingRotationLock() { m_IsRotationLock = !m_IsRotationLock; }
	void SwitchingMouseLock() { m_IsMouseLock = !m_IsMouseLock; }
	bool GetMouseLock() { return m_IsMouseLock; }
	void SetMouseLock(bool _isLock) { m_IsMouseLock = _isLock; }
	void MouseLockOn() { m_IsMouseLock = true; }
	void MouseLockOff() { m_IsMouseLock = false; }
	virtual void CameraTurnOn();
	virtual void CameraTurnOff();
	bool isRecording();

public:
	bool isCamShake();
	void CamShake(UINT _eShakeFlags, float _fDuration, ROT_DESC* _pRotDesc, LOC_DESC* _pLocDesc, FOV_DESC* _pFovDesc);
	void CamShakeRot(ESHAKEFUNC _eShakeFunc, float _fDuration, const _float3& _vRotAmplitude, const _float3& _vRotFrequency, bool _isOffSetRand = true, _float2 _vBlendInOut = { 0.0f ,0.0f }, _float2 _vInOutWeight = { 1.0f,1.0f });
	void CamShakeLoc(ESHAKEFUNC _eShakeFunc, float _fDuration, const _float3& _vLocAmplitude, const _float3& _vLocFrequency, bool _isOffSetRand = true, _float2 _vBlendInOut = { 0.0f ,0.0f }, _float2 _vInOutWeight = { 1.0f,1.0f });
	void CamShakeFov(ESHAKEFUNC _eShakeFunc, float _fDuration, float _fFovyAmplitude, float _fFovyFrequency, _float2 _vBlendInOut = { 0.0f ,0.0f }, _float2 _vInOutWeight = { 1.0f,1.0f });
	void CamShakeStop();

protected:

	HRESULT SetUpTransformMatices();

	/*Create Cascade Frustum*/
	HRESULT CreateLightFrustum();
	/*Cascade Camera�� �ʿ��� ����ü ������ ����ϰ� pipeline�� �Ѱ���*/
	HRESULT SetUpCascadeDesc();

	void TickLerpEyeAt(float _fTimeDelta);
	void TickLerpFovy(float _fTimeDelta);
	void TickShake(float _fTimeDelta);
	_float ComputeShakeWave(float _fAmplitude, float _fFrequency, float _fOffSet, float _fRatio);
	_float ComputeShakeLinear(float _fAmplitude, float _fFrequency, float _fOffSet);

protected:
	_float m_fFovy = 0.0f;
	_float m_fNear = 0.0f;
	_float m_fFar = 0.0f;
	_float m_fAspect = 0.0f;

	/* Fovy Lerp */
	bool  m_isFovyLerp{ false };
	float m_fStartFovy{ 0.0f };
	float m_fLerpToFovy{ 0.0f };
	float m_fFovyAccTime{ 0.0f };
	float m_fFovyLerpTime{ 0.0f };
	/* Fovy Lerp */

	/* EyeAt Lerp */
	bool	m_isEyeAtLerp{ false };
	_float3 m_vStartEye;
	_float3 m_vStartAt;
	_float3 m_vEndEye;
	_float3 m_vEndAt;
	float	m_fEyeAtAccTime{ 0.0f };
	float	m_fEyeAtLerpTime{ 0.0f };
	float	m_fEyeAtLerpWeight{ 0.0f };
	/* EyeAt Lerp */

	/* ī�޶� ȸ���� ���� */
	_bool m_IsRotationLock = true;
	/* ���콺 ��� ������ ���� */
	_bool m_IsMouseLock = true;

	_float3 m_vCamAt;

	bool m_isRecording = false;		/* �̰� true �� ī�޶��� View, Proj�� PipeLine �� �����*/

	/* Shake ���� */
	UINT	m_eShakeFlag = SHAKE_NONE;
	bool	m_isShake = false;
	float	m_fShakeDuration = 0.0f;
	float	m_fShakeAccTime = 0.0f;
	ESHAKEFUNC m_eShakeFunc[TYPE_END];

	float	m_fRotAmplitude[AXIS_END]{};
	float   m_fRotFrequency[AXIS_END]{};
	float   m_fRotRandOffSet[AXIS_END]{};
	_float2	m_vRotBlendInOut;
	_float2 m_vRotBlendWeight;

	float	m_fLocAmplitude[AXIS_END]{};
	float	m_fLocFrequency[AXIS_END]{};
	float	m_fLocRandOffSet[AXIS_END]{};
	_float2	m_vLocBlendInOut;
	_float2 m_vLocBlendWeight;

	float	m_fFovyAmplitude = 0.0f;
	float	m_fFovyFrequency = 0.0f;
	_float2 m_vFovyBlendInOut;
	_float2 m_vFovyBlendWeight;
	float	m_fShakeFovy = 0.0f;

	_float4x4 m_ShakeWorldMatrix;
	/* Shake ���� */
/*For. Compute Cascade*/
protected:
	_float m_fCascadeEnd[4];
	_float4x4 m_shadowOrthoProj[3];
	_float4x4 m_shadowOrthoReverseProj[3];
	_float4x4 m_shadowView[3];
	_float4x4 m_shadowWorld[3];
	_float3 m_vPrevCenterPos[3];

};

END