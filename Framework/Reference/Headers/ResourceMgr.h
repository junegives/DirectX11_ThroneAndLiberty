#pragma once

/* =============================================== */
//	[ResourceMgr]
// 
//  ���ҽ� ������ �ִ� �ؽ���, �� ������ ��ȸ�ϰ� �ڵ����� �߰����ִ� �۾��� ����
//	���ҽ� ������ ���� ���� �����س��� ������ ������ �����ؼ� �з��ص� ��!  
//
/* =============================================== */

#include "Engine_Defines.h"

BEGIN(Engine)

class CResourceMgr
{
public:
	CResourceMgr(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	~CResourceMgr() = default;

/*Funtion.Call Load Texture*/
public:
	void LoadLevelResource(_uint _iLevelIdx);


public:
	void ClearResource();

public:
	_bool GetStaticResLoadDone() { return m_IsStaticResDone; }

/*Funtion.Level Load*/
private:
	void StaticResourceLoad();
	void LogoResourceLoad();
	void DungeonMainResourceLoad();
	void DungeonFinalResourceLoad();
	void MapToolResourceLoad();
	void EffectToolResourceLoad();
	void VillageResourceLoad();
	void FonosResourceLoad();

/*Common*/
private:
	void LoadTexture(const wstring& _strTexPath);
	void LoadModels(const wstring& _strModelPath);
	void LoadBuffers();
	void LoadHDRCube();

private:
	HRESULT CreateSRV(wstring _strTexturePath, wstring _strExt, string _strTexName);

/*Function.Get Resource*/
public:
	shared_ptr<class CTexture> GetTexture(string _strTexKey);
	shared_ptr<class CModel> GetModel(string _strModelKey);
	shared_ptr<class CVIBuffer> GetBuffer(string _strBufferKey);

	wrl::ComPtr<ID3D11ShaderResourceView> GetSRV(string _strTexTag);

private:
	const wstring& m_strBaseResPath = TEXT("../../Client/Bin/Resources/");

private:
	
	unordered_map<string, pair<shared_ptr<class CTexture>, _bool>> m_Textures;
	unordered_map<string, pair<shared_ptr<class CModel>, _bool>> m_Models;
	unordered_map<string, shared_ptr<class CVIBuffer>> m_Buffers;
	unordered_map<string, pair<wrl::ComPtr<ID3D11ShaderResourceView>, _bool >> m_SRVs;

	/*HDR Cube��*/
	unordered_map<string, wrl::ComPtr<ID3D11ShaderResourceView>> m_CubeSRVs;

private:
	wrl::ComPtr<ID3D11Device> m_pDevice = nullptr;
	wrl::ComPtr<ID3D11DeviceContext> m_pContext = nullptr;

private:
	_bool m_IsStaticRes = false;
	_bool m_IsFieldResLoadDone = false;

	_bool m_IsStaticResDone = false;
public:
	void Free();

public:
	static shared_ptr<CResourceMgr> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);

};

END
