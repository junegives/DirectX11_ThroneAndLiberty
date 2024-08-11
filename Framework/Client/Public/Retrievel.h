#pragma once

#include "NPC.h"

// ������ Ƣ����� �ϱ�(õõ�� ������ �������µ� ��ȣ�ۿ��ϸ� ���� ���Ŀ� �ѹ� ¢�� ����Ŀ� �ٽ� ������ ��)
// ũ�Ⱑ ū��, ������ �ΰ���

enum Retrievel_GenType
{
	RETRIEVEL_G_RUN = 0,
	RETRIEVEL_G_EAT = 1,
};

// Base ����
enum Retrievel_BaseState
{
	RETRIEVEL_B_BEFOREMEET = 0,			// ��ȣ�ۿ뵵 ���ϰ� �ֶ�
	RETRIEVEL_B_MEET,						// ��ȣ�ۿ��� �������� ��ȣ�ۿ��� ������������ ����ﶧ
	RETRIEVEL_B_INTERACT,
};

// �Ÿ��� ���� ����
enum Retrievel_DistanceState
{
	RETRIEVEL_D_FARRANGE = 0,				// �ֶ�
	RETRIEVEL_D_MEETRANGE					// ����ﶧ
};

// Ư�� �׼� ����
enum Retrievel_ActionState
{
	RETRIEVEL_A_IDLE = 0,
	RETRIEVEL_A_CHARM3 = 1,
	RETRIEVEL_A_CHARM2 = 2,
	RETRIEVEL_A_CHARM1 = 3,
	RETRIEVEL_A_EAT3 = 4,
	RETRIEVEL_A_EAT2 = 5,
	RETRIEVEL_A_EAT1 = 6,
	RETRIEVEL_A_TALK = 7,
	RETRIEVEL_A_WALK = 12
};

BEGIN(Client)

class CRetrievel final : public CNPC
{
public:
	CRetrievel();
	virtual ~CRetrievel() = default;

public:
	HRESULT Initialize(_float3 _vCreatePos);
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<CRetrievel> Create(_float3 _vCreatePos, _float3 _vInitLookPos = _float3(0.f,0.f,0.f), _int _iGenType = 0);

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

	_float		m_fInitRotation = 3.14f * 0.2f;

	// �Ÿ�
	_float		m_fMeetRange = 5.f;

	_float		m_fEatIntervel = 2.f;		// �Դ� �ֱ�

	_float		m_fWalkSpeed = 2.f;
	_float		m_fLifeTime = 20.f;			// ����Ⱓ

	_bool		m_bInteractDog = false;
	_bool		m_bFirstInit = true;

private:
	_int		m_iBaseState = RETRIEVEL_B_BEFOREMEET;			// Base����
	_int		m_iDistanceState = RETRIEVEL_D_FARRANGE;	// �Ÿ��� ���� ����
	_int		m_iActionState = RETRIEVEL_A_TALK;			// �ൿ����
};

END