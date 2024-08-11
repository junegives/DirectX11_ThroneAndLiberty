#pragma once

#include "Monster.h"
#include "Arrow.h"
// ����ȭ(ü�� ���� ���Ͻ� �ൿ ������), ����(�Ϲݹ߻�, (����ź)���߻�), ȭ���и�����

// CoolTime
enum LA_A_CoolTime
{
	LA_AC_C_NORMALATTACK1 = 0,
	LA_AC_C_NORMALATTACK2,
	LA_AC_C_FURYATTACK1,
	LA_AC_C_FURYATTACK2
};

// Base ����
enum LA_A_BaseState
{
	LA_AC_B_BEFOREMEET = 0,
	LA_AC_B_GENERATE,
	LA_AC_B_NORMAL,
	LA_AC_B_WHILEFURY,
	LA_AC_B_FURY,
	LA_AC_B_HITTED,
	LA_AC_B_HITTED2,
	LA_AC_B_HITTED3,
	LA_AC_B_STUN,
	LA_AC_B_SHOCKED,
	LA_AC_B_DOWN,
	LA_AC_B_DEATH
};

// �Ÿ��� ���� ����
enum LA_A_DistanceState
{
	LA_AC_D_MOVERANGE = 0,
	LA_AC_D_ATTACKRANGE,
	LA_AC_D_OUTRANGE,
	LA_AC_D_BEFOREMEET
};

// Ư�� �׼� ����
enum LA_A_ActionState
{
	LA_AC_A_COMBATSTART = 4,
	LA_AC_A_DEAD = 5,
	LA_AC_A_DOWN3 = 8,
	LA_AC_A_DOWN2 = 9,
	LA_AC_A_DOWN1 = 10,
	LA_AC_A_FRONTBE = 14,
	LA_AC_A_FRONTBL = 15,
	LA_AC_A_FRONTBS = 16,
	LA_AC_A_RUN = 20,
	LA_AC_A_SEARCH3 = 21,
	LA_AC_A_SEARCH2 = 22,
	LA_AC_A_SEARCH1 = 23,
	LA_AC_A_SHOCKED = 24,
	LA_AC_A_FURY = 25,
	LA_AC_A_ATTACK1 = 26,
	LA_AC_A_ATTACK2 = 27,
	LA_AC_A_APPEAR = 29,
	LA_AC_A_STDALT1 = 30,
	LA_AC_A_STDALT2 = 31,
	LA_AC_A_STDALT3 = 32,
	LA_AC_A_STDALT4 = 33,
	LA_AC_A_STDIDL = 34,
	LA_AC_A_STUN = 35,
	LA_AC_A_WALK = 36
};

BEGIN(Client)

class CLivingArmor_Archer final : public CMonster
{
public:
	struct LIVINGARMOR_ARCHER_DESC
	{
		CMonster::MONSTER_DESC pTarget;
	};

public:
	CLivingArmor_Archer();
	virtual ~CLivingArmor_Archer()  override;

public:
	HRESULT Initialize();
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<CLivingArmor_Archer> Create(_float3 _vCreatePos);

protected:
	HRESULT AddRigidBody(string _strModelKey);		// RigidBody �߰�
	virtual void OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ��� ����
	virtual void OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;	// ��� ���� ��
	virtual void OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ������

	virtual void EnableCollision(const char* _strShapeTag) override;
	virtual void DisableCollision(const char* _strShapeTag) override;

	HRESULT		AddPartObjects(string _strModelKey);
	void		UpdateDistanceState();
	void		UpdateCoolTime(_float _fTimeDelta);
	void		ActiveNormalArrow();
	void		ActivePowerArrow();

	// BaseState ���� FSM
	void		FSM_BeforeMeet(_float _fTimeDelta);
	void		FSM_Generate(_float _fTimeDelta);
	void		FSM_Normal(_float _fTimeDelta);
	void		FSM_WhileFury(_float _fTimeDelta);
	void		FSM_Fury(_float _fTimeDelta);
	void		FSM_Hitted(_float _fTimeDelta);
	void		FSM_Hitted2(_float _fTimeDelta);
	void		FSM_Hitted3(_float _fTimeDelta);
	void		FSM_Stun(_float _fTimeDelta);
	void		FSM_Shocked(_float _fTimeDelta);
	void		FSM_Down(_float _fTimeDelta);
	void		FSM_Death(_float _fTimeDelta);

	void		CheckFury();
	void		CheckDead();
	void		CheckAttacked();

private:
	_int		m_iEffectIdx = 0;

	_int		m_iBaseState = LA_AC_B_BEFOREMEET;		// Base����
	_int		m_iDistanceState = LA_AC_D_BEFOREMEET;	// �Ÿ��� ���� ����
	_int		m_iActionState = LA_AC_A_DOWN2;			// �ൿ����

	_float		m_fAttack1Range = 30.f;					// ���� Range
	_float		m_fOutRange = 10000.f;					// �������� ���� Range
	_float		m_fMeetRange = 25.f;					// ���� Range

	// ȭ�� ���� Render����
	_float		m_fCalculateTime = 0.f;					// ���� Time
	_float		m_fAttack1ArrowGenTime = 10.f * 0.05f;
	_float		m_fAttack1ArrowOutTime = 35.f * 0.05f;
	_float		m_fAttack2ArrowGenTime = 20.f * 0.05f;
	_float		m_fAttack2ArrowOutTime = 83.f * 0.05f;
	_float		m_fFAttack1ArrowGenTime = 10.f * 0.05f / 1.5f;
	_float		m_fFAttack1ArrowOutTime = 35.f * 0.05f / 1.5f;
	_float		m_fFAttack2ArrowGenTime = 20.f * 0.05f / 1.5f;
	_float		m_fFAttack2ArrowOutTime = 83.f * 0.05f / 1.5f;

	_bool		m_bPartArrowGen = false;

	// ��Ÿ��
	_bool		m_bAttackReady[4] = { true, false, false, false };
	_float		m_fAttackCoolTime[4] = { 1.8f, 2.5f, 1.5f, 2.f };
	_float		m_fBAttackCoolTime[4] = { 1.8f, 2.5f, 1.5f, 2.f };

	_int		m_NormalAttackCount = 2;	// Normal ���� ���� ����
	_int		m_FuryAttackCount = 2;		// Fury ���� ���� ����

	// ����ȭ�Ǵ� hp
	_float		m_fFuryHp = 50.f;
	// ����ȭ�� �������� �ӵ�
	_float		m_fFuryActionSpeed = 1.2f;
	
	// ����ȭ�� �ӵ�
	_float		m_fFurySpeed = 1.2f;

	// ȭ��
	vector<shared_ptr<CArrow>>		m_vecArrow;
	_int		m_iArrowCount = 10;
};

END