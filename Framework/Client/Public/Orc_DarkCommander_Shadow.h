#pragma once

#include "Monster.h"
// ����
// ��������(����������, ��������, �������ߴ�����, 3����, 2����, ���Ƽ�����)
// Ư��(�޷��� ��ġ��, Ư������, ��ȿ(������), ���鰭Ÿ(�������ǰ���))
// ����1: �޷��� ��ġ��, �޸���, ���Ƽ� ����(�и�), 2����, ��ȿ
// ����2: �޷��� ��ġ��, �޸���, Ư������(�и�), ���ִٰ� ���鰭Ÿ
// ����3: �޷��� ��ġ��, ���ִٰ� ���鰭Ÿ, �޸���, 3����(�и�), ��ȿ
// ����4: �޷��� ��ġ��, �޸���, �������� ����, Ư������(�и�), �޸���, 2����
// �и�����: ���Ƽ�����, 3������ ������, Ư������ ������
// Ư�� �ý���: �뽬�� ���������� �ε�ġ�� ����

// Skill ����
enum OC_DC_SH_SkillState
{
	OC_DC_SH_S_SKILL1 = 0,	// ����1
	OC_DC_SH_S_SKILL2,		// ����2
	OC_DC_SH_S_SKILL3,		// ����3
	OC_DC_SH_S_SKILL4,		// ����4
	OC_DC_SH_S_NOSKILL		// x
};

// Base ����
enum OC_DC_SH_BaseState
{
	OC_DC_SH_B_BEFOREMEET = 0,
	OC_DC_SH_B_GENERATE,
	OC_DC_SH_B_NORMAL,
	OC_DC_SH_B_BLOCKED,
	OC_DC_SH_B_HITTED,
	OC_DC_SH_B_HITTED2,
	OC_DC_SH_B_HITTED3,
	OC_DC_SH_B_STUN,
	OC_DC_SH_B_SHOCKED,
	OC_DC_SH_B_DOWN,
	OC_DC_SH_B_DEATH
};

// �Ÿ��� ���� ����
enum OC_DC_SH_DistanceState
{
	OC_DC_SH_D_MOVERANGE = 0,
	OC_DC_SH_D_ATTACKRANGE,
	OC_DC_SH_D_OUTRANGE,
	OC_DC_SH_D_BEFOREMEET
};

// Ư�� �׼� ����
enum OC_DC_SH_ActionState
{
	OC_DC_SH_A_DEAD = 4,
	OC_DC_SH_A_DEATH = 6,
	OC_DC_SH_A_DOWN3 = 9,
	OC_DC_SH_A_DOWN2 = 10,
	OC_DC_SH_A_DOWN1 = 11,
	OC_DC_SH_A_FRONTBE = 18,
	OC_DC_SH_A_BLOCKED = 21,
	OC_DC_SH_A_RUN = 23,
	OC_DC_SH_A_SHOCKED = 25,
	OC_DC_SH_A_FURY = 26,
	OC_DC_SH_A_SPECIAL = 27,
	OC_DC_SH_A_ATTACK1_A = 28,		// ����������
	OC_DC_SH_A_ATTACK1_A2 = 29,	// ����������
	OC_DC_SH_A_ATTACK1_B = 30,		// ��������
	OC_DC_SH_A_ATTACK1_B2 = 31,	// ��������
	OC_DC_SH_A_ATTACK1_C = 32,		// �������ߴ�����
	OC_DC_SH_A_ATTACK1_C2 = 33,	// �������ߴ�����
	OC_DC_SH_A_ATTACK2_A1 = 34,	// 3���� ����
	OC_DC_SH_A_ATTACK2_A2 = 35,
	OC_DC_SH_A_ATTACK2_A3 = 36,
	OC_DC_SH_A_ATTACK2_A4 = 37,
	OC_DC_SH_A_ATTACK2_A5 = 38,
	OC_DC_SH_A_ATTACK2_A6 = 39,
	OC_DC_SH_A_ATTACK2_B1 = 40,	// 2���� ����
	OC_DC_SH_A_ATTACK2_B2 = 41,
	OC_DC_SH_A_ATTACK2_B3 = 42,
	OC_DC_SH_A_ATTACK2_B4 = 43,
	OC_DC_SH_A_ATTACK2_C1 = 44,	// ���� ����
	OC_DC_SH_A_ATTACK2_C2 = 45,
	OC_DC_SH_A_ATTACK3_A1 = 46,	// Ư�� ����
	OC_DC_SH_A_ATTACK3_A2 = 47,
	OC_DC_SH_A_ATTACK3_A3 = 48,
	OC_DC_SH_A_ATTACK3_A4 = 49,
	OC_DC_SH_A_ATTACK3_A5 = 50,
	OC_DC_SH_A_ATTACK3_A6 = 51,
	OC_DC_SH_A_ATTACK3_A7 = 52,
	OC_DC_SH_A_ATTACK4 = 53,		// �������ǰ�Ÿ����
	OC_DC_SH_A_DASH3 = 55,
	OC_DC_SH_A_DASH2 = 56,
	OC_DC_SH_A_DASH1 = 57,
	OC_DC_SH_A_SKTRFA1 = 58,
	OC_DC_SH_A_SKTRFA2 = 59,
	OC_DC_SH_A_APPEAR = 60,
	OC_DC_SH_A_STDALT = 62,
	OC_DC_SH_A_STDIDLE = 63,
	OC_DC_SH_A_STUN = 64,
	OC_DC_SH_A_WALK = 65,
};


BEGIN(Client)

class COrc_DarkCommander_Shadow final : public CMonster
{
public:
	struct DARKCOMMANDER_DESC
	{
		CMonster::MONSTER_DESC pTarget;
	};

public:
	COrc_DarkCommander_Shadow();
	virtual ~COrc_DarkCommander_Shadow()  override;

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
	static shared_ptr<COrc_DarkCommander_Shadow> Create(_float3 _vCreatePos);
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

	_bool		Update2ComboState(_float _fTimeDelta);		// 2���� ������Ʈ
	_bool		UpdateSAttackState(_float _fTimeDelta);		// ������� ������Ʈ
	_bool		UpdateRAttackState(_float _fTimeDelta);		// �����ʰ��� ������Ʈ
	_bool		UpdateRMAttackState(_float _fTimeDelta);	// �������ߴܰ��� ������Ʈ
	_bool		UpdateLAttackState(_float _fTimeDelta);		// �����ʰ��� ������Ʈ
	_bool		Update3ComboState(_float _fTimeDelta);		// 3���� ������Ʈ
	_bool		UpdateSpecialState(_float _fTimeDelta);		// Ư������ ������Ʈ

	void		ResetTime();
	void		CheckDead();
	void		CheckAttacked();
	_bool		CheckAnimFinish();
	
private:
	_int		m_iBaseState = OC_DC_SH_B_BEFOREMEET;			// Base����
	_int		m_iDistanceState = OC_DC_SH_D_BEFOREMEET;		// �Ÿ��� ���� ����
	_int		m_iActionState = OC_DC_SH_A_STDIDLE;			// �ൿ����
	_int		m_iSkillState = OC_DC_SH_S_NOSKILL;			// ��ų����

	_float		m_fMeleeRange = 2.f;					// �������� Range
	_float		m_fAttack1Range = 3.f;					// �������� Range
	_float		m_fDashEndRange = 5.f;					// �뽬�ؼ� �ε�ġ�� �Ÿ�
	_float		m_fOutRange = 10000.f;					// �������� ���� Range
	_float		m_fMeetRange = 100.f;					// ���� Range
	
	_float		m_fCalculateTime = 0.f;						// ���� Time
	_float		m_fCalculateTime2 = 0.f;					// ���� Time2

	_bool		m_bSkill1State[10] = { 0, };
	_bool		m_bSkill2State[10] = { 0, };
	_bool		m_bSkill3State[10] = { 0, };
	_bool		m_bSkill4State[10] = { 0, };

	_bool		m_bShadowActiveOn = false;

	// ���� Ȥ�� ������ ��ġ
	_float3		m_vMovePos = _float3(0.f, 0.f, 0.f);

	// ȸ������
	void		UpdateDir(_float _fTimeDelta);

	// ���� ����, �ӵ�
	_float3		m_vJumpNormalMoveDir = _float3(0.f, 0.f, 0.f);
	_float		m_fJumpMoveSpeed = 1.f;

	// �ִϸ��̼� �ӵ� ����
	_float		m_fAnimSpeed = 1.f;
	_float		m_fActionAnimSpeed[100] = { 0, };
	_float		m_fAttack4AnimSpeed = 0.7f;
	_float		m_fBeforeQuakeTime = 2.5f;

	// Ű������ �ð� ����
	
	// �и� ����
	_int		m_iParryingStack = 0;
	_int		m_iBParryingStack = 3;

	_int		m_iSAttParryStack = 1;
	_int		m_i3ComboAttParryStack = 1;
	_int		m_iSpecialAttParryStack = 2;
	_int		m_iParryType = 0;		// 0: ���Ƽ� ����, 1: 3������ ������, 2: Ư������ ������

	// InSkill ����

	// ���ݸ��� �̲������� �ӵ�
	_float		m_fDash3SlideSpeed = 50.f;
	_float		m_f2Combo1SlideSpeed = 10.f;
	_float		m_f2Combo2SlideSpeed = 10.f;
	_float		m_f3Combo1SlideSpeed = 10.f;
	_float		m_f3Combo2SlideSpeed = 10.f;
	_float		m_f3Combo3SlideSpeed = 10.f;
	_float		m_fSAttackSlideSpeed = 10.f;
	_float		m_fRAttackSlideSpeed = 10.f;
	_float		m_fLAttackSlideSpeed = 10.f;
	_float		m_fSCombo1SlideSpeed = 10.f;
	_float		m_fSCombo2SlideSpeed = 2.f;
	_float		m_fSCombo3SlideSpeed = 25.f;
	_float		m_fSCombo4SlideSpeed = 20.f;

	// ����
	_float		m_fRunSpeed = 8.f;

	// Skill1
	_float		m_fSkill1StandTime = 1.f;
	_float		m_fSkill1DashSpeed = 20.f;
	_float		m_fSkill1TimeBeforeFury = 1.f;

	// Skill2
	_float		m_fSkill2StandTime = 1.f;
	_float		m_fSkill2DashSpeed = 20.f;
	_float		m_fSkill2MovePos = 3.f;
	
	// Skill3
	_float		m_fSkill3StandTime = 1.f;
	_float		m_fSkill3DashSpeed = 20.f;
	_float		m_fSkill3TimeBeforeFury = 1.5f;

	// Skill4
	_float		m_fSkill4StandTime = 1.f;
	_float		m_fSkill4DashSpeed = 20.f;

	// ��ų���� ����
	_int		m_iSkillOrder[100] = { 3,0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3 };
	//_int		m_iSkillOrder[100] = { 1,2,1,2,1,2,1,2,1,2 };
	_int		m_iPatternNum = 0;

	// �ݶ��̴� ����
	// 3������ ������
	const char* m_strATT1Collider = "M_Orc_LowDarkCommander_SK.ao|M_Orc_LowDarkCommander_SkdmgB.005";
	// ���Ƽ� ����
	const char* m_strATT2Collider = "M_Orc_LowDarkCommander_SK.ao|M_Orc_LowDarkCommander_SkdmgB_3.001";
	// Ư������ ��Ÿ
	const char* m_strATT3Collider = "M_Orc_LowDarkCommander_SK.ao|M_Orc_LowDarkCommander_SkdmgD.006";
	
	_bool		m_bBug = false;
};

END