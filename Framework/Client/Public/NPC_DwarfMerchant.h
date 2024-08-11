#pragma once

#include "NPC.h"

// GenType
enum NPC_DwarfMerchant_GenType
{
	DWARFMERCHANT_G_IDLE = 0,
	DWARFMERCHANT_G_LAUGH,
	DWARFMERCHANT_G_TALK,
	DWARFMERCHANT_G_WALK,
};

// Ư�� �׼� ����
enum NPC_DwarfMerchant_ActionState
{
	DWARFMERCHANT_A_IDLE = 0,
	DWARFMERCHANT_A_LAUGH = 1,
	DWARFMERCHANT_A_TALK = 2,
	DWARFMERCHANT_A_WALK = 3,
};

BEGIN(Client)

class CNPC_DwarfMerchant final : public CNPC
{
public:
	CNPC_DwarfMerchant();
	virtual ~CNPC_DwarfMerchant() = default;

public:
	HRESULT Initialize(_float3 _vCreatePos);
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<CNPC_DwarfMerchant> Create(_float3 _vCreatePos, _float3 _vInitLookPos = _float3(0.f, 0.f, 0.f),
		_int _iGenType = 0);

public:
	virtual void InteractionOn();
	void	SetPatrolPos(_float3 _vPatrolPos) { m_vPatrolPos = _vPatrolPos; }

protected:
	HRESULT			AddRigidBody(string _strModelKey);		// RigidBody �߰�
	virtual void	OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ��� ����
	virtual void	OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;	// ��� ���� ��
	virtual void	OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ������

	// GenType�� ���� FSM
	void		FSM_BeforeMeet(_float _fTimeDelta);
	void		FSM_Meet(_float _fTimeDelta);
	void		FSM_Walk(_float _fTimeDelta);
	void		FSM_Interact(_float _fTimeDelta);
	
	// �Ÿ�
	_float		m_fMeetRange = 3.f;

private:
	_int		m_iActionState = DWARFMERCHANT_A_IDLE;			// �ൿ����

	_float3		m_vPatrolPos = _float3(0.f, 0.f, 0.f);			// ��������
	_bool		m_bPatrolDirection = true;						// ���� ����

	/*Hair Color*/
private:
	_float3 m_vHairMainColor = { 0.02f, 0.02f, 0.07f };
	_float3 m_vHairSubColor = { 0.4f, 0.4f, 0.4f };
};

END