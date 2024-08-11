#pragma once

#include "Component.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;


class CVIPhysX;



class ENGINE_DLL CRigidBody final : public CComponent
{
public:
	enum ELOCK_FLAG
	{
		NONE			= 0,
		LOCK_POS_X		= (1 << 0),
		LOCK_POS_Y		= (1 << 1),
		LOCK_POS_Z		= (1 << 2),
		LOCK_ROT_X		= (1 << 3),
		LOCK_ROT_Y		= (1 << 4),
		LOCK_ROT_Z		= (1 << 5),

		LOCK_POS_ALL	= LOCK_POS_X | LOCK_POS_Y | LOCK_POS_Z,
		LOCK_ROT_ALL	= LOCK_ROT_X | LOCK_ROT_Y | LOCK_ROT_Z,
		LOCK_ALL		= LOCK_POS_ALL | LOCK_ROT_ALL
	};

	struct RIGIDBODYDESC
	{
		/* Create Actor */
		bool			isStatic			= false;	/* ������ ��ü�� true */
		bool			isTrigger			= false;	/* Ʈ���� ��ü�� true */
		bool			isGravity			= true;		/* �߷��� ���� �޴� ��ü�� true, (static �� �߷��� ��������)*/
		bool			isInitCol			= true;		/* �������ڸ��� �浹�� ������ false */
		shared_ptr<CTransform> pTransform = nullptr;	/* Transform ��°�� ������ �� */
		UINT			eLock_Flag			= ELOCK_FLAG::NONE; /* Static�� �ʿ����,CRigidBody::ELOCK_FLAG */
		
		/* Create Shape */
		weak_ptr<CGameObject> pOwnerObject;				/* ���� ��ü�� ������ */ 
		float			fStaticFriction		= 0.5f;		/* ���� ������(0.0f~) */
		float			fDynamicFriction	= 0.5f;		/* � ������(0.0f~) */
		float			fRestitution		= 0.0f;		/* ź����(0.0f~1.0f) */
		Geometry*		pGeometry			= nullptr;	/* ���(GeometrySphere, GeometryBox, etc) */
		ECOLLISIONFLAG	eThisColFlag		= COL_NONE;	/* �ڱ� �ڽ��� �浹 Flag */
		UINT			eTargetColFlag		= COL_NONE; /* �浹�� ��ü�� Flag�� �߰� ex) COL_MONSTER | COL_ITEM  */

		void*			pArg				= nullptr;	/* ���� ������, ����ü�� "new"�� �Ҵ��ؼ� �־��� �� */
	};

public:
	CRigidBody(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	CRigidBody(const shared_ptr<CRigidBody> _pOrigin);
	virtual ~CRigidBody();

public:
	static shared_ptr<CRigidBody> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual shared_ptr<CComponent> Clone(void* _pArg) override;

public:
	HRESULT Initialize();;
	virtual HRESULT InitializeClone(void* _pArg) override;

	/* �߰����� ����� �� �������� �̰ɷ� ������ ��
	   ��, �̹� ������� Actor�� �ٲ� �� ����. (static �̳� eLock_Flag ���� �͵�)     */
	HRESULT CreateShape(RIGIDBODYDESC* _pDesc, bool _isShared = false);

public:
	_float3 GetPosition();
	_quaternion GetQuat();

public:
	HRESULT ChangeShape(const char* _szShapeTag);	/* ������ ����� ���� shape�� ����, Actor�� �پ��ִ� ��� shape�� �������� */
	HRESULT AttachShape(const char* _szShapeTag);	/* ������ ����� ���� shape�� �����δ�, Actor�� �پ��ִ� shpae�� ���� */
	void SetPosition(const _float3& _vPosition, bool _isAutowake = true);
	void SetRotation(const _quaternion& _vQuat, bool _isAutowake = true);
	void SetGlobalPos(const _float3& _vPosition, const _quaternion& _Quat, bool _isAutowake = true);
	void EnableCollision(const char* _szShapeTag);
	void DisableCollision(const char* _szShapeTag);
	void WakeUp();

public:		/* Dynamic Function */
	_float3 GetLinearVelocity();
	_float3 GetAngularVelocity();
	void SetLinearVelocity(const PxVec3& _vLinVel, bool _isAutowake = true);
	void SetAngularVelocity(const PxVec3& _vAngVel, bool _isAutowake = true);
	void SetMaxLinearVelocity(float _fMaxLinVel);
	void SetLockFlags(UINT _eLockFlags, bool _isLock);	/* CRigidBody::ELOCK_FLAG �� ���� */
	void AddForce(const PxVec3& _vForce, PxForceMode::Enum _eMode = PxForceMode::eFORCE, bool _isAutowake = true);
	void SetMass(float _fMass);
	void GravityOn();
	void GravityOff();
	void SimulationOn();
	void SimulationOff();

private:
	HRESULT CheckArgument(RIGIDBODYDESC* _pDesc);
	HRESULT CreateActor(RIGIDBODYDESC* _pDesc);
	HRESULT CreateGeometry(RIGIDBODYDESC* _pDesc, bool _isShared);

	PxShape* Find_Shape(const char* _szShapeTag);

	virtual HRESULT Render() override;
	HRESULT CreateBuffer(PxShape* _pShape, float _fSizeX, float _fSizeY, float _fSizeZ, const _float3& _vOffsetPosition, const _quaternion& _Quat);

private:
	PxRigidActor*	m_pActor	= nullptr;
	PxMaterial*		m_pMaterial = nullptr;
	unordered_multimap<string, pair<PxShape*, shared_ptr<COLLISIONDATA>>> m_Shapes; // �÷��̾��� ��� ���, ����, ������ ��
	PxScene*		m_pScene	= nullptr;
	
	bool			m_isStatic	= false;


private:
	list<pair<PxShape*, shared_ptr<CVIPhysX>>> m_lstVIBuffer;

private:
	void Free();
};

END