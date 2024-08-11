#pragma once

#include "Monster.h"
#include "Stone.h"
// ����(��������, ��������), �� �и�����, �������� �и�����
// �⺻ �ٴڿ��� �ö����

// CoolTime
enum MI_S_CoolTime
{
	MI_S_C_NORMALATTACK1 = 0,
	MI_S_C_NORMALATTACK2
};

// Base ����
enum MI_S_BaseState
{
	MI_S_B_BEFOREMEET = 0,
	MI_S_B_ALERT,
	MI_S_B_NORMAL,
	MI_S_B_HITTED,
	MI_S_B_HITTED2,
	MI_S_B_HITTED3,
	MI_S_B_STUN,
	MI_S_B_SHOCKED,
	MI_S_B_DOWN,
	MI_S_B_DEATH
};

// �Ÿ��� ���� ����
enum MI_S_DistanceState
{
	MI_S_D_MOVERANGE = 0,
	MI_S_D_ATTACK1RANGE,
	MI_S_D_ATTACK2RANGE,
	MI_S_D_OUTRANGE,
	MI_S_D_BEFOREMEET
};

// Ư�� �׼� ����
enum MI_S_ActionState
{
	MI_S_A_BLOCKED = 0,
	MI_S_A_DOWN3 = 3,
	MI_S_A_DOWN2 = 4,
	MI_S_A_DOWN1 = 5,
	MI_S_A_RUNFL = 7,
	MI_S_A_RUNFR = 8,
	MI_S_A_SEARCH = 9,
	MI_S_A_DANCE1 = 13,
	MI_S_A_DANCE2 = 14,
	MI_S_A_DANCE3 = 15,
	MI_S_A_DOWNJUMP = 16,
	MI_S_A_UPJUMP = 20,
	MI_S_A_THREAT = 29,
	MI_S_A_STDALT = 30,
	MI_S_A_DEAD = 38,
	MI_S_A_DEATH = 39,
	MI_S_A_FRONTBE = 46,
	MI_S_A_FRONTBL = 47,
	MI_S_A_FRONTBS = 48,
	MI_S_A_RUN = 49,
	MI_S_A_SHOCKED = 50,
	MI_S_A_BUFF = 51,
	MI_S_A_ATTACK1 = 52,
	MI_S_A_ATTACK2 = 53,
	MI_S_A_STDIDL = 54,
	MI_S_A_STUN = 55,
	MI_S_A_WALK = 56
};

BEGIN(Client)

class CMidget_Sling final : public CMonster
{
public:
	struct MIDGET_SLING_DESC
	{
		CMonster::MONSTER_DESC pTarget;
	};

public:
	CMidget_Sling();
	virtual ~CMidget_Sling()  override;

public:
	HRESULT Initialize();
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<CMidget_Sling> Create(_float3 _vCreatePos, _int _iGenType);
	void		SetGenType(_int _iGenType) { m_iGenType = _iGenType; }

protected:
	HRESULT		AddRigidBody(string _strModelKey);		// RigidBody �߰�
	virtual void OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ��� ����
	virtual void OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;	// ��� ���� ��
	virtual void OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ������

	virtual void EnableCollision(const char* _strShapeTag) override;
	virtual void DisableCollision(const char* _strShapeTag) override;

	HRESULT		AddPartObjects(string _strModelKey);
	void		UpdateDistanceState();
	void		UpdateCoolTimeRange(_float _fTimeDelta);
	void		UpdateCoolTimeMelee(_float _fTimeDelta);
	void		ActiveNormalStone();

	// BaseState ���� FSM
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
	_int		m_iBaseState = MI_S_B_BEFOREMEET;		// Base����
	_int		m_iDistanceState = MI_S_D_BEFOREMEET;	// �Ÿ��� ���� ����
	_int		m_iActionState = MI_S_A_DANCE3;			// �ൿ����

	_float		m_fAttack1CoolTimeRange = 4.f;			// �������� ���ִ� �Ÿ�
	_float		m_fAttack1Range = 1.7f;					// ���� Range
	_float		m_fAttack2Range = 30.f;					// �� ������ Range
	_float		m_fOutRange = 10000.f;					// �������� ���� Range
	_float		m_fMeetRange = 35.f;					// ���� Range
	_float		m_fFightRange = 15.f;					// �ο� Range

	// �� ���� Render����
	_float		m_fCalculateTime = 0.f;					// ���� Time
	_float		m_fAttack2StoneGenTime = 1.f * 0.05f;
	_float		m_fAttack2StoneOutTime = 32.f * 0.05f;

	_bool		m_bPartStoneGen = false;

	// ��Ÿ��
	_bool		m_bAttackReady[2] = { false, true };
	_float		m_fAttackCoolTime[2] = { 1.8f, 3.f };
	_float		m_fBAttackCoolTime[2] = { 1.8f, 3.f };

	// �ݶ��̴� ����
	const char* m_strATT1Collider1 = "M_L1Midget_Sling_SK.ao|M_L1Midget_Sling_USkdmgA";

	_int		m_NormalAttackCount = 2;	// Normal ���� ���� ����

	// ȭ��
	vector<shared_ptr<CStone>>		m_vecStone;
	_int		m_iStoneCount = 5;
	_int		m_iGenType = 0;	// 0: �Ʒ����� �ö����, 1: �������� ��������

	/*Hair Color*/
private:
	_float3 m_vHairMainColor = { 1.f, 1.f, 1.f };
	_float3 m_vHairSubColor = { 0.4f, 0.4f, 0.4f };
};

END