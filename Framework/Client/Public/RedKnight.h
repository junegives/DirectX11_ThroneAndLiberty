#pragma once

#include "Monster.h"
#include "Pillar.h"
#include "FallingSword.h"
#include "Slash.h"
#include "BindMagic.h"
#include "RedKnight_Shadow.h"
#include "Midget_Berserker_Shadow.h"
#include "Orc_DarkCommander_Shadow.h"
//�ý���: ���� ������ ������ �÷��̾���� ��ġ�� �������� �ٰŸ��� �ٰŸ� ��������, �߰Ÿ��� �߰Ÿ� ��������, ��Ÿ��� ��Ÿ� ��������
//�и� : ��������(2�޺�, 3�޺�)�߿� ������ ������ �и��ϸ� �и������� 1�� ������ �������Լ� ���� �и������ ������ �ȴ�(ª�� ����Ÿ��)
//�÷��̾�� �뽬������ �и��ϸ� �и������� 3�� ������ �������Լ� ���� �и������ ������ �ȴ�(ª�� ����Ÿ��)
//�и����� : �и��� ��� �Ͽ� �и������� 5�� �ǰ� �Ǹ� 0���� �ʱ�ȭ �ǰ� ���ÿ� �����̻��� ������ ���� �и������ �߰� ���ÿ� ���� �����ϴ� ������ ���� ��	�� �ȴ�(����Ÿ��)
//
//1������
//�ٰŸ�
//1���� : �����̵� �����Ŀ� �Բ� �÷��̾� ��ó�� �����̵� �Ŀ� 3�޺� ���� ���Ŀ� �÷��̾ ����뽬�� �Ͽ� �߰Ÿ��� �� �Ŀ� ū ������ �÷��̾�� ����
//2���� : �׸��� ��ȯ ����ó 1�� ���� �����Ͽ� ���� ��� �׸��ڸ� ��ȯ���Ŀ� ���� �׸��� ��ȯ ����ó ���Ŀ�
//2�޺� ������ �ϴ� �׸��� 2������ ���ʷ� ��ȯ ���Ŀ� �÷��̾ ����뽬�� �� �Ŀ� ū ������ �÷��̾�� ����
//
//�߰Ÿ�
//1���� : �������� ��ǰ� �Բ� ���������� �����Ŀ� �÷��̾ ���� ��� ������ �Ͽ� ���ǹ��� ������ �� �Ŀ� �����̵� �����Ŀ� �Բ� �÷��̾� ��ó��
//�����̵� �� �Ŀ� 2�޺� ������ �ϰ� ���� �̵�
//2���� : ������ ������ �׸��� 2������ �ڽ��� ������ ���ʷ� ��ȯ�Ŀ� �÷��̾� ��ó�� 2�޺� ������ �ϴ� 2������ ���ʷ� ��ȯ���Ŀ� ���� �̵�
//
//���Ÿ�
//1���� : ������ 3�� ������ �����Ŀ� �������Ŀ� �÷��̾�� ����뽬�� �ϰ� ���Ŀ� �÷��̾� �������� Ÿ���� �뽬�� �ϰ� 2�޺� ����->�ٰŸ�
//2���� : �÷��̾ ���� ����뽬�� �ϴ� �׸��� 2������ ���ʷ� ��ȯ���Ŀ� �÷��̾��� ���ķ� �����Ͽ� �÷��̾�� Ÿ���� �뽬�� �ϰ� 3�޺� ����->�ٰŸ�
//
//�ǰ� 75�ۼ�Ʈ�� ��� �Ǹ� ������ �������� ������ �����Ͽ� �̵��ϰ� Ư��������1�� ����
//
//Ư��������1
//1. Ư�� ��ǰ� �Բ� �ʵ忡 5���� �Ŵ� ���� ����߸�
//2. �׸��� ��ȯ ��ǰ� �Բ� 4������ �׸��ڸ� ������ �ڽ��� ���Ŀ� ��ȯ�ϰ� ����� �׸��ڵ��� �ʵ��� ������ ��ġ�� �����Ͽ� �̵��Ѵ�
//3. ���߿� �ϳ��� ���������� ������ �����ϰ� ��ġ�� ���� �׸��ڵ��� �÷��̾ ���� ū ������ ������ �����ϴµ� ���� ������ �׸��ڵ��� ������ �°� �����Ͽ� ���� �Ǹ� ��ƾ� �Ѵ� �׷��� ���� ���� ���� ������ �ǰ� �ٸ� ���� ���� ���� �ȴ� �̸� 3���� ���� ���� ���������� �ݺ��Ѵ�
//4. �׸��ڵ��� �����ǰ� �Բ� �װ�, �������� ������ ������ �ɸ��µ�, �Ǹ� ���� �˵��� Ȳ�ݺ��� �Ǿ� �������� ������ �������� ���� �������� ������ ���¿��� �ʵ�� �������� ������ ������ �ٽ� �Ͼ 2������� ���Եȴ�

enum RK_SkillState
{
	RK_S_INSKILL1 = 0,	// �����Ÿ� �����Ÿ��̳� 1)
	RK_S_INSKILL2,		// �����Ÿ� �����Ÿ��̳� 2)
	RK_S_MIDDLESKILL1,	// �߰��Ÿ�	 �����Ÿ��̻� 1)
	RK_S_MIDDLESKILL2,	// �߰��Ÿ�   �����Ÿ��̻� 2)
	RK_S_OUTSKILL1,		// �հŸ� 1)
	RK_S_OUTSKILL2,		// �հŸ� 2)
	RK_S_OUTSKILL3,		// �հŸ� 3)
	RK_S_OUTSKILL4,		// �հŸ� 4)
	RK_S_SPECIAL1,		// ��� ����߸���
	RK_S_NOSKILL		// x
};

// Phase
enum RK_Phase
{
	RK_P_NORMAL = 0,
	RK_P_GIMIC1,
	RK_P_TOFURY1,
	RK_P_FURY1,
	RK_P_GIMIC2,
	RK_P_TOFURY2,
	RK_P_FURY2
};

// Base ����
enum RK_BaseState
{
	RK_B_BEFOREMEET = 0,
	RK_B_BEALERT,
	RK_B_ALERT,
	RK_B_NORMAL,
	RK_B_GIMICPATTERN1,
	RK_B_WHILEFURY,
	RK_B_FURY,
	RK_B_GIMICPATTERN2,
	RK_B_WHILEFURY2,
	RK_B_FURY2,
	RK_B_BLOCKED,
	RK_B_HITTED,
	RK_B_HITTED2,
	RK_B_HITTED3,
	RK_B_STUN,
	RK_B_SHOCKED,
	RK_B_DOWN,
	RK_B_DEATH
};

// �Ÿ��� ���� ����
enum RK_DistanceState
{
	RK_D_IN1RANGE = 0,			// �����Ÿ�
	RK_D_IN2RANGE,				// �߰��Ÿ�
	RK_D_OUTRANGE,				// ���Ÿ�
	RK_D_BEFOREMEET			
};

// Ư�� �׼� ����
enum RK_ActionState
{
	RK_A_BLOCKED = 0,
	RK_A_DASHATT1 = 3,
	RK_A_DASHWHILE = 5,
	RK_A_DASHSTART = 6,
	RK_A_SPECIAL1 = 7,				// ������ ���(���� ���ǰ���)
	RK_A_SHORTJUMP = 8,
	RK_A_SPECIAL2 = 9,				// ������ 2�����(�ʱ��� ���ǰ���)
	RK_A_RUNFL = 12,
	RK_A_RUNFR = 13,
	RK_A_MAGIC = 15,				// �ӹڸ���
	RK_A_TOFURY = 16,
	RK_A_CHARGESLASH = 17,			// ��¡������
	RK_A_FRONTQUAKE = 18,			// ��������
	RK_A_GOUP = 19,
	RK_A_DEATH = 27,
	RK_A_DIE = 28,
	RK_A_DOWN3 = 29,
	RK_A_DOWN2 = 30,
	RK_A_DOWN1 = 31,
	RK_A_FRONTDOWN3 = 32,
	RK_A_FRONTDOWN2 = 33,
	RK_A_FRONTDOWN1 = 34,
	RK_A_HITTED = 38,
	RK_A_RUN = 41,
	RK_A_SHOCKED = 42,
	RK_A_GROUNDMAGIC = 43,			// ������ ���� ����
	RK_A_SUMMONSHADOW = 44,			// �׸��� ��ȯ
	RK_A_MAGIC2 = 45,				// �� ����߸���
	RK_A_TELEPORT = 46,				// �ڷ���Ʈ
	RK_A_WAITTOALERT = 47,
	RK_A_WAIT = 48,
	RK_A_ALERTTOWAIT = 49,
	RK_A_SHOTSWORD = 50,			// �� �ļ� ������
	RK_A_ATT_D1 = 52,
	RK_A_ATT_D2 = 53,
	RK_A_ATT_D3 = 54,
	RK_A_ATT_D4 = 55,
	RK_A_ATT_L1 = 57,
	RK_A_ATT_L2 = 58,
	RK_A_ATT_L3 = 59,
	RK_A_ATT_L4 = 60,
	RK_A_ATT_LM1 = 62,
	RK_A_ATT_LM2 = 63,
	RK_A_ATT_U1 = 65,
	RK_A_ATT_U2 = 66,
	RK_A_ATT_U3 = 67,
	RK_A_ATT_U4 = 68,
	RK_A_ATT_U5 = 69,
	RK_A_SUMMON = 71,				// �� ����߸���
	RK_A_ALT = 72,
	RK_A_IDLE = 73,
	RK_A_STUN = 74,
	RK_A_WALK = 75
};

BEGIN(Client)

class CRedKnight final : public CMonster
{
public:
	struct REDKNIGHT_DESC
	{
		CMonster::MONSTER_DESC pTarget;
	};

public:
	CRedKnight();
	virtual ~CRedKnight()  override;

public:
	HRESULT			Initialize();
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
	static shared_ptr<CRedKnight> Create(_float3 _vCreatePos);
	
protected:
	void		ActivePillar();			// ��� ��ȯ
	void		ActivePillarMoveUp();	// ��� �ö����
	void		ActivePillarMoveDown();	// ��� ��������
	void		ActiveSword();		// �� ��ȯ
	void		ActiveGimicShadow4();					// ��� �нŵ� ��ȯ
	void		Active2ComboShadow(_int _iNum);			// 2�޺� �нŵ� ��ȯ
	void		Active3ComboShadow(_int _iNum);			// 3�޺� �нŵ� ��ȯ
	void		ActiveJumpAttackShadow(_int _iNum, _int _iType = 0);		// ���ǰ��� �нŵ� ��ȯ
	void		ActiveSlashShadow(_int _iNum);			// ���� �нŵ� ��ȯ
	void		ActiveDashThroughShadow(_int _iNum, _int _iType = 0);	// ���� �нŵ� ��ȯ
	void		ActiveBindGroundShadow(_int _iNum, _int _iType = 0);	// �ӹ����� �нŵ� ��ȯ
	void		ActiveQuakeShadow(_int _iNum, _int _iType = 0);	// �ӹ����� �нŵ� ��ȯ
	void		ActiveMidgetBerserkerShadow();			// ��� ���� ���� ��ȯ
	void		ActiveDarkOrcShadow();					// ��ũ ���� ���� ��ȯ
	HRESULT		AddRigidBody(string _strModelKey);		// RigidBody �߰�
	virtual void OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ��� ����
	virtual void OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;	// ��� ���� ��
	virtual void OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ������
	
	virtual void EnableCollision(const char* _strShapeTag) override;
	virtual void DisableCollision(const char* _strShapeTag) override;
	
	void		ResetBoss();		// ���� ����
	void		ResetTimer();
	void		ResetSkillState();	// ��ų ���� ����
	void		ActiveSlash(_int _iSlashType);
	void		ActiveBindMagic(_int _iBindMagicType, _int _iCircleMagicType = 0, _float _fReadyTime = 0.f);
	
	HRESULT		AddPartObjects(string _strModelKey);
	void		UpdateDistanceState();
	// �����߿� ȸ�� ����
	void		UpdateAttackDir(_float _fTimeDelta);
	// ���������߿� ȸ�� ����
	void		UpdateSlashDir(_float _fTimeDelta);

	// �ð� �������� Ư���ӵ��� ���¹������� �̲��������ϱ�
	void		UpdateMoveSlide(_float _fSlideStart, _float _fSlideEnd, _float _fSpeed, _float _fTimeDelta);
	// �������� ��ġ ����
	void		UpdateCirclePos(_bool _bDir, _float _fTimeDelta);
	// ���� �ӵ��� ���� ����
	void		UpdateJumpDirSpeed();
	void		UpdateJumpToThroneDirSpeed();
	void		UpdateSpecial1DirSpeed();
	void		UpdateSpecial2DirSpeed();

	_float3		CalculateBindary(_float3 _vPos);		// ���� ��ġ�� ��輱�� ���������� �� �������� ������

	_int		CheckParryingStack(_int _iParryingStack);

	_float3		CalculateTeleportFrontPos();	// �÷��̾� ������ �����̵��� ��ġ ���
	_float3		CalculateTeleportBackPos();		// �÷��̾� ������ �����̵��� ��ġ ���
	_float3		CalculateTeleportFarBackPos();		// �÷��̾� ������ �����̵��� ��ġ ���
	_float3		CalculateCircleRunPos();		// ���� ������ ��ġ ���
	_float3		CalculateDodgePos();			// ȸ�������� �����̵��� ��ġ ���(�߰Ÿ�)
	_float3		CalculateDodge2Pos();			// ȸ�������� �����̵��� ��ġ ���(��Ÿ�)
	

	// ����
	_float3		RandomJumpFrontOrBackPos();
	_float3		JumpFarBackPos();
	_float3		RandomBlinkPos();			// �÷��̾� ������ ������ ���� ��ġ
	_float3		RandomBlinkSlashPos();		// �÷��̾� ���� ��ġ ������ ���� ��ġ
	_float3		RandomBlinkDashPos();		// �÷��̾� ���� ��ġ ������ ���� ��ġ
	_float3		RandomDashOutPos(_float _fDashOutRange = 10.f, _int _iType = 0);			// ���� ���� ������ġ
	// ���� �� ���̵� ��ġ(�� ������)
	_int		RandomSlowSlash();
	_int		RandomNormal2ComboStartAttack();
	_int		RandomNormal3ComboStartAttack();

	// Ư�� ���� �ִϸ��̼� �ڵ����� ������
	// 2Combo
	_bool		Update2ComboAttackState(float _fTimeDelta);
	// 3Combo
	_bool		Update3ComboAttackState(float _fTimeDelta);
	// ����
	_bool		UpdateAttackState(float _fTimeDelta);
	// ����뽬
	_bool		UpdateThroughDashState(float _fTimeDelta, _float _fSpeed = 100.f, _float _fDistance = 30.f);
	// �뽬�� ����뽬 ����
	_bool		UpdateAThroughDashState(float _fTimeDelta, _float _fSpeed = 150.f, _float _fDistance = 30.f);
	// �ձ��� �뽬
	_bool		UpdateFrontDashState(float _fTimeDelta, _float _fSpeed = 100.f, _float _fFrontDistance = 4.f);
	// �������¿��� ����뽬
	_bool		UpdateJumpUpDashState(float _fTimeDelta, _float _fSpeed = 100.f, _float _fDistance = 30.f);
	// �����ؼ� ����뽬
	_bool		UpdateJumpDashState(float _fTimeDelta, _float _fSpeed = 100.f, _float _fDistance = 30.f);
	// Ư���������� ������ �뽬
	_bool		UpdatePositionDashState(float _fTimeDelta, _float _fSpeed = 100.f);
	
	// BaseState�� ���� FSM
	void		FSM_BeforeMeet(_float _fTimeDelta);
	void		FSM_Alert(_float _fTimeDelta);
	void		FSM_BeAlert(_float _fTimeDelta);
	void		FSM_Normal(_float _fTimeDelta);
	void		FSM_GimicPattern1(_float _fTimeDelta);
	void		FSM_WhileFury(_float _fTimeDelta);
	void		FSM_Fury(_float _fTimeDelta);
	void		FSM_GimicPattern2(_float _fTimeDelta);
	void		FSM_WhileFury2(_float _fTimeDelta);
	void		FSM_Fury2(_float _fTimeDelta);
	void		FSM_Blocked(_float _fTimeDelta);
	void		FSM_Hitted(_float _fTimeDelta);
	void		FSM_Hitted2(_float _fTimeDelta);
	void		FSM_Hitted3(_float _fTimeDelta);
	void		FSM_Stun(_float _fTimeDelta);
	void		FSM_Shocked(_float _fTimeDelta);
	void		FSM_Down(_float _fTimeDelta);
	void		FSM_Death(_float _fTimeDelta); 

	void		CheckGimicPattern1();
	void		CheckGimicPattern2();
	void		CheckDead();
	void		CheckAttacked();

private:
	_bool		m_bFirstCinematic = false;				// ù��° ��������
	_bool		m_bFirstCinematicEnd = false;			// ù��° ���� �������� ����
	_int		m_iEffectIdx = 0;

	_int		m_iBaseState = RK_B_BEFOREMEET;			// Base����
	_int		m_iDistanceState = RK_D_BEFOREMEET;		// �Ÿ��� ���� ����
	_int		m_iActionState = RK_A_IDLE;				// �ൿ����
	_int		m_iSkillState = RK_S_NOSKILL;
	
	_float		m_fMeleeRange = 4.f;		// �������� Range
	_float		m_fMelee2Range = 2.f;		// �������� Range(������)
	_float		m_fMelee3Range = 20.f;		// ���ķ� �ָ����� Range ����
	_float		m_fPillarNearRange = 10.f;
	_float		m_fPillarMiddleRange = 25.f;
	_float		m_fPillarFarRange = 40.f;
	_float		m_fBlinkSlashRange = 10.f;
	_float		m_fBlinkDashRange = 30.f;
	_float		m_fShortRange = 10.f;		// �÷��̾ ����� �Ÿ����� ����� �Ÿ�
	_float		m_fInRange = 30.f;			// �÷��̾ ����� �Ÿ� 
	_float		m_fOutRange = 10000.f;		// �÷��̾ �� �Ÿ�
	_float		m_fDashOutRange = 10.f;
	
	_float		m_fFightRange = 40.f;
	_float		m_fMeetRange = 50.f;		// ���� Range
	
	_float		m_fCalculateTime = 0.f;						// ���� Time
	_float		m_fCalculateTime2 = 0.f;					// ���� Time2
	_float		m_fCalculateTime3 = 0.f;					// ���� Time3
	_float		m_fCalculateTime4 = 0.f;					// ���� Time3

	_int		m_i2ComboType = 10;		// 2�޺��� ����
	_int		m_i3ComboType = 10;		// 3�޺��� ����
	
	// �ִϸ��̼� �ӵ� ����
	_float		m_fAnimSpeed = 1.f;
	_float		m_fActionAnimSpeed[100] = { 0, };

	// Ű������ �ð� ����
	_float		m_fFastJumpStartTime = m_fKeyFrameSpeed * 12;
	_float		m_fFastJumpEndTime = m_fKeyFrameSpeed * 20;
	_float		m_fSpecial1JumpStartTime = m_fKeyFrameSpeed * 11;
	_float		m_fSpecial1JumpEndTime = m_fKeyFrameSpeed * 31;
	_float		m_fSpecial2JumpStartTime = m_fKeyFrameSpeed * 11;
	_float		m_fSpecial2JumpEndTime = m_fKeyFrameSpeed * 31;
	_float		m_fChargeSlashEndDirTime = m_fKeyFrameSpeed * 50.f * 1.f/ 1.f;
	_float		m_fChargeSlashStartSlashTime = m_fKeyFrameSpeed * 58.f / 2.f * 1.f / 1.f;
	_float		m_fBindMagicStartTime = m_fKeyFrameSpeed * 31.f / 2.f;
	_float		m_fBindMagicFireTime = m_fKeyFrameSpeed * 30.f / 2.f;
	_float		m_fSpecial2FirstTime = m_fKeyFrameSpeed * 33.f;
	_float		m_fSpecial2SecondTime = m_fKeyFrameSpeed * 144.f;
	_float		m_fQuakeStartTime = m_fKeyFrameSpeed * 40.f;
	_float		m_fJumpAttackEndTime = m_fKeyFrameSpeed * 38.f;
	_float		m_fFinalRoarEndTime = m_fKeyFrameSpeed * 40.f * 4.f;
	_float		m_fQuakeStartTime2 = m_fKeyFrameSpeed * 48.f;

	_bool		m_bQuakeSoundStart = false;
	_bool		m_bQuakeCameraStart = false;
	_bool		m_bJumpAttackSoundStart = false;
	_bool		m_bFinalRoarSoundStart = false;
	
	// InSkill ����
	_bool		m_bSkill1State[10] = { 0, };		// �ٰŸ�1
	_bool		m_bSkill2State[10] = { 0, };		// �߰Ÿ�1
	_bool		m_bSkill3State[10] = { 0, };		// ��ռ�ȯ1	
	_bool		m_bSkill4State[10] = { 0, };		// �ٰŸ�2
	_bool		m_bSkill5State[10] = { 0, };		// �߰Ÿ�2

	// OutSkill ����
	_bool		m_bRaSkill1State[10] = { 0, };		// ���Ÿ�1
	_bool		m_bRaSkill2State[10] = { 0, };		// ���Ÿ�2

	// GimicPattern1 ����
	_bool		m_bGP1SkillState[10] = { 0, };
	_float		m_fGP1DownSpeed = 5.f;

	// GimicPattern2 ����
	_bool		m_bGP2SkillState[10] = { 0, };
	
	// ������ ��ġ, �Ÿ�
	_float3		m_vDodgePos = _float3(0.f, 0.f, 0.f);
	_float		m_vDodgeRange1 = 25.f;			//InrangeSkill1 ����
	_float		m_vDodgeRange2 = 35.f;			//InrangeSkill2 ����

	// ���� Ȥ�� ������ ��ġ
	_float3		m_vMovePos = _float3(0.f, 0.f, 0.f);

	// center����
	_float3		m_vTargetCenterDir = _float3(0.f, 0.f, 0.f);

	// ���� ����, �ӵ�
	_float3		m_vJumpNormalMoveDir = _float3(0.f, 0.f, 0.f);
	_float		m_fJumpMoveSpeed = 1.f;
	_float		m_fJumpMoveSpeedY = 0.f;

	// Ư���������� ���� ü��
	_float		m_fGimicPattern1Hp = 1500.f;
	_float		m_fGimicPattern2Hp = 800.f;
	_float		m_fFury1Hp = 1300.f;
	_float		m_fFury2Hp = 200.f;
	_float		m_fSwordPower = 200.f;

	// ����ȭ�� �������� �ӵ�
	_float		m_fFuryActionSpeed = 1.5f;

	// ���ݸ��� �̲������� �ӵ�
	_float		m_fL2SlideSpeed = 20.f;
	_float		m_fLM2SlideSpeed = 20.f;
	_float		m_fL42SlideSpeed = 20.f;
	_float		m_fU2SlideSpeed = 20.f;
	_float		m_fU3SlideSpeed = 20.f;
	_float		m_fU5SlideSpeed = 20.f;

	// ������
	// ����
	_float		m_fAnimNomalJumpChangeTime = 0.5f;
	_float		m_fAnimNormalAttackStartTime = 0.1f;
	_float		m_fAnimAttackAfterDashStartTime = 0.3f;
	_float		m_fAnimNormalAltChangeTime = 0.1f;
	_float		m_fAnimNormalFastSlashChangeTime = 0.5f;
	_float		m_fAnimNormalRunChangeTime = 0.5f;
	_float		m_fDashDistanceXZ = 0.f;				// �뽬 ����Ÿ�
	_float		m_fDashDistanceXZGround = 0.f;			// ���� ����� �뽬 ����Ÿ�
	_float		m_fDashDistanceY = 0.f;					// �뽬 �����Ÿ�
	_float		m_fDashTime = 0.f;						// �뽬 �ð�
	_float		m_fDashYTime = 0.f;						// �뽬Y �������� �ð�
	_float		m_fDashSpeedY = 0.f;					// �뽬 �����ӵ�
	_float3		m_vDashDir = _float3(0.f, 0.f, 0.f);	// �뽬 ����
	_float		m_fDashBeforeY = 0.f;					// �����ؼ� �뽬 ���� y
	_float		m_fDashUpDistance = 5.5f;				// ���� �뽬�ؼ� �ö󰡴� y
	_float		m_fGroundPosY = -30.8f;					// ���������� ���� y
	_float		m_fDashOutSleepSpeed = 3.f;				// ������ �뽬 �̲������� �ӵ�

	_float3		m_vDashOutPos = _float3(0.f, 0.f, 0.f);
	_float		m_fDistanceToDashOut = 1000.f;
	_float3		m_vDashOutDir = _float3(0.f, 0.f, 0.f);

	// InRange
	// Skill1
	_float		m_fIn1Skill1DashDistanceXZ = 0.f;		// �뽬 ����Ÿ�
	_float		m_fIn1Skill1DashDistanceY = 0.f;		// �뽬 �����Ÿ�
	_float		m_fIn1Skill1DashTime = 0.f;				// �뽬 �ð�
	_float		m_fIn1Skill1DashSpeedXZ = 100.f;		// �뽬 ����ӵ�(������)
	_float		m_fIn1Skill1DashSpeedY = 0.f;			// �뽬 �����ӵ�
	_float3		m_fIn1Skill1DashDir = _float3(0.f, 0.f, 0.f);
	_float		m_fIn1Skill1StandTime = 0.5f;
	_float		m_fIn1Skill1WalkTime = 5.f;
	_float		m_fIn1Skill1Combo3Speed = 2.f;
	_float		m_fIn1Skill1Combo2Speed = 1.5f;
	_float		m_fIn1Skill1SlashSpeed = 1.5f;
	//_float	m_fIn1Skill1WalkTime = 0.3f;
	_float		m_fIn1Skill1WalkSpeed = 1.f;
	_float		m_fIn1To2ComboTime = 0.2f;
	_float		m_fIn1ToDashTime = 1.f;
	_float		m_fIn1Skill1NRunSpeed = 12.f;	// normal
	_float		m_fIn1Skill1RunSpeed = 12.f;	// fury
	_float		m_fIn1Skill1BlinkTime = 1.f;	// fury
	_float		m_fIn1FSkill1Combo3Speed = 1.5f;	// fury
	_float		m_fIn1FSkill1Combo2Speed = 1.8f;	// fury
	_float		m_fIn1FSkill1SlashSpeed = 1.2f;	// fury
	_float		m_fIn1Skill1AttackIntervel = 0.7f;	// fury

	// Skill2
	_float		m_fIn1Skill2Combo3Speed = 1.3f;
	_float		m_fIn1Skill2Combo2Speed = 1.5f;
	_float		m_fIn1Skill2SlashSpeed = 1.f;
	_float		m_fIn2Skill2StandTime = 1.f;
	_float		m_fIn2Skill2WalkTime = 5.f;
	_float		m_fIn2To2ComboTime = 0.2f;
	_float		m_fIn2Skill2CircleRadian = 3.14f / 180.f * 120.f;
	_float		m_fIn2Skill2CircleDistance = 15.f;
	_float		m_fIn2Skill2CircleRunSpeed = 25.f;
	_float		m_fIn2Skill2FCircleRunSpeed = 50.f;	// fury
	_float3		m_fIn2Skill2CircleCenterPos = _float3(0.f, 0.f, 0.f);
	_float		m_fIn2Skill2CircleRunStartTime = 0.1f;
	_float		m_fIn2Skill2CircleTime = 5.f;
	_float		m_fIn1FSkill2Combo3Speed = 1.5f;	// fury
	_float		m_fIn1FSkill2Combo2Speed = 1.5f;	// fury
	_float		m_fIn1FSkill2SlashSpeed = 1.2f;	// fury
	_float		m_fIn1FSkill2SpecialAnimTime = 0.5f;

	// Skill3
	_float		m_fIn1Skill3StandTime = 1.f;
	_float		m_fIn2Skill3SpecialAnimTime = 0.5f;

	// Skill4
	
	// Skill5

	// Fury1
	// Skill1
	_float		m_fF1In1StandTime = 0.5f;
	_float		m_fF1In1Combo2Speed = 2.f;
	_float		m_fF1In1ToDashTime = 1.f;

	// Skill2
	_float		m_fF1In2StandTime = 0.5f;
	_float		m_fF1In2Combo3Speed = 2.f;

	// Skill3
	_float		m_fF1Middle1StandTime = 1.f;
	_bool		m_bF1Middle1FirstSummon = false;

	// Skill4
	_float		m_fF1Middle2StandTime = 0.5f;
	_float		m_fF1Middle2SlashSpeed = 3.f;

	// Skill5
	_float		m_fF1Range1StandTime = 0.5f;
	_float		m_fSummonIntervel = 1.f;
	_float		m_fSummonIntervel2 = 1.5f;
	_int		m_iMagicCount = 0;
	_int		m_iBMagicCount = 5;

	// �н�
	vector<shared_ptr<CRedKnight_Shadow>>		m_vecRedKnightShadow;
	_int		m_iRedKnightShadowCount = 12;
	_int		m_iRedKnightGimicShadowCount = 4;
	
	vector<shared_ptr<CMidget_Berserker_Shadow>>		m_vecMidgetBerserkerShadow;
	_int		m_iMidgetBerserkerShadowCount = 1;

	vector<shared_ptr<COrc_DarkCommander_Shadow>>		m_vecOrcDarkCommanderShadow;
	_int		m_iOrcDarkCommanderShadowCount = 1;

	// Ư������1
	// �������� ��
	vector<shared_ptr<CFallingSword>>		m_vecFallingSword;
	_int		m_iFallingSwordCount = 5;
	_int		m_iTargetSwordIndex = 0;
	_bool		m_bBeforeTargetSword = true;	// ������ �� ���� ����
	_int		m_iTargetCount = 0;				// ������ �� count
	_int		m_iBTargetCount = 3;			// ������ �� countMax
	_float		m_fGP1DownTime = 2.f;
	_bool		m_bSummonShadow[10] = { 0, };
	_float		m_fGimicDisruptShadowTime = 7.f;
	
	// ���
	vector<shared_ptr<CPillar>>		m_vecPillar;
	_int		m_iPillarCount = 10;
	_float		m_fGimic2ReadyTime = 10.f;	// ��� �ö���� �ϰ� ��ٸ��� �ð�
	_float		m_fGimic23SlashSpeed = 2.f;
	_int		m_bGimic2SlashType = 0;	// 1�̸� 3����, 2�̸� ��¡����, 3�� �ӹڸ���
	_float		m_fGimic2TimeLimit = 50.f; // 50.f
	_float		m_fGimic2BeforePillarDownTime = 3.f;

	_bool		m_bSpecial2FirstGround = false;
	_bool		m_bSpecial2SecondGround = false;

	// OutRange
	// Skill1
	_float		m_fOutSkill1SlashSpeed = 2.f;
	_float		m_fOutSkill1Combo2Speed = 1.5f;
	_float		m_fOutSkill1StandTime = 1.f;
	//_int		m_iOutSkill1SlashTotal = 1;

	// Skill2
	_float		m_fOutSkill2Combo3Speed = 2.f;
	_float		m_fOutSkill2StandTime = 1.f;
	_float		m_fOutSkill2WalkSpeed = 1.f;

	// Skill3
	_bool		m_bHealthUnder125 = false;
	_float		m_fOutSkill3StandTime = 1.f;
	_float		m_fIn1Skill3SlashSpeed = 2.f;
	_float		m_fOutSkill3SpecialAnimTime = 0.5f;
	_int		m_iOutSkill3SlashCount = 0;
	_int		m_iOutSkill3BSlashCount = 3;

	// ���� �ö󰡼� �ӹڸ��� ��ȯ ����
	// Skill4
	_bool		m_bOutSkill4ShadowStart = false;
	_bool		m_bOutSkill4Summon[6] = { 0, };
	_float		m_fOutSkill4StandTime = 0.5f;
	_bool		m_bOutSkill4FirstDash = true;
	_float		m_fOutSkill4RunSpeed = 8.f;
	_float		m_fOutSkill4RunTime = 2.f;
	_int		m_iOutSkill4DashCount = 0;
	_int		m_iOutSkill4DashTotalCount = 5;
	_float		m_fOutSkill4AltAfterDashTime = 0.5f;
	_float		m_fOutSkill4AnimAttackSpeed = 1.5f;

	// �����ϴ� �׸��� ����
	_int		m_iGimic1ShadowOrder[100] = { 0,1,2,1,0,2,0,1,2,0,0,1,2,1,0,2,0,1,2,0,0,1,2,1,0,2,0,1,2,0 };
	_int		m_iGimic1ShadowNum = 0;

	// ���� ���� ����
	_int		m_iMeleeSkillOrder[100] = { 0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1 };
	//_int		m_iMeleeSkillOrder[100] = { 1,1,1,1,1,1,1,1,1,1,1 };
	_int		m_iMeleePatternNum = 0;

	// �߰Ÿ� ���� ����
	_int		m_iMiddleRangeSkillOrder[100] = { 2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3 };
	//_int		m_iMiddleRangeSkillOrder[100] = { 2,2,2,2,2,2,2,2,2,2,2,2 };
	_int		m_iMiddleRangePatternNum = 0;
	
	// ���Ÿ� ���� ����
	_int		m_iRangeSkillOrder[100] = { 4,5,4,5,4,5,4,5,4,5,4,5,4,5,4,5,4,5,4,5,4,5,4,5,4,5,4,5,4,5 };
	_int		m_iRangeFurySkillOrder[100] = { 5,5,4,5,5,4,5,5,4,5,5,4,5,5,4,5,5,4,5,5,4,5,5,4,5,5,4,5,5,4,5,5 };
	//_int		m_iRangeSkillOrder[100] = { 4,4,4,4,4,4,4,4,4,4,4,4,4 };
	_int		m_iRangePatternNum = 0;
	
	// �и� ����
	_int		m_iParryingStack = 0;
	_int		m_iBParryingStack = 5;

	_int		m_iAttParryStack = 1;
	_int		m_iDashParryStack = 3;
	_int		m_iParryType = 0;		// 0: �⺻����, 1: �뽬

	// �ܻ��
	_float4		m_vOutDashColor = _float4(1.f, 1.f, 1.f, 1.f);
	_float4		m_vThroughDashColor = _float4(1.f, 1.f, 1.f, 1.f);
	_float4		m_vFrontDashColor = _float4(0.5f, 0.f, 0.f, 1.f);

	// ����
	vector<shared_ptr<CSlash>>		m_vecSlashLD;
	vector<shared_ptr<CSlash>>		m_vecSlashLU;
	vector<shared_ptr<CSlash>>		m_vecSlashLR;
	vector<shared_ptr<CSlash>>		m_vecSlashLRD;
	vector<shared_ptr<CSlash>>		m_vecSlashD;
	vector<shared_ptr<CSlash>>		m_vecSlashU;
	vector<shared_ptr<CSlash>>		m_vecSlashR;
	vector<shared_ptr<CSlash>>		m_vecSlashBig;
	_int		m_iSlashCount = 5;
	_bool		m_bFastComboSlash = false;		// ���� 3ȸ�޺��� ������ �پ ����
	
	_float		m_fFastSlashStartTime = m_fKeyFrameSpeed * 25.f;
	_bool		m_bSlashGen = false;
	_float		m_fFastSlashSpeed = 35.f;
	_float		m_fFFastSlashSpeed = 40.f;
	_float		m_fLongSlashSpeed = 20.f;
	_float		m_fBigSlashSpeed = 100.f;

	_bool		m_bJumpAttack1EffectOn = false;

	// �ӹڸ���
	vector<shared_ptr<CBindMagic>>		m_vecBindMagic;

	_int		m_iBindMagicCount = 10;
	_float		m_fBindMagicSpeed = 50.f;
	_float		m_fBindGuidedMagicSpeed = 20.f;
	_float		m_fBindGuidedMagicSpeed2 = 10.f;
	_bool		m_bBindMagicGen = false;
	_bool		m_bBindCircleMagicStart = false;
	_bool		m_bBindCircleMagicStop = false;
	_float		m_fBindIntervel = 1.f;
	_int		m_iBindMagicGenNum = 0;
	_float		m_fOutRange2ReadyTime = 20.f;

	// ��ġ����
	_float3		m_vCenterPos = _float3(0.f, 0.f, 0.f);
	// �� ������
	_float3		m_vRightTopPos = _float3(0.f, 0.f, 0.f);
	_float3		m_vRightBottomPos = _float3(0.f, 0.f, 0.f);
	_float3		m_vLeftTopPos = _float3(0.f, 0.f, 0.f);
	_float3		m_vLeftBottomPos = _float3(0.f, 0.f, 0.f);

	_float3		m_vSwordFallPos1 = _float3(0.f, 0.f, 0.f);	// ����
	_float3		m_vSwordFallPos2 = _float3(0.f, 0.f, 0.f);	// �»��
	_float3		m_vSwordFallPos3 = _float3(0.f, 0.f, 0.f);	// ���ϴ�
	_float3		m_vSwordFallPos4 = _float3(0.f, 0.f, 0.f);	// ���ϴ�
	_float3		m_vSwordFallPos5 = _float3(0.f, 0.f, 0.f);	// �߾�

	_float3		m_vFrontThronePos = _float3(0.f, 0.f, 0.f); // ���� ��
	
	_float		m_fRightEnd = 0.f;
	_float		m_fLeftEnd = 0.f;
	_float		m_fTopEnd = 0.f;
	_float		m_fBottomEnd = 0.f;
	
	_float		m_fZHLength = 50.f;			// �簢�� �����ϼ� �ִ� ���� ������ ��
	_float		m_fZHLength2 = 30.f;			// �簢�� �����ϼ� �ִ� ���� ������ ��
	_float		m_fCircleRadius = 45.f;		// ���� ���� ������

	_int		m_iPhase = RK_P_NORMAL;

	// �ݶ��̴� ����
	// ���� ����
	const char* m_strATTCollider1 = "M_UndeadArmy_Knight_SK.ao|M_UndeadArmy_Knight_USkdmgA";
	const char* m_strATTCollider2 = "M_UndeadArmy_Knight_SK.ao|M_UndeadArmy_Knight_USkdmgA.002";
	const char* m_strATTCollider3 = "M_UndeadArmy_Knight_SK.ao|M_UndeadArmy_Knight_USkdmgA.004";
	const char* m_strATTCollider4 = "M_UndeadArmy_Knight_SK.ao|M_UndeadArmy_Knight_USkdmgB";
	const char* m_strATTCollider5 = "M_UndeadArmy_Knight_SK.ao|M_UndeadArmy_Knight_USkdmgB.002";
	const char* m_strATTCollider6 = "M_UndeadArmy_Knight_SK.ao|M_UndeadArmy_Knight_USkdmgB.004";
	const char* m_strATTCollider7 = "M_UndeadArmy_Knight_SK.ao|M_UndeadArmy_Knight_USkdmgB_M";
	const char* m_strATTCollider8 = "M_UndeadArmy_Knight_SK.ao|M_UndeadArmy_Knight_USkdmgB_M.002";
	const char* m_strATTCollider9 = "M_UndeadArmy_Knight_SK.ao|M_UndeadArmy_Knight_USkdmgC";
	const char* m_strATTCollider10 = "M_UndeadArmy_Knight_SK.ao|M_UndeadArmy_Knight_USkdmgC.002";
	const char* m_strATTCollider11 = "M_UndeadArmy_Knight_SK.ao|M_UndeadArmy_Knight_USkdmgC.003";
	const char* m_strATTCollider12 = "M_UndeadArmy_Knight_SK.ao|M_UndeadArmy_Knight_USkdmgC.005";
	// �뽬 ����
	const char* m_strDashAttCollider = "M_UndeadArmy_Knight_SK.ao|M_UndeadArmy_Knight_DashAttack_E.001";

	/* For.Trail */
private:
	shared_ptr<class CPointTrail> m_pWeaponTrail;
	HRESULT AddWeaponTrail();		// Add Weapon Trail
	HRESULT ChangeTrailType();		// Change WeaponType, TrailColor

	/* Motion Trail */
	shared_ptr<class CMotionTrail> m_pMotionTrail;
	HRESULT AddMotionTrail(_int _iTrailCnt, _float3 _vTrailColor, _float _fTrailCntPerTick);
	HRESULT SetUseMotionTrail(_bool _bUseMotionTrail, _bool _bUseTimer = false, _float _fTimer = 0.f, _bool _bChangeColor = false, _float4 _vColor = _float4());
};

END