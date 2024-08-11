#pragma once

#include "NPC.h"

// Base ����
enum NPC_Ashien_BaseState
{
	ASHIEN_B_BEFOREMEET = 0,			// ��ȣ�ۿ뵵 ���ϰ� �ֶ�
	ASHIEN_B_MEET,						// ��ȣ�ۿ��� �������� ��ȣ�ۿ��� ������������ ����ﶧ
	ASHIEN_B_INTERACT,					// ��ȣ�ۿ��Ҷ�
};

// �Ÿ��� ���� ����
enum NPC_Ashien_DistanceState
{
	ASHIEN_D_FARRANGE = 0,				// �ֶ�
	ASHIEN_D_MEETRANGE					// ����ﶧ
};

// Ư�� �׼� ����
enum NPC_Ashien_ActionState
{
	ASHIEN_A_TALK1 = 0,
	ASHIEN_A_TALK2 = 1,
	ASHIEN_A_TALK3 = 2,
	ASHIEN_A_TALK4 = 3,			// ���ư���
	ASHIEN_A_TALK5 = 4,
	ASHIEN_A_IDLE = 13,
	ASHIEN_A_TURNL130 = 14,
	ASHIEN_A_TURNL80 = 15,
	ASHIEN_A_TURNR130 = 16,
	ASHIEN_A_TURNR80 = 17
};

BEGIN(Client)

class CNPC_Ashien final : public CNPC
{
public:
	CNPC_Ashien();
	virtual ~CNPC_Ashien() = default;

public:
	HRESULT Initialize(_float3 _vCreatePos);
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<CNPC_Ashien> Create(_float3 _vCreatePos, _float3 _vInitLookPos = _float3(0.f,0.f,0.f));

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
	_int		m_iBaseState = ASHIEN_B_BEFOREMEET;		// Base����
	_int		m_iDistanceState = ASHIEN_D_FARRANGE;	// �Ÿ��� ���� ����
	_int		m_iActionState = ASHIEN_A_IDLE;			// �ൿ����

	/*Hair Color*/
private:
	_float3 m_vHairMainColor = { 194.f / 255.f, 165.f / 255.f, 90.f / 255.f };
	_float3 m_vHairSubColor = { 0.4f, 0.4f, 0.4f };
};

END