#pragma once

#include "Monster.h"

// ����
// ��������(�Ϲݰ���, �������(�ѹ�)�̲�������, �������(�������̲�������)
// ȸ������(1,2, �ٰ�����, 1�� ������ ������ ���� ����, 2�� ������ ������ ũ�� ����), �����������(1,2, 1�� ��� ���, 2�� Į�� ���))
// ����1: �����������1 -> �ٴٰ�(�ָ�) -> �������1 -> ȸ������1 -> ���ϻ���
// ����2: �����������2 -> �ٴٰ�(�ָ�) -> �������1 -> �ٴٰ�(�ָ�) -> �Ϲݰ���
// ����3: ȸ������2 -> ���ϻ��� -> �ȴٰ� ��(�ָ�) -> �������2(������) -> �����������2 -> �ٴٰ�(�ָ�) -> �Ϲݰ���
// ����4: �����������2 -> �����������2 -> �����������2 -> �ٴٰ�(�ָ�) -> �������2(������)
// �и�����: �Ϲݰ���

// Skill ����
enum MI_B_SH_SkillState
{
	MI_B_SH_S_SKILL1 = 0,	// ����1
	MI_B_SH_S_SKILL2,		// ����2
	MI_B_SH_S_SKILL3,		// ����3
	MI_B_SH_S_SKILL4,		// ����4
	MI_B_SH_S_NOSKILL		// x
};

// Base ����
enum MI_B_SH_BaseState
{
	MI_B_SH_B_BEFOREMEET = 0,
	MI_B_SH_B_GENERATE,
	MI_B_SH_B_NORMAL,
	MI_B_SH_B_BLOCKED,
	MI_B_SH_B_HITTED,
	MI_B_SH_B_HITTED2,
	MI_B_SH_B_HITTED3,
	MI_B_SH_B_STUN,
	MI_B_SH_B_SHOCKED,
	MI_B_SH_B_DOWN,
	MI_B_SH_B_DEATH
};

// �Ÿ��� ���� ����
enum MI_B_SH_DistanceState
{
	MI_B_SH_D_MOVERANGE = 0,
	MI_B_SH_D_ATTACKRANGE,
	MI_B_SH_D_OUTRANGE,
	MI_B_SH_D_BEFOREMEET
};

// Ư�� �׼� ����
enum MI_B_SH_ActionState
{
	MI_B_SH_A_BLOCKED = 0,
	MI_B_SH_A_COMBATSTART = 5,
	MI_B_SH_A_DEAD = 6,
	MI_B_SH_A_DEATH = 7,
	MI_B_SH_A_DOWN3 = 10,
	MI_B_SH_A_DOWN2 = 11,
	MI_B_SH_A_DOWN1 = 12,
	MI_B_SH_A_FRONTBE = 16,
	MI_B_SH_A_RUN = 22,
	MI_B_SH_A_RUNFL = 23,
	MI_B_SH_A_RUNFR = 24,
	MI_B_SH_A_SEARCH = 25,
	MI_B_SH_A_SHOCKED = 26,
	MI_B_SH_A_ATTACK1_A = 29,		// �Ϲݰ���
	MI_B_SH_A_ATTACK1_B = 30,
	MI_B_SH_A_ATTACK1_C = 31,
	MI_B_SH_A_ATTACK2_A = 32,		// �������2
	MI_B_SH_A_ATTACK2_B = 33,
	MI_B_SH_A_ATTACK3_A = 35,		// ȸ������1
	MI_B_SH_A_ATTACK3_B = 36,
	MI_B_SH_A_ATTACK4_A = 37,		// ȸ������2
	MI_B_SH_A_ATTACK4_B = 38,
	MI_B_SH_A_ATTACK5_A = 39,		// �������1
	MI_B_SH_A_ATTACK5_B = 40,
	MI_B_SH_A_ATTACK5_C = 41,
	MI_B_SH_A_JUMPATTACK1_C = 42,
	MI_B_SH_A_JUMPATTACK1_B = 43,
	MI_B_SH_A_JUMPATTACK1_A = 44,	// ��������1
	MI_B_SH_A_JUMPATTACK2_C = 45,
	MI_B_SH_A_JUMPATTACK2_B = 46,
	MI_B_SH_A_JUMPATTACK2_A = 47,	// ��������2
	MI_B_SH_A_APPEAR = 48,			// �پ����
	MI_B_SH_A_DANCE1 = 49,		
	MI_B_SH_A_DANCE2 = 50,
	MI_B_SH_A_DANCE3 = 51,
	MI_B_SH_A_STDALT = 57,			// �����ڼ�
	MI_B_SH_A_STDALT1 = 58,
	MI_B_SH_A_STDALT2 = 59,
	MI_B_SH_A_STDIDLE = 60,
	MI_B_SH_A_STUNE = 61,
	MI_B_SH_A_STUNL = 62,
	MI_B_SH_A_STUNS = 63,
	MI_B_SH_A_WALK = 64
};


BEGIN(Client)

class CMidget_Berserker_Shadow final : public CMonster
{
public:
	struct LIVINGARMOR_SOLDIER_DESC
	{
		CMonster::MONSTER_DESC pTarget;
	};

public:
	CMidget_Berserker_Shadow();
	virtual ~CMidget_Berserker_Shadow()  override;

public:
	HRESULT Initialize();
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public: /* UI Get Function */
	_int GetCurParryingStack() { return m_iParryingStack; }
	_int GetMaxParryingStack() { return m_iBParryingStack; }
	_float GetHPRatio();
	_int GetBossActionState() { return m_iActionState; }

public:
	static shared_ptr<CMidget_Berserker_Shadow> Create(_float3 _vCreatePos);
	void	SetShadowActiveOn(_bool _bActiveOn) { m_bShadowActiveOn = _bActiveOn; }

protected:
	HRESULT AddRigidBody(string _strModelKey);		// RigidBody �߰�
	virtual void OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag);		// ��� ����
	virtual void OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag);	// ��� ���� ��
	virtual void OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag);		// ������

	virtual void EnableCollision(const char* _strShapeTag) override;
	virtual void DisableCollision(const char* _strShapeTag) override;

	void		ResetBoss();		// ���� ����

	HRESULT		AddPartObjects(string _strModelKey);
	void		UpdateDistanceState();

	// �ð� �������� Ư���ӵ��� ���¹������� �̲��������ϱ�
	void		UpdateMoveSlide(_float _fSlideStart, _float _fSlideEnd, _float _fSpeed, _float _fTimeDelta);
	void		UpdateAttack2MoveSlide(_float _fTimeDelta);
	_int		CheckParryingStack(_int _iParryingStack);

	// BaseState�� ���� FSM
	void		FSM_BeforeMeet(_float _fTimeDelta);
	void		FSM_Generate(_float _fTimeDelta);
	void		FSM_Normal(_float _fTimeDelta);
	void		FSM_Blocked(_float _fTimeDelta);
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
	_int		m_iBaseState = MI_B_SH_B_BEFOREMEET;			// Base����
	_int		m_iDistanceState = MI_B_SH_D_BEFOREMEET;		// �Ÿ��� ���� ����
	_int		m_iActionState = MI_B_SH_A_STDIDLE;			// �ൿ����
	_int		m_iSkillState = MI_B_SH_S_NOSKILL;				// ��ų����

	_float		m_fMeleeRange = 2.f;					// �������� Range
	_float		m_fAttack1Range = 3.f;					// ������� Range
	_float		m_fOutRange = 10000.f;					// �������� ���� Range
	_float		m_fMeetRange = 100.f;					// ���� Range

	_float		m_fCalculateTime = 0.f;						// ���� Time
	_float		m_fCalculateTime2 = 0.f;					// ���� Time2

	_bool		m_bSkill1State[10] = { 0, };
	_bool		m_bSkill2State[10] = { 0, };
	_bool		m_bSkill3State[10] = { 0, };
	_bool		m_bSkill4State[10] = { 0, };

	_bool		m_bShadowActiveOn = false;

	// ȸ������ ���� ������
	_bool		m_bWhileStun = false;

	// ���� Ȥ�� ������ ��ġ
	_float3		m_vMovePos = _float3(0.f, 0.f, 0.f);

	// ���� �ӵ��� ���� ����
	void		UpdateJumpDirSpeed1();	// Jump1 ����
	void		UpdateJumpDirSpeed2();	// Jump2 ����

	// ȸ������
	void		UpdateDir(_float _fTimeDelta);

	// ���� ����, �ӵ�
	_float3		m_vJumpNormalMoveDir = _float3(0.f, 0.f, 0.f);
	_float		m_fJumpMoveSpeed = 1.f;

	// �ִϸ��̼� �ӵ� ����
	_float		m_fAnimSpeed = 1.f;
	_float		m_fActionAnimSpeed[100] = { 0, };

	// Ű������ �ð� ����
	_float		m_fJump1UpTime = m_fKeyFrameSpeed * 5;
	_float		m_fSpin1EndTime = m_fKeyFrameSpeed * 93;
	_float		m_fAttack2Move1EndTime = m_fKeyFrameSpeed * 8;
	_float		m_fAttack2Move2StartTime = m_fKeyFrameSpeed * 24;
	_float		m_fAttack2Move2EndTime = m_fKeyFrameSpeed * 28;
	_float		m_fAttack2Move3StartTime = m_fKeyFrameSpeed * 36;
	_float		m_fAttack2Move3EndTime = m_fKeyFrameSpeed * 41;

	// �и� ����
	_int		m_iParryingStack = 0;
	_int		m_iBParryingStack = 3;

	_int		m_iAtt1ParryStack = 1;
	_int		m_iAtt5ParryStack = 2;
	_int		m_iJumpAtt2ParryStack = 3;
	_int		m_iParryType = 0;		// 0: �������1, 1: ����ġ��, 2: ���� ����ġ��

	// InSkill ����

	// ���ݸ��� �̲������� �ӵ�
	_float		m_fA2SlideSpeed = 5.f;
	_float		m_fA5SlideSpeed = 3.f;

	// Skill1
	_float		m_fSkill1StandTime = 1.f;
	_float		m_fSkill1RunSpeed = 5.f;
	_float		m_fSkill1SpinSpeed = 8.f;

	// Skill2
	_float		m_fSkill2StandTime = 1.f;
	_float		m_fSkill2RunSpeed = 5.f;

	// Skill3
	_float		m_fSkill3StandTime = 1.f;
	_float		m_fSkill3WalkSpeed = 1.f;
	_float		m_fSkill3WalkTime = 2.f;
	_float		m_fSkill3RunSpeed = 5.f;
	_float		m_fSkill3SpinSpeed = 4.f;

	// Skill4
	_float		m_fSkill4StandTime = 1.f;
	_float		m_fSkill4RunSpeed = 5.f;

	// ��ų���� ����
	_int		m_iSkillOrder[100] = { 0,3,2,1,3,1,2,0,0,3,2,1,3,1,2,0,0,3,2,1,3,1,2,0,0,3,2,1,3,1,2,0 };
	//_int		m_iSkillOrder[100] = { 1,1,1,1,1,1,1,1,1 };
	_int		m_iPatternNum = 0;

	// �ݶ��̴� ����
	// ���� �������1
	const char* m_strATT5Collider = "M_L1Midget_Berserker_SK.ao|M_L1Midget_Berserker_SkdmgD.001";
	// ���� �� ����ġ��
	const char* m_strATT1Collider = "M_L1Midget_Berserker_SK.ao|M_L1Midget_Berserker_SkdmgA.001";
	// ���� Į ����ġ��
	const char* m_strJumpATT2Collider = "M_L1Midget_Berserker_SK.ao|M_L1Midget_Berserker_SkmovBE";

	_bool		m_bBug = false;
};

END