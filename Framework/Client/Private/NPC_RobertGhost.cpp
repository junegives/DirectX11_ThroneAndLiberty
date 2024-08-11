#include "NPC_RobertGhost.h"
#include "GameInstance.h"
#include "Model.h"
#include "Weapon.h"
#include "RigidBody.h"
#include "Animation.h"
#include "EffectMgr.h"
#include "DialogueMgr.h"
#include "QuestMgr.h"
#include "Weapon.h"
#include "UIMgr.h"

CNPC_RobertGhost::CNPC_RobertGhost()
    : CNPC()
{
}

HRESULT CNPC_RobertGhost::Initialize(_float3 _vCreatePos)
{
    // ������ ��ġ�� ����
    __super::Initialize(_vCreatePos);

    /*Shader Info*/
    m_eShaderType = ESHADER_TYPE::ST_ANIM;
    /*Trail Pass Number*/
    m_iShaderPass = 6;

    m_pModel = GAMEINSTANCE->GetModel("CT_Named_Robert_SK");
    m_strModelName = "CT_Named_RobertGhost_SK";
    AddComponent(L"Com_Model", m_pModel);
    m_pTransformCom->SetScaling(1.f, 1.f, 1.f);
    //m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), 3.14f);

    m_pModel->ChangeAnim(m_iActionState, 0.1f, true);
    
    if (FAILED(AddPartObjects("IT_N_Bow_NamedRobert_SK")))
        return E_FAIL;

    m_fSpeed = 1.f;

    // ���� ������Ʈ ����
    SetWeaponRimLight(true, _float3(0.5f, 0.8f, 1.f), 0.5f);
    
    AddRigidBody("CT_Named_Robert_SK");
    //m_pRigidBody->SimulationOn();

    PrepareNPCBar(L"RobertGhost");
    m_strDisplayName = L"RobertGhost";

    return S_OK;
}

void CNPC_RobertGhost::PriorityTick(_float _fTimeDelta)
{
    __super::PriorityTick(_fTimeDelta);
}

void CNPC_RobertGhost::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);

    // Base���¿� ���� FSM
    switch (m_iBaseState)
    {
    case ROBERT_G_B_BEFOREMEET:
        FSM_BeforeMeet(_fTimeDelta);
        break;
    case ROBERT_G_B_MEET:
        FSM_Meet(_fTimeDelta);
        break;
    case ROBERT_G_B_INTERACT:
        FSM_Interact(_fTimeDelta);
        break;
    }

    m_pModel->PlayAnimation(_fTimeDelta);

    m_pTransformCom->Tick(_fTimeDelta);
}

void CNPC_RobertGhost::LateTick(_float _fTimeDelta)
{
    //m_pModelCom->CheckDisableAnim();

    m_pTransformCom->LateTick(_fTimeDelta);

    __super::LateTick(_fTimeDelta);

    // frustrum
	if (GAMEINSTANCE->isInWorldSpace(m_pTransformCom->GetState(CTransform::STATE_POSITION), 3.f))
	{
		GAMEINSTANCE->AddRenderGroup(CRenderer::RENDER_TRAIL, shared_from_this());

#ifdef _DEBUG
		if (m_pRigidBody)
			GAMEINSTANCE->AddDebugComponent(m_pRigidBody);
#endif // _DEBUG
	}

}

HRESULT CNPC_RobertGhost::Render()
{
    GAMEINSTANCE->ReadyShader(m_eShaderType, m_iShaderPass);
    GAMEINSTANCE->BindWVPMatrixPerspective(m_pTransformCom->GetWorldMatrix());
    _uint iNumMeshes = m_pModel->GetNumMeshes();

    UsingRimLight(_float3(0.5f, 0.8f, 1.f), 0.5f);

    for (size_t i = 0; i < iNumMeshes; i++) {
        GAMEINSTANCE->BeginAnimModel(m_pModel, (_uint)i);
    }

    /*RimLight Off*/
    _bool bRimOff = false;
    GAMEINSTANCE->BindRawValue("g_IsUsingRim", &bRimOff, sizeof(bRimOff));

    return S_OK;
}

shared_ptr<CNPC_RobertGhost> CNPC_RobertGhost::Create(_float3 _vCreatePos, _float3 _vInitLookPos)
{
    shared_ptr<CNPC_RobertGhost> pInstance = make_shared<CNPC_RobertGhost>();
    pInstance->SetInitLookPos(_vInitLookPos);
    if (FAILED(pInstance->Initialize(_vCreatePos)))
        MSG_BOX("Failed to Create : CNPC_RobertGhost");

    return pInstance;
}

HRESULT CNPC_RobertGhost::AddRigidBody(string _strModelKey)
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
    CapsuleDesc.strShapeTag = "NPC_RobertGhost_Body";
    RigidBodyDesc.pGeometry = &CapsuleDesc;

    RigidBodyDesc.eThisColFlag = COL_NPC;
    RigidBodyDesc.eTargetColFlag = COL_STATIC | COL_PLAYER;
    m_pRigidBody = GAMEINSTANCE->CloneRigidBody(&RigidBodyDesc);

    m_pTransformCom->SetRigidBody(m_pRigidBody);
    m_pRigidBody->SetMass(20.0f);
    return S_OK;
}

void CNPC_RobertGhost::OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

void CNPC_RobertGhost::OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

void CNPC_RobertGhost::OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

HRESULT CNPC_RobertGhost::AddPartObjects(string _strModelKey)
{
    shared_ptr<CWeapon::WEAPON_DESC> pWeaponDesc = make_shared<CWeapon::WEAPON_DESC>();
    pWeaponDesc->tPartObject.pParentTransform = m_pTransformCom;
    pWeaponDesc->pSocketMatrix = m_pModel->GetCombinedBoneMatrixPtr("Bn_Action_LeftHand");
    pWeaponDesc->OffsetMatrixNormal = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(90.f)) *
        SimpleMath::Matrix::CreateRotationX(XMConvertToRadians(90.f));

    pWeaponDesc->strModelKey = _strModelKey;
    
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


void CNPC_RobertGhost::UpdateDistanceState()
{
    // ��ġ �ٷ� ���޾ƿ��°� ������ �켱
    if (m_PlayerPos == _float3(0.f, 0.f, 0.f))
    {
        m_iDistanceState = ROBERT_G_D_FARRANGE;
        return;
    }

    if (m_fDistanceXZToPlayer < m_fMeetRange)
    {
        m_iDistanceState = ROBERT_G_D_MEETRANGE;
    }
    else
    {
        m_iDistanceState = ROBERT_G_D_FARRANGE;
    }
}

void CNPC_RobertGhost::FSM_BeforeMeet(_float _fTimeDelta)
{
    // ������ �����ֱ�
    if (m_iActionState == ROBERT_G_A_TURNR80)
    {
        if (m_pModel->GetIsFinishedAnimation())
        {
            m_iActionState = ROBERT_G_A_IDLE;
            m_pModel->ChangeAnim(m_iActionState, 0.f, true);
            m_pTransformCom->RotationPlus(_float3(0.f, 1.f, 0.f), 3.14f / 180.f * 80.f);
        }
    }
    if (m_iActionState == ROBERT_G_A_TURNL80)
    {
        if (m_pModel->GetIsFinishedAnimation())
        {
            m_iActionState = ROBERT_G_A_IDLE;
            m_pModel->ChangeAnim(m_iActionState, 0.f, true);
            m_pTransformCom->RotationPlus(_float3(0.f, 1.f, 0.f), 3.14f / 180.f * -80.f);
        }
    }
    if (m_iActionState == ROBERT_G_A_TURNR130)
    {
        if (m_pModel->GetIsFinishedAnimation())
        {
            m_iActionState = ROBERT_G_A_IDLE;
            m_pModel->ChangeAnim(m_iActionState, 0.f, true);
            m_pTransformCom->RotationPlus(_float3(0.f, 1.f, 0.f), 3.14f / 180.f * 130.f);
        }
    }
    if (m_iActionState == ROBERT_G_A_TURNL130)
    {
        if (m_pModel->GetIsFinishedAnimation())
        {
            m_iActionState = ROBERT_G_A_IDLE;
            m_pModel->ChangeAnim(m_iActionState, 0.f, true);
            m_pTransformCom->RotationPlus(_float3(0.f, 1.f, 0.f), 3.14f / 180.f * -130.f);
        }
    }

    if (m_iActionState == ROBERT_G_A_IDLE)
    {
        UpdateDistanceState();
        if (m_iDistanceState == ROBERT_G_D_MEETRANGE)
        {
            // ���� ���� 36�� �ȿ� �÷��̾ �ִٸ�
            if (IsInPlayerDegree(m_fDegree1))
            {
                m_iBaseState = ROBERT_G_B_MEET;
                m_bInteractAble = true;
            }
            // 108�� ���� ���̸�
            else if (IsInPlayerDegree(m_fDegree2))
            {
                // �÷��̾ �����ʿ� ������
                if (IsPlayerRight())
                {
                    m_iActionState = ROBERT_G_A_TURNR80;
                    m_pModel->ChangeAnim(m_iActionState, 0.1f, false);
                    m_iBaseState = ROBERT_G_B_MEET;
                    m_bTurnRight = true;
                }
                else
                {
                    m_iActionState = ROBERT_G_A_TURNL80;
                    m_pModel->ChangeAnim(m_iActionState, 0.1f, false);
                    m_iBaseState = ROBERT_G_B_MEET;
                    m_bTurnLeft = true;
                }
            }
            // 179�� ���� ���̸�
            else if (IsInPlayerDegree(m_fDegree3))
            {
                // �÷��̾ �����ʿ� ������
                if (IsPlayerRight())
                {
                    m_iActionState = ROBERT_G_A_TURNR130;
                    m_pModel->ChangeAnim(m_iActionState, 0.1f, false);
                    m_iBaseState = ROBERT_G_B_MEET;
                    m_bTurnRight130 = true;
                }
                else
                {
                    m_iActionState = ROBERT_G_A_TURNL130;
                    m_pModel->ChangeAnim(m_iActionState, 0.1f, false);
                    m_iBaseState = ROBERT_G_B_MEET;
                    m_bTurnLeft130 = true;
                }
            }
        }
    }
}

void CNPC_RobertGhost::FSM_Meet(_float _fTimeDelta)
{
    // ������ �����ֱ�
    if (m_iActionState == ROBERT_G_A_TURNR80)
    {
        if (m_pModel->GetIsFinishedAnimation())
        {
            m_iActionState = ROBERT_G_A_IDLE;
            m_pModel->ChangeAnim(m_iActionState, 0.f, true);
            m_pTransformCom->RotationPlus(_float3(0.f, 1.f, 0.f), 3.14f / 180.f * 80.f);
            m_bInteractAble = true;
        }
    }
    if (m_iActionState == ROBERT_G_A_TURNL80)
    {
        if (m_pModel->GetIsFinishedAnimation())
        {
            m_iActionState = ROBERT_G_A_IDLE;
            m_pModel->ChangeAnim(m_iActionState, 0.f, true);
            m_pTransformCom->RotationPlus(_float3(0.f, 1.f, 0.f), 3.14f / 180.f * -80.f);
            m_bInteractAble = true;
        }
    }
    if (m_iActionState == ROBERT_G_A_TURNR130)
    {
        if (m_pModel->GetIsFinishedAnimation())
        {
            m_iActionState = ROBERT_G_A_IDLE;
            m_pModel->ChangeAnim(m_iActionState, 0.f, true);
            m_pTransformCom->RotationPlus(_float3(0.f, 1.f, 0.f), 3.14f / 180.f * 130.f);
            m_bInteractAble = true;
        }
    }
    if (m_iActionState == ROBERT_G_A_TURNL130)
    {
        if (m_pModel->GetIsFinishedAnimation())
        {
            m_iActionState = ROBERT_G_A_IDLE;
            m_pModel->ChangeAnim(m_iActionState, 0.f, true);
            m_pTransformCom->RotationPlus(_float3(0.f, 1.f, 0.f), 3.14f / 180.f * -130.f);
            m_bInteractAble = true;
        }
    }

    if (m_iActionState == ROBERT_G_A_IDLE)
    {
        UpdateDistanceState();
        // �ٽ� �־�����
        if (m_iDistanceState == ROBERT_G_D_FARRANGE)
        {
            if (m_bTurnRight)
            {
                m_iActionState = ROBERT_G_A_TURNL80;
                m_pModel->ChangeAnim(m_iActionState, 0.1f, false);
                m_iBaseState = ROBERT_G_B_BEFOREMEET;
                m_bTurnRight = false;
                m_bInteractAble = false;
            }
            else if (m_bTurnLeft)
            {
                m_iActionState = ROBERT_G_A_TURNR80;
                m_pModel->ChangeAnim(m_iActionState, 0.1f, false);
                m_iBaseState = ROBERT_G_B_BEFOREMEET;
                m_bTurnLeft = false;
                m_bInteractAble = false;
            }
            else if (m_bTurnRight130)
            {
                m_iActionState = ROBERT_G_A_TURNL130;
                m_pModel->ChangeAnim(m_iActionState, 0.1f, false);
                m_iBaseState = ROBERT_G_B_BEFOREMEET;
                m_bTurnRight130 = false;
                m_bInteractAble = false;
            }
            else if (m_bTurnLeft130)
            {
                m_iActionState = ROBERT_G_A_TURNR130;
                m_pModel->ChangeAnim(m_iActionState, 0.1f, false);
                m_iBaseState = ROBERT_G_B_BEFOREMEET;
                m_bTurnLeft130 = false;
                m_bInteractAble = false;
            }
            else
            {
                m_iBaseState = ROBERT_G_B_BEFOREMEET;
                m_bInteractAble = false;
            }
        }
        // ������
        else
        {
        }
    }
}

void CNPC_RobertGhost::FSM_Interact(_float _fTimeDelta)
{
    UpdateDistanceState();
    if (m_iDistanceState == ROBERT_G_D_FARRANGE)
    {
        if (m_bTurnRight)
        {
            m_iActionState = ROBERT_G_A_TURNL80;
            m_pModel->ChangeAnim(m_iActionState, 0.1f, false);
            m_iBaseState = ROBERT_G_B_BEFOREMEET;
            m_bTurnRight = false;
            m_bInteractAble = false;
        }
        else if (m_bTurnLeft)
        {
            m_iActionState = ROBERT_G_A_TURNR80;
            m_pModel->ChangeAnim(m_iActionState, 0.1f, false);
            m_iBaseState = ROBERT_G_B_BEFOREMEET;
            m_bTurnLeft = false;
            m_bInteractAble = false;
        }
        else if (m_bTurnRight130)
        {
            m_iActionState = ROBERT_G_A_TURNL130;
            m_pModel->ChangeAnim(m_iActionState, 0.1f, false);
            m_iBaseState = ROBERT_G_B_BEFOREMEET;
            m_bTurnRight130 = false;
            m_bInteractAble = false;
        }
        else if (m_bTurnLeft130)
        {
            m_iActionState = ROBERT_G_A_TURNR130;
            m_pModel->ChangeAnim(m_iActionState, 0.1f, false);
            m_iBaseState = ROBERT_G_B_BEFOREMEET;
            m_bTurnLeft130 = false;
            m_bInteractAble = false;
        }
        else
        {
            m_iActionState = ROBERT_G_A_IDLE;
            m_pModel->ChangeAnim(m_iActionState, 0.1f, true);
            m_iBaseState = ROBERT_G_B_BEFOREMEET;
            m_bInteractAble = false;
        }
    }
}

void CNPC_RobertGhost::InteractionOn()
{
    if (m_iBaseState == ROBERT_G_B_MEET)
    {
        m_iBaseState = ROBERT_G_B_INTERACT;
        m_iActionState = ROBERT_G_A_BASE;
        m_pModel->ChangeAnim(m_iActionState, 0.5f, true);
    }

    if ((TEXT("�� �� ���� ����") == QUESTMGR->GetCurrentQuestKey(QUEST_SUB0) ||
        TEXT("�� �� ���� ����") == QUESTMGR->GetCurrentQuestKey(QUEST_SUB1)))
    {
        QUESTMGR->CheckDialogNPC(shared_from_this());
        DIALOGUEMGR->SetCurDialogue(TEXT("Robert1"));

    }
    else if ((TEXT("��ȥ���� �Ƚ���") == QUESTMGR->GetCurrentQuestKey(QUEST_SUB0) ||
        TEXT("��ȥ���� �Ƚ���") == QUESTMGR->GetCurrentQuestKey(QUEST_SUB1)))
    {
        QUESTMGR->CheckDialogNPC(shared_from_this());
        DIALOGUEMGR->SetCurDialogue(TEXT("Robert2"));

    }

}