#include "NPC_Resident2.h"
#include "GameInstance.h"
#include "Model.h"
#include "Weapon.h"
#include "RigidBody.h"
#include "Animation.h"
#include "EffectMgr.h"
#include "UIMgr.h"

CNPC_Resident2::CNPC_Resident2()
    : CNPC()
{
}

HRESULT CNPC_Resident2::Initialize(_float3 _vCreatePos)
{
    // ������ ��ġ�� ����
    __super::Initialize(_vCreatePos);

    /*Shader Info*/
    m_eShaderType = ESHADER_TYPE::ST_ANIM;
    m_iShaderPass = 0;

    m_pModel = GAMEINSTANCE->GetModel("Resident2");
    m_strModelName = "Resident2";
    AddComponent(L"Com_Model", m_pModel);
    m_pTransformCom->SetScaling(1.f, 1.f, 1.f);

    switch (m_iGenType)
    {
    case RESIDENT2_G_SIT:
        m_iActionState = RESIDENT2_A_SIT;
        break;
    case RESIDENT2_G_IDLE:
        m_iActionState = RESIDENT2_A_IDLE;
        m_pTransformCom->SetSpeed(0.7f);
        break;
    case RESIDENT2_G_TALK:
        m_iActionState = RESIDENT2_A_TALK;
        break;
    case RESIDENT2_G_WALK200:
        m_iActionState = RESIDENT2_A_WALK200;
        break;
    case RESIDENT2_G_WALK50:
        m_iActionState = RESIDENT2_A_WALK50;
        break;
    }

    m_pModel->ChangeAnim(m_iActionState, 0.1f, true);

    /*if (FAILED(AddPartObjects("")))
        return E_FAIL;*/

    m_fSpeed = 1.f;

    AddRigidBody("Resident2");
    
    //m_pRigidBody->SimulationOn();

    PrepareNPCBar(L"Resident");
    m_strDisplayName = L"Resident";

    return S_OK;
}

void CNPC_Resident2::PriorityTick(_float _fTimeDelta)
{
    __super::PriorityTick(_fTimeDelta);
}

void CNPC_Resident2::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);

    // �������¿� ���� FSM
    switch (m_iGenType)
    {
    case RESIDENT2_G_SIT:
        FSM_Idle(_fTimeDelta);
        break;
    case RESIDENT2_G_IDLE:
        FSM_Idle(_fTimeDelta);
        break;
    case RESIDENT2_G_TALK:
        FSM_Idle(_fTimeDelta);
        break;
    case RESIDENT2_G_WALK200:
        FSM_Walk1(_fTimeDelta);
        m_pTransformCom->SetSpeed(1.5f);
        break;
    case RESIDENT2_G_WALK50:
        FSM_Walk2(_fTimeDelta);
        m_pTransformCom->SetSpeed(0.5f);
        break;
    }

    m_pModel->PlayAnimation(_fTimeDelta);

    m_pTransformCom->Tick(_fTimeDelta);
}

void CNPC_Resident2::LateTick(_float _fTimeDelta)
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

HRESULT CNPC_Resident2::Render()
{
    /*if (!m_pNPCParts.begin()->second->GetActiveOn())
        m_pNPCParts.begin()->second->SetActiveOn(true);*/
    __super::Render();
    _uint iNumMeshes = m_pModel->GetNumMeshes();

    for (UINT i = 0; i < iNumMeshes; i++) {
        /*�Ӹ�, ����*/
        if (i == 0) {

            //Pass ����
            GAMEINSTANCE->ReadyShader(m_eShaderType, 1);
            GAMEINSTANCE->BindWVPMatrixPerspective(m_pTransformCom->GetWorldMatrix());

            //�� Bind
            GAMEINSTANCE->BindRawValue("g_vHairMainColor", &m_vHairMainColor, sizeof(_float3));
            GAMEINSTANCE->BindRawValue("g_vHairSubColor", &m_vHairSubColor, sizeof(_float3));
            
            GAMEINSTANCE->BeginAnimModel(m_pModel, i);
        }
        // ����, ��
        else if (i == 3 || i == 4)
        {
            continue;
        }
        // �Ȱ� ��
        /*else if (i == 9)
        {
            continue;
        }*/
        else {
            GAMEINSTANCE->ReadyShader(m_eShaderType, m_iShaderPass);
            GAMEINSTANCE->BindWVPMatrixPerspective(m_pTransformCom->GetWorldMatrix());
            GAMEINSTANCE->BeginAnimModel(m_pModel, i);
        }
    }

    return S_OK;
}

shared_ptr<CNPC_Resident2> CNPC_Resident2::Create(_float3 _vCreatePos, _float3 _vInitLookPos, _int _iGenType, _float3 _vHairColor, _float3 _vHairSubColor)
{
    shared_ptr<CNPC_Resident2> pInstance = make_shared<CNPC_Resident2>();
    pInstance->SetInitLookPos(_vInitLookPos);
    pInstance->SetPatrolPos(_vInitLookPos);
    pInstance->SetGenType(_iGenType);
    pInstance->SetHairMainColor(_vHairColor);
    pInstance->SetHairSubColor(_vHairSubColor);
    if (FAILED(pInstance->Initialize(_vCreatePos)))
        MSG_BOX("Failed to Create : CNPC_Resident2");

    return pInstance;
}

HRESULT CNPC_Resident2::AddRigidBody(string _strModelKey)
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
    RigidBodyDesc.isGravity = true;
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
    CapsuleDesc.vOffSetPosition = { 0.0f, 1.f, 0.0f };
    CapsuleDesc.vOffSetRotation = { 0.0f, 0.0f, 90.0f };
    CapsuleDesc.strShapeTag = "NPC_Resident2_Body";
    RigidBodyDesc.pGeometry = &CapsuleDesc;

    RigidBodyDesc.eThisColFlag = COL_NPC;
    RigidBodyDesc.eTargetColFlag = COL_STATIC | COL_PLAYER;
    m_pRigidBody = GAMEINSTANCE->CloneRigidBody(&RigidBodyDesc);

    m_pTransformCom->SetRigidBody(m_pRigidBody);
    m_pRigidBody->SetMass(20.0f);
    return S_OK;
}

void CNPC_Resident2::OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

void CNPC_Resident2::OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

void CNPC_Resident2::OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

//HRESULT CNPC_Resident2::AddPartObjects(string _strModelKey)
//{
//    shared_ptr<CWeapon::WEAPON_DESC> pWeaponDesc = make_shared<CWeapon::WEAPON_DESC>();
//    pWeaponDesc->tPartObject.pParentTransform = m_pTransformCom;
//    pWeaponDesc->pSocketMatrix = m_pModelCom->GetCombinedBoneMatrixPtr("Bn_Action_RightHand");
//    pWeaponDesc->strModelKey = _strModelKey;
//    pWeaponDesc->OffsetMatrixNormal = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(90.f)) *
//        SimpleMath::Matrix::CreateRotationX(XMConvertToRadians(90.f));
//    
//    shared_ptr<CTransform::TRANSFORM_DESC> pTransformDesc = make_shared<CTransform::TRANSFORM_DESC>();
//
//    pTransformDesc->fSpeedPerSec = 10.f;
//    pTransformDesc->fRotationPerSec = XMConvertToRadians(90.0f);
//
//    shared_ptr<CWeapon> pWeapon = CWeapon::Create(pWeaponDesc.get(), pTransformDesc.get());
//    if (nullptr == pWeapon)
//        return E_FAIL;
//
//    shared_ptr<CPartObject> pPartObject = static_pointer_cast<CPartObject>(pWeapon);
//    pPartObject->SetActive(false);
//    m_pNPCParts.emplace("Part_Weapon", pPartObject);
//
//    return S_OK;
//}

void CNPC_Resident2::FSM_Idle(_float _fTimeDelta)
{
}

void CNPC_Resident2::FSM_Walk1(_float _fTimeDelta)
{
    if (m_bPatrolDirection && (m_iActionState == RESIDENT2_G_WALK200))
    {
        m_pTransformCom->GoStraight(_fTimeDelta);
        _float2 vTargetPosXZ = _float2(m_vPatrolPos.x, m_vPatrolPos.z);
        _float2 vCurrentPosXZ = _float2(m_vCurrentPos.x, m_vCurrentPos.z);
        _float m_fDistnce = (vTargetPosXZ - vCurrentPosXZ).Length();

        if (m_fDistnce < 0.5f)
        {
            m_iActionState = RESIDENT2_A_IDLE;
            m_pModel->ChangeAnim(m_iActionState, 0.1f, true);

            m_bPatrolDirection = false;
        }
    }
    if (!m_bPatrolDirection && (m_iActionState == RESIDENT2_G_WALK200))
    {
        m_pTransformCom->GoStraight(_fTimeDelta);
        _float2 vTargetPosXZ = _float2(m_vFirstCreatePos.x, m_vFirstCreatePos.z);
        _float2 vCurrentPosXZ = _float2(m_vCurrentPos.x, m_vCurrentPos.z);
        _float m_fDistnce = (vTargetPosXZ - vCurrentPosXZ).Length();

        if (m_fDistnce < 0.5f)
        {
            m_iActionState = RESIDENT2_A_IDLE;
            m_pModel->ChangeAnim(m_iActionState, 0.1f, true);

            m_bPatrolDirection = true;
        }
    }
    if (m_iActionState == RESIDENT2_A_IDLE)
    {
        m_fCalculateTime1 += _fTimeDelta;
        if (m_fCalculateTime1 > 3.f)
        {
            m_fCalculateTime1 = 0.f;
            if (m_bPatrolDirection)
            {
                m_pTransformCom->LookAtDir(_float3(m_vPatrolPos.x, 0.f, m_vPatrolPos.z) -
                    _float3(m_vFirstCreatePos.x, 0.f, m_vFirstCreatePos.z));
                m_iActionState = RESIDENT2_G_WALK200;
                m_pModel->ChangeAnim(m_iActionState, 0.1f, true);
            }
            else
            {
                m_pTransformCom->LookAtDir(_float3(m_vFirstCreatePos.x, 0.f, m_vFirstCreatePos.z) -
                    _float3(m_vPatrolPos.x, 0.f, m_vPatrolPos.z));
                m_iActionState = RESIDENT2_G_WALK200;
                m_pModel->ChangeAnim(m_iActionState, 0.1f, true);
            }
        }
    }
}

void CNPC_Resident2::FSM_Walk2(_float _fTimeDelta)
{
    if (m_bPatrolDirection && (m_iActionState == RESIDENT2_G_WALK50))
    {
        m_pTransformCom->GoStraight(_fTimeDelta);
        _float2 vTargetPosXZ = _float2(m_vPatrolPos.x, m_vPatrolPos.z);
        _float2 vCurrentPosXZ = _float2(m_vCurrentPos.x, m_vCurrentPos.z);
        _float m_fDistnce = (vTargetPosXZ - vCurrentPosXZ).Length();

        if (m_fDistnce < 0.5f)
        {
            m_iActionState = RESIDENT2_A_IDLE;
            m_pModel->ChangeAnim(m_iActionState, 0.1f, true);

            m_bPatrolDirection = false;
        }
    }
    if (!m_bPatrolDirection && (m_iActionState == RESIDENT2_G_WALK50))
    {
        m_pTransformCom->GoStraight(_fTimeDelta);
        _float2 vTargetPosXZ = _float2(m_vFirstCreatePos.x, m_vFirstCreatePos.z);
        _float2 vCurrentPosXZ = _float2(m_vCurrentPos.x, m_vCurrentPos.z);
        _float m_fDistnce = (vTargetPosXZ - vCurrentPosXZ).Length();

        if (m_fDistnce < 0.5f)
        {
            m_iActionState = RESIDENT2_A_IDLE;
            m_pModel->ChangeAnim(m_iActionState, 0.1f, true);

            m_bPatrolDirection = true;
        }
    }
    if (m_iActionState == RESIDENT2_A_IDLE)
    {
        m_fCalculateTime1 += _fTimeDelta;
        if (m_fCalculateTime1 > 3.f)
        {
            m_fCalculateTime1 = 0.f;
            if (m_bPatrolDirection)
            {
                m_pTransformCom->LookAtDir(_float3(m_vPatrolPos.x, 0.f, m_vPatrolPos.z) -
                    _float3(m_vFirstCreatePos.x, 0.f, m_vFirstCreatePos.z));
                m_iActionState = RESIDENT2_G_WALK50;
                m_pModel->ChangeAnim(m_iActionState, 0.1f, true);
            }
            else
            {
                m_pTransformCom->LookAtDir(_float3(m_vFirstCreatePos.x, 0.f, m_vFirstCreatePos.z) -
                    _float3(m_vPatrolPos.x, 0.f, m_vPatrolPos.z));
                m_iActionState = RESIDENT2_G_WALK50;
                m_pModel->ChangeAnim(m_iActionState, 0.1f, true);
            }
        }
    }
}

void CNPC_Resident2::InteractionOn()
{
}