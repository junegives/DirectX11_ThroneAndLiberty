#include "EffectHelper.h"

CEffectHelper::CEffectHelper()
{
}

CEffectHelper::~CEffectHelper()
{
}

HRESULT CEffectHelper::Initialize(EffectHelper_Desc eDesc)
{
	m_eDesc = eDesc;

	__super::Initialize(nullptr);

	return S_OK;
}

void CEffectHelper::PriorityTick(_float _fTimeDelta)
{
	m_eDesc.fLifeTime -= _fTimeDelta;

	if (0 >= m_eDesc.fLifeTime)
	{
		m_bPlay = false;
		m_IsEnabled = false;
	}
}

void CEffectHelper::Tick(_float _fTimeDelta)
{
	
}

void CEffectHelper::Play(_float _fLifeTime, shared_ptr<CGameObject> _pOwner)
{
	m_bPlay = true;
	m_IsEnabled = true;

	m_pOwner = _pOwner;

	_float3 vStartPos = m_eDesc.HelperKeyFrames.begin()->vPos;
	_float4x4 StartOwnerMatrix;

	if (m_eDesc.bFollowOwnerPos && m_pOwner.lock())
	{
		switch (m_eDesc.iLookType)
		{
		case -1:
			m_pTransformCom->SetState(CTransform::STATE_POSITION, vStartPos + m_pOwner.lock()->GetTransform()->GetState(CTransform::STATE_POSITION));
			break;
		//case 0:
		//	StartOwnerMatrix = m_pOwner.lock()->GetTransform()->GetWorldMatrix();
		//	// �θ��� ���� ��Ŀ��� ȸ�� �κ�(3x3 �κ�)�� �����Ͽ� Z �� ���͸� �����ϴ�.
		//	_float3 parentWorldZAxis = -SimpleMath::Vector3::TransformNormal(SimpleMath::Vector3::UnitZ, StartOwnerMatrix);

		//	// �ڽ��� ���� ������ ���͸� �θ��� ���� ����� Look ���Ϳ� ���� ȸ�� ��ķ� ��ȯ�մϴ�.
		//	_float3 rotatedLocalPosition = SimpleMath::Vector3::Transform(vStartPos, SimpleMath::Matrix::CreateWorld(SimpleMath::Vector3::Zero, parentWorldZAxis, SimpleMath::Vector3::UnitY));

		//	// �θ��� ���� ����� ��ġ ���Ϳ� ȸ���� �ڽ��� ���� ������ ���͸� ���Ͽ� �ڽ��� ���� ����� ��ġ�� ����մϴ�.
		//	_float3 childWorldPosition = StartOwnerMatrix.Translation() + rotatedLocalPosition;

		//	// �ڽ��� ���� ����� �����մϴ�.
		//	vStartPos = SimpleMath::Matrix::CreateTranslation(childWorldPosition).Translation();

		//	m_pTransformCom->SetState(CTransform::STATE_POSITION, vStartPos);
		//	break;
		//case 1:
		//	StartOwnerMatrix = m_pOwner.lock()->GetTransform()->GetWorldMatrix();
		//	// �θ��� ���� ��Ŀ��� ȸ�� �κ�(3x3 �κ�)�� �����Ͽ� Z �� ���͸� �����ϴ�.
		//	_float3 parentWorldZAxis = -SimpleMath::Vector3::TransformNormal(SimpleMath::Vector3::UnitZ, StartOwnerMatrix);

		//	// �ڽ��� ���� ������ ���͸� �θ��� ���� ����� Look ���Ϳ� ���� ȸ�� ��ķ� ��ȯ�մϴ�.
		//	_float3 rotatedLocalPosition = SimpleMath::Vector3::Transform(vStartPos, SimpleMath::Matrix::CreateWorld(SimpleMath::Vector3::Zero, parentWorldZAxis, SimpleMath::Vector3::UnitY));

		//	// �θ��� ���� ����� ��ġ ���Ϳ� ȸ���� �ڽ��� ���� ������ ���͸� ���Ͽ� �ڽ��� ���� ����� ��ġ�� ����մϴ�.
		//	_float3 childWorldPosition = StartOwnerMatrix.Translation() + rotatedLocalPosition;

		//	// �ڽ��� ���� ����� �����մϴ�.
		//	vStartPos = SimpleMath::Matrix::CreateTranslation(childWorldPosition).Translation();

		//	m_pTransformCom->SetState(CTransform::STATE_POSITION, vStartPos);
		//	break;
		default:
			break;
		}
	}

	else
	{
		m_pTransformCom->SetState(CTransform::STATE_POSITION, vStartPos);
	}
}

shared_ptr<CEffectHelper> CEffectHelper::Create(EffectHelper_Desc eDesc)
{
	shared_ptr<CEffectHelper> pInstance = make_shared<CEffectHelper>();

	if (FAILED(pInstance->Initialize(eDesc)))
		MSG_BOX("Failed to Create : CEffectHelper");

	return pInstance;
}
