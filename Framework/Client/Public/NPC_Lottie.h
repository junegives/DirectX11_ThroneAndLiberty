#pragma once

#include "NPC.h"

// Base ����
enum NPC_Lottie_BaseState
{
	LOTTIE_B_BEFOREMEET = 0,			// ��ȣ�ۿ뵵 ���ϰ� �ֶ�
	LOTTIE_B_MEET,						// ��ȣ�ۿ��� �������� ��ȣ�ۿ��� ������������ ����ﶧ
	LOTTIE_B_INTERACT,					// ��ȣ�ۿ��Ҷ�
};

// �Ÿ��� ���� ����
enum NPC_Lottie_DistanceState
{
	LOTTIE_D_FARRANGE = 0,				// �ֶ�
	LOTTIE_D_MEETRANGE					// ����ﶧ
};

// Ư�� �׼� ����
enum NPC_Lottie_ActionState
{
	LOTTIE_A_RUN = 0,
	LOTTIE_A_TALKEND = 2,
	LOTTIE_A_TALK1 = 3,
	LOTTIE_A_IDLE = 9,
	LOTTIE_A_LOOKSKY2 = 18,
	LOTTIE_A_LOOKSKY1 = 20,
	LOTTIE_A_IDLE2 = 26,
	LOTTIE_A_TURNL130 = 28,
	LOTTIE_A_TURNL80 = 29,
	LOTTIE_A_TURNR130 = 30,
	LOTTIE_A_TURNR80 = 31,
	LOTTIE_A_WALK = 33,
};

BEGIN(Client)

class CNPC_Lottie final : public CNPC
{
public:
	CNPC_Lottie();
	virtual ~CNPC_Lottie() = default;

public:
	HRESULT Initialize(_float3 _vCreatePos);
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render()  override;

public:
	static shared_ptr<CNPC_Lottie> Create(_float3 _vCreatePos, _float3 _vInitLookPos = _float3(0.f, 0.f, 0.f), _float3 _vCheckPoint1 = _float3(0.f, 0.f, 0.f),
		_float3 _vCheckPoint2 = _float3(0.f, 0.f, 0.f), _float3 _vCheckPoint3 = _float3(0.f, 0.f, 0.f));

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
	_int		m_iBaseState = LOTTIE_B_BEFOREMEET;		// Base����
	_int		m_iDistanceState = LOTTIE_D_FARRANGE;	// �Ÿ��� ���� ����
	_int		m_iActionState = LOTTIE_A_IDLE;			// �ൿ����

	/*Hair Color*/
private:
	_float3 m_vHairMainColor = { 194.f / 255.f, 64.f / 255.f, 33.f / 255.f };
	_float3 m_vHairSubColor = { 0.4f, 0.4f, 0.4f };

	_float3 m_vEyeBrowMainColor = { 0.02f, 0.02f, 0.07f };
	_float3 m_vEyeBrowSubColor = { 0.4f, 0.4f, 0.4f };

	_float3	m_vCheckPoint1 = _float3(0.f, 0.f, 0.f);
	_float3	m_vCheckPoint2 = _float3(0.f, 0.f, 0.f);
	_float3	m_vCheckPoint3 = _float3(0.f, 0.f, 0.f);

};

END