#pragma once

#include "GameObject.h"
#include "PartObject.h"
#include "Client_Defines.h"
#include "RigidBody.h"
#include "EffectMgr.h"


BEGIN(Engine)
class CModel;
class CTexture;
END

BEGIN(Client)

enum ProjectileType
{
	PJ_DIRECT = 0,
	PJ_READYDIRECT,					// ����� �������� ������
	PJ_GUIDED,
	PJ_READYGUIDED,					// ����� ����ź
	PJ_FALL,						// ��������
	PJ_SUMMON,						// ��ȯ
	PJ_NONE
};

enum ProjectileAbnormal
{
	PJ_A_NORMAL,					// �Ϲ� �ǰ�
	PJ_A_SHORTHITTED,				// ª�� ����
	PJ_A_LONGHITTED,				// �� ����
	PJ_A_GOOUT,						// ���󰡱�
	PJ_A_BIND,						// �ӹ�
	PJ_A_SHOCK						// ����
};


class CProjectile : public CGameObject
{
public:
	struct PROJECTILE_DESC
	{
		shared_ptr<CGameObject> pTarget = nullptr;
	};

public:
	CProjectile();
	virtual ~CProjectile()  override;

public:
	virtual HRESULT Initialize(PROJECTILE_DESC* _pProjectileDesc, CTransform::TRANSFORM_DESC* _pTransformDesc);
	virtual void	PriorityTick(_float _fTimeDelta)  override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta)  override;
	virtual HRESULT Render()  override;
	virtual HRESULT RenderGlow() override;

public:
	_float3	GetCurrentPos() { return m_vCurrentPos; }
	void	SetOnlyRender(_bool _bOnlyRender) { m_bOnlyRender = _bOnlyRender; }
	void	OnCollisionProjectile();
	void	OffCollisionProjectile();
	void	CheckCollisionActive();
	void	UseGlow() { m_bUseGlow = true; };
	void	SimulationOn() { m_pRigidBody->SimulationOn(); m_bSimulationOn = true; }
	void	SimulationOff() { m_pRigidBody->SimulationOff();  m_bSimulationOn = false;}

public:
	_int		GetPower() { return m_iPower; }
	void		SetPower(_int _iPower) { m_iPower = _iPower; }
	_int		GetAbnormal() { return m_iAbnormal; }
	void		SetAbnormal(_int _iAbnormal) { m_iAbnormal = _iAbnormal; }
	void		UpdateTargetPos();
	void		UpdateTargetDir(_float3 _vUp);
	void		UpdateDirection();	// �߻�ü ������� �־�� �Ҷ�
	void		MoveToTarget(_float _fTimeDelta);	// Ÿ������ �̵�
	void		MoveToDir(_float _fTimeDelta);
	void		SetProjectileType(_int _iProjectileType) { m_iProjectileType = _iProjectileType; }
	void		SetReady(_float _fReadyTime) { m_fReadyTime = _fReadyTime; m_bReadyFire = true; }
	void		OnEffect() { m_bStartEffect = true; }

protected:
	void		MovePos(_float3 _vPos);

protected:
	shared_ptr<CModel> m_pModelCom = nullptr;
	shared_ptr<CRigidBody>	m_pRigidBody = { nullptr };				// Rigid Body
	
protected:
	_bool			m_bSimulationOn = false;			// �ùķ��̼��� �۵��ƴ���

	// Tick������ ���� �浹ü ���ֱ� ����
	_bool			m_bCollisionActive = false;
	_bool			m_bPrevCollisionActive = false;
	const char*		m_strShapeTag = "";

	_bool			m_bActiveOff = false;			// m_bEnable�� latetick�������� ȣ�����ֱ� ����

	_bool			m_bStartEffect = false;

	// Ÿ��
	weak_ptr<CGameObject>		m_pTarget;
	_float3			m_vTargetPos = _float3(0.f, 0.f, 0.f);
	_float3			m_vTargetDir = _float3(0.f, 0.f, 0.f);
	_float3			m_vCurrentPos = _float3(0.f, 0.f, 0.f);
	_float			m_fDistanceToTarget = 30.f;	// �پ��� �������� ���Ƕ����� 30

	weak_ptr<CGameObject>		m_pOwner;

	// ���� �ð�
	_float			m_fCalculateTime1 = 0.f;
	_float			m_fCalculateTime2 = 0.f;
	_float			m_fCalculateTime3 = 0.f;

	// ����ð�
	_float			m_fLifeTime = 0.f;

	// �߻�ü Ÿ��
	_int			m_iProjectileType = PJ_DIRECT;
	// �߻�ü �߻��� ���ð�
	_float			m_fReadyTime = 1.f;
	_bool			m_bReadyFire = true;
	// �߻�ü ����
	_int			m_iPower = 1;	// (1~3, 3�ϼ��� ����)
	// �߻�ü �����̻�
	_int			m_iAbnormal = PJ_A_NORMAL;

	// Render�� �ǰ�
	_bool			m_bOnlyRender = false;

	// Glow��
	_bool			m_bUseGlow = false;			// �۷ο� ��������
	_bool			m_bUseGlowColor = false;	// �۷ο� ���� ������ ���� (�ȳ����� ���� ���� ��¦ ����)
	_float4			m_vGlowColor = { 1.f, 1.f, 1.f, 1.f };	// �۷ο� ����
};

END