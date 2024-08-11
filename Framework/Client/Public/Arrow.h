#pragma once

#include "GameObject.h"
#include "Projectile.h"
#include "Client_Defines.h"

BEGIN(Client)


class CArrow final: public CProjectile
{
public:
	struct ARROW_DESC
	{
		CProjectile::PROJECTILE_DESC tProjectile;
		_int	m_iArrowType;
	};

public:
	CArrow();
	virtual ~CArrow()  override;

public:
	virtual HRESULT Initialize(ARROW_DESC* _pArrowDesc, CTransform::TRANSFORM_DESC* _pTransformDesc);
	virtual void	PriorityTick(_float _fTimeDelta)  override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta)  override;
	virtual HRESULT Render()  override;

	void	SetDeleteKey(_int _iDeleteKey) { m_iDeleteKey = _iDeleteKey; }

protected:
	HRESULT		AddRigidBody();		// RigidBody �߰�
	virtual void OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ��� ����
	virtual void OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;	// ��� ���� ��
	virtual void OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ������

public:
	static shared_ptr<CArrow> Create(ARROW_DESC* _pArrowDesc, CTransform::TRANSFORM_DESC* _pTransformDesc);

private:
	_int	m_iDeleteKey = 0;
};

END