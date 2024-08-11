#pragma once

#include "NPC.h"

// GenType
enum NPC_Soldier_GenType
{
	SOLDIER_G_WALK200 = 0,		// ��Ӽ�����
	SOLDIER_G_WALK50,			// ���Ӽ���
	SOLDIER_G_PRACTICE,			// �Ʒ�
	SOLDIER_G_TALK,				// ��ȭ1
	SOLDIER_G_LAUGH,			// ����
	SOLDIER_G_IDLE				// ���ֱ�			
};

// Ư�� �׼� ����
enum NPC_Soldier_ActionState
{
	SOLDIER_A_WALK200 = 0,
	SOLDIER_A_WALK50 = 1,
	SOLDIER_A_ATT1 = 3,
	SOLDIER_A_ATT2 = 4,
	SOLDIER_A_ATT3 = 5,
	SOLDIER_A_ATT4 = 6,
	SOLDIER_A_ATT5 = 7,
	SOLDIER_A_ALT = 8,
	SOLDIER_A_TALK3 = 9,
	SOLDIER_A_TALK2 = 10,
	SOLDIER_A_TALK1 = 11,
	SOLDIER_A_IDLE = 13,
	SOLDIER_A_LAUGH3 = 14,
	SOLDIER_A_LAUGH2 = 15,
	SOLDIER_A_LAUGH1 = 16
};

BEGIN(Client)

class CNPC_Soldier final : public CNPC
{
public:
	CNPC_Soldier();
	virtual ~CNPC_Soldier() = default;

public:
	HRESULT Initialize(_float3 _vCreatePos);
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<CNPC_Soldier> Create(_float3 _vCreatePos, _float3 _vInitLookPos = _float3(0.f, 0.f, 0.f),
		_int _iGenType = 0);

public:
	virtual void InteractionOn();
	void	SetPatrolPos(_float3 _vPatrolPos) { m_vPatrolPos = _vPatrolPos; }
	
protected:
	HRESULT			AddRigidBody(string _strModelKey);		// RigidBody �߰�
	virtual void	OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ��� ����
	virtual void	OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;	// ��� ���� ��
	virtual void	OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ������

	HRESULT		AddPartObjects(string _strModelKey);

	// GenType�� ���� FSM
	void		FSM_Walk1(_float _fTimeDelta);
	void		FSM_Walk2(_float _fTimeDelta);
	void		FSM_Practice(_float _fTimeDelta);
	void		FSM_Talk(_float _fTimeDelta);
	void		FSM_Laugh(_float _fTimeDelta);
	void		FSM_Idle(_float _fTimeDelta);
	
	// �Ÿ�
	_float		m_fMeetRange = 3.f;

private:
	_int		m_iActionState = SOLDIER_A_LAUGH1;			// �ൿ����

	_float3		m_vPatrolPos = _float3(0.f, 0.f, 0.f);			// ��������
	_bool		m_bPatrolDirection = true;						// ���� ����

	/*Hair Color*/
private:
	_float3 m_vHairMainColor = { 0.02f, 0.02f, 0.07f };
	_float3 m_vHairSubColor = { 0.4f, 0.4f, 0.4f };
};

END