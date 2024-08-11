#pragma once

#include "NPC.h"

// Base ����
enum NPC_Davinci_BaseState
{
	DAVINCI_B_BEFOREMEET = 0,			// ��ȣ�ۿ뵵 ���ϰ� �ֶ�
	DAVINCI_B_MEET,						// ��ȣ�ۿ��� �������� ��ȣ�ۿ��� ������������ ����ﶧ
	DAVINCI_B_INTERACT,					// ��ȣ�ۿ��Ҷ�
};

// �Ÿ��� ���� ����
enum NPC_Davinci_DistanceState
{
	DAVINCI_D_FARRANGE = 0,				// �ֶ�
	DAVINCI_D_MEETRANGE					// ����ﶧ
};

// Ư�� �׼� ����
enum NPC_Davinci_ActionState
{
	DAVINCI_A_BASE1 = 1,
	DAVINCI_A_BASE2 = 2,
	DAVINCI_A_IDLE = 7,
	DAVINCI_A_TURNL130 = 35,
	DAVINCI_A_TURNL80 = 36,
	DAVINCI_A_TURNR130 = 37,
	DAVINCI_A_TURNR80 = 38,
	DAVINCI_A_RUN = 20,
	DAVINCI_A_WALK = 41,
};

BEGIN(Client)

class CNPC_Davinci final : public CNPC
{
public:
	CNPC_Davinci();
	virtual ~CNPC_Davinci() = default;

public:
	HRESULT Initialize(_float3 _vCreatePos);
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<CNPC_Davinci> Create(_float3 _vCreatePos, _float3 _vInitLookPos = _float3(0.f, 0.f, 0.f));

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
	_int		m_iBaseState = DAVINCI_B_BEFOREMEET;		// Base����
	_int		m_iDistanceState = DAVINCI_D_FARRANGE;	// �Ÿ��� ���� ����
	_int		m_iActionState = DAVINCI_A_IDLE;			// �ൿ����

	/*Hair Color*/
private:
	_float3 m_vHairMainColor = { 0.02f, 0.02f, 0.07f };
	_float3 m_vHairSubColor = { 0.4f, 0.4f, 0.4f };
};

END