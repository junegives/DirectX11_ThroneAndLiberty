#pragma once

#include "NPC.h"

enum Rabbit_GenType
{
	RABBIT_G_RUN = 0,
	RABBIT_G_EAT = 1,
};

// Base ����
enum Rabbit_BaseState
{
	RABBIT_B_BEFOREMEET = 0,			// ��ȣ�ۿ뵵 ���ϰ� �ֶ�
	RABBIT_B_MEET,						// ��ȣ�ۿ��� �������� ��ȣ�ۿ��� ������������ ����ﶧ
	RABBIT_B_INTERACT,
};

// �Ÿ��� ���� ����
enum Rabbit_DistanceState
{
	RABBIT_D_FARRANGE = 0,				// �ֶ�
	RABBIT_D_MEETRANGE					// ����ﶧ
};

// Ư�� �׼� ����
enum Rabbit_ActionState
{
	RABBIT_A_GLIDING = 7,
};

BEGIN(Client)

class CRabbit final : public CNPC
{
public:
	CRabbit();
	virtual ~CRabbit() = default;

public:
	HRESULT Initialize(_float3 _vCreatePos);
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<CRabbit> Create(_float3 _vCreatePos, _float3 _vInitLookPos = _float3(0.f,0.f,0.f), _int _iGenType = 0);

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

	_float		m_fLifeTime = 10.f;			// ����Ⱓ

private:
	_int		m_iBaseState = RABBIT_B_BEFOREMEET;		// Base����
	_int		m_iDistanceState = RABBIT_D_FARRANGE;	// �Ÿ��� ���� ����
	_int		m_iActionState = RABBIT_A_GLIDING;			// �ൿ����
};

END