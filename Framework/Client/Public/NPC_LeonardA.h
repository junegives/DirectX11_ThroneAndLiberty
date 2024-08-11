#pragma once

#include "NPC.h"

// Base ����
enum NPC_LeonardA_BaseState
{
	LEONARD_B_BEFOREMEET = 0,			// ��ȣ�ۿ뵵 ���ϰ� �ֶ�
	LEONARD_B_MEET,						// ��ȣ�ۿ��� �������� ��ȣ�ۿ��� ������������ ����ﶧ
	LEONARD_B_INTERACT,					// ��ȣ�ۿ��Ҷ�
};

// �Ÿ��� ���� ����
enum NPC_LeonardA_DistanceState
{
	LEONARD_D_FARRANGE = 0,				// �ֶ�
	LEONARD_D_MEETRANGE					// ����ﶧ
};

// Ư�� �׼� ����
enum NPC_LeonardA_ActionState
{
	LEONARD_A_WALK = 7,
	LEONARD_A_RUN = 8,
	LEONARD_A_SOCLOOPINGE = 22,
	LEONARD_A_SOCLOOPINGL = 23,
	LEONARD_A_SOCLOOPINGS = 26,
	LEONARD_A_IDLE = 30,
	LEONARD_A_TURNL130 = 31,
	LEONARD_A_TURNL80 = 32,
	LEONARD_A_TURNR130 = 33,
	LEONARD_A_TURNR80 = 34,
	LEONARD_A_TALKE = 47,
	LEONARD_A_TALKL = 48,
	LEONARD_A_TALKS = 49,
};

BEGIN(Client)

class CNPC_LeonardA final : public CNPC
{
public:
	CNPC_LeonardA();
	virtual ~CNPC_LeonardA() = default;

public:
	HRESULT Initialize(_float3 _vCreatePos);
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<CNPC_LeonardA> Create(_float3 _vCreatePos, _float3 _vInitLookPos = _float3(0.f, 0.f, 0.f));

public:
	virtual void InteractionOn();

protected:
	HRESULT			AddRigidBody(string _strModelKey);		// RigidBody �߰�
	virtual void	OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ��� ����
	virtual void	OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;	// ��� ���� ��
	virtual void	OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ������

	//HRESULT		AddPartObjects(string _strModelKey);
	void		UpdateDistanceState();

	// BaseState ���� FSM
	void		FSM_BeforeMeet(_float _fTimeDelta);
	void		FSM_Meet(_float _fTimeDelta);
	void		FSM_Interact(_float _fTimeDelta);

	// �Ÿ�
	_float		m_fMeetRange = 3.f;

private:
	_int		m_iBaseState = LEONARD_B_BEFOREMEET;		// Base����
	_int		m_iDistanceState = LEONARD_D_FARRANGE;	// �Ÿ��� ���� ����
	_int		m_iActionState = LEONARD_A_IDLE;			// �ൿ����

	/*Hair Color*/
private:
	_float3 m_vHairMainColor = { 0.02f, 0.02f, 0.07f };
	_float3 m_vHairSubColor = { 0.4f, 0.4f, 0.4f };
};

END