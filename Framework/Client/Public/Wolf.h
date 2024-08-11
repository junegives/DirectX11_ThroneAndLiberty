#pragma once
#include "Polymorph.h"

BEGIN(Engine)

END

BEGIN(Client)
class CCameraSpherical;

class CWolf : public CPolymorph
{
public:
	enum ANIMAL_BASESTATE
	{
		STATE_LOOP, STATE_ACTIVE, STATE_END
	};
	enum WOLF_ANIMATION
	{
		ANIMAL_WOLF_EMOTION_1			= 9, //o
		ANIMAL_WOLF_EMOTION_2			= 0, //o
		ANIMAL_WOLF_IDLE				= 1,
		ANIMAL_WOLF_RUN_NORMAL			= 7, //o
		ANIMAL_WOLF_RUN_FAST			= 6, //o
		ANIMAL_WOLF_RUN_NORMAL_TO_FAST	= 8,
		ANIMAL_WOLF_JUMP_START			= 5, //o
		ANIMAL_WOLF_JUMP_LOOP			= 3,
		ANIMAL_WOLF_JUMP_END			= 2, //o
		ANIMAL_WOLF_JUMP_END_RUN		= 4 //o
	};

public:
	CWolf();
	virtual ~CWolf()  override;

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

public:
	virtual HRESULT RenderShadow() override;

private:
	// FSM ����
	void	LoopStateFSM(_float _fTimeDelta);
	void	ActiveStateFSM(_float _fTimeDelta);

	// ���� ����
	 _bool	MoveControl(_float _fTimeDelta, _bool _bMove = true);	// �̵� ����	
	 void	LandingControl();	// ���� ����
	 _bool	EmotionControl();	// ����ǥ�� ����


	// ���� ���� 
	void	CommonTick(_float _fTimeDelta);
	void	ChangeAnim(_uint _iAnimIndex, _float _fChangingDelay, _bool _isLoop = false, _float _fCancleRatio = 1.f);
	_float3	SetDirectionFromCamera(_float _fDgree, _bool _isSoft = true);	// ī�޶� �������� ������ ����
	_bool	PlayAnimationSet();											// �ִϸ��̼� ��Ʈ ���
	void	CancleAnimationSet();										// �ִϸ��̼� ��Ʈ ��� ĵ��
	void	TriggerAnimationSet(string _strSetName, _uint _SetFlag);	// �ִϸ��̼� ��Ʈ ��� Ʈ����
	void	PolymorphControl(_float _fTimeDelta);	// ���� ��Ʈ��

private:
	_uint	m_iSetTrigger	= { false };	// �ִϸ��̼� ��Ʈ ��� �÷��� [0==����, 1==!Loop, 2==Loop]
	_uint	m_iAnimSetCount = { 0 };		// �ִϸ��̼� ��Ʈ ī��Ʈ
	_bool	m_isJumpState	= { false };		// ���� ��������
	_bool	m_isLanding		= { true };	// ��������
	_int	m_iJumpFlag		= { -1 };		// ���� ���� �÷��� [-1==����x, 0==���ڸ�����, 1==�̵�����, 2==��������]
	_bool	m_bChanging		= { false };	// ������� ���� ��,,
	_float	m_fChangeingTime = { 0.f };		// ������� �����ϴµ� �ɸ��� �ð�

	_float			m_fJumpMinActiveTime = { 0.f };	// ���� �ּ� ���� �ð�
	_bool			m_isJumpMinActive = { false };	// ���� �ּ� ���� ��

	vector<tuple<WOLF_ANIMATION, _float, _float>>	m_vecAnimationSet;	// ���� �ִϸ��̼� ��Ʈ

	_bool	m_isMoveState = { false };	// �����̴� ��������
	_bool	m_isSprintState = { false }; // ���� ��������

private:
	ANIMAL_BASESTATE	m_eCurrentBaseState = { STATE_LOOP };		// ���� �� ����
	WOLF_ANIMATION		m_eCurrentAnimation = { ANIMAL_WOLF_IDLE };	// ���� �ִϸ��̼� (��ü or ��ü)

private:
	_int	m_iStepEffectIdx = -1;
	string	m_strStepEffectKey = "Wolf_Dust";
	_int	m_iTestKeyInput = 0;

private:
	// �ִϸ��̼� ���� ���� ��Ʈ�� map<���� �̸�, vector<tuple<�ִϸ��̼�����, �����ð�, ĵ������ > > >
	map<string, vector<tuple<WOLF_ANIMATION, _float, _float > > >	m_AnimationSets;

private:
	HRESULT AddRigidBody(string _strModelKey);		// RigidBody �߰�
	HRESULT MakeAnimationSet();						// �ִϸ��̼� ��Ʈ ���


public:
	static shared_ptr<CWolf> Create();

};

END
