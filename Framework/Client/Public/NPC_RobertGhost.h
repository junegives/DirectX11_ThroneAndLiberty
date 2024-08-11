#pragma once

#include "NPC.h"

// Base ����
enum NPC_RobertGhost_BaseState
{
	ROBERT_G_B_BEFOREMEET = 0,			// ��ȣ�ۿ뵵 ���ϰ� �ֶ�
	ROBERT_G_B_MEET,						// ��ȣ�ۿ��� �������� ��ȣ�ۿ��� ������������ ����ﶧ
	ROBERT_G_B_INTERACT,					// ��ȣ�ۿ��Ҷ�
};

// �Ÿ��� ���� ����
enum NPC_RobertGhost_DistanceState
{
	ROBERT_G_D_FARRANGE = 0,				// �ֶ�
	ROBERT_G_D_MEETRANGE					// ����ﶧ
};

// Ư�� �׼� ����
enum NPC_RobertGhost_ActionState
{
	ROBERT_G_A_BASE = 5,
	ROBERT_G_A_RUN = 16,
	ROBERT_G_A_IDLE = 26,
	ROBERT_G_A_TALK1 = 28,
	ROBERT_G_A_TALK2 = 29,
	ROBERT_G_A_TALK3 = 30,
	ROBERT_G_A_TALK4 = 31,
	ROBERT_G_A_TALK5 = 32,
	ROBERT_G_A_TURNL130 = 40,
	ROBERT_G_A_TURNL80 = 41,
	ROBERT_G_A_TURNR130 = 42,
	ROBERT_G_A_TURNR80 = 43,
	ROBERT_G_A_WALK = 45,
};

BEGIN(Client)

class CNPC_RobertGhost final : public CNPC
{
public:
	CNPC_RobertGhost();
	virtual ~CNPC_RobertGhost() = default;

public:
	HRESULT Initialize(_float3 _vCreatePos);
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<CNPC_RobertGhost> Create(_float3 _vCreatePos, _float3 _vInitLookPos = _float3(0.f, 0.f, 0.f));

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
	void		FSM_Interact(_float _fTimeDelta);

	// �Ÿ�
	_float		m_fMeetRange = 3.f;

private:
	_int		m_iBaseState = ROBERT_G_B_BEFOREMEET;		// Base����
	_int		m_iDistanceState = ROBERT_G_D_FARRANGE;	// �Ÿ��� ���� ����
	_int		m_iActionState = ROBERT_G_A_IDLE;			// �ൿ����

	/*Hair Color*/
private:
	_float3 m_vHairMainColor = { 0.02f, 0.02f, 0.07f };
	_float3 m_vHairSubColor = { 0.4f, 0.4f, 0.4f };

	_float3 m_vFurMainColor = { 184.f / 255.f, 188.f / 255.f, 195.f / 255.f };
	_float3 m_vFurSubColor = { 0.4f, 0.4f, 0.4f };
};

END