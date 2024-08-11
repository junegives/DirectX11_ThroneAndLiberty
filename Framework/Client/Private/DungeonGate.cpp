#include "DungeonGate.h"
#include "GameInstance.h"
#include "Model.h"
#include "RigidBody.h"
#include "PlayerProjectile.h"
#include "Statue.h"
#include "QuestMgr.h"
#include "UIMgr.h"

CDungeonGate::CDungeonGate()
{
}

HRESULT CDungeonGate::Initialize(_float3 _vPosition)
{
    CInteractionObj::Initialize(nullptr);

    m_pModel = GAMEINSTANCE->GetModel("BG_HumanStatue_08_01_SM");
    AddRigidBody("BG_HumanStatue_08_01_SM");
    m_pTransformCom->SetScaling(0.55f, 0.4f, 0.55f);

    m_eShaderType = ST_NONANIM;
    m_iShaderPass = 0;

    m_vUIDistance = _float3(0.f, 1.f, 0.f);
    m_pTransformCom->SetState(CTransform::STATE_POSITION, _vPosition);
    m_pTransformCom->SetSpeed(30.f); // 1�ʿ� ������ �ӵ� ����

    m_fInteractDistance = 8.f;
    
    return S_OK;
}

void CDungeonGate::PriorityTick(_float _fTimeDelta)
{
    __super::PriorityTick(_fTimeDelta);

    m_vCurrentPos = _float3(m_pTransformCom->GetState(CTransform::STATE_POSITION));

    if (GAMEINSTANCE->KeyDown(DIK_9))
    {
        //MovePos(m_vCurrentPos + _float3(1.f, 0.f, 0.f));
        m_bOpen = true;
        m_bAllStatueCorrect = true;
        m_fCalculateTime1 = 0.f;
        GAMEINSTANCE->PlaySoundW("DStatueMove", 1.5f);
    }
}

void CDungeonGate::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);
    
    // �ٸ� ��������� �°� ��ġ�Ǿ�����
    if (!m_bAllStatueCorrect)
    {
        if (m_pMatchDungeonGate1.lock()->GetMatchCorrect() &&
            m_pMatchDungeonGate2.lock()->GetMatchCorrect() &&
            m_pMatchDungeonGate3.lock()->GetMatchCorrect() &&
            m_pMatchDungeonGate4.lock()->GetMatchCorrect())
        {
            m_bAllStatueCorrect = true;
            m_bOpen = true;
            m_fCalculateTime1 = 0.f;
            m_bInteractAble = false;
            GAMEINSTANCE->PlaySoundW("DStatueMove", 1.5f);
        }
    }

    // ������ ������
    if (m_bOpen)
    {
        if (MoveSlide(_fTimeDelta))
        {
            QUESTMGR->ActiveEventTag(QUEST_MAIN, "Event_EnterUnderWorld");
            m_bOpen = false;
        }
    }
    m_pTransformCom->Tick(_fTimeDelta);
}

void CDungeonGate::LateTick(_float _fTimeDelta)
{
    m_pTransformCom->LateTick(_fTimeDelta);

    __super::LateTick(_fTimeDelta);

    if (GAMEINSTANCE->isInWorldSpace(m_pTransformCom->GetState(CTransform::STATE_POSITION), 15.0f))
    {
        GAMEINSTANCE->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this());

#ifdef _DEBUG
        if (m_pRigidBody)
            GAMEINSTANCE->AddDebugComponent(m_pRigidBody);
#endif // _DEBUG
    }
}

HRESULT CDungeonGate::Render()
{
    GAMEINSTANCE->ReadyShader(m_eShaderType, m_iShaderPass);
    GAMEINSTANCE->BindWVPMatrixPerspective(m_pTransformCom->GetWorldMatrix());

    _uint iNumMeshes = m_pModel->GetNumMeshes();

    if(m_bAllStatueCorrect)
        UsingRimLight(_float3(1.f, 1.f, 1.f), 0.5f);

    for (size_t i = 0; i < iNumMeshes; i++) {

        GAMEINSTANCE->BeginNonAnimModel(m_pModel, (_uint)i);
    }

    /*RimLight Off*/
    _bool bRimOff = false;
    GAMEINSTANCE->BindRawValue("g_IsUsingRim", &bRimOff, sizeof(bRimOff));

    return S_OK;
}

void CDungeonGate::SimulationOn()
{
    m_pRigidBody->SimulationOn();
}

void CDungeonGate::SimulationOff()
{
    m_pRigidBody->SimulationOn();
}

void CDungeonGate::InteractionOn()
{
    if (!m_bAllStatueCorrect)
    {
        UIMGR->SetInteractionPopUpContent(L"���� ��Ű�� ��ȣ�ڵ��� ������ ������ �����Ͽ�\n������ ��ã���� ���� ��μ� ������ ��������");
        UIMGR->ActivateInteractionPopUp();
    }
}

HRESULT CDungeonGate::AddRigidBody(string _strModelKey)
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

    RigidBodyDesc.isStatic = true;
    RigidBodyDesc.isTrigger = false;
    RigidBodyDesc.isGravity = true;
    RigidBodyDesc.isInitCol = true;
    RigidBodyDesc.pTransform = m_pTransformCom;
    RigidBodyDesc.eLock_Flag = CRigidBody::LOCK_ROT_ALL;

    RigidBodyDesc.pOwnerObject = shared_from_this();
    RigidBodyDesc.fStaticFriction = 1.0f;
    RigidBodyDesc.fDynamicFriction = 1.0f;
    RigidBodyDesc.fRestitution = 0.0f;
    
    GeometryBox RectDesc;
    RectDesc.vSize.x = 9.f;
    RectDesc.vSize.y = 20.f;
    RectDesc.vSize.z = 9.f;
    RectDesc.vOffSetPosition = { 0.f, 10.f, 0.f };
    RectDesc.vOffSetRotation = { 0.0f, 0.0f, 0.0f };
    RectDesc.strShapeTag = "DungeonGate_Body";
    m_strModelName = _strModelKey;
    RigidBodyDesc.pGeometry = &RectDesc;

    RigidBodyDesc.eThisColFlag = COL_STATIC;
    RigidBodyDesc.eTargetColFlag = COL_STATIC | COL_PLAYER | COL_PLAYERPROJECTILE;
    m_pRigidBody = GAMEINSTANCE->CloneRigidBody(&RigidBodyDesc);

    m_pTransformCom->SetRigidBody(m_pRigidBody);
    m_pRigidBody->SetMass(20.0f);
    return S_OK;
}

void CDungeonGate::OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

void CDungeonGate::OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

void CDungeonGate::OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

_bool CDungeonGate::MoveSlide(_float _fTimeDelta)
{
    m_fCalculateTime1 += _fTimeDelta;

    MovePos(_float3(m_vCurrentPos.x + _fTimeDelta * 1.5f, m_vCurrentPos.y, m_vCurrentPos.z));

    if (m_fCalculateTime1 > 6.f)
    {
        m_fCalculateTime1 = 0.f;
        return true;
    }
    else
    {
        return false;
    }
}

void CDungeonGate::MovePos(_float3 _vPos)
{
    m_pTransformCom->SetState(CTransform::STATE_POSITION, _vPos);
    m_vCurrentPos = _vPos;
}

shared_ptr<CDungeonGate> CDungeonGate::Create(_float3 _vPosition)
{
    shared_ptr<CDungeonGate> pDungeonGate = make_shared<CDungeonGate>();

    if (FAILED(pDungeonGate->Initialize(_vPosition)))
        MSG_BOX("Failed to Create : CDungeonGate");

    return pDungeonGate;
}

