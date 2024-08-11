#pragma once

#include "Assimp_Defines.h"

BEGIN(Assimp)

class CMainConverter final
{
public:
	CMainConverter();
	virtual ~CMainConverter();

public:
	HRESULT Initialize();
	void Tick(_float _fTimeDelta);
	HRESULT Render();

public:
	void FindFBXFile(wstring _strFilePath);
	/*���� ���� ������ ���� �� �ִ� ������ ��ִ� ������ �������ش�*/
	_uint CheckModelType(string _strPath);
	/*�ڵ� ���� ��� ����*/
	wstring MakeSavePath(wstring _strPath, wstring _strFileName);

private:
	/*FBX�� �д� �⺻ ���*/
	wstring m_strBasePath = TEXT("..\\Bin\\Resource");
	/*Ŭ���̾�Ʈ ������*/
	wstring strClientBase = TEXT("..\\..\\Client\\Bin\\Resources");
	/*MapTool �з� ���*/
	wstring strMapToolBase = TEXT("..\\..\\Tool_Map\\Bin\\Resources");

public:
	void Free();

};

END