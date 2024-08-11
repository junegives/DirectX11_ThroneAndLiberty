#include "Arrow.h"
#include "Model.h"
#include "RigidBody.h"

CArrow::CArrow()
{
}

CArrow::~CArrow()
{
}

HRESULT CArrow::Initialize(ARROW_DESC* _pArrowDesc, CTransform::TRANSFORM_DESC* _pTransformDesc)
{
    PROJECTILE_DESC* pProjectileDesc = (PROJECTILE_DESC*)_pArrowDesc;

    __super::Initialize(pProjectileDesc, _pTransformDesc);

    /*Shader Info*/
    m_eShaderType = ESHADER_TYPE::ST_NONANIM;
    m_iShaderPass = 0;

    switch (_pArrowDesc->m_iArrowType)
    {
    case 0:
        m_pModelCom = GAMEINSTANCE->GetModel("Arrow_00001");
        break;
    case 1:
        m_pModelCom = GAMEINSTANCE->GetModel("Arrow_00002");
        break;
    }
    m_pTransformCom->SetScaling(1.f, 1.f, 1.f);

    // �߻�ü ���� �����ֱ�
    m_iProjectileType = PJ_DIRECT;

    AddRigidBody();

    SimulationOff();

    m_IsEnabled = false;

    return S_OK;
}

void CArrow::PriorityTick(_float _fTimeDelta)
{
    if (m_bOnlyRender)
        return;

    __super::PriorityTick(_fTimeDelta);
}

void CArrow::Tick(_float _fTimeDelta)
{
    CheckCollisionActive();

    if (m_bOnlyRender)
        return;

    __super::Tick(_fTimeDelta);
    //m_pRigidBody->EnableCollision("Arrow_Body");
    // �����Ÿ� �����
    if (m_fDistanceToTarget > 50.f)
    {
        m_vCurrentPos.y = 100.f;
        m_fDistanceToTarget = 30.f;
        SimulationOff();
        m_IsEnabled = false;
    }

    m_pTransformCom->Tick(_fTimeDelta);
}

void CArrow::LateTick(_float _fTimeDelta)
{
	if (!m_bOnlyRender)
	{
		m_pTransformCom->LateTick(_fTimeDelta);
	}

	if (GAMEINSTANCE->isInWorldSpace(m_pTransformCom->GetState(CTransform::STATE_POSITION), 2.f))
	{
		if (!m_bOnlyRender)
		{
			__super::LateTick(_fTimeDelta);
		}

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

HRESULT CArrow::Render()
{
    __super::Render();

    _uint iNumMeshes = m_pModelCom->GetNumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++) {
        GAMEINSTANCE->BeginNonAnimModel(m_pModelCom, (_uint)i);
    }
    
    return S_OK;
}

HRESULT CArrow::AddRigidBody()
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
    RectDesc.vSize.x = 0.1f;
    RectDesc.vSize.y = 0.1f;
    RectDesc.vSize.z = 1.2f;
    RectDesc.vOffSetPosition = { 0.05f, 0.05f, 0.6f };
    RectDesc.vOffSetRotation = { 0.0f, 0.0f, 90.0f };
    RectDesc.strShapeTag = "Arrow_Body";
    m_strShapeTag = "Arrow_Body";
    RigidBodyDesc.pGeometry = &RectDesc;

   /* GeometryCapsule CapsuleDesc;
    CapsuleDesc.fHeight = 0.4f;
    CapsuleDesc.fRadius = 0.1f;
    CapsuleDesc.vOffSetPosition = { 0.0f, 0.2f, 0.0f };
    CapsuleDesc.vOffSetRotation = { 0.0f, 0.0f, 0.0f };
    CapsuleDesc.strShapeTag = "Arrow_Body";
    RigidBodyDesc.pGeometry = &CapsuleDesc;*/

    RigidBodyDesc.eThisColFlag = COL_MONSTERPROJECTILE;
    RigidBodyDesc.eTargetColFlag = COL_STATIC | COL_PLAYER | COL_PLAYERPARRYING;
    m_pRigidBody = GAMEINSTANCE->CloneRigidBody(&RigidBodyDesc);

    m_pTransformCom->SetRigidBody(m_pRigidBody);

    return S_OK;
}

void CArrow::OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
    // ���������� �ε�������
    if (_ColData.eColFlag & COL_STATIC)
    {
        m_bCollisionActive = false;
        m_bOnlyRender = true;
        
        if (m_iProjectileType == PJ_DIRECT)
        {
            EFFECTMGR->StopEffect("Mon_Arrow_Basic", m_iDeleteKey);
        }
        if (m_iProjectileType == PJ_GUIDED)
        {
            EFFECTMGR->StopEffect("Mon_Arrow_Enhanced", m_iDeleteKey);
        }
    }
    // �÷��̾ �ε�������
    if ((_ColData.eColFlag & COL_PLAYER) && !m_bOnlyRender)
    {
        m_pTransformCom->SetPositionParam(1, 100.f);
        m_vCurrentPos = _float3(m_pTransformCom->GetState(CTransform::STATE_POSITION));
        m_fDistanceToTarget = 30.f;
        m_bActiveOff = true;
        if (m_iProjectileType == PJ_DIRECT)
        {
            EFFECTMGR->StopEffect("Mon_Arrow_Basic", m_iDeleteKey);
        }
        if (m_iProjectileType == PJ_GUIDED)
        {
            EFFECTMGR->StopEffect("Mon_Arrow_Enhanced", m_iDeleteKey);
            GAMEINSTANCE->PlaySoundW("LA_ArrowHitted", 0.5f);
        }
    }
    // �÷��̾� ���⿡ �ε�������
    if ((_ColData.eColFlag & COL_PLAYERPARRYING) && !m_bOnlyRender)
    {
        m_pTransformCom->SetPositionParam(1, 100.f);
        m_vCurrentPos = _float3(m_pTransformCom->GetState(CTransform::STATE_POSITION));
        m_fDistanceToTarget = 30.f;
        m_bActiveOff = true;
        if (m_iProjectileType == PJ_DIRECT)
        {
            EFFECTMGR->StopEffect("Mon_Arrow_Basic", m_iDeleteKey);
        }
        if (m_iProjectileType == PJ_GUIDED)
        {
            EFFECTMGR->StopEffect("Mon_Arrow_Enhanced", m_iDeleteKey);
            GAMEINSTANCE->PlaySoundW("LA_ArrowHitted", 0.5f);
        }
    }
}

void CArrow::OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

void CArrow::OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag)
{
}

shared_ptr<CArrow> CArrow::Create(ARROW_DESC* _pArrowDesc, CTransform::TRANSFORM_DESC* _pTransformDesc)
{
    shared_ptr<CArrow> pInstance = make_shared<CArrow>();

    if (FAILED(pInstance->Initialize(_pArrowDesc, _pTransformDesc)))
        MSG_BOX("Failed to Create : Arrow");

    return pInstance;
}
