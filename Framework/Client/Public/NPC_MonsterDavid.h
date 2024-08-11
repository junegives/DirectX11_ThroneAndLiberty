#pragma once

#include "Monster.h"

// ����
// ��������(�Ʒ�����1(ª��), �Ʒ�����2(����), ȸ������(���󰡱�(��), �뽬����1(���), �뽬����2(����),
// ���ΰ���1(ª��), ���ΰ���2(���󰡱�(��))
// ���� ���ѹݺ�: �뽬����1, �Ʒ�����2, ȸ������, �뽬����2, �뽬����1, ���ΰ���1, �Ʒ�����1, ���ΰ���2
// �и�����: �Ʒ�����2, ���ΰ���2, �뽬����1

// Dialog Order ����
enum NPC_DavidMonster_DialogOrderState
{
	DAVIDMONSTER_DI_PRACTICEMEET2,			// ���ΰ����� �±� FSM
	DAVIDMONSTER_DI_PRACTICEMEET3,			// ���ΰ� ������ FSM
	DAVIDMONSTER_DI_PRACTICEMEET4,			// ���ΰ��� ��� FSM
};

// Base ����
enum NPC_DavidMonster_BaseState
{
	DAVIDMONSTER_B_BATTLENORMAL,
	DAVIDMONSTER_B_HITTED,
	DAVIDMONSTER_B_HITTED2,
	DAVIDMONSTER_B_HITTED3,
	DAVIDMONSTER_B_STUN,
	DAVIDMONSTER_B_SHOCKED,
	DAVIDMONSTER_B_DOWN,
	DAVIDMONSTER_B_NONE
};

// �Ÿ��� ���� ����
enum NPC_DavidMonster_DistanceState
{
	DAVIDMONSTER_D_ATTACKRANGE,					// �Ϲݰ��� �Ÿ�
	DAVIDMONSTER_D_MOVERANGE,					// ������ �Ÿ�
	DAVIDMONSTER_D_OUTRANGE
};

// Ư�� �׼� ����
enum NPC_DavidMonster_ActionState
{
	DAVIDMONSTER_A_BLADEDANCE1 = 0,			// ȸ�� ����1
	DAVIDMONSTER_A_BLADEDANCE2 = 1,			// ȸ�� ����2
	DAVIDMONSTER_A_BLADEDANCE3 = 2,			// ȸ�� ����3
	DAVIDMONSTER_A_BLOCKED = 3,
	DAVIDMONSTER_A_SWORDIN = 8,				// �� ����ֱ�
	DAVIDMONSTER_A_SWORDOUT = 9,			// �� ������
	DAVIDMONSTER_A_TALK1 = 10,
	DAVIDMONSTER_A_TALK2 = 11,
	DAVIDMONSTER_A_TALK3 = 12,
	DAVIDMONSTER_A_TALK4 = 13,
	DAVIDMONSTER_A_DASH1 = 14,				// �뽬 ����1(�ձ���)
	DAVIDMONSTER_A_DASH2 = 15,				// �뽬 ����2(����)
	DAVIDMONSTER_A_IDLE = 18,
	DAVIDMONSTER_A_DOWN3 = 19,
	DAVIDMONSTER_A_DOWN2 = 20,
	DAVIDMONSTER_A_DOWN1 = 21,
	DAVIDMONSTER_A_FRONTBE = 29,
	DAVIDMONSTER_A_RUN = 31,				// �޸���
	DAVIDMONSTER_A_SHOCKED = 32,			// ����
	DAVIDMONSTER_A_STDALT = 33,				// �����¼�
	DAVIDMONSTER_A_STUN = 34,
	DAVIDMONSTER_A_ATT1 = 35,				// ����1(�Ʒ������)
	DAVIDMONSTER_A_ATT1_2 = 36,				// ����1(�Ʒ������)
	DAVIDMONSTER_A_ATT2 = 37,				// ����2(���οø���)
	DAVIDMONSTER_A_ATT2_2 = 38,				// ����2(���οø���)
	DAVIDMONSTER_A_ALTRUN = 39,				// �������� �޸���
	DAVIDMONSTER_A_ALTWALK = 40,			// �������� �ȱ�
	DAVIDMONSTER_A_WALK = 45,
};

BEGIN(Client)

class CNPC_MonsterDavid final : public CMonster
{
public:
	CNPC_MonsterDavid();
	virtual ~CNPC_MonsterDavid() = default;

public:
	HRESULT Initialize();
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;
	virtual HRESULT RenderShadow()  override;

public:
	static shared_ptr<CNPC_MonsterDavid> Create(_float3 _vCreatePos);
	void		SetDialogState(_int _iNum);
	_bool		GetSwitch() { return m_bSwitch; }
	void		SetSwitch(_bool	_bSwitch) { m_bSwitch = _bSwitch; }
	void		GoSwitchPosition();
	void		SetNPCPos(_float3 _vCreatePos) { m_vNPCPos = _vCreatePos; }
	_bool		GetDoingEnd() { return m_bDoingEnd; }

protected:
	HRESULT			AddRigidBody(string _strModelKey);		// RigidBody �߰�
	virtual void	OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ��� ����
	virtual void	OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;	// ��� ���� ��
	virtual void	OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ������

	virtual void EnableCollision(const char* _strShapeTag) override;
	virtual void DisableCollision(const char* _strShapeTag) override;

	HRESULT		AddPartObjects(string _strModelKey);
	void		UpdateDistanceState();
	void		RenderBackWeapon();
	void		RenderHandWeapon();
	void		ResetTimer();

	void		FSM_PracticeMeet2(float _fTimeDelta);	// �±�
	void		FSM_PracticeMeet3(float _fTimeDelta);	// ������
	void		FSM_PracticeMeet4(float _fTimeDelta);	// ���

	// �����̻� FSM
	void		FSM_Hitted(_float _fTimeDelta);
	void		FSM_Hitted2(_float _fTimeDelta);
	void		FSM_Hitted3(_float _fTimeDelta);
	void		FSM_Stun(_float _fTimeDelta);
	void		FSM_Shocked(_float _fTimeDelta);
	void		FSM_Down(_float _fTimeDelta);

	void		UpdateDir(_float _fTimeDelta);

	_bool		CheckLoose();
	void		CheckAttacked();

	_int		NextAttack();

	// Ư�� ���� �ִϸ��̼� �ڵ����� ������
	// ȸ������ 
	_bool		UpdateSpinState(_float _fTimeDelta);
	// �ձ��� �뽬
	_bool		UpdateFrontDashState(_float _fTimeDelta, _float _fFrontDistance = 1.5f);
	// �մ� �뽬
	_bool		UpdateThroughDashState(float _fTimeDelta, _float _fDistance = 8.f);
	// �Ʒ�����1
	_bool		UpdateBottomAttack1(_float _fTimeDelta);
	// �Ʒ�����2
	_bool		UpdateBottomAttack2(_float _fTimeDelta);
	// ���ΰ���1
	_bool		UpdateUpAttack1(_float _fTimeDelta);
	// ���ΰ���2
	_bool		UpdateUpAttack2(_float _fTimeDelta);
	
	// �Ÿ�
	// ��ȭ ��ȣ�ۿ��� ���� �Ÿ�
	_float		m_fMeetRange = 10.f;
	_float		m_fCloseRange = 3.f;

	// ������ ���� �Ÿ�
	_float		m_fAttack1Range = 2.f;		// �������� Range
	_float		m_fOutRange = 10000.f;		// �������� ���� Range

private:
	_float		m_fBacKWalkRange = 5.f;

	_float3		m_vNPCPos = _float3(0.f, 0.f, 0.f);	// ��ü�� ������ġ

	_float		m_fWalkRemainDistance = 0.05f;

	// �ִϸ��̼� �ӵ�����
	_float		m_fActionAnimSpeed[100] = { 0, };
	// �ִϸ��̼� ��� �ӵ�
	_float		m_fAnimSpeed = 1.f;

	_float		m_fCalculateTime1 = 0.f;				// ���� Time1
	_float		m_fCalculateTime2 = 0.f;				// ���� Time2
	_float		m_fCalculateTime3 = 0.f;				// ���� Time3
	
	_int		m_iDistanceState = DAVIDMONSTER_D_MOVERANGE;		// �Ÿ��� ���� ����
	_int		m_iActionState = DAVIDMONSTER_A_IDLE;				// �ൿ����
	_int		m_iBaseState = DAVIDMONSTER_B_NONE;					// Base ����
	_int		m_iDialogOrder = DAVIDMONSTER_DI_PRACTICEMEET2;		// �� ������ ���� ���°� ����
	
	// �ִϸ��̼� �ð�
	_float		m_fWeaponOuTime = m_fKeyFrameSpeed * 18.f;	// ���� ������ �ð�
	_float		m_fWeaponInTime = m_fKeyFrameSpeed * 22.f;	// ���� ����ִ� �ð�

	// �뽬����
	_float		m_fDashStartTime = m_fKeyFrameSpeed * 5.f;	// �뽬 ���� �ð�
	_float		m_fDashEndTime = m_fKeyFrameSpeed * 12.f;

	_bool		m_bHandWeaponRender = false;
	_bool		m_bBackWeaponRender = true;

	// ��ȭ����2������ �Ϲݰ��� ����
	_float		m_fD2AttackIntervel = 3.f;				// �Ϲݰ��� ����
	_float		m_fD2LimitTime = 60.f;					// ���ѽð�

	// ��ȭ����3������ ���ݵ� ����
	_float		m_fD3AttackIntervel = 3.f;				// ���ݵ� ����
	//_float		m_fD3AttackIntervel = 9.f;			// ���ݵ� ����
	_int		m_iD3AttackNum = 0;						// ���� ����
	_int		m_iMaxD3AttackNum = 6;
	_int		m_iD3AttackSkill = 0;

	// ��ȭ����4������ ���
	_float		m_fStandTime = 0.5f;		// ��� ���̸��� ���ִ� �ð�

	_bool		m_bNotHitted = false;		// ������ ������ ����

	_bool		m_bDialog1State[20] = { 0, };		// ���ΰ����� �±�
	_bool		m_bDialog2State[20] = { 0, };		// ���ΰ� ������
	_bool		m_bDialog3State[20] = { 0, };		// ���ΰ��� ���
	_bool		m_bDialog3CanHitted = false;		// ��ȭ����4���� ������ �ִ� Ÿ�̹�

	_bool		m_bSwitch = false;		// true�� NPC������ �ڸ��ٲٱ�
	_bool		m_bDoingEnd = false;	// ������ ��� ������ true

	// �뽬����
	_float		m_fDashDistanceXZ = 0.f;				// �뽬 ����Ÿ�
	_float		m_fDashTime = 0.f;						// �뽬 �ð�
	_float		m_fDashSpeed = 0.f;						// �뽬 �ӵ�
	_float3		m_vDashDir = _float3(0.f, 0.f, 0.f);	// �뽬 ����

	_bool		m_bParriedBefore = false;			// �뽬 ���¿��� �и��޾Ҵ���

	// �и������� �ݶ��̴� ����
	const char* m_strSpinAttCollider1 = "CT_Named_ArthurTalon_SK.ao|CT_Named_ArthurTalon_BladeDance";
	const char* m_strSpinAttCollider2 = "CT_Named_ArthurTalon_SK.ao|CT_Named_ArthurTalon_BladeDance.001";
	const char* m_strSpinAttCollider3 = "CT_Named_ArthurTalon_SK.ao|CT_Named_ArthurTalon_BladeDance.002";
	const char* m_strDashAttCollider1 = "CT_Named_ArthurTalon_SK.ao|CT_Named_ArthurTalon_Dash";
	const char* m_strDashAttCollider2 = "CT_Named_ArthurTalon_SK.ao|CT_Named_ArthurTalon_Dash.001";
	const char* m_stDownAttCollider1 = "CT_Named_ArthurTalon_SK.ao|CT_Named_ArthurTalon_SwingA";
	const char* m_stDownAttCollider2 = "CT_Named_ArthurTalon_SK.ao|CT_Named_ArthurTalon_SwingA.001";
	const char* m_stUpAttCollider1 = "CT_Named_ArthurTalon_SK.ao|CT_Named_ArthurTalon_SwingB";
	const char* m_stUpAttCollider2 = "CT_Named_ArthurTalon_SK.ao|CT_Named_ArthurTalon_SwingB.001";


	/*Hair Color*/
private:
	_float3 m_vHairMainColor = { 0.02f, 0.02f, 0.07f };
	_float3 m_vHairSubColor = { 0.4f, 0.4f, 0.4f };
};

END