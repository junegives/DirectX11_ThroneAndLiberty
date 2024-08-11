#pragma once

#include "Engine_Defines.h"
#include "PxEventCallback.h"

BEGIN(Engine)

class CRigidBody;

class CPhysXMgr
{
public:
	CPhysXMgr();
	~CPhysXMgr();

public:
	static shared_ptr<CPhysXMgr> Create();

public:
	HRESULT Initialize();
	void Tick(float _fTimeDelta);

public:
	PxPhysics*	GetPhysics() { return m_pPhysics; }
	PxScene*	GetScene() { return m_pScene; }
	void SetSimultation(bool _isSimulation) { m_isSimulation = _isSimulation; }
	
	shared_ptr<CRigidBody> CloneRigidBody(void* _pArg);

public:
	void Free();

private:
	// Foundation�� �����ϴµ� �ʿ��� ����
	PxDefaultAllocator m_DefaultAllocator;
	PxDefaultErrorCallback m_DefaultErrorCallback;
	PxFoundation* m_pFoundation = nullptr;

	// PxFoundation�� �־�� m_Physics�� ������ �� �ִ�.
	PxPhysics* m_pPhysics = nullptr;

	// GPU ����� ���� ��ü
	PxCudaContextManager* m_pCudaContextManager = nullptr;

	// CPU ���ҽ��� ȿ�������� ������ �� �ֵ��� �ϱ� ���� ����
	PxDefaultCpuDispatcher* m_pDispatcher = nullptr;

	// �ùķ��̼��� ���� ���ο� ����
	PxScene* m_pScene = nullptr;

	// �ùķ��̼� �̺�Ʈ�� �����ϱ� ���� ����ڰ� ����
	CPxEventCallback* m_pPxEventCallback = nullptr;

#ifdef _PVD
	PxPvd* m_pPvd = nullptr;
#endif

	bool m_isSimulation = false;

	shared_ptr<CRigidBody> m_pProtoRigidBody = nullptr;

	float m_fTrackPosition{ 0.0f };
	float m_fTime60{ 1.0f / 60.0f };
};

END