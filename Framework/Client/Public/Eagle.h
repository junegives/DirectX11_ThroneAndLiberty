#pragma once
#include "Polymorph.h"

BEGIN(Engine)

END

BEGIN(Client)
class CCameraSpherical;

class CEagle : public CPolymorph
{
public:
	enum ANIMAL_BASESTATE
	{
		STATE_LOOP, STATE_ACTIVE, STATE_END
	};
	enum EAGLE_ANIMATION
	{
		ANIMAL_EAGLE_EMOTION_1		= 0, // o

		ANIMAL_EAGLE_TRANS_START	= 31, // o

		ANIMAL_EAGLE_TRANS_LAND_START	= 16, // 
		ANIMAL_EAGLE_TRANS_LAND_END		= 18, // o

		ANIMAL_EAGLE_TRANS_LAND_DIVE = 2, // o

		ANIMAL_EAGLE_GLIDING_NORMAL			= 19, //
		ANIMAL_EAGLE_GLIDING_NORMAL_UP		= 27, // o
		ANIMAL_EAGLE_GLIDING_NORMAL_L_START = 21, // o
		ANIMAL_EAGLE_GLIDING_NORMAL_L		= 20, //
		ANIMAL_EAGLE_GLIDING_NORMAL_L_UP	= 28, // o
		ANIMAL_EAGLE_GLIDING_NORMAL_R_START = 23, // o
		ANIMAL_EAGLE_GLIDING_NORMAL_R		= 22, //
		ANIMAL_EAGLE_GLIDING_NORMAL_R_UP	= 29, // o

		ANIMAL_EAGLE_GLIDING_NORMAL_TO_FAST = 26, // o
		ANIMAL_EAGLE_GLIDING_FAST_TO_NORMAL	= 11, // o

		ANIMAL_EAGLE_GLIDING_FAST			= 6, //
		ANIMAL_EAGLE_GLIDING_FAST_UP		= 12, // o
		ANIMAL_EAGLE_GLIDING_FAST_L_START	= 8, // o
		ANIMAL_EAGLE_GLIDING_FAST_L			= 7, //
		ANIMAL_EAGLE_GLIDING_FAST_L_UP		= 13, // o
		ANIMAL_EAGLE_GLIDING_FAST_R_START	= 10, // o
		ANIMAL_EAGLE_GLIDING_FAST_R			= 9, //
		ANIMAL_EAGLE_GLIDING_FAST_R_UP		= 14, // o

		ANIMAL_EAGLE_GLIDING_NORMAL_TO_DIVE	= 25, // o
		ANIMAL_EAGLE_GLIDING_DIVE			= 1, //
		ANIMAL_EAGLE_GLIDING_DIVE_TO_NORMAL = 5, // o
	};

public:
	CEagle();
	virtual ~CEagle()  override;

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
	_bool	EmotionControl();						// ����ǥ�� ����
	_bool	SprintControl(_float _fTimeDelta);		// ���� ����
	_bool	DiveControl(_float _fTimeDelta);		// ���� ����
	_bool	DirectionControl(_float _fTimeDelta);	// ���� ����
	void	GravityControl(_float _fTimeDelta);		// �߷� ����
	void	EffectControl();						// ����Ʈ ����

	// ���� ���� 
	void	CommonTick(_float _fTimeDelta);
	void	ChangeAnim(_uint _iAnimIndex, _float _fChangingDelay, _bool _isLoop = false, _float _fCancleRatio = 1.f);
	_float3	SetDirectionFromCamera(_float _fDgree, _bool _isSoft = true);	// ī�޶� �������� ������ ����
	_bool	PlayAnimationSet();											// �ִϸ��̼� ��Ʈ ���
	void	CancleAnimationSet();										// �ִϸ��̼� ��Ʈ ��� ĵ��
	void	TriggerAnimationSet(string _strSetName, _uint _SetFlag);	// �ִϸ��̼� ��Ʈ ��� Ʈ����
	void	PolymorphControl(_float _fTimeDelta);	// ���� ��Ʈ��

private:
	_uint	m_iSetTrigger		= { false };	// �ִϸ��̼� ��Ʈ ��� �÷��� [0==����, 1==!Loop, 2==Loop]
	_uint	m_iAnimSetCount		= { 0 };		// �ִϸ��̼� ��Ʈ ī��Ʈ
	_bool	m_bChanging			= { false };	// ���� ��,,
	_float	m_fChangeingTime	= { 0.f };		// ���� �ð�
	_bool	m_isSprint			= { false };	// ��Ӻ������
	_bool	m_isPrevSprint		= { false };	// ���� ��Ӻ������
	_bool	m_isDive			= { false };	// �ϰ�����
	_bool	m_isPrevDive		= { false };	// ���� �ϰ�����
	_float	m_fDiveFrontSpeed	= { 11.f };		// �ϰ� ��, ���� �ӵ� ������
	_uint	m_iDirection		= { 1 };	// ������� [0==��ȸ��, 1==����, 2==��ȸ��]


	_uint	m_iLanded = { 0 };		// 0:����, 1:�Ϲ�����, 2:���̺�����

	// �߷� ����
	_float	m_fUpGravity		= { -4.f };		// ��½� �ӵ� 
	_float	m_fCurrentGravity	= { 0.f };		// ���� �ϰ��ӵ� 
	_float	m_fGravityAccel		= { 0.8f };		// �߷� ���ӵ�
	
	vector<tuple<EAGLE_ANIMATION, _float, _float>>	m_vecAnimationSet;	// ���� �ִϸ��̼� ��Ʈ

private:
	ANIMAL_BASESTATE	m_eCurrentBaseState = { STATE_LOOP };		// ���� �� ����
	EAGLE_ANIMATION		m_eCurrentAnimation = { ANIMAL_EAGLE_TRANS_START };	// ���� �ִϸ��̼� (��ü or ��ü)

private:
	_int	m_iStepEffectIdx = -1;
	string	m_strStepEffectKey = "Eagle_Leave";
	_int	m_iStepEffectTest = 0;

private:
	// �ִϸ��̼� ���� ���� ��Ʈ�� map<���� �̸�, vector<tuple<�ִϸ��̼�����, �����ð�, ĵ������ > > >
	map<string, vector<tuple<EAGLE_ANIMATION, _float, _float > > >	m_AnimationSets;

private:
	HRESULT AddRigidBody(string _strModelKey);		// RigidBody �߰�
	HRESULT MakeAnimationSet();						// �ִϸ��̼� ��Ʈ ���


public:
	static shared_ptr<CEagle> Create();

};

END
