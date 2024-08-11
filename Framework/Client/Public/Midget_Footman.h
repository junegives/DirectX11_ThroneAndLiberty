#pragma once

#include "Monster.h"
#include "Axe.h"
// ����(����1, ����2(���� ������)), �ֱ������� ���� �İ� �� ���� ������ ��ġ�� ����
// ���� �и�����, ���� �и�����

// CoolTime
enum MI_F_CoolTime
{
	MI_F_C_NORMALATTACK1 = 0,
	MI_F_C_NORMALATTACK2,
	MI_F_C_NORMALATTACK3			// �ٴ� ����
};

// Base ����
enum MI_F_BaseState
{
	MI_F_B_BEFOREMEET = 0,
	MI_F_B_ALERT,
	MI_F_B_NORMAL,
	MI_F_B_HITTED,
	MI_F_B_HITTED2,
	MI_F_B_HITTED3,
	MI_F_B_STUN,
	MI_F_B_SHOCKED,
	MI_F_B_DOWN,
	MI_F_B_DEATH
};

// �Ÿ��� ���� ����
enum MI_F_DistanceState
{
	MI_F_D_MOVERANGE = 0,
	MI_F_D_ATTACK1RANGE,
	MI_F_D_ATTACK2RANGE,
	MI_F_D_OUTRANGE,
	MI_F_D_BEFOREMEET
};

// Ư�� �׼� ����
enum MI_F_ActionState
{
	MI_F_A_BLOCKED = 0,
	MI_F_A_DOWN3 = 3,
	MI_F_A_DOWN2 = 4,
	MI_F_A_DOWN1 = 5,
	MI_F_A_RUNFL = 7,
	MI_F_A_RUNFR = 8,
	MI_F_A_SEARCH = 9,
	MI_F_A_ATTACK1 = 10,	// �ٰ�
	MI_F_A_ATTACK2 = 11,	// ����
	MI_F_A_DANCE1 = 19,
	MI_F_A_DANCE2 = 20,
	MI_F_A_DANCE3 = 21,
	MI_F_A_DISAPPEAR = 22,	// ��������
	MI_F_A_APPEAR = 27,
	MI_F_A_THREAT = 31,
	MI_F_A_STDALT = 32,
	MI_F_A_DEAD = 40,
	MI_F_A_DEATH = 41,
	MI_F_A_FRONTBE = 48,
	MI_F_A_FRONTBL = 49,
	MI_F_A_FRONTBS = 50,
	MI_F_A_RUN = 51,
	MI_F_A_SHOCKED = 52,
	MI_F_A_STDIDL = 53,
	MI_F_A_STUN = 54,
	MI_F_A_WALK = 55
};


BEGIN(Client)

class CMidget_Footman final : public CMonster
{
public:
	struct MIDGET_FOOTMAN_DESC
	{
		CMonster::MONSTER_DESC pTarget;
	};

public:
	CMidget_Footman();
	virtual ~CMidget_Footman()  override;

public:
	HRESULT			Initialize();
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<CMidget_Footman> Create(_float3 _vCreatePos);

protected:
	HRESULT		AddRigidBody(string _strModelKey);		// RigidBody �߰�
	virtual void OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ��� ����
	virtual void OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;	// ��� ���� ��
	virtual void OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ������

	virtual void EnableCollision(const char* _strShapeTag) override;
	virtual void DisableCollision(const char* _strShapeTag) override;

	HRESULT		AddPartObjects(string _strModelKey);
	void		UpdateDistanceState();
	void		UpdateCoolTime(_float _fTimeDelta);
	void		ActiveNormalAxe();

	// BaseState�� ���� FSM
	void		FSM_BeforeMeet(_float _fTimeDelta);
	void		FSM_Alert(_float _fTimeDelta);
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
	_int		m_iBaseState = MI_F_B_BEFOREMEET;		// Base����
	_int		m_iDistanceState = MI_F_D_BEFOREMEET;	// �Ÿ��� ���� ����
	_int		m_iActionState = MI_F_A_STDIDL;		// �ൿ����

	_float		m_fAttack1Range = 2.f;		// �������� Range
	_float		m_fAttack2Range = 10.f;		// ����������, ���ı� Range
	_float		m_fOutRange = 10000.f;		// �������� ���� Range
	_float		m_fMeetRange = 20.f;		// ���� Range
	_float		m_fFightRange = 9.f;		// �ο� Range

	_float		m_fCalculateTime = 0.f;						// ���� Time
	_float		m_fThrowAxeTime = 40.f * 0.05f;
	_bool		m_bAxeGen = false;

	_float		m_fDigTime = 0.f;		// �ٴڿ� �ִ� �ð�

	// ��Ÿ��
	_bool		m_bAttackReady[4] = { true, false, false};
	_float		m_fAttackCoolTime[4] = { 2.f, 2.8f, 4.f};
	_float		m_fBAttackCoolTime[4] = { 2.f, 2.8f, 4.f };

	_int		m_NormalAttackCount = 3;	// Normal ���� ���� ����

	// �ݶ��̴� ����
	const char*		m_strATT1Collider1 = "M_L1Midget_Footman_SK.ao|M_L1Midget_Footman_SkdmgA";

	// ����
	vector<shared_ptr<CAxe>>		m_vecAxe;
	_int		m_iAxeCount = 5;

	/*Beard Color*/
private:
	_float3 m_vHairMainColor = { 1.f, 1.f, 1.f };
	_float3 m_vHairSubColor = { 0.4f, 0.4f, 0.4f };
};

END