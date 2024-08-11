#include "ShockWave.h"
#include "Model.h"
#include "RigidBody.h"

CShockWave::CShockWave()
{
}

CShockWave::~CShockWave()
{
}

HRESULT CShockWave::Initialize(SHOCKWAVE_DESC* _pShockWaveDesc, CTransform::TRANSFORM_DESC* _pTransformDesc)
{
    PROJECTILE_DESC* pProjectileDesc = (PROJECTILE_DESC*)_pShockWaveDesc;

    __super::Initialize(pProjectileDesc, _pTransformDesc);

    /*Shader Info*/
    m_eShaderType = ESHADER_TYPE::ST_NONANIM;
    m_iShaderPass = 0;

    m_pModelCom = GAMEINSTANCE->GetModel("Arrow_00001");
    m_pTransformCom->SetScaling(1.f, 1.f, 1.f);

    // �߻�ü ���� �����ֱ�
    m_iProjectileType = PJ_NONE;

    AddRigidBody();

    SimulationOff();

    m_IsEnabled = false;

    return S_OK;
}

void CShockWave::PriorityTick(_float _fTimeDelta)
{
    if (m_bOnlyRender)
        return;

    __super::PriorityTick(_fTimeDelta);
}

void CShockWave::Tick(_float _fTimeDelta)
{
    CheckCollisionActive();

    if (m_bOnlyRender)
        return;
    
    __super::Tick(_fTimeDelta);
    
    // �����ð� �ִٰ�
    if (m_bSimulationOn)
    {
        m_vCurrentPos;
        m_fCalculateTime += _fTimeDelta;

        if (m_fCalculateTime > m_fCollisionTime)
        {
            m_vCurrentPos;
            m_fCalculateTime = 0.f;
            SimulationOff();
            m_IsEnabled = false;
        }
    }

    m_pTransformCom->Tick(_fTimeDelta);
}

void CShockWave::LateTick(_float _fTimeDelta)
{
    if (!m_bOnlyRender)
    {
        m_pTransformCom->LateTick(_fTimeDelta);         
        __super::LateTick(_fTimeDelta);
    }

    if (GAMEINSTANCE->isInWorldSpace(m_pTransformCom->GetState(CTransform::STATE_POSITION), 4.f))
    {
		GAMEINSTANCE->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this());

#ifdef _DEBUG
		if (m_pRigidBody)
			GAMEINSTANCE->AddDebugComponent(m_pRigidBody);
#endif // _DEBUG
    }

    if (m_bActiveOff)
    {
        m_pRigidBody->DisableCollision(m_strShapeTag);
        m_bCollisionActive = false;
        m_bPrevCollisionActive = false;
        m_bActiveOff = false;
        SimulationOff();
        m_IsEnabled = false;
    }
}

HRESULT CShockWave::Render()
{
    __super::Render();

    _uint iNumMeshes = m_pModelCom->GetNumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++) {
        GAMEINSTANCE->BeginNonAnimModel(m_pModelCom, (_uint)i);
    }
    
    return S_OK;
}

HRESULT CShockWave::AddRigidBody()
{
    CRigidBody::RIGIDBODYDESC RigidBodyDesc;

    /* Create Actor */
//bool			isStatic = false;	/* ������ ��ü�� true */
//bool			isTrigger = false;	/* Ʈ���� ��ü�� true, false�� �����ۿ��, true�� �����ۿ��� �ȵ� */
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
    RigidBodyDesc.isTrigger = true;
    RigidBodyDesc.isGravity = false;
    RigidBodyDesc.isInitCol = false;
    RigidBodyDesc.pTransform = m_pTransformCom;
    RigidBodyDesc.eLock_Flag = CRigidBody::LOCK_ROT_ALL;

    RigidBodyDesc.pOwnerObject = shared_from_this();
    RigidBodyDesc.fStaticFriction = 1.0f;
    RigidBodyDesc.fDynamicFriction = 0.0f;
    RigidBodyDesc.fRestitution = 0.0f;

    GeometryBox RectDesc;
    RectDesc.vSize.x = 5.f;
    RectDesc.vSize.y = 0.5f;
    RectDesc.vSize.z = 5.f;
    RectDesc.vOffSetPosition = { 0.05f, 0.05f, 0.6f };
    RectDesc.vOffSetRotation = { 0.0f, 0.0f, 90.0f };
    RectDesc.strShapeTag = "ShockWave_Body";
    m_strShapeTag = "ShockWave_Body";
    RigidBodyDesc.pGeometry = &RectDesc;

   /* GeometryCapsule CapsuleDesc;
    CapsuleDesc.fHeight = 0.4f;
    CapsuleDesc.fRadius = 0.1f;
    CapsuleDesc.vOffSetPosition = { 0.0f, 0.2f, 0.0f };
    CapsuleDesc.vOffSetRotation = { 0.0f, 0.0f, 0.0f };
    CapsuleDesc.strShapeTag = "ShockWave_Body";
    RigidBodyDesc.pGeometry = &CapsuleDesc;*/

    RigidBodyDesc.eThisColFlag = COL_MONSTERPROJECTILE;
    RigidBodyDesc.eTargetColFlag = COL_PLAYER | COL_PLAYERPARRYING;
    m_pRigidBody = GAMEINSTANCE->CloneRigidBody(&RigidBodyDesc);

    m_pTransformCom->SetRigidBody(m_pRigidBody);

    return S_OK;
}

void CShockWave::OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
    // ���������� �ε�������
    if (_ColData.eColFlag & COL_STATIC)
    {
        return;
    }

    // �÷��̾ �ε�������
    if ((_ColData.eColFlag & COL_PLAYER) && !m_bOnlyRender)
    {
        m_bActiveOff = true;
    }
    // �÷��̾� �и�
    if ((_ColData.eColFlag & COL_PLAYERPARRYING) && !m_bOnlyRender)
    {
        m_bActiveOff = true;
    }
}

void CShockWave::OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

void CShockWave::OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

shared_ptr<CShockWave> CShockWave::Create(SHOCKWAVE_DESC* _pShockWaveDesc, CTransform::TRANSFORM_DESC* _pTransformDesc)
{
    shared_ptr<CShockWave> pInstance = make_shared<CShockWave>();

    if (FAILED(pInstance->Initialize(_pShockWaveDesc, _pTransformDesc)))
        MSG_BOX("Failed to Create : ShockWave");

    return pInstance;
}
