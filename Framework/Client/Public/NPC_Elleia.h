#pragma once

#include "NPC.h"

// Base ����
enum NPC_Elleia_BaseState
{
	ELLEIA_B_BEFOREMEET = 0,			// ��ȣ�ۿ뵵 ���ϰ� �ֶ�
	ELLEIA_B_MEET,						// ��ȣ�ۿ��� �������� ��ȣ�ۿ��� ������������ ����ﶧ
	ELLEIA_B_INTERACT,					// ��ȣ�ۿ��Ҷ�
};

// �Ÿ��� ���� ����
enum NPC_Elleia_DistanceState
{
	ELLEIA_D_FARRANGE = 0,				// �ֶ�
	ELLEIA_D_MEETRANGE					// ����ﶧ
};

// Ư�� �׼� ����
enum NPC_Elleia_ActionState
{
	ELLEIA_A_BASE = 0,
	ELLEIA_A_TALK1 = 1,
	ELLEIA_A_TALK2 = 2,
	ELLEIA_A_TALK3 = 3,
	ELLEIA_A_TALK4 = 4,
	ELLEIA_A_TALK5 = 5,
	ELLEIA_A_IDLE = 15,
	ELLEIA_A_SOCLOOPINGE = 19,
	ELLEIA_A_SOCLOOPINGL = 20,
	ELLEIA_A_SOCLOOPINGS = 21,
	ELLEIA_A_TURNL130 = 22,
	ELLEIA_A_TURNL80 = 23,
	ELLEIA_A_TURNR130 = 24,
	ELLEIA_A_TURNR80 = 25
};

BEGIN(Client)

class CNPC_Elleia final : public CNPC
{
public:
	CNPC_Elleia();
	virtual ~CNPC_Elleia() = default;

public:
	HRESULT Initialize(_float3 _vCreatePos);
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<CNPC_Elleia> Create(_float3 _vCreatePos, _float3 _vInitLookPos = _float3(0.f, 0.f, 0.f));

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
	_int		m_iBaseState = ELLEIA_B_BEFOREMEET;		// Base����
	_int		m_iDistanceState = ELLEIA_D_FARRANGE;	// �Ÿ��� ���� ����
	_int		m_iActionState = ELLEIA_A_SOCLOOPINGL;	// �ൿ����
	
	/*Hair Color*/
private:
	_float3 m_vHairMainColor = { 194.f / 255.f, 165.f / 255.f, 90.f / 255.f };
	_float3 m_vHairSubColor = { 0.4f, 0.4f, 0.4f };
};

END