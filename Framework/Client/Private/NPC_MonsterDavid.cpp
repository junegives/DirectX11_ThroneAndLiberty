#include "NPC_MonsterDavid.h"
#include "Model.h"
#include "Weapon.h"
#include "RigidBody.h"
#include "Animation.h"
#include "EffectMgr.h"
#include "PlayerProjectile.h"
#include "NPC_David.h"

CNPC_MonsterDavid::CNPC_MonsterDavid()
    : CMonster()
{
}

HRESULT CNPC_MonsterDavid::Initialize()
{
    shared_ptr<CTransform::TRANSFORM_DESC> pArg = make_shared<CTransform::TRANSFORM_DESC>();

    pArg->fSpeedPerSec = 1.f;
    pArg->fRotationPerSec = XMConvertToRadians(1.0f);

    shared_ptr<MONSTER_DESC> pMonsterDesc = make_shared<CMonster::MONSTER_DESC>();
    pMonsterDesc->pTarget = GAMEINSTANCE->GetGameObject(LEVEL_STATIC, TEXT("Layer_Player"));

    __super::Initialize(pMonsterDesc.get(), pArg.get());
    
    /*Shader Info*/
    m_eShaderType = ESHADER_TYPE::ST_ANIM;
    m_iShaderPass = 0;

    m_pModelCom = GAMEINSTANCE->GetModel("CT_Named_ArthurTalon_SK");
    m_strModelName = "CT_Named_ArthurTalon_SK";
    AddComponent(L"Com_Model", m_pModelCom);
    m_pTransformCom->SetScaling(1.f, 1.f, 1.f);
    m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), 3.14f/2.f);

    m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);

    if (FAILED(AddPartObjects("Sword2h_Sample")))       // ��
        return E_FAIL;

    if (FAILED(AddPartObjects("Sword2h_Sample2")))      // ���
        return E_FAIL;

    m_pMonsterParts.find("Part_Weapon")->second->SetActiveOn(false);
    m_bHandWeaponRender = false;
    m_pMonsterParts.find("Part_InWeapon")->second->SetActiveOn(true);
    m_bBackWeaponRender = true;

    m_fSpeed = 5.f;
    m_fAnimSpeed = 1.f;

    AddRigidBody("CT_Named_ArthurTalon_SK");

    m_pRigidBody->SimulationOff();

    m_vLockOnOffSetPosition = { 0.0f,0.8f,0.0f };

    PrepareMonsterHPBar(TEXT("David"));

    m_pTransformCom->LookAtDir(_float3(0.f, 0.f, 0.f) - _float3(10.f, 0.f, 0.f));
    
    return S_OK;
}

void CNPC_MonsterDavid::PriorityTick(_float _fTimeDelta)
{
    __super::PriorityTick(_fTimeDelta);
}

void CNPC_MonsterDavid::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);

    // Base���¿� ��ȭ������ ���� ���� ����
    if (m_iBaseState == DAVIDMONSTER_B_BATTLENORMAL)
    {
        switch (m_iDialogOrder)
        {
        case DAVIDMONSTER_DI_PRACTICEMEET2:
            FSM_PracticeMeet2(_fTimeDelta);
            break;
        case DAVIDMONSTER_DI_PRACTICEMEET3:
            FSM_PracticeMeet3(_fTimeDelta);
            break;
        case DAVIDMONSTER_DI_PRACTICEMEET4:
            FSM_PracticeMeet4(_fTimeDelta);
            break;
        }
    }
    // �����̻�
    else
    {
        switch (m_iBaseState)
        {
        case DAVIDMONSTER_B_HITTED:
            FSM_Hitted(_fTimeDelta);
            break;
        case DAVIDMONSTER_B_HITTED2:
            FSM_Hitted2(_fTimeDelta);
            break;
        case DAVIDMONSTER_B_HITTED3:
            FSM_Hitted3(_fTimeDelta);
            break;
        case DAVIDMONSTER_B_STUN:
            FSM_Stun(_fTimeDelta);
            break;
        case DAVIDMONSTER_B_SHOCKED:
            FSM_Shocked(_fTimeDelta);
            break;
        case DAVIDMONSTER_B_DOWN:
            FSM_Down(_fTimeDelta);
            break;
        case DAVIDMONSTER_B_NONE:
            break;
        }
    }

    // �ִϸ��̼� �ӵ� ����
    for (int i = 0; i < 100; i++)
    {
        switch (i)
        {
        case DAVIDMONSTER_A_BLADEDANCE1:
            m_fActionAnimSpeed[i] = 2.f;
            break;
        case DAVIDMONSTER_A_BLADEDANCE2:
            m_fActionAnimSpeed[i] = 2.f;
            break;
        case DAVIDMONSTER_A_BLADEDANCE3:
            m_fActionAnimSpeed[i] = 2.f;
            break;
        case DAVIDMONSTER_A_BLOCKED:
            switch (m_iParriedPower)
            {
            case PARRIEDWEAK:
                m_fActionAnimSpeed[i] = 1.f;
                break;
            case PARRIEDNORMAL:
                m_fActionAnimSpeed[i] = 1.f;
                break;
            case PARRIEDSTRONG:
                m_fActionAnimSpeed[i] = 0.4f;
                break;
            }
            break;
        case DAVIDMONSTER_A_FRONTBE:
            switch (m_iBaseState)
            {
            case DAVIDMONSTER_B_HITTED:
                m_fActionAnimSpeed[i] = 3.f;
                break;
            case DAVIDMONSTER_B_HITTED2:
                m_fActionAnimSpeed[i] = 1.2f;
                break;
            case DAVIDMONSTER_B_HITTED3:
                m_fActionAnimSpeed[i] = 1.f;
                break;
            }
            break;
        case DAVIDMONSTER_A_ATT1:
            m_fActionAnimSpeed[i] = 2.f;
            break;
        case DAVIDMONSTER_A_ATT1_2:
            m_fActionAnimSpeed[i] = 1.4f;
            break;
        case DAVIDMONSTER_A_ATT2:
            m_fActionAnimSpeed[i] = 2.f;
            break;
        case DAVIDMONSTER_A_ATT2_2:
            m_fActionAnimSpeed[i] = 1.4f;
            break;
        default:
            m_fActionAnimSpeed[i] = 1.f;
            break;
        }
    }

    m_pModelCom->PlayAnimation(m_fActionAnimSpeed[m_iActionState] * m_fAnimSpeed * _fTimeDelta);

    m_pTransformCom->Tick(_fTimeDelta);
}

void CNPC_MonsterDavid::LateTick(_float _fTimeDelta)
{
    m_IsEdgeRender = false;

    m_pModelCom->CheckDisableAnim();

    m_pTransformCom->LateTick(_fTimeDelta);

    __super::LateTick(_fTimeDelta);

    // frustrum
    if (GAMEINSTANCE->isInWorldSpace(m_pTransformCom->GetState(CTransform::STATE_POSITION), 3.f))
    {
        GAMEINSTANCE->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this());
        GAMEINSTANCE->AddCascadeGroup(0, shared_from_this());

#ifdef _DEBUG
        if (m_pRigidBody)
            GAMEINSTANCE->AddDebugComponent(m_pRigidBody);
#endif // _DEBUG
    }
}

HRESULT CNPC_MonsterDavid::Render()
{
    __super::Render();

    _bool bRimOff = false;
    GAMEINSTANCE->BindRawValue("g_IsUsingRim", &bRimOff, sizeof(bRimOff));

    _uint iNumMeshes = m_pModelCom->GetNumMeshes();
    
    for (UINT i = 0; i < iNumMeshes; i++) {
        /*�Ӹ�, ����*/
        if (i == 4) {

            //Pass ����
            GAMEINSTANCE->ReadyShader(m_eShaderType, 1);
            GAMEINSTANCE->BindWVPMatrixPerspective(m_pTransformCom->GetWorldMatrix());

            //�� Bind
            GAMEINSTANCE->BindRawValue("g_vHairMainColor", &m_vHairMainColor, sizeof(_float3));
            GAMEINSTANCE->BindRawValue("g_vHairSubColor", &m_vHairSubColor, sizeof(_float3));

            GAMEINSTANCE->BeginAnimModel(m_pModelCom, i);
        }
        else if (i == 2)
        {
            continue;
        }
        else {
            GAMEINSTANCE->ReadyShader(m_eShaderType, m_iShaderPass);
            GAMEINSTANCE->BindWVPMatrixPerspective(m_pTransformCom->GetWorldMatrix());

            GAMEINSTANCE->BeginAnimModel(m_pModelCom, i);
        }
    }
    return S_OK;
}

HRESULT CNPC_MonsterDavid::RenderShadow()
{
    GAMEINSTANCE->ReadyShader(m_eShaderType, 3);
    GAMEINSTANCE->BindWVPMatrixPerspective(m_pTransformCom->GetWorldMatrix());
    GAMEINSTANCE->BindLightProjMatrix();
   
    _uint iNumMeshes = m_pModelCom->GetNumMeshes();

    for (UINT i = 0; i < iNumMeshes; i++) {
       
       GAMEINSTANCE->BeginAnimModel(m_pModelCom, i);
        
    }
    return S_OK;
}

shared_ptr<CNPC_MonsterDavid> CNPC_MonsterDavid::Create(_float3 _vCreatePos)
{
    shared_ptr<CNPC_MonsterDavid> pInstance = make_shared<CNPC_MonsterDavid>();

    pInstance->SetCurrentPos(_vCreatePos); // ��ġ ����
    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CNPC_MonsterDavid");

    return pInstance;
}

void CNPC_MonsterDavid::SetDialogState(_int _iNum)
{
    m_iDialogOrder = _iNum;
    m_iBaseState = DAVIDMONSTER_B_BATTLENORMAL;
    switch (_iNum)
    {
    case DAVIDMONSTER_DI_PRACTICEMEET2:
        m_bDialog1State[0] = true;
        break;
    case DAVIDMONSTER_DI_PRACTICEMEET3:
        m_bDialog2State[0] = true;
        break;
    case DAVIDMONSTER_DI_PRACTICEMEET4:
        m_bDialog3State[0] = true;
        break;
    }
}

HRESULT CNPC_MonsterDavid::AddRigidBody(string _strModelKey)
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
    CapsuleDesc.strShapeTag = "NPC_MonsterDavid_Body";
    RigidBodyDesc.pGeometry = &CapsuleDesc;

    RigidBodyDesc.eThisColFlag = COL_MONSTER;
    RigidBodyDesc.eTargetColFlag = COL_STATIC | COL_PLAYER | COL_PLAYERWEAPON | COL_PLAYERPROJECTILE | COL_PLAYERSKILL | COL_CHECKBOX;
    m_pRigidBody = GAMEINSTANCE->CloneRigidBody(&RigidBodyDesc);
    
    m_pTransformCom->SetRigidBody(m_pRigidBody);
    m_pRigidBody->SetMass(20.0f);

    //=========================================
    //=============JSON�о ����==============

    wstring wstrModelName(_strModelKey.begin(), _strModelKey.end());
    wstring strCompletePath = L"..\\..\\Tool_Animation\\Bin\\DataFiles\\" + wstrModelName + TEXT(".json");

    Json::Value Model;

    filesystem::path path(strCompletePath);
    if (!(filesystem::exists(path) && filesystem::is_regular_file(path)))
        return S_OK;

    Model = GAMEINSTANCE->ReadJson(strCompletePath);

    auto iter = Model.begin();
    RigidBodyDesc.isStatic = false;
    RigidBodyDesc.isTrigger = true;
    RigidBodyDesc.isInitCol = false;

    GeometryBox		TriggerBoxDesc;
    GeometrySphere  TriggerSphereDesc;
    GeometryCapsule TriggerCapsuleDesc;

    for (iter; iter != Model.end(); ++iter)
    {
        string strAnimIndex = iter.key().asString();
        _uint iAnimIndex = atoi(strAnimIndex.c_str());

        _float3 vOffsetPos = { Model[strAnimIndex]["ShapeInfo"]["OffSetPos"][0].asFloat(),
                                Model[strAnimIndex]["ShapeInfo"]["OffSetPos"][1].asFloat(),
                                Model[strAnimIndex]["ShapeInfo"]["OffSetPos"][2].asFloat() };
        _float3 vOffsetRot = { Model[strAnimIndex]["ShapeInfo"]["OffSetRot"][0].asFloat(),
                                Model[strAnimIndex]["ShapeInfo"]["OffSetRot"][1].asFloat(),
                                Model[strAnimIndex]["ShapeInfo"]["OffSetRot"][2].asFloat() };

        const char* strShapeTag = Model[strAnimIndex]["AnimName"].asCString();

        _uint iGeoType = Model[strAnimIndex]["ShapeInfo"]["Type"].asInt();

        if (iGeoType == 1)
        {
            TriggerBoxDesc.vSize = _float3(Model[strAnimIndex]["ShapeInfo"]["Size"]["BoxSize"][0].asFloat(),
                Model[strAnimIndex]["ShapeInfo"]["Size"]["BoxSize"][1].asFloat(),
                Model[strAnimIndex]["ShapeInfo"]["Size"]["BoxSize"][2].asFloat());

            TriggerBoxDesc.strShapeTag = strShapeTag;

            TriggerBoxDesc.vOffSetPosition = vOffsetPos;
            TriggerBoxDesc.vOffSetRotation = vOffsetRot;

            RigidBodyDesc.pGeometry = &TriggerBoxDesc;
        }

        else if (iGeoType == 2)
        {
            TriggerSphereDesc.fRadius = Model[strAnimIndex]["ShapeInfo"]["Size"]["SphereRadious"].asFloat();

            TriggerSphereDesc.strShapeTag = strShapeTag;

            TriggerSphereDesc.vOffSetPosition = vOffsetPos;
            TriggerSphereDesc.vOffSetRotation = vOffsetRot;

            RigidBodyDesc.pGeometry = &TriggerSphereDesc;
        }

        else if (iGeoType == 3)
        {
            TriggerCapsuleDesc.fHeight = Model[strAnimIndex]["ShapeInfo"]["Size"]["CapsuleHeight"].asFloat();
            TriggerCapsuleDesc.fRadius = Model[strAnimIndex]["ShapeInfo"]["Size"]["CapsuleRadius"].asFloat();

            TriggerCapsuleDesc.strShapeTag = strShapeTag;

            TriggerCapsuleDesc.vOffSetPosition = vOffsetPos;
            TriggerCapsuleDesc.vOffSetRotation = vOffsetRot;

            RigidBodyDesc.pGeometry = &TriggerCapsuleDesc;
        }

        RigidBodyDesc.eThisColFlag = (ECOLLISIONFLAG)Model[strAnimIndex]["ShapeInfo"]["MyFlag"].asInt();
        RigidBodyDesc.eTargetColFlag = (ECOLLISIONFLAG)Model[strAnimIndex]["ShapeInfo"]["TargetFlag"].asInt();

        m_pRigidBody->CreateShape(&RigidBodyDesc);

        vector<shared_ptr<class CAnimation>> vecAnims = m_pModelCom->GetAnimations();
        vecAnims[iAnimIndex]->SetOwnerObject(shared_from_this());
        vecAnims[iAnimIndex]->SetColStartTrackPos(Model[strAnimIndex]["TrackPos"]["StartPos"].asFloat());
        vecAnims[iAnimIndex]->SetColEndTrackPos(Model[strAnimIndex]["TrackPos"]["EndPos"].asFloat());
    }

    //=========================================
    //=========================================
    return S_OK;
}

void CNPC_MonsterDavid::OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{


    // �ǰݽ�
    if (_ColData.eColFlag & COL_STATIC || _ColData.eColFlag & COL_PLAYER || _ColData.eColFlag & COL_CHECKBOX || _ColData.eColFlag & COL_MONSTER)
        return;

    // �Ϲ��ǰ�
    if ((m_iActionState == DAVIDMONSTER_A_STDALT) || (m_iActionState == DAVIDMONSTER_A_RUN) || (m_iActionState == DAVIDMONSTER_A_BLOCKED)
        || ((m_iActionState == DAVIDMONSTER_A_FRONTBE) && ((m_iBaseState != DAVIDMONSTER_B_HITTED2) && (m_iBaseState != DAVIDMONSTER_B_HITTED3))) && !m_bNotHitted)
    {
        if ((_ColData.eColFlag & COL_PLAYERWEAPON) &&
            ((_ColData.szShapeTag == "Armature|P_Hu_M_SW2_Skl_S_BasicAttack_1") ||
                (_ColData.szShapeTag == "Armature|P_Hu_M_SW2_Skl_S_BasicAttack_2") ||
                (_ColData.szShapeTag == "Armature|P_Hu_M_SW2_Skl_S_BasicAttack_3")))
        {
            m_bHitted = true;
        }
        if ((_ColData.eColFlag & COL_PLAYERSKILL) &&
            ((_ColData.szShapeTag == "Armature|P_Hu_M_SW2_Skl_S_MainAttack") ||
                (_ColData.szShapeTag == "Armature|P_Hu_M_SW2_Skl_S_MainAttack2")))
        {
            m_bHitted = true;
        }
        if (_ColData.eColFlag & COL_PLAYERPROJECTILE)
        {
            PlayerProjAbnormal iAbnormal = dynamic_pointer_cast<CPlayerProjectile>(_ColData.pGameObject.lock())->GetAbnormal();

            if (iAbnormal == PLAYER_PJ_A_NORMAL)
            {
                m_bHitted = true;
            }
        }
    }

    // ª������
    if ((m_iBaseState == DAVIDMONSTER_B_BATTLENORMAL) || (m_iBaseState == DAVIDMONSTER_B_HITTED) || (m_iBaseState == DAVIDMONSTER_B_HITTED2) && !m_bNotHitted)
    {
        if ((_ColData.eColFlag & COL_PLAYERSKILL) &&
            ((_ColData.szShapeTag == "Armature|P_Hu_M_SW2_Skl_S_MainAttack3") ||
                (_ColData.szShapeTag == "Armature|P_Hu_M_SW2_Skl_CM_SW2_DashAttack") ||
                (_ColData.szShapeTag == "Armature|P_Hu_M_SW2_Skl_S_SpinAttack_1") ||
                (_ColData.szShapeTag == "Armature|P_Hu_M_SW2_Skl_S_SpinAttack_2") ||
                (_ColData.szShapeTag == "Armature|P_Hu_M_SW2_Skl_S_SpinAttack_3")))
        {
            m_bHitted2 = true;
        }

        if (_ColData.eColFlag & COL_PLAYERPROJECTILE)
        {
            PlayerProjAbnormal iAbnormal = dynamic_pointer_cast<CPlayerProjectile>(_ColData.pGameObject.lock())->GetAbnormal();

            if (iAbnormal == PLAYER_PJ_A_STIFF_SHORT)
            {
                m_bHitted2 = true;
            }
        }
    }

    // �����
    if ((m_iBaseState == DAVIDMONSTER_B_BATTLENORMAL) || (m_iBaseState == DAVIDMONSTER_B_HITTED)
        || (m_iBaseState == DAVIDMONSTER_B_HITTED2) || (m_iBaseState == DAVIDMONSTER_B_HITTED3) && !m_bNotHitted)
    {
        if (_ColData.eColFlag & COL_PLAYERPROJECTILE)
        {
            PlayerProjAbnormal iAbnormal = dynamic_pointer_cast<CPlayerProjectile>(_ColData.pGameObject.lock())->GetAbnormal();

            if (iAbnormal == PLAYER_PJ_A_STIFF_LONG)
            {
                m_bHitted3 = true;
            }
        }
    }

    // ����
    if ((m_iBaseState == DAVIDMONSTER_B_BATTLENORMAL) || (m_iBaseState == DAVIDMONSTER_B_HITTED)
        || (m_iBaseState == DAVIDMONSTER_B_HITTED2) || (m_iBaseState == DAVIDMONSTER_B_HITTED3) || (m_iBaseState == DAVIDMONSTER_B_STUN) && !m_bNotHitted)
    {
        if ((_ColData.eColFlag & COL_PLAYERSKILL) &&
            ((_ColData.szShapeTag == "Armature|P_Hu_M_SW2_Skl_S_GaiaCrash_Legend")))
        {
            m_bStun = true;
        }

        if (_ColData.eColFlag & COL_PLAYERPROJECTILE)
        {
            PlayerProjAbnormal iAbnormal = dynamic_pointer_cast<CPlayerProjectile>(_ColData.pGameObject.lock())->GetAbnormal();

            if (iAbnormal == PLAYER_PJ_A_STUN)
            {
                m_bStun = true;
            }
        }

    }

    // ����
    if ((m_iBaseState == DAVIDMONSTER_B_BATTLENORMAL) || (m_iBaseState == DAVIDMONSTER_B_HITTED)
        || (m_iBaseState == DAVIDMONSTER_B_HITTED2) || (m_iBaseState == DAVIDMONSTER_B_HITTED3) || (m_iBaseState == DAVIDMONSTER_B_STUN)
        || (m_iBaseState == DAVIDMONSTER_B_SHOCKED) && !m_bNotHitted)
    {
        if (_ColData.eColFlag & COL_PLAYERPROJECTILE)
        {
            PlayerProjAbnormal iAbnormal = dynamic_pointer_cast<CPlayerProjectile>(_ColData.pGameObject.lock())->GetAbnormal();

            if (iAbnormal == PLAYER_PJ_A_SHOCK)
            {
                m_bShocked = true;
            }
        }
    }

    // �Ѿ�߸���
    if ((m_iBaseState == DAVIDMONSTER_B_BATTLENORMAL) || (m_iBaseState == DAVIDMONSTER_B_HITTED)
        || (m_iBaseState == DAVIDMONSTER_B_HITTED2) || (m_iBaseState == DAVIDMONSTER_B_HITTED3) || (m_iBaseState == DAVIDMONSTER_B_STUN)
        || (m_iBaseState == DAVIDMONSTER_B_SHOCKED) || (m_iBaseState == DAVIDMONSTER_B_DOWN) && !m_bNotHitted)
    {
        if ((_ColData.eColFlag & COL_PLAYERWEAPON) &&
            ((_ColData.szShapeTag == "Armature|P_Hu_M_SW2_Skl_S_PowerAttack")))
        {
            m_bDown = true;
        }
        if ((_ColData.eColFlag & COL_PLAYERSKILL) &&
            ((_ColData.szShapeTag == "Armature|P_Hu_M_SW2_Skl_S_UpperAttack")))
        {
            m_bDown = true;
        }

        if (_ColData.eColFlag & COL_PLAYERPROJECTILE)
        {
            PlayerProjAbnormal iAbnormal = dynamic_pointer_cast<CPlayerProjectile>(_ColData.pGameObject.lock())->GetAbnormal();

            if (iAbnormal == PLAYER_PJ_A_DOWN)
            {
                m_bDown = true;
            }
        }
    }

    // �и�
    if (((_szMyShapeTag == m_strDashAttCollider1) || (_szMyShapeTag == m_stDownAttCollider2) || (_szMyShapeTag == m_stUpAttCollider2)) && (_ColData.eColFlag & COL_PLAYERPARRYING))
    {
        m_bParried = true;
    }

    // �������϶��� �����ϰ� ü�°���, ���ΰ����� �±�, ����߿���
    if (((m_iBaseState == DAVIDMONSTER_B_BATTLENORMAL) || (m_iBaseState == DAVIDMONSTER_B_HITTED)
        || (m_iBaseState == DAVIDMONSTER_B_HITTED2) || (m_iBaseState == DAVIDMONSTER_B_HITTED3) || (m_iBaseState == DAVIDMONSTER_B_STUN)
        || (m_iBaseState == DAVIDMONSTER_B_SHOCKED) || (m_iBaseState == DAVIDMONSTER_B_DOWN))
        && !(_ColData.eColFlag & COL_PLAYERPARRYING) &&
        (m_bDialog1State[1] || m_bDialog3CanHitted) && !m_bNotHitted)
    {
        m_fHp -= 10.f;
        ToggleHPBar();

        ShowEffect(_ColData, _szMyShapeTag);
    }
}

void CNPC_MonsterDavid::OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

void CNPC_MonsterDavid::OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

void CNPC_MonsterDavid::EnableCollision(const char* _strShapeTag)
{
    m_pRigidBody->EnableCollision(_strShapeTag);
}

void CNPC_MonsterDavid::DisableCollision(const char* _strShapeTag)
{
    m_pRigidBody->DisableCollision(_strShapeTag);
}

HRESULT CNPC_MonsterDavid::AddPartObjects(string _strModelKey)
{
    if (_strModelKey == "Sword2h_Sample")
    {
        shared_ptr<CWeapon::WEAPON_DESC> pWeaponDesc = make_shared<CWeapon::WEAPON_DESC>();
        pWeaponDesc->tPartObject.pParentTransform = m_pTransformCom;
        pWeaponDesc->pSocketMatrix = m_pModelCom->GetCombinedBoneMatrixPtr("Bn_Action_RightHand");
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
        pPartObject->SetActive(false);
        m_pMonsterParts.emplace("Part_Weapon", pPartObject);
    }

    // ��� ��
    else if (_strModelKey == "Sword2h_Sample2")
    {
        shared_ptr<CWeapon::WEAPON_DESC> pWeaponDesc2 = make_shared<CWeapon::WEAPON_DESC>();
        pWeaponDesc2->tPartObject.pParentTransform = m_pTransformCom;
        pWeaponDesc2->pSocketMatrix = m_pModelCom->GetCombinedBoneMatrixPtr("Bip001-Neck");
        pWeaponDesc2->strModelKey = _strModelKey;
        pWeaponDesc2->OffsetMatrixNormal = SimpleMath::Matrix::CreateRotationX(XMConvertToRadians(-180.f)) *
            SimpleMath::Matrix::CreateRotationZ(XMConvertToRadians(-105.f));

        shared_ptr<CTransform::TRANSFORM_DESC> pTransformDesc2 = make_shared<CTransform::TRANSFORM_DESC>();

        pTransformDesc2->fSpeedPerSec = 10.f;
        pTransformDesc2->fRotationPerSec = XMConvertToRadians(90.0f);

        shared_ptr<CWeapon> pWeapon2 = CWeapon::Create(pWeaponDesc2.get(), pTransformDesc2.get());
        if (nullptr == pWeapon2)
            return E_FAIL;

        shared_ptr<CPartObject> pPartObject2 = static_pointer_cast<CPartObject>(pWeapon2);
        pPartObject2->SetActive(false);
        m_pMonsterParts.emplace("Part_InWeapon", pPartObject2);
    }

    return S_OK;
}

void CNPC_MonsterDavid::UpdateDistanceState()
{
    if (m_fDistanceToTarget < m_fAttack1Range)
    {
        m_iDistanceState = DAVIDMONSTER_D_ATTACKRANGE;
    }
    else if (m_fDistanceToTarget >= m_fAttack1Range && m_fDistanceToTarget < m_fOutRange)
    {
        m_iDistanceState = DAVIDMONSTER_D_MOVERANGE;
    }
    else
    {
        m_iDistanceState = DAVIDMONSTER_D_OUTRANGE;
    }
}

void CNPC_MonsterDavid::RenderBackWeapon()
{
    m_pMonsterParts.find("Part_Weapon")->second->SetActiveOn(false);
    m_pMonsterParts.find("Part_InWeapon")->second->SetActiveOn(true);
}

void CNPC_MonsterDavid::RenderHandWeapon()
{
    m_pMonsterParts.find("Part_Weapon")->second->SetActiveOn(true);
    m_pMonsterParts.find("Part_InWeapon")->second->SetActiveOn(false);
}

void CNPC_MonsterDavid::ResetTimer()
{
    m_fCalculateTime = 0.f;
    m_fCalculateTime1 = 0.f;
    m_fCalculateTime2 = 0.f;
    m_fCalculateTime3 = 0.f;
}

void CNPC_MonsterDavid::FSM_PracticeMeet2(float _fTimeDelta)
{
    //UpdateDir(_fTimeDelta);
    // ���ѽð� Count
    m_fCalculateTime2 += _fTimeDelta;
    // ���� �̾Ƽ� �ο��¼���
    if (m_bDialog1State[0])
    {
        if (m_iActionState == DAVIDMONSTER_A_IDLE)
        {
            m_fCalculateTime1 += _fTimeDelta;
            if (m_fCalculateTime1 > 2.f)
            {
                m_fCalculateTime1 = 0.f;
                m_iActionState = DAVIDMONSTER_A_SWORDOUT;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
            }
        }
        // ���� ������
        if (m_iActionState == DAVIDMONSTER_A_SWORDOUT)
        {
            m_fCalculateTime1 += _fTimeDelta;
            if (m_fCalculateTime1 > m_fWeaponOuTime && !m_bHandWeaponRender)
            {
                RenderHandWeapon();
                m_bHandWeaponRender = true;
                m_bBackWeaponRender = false;
            }
            if (m_pModelCom->GetIsFinishedAnimation())
            {
                m_fCalculateTime1 = 0.f;
                m_iActionState = DAVIDMONSTER_A_STDALT;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
                m_bDialog1State[0] = false;
                m_bDialog1State[1] = true;
            }
        }
    }
    // ������������ �Ϲݰ���, ���ѽð� �ٵǸ� Į ����ְ� ���ֱ�, �´� Ÿ�̹�
    if (m_bDialog1State[1])
    {
        // ���ݵ��� ���� üũ
        CheckAttacked();

        // ������������ �Ϲݰ����ϱ�
        if (m_iActionState == DAVIDMONSTER_A_STDALT)
        {

            m_fCalculateTime1 += _fTimeDelta;
            if (m_fCalculateTime1 > m_fD2AttackIntervel)
            {
                m_fCalculateTime1 = 0.f;
                m_iActionState = DAVIDMONSTER_A_ATT1;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f);
            }

            if (m_fCalculateTime2 > m_fD2LimitTime)
            {
                ResetTimer();
                m_iActionState = DAVIDMONSTER_A_SWORDIN;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
                m_bDialog1State[1] = false;
                m_bDialog1State[2] = true;
            }

            // �ǰ� 0���ϰ� �ȴٸ�
            if (m_fCalculateTime1 > 2.f)
            {
                if (CheckLoose())
                {
                    ResetTimer();
                    m_iActionState = DAVIDMONSTER_A_SWORDIN;
                    m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
                    m_bDialog1State[1] = false;
                    m_bDialog1State[2] = true;
                }
            }
        }
        if (m_iActionState == DAVIDMONSTER_A_ATT1)
        {
            if (m_pModelCom->GetIsFinishedAnimation())
            {
                m_iActionState = DAVIDMONSTER_A_STDALT;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            }
        }
    }
    // ���� ����ְ� ���ֱ�
    if (m_bDialog1State[2])
    {
        // ���� ����ֱ�
        if (m_iActionState == DAVIDMONSTER_A_SWORDIN)
        {
            m_fCalculateTime1 += _fTimeDelta;
            if (m_fCalculateTime1 > m_fWeaponInTime && !m_bBackWeaponRender)
            {
                RenderBackWeapon();
                m_bHandWeaponRender = false;
                m_bBackWeaponRender = true;
            }
            if (m_pModelCom->GetIsFinishedAnimation())
            {
                m_bDialog1State[2] = false;
                m_fCalculateTime1 = 0.f;
                m_iActionState = DAVIDMONSTER_A_IDLE;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);

                m_iBaseState = DAVIDMONSTER_B_NONE;
                m_bSwitch = true;
                SimulationOff();
                m_pTransformCom->LookAtDir(_float3(0.f, 0.f, 0.f) - _float3(10.f, 0.f, 0.f));
            }
        }
    }
}

void CNPC_MonsterDavid::FSM_PracticeMeet3(float _fTimeDelta)
{
    UpdateDir(_fTimeDelta);
    // ���� �̾Ƽ� �ο��¼���
    if (m_bDialog2State[0])
    {
        if (m_iActionState == DAVIDMONSTER_A_IDLE)
        {
            m_fCalculateTime1 += _fTimeDelta;
            if (m_fCalculateTime1 > 2.f)
            {
                m_fCalculateTime1 = 0.f;
                m_iActionState = DAVIDMONSTER_A_SWORDOUT;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
            }
        }
        // ���� ������
        if (m_iActionState == DAVIDMONSTER_A_SWORDOUT)
        {
            m_fCalculateTime1 += _fTimeDelta;
            if (m_fCalculateTime1 > m_fWeaponOuTime && !m_bHandWeaponRender)
            {
                RenderHandWeapon();
                m_bHandWeaponRender = true;
                m_bBackWeaponRender = false;
            }
            if (m_pModelCom->GetIsFinishedAnimation())
            {
                m_fCalculateTime1 = 0.f;
                m_iActionState = DAVIDMONSTER_A_STDALT;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
                m_bDialog2State[0] = false;
                m_bDialog2State[1] = true;
            }
        }
    }
    // ������������ Ư������, ���ѽð� �ٵǸ� Į ����ְ� ���ֱ�, ���ݸ��ϴ� Ÿ�̹�
    if (m_bDialog2State[1])
    {
        m_fCalculateTime1 += _fTimeDelta;
        if (m_iActionState == DAVIDMONSTER_A_STDALT)
        {
            if (m_fCalculateTime1 > m_fD3AttackIntervel)
            {
                m_fCalculateTime1 = 0.f;
                if (NextAttack() != DAVIDMONSTER_A_DASH1 && NextAttack() != DAVIDMONSTER_A_DASH2)
                {
                    UpdateDistanceState();
                    if (m_iDistanceState == DAVIDMONSTER_D_ATTACKRANGE)
                    {
                        m_iActionState = NextAttack();
                        m_pModelCom->ChangeAnim(m_iActionState, 0.1f);
                        m_iD3AttackNum++;
                        m_bDialog2State[1] = false;
                        m_bDialog2State[2] = true;
                    }
                    else
                    {
                        m_iActionState = DAVIDMONSTER_A_RUN;
                        m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
                    }
                }
                else if(NextAttack() == DAVIDMONSTER_A_DASH1)
                {
                    m_iD3AttackNum++;
                    m_bDialog2State[1] = false;
                    m_bDialog2State[2] = true;
                }
                else if (NextAttack() == DAVIDMONSTER_A_DASH2)
                {
                    m_iD3AttackNum++;
                    m_bDialog2State[1] = false;
                    m_bDialog2State[2] = true;
                }
            }
        }
        if (m_iActionState == DAVIDMONSTER_A_RUN)
        {
            UpdateDirection();
            MoveToTarget(_fTimeDelta);
            UpdateDistanceState();
            if (m_iDistanceState == DAVIDMONSTER_D_ATTACKRANGE)
            {
                if (m_fCalculateTime1 > m_fD3AttackIntervel)
                {
                    m_fCalculateTime1 = 0.f;
                    m_iActionState = NextAttack();
                    m_pModelCom->ChangeAnim(m_iActionState, 0.1f);
                    m_iD3AttackNum++;
                    m_bDialog2State[1] = false;
                    m_bDialog2State[2] = true;
                }
                else
                {
                    m_iActionState = DAVIDMONSTER_A_STDALT;
                    m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
                }
            }
        }
    }
    // ����
    if (m_bDialog2State[2])
    {
        switch (m_iD3AttackSkill)
        {
        case DAVIDMONSTER_A_BLADEDANCE1:
            if (UpdateSpinState(_fTimeDelta))
            {
                m_bDialog2State[2] = false;
                m_bDialog2State[1] = true;
                m_bParriedBefore = false;
            }
            break;
        case DAVIDMONSTER_A_DASH1:
            if(UpdateFrontDashState(_fTimeDelta))
            {
                m_iActionState = DAVIDMONSTER_A_STDALT;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
                m_bParriedBefore = false;
                m_bDialog2State[2] = false;
                m_bDialog2State[1] = true;
            }
            break;
        case DAVIDMONSTER_A_DASH2:
            if (UpdateThroughDashState(_fTimeDelta))
            {
                _float2 vTargetPosXZ = _float2(m_vNPCPos.x, m_vNPCPos.z);
                _float2 vCurrentPosXZ = _float2(m_vCurrentPos.x, m_vCurrentPos.z);
                _float m_fDistnce = (vTargetPosXZ - vCurrentPosXZ).Length();

                if (m_fDistnce > m_fBacKWalkRange)
                {
                    m_iActionState = DAVIDMONSTER_A_RUN;
                    m_pModelCom->ChangeAnim(m_iActionState, 0.f, true);
                    m_pTransformCom->LookAtDir(_float3(m_vNPCPos.x, 0.f, m_vNPCPos.z) - _float3(m_vCurrentPos.x, 0.f, m_vCurrentPos.z));
                    m_pTransformCom->SetSpeed(4.f);
                }
                else
                {
                    m_iActionState = DAVIDMONSTER_A_WALK;
                    m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
                    m_pTransformCom->LookAtDir(_float3(m_vNPCPos.x, 0.f, m_vNPCPos.z) - _float3(m_vCurrentPos.x, 0.f, m_vCurrentPos.z));
                    m_pTransformCom->SetSpeed(1.f);
                }
                m_iD3AttackNum = 0;
                m_bDialog2State[2] = false;
                m_bDialog2State[3] = true;
            }
            break;
        case DAVIDMONSTER_A_ATT1:
            if (UpdateBottomAttack1(_fTimeDelta))
            {
                m_bDialog2State[2] = false;
                m_bDialog2State[1] = true;
            }
            break;
        case DAVIDMONSTER_A_ATT1_2:
            if (UpdateBottomAttack2(_fTimeDelta))
            {
                m_bDialog2State[2] = false;
                m_bDialog2State[1] = true;
            }
            break;
        case DAVIDMONSTER_A_ATT2:
            if (UpdateUpAttack1(_fTimeDelta))
            {
                m_bDialog2State[2] = false;
                m_bDialog2State[1] = true;
            }
            break;
        case DAVIDMONSTER_A_ATT2_2:
            if(UpdateUpAttack2(_fTimeDelta))
            {
                m_bDialog2State[2] = false;
                m_bDialog2State[1] = true;
            }
            break;
        }
    }
    // ���� ��ġ�� �ɾ��
    if (m_bDialog2State[3])
    {
        m_pTransformCom->GoStraight(_fTimeDelta);
        
        _float2 vTargetPosXZ = _float2(m_vNPCPos.x, m_vNPCPos.z);
        _float2 vCurrentPosXZ = _float2(m_vCurrentPos.x, m_vCurrentPos.z);
        _float m_fDistnce = (vTargetPosXZ - vCurrentPosXZ).Length();
        if (m_iActionState == DAVIDMONSTER_A_RUN && m_fDistnce <= m_fBacKWalkRange)
        {
            m_iActionState = DAVIDMONSTER_A_WALK;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            m_pTransformCom->SetSpeed(1.f);
        }

        if (m_fDistnce < m_fWalkRemainDistance)
        {
            m_iActionState = DAVIDMONSTER_A_SWORDIN;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
            m_pTransformCom->LookAtDir(_float3(0.f, 0.f, 0.f) - _float3(10.f, 0.f, 0.f));
            m_bDialog2State[3] = false;
            m_bDialog2State[4] = true;
        }
    }
    // ���� ����ְ� ���ֱ�
    if (m_bDialog2State[4])
    {
        // ���� ����ֱ�
        if (m_iActionState == DAVIDMONSTER_A_SWORDIN)
        {
            m_fCalculateTime1 += _fTimeDelta;
            if (m_fCalculateTime1 > m_fWeaponInTime && !m_bBackWeaponRender)
            {
                RenderBackWeapon();
                m_bHandWeaponRender = false;
                m_bBackWeaponRender = true;
            }
            if (m_pModelCom->GetIsFinishedAnimation())
            {
                m_bDialog2State[4] = false;
                m_fCalculateTime1 = 0.f;
                m_iActionState = DAVIDMONSTER_A_IDLE;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);

                m_iBaseState = DAVIDMONSTER_B_NONE;
                m_bSwitch = true;
                SimulationOff();
                m_pTransformCom->LookAtDir(_float3(0.f, 0.f, 0.f) - _float3(10.f, 0.f, 0.f));
            }
        }
    }
}

void CNPC_MonsterDavid::FSM_PracticeMeet4(float _fTimeDelta)
{
    UpdateDir(_fTimeDelta);
    // ���� �̾Ƽ� �ο��¼���
    if (m_bDialog3State[0])
    {
        if (m_iActionState != DAVIDMONSTER_A_SWORDOUT)
        {
            m_fCalculateTime1 += _fTimeDelta;
            if (m_fCalculateTime1 > 2.f)
            {
                m_fCalculateTime1 = 0.f;
                m_iActionState = DAVIDMONSTER_A_SWORDOUT;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
            }
        }
        // ���� ������
        if (m_iActionState == DAVIDMONSTER_A_SWORDOUT)
        {
            m_fCalculateTime1 += _fTimeDelta;
            if (m_fCalculateTime1 > m_fWeaponOuTime && !m_bHandWeaponRender)
            {
                RenderHandWeapon();
                m_bHandWeaponRender = true;
                m_bBackWeaponRender = false;
            }
            if (m_pModelCom->GetIsFinishedAnimation())
            {
                ResetTimer();
                m_iActionState = DAVIDMONSTER_A_STDALT;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
                m_bDialog3State[0] = false;
                m_bDialog3State[1] = true;

            }
        }
    }
    // ���ִٰ� ��������
    if (m_bDialog3State[1])
    {
        if (m_iActionState == DAVIDMONSTER_A_STDALT)
        {
            m_fCalculateTime += _fTimeDelta;
            if (m_fCalculateTime > m_fStandTime)
            {
                m_fCalculateTime = 0.f;
                m_bDialog3State[1] = false;
                m_bDialog3State[2] = true;
                m_bDialog3CanHitted = true;
                m_bParriedBefore = false;
            }
        }
    }
    // �뽬����1
    if (m_bDialog3State[2])
    {
        if (UpdateFrontDashState(_fTimeDelta))
        {
            m_iActionState = DAVIDMONSTER_A_STDALT;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            m_bDialog3State[2] = false;
            m_bDialog3State[3] = true;
            m_bParriedBefore = false;
        }
    }
    // �Ʒ�����2
    if (m_bDialog3State[3])
    {
        if (m_iActionState == DAVIDMONSTER_A_STDALT)
        {
            if (!m_bParriedBefore)
            {
                m_fCalculateTime += _fTimeDelta;
                if (m_fCalculateTime > m_fStandTime)
                {
                    m_fCalculateTime = 0.f;
                    m_iActionState = DAVIDMONSTER_A_RUN;
                    m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
                }
            }
            else
            {
                m_fCalculateTime += _fTimeDelta;

                if (m_fCalculateTime > 0.5f)
                {
                    m_fCalculateTime = 0.f;
                    m_bDialog3State[3] = false;
                    m_bDialog3State[4] = true;
                }
            }
        }
        if (m_iActionState == DAVIDMONSTER_A_RUN)
        {
            UpdateDirection();
            MoveToTarget(_fTimeDelta);
            UpdateDistanceState();
            if (m_iDistanceState == DAVIDMONSTER_D_ATTACKRANGE)
            {
                m_iActionState = DAVIDMONSTER_A_ATT1_2;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
                SetWeaponRimLight(true);
            }
        }
        if (UpdateBottomAttack2(_fTimeDelta))
        {
            m_bDialog3State[3] = false;
            m_bDialog3State[4] = true;
        }
    }
    // ȸ������
    if (m_bDialog3State[4])
    {
        if (m_iActionState == DAVIDMONSTER_A_STDALT)
        {
            m_fCalculateTime += _fTimeDelta;
            if (m_fCalculateTime > m_fStandTime)
            {
                m_fCalculateTime = 0.f;
                m_iActionState = DAVIDMONSTER_A_RUN;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            }
        }
        if (m_iActionState == DAVIDMONSTER_A_RUN)
        {
            UpdateDirection();
            MoveToTarget(_fTimeDelta);
            UpdateDistanceState();
            if (m_iDistanceState == DAVIDMONSTER_D_ATTACKRANGE)
            {
                m_iActionState = DAVIDMONSTER_A_BLADEDANCE1;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
            }
        }
        if (UpdateSpinState(_fTimeDelta))
        {
            m_bDialog3State[4] = false;
            m_bDialog3State[5] = true;
        }
    }
    // ����뽬
    if (m_bDialog3State[5])
    {
        if (UpdateThroughDashState(_fTimeDelta))
        {
            m_iActionState = DAVIDMONSTER_A_STDALT;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            m_bDialog3State[5] = false;
            m_bDialog3State[6] = true;
            m_bParriedBefore = false;
        }
    }
    // �뽬����1
    if (m_bDialog3State[6])
    {
        if (UpdateFrontDashState(_fTimeDelta))
        {
            m_iActionState = DAVIDMONSTER_A_STDALT;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            m_bDialog3State[6] = false;
            m_bDialog3State[7] = true;
        }
    }
    // ���ΰ���1
    if (m_bDialog3State[7])
    {
        if (m_iActionState == DAVIDMONSTER_A_STDALT)
        {
            m_fCalculateTime += _fTimeDelta;
            if (m_fCalculateTime > m_fStandTime)
            {
                m_fCalculateTime = 0.f;
                m_iActionState = DAVIDMONSTER_A_RUN;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            }
        }
        if (m_iActionState == DAVIDMONSTER_A_RUN)
        {
            UpdateDirection();
            MoveToTarget(_fTimeDelta);
            UpdateDistanceState();
            if (m_iDistanceState == DAVIDMONSTER_D_ATTACKRANGE)
            {
                m_iActionState = DAVIDMONSTER_A_ATT2;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
            }
        }
        if (UpdateUpAttack1(_fTimeDelta))
        {
            m_bDialog3State[7] = false;
            m_bDialog3State[8] = true;
        }
    }
    // �Ʒ�����1
    if (m_bDialog3State[8])
    {
        if (m_iActionState == DAVIDMONSTER_A_STDALT)
        {
            m_fCalculateTime += _fTimeDelta;
            if (m_fCalculateTime > m_fStandTime)
            {
                m_fCalculateTime = 0.f;
                m_iActionState = DAVIDMONSTER_A_RUN;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            }
        }
        if (m_iActionState == DAVIDMONSTER_A_RUN)
        {
            UpdateDirection();
            MoveToTarget(_fTimeDelta);
            UpdateDistanceState();
            if (m_iDistanceState == DAVIDMONSTER_D_ATTACKRANGE)
            {
                m_iActionState = DAVIDMONSTER_A_ATT1;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
            }
        }
        if (UpdateBottomAttack1(_fTimeDelta))
        {
            m_bDialog3State[8] = false;
            m_bDialog3State[9] = true;
            m_bParriedBefore = false;
        }
    }
    // ���ΰ���2
    if (m_bDialog3State[9])
    {
        if (m_iActionState == DAVIDMONSTER_A_STDALT)
        {
            if(!m_bParriedBefore)
            {
                m_fCalculateTime += _fTimeDelta;
                if (m_fCalculateTime > m_fStandTime)
                {
                    m_fCalculateTime = 0.f;
                    m_iActionState = DAVIDMONSTER_A_RUN;
                    m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
                }
            }
            else
            {
                m_fCalculateTime += _fTimeDelta;

                if (m_fCalculateTime > 0.5f)
                {
                    m_fCalculateTime = 0.f;
                    m_bDialog3State[9] = false;
                    m_bDialog3State[10] = true;
                }
            }
        }
        if (m_iActionState == DAVIDMONSTER_A_RUN)
        {
            UpdateDirection();
            MoveToTarget(_fTimeDelta);
            UpdateDistanceState();
            if (m_iDistanceState == DAVIDMONSTER_D_ATTACKRANGE)
            {
                m_iActionState = DAVIDMONSTER_A_ATT2_2;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
                SetWeaponRimLight(true);
            }
        }
        if (UpdateUpAttack2(_fTimeDelta))
        {
            m_bDialog3State[9] = false;
            m_bDialog3State[10] = true;
        }
    }
    // ����뽬
    if (m_bDialog3State[10])
    {
        if (UpdateThroughDashState(_fTimeDelta))
        {
            // ù��� ���ư���
            m_bDialog3State[10] = false;
            m_bDialog3State[2] = true;
            m_iActionState = DAVIDMONSTER_A_STDALT;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            m_bParriedBefore = false;
        }
    }
    // ���� ��ġ�� �ɾ��
    if (m_bDialog3State[11])
    {
        m_pTransformCom->GoStraight(_fTimeDelta);

        _float2 vTargetPosXZ = _float2(m_vNPCPos.x, m_vNPCPos.z);
        _float2 vCurrentPosXZ = _float2(m_vCurrentPos.x, m_vCurrentPos.z);
        _float m_fDistnce = (vTargetPosXZ - vCurrentPosXZ).Length();
        if (m_iActionState == DAVIDMONSTER_A_RUN && m_fDistnce <= m_fBacKWalkRange)
        {
            m_iActionState = DAVIDMONSTER_A_WALK;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            m_pTransformCom->SetSpeed(1.f);
        }

        if (m_fDistnce < m_fWalkRemainDistance)
        {
            m_iActionState = DAVIDMONSTER_A_SWORDIN;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
            m_pTransformCom->LookAtDir(_float3(0.f, 0.f, 0.f) - _float3(10.f, 0.f, 0.f));
            m_bDialog3State[11] = false;
            m_bDialog3State[12] = true;
            m_bNotHitted = true;
        }
    }
    // ���� ����ְ� ���ֱ�
    if (m_bDialog3State[12])
    {
        // ���� ����ֱ�
        if (m_iActionState == DAVIDMONSTER_A_SWORDIN)
        {
            m_fCalculateTime1 += _fTimeDelta;
            if (m_fCalculateTime1 > m_fWeaponInTime && !m_bBackWeaponRender)
            {
                RenderBackWeapon();
                m_bHandWeaponRender = false;
                m_bBackWeaponRender = true;
            }
            if (m_pModelCom->GetIsFinishedAnimation())
            {
                m_bNotHitted = false;
                m_bDialog3State[12] = false;
                m_fCalculateTime1 = 0.f;
                m_iActionState = DAVIDMONSTER_A_IDLE;
                m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);

                m_iBaseState = DAVIDMONSTER_B_NONE;
                m_bSwitch = true;
                m_bDoingEnd = true;
                SimulationOff();
                m_pTransformCom->LookAtDir(_float3(0.f, 0.f, 0.f) - _float3(10.f, 0.f, 0.f));
                m_IsEnabled = false;
            }
        }
    }

    // ���ݵ��� ���� üũ
    CheckAttacked();
    
    // ü�� üũ
    if (m_bDialog3CanHitted)
    {
        // ����
        if (m_iActionState == DAVIDMONSTER_A_STDALT)
        {
            if (CheckLoose())
            {
                for (int i = 0; i < 20; i++)
                {
                    m_bDialog3State[i] = false;
                }
                ResetTimer();
                m_bDialog3CanHitted = false;
                m_bDialog3State[11] = true;

                _float2 vTargetPosXZ = _float2(m_vNPCPos.x, m_vNPCPos.z);
                _float2 vCurrentPosXZ = _float2(m_vCurrentPos.x, m_vCurrentPos.z);
                _float m_fDistnce = (vTargetPosXZ - vCurrentPosXZ).Length();

                if (m_fDistnce > m_fBacKWalkRange)
                {
                    m_iActionState = DAVIDMONSTER_A_RUN;
                    m_pModelCom->ChangeAnim(m_iActionState, 0.f, true);
                    m_pTransformCom->LookAtDir(_float3(m_vNPCPos.x, 0.f, m_vNPCPos.z) - _float3(m_vCurrentPos.x, 0.f, m_vCurrentPos.z));
                    m_pTransformCom->SetSpeed(4.f);
                }
                else
                {
                    m_iActionState = DAVIDMONSTER_A_WALK;
                    m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
                    m_pTransformCom->LookAtDir(_float3(m_vNPCPos.x, 0.f, m_vNPCPos.z) - _float3(m_vCurrentPos.x, 0.f, m_vCurrentPos.z));
                    m_pTransformCom->SetSpeed(1.f);
                }
            }
        }
    }
}

void CNPC_MonsterDavid::FSM_Hitted(_float _fTimeDelta)
{
    if (m_iActionState == DAVIDMONSTER_A_FRONTBE)
    {
        if (m_bHitted)
        {
            UpdateDirection();
            m_bHitted = false;
            m_iActionState = DAVIDMONSTER_A_FRONTBE;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
        }
        if (m_pModelCom->GetIsFinishedAnimation())
        {
            m_iBaseState = DAVIDMONSTER_B_BATTLENORMAL;
            m_iActionState = DAVIDMONSTER_A_STDALT;
            m_pModelCom->ChangeAnim(m_iActionState, 0.5f, true);
            m_bHitted = false;
            m_bParriedBefore = true;
        }
    }

    // ���ݵ��� ���� üũ
    CheckAttacked();
}

void CNPC_MonsterDavid::FSM_Hitted2(_float _fTimeDelta)
{
    if (m_iActionState == DAVIDMONSTER_A_FRONTBE)
    {
        if (m_bHitted2)
        {
            //UpdateDirection();
            m_bHitted2 = false;
            m_iActionState = DAVIDMONSTER_A_FRONTBE;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
        }
        if (m_pModelCom->GetIsFinishedAnimation())
        {
            m_iBaseState = DAVIDMONSTER_B_BATTLENORMAL;
            m_iActionState = DAVIDMONSTER_A_STDALT;
            m_pModelCom->ChangeAnim(m_iActionState, 0.5f, true);
            m_bHitted2 = false;
            m_bParriedBefore = true;
        }
    }

    // ���ݵ��� ���� üũ
    CheckAttacked();
}

void CNPC_MonsterDavid::FSM_Hitted3(_float _fTimeDelta)
{
    if (m_iActionState == DAVIDMONSTER_A_FRONTBE)
    {
        if (m_bHitted3)
        {
            UpdateDirection();
            m_iActionState = DAVIDMONSTER_A_FRONTBE;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
            m_bHitted3 = false;
        }
        if (m_pModelCom->GetIsFinishedAnimation())
        {
            m_iBaseState = DAVIDMONSTER_B_BATTLENORMAL;
            m_iActionState = DAVIDMONSTER_A_STDALT;
            m_pModelCom->ChangeAnim(m_iActionState, 0.5f, true);
            m_bHitted3 = false;
            m_bParriedBefore = true;
        }
    }

    // ���ݵ��� ���� üũ
    CheckAttacked();
}

void CNPC_MonsterDavid::FSM_Stun(_float _fTimeDelta)
{
    if (m_iActionState == DAVIDMONSTER_A_STUN)
    {
        if (m_bStun)
        {
            UpdateDirection();
            m_bStun = false;
            m_iActionState = DAVIDMONSTER_A_STUN;
            m_pModelCom->ChangeAnim(m_iActionState, 0.3f, false);
        }
        if (m_pModelCom->GetIsFinishedAnimation())
        {
            m_iBaseState = DAVIDMONSTER_B_BATTLENORMAL;
            m_iActionState = DAVIDMONSTER_A_STDALT;
            m_pModelCom->ChangeAnim(m_iActionState, 0.3f, true);
            m_bStun = false;
            m_bParriedBefore = true;
        }
    }

    // ���ݵ��� ���� üũ
    CheckAttacked();
}

void CNPC_MonsterDavid::FSM_Shocked(_float _fTimeDelta)
{
    if (m_iActionState == DAVIDMONSTER_A_SHOCKED)
    {
        if (m_bShocked)
        {
            UpdateDirection();
            m_bShocked = false;
            m_iActionState = DAVIDMONSTER_A_SHOCKED;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
        }
        if (m_pModelCom->GetIsFinishedAnimation())
        {
            m_iBaseState = DAVIDMONSTER_B_BATTLENORMAL;
            m_iActionState = DAVIDMONSTER_A_STDALT;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            m_bShocked = false;
            m_bParriedBefore = true;
        }
    }

    // ���ݵ��� ���� üũ
    CheckAttacked();
}

void CNPC_MonsterDavid::FSM_Down(_float _fTimeDelta)
{
    if (m_iActionState == DAVIDMONSTER_A_DOWN1)
    {
        if (m_bDown)
        {
            UpdateDirection();
            m_fCalculateTime1 = 0.f;
            m_bDown = false;
            m_iActionState = DAVIDMONSTER_A_DOWN1;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
        }
        if (m_pModelCom->GetIsFinishedAnimation())
        {
            m_iActionState = DAVIDMONSTER_A_DOWN2;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
        }
    }
    if (m_iActionState == DAVIDMONSTER_A_DOWN2)
    {
        m_fCalculateTime1 += _fTimeDelta;
        if (m_pModelCom->GetIsFinishedAnimation() && m_fCalculateTime1 > 0.1f)
        {
            m_fCalculateTime1 = 0.f;
            m_iActionState = DAVIDMONSTER_A_DOWN3;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
        }
    }
    if (m_iActionState == DAVIDMONSTER_A_DOWN3)
    {
        m_fCalculateTime1 += _fTimeDelta;
        if (m_pModelCom->GetIsFinishedAnimation() && m_fCalculateTime1 > 0.1f)
        {
            m_fCalculateTime1 = 0.f;
            m_iBaseState = DAVIDMONSTER_B_BATTLENORMAL;
            m_iActionState = DAVIDMONSTER_A_STDALT;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            m_bDown = false;
            m_bParriedBefore = true;
        }
    }

    // ���ݵ��� ���� üũ
    CheckAttacked();
}

void CNPC_MonsterDavid::UpdateDir(_float _fTimeDelta)
{
    switch (m_iActionState)
    {
    case DAVID_A_STDALT:
        m_pTransformCom->TurnToDir(m_vTargetDir, 180.f * _fTimeDelta);
        break;
    case DAVID_A_ATT1:
        m_pTransformCom->TurnToDir(m_vTargetDir, 90.f * _fTimeDelta);
        break;
    case DAVID_A_ATT2:
        m_pTransformCom->TurnToDir(m_vTargetDir, 90.f * _fTimeDelta);
        break;
    }
}

_bool CNPC_MonsterDavid::CheckLoose()
{
    if (m_fHp <= 0.f)
    {
        m_fHp = m_fMaxHp;
        return true;
    }
    else
        return false;
}

void CNPC_MonsterDavid::CheckAttacked()
{
    // �ǰݽ�
    if (m_bHitted)
    {
        SetWeaponRimLight(false);
        m_iBaseState = DAVIDMONSTER_B_HITTED;
        m_iActionState = DAVIDMONSTER_A_FRONTBE;
    }

    // ª������
    if (m_bHitted2)
    {
        SetWeaponRimLight(false);
        m_iBaseState = DAVIDMONSTER_B_HITTED2;
        m_iActionState = DAVIDMONSTER_A_FRONTBE;
    }

    // �����
    if (m_bHitted3)
    {
        SetWeaponRimLight(false);
        m_iBaseState = DAVIDMONSTER_B_HITTED3;
        m_iActionState = DAVIDMONSTER_A_FRONTBE;
    }

    // ����
    if (m_bStun)
    {
        SetWeaponRimLight(false);
        m_iBaseState = DAVIDMONSTER_B_STUN;
        m_iActionState = DAVIDMONSTER_A_STUN;
    }

    // ����
    if (m_bShocked)
    {
        SetWeaponRimLight(false);
        m_iBaseState = DAVIDMONSTER_B_SHOCKED;
        m_iActionState = DAVIDMONSTER_A_SHOCKED;
    }

    // �Ѿ�����
    if (m_bDown)
    {
        SetWeaponRimLight(false);
        m_iBaseState = DAVIDMONSTER_B_DOWN;
        m_iActionState = DAVIDMONSTER_A_DOWN1;
    }
}

_int CNPC_MonsterDavid::NextAttack()
{
    switch(m_iD3AttackNum)
    {
    case 0:
        m_iD3AttackSkill = DAVIDMONSTER_A_ATT1;
        return DAVIDMONSTER_A_ATT1;
        break;
    case 1:
        m_iD3AttackSkill = DAVIDMONSTER_A_ATT1_2;
        return DAVIDMONSTER_A_ATT1_2;
        break;
    case 2:
        m_iD3AttackSkill = DAVIDMONSTER_A_BLADEDANCE1;
        return DAVIDMONSTER_A_BLADEDANCE1;
        break;
    case 3:
        m_iD3AttackSkill = DAVIDMONSTER_A_DASH1;
        return DAVIDMONSTER_A_DASH1;
        break;
    case 4:
        m_iD3AttackSkill = DAVIDMONSTER_A_ATT2_2;
        return DAVIDMONSTER_A_ATT2_2;
        break;
    case 5:
        m_iD3AttackSkill = DAVIDMONSTER_A_DASH2;
        return DAVIDMONSTER_A_DASH2;
        break;
    }
}

_bool CNPC_MonsterDavid::UpdateSpinState(_float _fTimeDelta)
{
    if (m_iActionState == DAVIDMONSTER_A_BLADEDANCE1)
    {
        m_fCalculateTime1 += _fTimeDelta;
        if (m_pModelCom->GetIsFinishedAnimation() && m_fCalculateTime1 > 0.1f)
        {
            m_fCalculateTime1 = 0.f;
            m_iActionState = DAVIDMONSTER_A_BLADEDANCE2;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
        }
    }
    if (m_iActionState == DAVIDMONSTER_A_BLADEDANCE2)
    {
        m_fCalculateTime1 += _fTimeDelta;
        if (m_pModelCom->GetIsFinishedAnimation() && m_fCalculateTime1 > 0.1f)
        {
            m_fCalculateTime1 = 0.f;
            m_iActionState = DAVIDMONSTER_A_BLADEDANCE3;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, false);
        }
    }
    if (m_iActionState == DAVIDMONSTER_A_BLADEDANCE3)
    {
        m_fCalculateTime1 += _fTimeDelta;
        if (m_pModelCom->GetIsFinishedAnimation() && m_fCalculateTime1 > 0.1f)
        {
            m_fCalculateTime1 = 0.f;
            m_iActionState = DAVIDMONSTER_A_STDALT;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            return true;
        }
    }
    return false;
}

_bool CNPC_MonsterDavid::UpdateFrontDashState(_float _fTimeDelta, _float _fFrontDistance)
{
    if (m_iActionState == DAVIDMONSTER_A_STDALT)
    {
        if (!m_bParriedBefore)
        {
            m_fCalculateTime += _fTimeDelta;

            if (m_fCalculateTime > 0.5f)
            {
                SetWeaponRimLight(true);
                m_fCalculateTime = 0.f;
                m_fDashDistanceXZ = m_fDistanceToTarget - _fFrontDistance;
                m_fDashTime = m_fDashEndTime - m_fDashStartTime;
                m_fDashSpeed = m_fDashDistanceXZ / m_fDashTime;
                m_iActionState = DAVIDMONSTER_A_DASH1;
                m_pModelCom->ChangeAnim(m_iActionState, 0.f, false);
                m_vDashDir = m_vNormTargetDirXZ;
                UpdateDirection();
            }
        }
        else
        {
            m_fCalculateTime += _fTimeDelta;

            if (m_fCalculateTime > 0.5f)
            {
                m_fCalculateTime = 0.f;
                return true;
            }
        }
    }
    if (m_iActionState == DAVIDMONSTER_A_DASH1)
    {
        m_fCalculateTime += _fTimeDelta;
        // �и�
        if (m_bParried)
        {
            SetWeaponRimLight(false);
            m_bParried = false;
            m_iActionState = DAVIDMONSTER_A_BLOCKED;
            m_pModelCom->ChangeAnim(m_iActionState, 0.f);
            m_iParriedPower = PARRIEDSTRONG;
            m_fCalculateTime = 0.f;
        }

        if ((m_iActionState != DAVIDMONSTER_A_BLOCKED) && ((m_fCalculateTime > m_fDashStartTime) && (m_fCalculateTime < m_fDashEndTime)))
        {
            MovePos(m_vCurrentPos + m_vDashDir * m_fDashSpeed * _fTimeDelta);
        }

        // �ð�������
        if (m_pModelCom->GetIsFinishedAnimation() && m_fCalculateTime > 0.1f)
        {
            SetWeaponRimLight(false);
            m_fCalculateTime = 0.f;
            UpdateDirection();

            return true;
        }
    }
    if (m_iActionState == DAVIDMONSTER_A_BLOCKED)
    {
        m_fCalculateTime += _fTimeDelta;
        if (m_pModelCom->GetIsFinishedAnimation() && m_fCalculateTime > 0.1f)
        {
            m_bParriedBefore = true;
            m_fCalculateTime = 0.f;
            m_fAnimSpeed = 1.f;
            return true;
        }
    }
    return false;
}

_bool CNPC_MonsterDavid::UpdateThroughDashState(float _fTimeDelta, _float _fDistance)
{
    if (m_iActionState == DAVIDMONSTER_A_STDALT)
    {
        m_fCalculateTime += _fTimeDelta;

        if (m_fCalculateTime > 0.5f)
        {
            m_fCalculateTime = 0.f;
            m_fDashDistanceXZ = m_fDistanceToTarget + _fDistance;
            m_fDashTime = m_fDashEndTime - m_fDashStartTime;
            m_fDashSpeed = m_fDashDistanceXZ / m_fDashTime;
            m_iActionState = DAVIDMONSTER_A_DASH2;
            m_pModelCom->ChangeAnim(m_iActionState, 0.f, false);
            m_vDashDir = m_vNormTargetDirXZ;
            UpdateDirection();
        }
    }
    if (m_iActionState == DAVIDMONSTER_A_DASH2)
    {
        m_fCalculateTime += _fTimeDelta;

        if ((m_fCalculateTime > m_fDashStartTime) && (m_fCalculateTime < m_fDashEndTime))
        {
            MovePos(m_vCurrentPos + m_vDashDir * m_fDashSpeed * _fTimeDelta);
        }

        // �ð�������
        if (m_pModelCom->GetIsFinishedAnimation())
        {
            m_fCalculateTime = 0.f;
            m_fAnimSpeed = 1.f;
            UpdateDirection();

            return true;
        }
    }
    return false;
}

_bool CNPC_MonsterDavid::UpdateBottomAttack1(_float _fTimeDelta)
{
    if (m_iActionState == DAVIDMONSTER_A_ATT1)
    {
        if (m_pModelCom->GetIsFinishedAnimation())
        {
            m_iActionState = DAVIDMONSTER_A_STDALT;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            return true;
        }
    }
    return false;
}

_bool CNPC_MonsterDavid::UpdateBottomAttack2(_float _fTimeDelta)
{
    if (m_iActionState == DAVIDMONSTER_A_ATT1_2)
    {
        // �и�
        if (m_bParried)
        {
            SetWeaponRimLight(false);
            m_bParried = false;
            m_iActionState = DAVIDMONSTER_A_BLOCKED;
            m_pModelCom->ChangeAnim(m_iActionState, 0.f);
            m_iParriedPower = PARRIEDSTRONG;
            m_fCalculateTime = 0.f;
        }
        if (m_pModelCom->GetIsFinishedAnimation())
        {
            SetWeaponRimLight(false);
            m_iActionState = DAVIDMONSTER_A_STDALT;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            return true;
        }
    }
    if (m_iActionState == DAVIDMONSTER_A_BLOCKED)
    {
        m_fCalculateTime += _fTimeDelta;
        if (m_pModelCom->GetIsFinishedAnimation() && m_fCalculateTime > 0.1f)
        {
            m_bParriedBefore = true;
            m_iActionState = DAVIDMONSTER_A_STDALT;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            m_fCalculateTime = 0.f;
            m_fAnimSpeed = 1.f;
            return true;
        }
    }
    return false;
}

_bool CNPC_MonsterDavid::UpdateUpAttack1(_float _fTimeDelta)
{
    if (m_iActionState == DAVIDMONSTER_A_ATT2)
    {
        if (m_pModelCom->GetIsFinishedAnimation())
        {
            m_iActionState = DAVIDMONSTER_A_STDALT;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            return true;
        }
    }
    return false;
}

_bool CNPC_MonsterDavid::UpdateUpAttack2(_float _fTimeDelta)
{
    if (m_iActionState == DAVIDMONSTER_A_ATT2_2)
    {
        // �и�
        if (m_bParried)
        {
            SetWeaponRimLight(false);
            m_bParried = false;
            m_iActionState = DAVIDMONSTER_A_BLOCKED;
            m_pModelCom->ChangeAnim(m_iActionState, 0.f);
            m_iParriedPower = PARRIEDSTRONG;
            m_fCalculateTime = 0.f;
        }
        if (m_pModelCom->GetIsFinishedAnimation())
        {
            SetWeaponRimLight(false);
            m_iActionState = DAVIDMONSTER_A_STDALT;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            return true;
        }
    }
    if (m_iActionState == DAVIDMONSTER_A_BLOCKED)
    {
        m_fCalculateTime += _fTimeDelta;
        if (m_pModelCom->GetIsFinishedAnimation() && m_fCalculateTime > 0.1f)
        {
            m_bParriedBefore = true;
            m_iActionState = DAVIDMONSTER_A_STDALT;
            m_pModelCom->ChangeAnim(m_iActionState, 0.1f, true);
            m_fCalculateTime = 0.f;
            m_fAnimSpeed = 1.f;
            return true;
        }
    }
    return false;
}

void CNPC_MonsterDavid::GoSwitchPosition()
{
    MovePos(m_vNPCPos + _float3(0.f, 100.f, 0.f));
}

