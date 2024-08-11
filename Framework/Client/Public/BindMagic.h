#pragma once

#include "GameObject.h"
#include "Projectile.h"
#include "Client_Defines.h"

BEGIN(Client)

enum BindMagicType
{
	BM_T_GUIDED,				// ���� �տ��� ����ź
	BM_T_UPGUIDED,				// ���� �Ӹ������� ����ź
	BM_T_SPINBACK				// ���� ��ڿ��� �����ϱ�
};

class CBindMagic final: public CProjectile
{
public:
	struct BINDMAGIC_DESC
	{
		CProjectile::PROJECTILE_DESC tProjectile;
		shared_ptr<CGameObject> pOwner = nullptr;
	};

public:
	CBindMagic();
	virtual ~CBindMagic()  override;
	void	SetMagicType(_int _iType) { m_iMagicType = _iType; }
	void	SetCircleMagicType(_int _iType) { m_iCircleMagicType = _iType; }

public:
	virtual HRESULT Initialize(BINDMAGIC_DESC* _pBindMagicDesc, CTransform::TRANSFORM_DESC* _pTransformDesc);
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
	static shared_ptr<CBindMagic> Create(BINDMAGIC_DESC* _pBindMagicDesc, CTransform::TRANSFORM_DESC* _pTransformDesc);

private:
	_float		m_fEnableDistance = 160.f;
	_int		m_iMagicType = 0;			// Ÿ��
	_int		m_iCircleMagicType = 0;		// ��ġ Ÿ��
	_float		m_fCircleMagicLength = 2.5f;
	_float		m_fZLength = 2.f;

	_float		m_fCircleRadius = 3.f;		// ���� ���� ������
	_float		m_fCircleRadian = 3.14f / 180.f * 120.f;
	_float		m_fSpinSpeed = 3.f;

	_float		m_fSound = 0.8f;
};

END