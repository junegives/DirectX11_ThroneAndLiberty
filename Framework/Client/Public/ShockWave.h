#pragma once

#include "GameObject.h"
#include "Projectile.h"
#include "Client_Defines.h"

BEGIN(Client)


class CShockWave final: public CProjectile
{
public:
	struct SHOCKWAVE_DESC
	{
		CProjectile::PROJECTILE_DESC tProjectile;
	};

public:
	CShockWave();
	virtual ~CShockWave()  override;

public:
	virtual HRESULT Initialize(SHOCKWAVE_DESC* _pShockWaveDesc, CTransform::TRANSFORM_DESC* _pTransformDesc);
	virtual void	PriorityTick(_float _fTimeDelta)  override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta)  override;
	virtual HRESULT Render()  override;

protected:
	HRESULT		AddRigidBody();		// RigidBody �߰�
	virtual void OnContactFound(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ��� ����
	virtual void OnContactPersist(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;	// ��� ���� ��
	virtual void OnContactLost(const COLLISIONDATA& _ColData, const string& _szMyShapeTag) override;		// ������

public:
	static shared_ptr<CShockWave> Create(SHOCKWAVE_DESC* _pShockWaveDesc, CTransform::TRANSFORM_DESC* _pTransformDesc);

private:
	_float	m_fCalculateTime = 0.f;			// ���� Ÿ�̸�

	_float	m_fCollisionTime = 1.f;
};

END