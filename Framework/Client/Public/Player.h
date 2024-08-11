#pragma once
#include "Polymorph.h"

#include "Client_Defines.h"
#include "Weapon.h"
#include "Equipment.h"
#include "PlayerRopeLine.h"

BEGIN(Engine)
class CModel;
class CRigidBody;
END

BEGIN(Client)
class CCameraSpherical;

class CPlayer : public CPolymorph
{
public:
	enum PLAYER_WEAPON_TYPE
	{
		WEAPON_TYPE_SWORD, WEAPON_TYPE_CROSSBOW, WEAPON_TYPE_STAFF, WEAPON_TYPE_END
	};
	enum PLAYER_BASESTATE
	{
		STATE_LOOP, STATE_ACTIVE, STATE_ISDAMAGED, STATE_END
	};
	enum PLAYER_ABNORMAL
	{
		ABNORMAL_NORMAL,					// �Ϲ� �ǰ�
		ABNORMAL_SHORTHITTED,				// ª�� ����
		ABNORMAL_LONGHITTED,				// �� ����
		ABNORMAL_BIND,						// �ӹ�
		ABNORMAL_SHOCK,						// ����
		ABNORMAL_GOOUT,						// ���󰡱�(��)
		ABNORMAL_GOOUT_STRONG				// ���󰡱�(��)
	};
	enum PLAYER_SWORD_SKILL
	{
		SWORD_SKILL_PARRYING, SWORD_SKILL_BRAVE_STRIKE, SWORD_SKILL_RUSH_ATTACK, 
		SWORD_SKILL_GAIA_CRASH, SWORD_SKILL_UPPER_ATTACK, SWORD_SKILL_SPIN_ATTACK,
		SWORD_SKILL_END
	};
	enum PLAYER_CROSSBOW_SKILL
	{
		CROSSBOW_SKILL_PARRYING, CROSSBOW_SKILL_RAPIDSHOT, CROSSBOW_SKILL_MULTIPLESHOT,
		CROSSBOW_SKILL_INDOMITABLESPIRIT, CROSSBOW_SKILL_ELECTRICSHOT, CROSSBOW_SKILL_FURIOUSFIRE,
		CROSSBOW_SKILL_END	
	};
	enum PLAYER_STAFF_SKILL
	{
		STAFF_SKILL_PARRYING, STAFF_SKILL_FIREBALL, STAFF_SKILL_ICESPEAR,
		STAFF_SKILL_FIREBLAST, STAFF_SKILL_LIGHTNIGJUDGMENT, STAFF_SKILL_METEOR,
		STAFF_SKILL_END
	};
	enum PLAYER_ANIMATION
	{
		//====================================
		// Sword
		//====================================	
		//===========Sword �⺻ ���===========
		WEAPON_SWORD_IDLE = 311,
		WEAPON_SWORD_RUN = 349, //o
		WEAPON_SWORD_ROLLING = 361, //o
		WEAPON_SWORD_PUTOUT = 313, //o

		WEAPON_SWORD_JUMP_READY = 328, 
		WEAPON_SWORD_JUMP_MAIN = 333, //o
		WEAPON_SWORD_JUMP_LOOP = 325, 
		WEAPON_SWORD_JUMP_LAND = 324, //o

		WEAPON_SWORD_JUMP_READY_RUN = 329,
		WEAPON_SWORD_JUMP_MAIN_RUN = 334, //o
		WEAPON_SWORD_JUMP_LOOP_RUN = 326,
		WEAPON_SWORD_JUMP_LAND_RUN = 331,

		WEAPON_SWORD_PARRYING = 369, // o
		WEAPON_SWORD_PARRYING_ATTACK = 370,
		//=======================================

		//===========Sword ���� ���===========
		WEAPON_SWORD_ATTACK_COMBO_1 = 375,
		WEAPON_SWORD_ATTACK_COMBO_2 = 376,
		WEAPON_SWORD_ATTACK_COMBO_3 = 377,
		WEAPON_SWORD_ATTACK_COMBO_4 = 403,

		WEAPON_SWORD_SKILL_BRAVE_STRIKE_1 = 394,
		WEAPON_SWORD_SKILL_BRAVE_STRIKE_2 = 395,
		WEAPON_SWORD_SKILL_BRAVE_STRIKE_3 = 397,

		WEAPON_SWORD_SKILL_RUSH_ATTACK = 360,

		WEAPON_SWORD_SKILL_GAIA_CRASH = 389,

		WEAPON_SWORD_SKILL_UPPER_ATTACK = 410,

		WEAPON_SWORD_SKILL_SPIN_ATTACK_1 = 405,
		WEAPON_SWORD_SKILL_SPIN_ATTACK_2 = 406,
		WEAPON_SWORD_SKILL_SPIN_ATTACK_3 = 407,
		//=======================================

		//============Sword �ʻ� ���============
		WEAPON_SWORD_SUPREME_SKILL_READY	= 382,
		WEAPON_SWORD_SUPREME_SKILL_ATTACK	= 370,
		//=======================================
		
		//===========Sword �ǰ� ���===========
		WEAPON_SWORD_DAMAGED_NORMAL_B		= 304,
		WEAPON_SWORD_DAMAGED_NORMAL_F		= 305,
		WEAPON_SWORD_DAMAGED_NORMAL_L		= 306,
		WEAPON_SWORD_DAMAGED_NORMAL_R		= 307,
		WEAPON_SWORD_DAMAGED_STIFF_SHORT	= 274, //o
		WEAPON_SWORD_DAMAGED_STIFF_LONG_S	= 280,
		WEAPON_SWORD_DAMAGED_STIFF_LONG_L	= 279, //o
		WEAPON_SWORD_DAMAGED_STIFF_LONG_E	= 278,
		WEAPON_SWORD_DAMAGED_DOWN_START		= 286,//o
		WEAPON_SWORD_DAMAGED_DOWN_LOOP		= 284,
		WEAPON_SWORD_DAMAGED_DOWN_END		= 282,
		WEAPON_SWORD_DAMAGED_DOWN_LONG_START= 285,
		WEAPON_SWORD_DAMAGED_DOWN_LONG_LOOP	= 283, //o
		WEAPON_SWORD_DAMAGED_DOWN_LONG_END	= 281,
		WEAPON_SWORD_DAMAGED_SHOCK			= 293,
		WEAPON_SWORD_DAMAGED_STUN			= 294, //o
		//========================================


		//=======================================
		// CrossBow
		//=======================================
		//===========CrossBow �⺻ ���===========
		WEAPON_CROSSBOW_IDLE = 52,
		WEAPON_CROSSBOW_RUN = 112, //o
		WEAPON_CROSSBOW_BACKSTEP = 136,
		WEAPON_CROSSBOW_ROLLING = 412, //o
		WEAPON_CROSSBOW_PUTOUT = 56, //o

		WEAPON_CROSSBOW_JUMP_READY = 93,
		WEAPON_CROSSBOW_JUMP_MAIN = 96, //o
		WEAPON_CROSSBOW_JUMP_LOOP = 89,
		WEAPON_CROSSBOW_JUMP_LAND = 88, //o

		WEAPON_CROSSBOW_JUMP_READY_RUN = 92,
		WEAPON_CROSSBOW_JUMP_MAIN_RUN = 97, //o
		WEAPON_CROSSBOW_JUMP_LOOP_RUN = 90,
		WEAPON_CROSSBOW_JUMP_LAND_RUN = 94,

		WEAPON_CROSSBOW_PARRYING = 42,
		//=======================================

		//===========CrossBow ���� ���===========
		WEAPON_CROSSBOW_ATTACK_COMBO_1 = 159,
		WEAPON_CROSSBOW_ATTACK_COMBO_2 = 161,
		WEAPON_CROSSBOW_ATTACK_COMBO_3 = 163,
		WEAPON_CROSSBOW_ATTACK_COMBO_4 = 165,

		WEAPON_CROSSBOW_ATTACK_LOCK_ON_COMBO_1 = 162,
		WEAPON_CROSSBOW_ATTACK_LOCK_ON_COMBO_2 = 164,
		WEAPON_CROSSBOW_ATTACK_LOCK_ON_COMBO_3 = 147,
		WEAPON_CROSSBOW_ATTACK_LOCK_ON_COMBO_4 = 145,

		WEAPON_CROSSBOW_SKILL_RAPIDSHOT			= 74, // �и�����

		WEAPON_CROSSBOW_SKILL_MULTIPLESHOT		= 68, // �и�����

		WEAPON_CROSSBOW_SKILL_INDOMITABLESPIRIT = 64, // �и�����

		WEAPON_CROSSBOW_SKILL_ELECTRICSHOT		= 152,// �и��Ұ���

		WEAPON_CROSSBOW_SKILL_FURIOUSFIRE_READY = 82, // �и��Ұ���
		WEAPON_CROSSBOW_SKILL_FURIOUSFIRE_LOOP	= 81, // �и��Ұ���
		WEAPON_CROSSBOW_SKILL_FURIOUSFIRE_END	= 80, // �и��Ұ���
		//=======================================

		//===========CrossBow �ǰ� ���===========
		WEAPON_CROSSBOW_DAMAGED_NORMAL_B		= 46,
		WEAPON_CROSSBOW_DAMAGED_NORMAL_F		= 47,
		WEAPON_CROSSBOW_DAMAGED_NORMAL_L		= 48,
		WEAPON_CROSSBOW_DAMAGED_NORMAL_R		= 49,
		WEAPON_CROSSBOW_DAMAGED_STIFF_SHORT		= 274,	// ��հ� ���
		WEAPON_CROSSBOW_DAMAGED_STIFF_LONG_S	= 19,
		WEAPON_CROSSBOW_DAMAGED_STIFF_LONG_L	= 18, //
		WEAPON_CROSSBOW_DAMAGED_STIFF_LONG_E	= 17,
		WEAPON_CROSSBOW_DAMAGED_DOWN_START		= 25, //
		WEAPON_CROSSBOW_DAMAGED_DOWN_LOOP		= 23,
		WEAPON_CROSSBOW_DAMAGED_DOWN_END		= 21,
		WEAPON_CROSSBOW_DAMAGED_DOWN_LONG_START = 24,
		WEAPON_CROSSBOW_DAMAGED_DOWN_LONG_LOOP	= 22, //
		WEAPON_CROSSBOW_DAMAGED_DOWN_LONG_END	= 20,
		WEAPON_CROSSBOW_DAMAGED_SHOCK			= 293, // ��հ� ���
		WEAPON_CROSSBOW_DAMAGED_STUN			= 32, //
		//========================================


		//=======================================
		// Staff
		//=======================================
		//===========Staff �⺻ ���===========
		WEAPON_STAFF_IDLE		= 192,
		WEAPON_STAFF_RUN		= 222, //o
		WEAPON_STAFF_ROLLING	= 234, //o
		WEAPON_STAFF_PUTOUT		= 193, //o

		WEAPON_STAFF_JUMP_READY	= 201,
		WEAPON_STAFF_JUMP_MAIN	= 206, //o 
		WEAPON_STAFF_JUMP_LOOP	= 199,
		WEAPON_STAFF_JUMP_LAND	= 198, //o 

		WEAPON_STAFF_JUMP_READY_RUN = 202,
		WEAPON_STAFF_JUMP_MAIN_RUN	= 207, //o
		WEAPON_STAFF_JUMP_LOOP_RUN	= 200,
		WEAPON_STAFF_JUMP_LAND_RUN	= 204,

		WEAPON_STAFF_PARRYING = 245,
		//=======================================

		//===========Staff ���� ���===========
		WEAPON_STAFF_ATTACK_COMBO_1 = 237,
		WEAPON_STAFF_ATTACK_COMBO_2 = 239,
		WEAPON_STAFF_ATTACK_COMBO_3 = 240,

		WEAPON_STAFF_SKILL_FIREBALL			= 264,
		WEAPON_STAFF_SKILL_ICESPEAR			= 230,
		WEAPON_STAFF_SKILL_FIREBLAST		= 266,
		WEAPON_STAFF_SKILL_LIGHTNIGJUDGMENT = 229,
		WEAPON_STAFF_SKILL_METEOR			= 268,

		//=======================================

		//===========Staff �ǰ� ���===========
		WEAPON_STAFF_DAMAGED_NORMAL_B		= 184,
		WEAPON_STAFF_DAMAGED_NORMAL_F		= 185,
		WEAPON_STAFF_DAMAGED_NORMAL_L		= 186,
		WEAPON_STAFF_DAMAGED_NORMAL_R		= 187,
		WEAPON_STAFF_DAMAGED_STIFF_SHORT	= 274, // ��հ� ��� 
		WEAPON_STAFF_DAMAGED_STIFF_LONG_S	= 19, // ũ�ν����� ��� 
		WEAPON_STAFF_DAMAGED_STIFF_LONG_L	= 18, // ũ�ν����� ���
		WEAPON_STAFF_DAMAGED_STIFF_LONG_E	= 17, // ũ�ν����� ���
		WEAPON_STAFF_DAMAGED_DOWN_START		= 25, // ũ�ν����� ���
		WEAPON_STAFF_DAMAGED_DOWN_LOOP		= 23, // ũ�ν����� ���
		WEAPON_STAFF_DAMAGED_DOWN_END		= 21, // ũ�ν����� ���
		WEAPON_STAFF_DAMAGED_DOWN_LONG_START= 24, // ũ�ν����� ���
		WEAPON_STAFF_DAMAGED_DOWN_LONG_LOOP	= 22, // ũ�ν����� ���
		WEAPON_STAFF_DAMAGED_DOWN_LONG_END	= 20, // ũ�ν����� ���
		WEAPON_STAFF_DAMAGED_SHOCK			= 177,
		WEAPON_STAFF_DAMAGED_STUN			= 178, //
		//========================================
		
		
		//========================================
		// �Ͽ� �̵�
		LOCK_ON_MOVE_F = 418,
		LOCK_ON_MOVE_L = 421,
		LOCK_ON_MOVE_R = 422,
		LOCK_ON_MOVE_B = 417,
		LOCK_ON_MOVE_FL = 419,
		LOCK_ON_MOVE_FR = 420,
		LOCK_ON_MOVE_BL = 417,
		LOCK_ON_MOVE_BR = 417,
		//========================================


		//========================================
		// ���� �ִϸ��̼�
		POLYMORPH_HUMAN_TO_WOLF		= 413, 
		POLYMORPH_WOLF_TO_HUMAN		= 415,
		POLYMORPH_HUMAN_TO_WOLF_RUN = 414,  //o
		POLYMORPH_WOLF_TO_HUMAN_RUN = 416,  //o 
		//========================================

		//========================================
		// �����׼� �ִϸ��̼�
		ROPE_ACTION_ALL = 425,
		//========================================

	};
	enum PLAYER_PROJECTILE_OWNER
	{
		CROSSBOW_L, CROSSBOW_R, STAFF, STAFF_HAND, TARGET, FRONT, END_PROJECTILE_OWNER
	};

public:
	CPlayer();
	virtual ~CPlayer()  override;

public:
	virtual HRESULT Initialize();
	virtual void	PriorityTick(_float _fTimeDelta)  override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta)  override;
	virtual HRESULT Render()  override;

public:
	virtual void OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;
	virtual void OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;
	virtual void OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;

	virtual void EnableCollision(const char* _strShapeTag) override;
	virtual void DisableCollision(const char* _strShapeTag) override;

	virtual void PlayEffect(string _strGroupID) override;
	virtual void ShootProjectile(_int _eProjectileOwner) override;		// Projectile �߻� 

public:
	virtual HRESULT RenderShadow() override;

public:
	void	RestoreHp() { m_fCurrentHp = m_fMaxHp; }
	void	SimulationOff();
	void	SetInDungeon(_bool _IsDungeon) { m_IsDungeonLevel = _IsDungeon; }
	void	LockOnChangeToBoss();	// �Ͽ��� ������ ����
	void	LockOnChangeToWeapon();	// �Ͽ��� ���⺰��
	void	LockOnOff();			// �Ͽ²���
	void	AddSupremeGauge(_float _AddGauge);	// �ʻ�� ������ ����


private:
	// FSM ����
	void	LoopStateFSM(_float _fTimeDelta);
	void	ActiveStateFSM(_float _fTimeDelta);
	void	IsDamagedFSM(_float _fTimeDelta);

	_float	m_fCalculateTime = 0.f;

	// �� �̵���
	_bool	m_bSceneChange = false;
	_int	m_iChangeScene = 0;

	// ���� ����
	_bool	MoveControlNormal(_float _fTimeDelta, _bool _bMove = true);	// �Ϲ� �̵� ���� ���� [ fasle : �̵�x, �̵��� ���θ� ��ȯ ] 
	_bool	MoveControlSeparate(_float _fTimeDelta);		// �и� �̵� ���� ����
	_bool	MoveControlSkill(_float _fTimeDelta);			// ��ų �̵� ���� ����
	_bool	ComboAttackControl(_float _fTimeDelta);			// �޺� ���� ����
	_bool	SkillControl();									// ��ų ���� ����
	_bool	CheckWeaponChange();							// ���� Ÿ�� ���� üũ
	void	AnimationActive(_float _fTimeDelta);			// �ִϸ��̼ǿ� �ʿ��� ���� ó��
	void	PlayReservedAnim();								// ����� �ִϸ��̼� ���
	_bool	PlayAnimationSet();								// �ִϸ��̼� ��Ʈ ���
	void	CancleAnimationSet();							// �ִϸ��̼� ��Ʈ ��� ĵ��
	void	TriggerAnimationSet(string _strSetName, _uint _SetFlag);	// �ִϸ��̼� ��Ʈ ��� Ʈ����
	_bool	AssaultAnim(_uint _iAnimIndex, _float _fChangingDelay, _float _fCancleRatio = 1.f);	// ���Ʈ �ִϸ��̼� 
	void	TriggerAssaultAnimSet(string _strSetName, _uint _SetFlag);	// ���Ʈ �ִϸ��̼� ��Ʈ
	void	DamagedStateControl();							// �ǰ� ���� ����
	_bool	RopeActionControl();							// �����׼� ����


	// ���� ����
	void	CommonTick(_float _fTimeDelta);					// ���������� ���� Tick���� ��Ƶ�
	void	DashMove(_float _fTimeDelta, _float _fRatio);	// �뽬
	_float3	SetDirectionFromCamera(_float _fDgree, _bool _isSoft = true, _bool _bGetDir = false);	// ī�޶� �������� �÷��̾��� ������ ����
	void	SetSeparateDir(_float _fTimeDelta, _float _fDgree);	// �� ��ü �и� ��, [�̵�����, �ִϸ��̼ǹ���] �� ����
	void	SetToLockOnDir();						// �Ͽ� �� Ÿ�� �������� �ٶ󺾴ϴ�.
	_bool	ChangeAnim(_uint _iAnimIndex, _float _fChangingDelay, _bool _isLoop = false, _float _fCancleRatio = 1.f);	// �ִϸ��̼� ����
	_bool	ReserveAnim(PLAYER_ANIMATION _eReservationAnim, _float _fDashSpeed = 0.f, _float _fCancelRatio = 1.f);		// �ִϸ��̼� ����
	_bool	PlaySkill(_uint _iSkillIndex);			// ��ų ���
	_bool	PlaySupremeSkill();			// �ʻ�� ���
	_bool	CheckCanSeparate(_uint _iAnimIndex);	// ���� �ִϸ��̼��� �и� ������ �ִϸ��̼����� ��ȯ
	_bool	CheckCanMovement(_uint _iAnimIndex);	// ���� �ִϸ��̼��� �̵� ������ �ִϸ��̼����� ��ȯ
	_bool	CheckSkillAnim(_uint _iAnimIndex);		// ���� �ִϸ��̼��� ��ų ��� �ִϸ��̼����� ��ȯ
	_bool	CheckIgnoreAbnormalAnim(_uint _iAnimIndex);		// ���� �ִϸ��̼��� �ǰ� ��� ���� �ִϸ��̼����� ��ȯ
	void	LockOnControl();						// �Ͽ� ����
	void	LandingControl();						// ���� ����
	void	CheckTouched();							// Ÿ�ٰ� ����ִ��� üũ
	void	ShootBuffProjectile(_float3 _vStartPos, _float3 _vTargetPos);		// ���� �߰�Ÿ �߻�
	void	PolymorphControl(_float _fTimeDelta);	// ���� ��Ʈ��


	// ���� ����
	void	TickWeapon(_float _fTimeDelta);					// ���õ� ���⸦ Tick
	void	LateTickWeapon(_float _fTimeDelta);				// ���õ� ���⸦ LateTick
	void	TickEquipment(_float _fTimeDelta);				// ���õ� ��� Tick
	void	LateTickEquipment(_float _fTimeDelta);			// ���õ� ��� LateTick

public:
	// ���� ���� �ӽ�
	void	ChangeEquipment(ITEM_EQUIP_TYPE _eparts, string _ModelName);
	void	NoneEquipment(ITEM_EQUIP_TYPE _eparts);

public:
	void	SetPlayerHp(_float _fHpVariation);
	void	SetUIInputState(_bool _isInputStop) { m_isUIInputStop = _isInputStop; }
	void	SetAllInputState(_bool _isInputStop);
	void	SetRopeTargetPos(_float3 _vRopeTargetPos);
	void	SetHealing();


public:
	_float	GetPlayerHp() { return m_fCurrentHp; }
	_float	GetPlayerHpRatio() { return (m_fCurrentHp/m_fMaxHp) ; }
	PLAYER_ANIMATION	GetCurrentAnimation() { return m_eCurrentAnimation; }
	_bool	IsCrossBowBuffed() { return m_bCrossBowBuff; }


private:
	HRESULT MakeAnimationSet();		// �ִϸ��̼� ��Ʈ ���
	HRESULT SettingSkillCoolTime();	// ��ų ��Ÿ�� ����
	HRESULT AddSeparateAnims();		// ��, ��ü �и� ������ �ִϸ��̼ǵ� ���
	HRESULT AddMovementAnims();		// �̵��� ������ �ִϸ��̼ǵ� ���
	HRESULT AddSkillAnims();		// ��ų �ִϸ��̼ǵ� ���
	HRESULT AddIgnoreAbnormalAnims();	// �ǰݸ�� ���� ������ �ִϸ��̼ǵ� ���
	HRESULT SetAnimOwner();			// �ִϸ��̼ǵ��� ���� ���
	HRESULT CreateCamera();

private:
	HRESULT AddWeapons();		// ���� �߰�
	HRESULT AddEquipments();	// ��� �߰�
	HRESULT AddRigidBody(string _strModelKey);	// RigidBody �߰�
	HRESULT AddProjectilePool();				// ProjectilePool �߰�
	HRESULT AddProjectileTriggers(string _strModelKey);	// ProjectileTriggers �߰�
	HRESULT AddRopeLine();		// �����׼� ��, �׷��� ���� �߰�


public:
	HRESULT ReadyPlayerLight();
	
/*Player Lighting*/
private:
	_uint m_iPlayerLightNum = 0;
	_uint m_iPlayerLightNum2 = 0;


private:
	_bool*					 m_pSeparating= { nullptr };	// ��,��ü �и� �� ����
	vector<PLAYER_ANIMATION> m_SeparateAnims;				// ��, ��ü �и� ������ �ִϸ��̼ǵ�
	vector<PLAYER_ANIMATION> m_MovementAnims;				// �̵��� ������ �ִϸ��̼ǵ�
	vector<PLAYER_ANIMATION> m_SKillAnims;					// ��ų �ִϸ��̼ǵ�
	vector<PLAYER_ANIMATION> m_IgnoreAbnormalAnims;			// �ǰݸ�� ���� ������ �ִϸ��̼ǵ�


	_float				m_fDashSpeed		= { 5.f };		// �뽬 �ӵ�

	_float				m_fReserveDashSpeed = { 0.f };		// �뽬 �ӵ� ����
	_float				m_fReserveCancelRatio = { 1.f };	// CancelRatio ����
	PLAYER_ANIMATION	m_eReservedAnimation = {};			// ���� �ִϸ��̼� ����
	_bool				m_isReserved = { false };			// ����� �ִϸ��̼��� �ִ���
	_float				m_fReservationTimer = { 0.f };		// ����ð� üũ

	_uint				m_iComboCount	= { 0 };			// �޺� ī��Ʈ
	_float				m_iComboTimer	= { 0.f };			// �޺� �ð� üũ

	_uint				m_iSetTrigger	= { false };	// �ִϸ��̼� ��Ʈ ��� �÷��� [0==����, 1==!Loop, 2==Loop]
	_uint				m_iAnimSetCount = { 0 };		// �ִϸ��̼� ��Ʈ ī��Ʈ
	vector<tuple<PLAYER_ANIMATION, _float, _float>>	m_vecAnimationSet;	// ���� �ִϸ��̼� ��Ʈ

	_float	m_fSKillCoolTime[WEAPON_TYPE_END][SWORD_SKILL_END] = {};	//��ų ��Ÿ��
	_float	m_fCurrentCoolTime[WEAPON_TYPE_END][SWORD_SKILL_END] = {};	// ���� ��Ÿ��
	_bool	m_bCanUseSkill[WEAPON_TYPE_END][SWORD_SKILL_END] = {};		// ��ų ��� ���� ����

	_bool	m_isSeparateLockOn	= { false };			// ī�޶� �Ͽ� ��������
	weak_ptr<CGameObject>		m_pCurrentLockOnTarget;	// �Ͽ� ��� ������Ʈ
	
	_int	m_iCurrentLockOnCursor	= { -1 };			// ���� �Ͽ� Ŀ��

	_bool	m_isJumpState	= { true };		// ���� ��������
	_bool	m_isLanding		= { false };	// ��������

	vector<string>		m_vecParrieds;		// �丵 ����
	map<string, _int>	m_mapAbnormals;		// �ǰ� ����

	_float			m_fMaxHp		= 1000.f;	// �ִ� Hp
	_float			m_fCurrentHp	= 1000.f;	// ���� Hp

	_bool			m_bRenderHair	= { true };		// �Ӹ�ī�� ���� On/Off
	_bool			m_isUIInputStop	= { false };	// KeyInput On/Off (UI)
	_bool			m_isAllInputStop = { false };	// KeyInput On/Off (����)
	_bool			m_isTouched		= { false };	// ���� Ÿ�ٰ� ����ִ� ��������.
	_bool			m_bCrossBowBuff = { false };	// ũ�ν� ���� �ړ� On/Off
	_float			m_fCrossBowBuffPersistTime = { 0 };		// ũ�ν� ���� �ړ� ���ӽð�

	_float			m_fEagleFormDuringTime	= { 0.5f };		// �̱��� ���� ��� �ð�(������ð�)
	_bool			m_isEagleChanging		= { false };	// �̱������� ���� ������

	string			m_strEffectGroupID = {""};				// ����Ʈ �׷��� ID
	_int			m_EffectGroupIDX	= { -1 };			// ���������� �ټ��� ����Ʈ �׷��� �� �� �ε�������.

	_float3			m_vRopeTargetDir	= { 0.f, 0.f, 0.f };	// ������ ���� ��ġ�� ���� ����(ũ�� ����)
	_bool			m_isRopeTarget		= { false };	// ���� Ÿ���� �ִ���
	_float			m_fRopeAnimDur		= { 0.f };		// �����׼��� �����ϰ� �帥 �ð�
	_float			m_fRopeMoveSpeed	= { 0.f };		// �����׼��̵� �ӵ�
	_bool			m_isGravity			= { true };		// �߷� ����������
	_bool			m_bRopeRend			= { false };	// ���� �����ϴ���

	_bool			m_bCrossHair		= { false };	// ũ�ν���� ���� �ִ��� 

	_float			m_fSupremeGaugeMax		= { 100 };	// �ִ� �ʻ�� ������
	_float			m_fCurrentSupremeGauge	= { 0 };	// �ʻ�� ������
	_bool			m_bCanUseSupreme		= { false };	// �ʻ�� ������

	_uint			m_iLockOnMode = { 0 };  // [0==���⺰�Ͽ� / 1==�����Ͽ� / 2==Off�Ͽ�]

	_float			m_fJumpMinActiveTime = { 0.f };	// ���� �ּ� ���� �ð�
	_bool			m_isJumpMinActive = { false };	// ���� �ּ� ���� ��
	_bool			m_bFuriousFire	= { false };
	_float3			m_vFuriousFireStartPos	= { 0.f, 0.f, 0.f };
	_float3			m_vFuriousFireTargetPos = { 0.f, 0.f, 0.f };

	_bool			m_bHealing = { false };
	_float			m_fHealingDelay = { 0.f };

private:
	_bool			m_IsDungeonLevel = false;

private:
	PLAYER_WEAPON_TYPE		m_eCurrentWeapon		= { WEAPON_TYPE_SWORD };	// ���� ���� Ÿ�� ����
	PLAYER_BASESTATE		m_eCurrentBaseState		= { STATE_LOOP };			// ���� �÷��̾� �� ����
	PLAYER_ANIMATION		m_eCurrentAnimation		= { WEAPON_SWORD_IDLE };	// ���� �÷��̾� �ִϸ��̼� (��ü or ��ü)

private:
	// �ִϸ��̼� ���� ���� ��Ʈ�� map<���� �̸�, vector<tuple<�ִϸ��̼�����, �����ð�, ĵ������ > > >
	map<string, vector<tuple<PLAYER_ANIMATION, _float, _float > > >	m_AnimationSets;	

private:
	map<string, shared_ptr<CWeapon>>		m_pPlayerWeapons;
	vector<shared_ptr<CEquipment>>			m_pPlayerEquipments;
	map<string, shared_ptr<CModel>>			m_pPolymorphs;
	vector<weak_ptr<CGameObject>>			m_pLockOnTargets;		// �Ͽ� ������ Ÿ�ٵ�
	vector<weak_ptr<CGameObject>>			m_pTouchedTargets;;		// ����ִ� Ÿ�ٵ�
	shared_ptr<CPlayerRopeLine>				m_pRopeLine = { nullptr };

private:
	shared_ptr<CWeapon>						m_pCurrentWeapon;
	shared_ptr<class CPlayerProjectilePool> m_pProjectilePool;
	_float4x4* pRHandMatrix = { nullptr };
	_float4x4* pLHandMatrix = { nullptr };

/*Hair Color*/
private:
	_float3 m_vHairMainColor = { 0.02f, 0.02f, 0.07f };
	_float3 m_vHairSubColor = { 0.4f, 0.4f, 0.4f };

/* For.Trail */
private:
	shared_ptr<class CPointTrail> m_pWeaponTrail;
	HRESULT AddWeaponTrail();		// Add Weapon Trail
	HRESULT ChangeTrailType();		// Change WeaponType, TrailColor
	void	SetWeaponGlow(_bool _bGlow, _bool _bUseGlowColor = false, _float4 _vGlowColor = _float4( 1.f, 1.f, 1.f, 1.f ), _bool _bUseGlowTime = false, _float _fGlowTime = 0.f);
	void	SetWeaponMeshTrail(_bool _bUseMeshTrail, _float4 _vMeshTrailColor = _float4(1.f, 1.f, 1.f, 1.f), _bool _bUseMeshTrailTime = false, _float _fMeshTrailTime = 0.f);
	_bool	TestKeyInput();

	HRESULT AddMotionTrail(_int _iTrailCnt);
	vector<shared_ptr<class CMotionTrail>> m_pMotionTrail;
	HRESULT SetUseMotionTrail(_bool _bUseMotionTrail, _bool _bUseTimer = false, _float _fTimer = 0.f, _bool _bChangeColor = false, _float4 _vColor = _float4());

public:
	static shared_ptr<CPlayer> Create();
};

END