#pragma once

#include "GameObject.h"
#include "Projectile.h"
#include "Client_Defines.h"

BEGIN(Client)

enum SlashType
{
	SLASHLDOWN = 0,					// �� �Ʒ�����
	SLASHLUP,						// �� ������
	SLASHLRIGHT,					// �� ������ ������
	SLASHLRIGHTDOWN,				// �� ������ �Ʒ�����
	SLASHDOWN,						// ª�� �Ʒ�����
	SLASHUP,						// ª�� ������
	SLASHRIGHT,						// ª�� ������ ������
	SLASHBIG						// �Ŵ� ����
};	

class CSlash final: public CProjectile
{
public:
	struct SLASH_DESC
	{
		CProjectile::PROJECTILE_DESC tProjectile;
		_int	m_iSlashType{ 0 };
	};

public:
	CSlash();
	virtual ~CSlash()  override;

public:
	virtual HRESULT Initialize(SLASH_DESC* _pSlashDesc, CTransform::TRANSFORM_DESC* _pTransformDesc);
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
	static shared_ptr<CSlash> Create(SLASH_DESC* _pSlashDesc, CTransform::TRANSFORM_DESC* _pTransformDesc);

private:
	_int		m_iSlashType = 0;
	_float		m_fEnableDistance = 160.f;
};

END