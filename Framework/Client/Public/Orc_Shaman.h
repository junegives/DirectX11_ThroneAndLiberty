#pragma once

#include "Monster.h"
#include "FireBall.h"
// ����(��������, ���ǹ���, ȭ����2��), ������ ������ �ɸ��� ����
// �������� �и�����

// CoolTime
enum OC_SH_CoolTime
{
	OC_SH_C_NORMALATTACK1 = 0,
	OC_SH_C_NORMALATTACK2,
	OC_SH_C_NORMALATTACK3,
};

// Base ����
enum OC_SH_BaseState
{
	OC_SH_B_BEFOREMEET = 0,
	OC_SH_B_GENERATE,
	OC_SH_B_NORMAL,
	OC_SH_B_HITTED,
	OC_SH_B_HITTED2,
	OC_SH_B_HITTED3,
	OC_SH_B_STUN,
	OC_SH_B_SHOCKED,
	OC_SH_B_DOWN,
	OC_SH_B_DEATH
};

// �Ÿ��� ���� ����
enum OC_SH_DistanceState
{
	OC_SH_D_MOVERANGE = 0,
	OC_SH_D_ATTACK1RANGE,
	OC_SH_D_ATTACK2RANGE,
	OC_SH_D_OUTRANGE,
	OC_SH_D_BEFOREMEET
};

// Ư�� �׼� ����
enum OC_SH_ActionState
{
	OC_SH_A_BLOCKED = 1,
	OC_SH_A_COMBATSTART = 2,
	OC_SH_A_RUNFL = 6,
	OC_SH_A_RUNFR = 7,
	OC_SH_A_SEARCH = 8,
	OC_SH_A_STDALT1 = 35,
	OC_SH_A_STDALT2 = 36,
	OC_SH_A_STDALT3 = 37,
	OC_SH_A_DEAD = 42,
	OC_SH_A_DEATH = 43,
	OC_SH_A_DOWN3 = 44,
	OC_SH_A_DOWN2 = 45,
	OC_SH_A_DOWN1 = 46,
	OC_SH_A_FRONTBE = 53,
	OC_SH_A_FRONTBL = 54,
	OC_SH_A_FRONTBS = 55,
	OC_SH_A_RUN = 56,
	OC_SH_A_SHOCKED = 57,
	OC_SH_A_ATTACK1 = 58,	// ��������
	OC_SH_A_ATTACK2 = 59,	// ����
	OC_SH_A_ATTACK3 = 60,	// ȭ���� 2��
	OC_SH_A_STDALT = 62,
	OC_SH_A_STDIDL = 63,
	OC_SH_A_STUN = 64,
	OC_SH_A_WALK = 65
};

BEGIN(Client)

class COrc_Shaman final : public CMonster
{
public:
	struct ORC_SHAMAN_DESC
	{
		CMonster::MONSTER_DESC pTarget;
	};

public:
	COrc_Shaman();
	virtual ~COrc_Shaman()  override;

public:
	HRESULT Initialize();
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<COrc_Shaman> Create(_float3 _vCreatePos);

protected:
	_bool	IsParried();
	HRESULT AddRigidBody(string _strModelKey);		// RigidBody �߰�
	virtual void OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ��� ����
	virtual void OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;	// ��� ���� ��
	virtual void OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ������

	virtual void EnableCollision(const char* _strShapeTag) override;
	virtual void DisableCollision(const char* _strShapeTag) override;

	void		ActiveFireBall();

	HRESULT		AddPartObjects(string _strModelKey);
	void		UpdateDistanceState();
	void		UpdateCoolTimeRange(_float _fTimeDelta);
	void		UpdateCoolTimeMelee(_float _fTimeDelta);

	// BaseState ���� FSM
	void		FSM_BeforeMeet(_float _fTimeDelta);
	void		FSM_Generate(_float _fTimeDelta);
	void		FSM_Normal(_float _fTimeDelta);
	void		FSM_Hitted(_float _fTimeDelta);
	void		FSM_Hitted2(_float _fTimeDelta);
	void		FSM_Hitted3(_float _fTimeDelta);
	void		FSM_Stun(_float _fTimeDelta);
	void		FSM_Shocked(_float _fTimeDelta);
	void		FSM_Down(_float _fTimeDelta);
	void		FSM_Death(_float _fTimeDelta);

	void		CheckDead();
	void		CheckAttacked();

private:
	_int		m_iBaseState = OC_SH_B_BEFOREMEET;			// Base����
	_int		m_iDistanceState = OC_SH_D_BEFOREMEET;		// �Ÿ��� ���� ����
	_int		m_iActionState = OC_SH_A_STDALT1;			// �ൿ����

	_float		m_fAttack1CoolTimeRange = 5.f;				// �������� ���ִ� �Ÿ�
	_float		m_fAttack1Range = 2.f;						// �������� Range
	_float		m_fAttack2Range = 15.f;						// ���� Range
	_float		m_fOutRange = 10000.f;						// �������� ���� Range
	_float		m_fMeetRange = 18.f;						// ���� Range

	_float		m_fCalculateTime = 0.f;						// ���� Time
	_float		m_fCalculateTime2 = 0.f;						// ���� Time

	// ȭ����
	vector<shared_ptr<CFireBall>>		m_vecFireBall;
	_int		m_iFireBallCount = 10;

	_float		m_fFireBallStartTime1 = m_fKeyFrameSpeed * 20.f * 1.25f;
	_float		m_fFireBallStartTime2 = m_fKeyFrameSpeed * 60.f * 1.25f;
	_float		m_fBuffStartTime = m_fKeyFrameSpeed * 72.f;
	_bool		m_bBuffSoundOn = false;
	_bool		m_bFireBallFire1 = false;
	_bool		m_bFireBallFire2 = false;

	// ��Ÿ��
	_bool		m_bAttackReady[3] = { true, false, false };
	_float		m_fAttackCoolTime[3] = { 2.f, 10.f, 5.f };
	_float		m_fBAttackCoolTime[3] = { 2.f, 2000.f, 5.f };

	_int		m_NormalAttackCount = 3;	// Normal ���� ���� ����

	// �ݶ��̴� ����
	const char* m_strATT1Collider1 = "M_Orc_Shaman_SK.ao|M_Orc_Shaman_USkdmgA";
};

END