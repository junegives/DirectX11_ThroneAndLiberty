#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

public:
	CModel(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	CModel(const shared_ptr<CModel> _pOrigin);
	virtual ~CModel();

public:
	HRESULT Initialize( const _char* pModelFilePath, _float4x4 PivotMatrix, wstring _strModelName);
	HRESULT InitializeClone();
	HRESULT Render(_uint iMeshIndex);

public:
	TYPE GetModelType() { return m_eModelType; }

public:
	_uint GetNumMeshes() const {
		return m_iNumMeshes;
	}
	_int GetBoneIndex(const _char* _pBoneName) const;

	_float4x4* GetCombinedBoneMatrixPtr(const _char* pBoneName);

	void ChangeAnim(_uint _iAnimIndex, _float _fChangingDelay = 0.1f, _bool _isLoop = false, _float _fCancleRatio = 1.f);
	void ChangeLowerAnim(_uint _iAnimIndex, _float _fChangingDelay = 0.1f, _bool _isLoop = false);

public:
	HRESULT BindMaterialShaderResource(_uint _iMeshIndex, ETEXTURE_TYPE _eMaterialType, const _char* _pConstantName);
	HRESULT BindBoneMatrices(shared_ptr<class CShader> _pShader, const _char* _pConstName, _uint _iMeshIndex);
	void PlayAnimation(_float _fTimeDelta);
	void PlayReferenceAnimation();



public:
	vector<shared_ptr<class CMesh>>		GetMeshes() { return m_Meshes; }
	vector<shared_ptr<class CAnimation>> GetAnimations() { return m_Animations; }
	vector<shared_ptr<class CMaterial>>* GetMaterial() { return &m_Materials; }
	vector<shared_ptr<class CBone>>		GetBones() { return m_Bones; }
	vector<shared_ptr<class CBone>>*	GetpBones() { return &m_Bones; }


	_float4x4* GetPrevBoneMatrix() { return m_pBoneMat; }

public:
	// �ִϸ��̼� ��� ���� ���θ� ��ȯ�մϴ�.[ture==����]. [�ִϸ��̼��� Loop�� ��� ���̸�, ������ true�� ��ȯ]
	_bool	GetIsFinishedAnimation(); 

	// �ִϸ��̼� ĵ�� ���� ���θ� ��ȯ�մϴ�.[ture==ĵ�����ɻ���]. [�ִϸ��̼��� Loop�� ��� ���̸�, ������ true�� ��ȯ]
	_bool	GetCanCancelAnimation();

	// ���� �ִϸ��̼��� ���� ������ ��ȯ�մϴ�.
	_float	GetCurrentAnimRatio();

	// ���� �ִϸ��̼��� ��ȯ�մϴ�.
	shared_ptr<class CAnimation> GetCurAnimation() { return m_Animations[m_iUpperCurrentAnimation]; }

	// �и����� ������ ��ȯ�մϴ�.
	_bool* GetSeparating() { return &m_isSeparating; };

public:
	// �ִϸ��̼� ����� [������/�������] �����մϴ�. [true==���]
	void	SetIsPlayAnimation(_bool _isPlayAnimation) { m_isPlayAnimation = _isPlayAnimation; }

	// ��, ��ü�� �����Ͽ� �ִϸ��̼��� ���� �� �ֵ��� �غ� �մϴ�.
	void	ReadySeparate(string _SeparateBone);

	// ������ ������ Matrix �����͸� ����
	void	ReadyReferenceBones(shared_ptr<CModel> _pReferenceModel);

	// Shape Disable�� ����Ǿ��ִ� �ִϸ��̼��� �ִ��� üũ�ϰ� Disable�� �����ݴϴ�. 
	void	CheckDisableAnim();

private:
	_float4x4 m_PivotMatrix;
	CModel::TYPE m_eModelType = CModel::TYPE::TYPE_END;

private:
	_uint m_iNumMeshes = { 0 };

	_uint m_iNumMaterials = { 0 };
	vector<shared_ptr<class CMaterial>> m_Materials;
	vector<shared_ptr<class CBone>> m_Bones;

	vector<shared_ptr<class CMesh>> m_Meshes;

	_uint m_iNumAnimations			= { 0 };
	_uint m_iUpperCurrentAnimation	= { 0 };		// ���� �ִϸ��̼� [ ��ü �ִϸ��̼� || ��ü �ִϸ��̼� ]
	_uint m_iLowerCurrentAnimation	= { 0 };		// ��ü �ִϸ��̼�

	vector<shared_ptr<class CAnimation>> m_Animations;

private:
	shared_ptr<class CAnimation> m_CurrentAnim = nullptr;
	shared_ptr<class CAnimation> m_NextAnim = nullptr;

	_uint m_RootBoneIdx			= { 0 };
	_bool m_isPlayAnimation		= { true };			// �ִϸ��̼� ����� [������/�������]
	_bool m_isFinishedAnimation	= { false };		// �ִϸ��̼� ����� ��������
	_bool m_isLoopUpper			= { false };		// ���� �ִϸ��̼ǿ� ������ ������
	_bool m_isLoopLower			= { false };		// ���� �ִϸ��̼ǿ� ������ ������

	_uint m_iNumBaseBones	= { 0 };				// ��, ��ü�� �б��� ������ Bone ����
	_uint m_iNumUpperBones	= { 0 };				// Root + ��ü�� Bone ����
	_bool m_isSeparateModel	= { false };			// ��, ��ü�� �и��� �� ����
	_bool m_isSeparating	= { false };			// ��, ��ü�� �и������ ����.

	_uint m_iReserveDisableAnimIndex	= { 0 };
	_bool m_isReserved					= { false };



private:
	_float4x4 m_pBoneMat[MAX_BONE];

private:
	HRESULT ReadyMeshes(ifstream& _ifs);
	HRESULT ReadyMaterials(ifstream& _ifs);
	HRESULT ReadyAnimations(ifstream& _ifs, wstring _strModelName);

public:
	virtual shared_ptr<CComponent> Clone(void* _pArg);

public:
	static shared_ptr<CModel> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext,  const _char* pModelFilePath, wstring _strModelName, _float4x4 PivotMatrix = XMMatrixIdentity());

};

END