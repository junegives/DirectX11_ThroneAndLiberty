#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCameraFilm;

class CCameraMgr
{
	DECLARE_SINGLETON(CCameraMgr)

public:
	CCameraMgr();
	~CCameraMgr();

public:
	void AddCamera(const string& _strCameraTag, shared_ptr<CCamera> _pCamera);
	void AddFilmCamera(const string& _strCameraTag, shared_ptr<CCameraFilm> _pCamera);
	void CameraTurnOn(const string& _strCameraTag);
	void CameraTurnOn(const string& _strCameraTag, const _float3& _vEye, const _float3& _vAt);
	void CameraTurnOn(const string& _strCameraTag, float _fTime, float _fWeight = 0.2f);			// ->CameraSpherical �� �ٲܶ� ���
	void CameraTurnOn(const string& _strCameraTag, const _float3& _vEye, const _float3& _vAt, float _fTime, float _fWeight = 0.2f);	// ->StaticCamera �� �ٲ� �� ���
	void FilmCameraTurnOn(const string& _strCameraTag, bool _isHUDActive = true);			// ���� ī�޶� �ѱ�, (�ٷ� ���۵ǰ�, �̹� ���� ���̴� ���� �����ϰ� ������)
	void FilmCameraTurnOn(const vector<string>& _vecCameraTags, bool _isHUDActive = true);	// ���� ī�޶� �ѱ�, ���޾� ���, (�ٷ� ���۵ǰ�, �̹� ���� ���̴� ���� �����ϰ� ������)
	void FilmCameraTurnOn(const string& _strCameraTag, const _float3& _vLook, const _float3& _vOffSetPosition, bool _isHUDActive = true, bool _isDeactiveHUD = false);			// ���� ī�޶� �ѱ�, (�ٷ� ���۵ǰ�, �̹� ���� ���̴� ���� �����ϰ� ������)

	void FilmEnd();
	void FilmEnd(const _float3& _vEye, const _float3& _vAt);

public:
	// CameraShake �Լ��� Desc�鿡 �� ���� ��������
	ROT_DESC RotSetting(_float3 vRotAmplitude, _float3 vRotFrequency, _float2	vBlendInOut = { 0.0f,0.0f }, _float2 vInOutWeight = { 1.0f,1.0f }, ESHAKEFUNC eShakeFunc = SHAKE_WAVE, bool isOffSetRand = true);
	LOC_DESC LocSetting(_float3 vLocAmplitude, _float3	vLocFrequency, _float2	vBlendInOut = { 0.0f,0.0f }, _float2 vInOutWeight = { 1.0f,1.0f }, ESHAKEFUNC eShakeFunc = SHAKE_WAVE, bool isOffSetRand = true);
	FOV_DESC FovSetting(float fFovyAmplitude, float fFovyFrequency, _float2 vBlendInOut = { 0.0f,0.0f }, _float2 vInOutWeight = { 1.0f,1.0f }, ESHAKEFUNC eShakeFunc = SHAKE_WAVE);
	// _eShakeFlags �� enum ESHAKEFLAG { SHAKE_NONE = 0, SHAKE_ROT = 1 << 0, SHAKE_LOC = 1 << 1, SHAKE_FOV = 1 << 2 }; �� SHAKE_ROT | SHAKE_LOC; �̷������� ���� �� ����
	void CameraShake(UINT _eShakeFlags, float _fDuration, ROT_DESC* _pRotDesc = nullptr, LOC_DESC* _pLocDesc = nullptr, FOV_DESC* _pFovDesc = nullptr);
	void CameraShakeStop();

public:
	map<const string, weak_ptr<CCamera>>* GetCameras();
	void SwitchingMouseLock();
	void MouseLockOn();
	void MouseLockOff();
	shared_ptr<CCamera> FindCamera(const string& _strCameraTag);
	shared_ptr<CCameraFilm> FindFilmCamera(const string& _strCameraTag);
	void DeleteCamera(const string& _strCameraTag);
	void DeleteCamera(shared_ptr<CCamera> _pThis);
	void DeleteFilmCamera(const string& _strCameraTag);
	void DeleteFilmCamera(shared_ptr<CCameraFilm> _pThis);
	void ClearLevel();
	void Clear();

private:
	weak_ptr<CCamera> m_pRecordingCamera;
	weak_ptr<CCameraFilm> m_pRecordingFilmCamera;
	map<const string, weak_ptr<CCamera>> m_Cameras;
	map<const string, weak_ptr<CCameraFilm>> m_FilmCameras;
	int m_iFilmCnt{ 0 };
	int m_iCurFilmIndex{ 0 };
	bool m_isHUDActive{ true };
	vector<string> m_vecFilmCameraTags;
};

END