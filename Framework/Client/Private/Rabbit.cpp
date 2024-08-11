#include "Rabbit.h"
#include "GameInstance.h"
#include "Model.h"
#include "RigidBody.h"
#include "Animation.h"
#include "EffectMgr.h"

CRabbit::CRabbit()
    : CNPC()
{
}

HRESULT CRabbit::Initialize(_float3 _vCreatePos)
{
    // ������ ��ġ�� ����
    __super::Initialize(_vCreatePos);

    /*Shader Info*/
    m_eShaderType = ESHADER_TYPE::ST_ANIM;
    m_iShaderPass = 0;

    m_pModel = GAMEINSTANCE->GetModel("T_GLD_Rabbit_Base_S_SK");
    m_strModelName = "T_GLD_Rabbit_Base_S_SK";
    AddComponent(L"Com_Model", m_pModel);
    m_pTransformCom->SetScaling(1.f, 1.f, 1.f);

    m_pModel->ChangeAnim(m_iActionState, 0.1f, true);

    AddRigidBody("T_GLD_Rabbit_Base_S_SK");

    m_vUIDistance = _float3(0.f, 1.f, 0.f);

    m_pRigidBody->SimulationOff();

    m_pTransformCom->SetSpeed(7.f);

    PrepareNPCBar(L"Rabbit");
    
    m_IsEnabled = false;

    return S_OK;
}

void CRabbit::PriorityTick(_float _fTimeDelta)
{
    __super::PriorityTick(_fTimeDelta);
}

void CRabbit::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);
    
    // Base���¿� ���� FSM
    switch (m_iBaseState)
    {
    case RABBIT_B_BEFOREMEET:
        FSM_BeforeMeet(_fTimeDelta);
        break;
    case RABBIT_B_MEET:
        FSM_Meet(_fTimeDelta);
        break;
    case RABBIT_B_INTERACT:
        FSM_Interact(_fTimeDelta);
        break;
    }

    m_pModel->PlayAnimation(_fTimeDelta);

    m_pTransformCom->Tick(_fTimeDelta);
}

void CRabbit::LateTick(_float _fTimeDelta)
{
    //m_pModelCom->CheckDisableAnim();

    m_pTransformCom->LateTick(_fTimeDelta);

    __super::LateTick(_fTimeDelta);

    // frustrum
	if (GAMEINSTANCE->isInWorldSpace(m_pTransformCom->GetState(CTransform::STATE_POSITION), 3.f))
	{
		GAMEINSTANCE->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this());

#ifdef _DEBUG
		if (m_pRigidBody)
			GAMEINSTANCE->AddDebugComponent(m_pRigidBody);
#endif // _DEBUG
	}
}

HRESULT CRabbit::Render()
{
    __super::Render();

    _uint iNumMeshes = m_pModel->GetNumMeshes();

    for (UINT i = 0; i < iNumMeshes; i++) {
        GAMEINSTANCE->ReadyShader(m_eShaderType, m_iShaderPass);
        GAMEINSTANCE->BindWVPMatrixPerspective(m_pTransformCom->GetWorldMatrix());
        GAMEINSTANCE->BeginAnimModel(m_pModel, i);
    }

    return S_OK;
}

shared_ptr<CRabbit> CRabbit::Create(_float3 _vCreatePos, _float3 _vInitLookPos, _int _iGenType)
{
    shared_ptr<CRabbit> pInstance = make_shared<CRabbit>();
    pInstance->SetInitLookPos(_vInitLookPos);
    pInstance->SetGenType(_iGenType);
    if (FAILED(pInstance->Initialize(_vCreatePos)))
        MSG_BOX("Failed to Create : CRabbit");

    return pInstance;
}

HRESULT CRabbit::AddRigidBody(string _strModelKey)
{
    CRigidBody::RIGIDBODYDESC RigidBodyDesc;

    /* Create Actor */
//bool			isStatic = false;	/* ������ ��ü�� true */
//bool			isTrigger = false;	/* Ʈ���� ��ü�� true */
//bool			isGravity = true;		/* �߷��� ���� �޴� ��ü�� true, (static �� �߷��� ��������)*/
//bool			isInitCol = true;		/* �������ڸ��� �浹�� ������ false */
//_float4x4		WorldMatrix = XMMatrixIdentity();	/* Transform �� ��°�� ������ �� */
//UINT			eLock_Flag = ELOCK_FLAG::NONE; /* Static�� �ʿ����,CRigidBody::ELOCK_FLAG */
//
///* Create Shape */
//weak_ptr<CGameObject> pOwnerObject;				/* ���� ��ü�� ������ */
//float			fStaticFriction = 0.5f;		/* ���� ������(0.0f~) */
//float			fDynamicFriction = 0.5f;		/* � ������(0.0f~) */
//float			fRestitution = 0.0f;		/* ź����(0.0f~1.0f) */
//Geometry* pGeometry = nullptr;	/* ���(GeometrySphere, GeometryBox, etc) */
//ECOLLISIONFLAG	eThisColFlag = COL_NONE;	/* �ڱ� �ڽ��� �浹 Flag */
//UINT			eTargetColFlag = COL_NONE; /* �浹�� ��ü�� Flag�� �߰� ex) COL_MONSTER | COL_ITEM  */

    RigidBodyDesc.isStatic = false;
    RigidBodyDesc.isTrigger = false;
    RigidBodyDesc.isGravity = false;
    RigidBodyDesc.isInitCol = true;
    RigidBodyDesc.pTransform = m_pTransformCom;
    RigidBodyDesc.eLock_Flag = CRigidBody::LOCK_ROT_ALL;

    RigidBodyDesc.pOwnerObject = shared_from_this();
    RigidBodyDesc.fStaticFriction = 1.0f;
    RigidBodyDesc.fDynamicFriction = 0.0f;
    RigidBodyDesc.fRestitution = 0.0f;

    GeometryCapsule CapsuleDesc;
    CapsuleDesc.fHeight = 0.8f;
    CapsuleDesc.fRadius = 0.6f;
    _float3 vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
    CapsuleDesc.vOffSetPosition = { 0.0f, 0.3f, 0.0f };
    CapsuleDesc.vOffSetRotation = { 0.0f, 0.0f, 90.0f };
    CapsuleDesc.strShapeTag = "Rabbit_Body";
    RigidBodyDesc.pGeometry = &CapsuleDesc;

    RigidBodyDesc.eThisColFlag = COL_NPC;
    RigidBodyDesc.eTargetColFlag = COL_STATIC | COL_PLAYER;
    m_pRigidBody = GAMEINSTANCE->CloneRigidBody(&RigidBodyDesc);

    m_pTransformCom->SetRigidBody(m_pRigidBody);
    m_pRigidBody->SetMass(10.0f);
    return S_OK;
}

void CRabbit::OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

void CRabbit::OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

void CRabbit::OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

void CRabbit::UpdateDistanceState()
{
    // ��ġ �ٷ� ���޾ƿ��°� ������ �켱
    if (m_PlayerPos == _float3(0.f, 0.f, 0.f))
    {
        m_iDistanceState = RABBIT_D_FARRANGE;
        return;
    }

    if (m_fDistanceXZToPlayer < m_fMeetRange)
    {
        m_iDistanceState = RABBIT_D_MEETRANGE;
    }
    else
    {
        m_iDistanceState = RABBIT_D_FARRANGE;
    }
}

void CRabbit::FSM_BeforeMeet(_float _fTimeDelta)
{
    if (m_iActionState == RABBIT_A_GLIDING)
    {
        MovePos(m_vCurrentPos + m_pTransformCom->GetState(CTransform::STATE_LOOK) * 5.f * _fTimeDelta + _float3(0.f, 0.8f, 0.f) * _fTimeDelta);
    }

    m_fCalculateTime1 += _fTimeDelta;
    if (m_fCalculateTime1 > m_fLifeTime)
    {
        m_fCalculateTime1 = 0.f;
        m_IsEnabled = false;
    }
}

void CRabbit::FSM_Meet(_float _fTimeDelta)
{
}

void CRabbit::FSM_Interact(_float _fTimeDelta)
{

}

void CRabbit::InteractionOn()
{
}