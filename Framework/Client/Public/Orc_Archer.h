#pragma once

#include "Monster.h"
#include "Arrow.h"

class CArrow;
// ����(��������, �Ϲݹ߻�), ������ ������ �ɸ��� ����(����, ���ݷ� up),
// �������� �и�����, ȭ�� �и�����

// CoolTime
enum OC_AC_CoolTime
{
	OC_AC_C_NORMALATTACK1 = 0,
	OC_AC_C_NORMALATTACK2,
	OC_AC_C_FURYATTACK1,
	OC_AC_C_FURYATTACK2,
};

// Base ����
enum OC_AC_BaseState
{
	OC_AC_B_BEFOREMEET = 0,
	OC_AC_B_GENERATE,
	OC_AC_B_NORMAL,
	OC_AC_B_WHILEFURY,
	OC_AC_B_FURY,
	OC_AC_B_HITTED,
	OC_AC_B_HITTED2,
	OC_AC_B_HITTED3,
	OC_AC_B_STUN,
	OC_AC_B_SHOCKED,
	OC_AC_B_DOWN,
	OC_AC_B_DEATH
};

// �Ÿ��� ���� ����
enum OC_AC_DistanceState
{
	OC_AC_D_MOVERANGE = 0,
	OC_AC_D_ATTACK1RANGE,
	OC_AC_D_ATTACK2RANGE,
	OC_AC_D_OUTRANGE,
	OC_AC_D_BEFOREMEET
};

// Ư�� �׼� ����
enum OC_AC_ActionState
{
	OC_AC_A_BLOCKED = 0,
	OC_AC_A_COMBATSTART = 1,
	OC_AC_A_RUNFL = 5,
	OC_AC_A_RUNFR = 6,
	OC_AC_A_SEARCH = 7,
	OC_AC_A_STDALT1 = 22,
	OC_AC_A_STDALT2 = 23,
	OC_AC_A_DEAD = 28,
	OC_AC_A_DEATH = 29,
	OC_AC_A_DOWN3 = 30,
	OC_AC_A_DOWN2 = 31,
	OC_AC_A_DOWN1 = 32,
	OC_AC_A_FRONTBE = 39,
	OC_AC_A_FRONTBL = 40,
	OC_AC_A_FRONTBS = 41,
	OC_AC_A_RUN = 42,
	OC_AC_A_SHOCKED = 43,
	OC_AC_A_BUFF = 44,
	OC_AC_A_ATTACK1 = 45,	// ��������
	OC_AC_A_ATTACK2 = 46,	// ȭ��߻�
	OC_AC_A_JUMPTO1 = 47,	// ���� �غ�
	OC_AC_A_JUMPTO2 = 48,	// ���� ����
	OC_AC_A_JUMPTO3 = 49,	// ���� �Ʒ�
	OC_AC_A_JUMPTO4 = 50,	// ����
	OC_AC_A_STDALT = 51,
	OC_AC_A_STDIDL = 52,
	OC_AC_A_STUN = 53,
	OC_AC_A_WALK = 54
};

BEGIN(Client)

class COrc_Archer final : public CMonster
{
public:
	struct ORC_ARCHER_DESC
	{
		CMonster::MONSTER_DESC pTarget;
	};

public:
	COrc_Archer();
	virtual ~COrc_Archer()  override;

public:
	HRESULT Initialize();
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<COrc_Archer> Create(_float3 _vCreatePos, _float3 _vCertainPos);
	void		SetCertainPos(_float3 _vCertainPos) { m_vCertainPos = _vCertainPos; };
	
protected:
	HRESULT AddRigidBody(string _strModelKey);		// RigidBody �߰�
	virtual void OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ��� ����
	virtual void OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;	// ��� ���� ��
	virtual void OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ������

	virtual void EnableCollision(const char* _strShapeTag) override;
	virtual void DisableCollision(const char* _strShapeTag) override;

	HRESULT		AddPartObjects(string _strModelKey);
	void		UpdateDistanceState();
	void		UpdateCoolTimeRange(_float _fTimeDelta);
	void		UpdateCoolTimeMelee(_float _fTimeDelta);	// �ٰŸ� ��
	void		ActiveNormalArrow();
	void		ActiveBuffArrow();

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
	_int		m_iBaseState = OC_AC_B_BEFOREMEET;			// Base����
	_int		m_iDistanceState = OC_AC_D_BEFOREMEET;		// �Ÿ��� ���� ����
	_int		m_iActionState = OC_AC_A_STDALT2;			// �ൿ����
	
	_float		m_fAttack1CoolTimeRange = 5.f;				// �������� ���ִ� �Ÿ�
	_float		m_fAttack1Range = 2.f;						// �������� Range
	_float		m_fAttack2Range = 100.f;					// ȭ�� Range
	_float		m_fOutRange = 10000.f;						// �������� ���� Range
	_float		m_fMeetRange = 25.f;						// ���� Range

	_bool		m_bDeadBack = true;				// �׾ ��� �и�

	// ȭ�� ���� Render����
	_float		m_fCalculateTime = 0.f;						// ���� Time
	_float		m_fCalculateTime2 = 0.f;						// ���� Time
	_float		m_fAttack2ArrowGenTime = 19.f * 0.05f;
	_float		m_fAttack2ArrowOutTime = 48.f * 0.05f;
	_float		m_fFAttack2ArrowGenTime = 19.f * 0.05f / 1.5f;
	_float		m_fFAttack2ArrowOutTime = 48.f * 0.05f / 1.5f;

	_float		m_fCombatStartMoveStartTime = 22.f * 0.05f;

	_bool		m_bPartArrowGen = false;
	_bool		m_bCombatStartMove = false;

	_bool		m_bJumpEnd = false;

	// ��Ÿ��
	_bool		m_bAttackReady[4] = { false, true, false, true };
	_float		m_fAttackCoolTime[4] = { 3.f, 4.f, 2.f, 3.f };
	_float		m_fBAttackCoolTime[4] = { 3.f, 4.f, 2.f, 3.f };

	_int		m_NormalAttackCount = 2;	// Normal ���� ���� ����
	_int		m_FuryAttackCount = 2;		// Fury ���� ���� ����


	// ������ ������ ���� �������� ������ ������
	// ����ȭ�Ǵ� hp -> ���߿��� ������ ���������� �ɸ��� �Ǵ°ɷ�
	_bool		m_bBuff = false;

	_float		m_fFuryHp = 50.f;

	// ����ȭ�� �������� �ӵ�
	_float		m_fFuryActionSpeed = 1.2f; 

	// ����ȭ�� �ӵ�
	_float		m_fFurySpeed = 2.5f;

	// ���۽� �̵��� Ư�� ��ġ
	_float3		m_vCertainPos = _float3(0.f, 0.f, 0.f);
	_float3		m_vJumpUpPos = _float3(0.f, 0.f, 0.f);
	_float3		m_vJumpStartPos = _float3(0.f, 0.f, 0.f);
	_float		m_vJumpHeight = 1.f;
	_float		m_fJumpUpTime = 0.3f;
	_float		m_fJumpDownTime = 0.f;

	// ȭ��
	vector<shared_ptr<CArrow>>		m_vecArrow;
	_int		m_iArrowCount = 10;
	
	// �ݶ��̴� ����
	const char* m_strATT1Collider1 = "M_Orc_Archer_SK.ao|M_Orc_Archer_USkdmgA";

	const char* m_strBUFFCollider = "M_Orc_Shaman_SK.ao|M_Orc_Shaman_USkdmgB";
};

END