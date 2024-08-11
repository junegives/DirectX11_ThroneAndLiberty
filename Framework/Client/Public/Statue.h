#pragma once

#include "InteractionObj.h"

// õ��		�Ķ�, ����â	
// ���		���, ��		���� �Ⱦ���
// ���		����, ��		ȭ��
// ������	�����ʷ�, ��������		����

enum ST_Type
{
	ST_ENGEL = 0,
	ST_LEEPER,
	ST_BLIND,
	ST_PRIEST,
};

BEGIN(Engine)
class CModel;
class CTexture;
END

BEGIN(Client)

class CStatue : public CInteractionObj
{
public:
	CStatue();
	virtual ~CStatue() = default;

public:
	virtual HRESULT Initialize(_float3 _vPosition);
	virtual void	PriorityTick(_float _fTimeDelta)  override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta)  override;
	virtual HRESULT Render();

public:
	void		SimulationOn();
	void		SimulationOff();

public:
	virtual void InteractionOn();
	void	SetStatueType(_int _iType) { m_iStatueType = _iType; }
	void	SetSpellCollisionAble(_bool _bAble) { m_bSpellCollisionAble = _bAble; }
	void	SetMatchStatue(weak_ptr<CStatue> _pMatchStatue) { m_pMatchStatue = _pMatchStatue; }
	_bool	GetSpellHitted() { return m_bSpellHitted; }
	_bool	GetMatchCorrect() { return m_bMatchCorrect; }

protected:
	HRESULT			AddRigidBody(string _strModelKey);		// RigidBody �߰�
	virtual void	OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ��� ����
	virtual void	OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;	// ��� ���� ��
	virtual void	OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ������

protected:
	_bool			Rotatate45Degree(_float _fTimeDelta);
	void			StatueRotateCount();

protected:
	_uint m_iLightIdx = 0;

public:
	static shared_ptr<CStatue> Create(_float3 _vPosition, _int _iStatueType, _bool _bGenType = false);

protected:
	shared_ptr<CRigidBody>	m_pRigidBody = { nullptr };				// Rigid Body
	string m_strModelName = "";										// Model �̸�

private:
	_int	m_iStatueType = 0;		// ������ Ÿ��

	_float	m_fCalculateTime1 = 0.f;
	_bool	m_bStatueInteractOn = false;		// ��ȣ�ۿ� ����
	_bool	m_bSpellCollisionAble = false;		// �����ϸ� ������ ��ġ�� ����
	_bool	m_bMatchCorrect = false;
	_bool	m_bSpellHitted = false;

	_int	m_iSpinCount = 0;		// ���� ������ �� Ƚ��
	_int	m_iTrueSpinCount = 0;	// ������ ���ƾ��ϴ� Ƚ��

	_float3	m_vRimLightColor = _float3(0.f, 0.f, 0.f);

	weak_ptr<CStatue>		m_pMatchStatue;		// ¦�´� ������
};

END