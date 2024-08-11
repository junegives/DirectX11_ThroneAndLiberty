#pragma once

#include "NPC.h"
#include "NPC_MonsterDavid.h"

// Dialog Order ����
enum NPC_David_DialogOrderState
{
	DAVID_DI_FIRSTMEET = 0,			// ó�� ��������
	DAVID_DI_MOVETOTRAINING,	// Ʈ���̴� ��ҷ� �̵�
	DAVID_DI_PRACTICEMEET1,			// �Ʒ��忡�� ����, �÷��̾�� ����ƺ� ������
	DAVID_DI_PRACTICEMEET2,			// ���ΰ����� �±�
	DAVID_DI_PRACTICEMEET3,			// ���ΰ� ������
	DAVID_DI_PRACTICEMEET4,			// ���ΰ��� ���
	DAVID_DI_PRACTICEMEET5,			// �������
	DAVID_DI_MOVEBACKTO				// ������ġ�� ���ư���
};

// Base ����
enum NPC_David_BaseState
{
	DAVID_B_NORMAL,	
	DAVID_B_HITTED,
	DAVID_B_HITTED2,
	DAVID_B_HITTED3,
	DAVID_B_STUN,
	DAVID_B_SHOCKED,
	DAVID_B_DOWN
};

// �Ÿ��� ���� ����
enum NPC_David_DistanceState
{
	DAVID_D_FARRANGE = 0,			// �ֶ�
	DAVID_D_MIDDLE,					// �߰�
	DAVID_D_CLOSE,					// �ſ� �����
};

// Ư�� �׼� ����
enum NPC_David_ActionState
{
	DAVID_A_BLADEDANCE1 = 0,		// ȸ�� ����1
	DAVID_A_BLADEDANCE2 = 1,		// ȸ�� ����2
	DAVID_A_BLADEDANCE3 = 2,		// ȸ�� ����3
	DAVID_A_BLOCKED = 3,
	DAVID_A_SWORDIN = 8,			// �� ����ֱ�
	DAVID_A_SWORDOUT = 9,			// �� ������
	DAVID_A_TALK1 = 10,				// ����Ű�� ��
	DAVID_A_TALK2 = 11,				// ��������
	DAVID_A_TALK3 = 12,				// ��Ȳ
	DAVID_A_TALK4 = 13,				// �Ǿ���
	DAVID_A_DASH1 = 14,				// �뽬 ����1
	DAVID_A_DASH2 = 15,				// �뽬 ����2
	DAVID_A_IDLE = 18,
	DAVID_A_DOWN3 = 19,
	DAVID_A_DOWN2 = 20,
	DAVID_A_DOWN1 = 21,
	DAVID_A_FRONTBE = 29,
	DAVID_A_RUN = 31,				// �޸���
	DAVID_A_SHOCKED = 32,			// ����
	DAVID_A_STDALT = 33,			// �����¼�
	DAVID_A_STUN = 34,			
	DAVID_A_ATT1 = 35,				// ����1(�Ʒ������)
	DAVID_A_ATT1_2 = 36,			// ����1(�Ʒ������)
	DAVID_A_ATT2 = 37,				// ����2(���οø���)
	DAVID_A_ATT2_2 = 38,			// ����2(���οø���)
	DAVID_A_ALTRUN = 39,			// �������� �޸���
	DAVID_A_ALTWALK = 40,			// �������� �ȱ�
	DAVID_A_WALK = 45,
};

BEGIN(Client)

class CNPC_David final : public CNPC
{
public:
	CNPC_David();
	virtual ~CNPC_David() = default;

public:
	HRESULT Initialize(_float3 _vCreatePos);
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<CNPC_David> Create(_float3 _vCreatePos, _float3 _vInitLookPos = _float3(0.f, 0.f, 0.f),
		_float3 _vMovePos = _float3(0.f, 0.f, 0.f), _float3 _vSecondPos = _float3(0.f, 0.f, 0.f));

public:
	virtual void InteractionOn();

	void		SetMovePos(_float3 _vMovePos) { m_vMovePos = _vMovePos; }
	void		SetSecondPos(_float3 _vSecondPos) { m_vTrainPos = _vSecondPos; }
	void		SetInitPos(_float3 _vInitPos) { m_vInitPos = _vInitPos; }

public: /* For Quest Skip */
	void SetMoveToTrainingTrigger(_bool _bMoveToTrainingTrigger) { m_bMoveToTraining1Trigger = _bMoveToTrainingTrigger; }
	void SetQuestSkipTime(_float _fQuestSkipTime) { m_fQuestSkipTime = _fQuestSkipTime; }
	void IncreaseDialogueOrder() { m_iDialogOrder++; }

protected:
	HRESULT			AddRigidBody(string _strModelKey);		// RigidBody �߰�
	virtual void	OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ��� ����
	virtual void	OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;	// ��� ���� ��
	virtual void	OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ������

	HRESULT		AddPartObjects(string _strModelKey);
	void		UpdateDistanceState();
	void		RenderBackWeapon();
	void		RenderHandWeapon();
	void		ResetTimer();
	void		AfterSwitch();

	// DialogOrder���¿� ���� ����
	void		FSM_FirstMeet(float _fTimeDelta);
	void		FSM_MoveToTraining(float _fTimeDelta);
	void		FSM_PracticeMeet1(float _fTimeDelta);
	void		FSM_PracticeMeet2(float _fTimeDelta);
	void		FSM_PracticeMeet3(float _fTimeDelta);
	void		FSM_PracticeMeet4(float _fTimeDelta);
	void		FSM_PracticeMeet5(float _fTimeDelta);
	void		FSM_MoveBackTo(float _fTimeDelta);

	// �Ÿ�
	// ��ȭ ��ȣ�ۿ��� ���� �Ÿ�
	_float		m_fMeetRange = 10.f;
	_float		m_fCloseRange = 3.f;

	// ������ ���� �Ÿ�
	_float		m_fAttack1Range = 2.f;		// �������� Range
	_float		m_fOutRange = 10000.f;		// �������� ���� Range



private:
	_bool		m_bMoveToTraining1Trigger = false;
	_bool		m_bMoveStart = false;
	_bool		m_bMoveToTraining1 = false;	// true�̸� Traning���� �̵�����
	_bool		m_bMoveToTraining2 = false;	// true�̸� Traning���� �̵�����
	_bool		m_bMoveToBack1 = false;	// true�̸� ������ġ�� �̵�����
	_bool		m_bMoveToBack2 = false;	// true�̸� ������ġ�� �̵�����
	_bool		m_bTrainStartTiming = false;

	// �Ʒý� ���� ��ġ
	_float3		m_vCreatePos = _float3(0.f, 0.f, 0.f);

	// �ִϸ��̼� �ӵ�����
	_float		m_fActionAnimSpeed[100] = { 0, };

	_int		m_iBaseState = DAVID_B_NORMAL;			// Base ����
	_int		m_iDistanceState = DAVID_D_FARRANGE;	// �Ÿ��� ���� ����
	_int		m_iActionState = DAVID_A_IDLE;			// �ൿ����

	// �ִϸ��̼� �ð�
	_float		m_fWeaponOuTime = m_fKeyFrameSpeed * 18.f;	// ���� ������ �ð�
	_float		m_fWeaponInTime = m_fKeyFrameSpeed * 22.f;	// ���� ����ִ� �ð�

	_float3		m_vMovePos = _float3(0.f, 0.f, 0.f);		// �Ʒ��� ���� ������
	_float3		m_vTrainPos = _float3(0.f, 0.f, 0.f);		// �Ʒ��忡���� ��ġ
	_float3		m_vInitPos = _float3(0.f, 0.f, 0.f);		// ���� ��ġ

	_bool		m_bHandWeaponRender = false;
	_bool		m_bBackWeaponRender = true;

	// �ִϸ��̼� ���� �ð�
	_float		m_fToIdle = 0.5f;
	_float		m_fToTalk = 0.5f;

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

private: /* Dialogue Complete */
	_bool m_bIsDialogueCompleted{ false };

private: /* For Quest Skip */
	_float m_fQuestSkipTime{ 20.f };

};

END