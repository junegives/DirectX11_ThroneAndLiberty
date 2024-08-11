#include "ObjectTool.h"

#include "EnvironmentObject.h"
#include "ToolMgr.h"
#include "Terrain.h"

#include "PickingMgr.h"

#include "GameObject.h"
#include "GroupObject.h"

#include "MapTool.h"

#include "Model.h"
#include "Mesh.h"

#include "VITerrain.h"
#include "Area.h"

CObjectTool::CObjectTool()
{
}

#ifdef CHECK_REFERENCE_COUNT
CObjectTool::~CObjectTool()
{
}
#endif // CHECK_REFERENCE_COUNT

HRESULT CObjectTool::Initialize()
{
	_uint iIndex{};
	_int iParentIndex{};

	// ���丮 �׸� ����.
	MakeDirectory(m_strFilePath, iIndex, iParentIndex);
	// �׷������Ʈ ������Ÿ�� ����. 
	MakePrefabGroupObjectsList();
	
	return S_OK;
}

void CObjectTool::Tick(_float _fTimeDelta)
{
	ResetEdge(MANAGER->GetMapTool()->GetCerrentEditModeArea());

	EditGizmo();

	ModelExplorer();

	EditGroupObject();

	KeyEvent();

	TrackingObject(_fTimeDelta);

	for (auto pMakeGroupMember : m_vecCreatePrefabGroupObjectMembers)
		pMakeGroupMember.lock()->SetEdgeRender(true);
	
}

#pragma region �ʱ� �׸� ����.
void CObjectTool::MakeDirectory(string _strFilePath, _uint& _iIndex, _int _iParentIndex)
{
	filesystem::path basePath(_strFilePath);

	_uint iIdxMyNode = _iIndex;

	NODE Node{};
	m_vecFileNodeTree.push_back(Node);
	m_vecFileNodeTree[iIdxMyNode].iIdxMyNode = iIdxMyNode;
	m_vecFileNodeTree[iIdxMyNode].strName = basePath.filename().string();
	m_vecFileNodeTree[iIdxMyNode].iIdxMyNode = iIdxMyNode;
	m_vecFileNodeTree[iIdxMyNode].iIdxParentNode = _iParentIndex;

	// �ش� ����� �ڽ� ��� ���� Ȯ���� ���� ��ȸ.

	// ������ ���� ����� �ε��� �����ޱ� ���� ���� �߰�.
	_uint iIdxLeftSiblingNode{};

	for (const auto& entry : filesystem::directory_iterator(basePath))
	{
		// �ڽ� ����� �� Ȯ���Ͽ� ����.
		if (filesystem::is_directory(entry.status()))
		{
			m_vecFileNodeTree[iIdxMyNode].iNumChild++;
			m_vecFileNodeTree[iIdxMyNode].vecDirectoryIdxList.push_back(++_iIndex);

			iIdxLeftSiblingNode = _iIndex;

			MakeDirectory(entry.path().string(), _iIndex, iIdxMyNode);

			m_vecFileNodeTree[iIdxLeftSiblingNode].iIdxSiblingNode = _iIndex + 1;
		}
		//�ڽ� ��尡 �ƴ� ������ ��� ������ ���� �̸��� ����
		else
		{
			string strFileName = entry.path().stem().string();
			//strFileName = strFileName.substr(3, strFileName.size() - 6);
			m_vecFileNodeTree[iIdxMyNode].vecFileList.push_back(strFileName);
		}
	}

	// �ڽ� ��� ���� Ȯ���Ͽ� �ش� ��� Ÿ�� ����.
	if (0 < m_vecFileNodeTree[iIdxMyNode].iNumChild)
		m_vecFileNodeTree[iIdxMyNode].eType = TYPE_INTERNAL;
	else
		m_vecFileNodeTree[iIdxMyNode].eType = TYPE_LEAF;

	// �ڽ� ��� ��
	m_vecFileNodeTree[iIdxMyNode].iNumChild = static_cast<_uint>(m_vecFileNodeTree[iIdxMyNode].vecDirectoryIdxList.size());

}

HRESULT CObjectTool::MakePrefabGroupObjectsList()
{
	m_vecProtoGroupObjects.clear();

	filesystem::path basePath(m_strPrefabGroupObjectDataFilePath);

	for (const auto& entry : filesystem::directory_iterator(basePath))
	{
		if (entry.is_regular_file())
		{
			ifstream InBinaryFile(m_strPrefabGroupObjectDataFilePath + entry.path().filename().string(), ios::in | ios::binary);
			if (InBinaryFile.is_open())
			{
				// �׷� ������Ʈ �̸� �б�.
				char szGroupObjectName[MAX_PATH]{};
				InBinaryFile.read(szGroupObjectName, sizeof(szGroupObjectName));

				// ��� ������Ʈ �� �б�.
				_uint iNumMemberObjects{};
				InBinaryFile.read(reinterpret_cast<char*>(&iNumMemberObjects), sizeof(_uint));

				vector<pair<string, _float4x4>> vecMemberInfo{};

				for (_uint iCntMemObj = 0; iCntMemObj < iNumMemberObjects; iCntMemObj++)
				{
					char szMemberObjectModelKey[MAX_PATH]{};
					InBinaryFile.read(szMemberObjectModelKey, sizeof(szMemberObjectModelKey));

					_float4x4 matWorld{};
					InBinaryFile.read(reinterpret_cast<char*>(&matWorld), sizeof(_float4x4));

					vecMemberInfo.push_back(make_pair(szMemberObjectModelKey, matWorld));
				}

				m_vecProtoGroupObjects.push_back(CGroupObject::Create(szGroupObjectName, vecMemberInfo));

				InBinaryFile.close();
			}
			else
			{
				m_vecProtoGroupObjects.clear();
				//MSG_BOX("Failed to Read : Binary File");
				return E_FAIL;
			}

			//MSG_BOX("Read Binary Success!");

		}
	}

	return S_OK;
}
#pragma endregion

#pragma region ���� ��.
void CObjectTool::ModelExplorer()
{
	if (ImGui::TreeNodeEx("Model Explorer", ImGuiTreeNodeFlags_DefaultOpen))
	{
		_uint iIndex{};

		ImGuiChildFlags_ ChildFlags = ImGuiChildFlags_Border;
		ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_None | ImGuiWindowFlags_MenuBar;
		_float fSizeX = ImGui::GetContentRegionAvail().x * 0.49f;



		ImGui::BeginChild("##Show Directory", ImVec2(fSizeX, 150), ChildFlags, WindowFlags);
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::Text("Directory");
				ImGui::EndMenuBar();
			}
			ShowDirectory(iIndex);
			ImGui::EndChild();
		}

		ImGui::SameLine();

		ImGui::BeginChild("##Show ModelList", ImVec2(fSizeX, 150), ChildFlags, WindowFlags);
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::Text("Model List");
				ImGui::EndMenuBar();
			}
			ShowModelList();
			ImGui::EndChild();
		}
	ImGui::TreePop();
	}
}

void CObjectTool::ShowDirectory(_uint& _iIndex)
{
	// ImGuiTreeNodeFlags_None
	if (ImGui::TreeNodeEx(m_vecFileNodeTree[_iIndex].strName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		_int iNumChild = static_cast<_uint>(m_vecFileNodeTree[_iIndex].vecDirectoryIdxList.size());
		for (_int i = 0; i < iNumChild; i++)
		{
			_iIndex++;
			if (TYPE_INTERNAL == m_vecFileNodeTree[_iIndex].eType)
			{
				ShowDirectory(_iIndex);
			}
			else // �ܸ� ����϶�
			{
				ImGuiTreeNodeFlags_ Flags{};
				if (false == m_mapLeafNodeList[_iIndex])
				{
					ImGui::SetNextItemOpen(false);
					Flags = ImGuiTreeNodeFlags_None;
				}
				else
				{
					ImGui::SetNextItemOpen(true);
					Flags = ImGuiTreeNodeFlags_Selected;
				}

				if (ImGui::TreeNodeEx(m_vecFileNodeTree[_iIndex].strName.c_str(), Flags))
				{
					for (auto it = m_mapLeafNodeList.begin(); it != m_mapLeafNodeList.end(); ++it)
					{
						if (_iIndex == it->first)
							it->second = true;
						else
							it->second = false;
					}

					ImGui::TreePop();
				}
				else
				{
					m_mapLeafNodeList[_iIndex] = false;
				}
			}

		}

		ImGui::TreePop();
	}
	else // ���� ���¶��!
		_iIndex = m_vecFileNodeTree[_iIndex].iIdxSiblingNode - 1;

}

void CObjectTool::ShowModelList()
{
	ImGui::SameLine();
	vector<string> vecFileList{};

	for (auto it = m_mapLeafNodeList.begin(); it != m_mapLeafNodeList.end(); ++it)
	{
		if (true == it->second)
			vecFileList = m_vecFileNodeTree[it->first].vecFileList;

	}
	
	
	if (ImGui::BeginListBox("##Model List", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
	{
		for (int iNumFile = 0; iNumFile < vecFileList.size(); iNumFile++)
		{
			// �ش� �׸��� Ȱ��ȭ �Ǿ��Ǿ��ٸ�
			m_bIsSelectedModel = (m_iIdxModel == iNumFile);
			if (ImGui::Selectable(vecFileList[iNumFile].c_str(), m_bIsSelectedModel))
			{
				m_iIdxModel = iNumFile;
				if (m_strCurSelectedModel != vecFileList[m_iIdxModel])
				{
					m_strCurSelectedModel = vecFileList[m_iIdxModel];
					m_pSelectedObject = CEnvironmentObject::Create(m_strCurSelectedModel);
					m_pSelectedPrefabGroupObject.reset();
				}

			}
			if (m_bIsSelectedModel)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
}
#pragma endregion


void CObjectTool::EditGroupObject()
{
	if (ImGui::TreeNodeEx("Edit GroupObject", ImGuiTreeNodeFlags_DefaultOpen))
	{
#pragma region Create Prefab GroupObject
		ImGui::SeparatorText("Create Prefab GroupObject");

		ShowGroupObjectMemberList();

		m_strCreatePrefabGroupObjectName.resize(MAX_PATH);
		ImGui::PushItemWidth(200);
		ImGui::InputText("##Create Prefab GroupObject Name", &m_strCreatePrefabGroupObjectName[0], m_strCreatePrefabGroupObjectName.size()); ImGui::SameLine();
		m_strCreatePrefabGroupObjectName.resize(m_strCreatePrefabGroupObjectName.find_last_not_of('\0') + 1);  // ���ڿ��� ������ ���� ���ķ� �߶���ϴ�.
		ImGui::Text("Prefab Name(.dat)");

		if (ImGui::Button("Create", ImVec2(200, 19)))
			CreatePrefabGroupObjectFile();

		if (ImGui::Button("Clear", ImVec2(96, 19)))
			ClearGroupObject();

		ImGui::SameLine();

		if (ImGui::Button("Delete", ImVec2(96, 19)))
			DeleteGroupObjectMember();
#pragma endregion // Create GroupObject

#pragma region Create GroupObject
		ImGui::SeparatorText("Prefab GroupObject List");

		ShowProtoGroupObjectList();
		ImGui::SameLine();
		ImGui::Text("Prefab List");

		if (ImGui::Button("Reload", ImVec2(200, 19)))
			MakePrefabGroupObjectsList();
#pragma endregion // Create GroupObject
	ImGui::TreePop();
	}
}

HRESULT CObjectTool::CreatePrefabGroupObjectFile()
{
	// 1. ���� ���ϱ�.
	_float3 vAvgPos{};
	for (auto pGroupMember : m_vecCreatePrefabGroupObjectMembers)
	{
		vAvgPos += pGroupMember.lock()->GetTransform()->GetState(CTransform::STATE_POSITION);
	}

	vAvgPos /= static_cast<_float>(static_cast<double>(m_vecCreatePrefabGroupObjectMembers.size()));
	_float4x4 matPos = _float4x4::CreateTranslation(vAvgPos);
	_float4x4 matInvPos = matPos.Invert();

	vector<pair<string, _float4x4>> vecMemberInfo{};

	for (auto pGroupMember : m_vecCreatePrefabGroupObjectMembers)
	{
		_float4x4 matWorld = pGroupMember.lock()->GetTransform()->GetWorldMatrix();
		vecMemberInfo.push_back(make_pair(pGroupMember.lock()->GetModelKey(), matWorld * matInvPos));
	}

	string strCreateGroupObjectFilePath = m_strPrefabGroupObjectDataFilePath + m_strCreatePrefabGroupObjectName + ".dat";

	ofstream OutBinaryFile(strCreateGroupObjectFilePath, ios::out | ios::binary);
	if (OutBinaryFile.is_open())
	{
		// �׷� ������Ʈ �̸� ����
		_char szGroupObjectName[MAX_PATH] = "";
		strcpy_s(szGroupObjectName, m_strCreatePrefabGroupObjectName.c_str());
		OutBinaryFile.write(szGroupObjectName, sizeof(szGroupObjectName));

		// ��� ������Ʈ �� ����
		_uint iNumMemberObjects = static_cast<_uint>(vecMemberInfo.size());
		OutBinaryFile.write(reinterpret_cast<const char*>(&iNumMemberObjects), sizeof(_uint));

		for (const auto& MemberInfo : vecMemberInfo)
		{
			// ��� ������Ʈ �� Ű ����
			_char szMemberObjectModelKey[MAX_PATH] = "";
			strcpy_s(szMemberObjectModelKey, MemberInfo.first.c_str());
			OutBinaryFile.write(szMemberObjectModelKey, sizeof(szMemberObjectModelKey));

			// ��� ������Ʈ ��� ����.
			OutBinaryFile.write(reinterpret_cast<const char*>(&MemberInfo.second), sizeof(_float4x4));
		}

		OutBinaryFile.close();
	}
	else // ���̳ʸ� ���� ����
	{
		MSG_BOX("WriteBinary Failed!");
		return E_FAIL;
	}

	MSG_BOX("WriteBinary Success!");

	ClearGroupObject();

	return S_OK;
}

void CObjectTool::ClearGroupObject()
{
	m_strCreatePrefabGroupObjectName = "";
	m_vecCreatePrefabGroupObjectMembers.clear();
}

void CObjectTool::ShowGroupObjectMemberList()
{
	if (ImGui::BeginListBox("##GroupMember List"))
	{
		for (int iCntGroupMember = 0; iCntGroupMember < m_vecCreatePrefabGroupObjectMembers.size(); iCntGroupMember++)
		{
			m_bIsSelectedGroupMember = (m_iIdxMakeGroupObjectMember == iCntGroupMember);
			if (ImGui::Selectable(m_vecCreatePrefabGroupObjectMembers[iCntGroupMember].lock()->GetModelKey().c_str(), m_bIsSelectedGroupMember))
			{
				m_iIdxMakeGroupObjectMember = iCntGroupMember;
				//m_strCurSelectedGroupMember = m_vecGroupMembers[m_iIdxGroupObjectMember];

			}
			if (m_bIsSelectedGroupMember)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

}

HRESULT CObjectTool::DeleteGroupObjectMember()
{
	if (true == m_vecCreatePrefabGroupObjectMembers.empty())
		return E_FAIL;
	
	m_vecCreatePrefabGroupObjectMembers.erase(m_vecCreatePrefabGroupObjectMembers.begin() + m_iIdxMakeGroupObjectMember);

	return S_OK;
}

void CObjectTool::CreateGroupObject()
{
	m_pSelectedObject.reset();
	m_pSelectedPrefabGroupObject.reset();
	m_pPickedObject.reset();
	m_pPickedGroupObject.reset();

	vector<pair<string, _float4x4>> vecMemberInfo{};

	for (auto pMemberObject : m_vecCreatePrefabGroupObjectMembers)
	{
		string MemberObjectModelKey = pMemberObject.lock()->GetModelKey();
		_float4x4 matMemberObjectWorld = pMemberObject.lock()->GetTransform()->GetWorldMatrix();

		vecMemberInfo.push_back(make_pair(MemberObjectModelKey, matMemberObjectWorld));

		_uint MemberObjectID = pMemberObject.lock()->GetObjID();

		map<string, vector<shared_ptr<CEnvironmentObject>>>& mapObjects = MANAGER->GetMapTool()->GetCerrentEditModeArea()->GetObjects();
		for (auto& pair : mapObjects)
		{
			if (pair.first == MemberObjectModelKey)
			{
				_uint iNumObjectsSize = static_cast<_uint>(pair.second.size());

				for (_uint iCntObjectsSize = 0; iCntObjectsSize < iNumObjectsSize; iCntObjectsSize++)
				{

					if (pair.second[iCntObjectsSize]->GetObjID() == MemberObjectID)
					{
						pair.second[iCntObjectsSize].reset();
						pair.second.erase(pair.second.begin() + iCntObjectsSize);
						break;
					}
				}
				break;
			}
		}
	}

	_float3 vAvgPos{};
	for (auto MemberInfo : vecMemberInfo)
		vAvgPos += MemberInfo.second.Translation();

	vAvgPos /= static_cast<_float>(static_cast<double>(vecMemberInfo.size()));

	_float4x4 matPos = _float4x4::CreateTranslation(vAvgPos);
	_float4x4 matInvPos = matPos.Invert();

	for (auto& MemberInfo : vecMemberInfo)
	{
		MemberInfo.second = MemberInfo.second * matInvPos;
	}

	shared_ptr<CGroupObject> pGroupObject = CGroupObject::Create("Temp", vecMemberInfo);
	pGroupObject->GetTransform()->SetWorldMatrix(matPos);
	MANAGER->GetMapTool()->GetCerrentEditModeArea()->AddGroupObject(pGroupObject);

	vector<shared_ptr<CEnvironmentObject>>& vecMemberObjects = pGroupObject->GetMemberObjects();

	for (auto& pMemberObject : vecMemberObjects)
		m_mapObjsID.emplace(pMemberObject->GetObjID(), pMemberObject);

	m_pPickedGroupObject = pGroupObject;

	m_vecCreatePrefabGroupObjectMembers.clear();

}

void CObjectTool::UnGroupObjects()
{
	m_pSelectedObject.reset();
	m_pSelectedPrefabGroupObject.reset();
	m_pPickedObject.reset();

	if (nullptr == m_pPickedGroupObject.lock())
		return;

	_float4x4 matGroupObjWorld = m_pPickedGroupObject.lock()->GetTransform()->GetWorldMatrix();
	vector<shared_ptr<CEnvironmentObject>>& vecMemberObjects = m_pPickedGroupObject.lock()->GetMemberObjects();
	//m_pPickedGroupObject.reset();


	_uint iNumMemberObjects = static_cast<_uint>(vecMemberObjects.size());

	for (_uint iCntMemberObjects = 0; iCntMemberObjects < iNumMemberObjects; iCntMemberObjects++)
	{
		_float4x4 matMemObjWorld = vecMemberObjects[iCntMemberObjects]->GetTransform()->GetWorldMatrix();

		vecMemberObjects[iCntMemberObjects]->SetIsGroup(false);
		vecMemberObjects[iCntMemberObjects]->GetTransform()->SetWorldMatrix(matMemObjWorld * matGroupObjWorld);
		MANAGER->GetMapTool()->GetCerrentEditModeArea()->AddObject(vecMemberObjects[iCntMemberObjects]);

		m_vecCreatePrefabGroupObjectMembers.push_back(vecMemberObjects[iCntMemberObjects]);
	}

	vecMemberObjects.clear();

	m_pPickedGroupObject.lock()->SetActive(false);
	m_pPickedGroupObject.reset();
}


#pragma region �׷� �� ������Ÿ��.
void CObjectTool::ShowProtoGroupObjectList()
{
	if (ImGui::BeginListBox("##Proto GroupObject List", ImVec2(200, 100)))
	{
		for (_int iCntProtoObj = 0; iCntProtoObj < m_vecProtoGroupObjects.size(); iCntProtoObj++)
		{
			m_bIsSelectedProtoGroupObject = (m_iIdxProtoGroupObject == iCntProtoObj);
			if (ImGui::Selectable(m_vecProtoGroupObjects[iCntProtoObj]->GetGroupObjectName().c_str(), m_bIsSelectedProtoGroupObject))
			{
				m_iIdxProtoGroupObject = iCntProtoObj;
				if (m_strCurSelectProtoGroupObjectName != m_vecProtoGroupObjects[m_iIdxProtoGroupObject]->GetGroupObjectName())
				{
					m_strCurSelectProtoGroupObjectName = m_vecProtoGroupObjects[m_iIdxProtoGroupObject]->GetGroupObjectName();
					m_pSelectedPrefabGroupObject = m_vecProtoGroupObjects[m_iIdxProtoGroupObject];
					m_pSelectedObject.reset();
				}


			}
			if (m_bIsSelectedProtoGroupObject)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
}
#pragma endregion

#pragma region ��Ÿ �� ���.
void CObjectTool::KeyEvent()
{
	_bool bIsHotkeyLock = MANAGER->GetIsHotkeyLock();

	if (GAMEINSTANCE->KeyDown(DIK_ESCAPE) && false == MANAGER->GetIsHotkeyLock() && false == bIsHotkeyLock)
	{
		m_pSelectedObject.reset();
		m_pSelectedPrefabGroupObject.reset();
		m_pPickedObject.reset();
		m_pPickedGroupObject.reset();
	}

	// ������Ʈ ��ġ
	if (GAMEINSTANCE->KeyDown(DIK_Q) && false == MANAGER->GetIsHotkeyLock() && false == bIsHotkeyLock)
		Placing();
	
	// ��ġ�� ������Ʈ ����
	if (GAMEINSTANCE->KeyDown(DIK_F) && false == MANAGER->GetIsHotkeyLock() && false == bIsHotkeyLock)
		PickedObject();

	// ���õ� ������Ʈ ����
	if (GAMEINSTANCE->KeyDown(DIK_DELETE) && false == MANAGER->GetIsHotkeyLock() && false == bIsHotkeyLock)
		DeleteObject();



	// �׷�ȭ ��� �߰�
	if (GAMEINSTANCE->KeyPressing(DIK_LCONTROL) && GAMEINSTANCE->MouseDown(DIM_LB) && false == bIsHotkeyLock)
		EditCreatePrefabGroupObjectMember(true);

	// �׷�ȭ ��� ���
	if (GAMEINSTANCE->KeyPressing(DIK_LALT) && GAMEINSTANCE->MouseDown(DIM_LB) && false == bIsHotkeyLock)
		EditCreatePrefabGroupObjectMember(false);

	// �ش� ���� �׷�ȭ
	if (GAMEINSTANCE->KeyPressing(DIK_LCONTROL) && GAMEINSTANCE->KeyDown(DIK_G) && false == bIsHotkeyLock)
		CreateGroupObject();

	// �ش� �׷� ������Ʈ ����
	if (GAMEINSTANCE->KeyPressing(DIK_LALT) && GAMEINSTANCE->KeyDown(DIK_G) && false == bIsHotkeyLock)
		UnGroupObjects();

	if (GAMEINSTANCE->KeyPressing(DIK_LALT) && GAMEINSTANCE->KeyDown(DIK_C) && false == bIsHotkeyLock)
		CopyObject();

	//if (GAMEINSTANCE->KeyPressing(DIK_LSHIFT) && GAMEINSTANCE->MouseDown(DIM_LB))
	//{
	//	PickedObject();

	//	if (nullptr == m_pPickedObject.lock())
	//		return;

	//	if (nullptr != m_pPickedObject.lock())
	//	{
	//		
	//		//MANAGER->GetMapTool()->AddObject(m_pPickedObject.lock());
	//		//m_vecEnableObject.push_back(m_pPickedObject.lock());
	//		m_pPickedObject.lock()->SetEnable(false);
	//	}
	//	//else
	//	//	m_vecEnableObject.push_back(m_pPickedGroupObject.lock());

	//	//for (auto pEnableObject : m_vecEnableObject)
	//	//	pEnableObject.lock()->SetEnable(false);

	//	m_pPickedObject.reset();
	//	m_pPickedGroupObject.reset();
	//}
		

	//m_pPickedObject.lock()->SetActive(false);


	//if (GAMEINSTANCE->KeyPressing(DIK_RCONTROL) && GAMEINSTANCE->KeyDown(DIK_X))
	//	TargetAxisInversion(CTransform::STATE_RIGHT);

	//if (GAMEINSTANCE->KeyPressing(DIK_RCONTROL) && GAMEINSTANCE->KeyDown(DIK_Y))
	//	TargetAxisInversion(CTransform::STATE_UP);

	//if (GAMEINSTANCE->KeyPressing(DIK_RCONTROL) && GAMEINSTANCE->KeyDown(DIK_Z))
	//	TargetAxisInversion(CTransform::STATE_LOOK);

// ���� ����
	if (GAMEINSTANCE->KeyPressing(DIK_LSHIFT) && GAMEINSTANCE->KeyDown(DIK_G) && bIsHotkeyLock)
		EditGroupObject();

	if (GAMEINSTANCE->KeyPressing(DIK_LALT) && GAMEINSTANCE->KeyDown(DIK_G) && bIsHotkeyLock)
		UnGroupObjects();
 
}

void CObjectTool::EditCreatePrefabGroupObjectMember(_bool _bIsAdd)
{
	PickedObject();

	if (nullptr == m_pPickedObject.lock())
		return;

	_uint iSelectObjectID = m_pPickedObject.lock()->GetObjID();
	_uint iNumMemberObjectsSize = static_cast<_uint>(m_vecCreatePrefabGroupObjectMembers.size());

	if (true == _bIsAdd)
	{
		for (auto pMemberObjects : m_vecCreatePrefabGroupObjectMembers)
		{
			if (iSelectObjectID == pMemberObjects.lock()->GetObjID())
			{
				return;
			}
		}
		m_vecCreatePrefabGroupObjectMembers.push_back(m_pPickedObject.lock());
	}
	else // false == _bIsAdd
	{
		for (_uint iCntMemberObjectsSize = 0; iCntMemberObjectsSize < iNumMemberObjectsSize; iCntMemberObjectsSize++)
		{
			if (iSelectObjectID == m_vecCreatePrefabGroupObjectMembers[iCntMemberObjectsSize].lock()->GetObjID())
			{
				m_vecCreatePrefabGroupObjectMembers.erase(m_vecCreatePrefabGroupObjectMembers.begin() + iCntMemberObjectsSize);
				return;
			}
		}
	}


}



void CObjectTool::ResetEdge(shared_ptr<CArea> _pArea)
{
	map<string, vector<shared_ptr<CEnvironmentObject>>>& mapObjects = _pArea->GetObjects();
	for (const auto& pair : mapObjects)
	{
		for (const auto& pObject : pair.second)
			pObject->SetEdgeRender(false);
	}
	
	vector<shared_ptr<CGroupObject>>& vecGroupObjects = _pArea->GetGroupObjects();
	for (const auto pGroupObject : vecGroupObjects)
	{
		vector<shared_ptr<CEnvironmentObject>>& vecMemberObjects = pGroupObject->GetMemberObjects();
		for (const auto pMemberObject : vecMemberObjects)
		{
			pMemberObject->SetEdgeRender(false);
		}
	}
}

void CObjectTool::PickedObject()
{
	m_pPickedObject.reset();
	m_pPickedGroupObject.reset();

	// ���� ���콺 Ʈ��ŷ ���� ������Ʈ�� ���ٸ�.
	if (nullptr != m_pSelectedObject && nullptr != m_pSelectedPrefabGroupObject)
		return;
	
	if (FAILED(GAMEINSTANCE->ComputePicking(CPickingMgr::TYPE_PIXEL)))
		return;

	_uint test = GAMEINSTANCE->GetPickedObjectID();

	shared_ptr<CEnvironmentObject> pPickedObject = m_mapObjsID[GAMEINSTANCE->GetPickedObjectID()].lock();
	

	if (nullptr == pPickedObject)
		return;

	if (true == pPickedObject->GetIsGroup())
	{
		m_pPickedGroupObject = pPickedObject->GetOnerGroupObject();
		m_pPickedObject.reset();
	}
	else
	{
		m_pPickedObject = pPickedObject;
		m_pPickedGroupObject.reset();
	}
	
	
}

void CObjectTool::Placing()
{
	string strDefault = "Default";
	if (strDefault == MANAGER->GetMapTool()->GetCerrentEditModeArea()->GetAreaName())
	{
		MSG_BOX("Failed to Placing : Current Area Default!");
		return;
	}

	if (nullptr != m_pSelectedObject && nullptr == m_pSelectedPrefabGroupObject)
	{
		shared_ptr<CEnvironmentObject> pEnvironment = CEnvironmentObject::Create(m_pSelectedObject->GetModelKey());

		*(pEnvironment->GetTransform()) = *(m_pSelectedObject->GetTransform());

		pEnvironment.swap(m_pSelectedObject);

		MANAGER->GetMapTool()->AddObject(pEnvironment);

		m_mapObjsID.emplace(pEnvironment->GetObjID(), pEnvironment);
	}
	
	// ���� ����!
	else if (nullptr != m_pSelectedPrefabGroupObject && nullptr == m_pSelectedObject)
	{
		shared_ptr<CGroupObject> pGroupObject = m_pSelectedPrefabGroupObject->Clone();
		MANAGER->GetMapTool()->AddGroupObject(pGroupObject);

		vector<shared_ptr<CEnvironmentObject>>& vecMemberObjects = pGroupObject->GetMemberObjects();
		
		for (auto& pMemberObject : vecMemberObjects)
			m_mapObjsID.emplace(pMemberObject->GetObjID(), pMemberObject);
	}
	
	else
		return;
}




////void CObjectTool::UnGroupObjects()
////{
 	
	//if (nullptr == m_pPickedGroupObject.lock())
	//	return;

	//_float4x4 matGroupObjWorld = m_pPickedGroupObject.lock()->GetTransform()->GetWorldMatrix();
	//vector<shared_ptr<CEnvironmentObject>> vecMemberObjects = m_pPickedGroupObject.lock()->GetMemberObjects();





	//for (auto pMemberObjects : vecMemberObjects)
	//{
	//	string strMemObjModelKey = pMemberObjects->GetModelKey();
	//	_float4x4 matMemObjWorld = pMemberObjects->GetTransform()->GetWorldMatrix();

	//	shared_ptr<CEnvironmentObject> pObject = CEnvironmentObject::Create(strMemObjModelKey, matMemObjWorld* matGroupObjWorld);

	//	GAMEINSTANCE->AddObject(GAMEINSTANCE->GetCurrentLevel(), TEXT("Layer_EnvironmentObject"), pObject);
	//	m_mapObjsID.emplace(pObject->GetObjID(), pObject);
	//	m_vecMakeGroupMembers.push_back(pObject);
	//	pMemberObjects->SetActive(false);
	//}
	//m_pPickedGroupObject.lock()->SetActive(false);
	//m_pPickedGroupObject.reset();
	
 
 
 
//	list<shared_ptr<CGameObject>>* plstObject = GAMEINSTANCE->GetObjectList(GAMEINSTANCE->GetCurrentLevel(), TEXT("Layer_GroupObject"));
//
//	if (0 == plstObject->size())
//	{
//		MSG_BOX("�׷� ������Ʈ�� �����ϴ�.");
//		return;
//	}
//
//	list<shared_ptr<CGameObject>> lstObject = *plstObject;
//
//	for (auto pObject : lstObject)
//	{
//		if (nullptr != pObject)
//		{
//			shared_ptr<CGroupObject> pGroupObject = static_pointer_cast<CGroupObject>(pObject);
//
//			_float4x4 matGroupObjWorld = pGroupObject->GetTransform()->GetWorldMatrix();
//
//			vector<shared_ptr<CEnvironmentObject>> vecMemberObjects = pGroupObject->GetMemberObjects();
//
//			for (auto pMemberObjects : vecMemberObjects)
//			{
//				string strMemObjModelKey = pMemberObjects->GetModelKey();
//				_float4x4 matMemObjWorld = pMemberObjects->GetTransform()->GetWorldMatrix();
//
//				shared_ptr<CEnvironmentObject> pObject = CEnvironmentObject::Create(strMemObjModelKey, matMemObjWorld * matGroupObjWorld);
//
//				GAMEINSTANCE->AddObject(GAMEINSTANCE->GetCurrentLevel(), TEXT("Layer_EnvironmentObject"), pObject);
//				//m_mapObjsID.emplace(pObject->GetObjID(), pObject);
//				//m_vecMakeGroupMembers.push_back(pObject);
//				pMemberObjects->SetActive(false);
//
//
//			}
//
//			pObject->SetActive(false);
//		}
//	}
//
//	
//}

//void CObjectTool::ShowEnableObject()
//{
//	if (true == m_vecEnableObject.empty())
//	{
//		MSG_BOX("���� Enable Objects�� �����ϴ�.");
//		return;
//	}
//	
//	m_bIsEnable = !m_bIsEnable;
//
//	for (auto pEnableObject : m_vecEnableObject)
//		pEnableObject.lock()->SetEnable(m_bIsEnable);
//}
//
//void CObjectTool::SetEnableObject()
//{
//	PickedObject();
//
//	if (nullptr == m_pPickedObject.lock() && nullptr == m_pPickedGroupObject.lock())
//		return;
//	
//	if (nullptr != m_pPickedObject.lock())
//		m_vecEnableObject.push_back(m_pPickedObject.lock());
//	else
//		m_vecEnableObject.push_back(m_pPickedGroupObject.lock());
//
//	for (auto pEnableObject : m_vecEnableObject)
//		pEnableObject.lock()->SetEnable(false);
//
//	m_pPickedObject.reset();
//	m_pPickedGroupObject.reset();
//}
//
//void CObjectTool::ReSetEnableObject()
//{
//	if (true == m_vecEnableObject.empty())
//	{
//		MSG_BOX("���� Enable Objects�� �����ϴ�.");
//		return;
//	}
//
//	for (auto pEnableObject : m_vecEnableObject)
//		pEnableObject.lock()->SetEnable(true);
//	m_vecEnableObject.clear();
//}

void CObjectTool::TargetAxisInversion(CTransform::STATE _eState)
{
	shared_ptr<CGameObject> pObject{};

	if (nullptr != m_pPickedObject.lock())
		pObject = static_pointer_cast<CGameObject>(m_pPickedObject.lock());

	if (nullptr != m_pPickedGroupObject.lock())
		pObject = static_pointer_cast<CGameObject>(m_pPickedGroupObject.lock());

	if (nullptr == pObject)
		return;

	_float3 vScaled = pObject->GetTransform()->GetScaled();

	switch (_eState)
	{
	case Engine::CTransform::STATE_RIGHT:
		vScaled.x *= -1;
		break;
	case Engine::CTransform::STATE_UP:
		vScaled.y *= -1;
		break;
	case Engine::CTransform::STATE_LOOK:
		vScaled.z *= -1;
		break;
	}
	
	pObject->GetTransform()->SetScaling(vScaled);
}

void CObjectTool::DeleteObject()
{
	if (nullptr != m_pPickedObject.lock())
	{
		_uint iDeleteObjectID = m_pPickedObject.lock()->GetObjID();

		_uint iNumCreatePrefabGroupObjectMember = static_cast<_uint>(m_vecCreatePrefabGroupObjectMembers.size());

		for (_uint iCntCreatePrefabGroupObjectMember = 0; iCntCreatePrefabGroupObjectMember < iNumCreatePrefabGroupObjectMember; iCntCreatePrefabGroupObjectMember++)
		{
			if (iDeleteObjectID == m_vecCreatePrefabGroupObjectMembers[iCntCreatePrefabGroupObjectMember].lock()->GetObjID())
			{
				m_vecCreatePrefabGroupObjectMembers.erase(m_vecCreatePrefabGroupObjectMembers.begin() + iCntCreatePrefabGroupObjectMember);
				break;
			}
		}

		auto iter = m_mapObjsID.find(iDeleteObjectID);
		if (m_mapObjsID.end() != iter)
			m_mapObjsID.erase(iter);


		m_pPickedObject.lock()->SetActive(false);
		m_pPickedObject.reset();
	}

	if (nullptr != m_pPickedGroupObject.lock())
	{
		m_pPickedGroupObject.lock()->SetActive(false);
		m_pPickedGroupObject.reset();
	}
}

void CObjectTool::TrackingObject(_float _fTimeDelta)
{
	CEnvironmentObject::m_bIsReanderPixelPicking = true;

	// �� �� Nullptr�̸�, �ٷ� ����
	if (nullptr == m_pSelectedObject && nullptr == m_pSelectedPrefabGroupObject)
		return;

	if (nullptr != m_pSelectedObject)
	{
		CEnvironmentObject::m_bIsReanderPixelPicking = false;
		if (SUCCEEDED(GAMEINSTANCE->ComputePicking(CPickingMgr::TYPE_PIXEL)))
		{
			m_pSelectedObject->GetTransform()->SetState(CTransform::STATE_POSITION, GAMEINSTANCE->GetWorldPosPicked());
			//m_pSelectedObject->PriorityTick(_fTimeDelta);
			//m_pSelectedObject->Tick(_fTimeDelta);
			m_pSelectedObject->LateTick(_fTimeDelta);
		}
	}

	if (nullptr != m_pSelectedPrefabGroupObject)
	{
		CEnvironmentObject::m_bIsReanderPixelPicking = false;
		if (SUCCEEDED(GAMEINSTANCE->ComputePicking(CPickingMgr::TYPE_PIXEL)))
		{
			m_pSelectedPrefabGroupObject->GetTransform()->SetState(CTransform::STATE_POSITION, GAMEINSTANCE->GetWorldPosPicked());
			//m_pSelectedPrefabGroupObject->PriorityTick(_fTimeDelta);
			//m_pSelectedPrefabGroupObject->Tick(_fTimeDelta);
			m_pSelectedPrefabGroupObject->LateTick(_fTimeDelta);
		}
	}
}

void CObjectTool::CopyObject()
{
	// ���� ������ ���ٸ� ����
	string strDefault = "Default";
	if (strDefault == MANAGER->GetMapTool()->GetCerrentEditModeArea()->GetAreaName())
	{
		MSG_BOX("Failed to Placing : Current Area Default!");
		return;
	}

	// ���� ����� �Ѵ� ���ٸ� ����
	if (nullptr == m_pPickedObject.lock() && nullptr == m_pPickedGroupObject.lock())
		return;

	if (nullptr != m_pPickedObject.lock())
	{
		shared_ptr<CEnvironmentObject> pEnvironment = CEnvironmentObject::Create(m_pPickedObject.lock()->GetModelKey());

		*(pEnvironment->GetTransform()) = *(m_pPickedObject.lock()->GetTransform());

		m_pSelectedObject = pEnvironment;

		m_pPickedObject.reset();
	}

	else if (nullptr != m_pPickedGroupObject.lock())
	{
		shared_ptr<CGroupObject> pGroupObject = m_pPickedGroupObject.lock()->Clone();

		m_pSelectedPrefabGroupObject = pGroupObject;

		m_pPickedGroupObject.reset();
	}
	
	else
		return;
}

void CObjectTool::EditGizmo()
{
	shared_ptr<CGameObject> pObject{};

	if (nullptr != m_pPickedObject.lock())
		pObject = static_pointer_cast<CGameObject>(m_pPickedObject.lock());

	if (nullptr != m_pPickedGroupObject.lock())
		pObject = static_pointer_cast<CGameObject>(m_pPickedGroupObject.lock());

	if (nullptr == pObject)
		ImGui::SetNextItemOpen(false);

	if (nullptr != pObject)
		ImGui::SetNextItemOpen(true);

	if (ImGui::TreeNodeEx("Edit Gizmo", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (nullptr == pObject)
		{
			MSG_BOX("Unable to Access : Selected Object Nullptr!");
			ImGui::TreePop();
			return;
		}

		_float4x4 matPickedObj = pObject->GetTransform()->GetWorldMatrix();

		static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);

		float vMeshMatrix[16] = {
			   matPickedObj._11, matPickedObj._12, matPickedObj._13, matPickedObj._14,
			   matPickedObj._21, matPickedObj._22, matPickedObj._23, matPickedObj._24,
			   matPickedObj._31, matPickedObj._32, matPickedObj._33, matPickedObj._34,
			   matPickedObj._41, matPickedObj._42, matPickedObj._43, matPickedObj._44,
		};

		// ���콺 
		if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;

		// �Է�â
		float vScale[3], vRotation[3], vTranslation[3];
		ImGuizmo::DecomposeMatrixToComponents(vMeshMatrix, vTranslation, vRotation, vScale);
		ImGui::InputFloat3("S", vScale, "%.5f");
		ImGui::InputFloat3("R", vRotation, "%.5f");
		ImGui::InputFloat3("T", vTranslation, "%.5f");
		ImGuizmo::RecomposeMatrixFromComponents(vTranslation, vRotation, vScale, vMeshMatrix);

		// ���� & ���� ��ư
		if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
				mCurrentGizmoMode = ImGuizmo::WORLD;
		}


		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::Manipulate(GAMEINSTANCE->GetTransformMatrix(CPipeLine::D3DTS_VIEW).m[0], GAMEINSTANCE->GetTransformMatrix(CPipeLine::D3DTS_PROJ).m[0], mCurrentGizmoOperation, mCurrentGizmoMode, vMeshMatrix, NULL, NULL);
		//_float4x4;
		_float4 vRight{ vMeshMatrix[0], vMeshMatrix[1],vMeshMatrix[2], vMeshMatrix[3] };
		_float4 vUp{ vMeshMatrix[4], vMeshMatrix[5],vMeshMatrix[6], vMeshMatrix[7] };
		_float4 vLook{ vMeshMatrix[8], vMeshMatrix[9],vMeshMatrix[10], vMeshMatrix[11] };
		_float4 vPos{ vMeshMatrix[12], vMeshMatrix[13],vMeshMatrix[14], vMeshMatrix[15] };

		_float4x4 matWorld(vRight, vUp, vLook, vPos);

		pObject->GetTransform()->SetWorldMatrix(matWorld);
	ImGui::TreePop();
	}
}
#pragma endregion

shared_ptr<CObjectTool> CObjectTool::Create()
{
	shared_ptr<CObjectTool> pInstance = WRAPPING(CObjectTool)();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CObjectTool");
		pInstance.reset();
	}

	return pInstance;
}