#include "Loader.h"

CLoader::CLoader()
{
}

#ifdef CHECK_REFERENCE_COUNT
CLoader::~CLoader()
{
	m_LodingThread.join();
}
#endif // CHECK_REFERENCE_COUNT

HRESULT ThreadMain(shared_ptr<CLoader> _pThis)
{
	if (FAILED(CoInitializeEx(nullptr, 0)))
		return E_FAIL;

	shared_ptr<CLoader> pLoader = _pThis;

	pLoader->Loading_Selector();

	return S_OK;
}

HRESULT CLoader::Initialize(LEVEL _eNextLevel)
{
	m_eNextLevel = _eNextLevel;

	m_LodingThread = thread(ThreadMain, shared_from_this());

	return S_OK;
}

HRESULT CLoader::Loading_Selector()
{
	HRESULT	hr = {};

	switch (m_eNextLevel)
	{
	case LEVEL_ATELIER:
		hr = Loading_For_AtelierLevel();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_AtelierLevel()
{
	m_strLoadingText = TEXT("�ؽ��ĸ�(��) �ε� �� �Դϴ�.");

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ATELIER, TEXT("Prototype_Component_Texture_Terrain"),
	//	CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resources/Textures/Terrain/Test/Test.dds"), 1))))
	//	return E_FAIL;

	m_strLoadingText = TEXT("�𵨸�(��) �ε� �� �Դϴ�.");

	m_strLoadingText = TEXT("�׺���̼Ǹ�(��) �ε� �� �Դϴ�.");

	m_strLoadingText = TEXT("�浹ü��(��) �ε� �� �Դϴ�.");
	// For.Prototype_Component_Collider
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ATELIER, TEXT("Prototype_Component_Collider"),
	//	CCollider::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	m_strLoadingText = TEXT("���۸�(��) �ε� �� �Դϴ�.");
	// For.Prototype_Component_VIBuffer_Terrain
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ATELIER, TEXT("Prototype_Component_VIBuffer_Terrain"),
	//	CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resources/Textures/Terrain/Test/Test.dds")))))
	//	return E_FAIL;

	m_strLoadingText = TEXT("���̴���(��) �ε� �� �Դϴ�.");
	// For.Prototype_Component_Shader_VtxMesh
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ATELIER, TEXT("Prototype_Component_Shader_VtxMesh"),
	//	CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../../ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements))))
	//	return E_FAIL;
	// For.Prototype_Component_Shader_VtxAnimMesh
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ATELIER, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
	//	CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../../ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements))))
	//	return E_FAIL;

	m_strLoadingText = TEXT("��ü������(��) �ε� �� �Դϴ�.");
	// For.Prototype_Camera_Free
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Camera_Free"),
	//	CCamera_Free::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	// For.Prototype_GameObject_Terrain
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
	//	CTerrain::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	m_strLoadingText = TEXT("�ε��� �Ϸ�Ǿ����ϴ�.");

	m_bIsFinished = true;

	return S_OK;
}

#ifdef _DEBUG
void CLoader::Output_LoadingText()
{
	SetWindowText(g_hWnd, m_strLoadingText.c_str());
}
#endif

shared_ptr<CLoader> CLoader::Create(LEVEL _eNextLevel)
{
	shared_ptr<CLoader> pInstance = WRAPPING(CLoader)();

	if (FAILED(pInstance->Initialize(_eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLoader");
		pInstance.reset();
	}

	return pInstance;
}
