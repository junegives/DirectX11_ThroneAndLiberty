#pragma once

#include "NPC.h"

enum Sheep_GenType
{
	SHEEP_G_RUN = 0,
	SHEEP_G_EAT = 1,
};

// Base ����
enum Sheep_BaseState
{
	SHEEP_B_BEFOREMEET = 0,			// ��ȣ�ۿ뵵 ���ϰ� �ֶ�
	SHEEP_B_MEET,						// ��ȣ�ۿ��� �������� ��ȣ�ۿ��� ������������ ����ﶧ
	SHEEP_B_INTERACT,
};

// �Ÿ��� ���� ����
enum Sheep_DistanceState
{
	SHEEP_D_FARRANGE = 0,				// �ֶ�
	SHEEP_D_MEETRANGE					// ����ﶧ
};

// Ư�� �׼� ����
enum Sheep_ActionState
{
	SHEEP_A_JUMPE = 2,
	SHEEP_A_JUMPL = 3,
	SHEEP_A_JUMPRUN = 4,
	SHEEP_A_JUMPS = 5,
	SHEEP_A_EATE = 7,
	SHEEP_A_EATL = 8,
	SHEEP_A_EATS = 9,
	SHEEP_A_IDLE = 16,
	SHEEP_A_WALK = 17,
};

BEGIN(Client)

class CSheep final : public CNPC
{
public:
	CSheep();
	virtual ~CSheep() = default;

public:
	HRESULT Initialize(_float3 _vCreatePos);
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<CSheep> Create(_float3 _vCreatePos, _float3 _vInitLookPos = _float3(0.f,0.f,0.f), _int _iGenType = 0);

public:
	virtual void InteractionOn();
	
protected:
	HRESULT			AddRigidBody(string _strModelKey);		// RigidBody �߰�
	virtual void	OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ��� ����
	virtual void	OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;	// ��� ���� ��
	virtual void	OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ������

	void		UpdateDistanceState();

	// BaseState ���� FSM
	void		FSM_BeforeMeet(_float _fTimeDelta);
	void		FSM_Meet(_float _fTimeDelta);
	void		FSM_Interact(_float _fTimeDelta);

	// �Ÿ�
	_float		m_fMeetRange = 5.f;

	_float		m_fEatIntervel = 2.f;		// �Դ� �ֱ�

	_float		m_fRunSpeed = 5.f;
	_float		m_fLifeTime = 20.f;			// ����Ⱓ

private:
	_int		m_iBaseState = SHEEP_B_BEFOREMEET;		// Base����
	_int		m_iDistanceState = SHEEP_D_FARRANGE;	// �Ÿ��� ���� ����
	_int		m_iActionState = SHEEP_A_IDLE;			// �ൿ����
};

END