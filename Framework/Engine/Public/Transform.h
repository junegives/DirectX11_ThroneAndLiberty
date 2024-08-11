#pragma once

#include "Component.h"

BEGIN(Engine)

class CRigidBody;

class ENGINE_DLL CTransform : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	struct TRANSFORM_DESC {

		_float fSpeedPerSec = { 0.f };
		_float fRotationPerSec = { 0.f };

	};

private:
	enum EMOVEMENT_STATE { MS_NONE = 0, MS_POSITION = 1 << 0, MS_ROTATION = 1 << 1 };

public:
	CTransform(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CTransform() = default;

public:
	static shared_ptr<CTransform> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, TRANSFORM_DESC* _pDesc = nullptr);

public:
	virtual HRESULT Initialize(TRANSFORM_DESC* _pDesc);

public:
	_float3 GetState(STATE _eState);
	_float3 GetScaled();
	_quaternion GetQuat();
	_float4x4 GetWorldMatrix();
	_float4x4 GetWorldMatrixInverse();
	_float4x4* GetWorldMatrixPtr();
	_float4x4 GetRotationMatrix();
	_float GetSpeed() { return m_fSpeedPerSec; }
	_float GetTurnSpeed() { return m_fRotationPerSec; }
	void SetScaling(_float3 _vScale);
	void SetScaling(_float _fX, _float _fY, _float _fZ);
	void SetState(STATE _eState, _float3 _vState);
	void SetQuaternion(const _quaternion& _vQuaternion);
	void PlusQuaternion(const _quaternion& _vQuaternion);
	void SetWorldMatrix(_float4x4 _worldMat);
	void SetSpeed(_float _fSpeed);
	void SetTurnSpeed(_float _fTurnSpeed);
	void SetUpDir(_float3 _vUpDir);

public:
	void Turn(_float3 _vAxis, _float _fTimeDelta);
	void TurnToDir(_float3 _vDir, _float _fTimeDelta);	// ��ǥ ���⺤�ͷ�
	void Rotation(_float3 _vAxis, _float _fRadian);
	void RotationAll(_float _fRadianX, _float _fRadianY, _float _fRadianZ);	// ���� �� �������� ���� �� ���� ȸ��
	void RotationPlus(_float3 _vAxis, _float _fRadian);	// ���� �������� �̸�ŭ �� ȸ��
	void RotationPlusScale(_float3 _vAxis, _float _fRadian);	// ���� �������� �̸�ŭ �� ȸ���ϸ鼭 ������ ����
	void MoveTo(_float3 _vPoint, _float _fLimit, _float _fTimeDelta);
	//void LookAt(_float4 _vPoint);
	void LookAt(_float3 _vPoint);
	void LookAtDir(_float3 _vDir);
	void LookAtForLandObject(_float3 _vPoint);
	void LookAtDirForLandObject(_float3 _vDir);

	// ������ x,y,z �� ���� ��ġ���� ����
	void SetPositionParam(_int _iType, _float _fParam);

	// Look ����
	void GoStraight(_float _fTimeDelta);
	void GoStraightLeft(_float _fTimeDelta);
	void GoStraightRight(_float _fTimeDelta);
	void GoBackward(_float _fTimeDelta);
	void GoBackwardLeft(_float _fTimeDelta);
	void GoBackwardRight(_float _fTimeDelta);
	void GoLeft(_float _fTimeDelta);
	void GoRight(_float _fTimeDelta);
	void GoStraightFreeSpeed(_float _fTimeDelta, _float _Speed);
	void GoUpFreeDistance(_float Distance);

	// ���� ���� ����
	void GoDStraight(_float _fTimeDelta);
	void GoDBackward(_float _fTimeDelta);
	void GoDLeft(_float _fTimeDelta);
	void GoDRight(_float _fTimeDelta);
	void GoDDown(_float _fTimeDelta);
	void GoDDownFreeSpeed(_float _fTimeDelta, _float _Speed);


	// ������ ��������
	void GoDir(_float _fTimeDelta, _float3 _vDir);
	void GoDirFreeSpeed(_float _fTimeDelta, _float3 _vDir, _float _Speed);
	void GoDirNotNormalize(_float _fTimeDelta, _float3 _vDir, _float _Speed);


	/*PhysX*/
	HRESULT SetRigidBody(shared_ptr<CRigidBody> _pRigidBody);
	void SetMaxLiearVelocity(float _fMaxLinVel);
	void Jump(_float _fJumpForce);
	void AddForce(const _float3& _vForce, PxForceMode::Enum _eMode = PxForceMode::eFORCE, bool _isAutowake = true);
	void Tick(_float _fTimeDelta);
	void LateTick(_float _fTimeDelta);
	void LateTickFragment(_float _fTimeDelta);

private:
	void AddPosition(const _float3& _vDeltaPos, _float _fTimeDelta);

private:
	_float4x4 m_WorldMatrix = {};
	_float m_fSpeedPerSec = { 0.f };
	_float m_fRotationPerSec = { 0.f };

	weak_ptr<CRigidBody> m_pRigidBody;
	_float3 m_Velocity = { 0.0f,0.0f,0.0f };
	UINT m_Movement_States = MS_NONE;
};

END