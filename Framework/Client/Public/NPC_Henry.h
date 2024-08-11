#pragma once

#include "NPC.h"

// Base ����
enum NPC_Henry_BaseState
{
	HENRY_B_BEFOREMEET = 0,			// ��ȣ�ۿ뵵 ���߰� �ֶ�
	HENRY_B_ALERT,					// ��ȣ�ۿ��� �������� ����ﶧ
	HENRY_B_READY,					// ��ȣ�ۿ��� �������� ��ȣ�ۿ��� ������������ ����ﶧ
	HENRY_B_NORMAL,					// ��ȣ�ۿ��Ҷ�
};

// �Ÿ��� ���� ����
enum NPC_Henry_DistanceState
{
	HENRY_D_FARRANGE = 0,			// �ֶ�
	HENRY_D_MEETRANGE,				// ����ﶧ
	HENRY_D_READYRANGE,				// �ſ� ����ﶧ
};

// Ư�� �׼� ����
enum NPC_Henry_ActionState
{
	HENRY_A_IDLE = 4,
	HENRY_A_RUN = 5,
	HENRY_A_TURNL130 = 14,
	HENRY_A_TURNL80 = 15,
	HENRY_A_TURNR130 = 16,
	HENRY_A_TURNR80 = 17,
	HENRY_A_WALK = 19,
};

BEGIN(Client)

class CNPC_Henry final : public CNPC
{
public:
	CNPC_Henry();
	virtual ~CNPC_Henry() = default;

public:
	HRESULT Initialize(_float3 _vCreatePos);
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<CNPC_Henry> Create(_float3 _vCreatePos);

public:
	virtual void InteractionOn();

protected:
	HRESULT			AddRigidBody(string _strModelKey);		// RigidBody �߰�
	virtual void	OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ��� ����
	virtual void	OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;	// ��� ���� ��
	virtual void	OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ������

	HRESULT		AddPartObjects(string _strModelKey);
	void		UpdateDistanceState();

	// BaseState ���� FSM
	void		FSM_BeforeMeet(_float _fTimeDelta);
	void		FSM_Meet(_float _fTimeDelta);
	void		FSM_Ready(_float _fTimeDelta);
	void		FSM_Normal(_float _fTimeDelta);

	// �Ÿ�
	_float		m_fMeetRange = 10.f;
	_float		m_fCloseRange = 3.f;

private:
	_int		m_iBaseState = HENRY_B_BEFOREMEET;		// Base����
	_int		m_iDistanceState = HENRY_D_FARRANGE;	// �Ÿ��� ���� ����
	_int		m_iActionState = HENRY_A_IDLE;			// �ൿ����

	/*Hair Color*/
private:
	_float3 m_vHairMainColor = { 194.f / 255.f, 165.f / 255.f, 90.f / 255.f };
	_float3 m_vHairSubColor = { 0.4f, 0.4f, 0.4f };
};

END