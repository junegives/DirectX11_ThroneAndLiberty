#include "Statue.h"
#include "GameInstance.h"
#include "Model.h"
#include "RigidBody.h"
#include "PlayerProjectile.h"
#include "UIMgr.h"
#include "UIPanel.h"

CStatue::CStatue()
{
}

HRESULT CStatue::Initialize(_float3 _vPosition)
{
    CInteractionObj::Initialize(nullptr);

    LIGHT_DESC desc = {};

    desc.bUsingNoise = true;
    desc.eType = LIGHT_DESC::TYPE_POINT;
    desc.fCullDistance = 20.f;
    desc.fRange = 6.f;

    switch (m_iStatueType)
    {
    case ST_ENGEL:
        m_vRimLightColor = _float3(0.3f, 0.3f, 0.95f);      // 
        m_pModel = GAMEINSTANCE->GetModel("BG_AngelStatue_02_01_SM");
        AddRigidBody("BG_AngelStatue_02_01_SM");
        m_pTransformCom->SetScaling(0.9f, 0.9f, 0.9f);
        // ���� ��
        if (m_bSpellCollisionAble)
            m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), -3.14f * 0.25f);
        else
            m_iTrueSpinCount = 3;
        break;
    case ST_LEEPER:
        m_vRimLightColor = _float3(0.75f, 0.7f, 0.2f);
        m_pModel = GAMEINSTANCE->GetModel("BG_HumanStatue_05_02_SM");
        AddRigidBody("BG_HumanStatue_05_02_SM");
        m_pTransformCom->SetScaling(1.1f, 1.1f, 1.1f);
        // ������ ��
        if (m_bSpellCollisionAble)
            m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), 3.14f * 0.25f);
        else
            m_iTrueSpinCount = 5;
        break;
    case ST_BLIND:
        m_vRimLightColor = _float3(0.9f, 0.3f, 0.3f);
        m_pModel = GAMEINSTANCE->GetModel("BG_HumanStatue_26_01_SM");
        AddRigidBody("BG_HumanStatue_26_01_SM");
        m_pTransformCom->SetScaling(0.7f, 0.7f, 0.7f);
        // ���� �Ʒ�
        if (m_bSpellCollisionAble)
            m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), -3.14f * 0.75f);
        else
            m_iTrueSpinCount = 1;
        break;
    case ST_PRIEST:
        m_vRimLightColor = _float3(0.1f, 0.8f, 0.5f);
        m_pModel = GAMEINSTANCE->GetModel("BG_HumanStatue_13_04_SM");
        AddRigidBody("BG_HumanStatue_13_04_SM");
        m_pTransformCom->SetScaling(1.6f, 1.6f, 1.6f);
        // ������ �Ʒ�
        if (m_bSpellCollisionAble)
            m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), 3.14f * 0.75f);
        else
            m_iTrueSpinCount = 7;
        break;
    }

    desc.vDiffuse = _float4(m_vRimLightColor.x, m_vRimLightColor.y, m_vRimLightColor.z, 1.f);
    desc.vSpecular = desc.vDiffuse;
    desc.vPosition = _float4(_vPosition.x, _vPosition.y + 3.f, _vPosition.z, 1.f);

    if (FAILED(GAMEINSTANCE->AddLight(desc, &m_iLightIdx)))
        return E_FAIL;

    m_eShaderType = ST_NONANIM;
    m_iShaderPass = 0;

    m_fInteractDistance = 2.f;

    m_vUIDistance = _float3(0.f, 1.f, 0.f);
    m_pTransformCom->SetState(CTransform::STATE_POSITION, _vPosition);
    m_pTransformCom->SetTurnSpeed(3.14f * 0.25f);    // 1�ʿ� 45�� ȸ���ϵ��� ����
    m_pTransformCom->SetSpeed(5.f);
    
    return S_OK;
}

void CStatue::PriorityTick(_float _fTimeDelta)
{
    __super::PriorityTick(_fTimeDelta);

}

void CStatue::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);
    
    // ��ȣ�ۿ�� 1�ʰ��� ������ ����
    if (m_bStatueInteractOn)
    {
#pragma region UI
        m_bInteractAble = false;

        shared_ptr<CUIBase> arrowPanel = UIMGR->FindUI("RotationArrowPanel");
        if (arrowPanel == nullptr)
            return;

        _float3 playerPos = m_pPlayer.lock()->GetTransform()->GetState(CTransform::STATE_POSITION);
        _float3 monsterPos = GetTransform()->GetState(CTransform::STATE_POSITION);
        _float dist = SimpleMath::Vector3::Distance(playerPos, monsterPos);

        _float4x4 followWorldMat{};
        _float3 followPos{};

        followPos = monsterPos + _float3(0.f, 1.5f, 0.f);
        followPos = SimpleMath::Vector3::Transform(followPos, GAMEINSTANCE->GetTransformMatrix(CPipeLine::D3DTS_VIEW));
        followPos = SimpleMath::Vector3::Transform(followPos, GAMEINSTANCE->GetTransformMatrix(CPipeLine::D3DTS_PROJ));

        if (followPos.z <= 0.f || followPos.z > 1.f)
            arrowPanel->SetIsRender(false);

        _float posX = (followPos.x + 1.f) * 0.5f;
        _float posY = (followPos.y - 1.f) * -0.5f;

        posX = g_iWinSizeX * posX - (g_iWinSizeX * 0.5f);
        posY = g_iWinSizeY * -posY + (g_iWinSizeY * 0.5f);

        /* LockOnUI ã�Ƽ� Pos Set */
        arrowPanel->SetUIPos(posX, posY);

        UIMGR->ActivateInteractionStatue();
        UIMGR->DeactivateInteractionUI();

#pragma endregion

        if (Rotatate45Degree(_fTimeDelta))
        {
            GAMEINSTANCE->StopSound("DStatueRotate");
            UIMGR->DeactivateInteractionStatue(); // For UI
            m_bInteractAble = true; // For UI

            m_bStatueInteractOn = false;
            StatueRotateCount();
        }
    }

    // �������� �� Ƚ���� �˸°� ¦������ ������ ������
     if (!m_bSpellCollisionAble && m_iSpinCount == m_iTrueSpinCount)
    {
         if (m_pMatchStatue.lock() != nullptr)
         {
             if (m_pMatchStatue.lock()->GetSpellHitted())
             {
                 m_bMatchCorrect = true;
             }
         }
    }
    m_pTransformCom->Tick(_fTimeDelta);
}

void CStatue::LateTick(_float _fTimeDelta)
{
    m_pTransformCom->LateTick(_fTimeDelta);

    __super::LateTick(_fTimeDelta);

    if (GAMEINSTANCE->isInWorldSpace(m_pTransformCom->GetState(CTransform::STATE_POSITION), 10.0f))
    {
        GAMEINSTANCE->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this());

#ifdef _DEBUG
        if (m_pRigidBody)
            GAMEINSTANCE->AddDebugComponent(m_pRigidBody);
#endif // _DEBUG
    }

    if (m_bSpellHitted || m_bMatchCorrect)
    {
        GAMEINSTANCE->SetLightEnabled(m_iLightIdx, true);
    }
    else {
        GAMEINSTANCE->SetLightEnabled(m_iLightIdx, false);
    }

}

HRESULT CStatue::Render()
{
    GAMEINSTANCE->ReadyShader(m_eShaderType, m_iShaderPass);
    GAMEINSTANCE->BindWVPMatrixPerspective(m_pTransformCom->GetWorldMatrix());

    _uint iNumMeshes = m_pModel->GetNumMeshes();

    if (m_bSpellHitted || m_bMatchCorrect)
    {
        UsingRimLight(m_vRimLightColor, 2.f);
    }

    for (size_t i = 0; i < iNumMeshes; i++) {

        GAMEINSTANCE->BeginNonAnimModel(m_pModel, (_uint)i);
    }

    /*RimLight Off*/
    _bool bRimOff = false;
    GAMEINSTANCE->BindRawValue("g_IsUsingRim", &bRimOff, sizeof(bRimOff));

    return S_OK;
}

void CStatue::SimulationOn()
{
    m_pRigidBody->SimulationOn();
}

void CStatue::SimulationOff()
{
    m_pRigidBody->SimulationOn();
}

void CStatue::InteractionOn()
{
    if (m_bSpellCollisionAble)
    {
        switch (m_iStatueType)
        {
        case ST_ENGEL:
            UIMGR->SetInteractionPopUpContent(L"�׳�� ������� �ڽ��� ��ӵ� ��ſ� ���ư�\n�ᱹ ������ �����ȴ�");
            break;
        case ST_LEEPER:
            UIMGR->SetInteractionPopUpContent(L"���� ���� �ð��̳� �ٸ� �ð��� �׷��� ���ߴ�");
            break;
        case ST_BLIND:
            UIMGR->SetInteractionPopUpContent(L"���� ������ �ʴ� �׿��� �¸����� �������� �ۿ�����\n�����ߴ�");
            break;
        case ST_PRIEST:
            UIMGR->SetInteractionPopUpContent(L"�׳��� ������ �� ������ �ڵ����� �״� ���� ������\n��� �̾�� ������");
            break;
        }
        UIMGR->ActivateInteractionPopUp();
    }
    else
    {
    }

    // ��ȣ�ۿ��� �����ϰ� ����κ��ְ� ��Ī�� �ȸ¾�����
    if (!m_bStatueInteractOn && !m_bSpellCollisionAble && !m_bMatchCorrect)
    {
        m_bStatueInteractOn = true;
        GAMEINSTANCE->PlaySoundW("DStatueRotate", 5.f);
    }

}

HRESULT CStatue::AddRigidBody(string _strModelKey)
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
    RectDesc.vSize.x = 2.f;
    RectDesc.vSize.y = 6.f;
    RectDesc.vSize.z = 2.f;
    RectDesc.vOffSetPosition = { 0.f, 3.f, 0.f };
    RectDesc.vOffSetRotation = { 0.0f, 0.0f, 0.0f };
    RectDesc.strShapeTag = "Statue_Body";
    m_strModelName = _strModelKey;
    RigidBodyDesc.pGeometry = &RectDesc;

    RigidBodyDesc.eThisColFlag = COL_STATIC;
    RigidBodyDesc.eTargetColFlag = COL_STATIC | COL_PLAYER | COL_PLAYERPROJECTILE;
    m_pRigidBody = GAMEINSTANCE->CloneRigidBody(&RigidBodyDesc);

    m_pTransformCom->SetRigidBody(m_pRigidBody);
    m_pRigidBody->SetMass(20.0f);
    return S_OK;
}

void CStatue::OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
    if (_ColData.szShapeTag  == "Player_Spell_IceSpear" && m_iStatueType == ST_ENGEL && m_bSpellCollisionAble)
    {
        m_bSpellHitted = true;
    }
    if (_ColData.szShapeTag == "Player_Spell_LightnigJudgment" && m_iStatueType == ST_LEEPER && m_bSpellCollisionAble)
    {
        m_bSpellHitted = true;
    }
    if (_ColData.szShapeTag == "Player_Spell_Meteor" && m_iStatueType == ST_BLIND && m_bSpellCollisionAble)
    {
        m_bSpellHitted = true;
    }
    if (_ColData.szShapeTag == "Player_Spell_FireBlast" && m_iStatueType == ST_PRIEST && m_bSpellCollisionAble)
    {
        m_bSpellHitted = true;
    }
}

void CStatue::OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

void CStatue::OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

_bool CStatue::Rotatate45Degree(_float _fTimeDelta)
{
    m_fCalculateTime1 += _fTimeDelta;
    m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), _fTimeDelta);
    if (m_fCalculateTime1 > 1.f)
    {
        m_fCalculateTime1 = 0.f;
        return true;
    }
    else
    {
        return false;
    }
}

shared_ptr<CStatue> CStatue::Create(_float3 _vPosition, _int _iStatueType, _bool _bGenType)
{
    shared_ptr<CStatue> pStatue = make_shared<CStatue>();

    
    pStatue->SetStatueType(_iStatueType);
    pStatue->SetSpellCollisionAble(_bGenType);
    if (FAILED(pStatue->Initialize(_vPosition)))
        MSG_BOX("Failed to Create : CStatue");
    

    return pStatue;
}

void CStatue::StatueRotateCount()
{
    m_iSpinCount++;
    if (m_iSpinCount > 7)
    {
        m_iSpinCount = 0;
    }
}
