#pragma once

#include "NPC.h"

// Base ����
enum NPC_JaniceCarter_BaseState
{
	JANICECARTER_B_BEFOREMEET = 0,			// ��ȣ�ۿ뵵 ���ϰ� �ֶ�
	JANICECARTER_B_MEET,						// ��ȣ�ۿ��� �������� ��ȣ�ۿ��� ������������ ����ﶧ
	JANICECARTER_B_INTERACT,					// ��ȣ�ۿ��Ҷ�
};

// �Ÿ��� ���� ����
enum NPC_JaniceCarter_DistanceState
{
	JANICECARTER_D_FARRANGE = 0,				// �ֶ�
	JANICECARTER_D_MEETRANGE					// ����ﶧ
};

// Ư�� �׼� ����
enum NPC_JaniceCarter_ActionState
{
	JANICECARTER_A_IDLE = 2,
	JANICECARTER_A_RUN = 9,
	JANICECARTER_A_TALK1 = 33,
	JANICECARTER_A_TALK2 = 34,
	JANICECARTER_A_TALK3 = 35,
	JANICECARTER_A_TALK4 = 36,
	JANICECARTER_A_TALK5 = 37,				// Aesine���ƿ��� ����
	JANICECARTER_A_TURNL130 = 39,
	JANICECARTER_A_TURNL80 = 40,
	JANICECARTER_A_TURNR130 = 41,
	JANICECARTER_A_TURNR80 = 42,
	JANICECARTER_A_WALK = 43,
};

BEGIN(Client)

class CNPC_JaniceCarter final : public CNPC
{
public:
	CNPC_JaniceCarter();
	virtual ~CNPC_JaniceCarter() = default;

public:
	HRESULT Initialize(_float3 _vCreatePos);
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<CNPC_JaniceCarter> Create(_float3 _vCreatePos, _float3 _vInitLookPos = _float3(0.f, 0.f, 0.f));

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
	_int		m_iBaseState = JANICECARTER_B_BEFOREMEET;		// Base����
	_int		m_iDistanceState = JANICECARTER_D_FARRANGE;		// �Ÿ��� ���� ����
	_int		m_iActionState = JANICECARTER_A_IDLE;			// �ൿ����

	/*Hair Color*/
private:
	_float3 m_vHairMainColor = { 0.02f, 0.02f, 0.07f };
	_float3 m_vHairSubColor = { 0.4f, 0.4f, 0.4f };

	_float3 m_vEyeBrowMainColor = { 0.02f, 0.02f, 0.07f };
	_float3 m_vEyeBrowSubColor = { 0.4f, 0.4f, 0.4f };


};

END