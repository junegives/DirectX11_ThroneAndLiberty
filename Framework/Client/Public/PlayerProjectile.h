#pragma once
#include "GameObject.h"
#include "Client_Defines.h"
#include "RigidBody.h"
#include "EffectMgr.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

enum PlayerProjAbnormal
{
	PLAYER_PJ_A_NORMAL,				// �Ϲ� �ǰ�
	PLAYER_PJ_A_STIFF_SHORT,		// ª�� ����
	PLAYER_PJ_A_STIFF_LONG,			// �� ����
	PLAYER_PJ_A_STUN,				// ����
	PLAYER_PJ_A_SHOCK,				// ����
	PLAYER_PJ_A_DOWN,				// �Ѿ�߸���
	PLAYER_PJ_NONE					// X
};

class CPlayerProjectile : public CGameObject
{
public:
	CPlayerProjectile();
	virtual ~CPlayerProjectile()  override;

public:
	virtual HRESULT Initialize(shared_ptr<CTransform::TRANSFORM_DESC> pArg);
	virtual void	PriorityTick(_float _fTimeDelta)  override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta)  override;
	virtual HRESULT Render()  override;
	virtual HRESULT RenderGlow() override;

public:
	PlayerProjAbnormal GetAbnormal() { return m_eMyAbNormal; }

public:
	virtual void	EnableProjectile(_float3 _vMyStartPos, _float3 _vTargetPos, PlayerProjAbnormal _ePlayerProjAbnormal, _bool isDistinguish = false, _uint _iIndex = 0) {};

protected:

protected:
	shared_ptr<CModel>		m_pModelCom = nullptr;
	shared_ptr<CRigidBody>	m_pRigidBody = { nullptr };
	PlayerProjAbnormal		m_eMyAbNormal = PLAYER_PJ_A_NORMAL;
};

END