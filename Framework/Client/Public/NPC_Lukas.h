#pragma once

#include "NPC.h"

// GenType 0: �Ϲ�, 1: ������

// Base ����
enum NPC_Lukas_BaseState
{
	LUKAS_B_BEFOREMEET = 0,			// ��ȣ�ۿ뵵 ���ϰ� �ֶ�
	LUKAS_B_MEET,						// ��ȣ�ۿ��� �������� ��ȣ�ۿ��� ������������ ����ﶧ
	LUKAS_B_INTERACT,					// ��ȣ�ۿ��Ҷ�
};

// �Ÿ��� ���� ����
enum NPC_Lukas_DistanceState
{
	LUKAS_D_FARRANGE = 0,				// �ֶ�
	LUKAS_D_MEETRANGE					// ����ﶧ
};

// Ư�� �׼� ����
enum NPC_Lukas_ActionState
{
	LUKAS_A_RUN = 0,
	LUKAS_A_ANGRY = 2,
	LUKAS_A_IDLE = 6,
	LUKAS_A_TALK1 = 20,
	LUKAS_A_TALK2 = 21,
	LUKAS_A_TALK3 = 22,
	LUKAS_A_TALK4 = 23,
	LUKAS_A_TALK5 = 24,
	LUKAS_A_TURNL130 = 30,
	LUKAS_A_TURNL80 = 31,
	LUKAS_A_TURNR130 = 33,
	LUKAS_A_TURNR80 = 34,
};

BEGIN(Client)

class CNPC_Lukas final : public CNPC
{
public:
	CNPC_Lukas();
	virtual ~CNPC_Lukas() = default;

public:
	HRESULT Initialize(_float3 _vCreatePos);
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<CNPC_Lukas> Create(_float3 _vCreatePos, _int _iGenType = 0, _float3 _vInitLookPos = _float3(0.f, 0.f, 0.f));

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
	_int		m_iBaseState = LUKAS_B_BEFOREMEET;		// Base����
	_int		m_iDistanceState = LUKAS_D_FARRANGE;	// �Ÿ��� ���� ����
	_int		m_iActionState = LUKAS_A_IDLE;			// �ൿ����

	/*Hair Color*/
private:
	_float3 m_vHairMainColor = { 194.f / 255.f, 165.f / 255.f, 90.f / 255.f };
	_float3 m_vHairSubColor = { 0.4f, 0.4f, 0.4f };
};

END