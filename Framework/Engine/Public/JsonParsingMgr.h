#pragma once

#include "Engine_Defines.h"

/* =============================================== */
//	[JsonParsingMgr]
// 
//  Json Parsing�� �Լ��� ��Ƶ� Ŭ���� 
//	
//
/* =============================================== */




BEGIN(Engine)

class CJsonParsingMgr final
{
public:
	CJsonParsingMgr();
	~CJsonParsingMgr() = default;

public:
	void WriteJson(Json::Value _value, const wstring& _strSavePath);
	Json::Value ReadJson(const wstring& _strReadPath);

public:
	static shared_ptr<CJsonParsingMgr> Create();

};

END