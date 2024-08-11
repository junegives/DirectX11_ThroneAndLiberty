#pragma once

/* =============================================== */
//	[ShaderHandler]
// 
//  ���̴� ��ü ���� �� Render �� �� Bind�� Begin ���� ������
//	�������ִ� �ڵ鷯
//
//  ��ü���� ���̴� ��ü�� ���� ��� �ִ°� �ƴ�, ���̴� Ÿ�Ը� ��� �ְ� �Ѵ�  
//  ���� �ƴ� ���������� ����� �� �ִ� Buffer(VIRect, VIPoint..)�� �ڵ����� �������� ���ش�
//  ���� ��� �� �����͸� �״�� �Ѱ� �̹��� ���ε��� �ڵ����� ���ְ� ���� ȣ����� ���ش� 
//
/* =============================================== */


#include "Engine_Defines.h"

BEGIN(Engine)

class CShaderHandler final
{

public:
	CShaderHandler(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	~CShaderHandler() = default;

public:
	HRESULT Initialize(_float _fScreenX, _float _fScreenY);

	HRESULT ReadyComputeShader();

/*Bind Shader*/
public:
	/*Bind ���� �ʱ� Shader ����*/
	HRESULT ReadyShader(ESHADER_TYPE _eShderType, _uint iPassIndex, _uint iTechniqueIndex = 0);
	
	/*���� ������� �ʴ� ��ü ���� Begin*/
	HRESULT BeginShader();

	/*For. Compute Shader*/
	HRESULT BeginCS(_uint _iThreadX, _uint _iThreadY, _uint _iThreadZ);

	/*�ִϸ��̼� �� ���� Begin*/
	HRESULT BeginAnimModel(shared_ptr<class CModel> _pModel , _uint _iMeshNum);

	/*�ִϸ��̼� �� & �� ���� Begin */
	HRESULT BeginAnimModelBone(shared_ptr<class CModel> _pModel, _float4x4* _BoneMatrices, _uint _iMeshNum);

	/*��ִ� �� ���� Begin*/
	HRESULT BeginNonAnimModel(shared_ptr<class CModel> _pModel, _uint _iMeshNum);

	/*��ִ� �ν��Ͻ� �� ���� Begin*/
	HRESULT BeginNonAnimInstance(shared_ptr<class CInstancingModel> _pModel, _uint _iMeshNum);

	/*�ͷ��� ���� Ư�� ����(�̸� ���� �Ұ����� �͵�) ���� Begin*/
	HRESULT BeginShaderBuffer(shared_ptr<class CVIBuffer> _pVIBuffer);

public:
	HRESULT BindWVPMatrixPerspective(_float4x4 _worldMat);
	HRESULT BindWVPMatrixOrthographic(_float4x4 _worldMat);
	HRESULT BindRawValue(const _char* _pConstantName, const void* _pData, _uint _iLength);
	HRESULT BindMatrix(const _char* _pConstantName, const _float4x4* _pMatrix);
	HRESULT BindMatrices(const _char* _pConstantName, const _float4x4* _pMatrices, _uint _iNumMatrices);
	HRESULT BindSRV(const _char* _pConstantName, string _strTexTag);
	HRESULT BindSRVDirect(const _char* _pConstantName, wrl::ComPtr<ID3D11ShaderResourceView> _pSRV);
	HRESULT BindFloatVectorArray(const _char* _pConstantName, const _float4* _pVectors, _uint _iNumVectors);
	HRESULT BindUAV(const _char* _pConstantName, wrl::ComPtr<ID3D11UnorderedAccessView> _pUAV);

public:
	HRESULT BindHDRCube();
	HRESULT BindLightProjMatrix();
	HRESULT BindLightVPMatrix();

public:
	/*�߰��� Pass Index�� �ٲ�� �� �� ���*/
	void ChangePassIdx(_uint _iPassIdx) { m_iCurrentPassIdx = _iPassIdx; }

/*���̴� �ε�*/
private:
	void ReadShaderFile(const wstring& _strBasePath);
	void ClassificationShaderFile(const wstring& _strFileName, const wstring& _strTotalPath);

/*�������� ����ϴ� ���� ��ü Create*/
private:
	void CreateBuffers();

private:
	/*�� �� �ؽ��İ� �ִ��� Ȯ���� �� Shader�� ��뿩��(bool)�� SRV�� �����ش�*/
	/*ai Texture Type�� �ִ� �ؽ��ĸ� ���� ������� �ʱ� ������ Ư�� ��ȣ�� �����ؼ� �� ��ȣ�� �ؽ��ĸ� ��ȸ�մϴ�.
	���ο� �ؽ��ĸ� Shader�� �߰��ߴٸ� bool ������ �� �ؽ��� ��ȣ Ž���� �߰��Ͻø� �˴ϴ�*/
	void ModelMaterialAutoBind(shared_ptr<class CModel> _pModel, _uint _iMeshNum);
	void InstanceModelMaterialAutoBind(shared_ptr<class CInstancingModel> _pModel, _uint _iMeshNum);
	
/*ComputeShader�� SRV*/
private: 
	wrl::ComPtr<ID3D11UnorderedAccessView> m_pUAV = nullptr;
	wrl::ComPtr<ID3D11ShaderResourceView> m_pSRV = nullptr;
	wrl::ComPtr<ID3D11DepthStencilView> m_pDSV = nullptr;

public:
	/*PBR Ambient Lighting�� ����� ť�긦 ����*/
	HRESULT CreateCubeMap();


/*���� Render�� ����ϴ� ���̴��� Pass Index*/
private:
	shared_ptr<class CShader> m_pCurrentShader = nullptr;
	ESHADER_TYPE m_eCurrentShaderType = ESHADER_TYPE::ST_END;
	_uint m_iCurrentPassIdx = 0;
	_uint m_iCurrentTechniqueIdx = 0;

/*Model Texture Bool Array*/
private:
	_bool m_bUsingTex[2] = { false, false };

/*Shader Files Path*/
private:
	const wstring& m_strClientShaderPath = TEXT("..\\..\\Client\\Bin\\ShaderFiles");
	const wstring& m_strEngineShaderPath = TEXT("..\\Bin\\ShaderFiles");

private:
	wrl::ComPtr<ID3D11Device> m_pDevice = nullptr;
	wrl::ComPtr<ID3D11DeviceContext> m_pContext = nullptr;

	_float2 m_vScreenSize = _float2();

private:
	/*Shader List*/
	shared_ptr<class CShader> m_Shaders[ESHADER_TYPE::ST_END];
	shared_ptr<class CVIBuffer> m_Buffers[ESHADER_TYPE::ST_END];

	unordered_map<string, shared_ptr<class CVIBuffer>> m_BufferList;

private:
	wrl::ComPtr<ID3D11ShaderResourceView> m_pHDRCubeSRV = nullptr;
	wrl::ComPtr<ID3D11RenderTargetView> m_pHDRCubeRTV = nullptr;

public:
	static shared_ptr<CShaderHandler> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, _float _fScreenX, _float _fScreenY);

};

END
