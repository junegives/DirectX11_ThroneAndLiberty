#include "NPC_Henry.h"
#include "GameInstance.h"
#include "Model.h"
#include "Weapon.h"
#include "RigidBody.h"
#include "Animation.h"
#include "EffectMgr.h"
#include "QuestMgr.h"
#include "DialogueMgr.h"
#include "UIMgr.h"

CNPC_Henry::CNPC_Henry()
    : CNPC()
{
}

HRESULT CNPC_Henry::Initialize(_float3 _vCreatePos)
{
    // ������ ��ġ�� ����
    __super::Initialize(_vCreatePos);

    /*Shader Info*/
    m_eShaderType = ESHADER_TYPE::ST_ANIM;
    m_iShaderPass = 0;

    m_pModel = GAMEINSTANCE->GetModel("CT_Named_Henry_SK");
    m_strModelName = "CT_Named_Henry_SK";
    AddComponent(L"Com_Model", m_pModel);
    m_pTransformCom->SetScaling(1.f, 1.f, 1.f);

    m_pModel->ChangeAnim(m_iActionState, 0.1f, true);

    if (FAILED(AddPartObjects("IT_Torch_00001_SK")))
        return E_FAIL;
    
    m_fSpeed = 1.f;

    AddRigidBody("CT_Named_Henry_SK");

    m_vUIDistance = _float3(0.f, 1.f, 0.f);

    //m_pRigidBody->SimulationOn();

    //m_vLockOnOffSetPosition = { 0.0f,0.8f,0.0f };

    PrepareNPCBar(L"Henry");
    m_strDisplayName = L"Henry";

    return S_OK;
}

void CNPC_Henry::PriorityTick(_float _fTimeDelta)
{
    __super::PriorityTick(_fTimeDelta);
}

void CNPC_Henry::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);
    
    // Base���¿� ���� FSM
    switch (m_iBaseState)
    {
    case HENRY_B_BEFOREMEET:
        FSM_BeforeMeet(_fTimeDelta);
        break;
    case HENRY_B_ALERT:
        FSM_Meet(_fTimeDelta);
        break;
    case HENRY_B_READY:
        FSM_Ready(_fTimeDelta);
        break;
    case HENRY_B_NORMAL:
        FSM_Normal(_fTimeDelta);
        break;
    }

    m_pModel->PlayAnimation(_fTimeDelta);

    m_pTransformCom->Tick(_fTimeDelta);
}

void CNPC_Henry::LateTick(_float _fTimeDelta)
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

HRESULT CNPC_Henry::Render()
{
    /*if (!m_pNPCParts.begin()->second->GetActiveOn())
        m_pNPCParts.begin()->second->SetActiveOn(true);*/
    __super::Render();
    _uint iNumMeshes = m_pModel->GetNumMeshes();

    for (UINT i = 0; i < iNumMeshes; i++) {
        /*�Ӹ�*/
        if (i == 3) {

            //Pass ����
            GAMEINSTANCE->ReadyShader(m_eShaderType, 1);
            GAMEINSTANCE->BindWVPMatrixPerspective(m_pTransformCom->GetWorldMatrix());

            //�� Bind
            GAMEINSTANCE->BindRawValue("g_vHairMainColor", &m_vHairMainColor, sizeof(_float3));
            GAMEINSTANCE->BindRawValue("g_vHairSubColor", &m_vHairSubColor, sizeof(_float3));

            GAMEINSTANCE->BeginAnimModel(m_pModel, i);
        }
        // ����
        else if (i == 5)
        {
            continue;
        }
        else {
            GAMEINSTANCE->ReadyShader(m_eShaderType, m_iShaderPass);
            GAMEINSTANCE->BindWVPMatrixPerspective(m_pTransformCom->GetWorldMatrix());
            GAMEINSTANCE->BeginAnimModel(m_pModel, i);
        }
    }

    /*if (m_pNPCParts.begin()->second->GetActiveOn())
        m_pNPCParts.begin()->second->SetActiveOn(false);*/

    return S_OK;
}

shared_ptr<CNPC_Henry> CNPC_Henry::Create(_float3 _vCreatePos)
{
    shared_ptr<CNPC_Henry> pInstance = make_shared<CNPC_Henry>();
    
    if (FAILED(pInstance->Initialize(_vCreatePos)))
        MSG_BOX("Failed to Create : CNPC_Henry");

    return pInstance;
}

HRESULT CNPC_Henry::AddRigidBody(string _strModelKey)
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
    CapsuleDesc.strShapeTag = "NPC_Henry_Body";
    RigidBodyDesc.pGeometry = &CapsuleDesc;

    RigidBodyDesc.eThisColFlag = COL_NPC;
    RigidBodyDesc.eTargetColFlag = COL_STATIC | COL_PLAYER;
    m_pRigidBody = GAMEINSTANCE->CloneRigidBody(&RigidBodyDesc);

    m_pTransformCom->SetRigidBody(m_pRigidBody);
    m_pRigidBody->SetMass(20.0f);
    return S_OK;
}

void CNPC_Henry::OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

void CNPC_Henry::OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

void CNPC_Henry::OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

HRESULT CNPC_Henry::AddPartObjects(string _strModelKey)
{
    shared_ptr<CWeapon::WEAPON_DESC> pWeaponDesc = make_shared<CWeapon::WEAPON_DESC>();
    pWeaponDesc->tPartObject.pParentTransform = m_pTransformCom;
    pWeaponDesc->pSocketMatrix = m_pModel->GetCombinedBoneMatrixPtr("Bn_Action_LeftHand");
    pWeaponDesc->strModelKey = _strModelKey;
    pWeaponDesc->OffsetMatrixNormal = SimpleMath::Matrix::CreateRotationX(XMConvertToRadians(-90.f)) *
        SimpleMath::Matrix::CreateRotationZ(XMConvertToRadians(-90.f));

    shared_ptr<CTransform::TRANSFORM_DESC> pTransformDesc = make_shared<CTransform::TRANSFORM_DESC>();

    pTransformDesc->fSpeedPerSec = 10.f;
    pTransformDesc->fRotationPerSec = XMConvertToRadians(90.0f);

    shared_ptr<CWeapon> pWeapon = CWeapon::Create(pWeaponDesc.get(), pTransformDesc.get());
    if (nullptr == pWeapon)
        return E_FAIL;

    shared_ptr<CPartObject> pPartObject = static_pointer_cast<CPartObject>(pWeapon);
    m_pNPCParts.emplace("Part_Weapon", pPartObject);

    return S_OK;
}


void CNPC_Henry::UpdateDistanceState()
{
    // ��ġ �ٷ� ���޾ƿ��°� ������ �켱
    if (m_PlayerPos == _float3(0.f, 0.f, 0.f))
    {
        m_iDistanceState = HENRY_D_FARRANGE;
        return;
    }

    if (m_fDistanceXZToPlayer < m_fCloseRange)
    {
        m_iDistanceState = HENRY_D_READYRANGE;
    }
    else if (m_fDistanceXZToPlayer < m_fMeetRange)
    {
        m_iDistanceState = HENRY_D_MEETRANGE;
    }
    else
    {
        m_iDistanceState = HENRY_D_FARRANGE;
    }
}

void CNPC_Henry::FSM_BeforeMeet(_float _fTimeDelta)
{
    //UpdateDirection();
}

void CNPC_Henry::FSM_Meet(_float _fTimeDelta)
{
    
}

void CNPC_Henry::FSM_Ready(_float _fTimeDelta)
{
}

void CNPC_Henry::FSM_Normal(_float _fTimeDelta)
{
}

void CNPC_Henry::InteractionOn()
{
    console("Henry\n");

    /* ��ǳ�� */
    UIMGR->SetCurNPC(static_pointer_cast<CNPC>(shared_from_this())); // ��ǳ���� ���� Bind
    //UIMGR->SetSpeechBallonContent(L"Henry", L"ũ�ƾƾƾƾƾ�"); // ��ǳ�� ������Ʈ

    /* GetCurrentQuestKey�� üũ �� ������ ���� �� return */


    /* ������ �ƹ��͵� ���ٸ� default ���� */
    DIALOGUEMGR->SetCurDefaultDialogue(L"Henry");
}