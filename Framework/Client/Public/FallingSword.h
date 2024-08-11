#pragma once

#include "GameObject.h"
#include "Projectile.h"
#include "ShockWave.h"
#include "Client_Defines.h"

BEGIN(Client)

class CUIBar;

class CFallingSword final: public CProjectile
{
public:
	struct FALLINGSWORD_DESC
	{
		CProjectile::PROJECTILE_DESC tProjectile;
	};

public:
	CFallingSword();
	virtual ~CFallingSword()  override;

public:
	virtual HRESULT Initialize(FALLINGSWORD_DESC* _pFallingSwordDesc, CTransform::TRANSFORM_DESC* _pTransformDesc);
	virtual void	PriorityTick(_float _fTimeDelta)  override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta)  override;
	virtual HRESULT Render()  override;
	virtual HRESULT RenderGlow() override;
	virtual HRESULT RenderDistortion() override;

public:
	static shared_ptr<CFallingSword> Create(FALLINGSWORD_DESC* _pFallingSwordDesc, CTransform::TRANSFORM_DESC* _pTransformDesc);
	void	FallStart() { m_bStartFall = true; }
	void	SetTargetOn(_bool _bTargetOn) { m_bTargetOn = _bTargetOn; }
	_bool	GetTargetOn() { return m_bTargetOn; }
	void	SetTargetOut(_bool _bTargetOut) { m_bTargetOut = _bTargetOut; }
	_bool	GetTargetOut() { return m_bTargetOut; }
	void	SetFallPos(_float3 _vFallPos) { m_vFallPos = _vFallPos; }
	void	AttackOn() { m_bSwordAttack = true; }
	_bool	GetFallGround() { return m_bFallGround; }
	void	SetFallGround(_bool _bFallGround) { m_bFallGround = _bFallGround; }
	void	SetRise(_bool _bRise) { m_bRise = _bRise; }
	void	SetSwordType(_int _iType) { m_iSwordType = _iType; }
	void	GravityOff();
	
protected:
	HRESULT			AddRigidBody();		// RigidBody �߰�
	virtual void	OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ��� ����
	virtual void	OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;	// ��� ���� ��
	virtual void	OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ������

protected:
	HRESULT			PrepareHPBar();
	void			UpdateUIPos();
	void			UpdateHPBar();

public:
	void			PlayEffect(const string& _strEffectID);
	void			StopEffect(const string& _strEffectID);

private:
	_float		m_fHp = 100.f;
	_bool		m_bStartFall = false;		// ó�� �� �������� �����ϴ� ����
	_bool		m_bSwordAttack = false;		// ������ ������ �˵�
	_bool		m_bTargetOn = false;		// ������ ������ ����
	_bool		m_bTargetOut = false;		// ������ �� ü�� �ٱ��

	_float		m_fCalculateTime = 0.f;		// ���� Ÿ�̸�
	_float		m_fCalculateTime2 = 0.f;		// ���� Ÿ�̸�

	_bool		m_bMoveState[10] = { 0, };
	_bool		m_bFallGround = false;		// ���� ���� ����
	_bool		m_bRising = false;			// �����ٰ� �ö󰡴� ��
	_bool		m_bRise = false;			
	_float		m_fUpSpeed = 3.f;
	_float		m_fRiseSpeed = 10.f;

	_bool		m_vShakeUp = false;			// �Ʒ��� ��鸮����
	_float		m_fShakeIntervel = 0.05f;	// ��鸮�� ����
	
	_float3		m_vFallPos = _float3(0.f, 0.f, 0.f);

	_int		m_iSwordType = 0;			// �� Ÿ��	1: ��, 2: ��, 3: �߾�
	_bool		m_bStartRotation = false;
	_float		m_fShakePower = 8.f;
	_bool		m_bFallMove = false;
	_float		m_fMoveSpeed = 10.f;

	_int		m_iEffectIdx = -1;

private: /* For UI */
	_float m_fMaxHP = 100.f;
	
	shared_ptr<CUIBar> m_pHPBar{ nullptr };
};

END