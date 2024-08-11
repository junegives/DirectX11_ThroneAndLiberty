#include "MapTool.h"

#include "ToolMgr.h"

#include "Transform.h"
#include "Model.h"
#include "VIBuffer.h"
#include "Mesh.h"
#include "Material.h"

#include "TerrainTool.h"
#include "ObjectTool.h"

#include "CameraFree.h"
#include "Map.h"
#include "Area.h"
#include "Terrain.h"
#include "EnvironmentObject.h"
#include "GroupObject.h"

CMapTool::CMapTool()
{
}

#ifdef CHECK_REFERENCE_COUNT
CMapTool::~CMapTool()
{
}
#endif // CHECK_REFERENCE_COUNT

HRESULT CMapTool::Initialize()
{
#pragma region Ready Map
	MakeList(m_strOriginMapDataFilePath);
	m_pMapTransform = CTransform::Create(DEVICE, CONTEXT, nullptr);
#pragma endregion

	m_bIsMapToolWindow = true;

	m_pTerrainTool = MANAGER->GetTerrainTool();
	m_pObjectTool = MANAGER->GetObjectTool();

	return S_OK;
}

void CMapTool::Tick(_float _fTimeDelta)
{
	//ImGui::Begin("MapTool", &m_bIsMapToolWindow, ImGuiWindowFlags_NoResize);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	ImGui::Begin("MapTool", &m_bIsMapToolWindow, ImGuiWindowFlags_None);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	{
		if (ImGui::CollapsingHeader("Map"))
		{
			FileMap();

			EditArea();
		}

		if (nullptr == m_pMap.lock())
			ImGui::SetNextItemOpen(false);

		if (ImGui::CollapsingHeader("Object"))
		{
			if (nullptr != m_pMap.lock())
				m_pObjectTool.lock()->Tick(_fTimeDelta);
			else
				MSG_BOX("Unable to Access: Terrain Nullptr!");
		}

		if (ImGui::CollapsingHeader("Terrain"))
		{
			m_pTerrainTool.lock()->Tick(_fTimeDelta);
		}

		ImGui::End();
	}
	
	if (GAMEINSTANCE->KeyDown(DIK_K) && false == MANAGER->GetIsHotkeyLock())
	{
		shared_ptr<CArea> pTempArea = GetCerrentEditModeArea();
		//string strAreaName = pTempArea->GetAreaName();
		//string AddAreaName = "De";

		//string EditAreaName = AddAreaName + strAreaName;

		pTempArea->SetAreaName(m_strCreateAreaName);
	}
	if (GAMEINSTANCE->KeyDown(DIK_M) && false == MANAGER->GetIsHotkeyLock())
	{
		shared_ptr<CArea> pTempArea = GetCerrentEditModeArea();
		m_vecTempArea.push_back(pTempArea);
		pTempArea->SetType(CArea::TYPE_WAIT);
		vector<shared_ptr<CArea>>& vecAreas = m_pMap.lock()->GetAreas();
		vecAreas.erase(vecAreas.begin() + m_iCurEditModeAreaIndex);
		m_iSelectedEditModeAreaIndex = 0;
		m_iCurEditModeAreaIndex = 0;
	}
	if (GAMEINSTANCE->KeyDown(DIK_N) && false == MANAGER->GetIsHotkeyLock())
	{
		vector<shared_ptr<CArea>>& vecAreas = m_pMap.lock()->GetAreas();

		for (auto pArea : m_vecTempArea)
			vecAreas.push_back(pArea);

		m_vecTempArea.clear();

	}
	
}

void CMapTool::FileMap()
{
#pragma region Load Map
		if (ImGui::TreeNodeEx("Load Map", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PushItemWidth(200);
			SelectedComboListIndex("##Load Map Data File List", m_vecLoadMapDataFile, m_SelectedLoadMapDataFileIndex); ImGui::SameLine();
			ImGui::Text("Map Name(.dat)");

			if (ImGui::Button("Load", ImVec2(96, 19)))
				LoadMap();

			ImGui::SameLine();

			if (ImGui::Button("Reload", ImVec2(96, 19)))
			{
				m_vecLoadMapDataFile.clear();
				MakeList(m_strOriginMapDataFilePath);
			}

			ImGui::TreePop();
		}
#pragma endregion

#pragma region Save Map
		if (ImGui::TreeNodeEx("Save Map", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//##Save Map Data File List;
			
			ImGui::PushItemWidth(200);
			SelectedComboListIndex("##Save Map Data File Mode List", m_vecSaveMapDataFileMode, m_iSelectedSaveMapDataFileModeIndex); ImGui::SameLine();
			ImGui::Text("Save Mode");
			
			ImGui::Checkbox("Save Terrain", &m_bIsTerrainSaveNeeded);
			ImGui::SameLine();
			ImGui::Checkbox("Save Areas", &m_bIsAreasSaveNeeded);

			m_strSaveMapDataFileName.resize(MAX_PATH);
			ImGui::PushItemWidth(200);
			ImGui::InputText("##Save Map Data File Name", &m_strSaveMapDataFileName[0], m_strSaveMapDataFileName.size()); ImGui::SameLine();
			m_strSaveMapDataFileName.resize(m_strSaveMapDataFileName.find_last_not_of('\0') + 1);  // ���ڿ��� ������ ���� ���ķ� �߶���ϴ�.
			ImGui::Text("Map Name(.dat)");

			if (ImGui::Button("Save", ImVec2(96, 19)))
			{
				if (true == m_strSaveMapDataFileName.empty())
					MSG_BOX("Empty File Name : Map Data!");
				else
				{
					if (nullptr == m_pMap.lock())
						MSG_BOX("Failed : Map Nullptr!");
					else
						SaveMap(static_cast<SAVE>(m_iSelectedSaveMapDataFileModeIndex));
				}
			}
			
			ImGui::SameLine();

			if (ImGui::Button("Resource Extract", ImVec2(96, 19)))
			{
				ResourceExtract(static_cast<SAVE>(m_iSelectedSaveMapDataFileModeIndex));
			}
			
		ImGui::TreePop();
		}
#pragma endregion

#pragma region Clear Map
		if (ImGui::TreeNodeEx("Clear Map", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Button("Delete", ImVec2(96, 19)))
			{
				if (nullptr == m_pMap.lock())
					MSG_BOX("Failed: Map Nullptr!");
				else
				{
					//shared_ptr<CTerrain> pTerrain = static_pointer_cast<CTerrain>(GAMEINSTANCE->GetGameObject(GAMEINSTANCE->GetCurrentLevel(), TEXT("Layer_Terrain")));
					if (nullptr != MANAGER->GetTerrainTool()->GetTerrain())
						MSG_BOX("warning : Save and Delete Terrain");
					else
					{
						MANAGER->GetObjectTool()->GetObjectsID().clear();
						m_pMap.lock()->SetActive(false);
					}
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Reset", ImVec2(96, 19)))
			{
				if (nullptr == m_pMap.lock())
					MSG_BOX("Failed : Map Nullptr!");
				else
				{
					shared_ptr<CTerrain> pTerrain = static_pointer_cast<CTerrain>(GAMEINSTANCE->GetGameObject(GAMEINSTANCE->GetCurrentLevel(), TEXT("Layer_Terrain")));
					if (nullptr != pTerrain)
						MSG_BOX("warning : Save and Delete Terrain");
					else
					{
						MANAGER->GetObjectTool()->GetObjectsID().clear();
						m_pMap.lock()->ResetMap();
					}
				}
			}

			ImGui::TreePop();
		}
#pragma endregion
	
	return;
}

HRESULT CMapTool::LoadMap()
{
	// ���� �ͷ��ΰ� ������Ʈ�� �ִٸ�! ���
	if (nullptr != m_pTerrainTool.lock()->GetTerrain())
	{
		MSG_BOX("Failed to Read : Terrain in progress remains.");
		return E_FAIL;
	}
	if (false == m_pObjectTool.lock()->IsObjectsEmpty())
	{
		MSG_BOX("Failed to Read : Objects in progress remains.");
		return E_FAIL;
	}

	
	string strSelectMapFilePath = m_strOriginMapDataFilePath + m_vecLoadMapDataFile[m_SelectedLoadMapDataFileIndex];



	ifstream InBinaryFile(strSelectMapFilePath, ios::in | ios::binary);
	if (InBinaryFile.is_open())
	{
		//DungeonMap.dat
		//m_pMap = CMap::Create("DungeonMap.dat");
		//GAMEINSTANCE->AddObject(GAMEINSTANCE->GetCurrentLevel(), TEXT("Layer_Map"), m_pMap);

		// �ش� Map�� Name ����.
		char szMapName[MAX_PATH]{};
		InBinaryFile.read(szMapName, sizeof(szMapName));
		
		// �ش� Map�� World Matrix ����.
		_float4x4 matMapWorld{};
		InBinaryFile.read(reinterpret_cast<char*>(&matMapWorld), sizeof(_float4x4));

		shared_ptr<CMap> pMap = CMap::Create(szMapName, matMapWorld);
		pMap->SetEditModeArea(&m_iCurEditModeAreaIndex);
		GAMEINSTANCE->AddObject(GAMEINSTANCE->GetCurrentLevel(), TEXT("Layer_Map"), pMap);
		m_pMap = pMap;

		
		_bool bIsTerrain{};
		InBinaryFile.read(reinterpret_cast<char*>(&bIsTerrain), sizeof(_bool));
		if (true == bIsTerrain)
		{
			_char szTerrainDataFileName[MAX_PATH] = "";
			InBinaryFile.read(szTerrainDataFileName, sizeof(szTerrainDataFileName));
			string strTerrainDataFileName = szTerrainDataFileName;
			if (FAILED(MANAGER->GetTerrainTool()->LoadTerrain(strTerrainDataFileName)))
			{
				MSG_BOX("Failed: Terrain Read!");
				InBinaryFile.close();
				return E_FAIL;
			}
		}
		

		// �ش� Map�� Area ���� ����.
		_uint iNumAreas{};
		InBinaryFile.read(reinterpret_cast<char*>(&iNumAreas), sizeof(_uint));

		// �� Area�� ���� Name�� World Matrix ����.
		for (_uint iCntAreas = 0; iCntAreas < iNumAreas; iCntAreas++)
		{
			// Area�� Name
			_char szAreaName[MAX_PATH] = "";
			InBinaryFile.read(szAreaName, sizeof(szAreaName));

			// Area�� World Matrix
			_float4x4 matAreaWorld{};
			InBinaryFile.read(reinterpret_cast<char*>(&matAreaWorld), sizeof(_float4x4));

			shared_ptr<CArea> pArea = CArea::Create(szAreaName, matAreaWorld);

			m_pMap.lock()->AddArea(pArea);


			/////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////

			// Area�� ���� Model ��
			_uint iNumModels{};
			InBinaryFile.read(reinterpret_cast<char*>(&iNumModels), sizeof(_uint));

			for (_uint iCntModels = 0; iCntModels < iNumModels; iCntModels++)
			{
				// Model�� Key
				_char szModelKey[MAX_PATH] = "";
				InBinaryFile.read(szModelKey, sizeof(szModelKey));

				// �ش� Model�� �����ϴ� Objects�� ��
				_uint iNumObjects{};
				InBinaryFile.read(reinterpret_cast<char*>(&iNumObjects), sizeof(_uint));

				for (_uint iCntObjects = 0; iCntObjects < iNumObjects; iCntObjects++)
				{
					_float4x4 matObjectWorld{};
					InBinaryFile.read(reinterpret_cast<char*>(&matObjectWorld), sizeof(_float4x4));

					_float3 vCenterPoint{};
					InBinaryFile.read(reinterpret_cast<char*>(&vCenterPoint), sizeof(_float3));

					_float fRadius{};
					InBinaryFile.read(reinterpret_cast<char*>(&fRadius), sizeof(_float));

					shared_ptr<CEnvironmentObject> pObject = CEnvironmentObject::Create(szModelKey, matObjectWorld);
					pArea->AddObject(pObject);


					pObject->SetCenterPoint(vCenterPoint);
					pObject->SetRadius(fRadius);
					pObject->SetCollider();
				}


			}

			/////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////
			
			_uint iNumGroupObjects{};
			InBinaryFile.read(reinterpret_cast<char*>(&iNumGroupObjects), sizeof(_uint));

			for (_uint iCntGroupObjects = 0; iCntGroupObjects < iNumGroupObjects; iCntGroupObjects++)
			{ 
				_char szGroupObjectName[MAX_PATH] = "";
				InBinaryFile.read(szGroupObjectName, sizeof(szGroupObjectName));

				_float4x4 matGroupObjectWorld{};
				InBinaryFile.read(reinterpret_cast<char*>(&matGroupObjectWorld), sizeof(_float4x4));

				_uint iNumMemObjects{};
				InBinaryFile.read(reinterpret_cast<char*>(&iNumMemObjects), sizeof(_uint));

				vector<CGroupObject::MEMBER_DESC> vecMemberObjectDesc(iNumMemObjects);
			
				for (_uint iCntMemObjects = 0; iCntMemObjects < iNumMemObjects; iCntMemObjects++)
				{
					CGroupObject::MEMBER_DESC MemberObjectDesc{};
					// �̸�
					_char szMemberObjectName[MAX_PATH] = "";
					InBinaryFile.read(szMemberObjectName, sizeof(szMemberObjectName));
					MemberObjectDesc.strModelKey = szMemberObjectName;

					// �������
					_float4x4 matMemverObjectWorld{};
					InBinaryFile.read(reinterpret_cast<char*>(&matMemverObjectWorld), sizeof(_float4x4));
					MemberObjectDesc.matWorld = matMemverObjectWorld;

					// ����
					_float3 vCenterPoint{};
					InBinaryFile.read(reinterpret_cast<char*>(&vCenterPoint), sizeof(_float3));
					MemberObjectDesc.vCenterPoint = vCenterPoint;

					// ������
					_float fRadius{};
					InBinaryFile.read(reinterpret_cast<char*>(&fRadius), sizeof(_float));
					MemberObjectDesc.fRadius = fRadius;

					vecMemberObjectDesc[iCntMemObjects] = MemberObjectDesc;
				}

				pArea->AddGroupObject(CGroupObject::Create(szGroupObjectName, matGroupObjectWorld, vecMemberObjectDesc));
			}
			
			
		}
		


		//if (FAILED(m_pTerrainTool.lock()->LoadTerrain(InBinaryFile)))
		//	return E_FAIL;

		//if (FAILED(m_pObjectTool.lock()->LoadObjects(InBinaryFile)))
		//	return E_FAIL;

		InBinaryFile.close();
	}
	else
	{
		MSG_BOX("Failed to Read : Binary File");
		return E_FAIL;
	}

	MSG_BOX("Read Binary Success!");

	return S_OK;
}

HRESULT CMapTool::SaveMap(SAVE _eSaveType)
{
	// ���� ��Ʈ���� ����
	// �ͷ����� ���� ���� = ��....
	// ���ӿ�����Ʈ�� ���� ���� = �������

	string strSaveMapFilePath = m_strOriginMapDataFilePath + m_strSaveMapDataFileName + ".dat";

	ofstream OutBinaryFile(strSaveMapFilePath, ios::out | ios::binary);
	if (OutBinaryFile.is_open())
	{

		// �ش� Map�� Name ����.
		_char szMapName[MAX_PATH] = "";
		strcpy_s(szMapName, m_strSaveMapDataFileName.c_str());
		OutBinaryFile.write(szMapName, sizeof(szMapName));

		// �ش� Map�� World Matrix ����.
		_float4x4 matMapWorld = m_pMap.lock()->GetTransform()->GetWorldMatrix();
		OutBinaryFile.write(reinterpret_cast<const char*>(&matMapWorld), sizeof(_float4x4));

		// �ش� �ʿ� �ͷ��� ������ �ʿ��Ѱ�?
		OutBinaryFile.write(reinterpret_cast<const char*>(&m_bIsTerrainSaveNeeded), sizeof(_bool));
		if (true == m_bIsTerrainSaveNeeded)
		{
			// ���� �ͷ����� �������� �ʴ´ٸ�
			if (nullptr == MANAGER->GetTerrainTool()->GetTerrain())
			{
				MSG_BOX("Failed : Terrain Nullptr!");
				OutBinaryFile.close();
				return E_FAIL;
			}

			// �ͷ��� ���� �̸�
			string strSaveTerrainDataFileName = MANAGER->GetTerrainTool()->GetSaveTerrainDataFileName();
			
			// ���� �ͷ��� �̸��� ����ִٸ�.
			if (true == strSaveTerrainDataFileName.empty())
			{
				MSG_BOX("Empty File Name : Terrain Data!");
				OutBinaryFile.close();
				return E_FAIL;
			}
			else
			{
				_char szSaveTerrainDataFileName[MAX_PATH] = "";
				//strSaveTerrainDataFileName = strSaveTerrainDataFileName + ".dat";
				strcpy_s(szSaveTerrainDataFileName, strSaveTerrainDataFileName.c_str());
				OutBinaryFile.write(szSaveTerrainDataFileName, sizeof(szSaveTerrainDataFileName));
				
				// �ͷ��� ���� ��� ����
				if (FAILED(MANAGER->GetTerrainTool()->SaveFileNameCheck()))
				{
					MSG_BOX("Failed: Terrain Saved!");
					OutBinaryFile.close();
					return E_FAIL;
				}
			}

			


		}

		// �ش� Map�� Area ���� ����.
		vector<shared_ptr<CArea>> vecSaveAreas{};

		if (SAVE_PART == _eSaveType)
		{
			vector<shared_ptr<CArea>>& vecAreas = m_pMap.lock()->GetAreas();
			for (const auto pArea : vecAreas)
			{
				if ("Default" == pArea->GetAreaName())
				{
					vecSaveAreas.push_back(pArea);
					continue;
				}

				if (CArea::TYPE_VIEW == pArea->GetType())
				{
					vecSaveAreas.push_back(pArea);
				}
			}
		}
		else // (SAVE_FULL == _eSaveType)
		{
			vecSaveAreas = m_pMap.lock()->GetAreas();
		}

		_uint iNumAreas = static_cast<_uint>(vecSaveAreas.size());
		OutBinaryFile.write(reinterpret_cast<const char*>(&iNumAreas), sizeof(_uint));
		
		// �� Area�� ���� Name�� World Matrix ����.
		for (const auto pArea : vecSaveAreas)
		{
			// Area�� Name
			_char szAreaName[MAX_PATH] = "";
			strcpy_s(szAreaName, pArea->GetAreaName().c_str());
			OutBinaryFile.write(szAreaName, sizeof(szAreaName));
			// Area�� World Matrix
			_float4x4 matAreaWorld = pArea->GetTransform()->GetWorldMatrix();
			OutBinaryFile.write(reinterpret_cast<const char*>(&matAreaWorld), sizeof(_float4x4));

			/////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////


			// Area�� ���� Model ��
			map<string, vector<shared_ptr<CEnvironmentObject>>>& mapObjects = pArea->GetObjects();

			_uint iNumModels = static_cast<_uint>(mapObjects.size());
			OutBinaryFile.write(reinterpret_cast<const char*>(&iNumModels), sizeof(_uint));

			for (const auto& pair : mapObjects)
			{
				_char szModelKey[MAX_PATH] = "";
				strcpy_s(szModelKey, pair.first.c_str());
				OutBinaryFile.write(szModelKey, sizeof(szModelKey));

				// �ش� Model�� �����ϴ� Objects�� ��
				_uint iNumObjects = static_cast<_uint>(pair.second.size());
				OutBinaryFile.write(reinterpret_cast<const char*>(&iNumObjects), sizeof(_uint));

				// �� Object�� ���� world Matrix�� Object Model�� ����, ������ ���.
				for (const auto pObject : pair.second)
				{
					_float4x4 matObjectWorld = pObject->GetTransform()->GetWorldMatrix();
					OutBinaryFile.write(reinterpret_cast<const char*>(&matObjectWorld), sizeof(_float4x4));
					
					_float fMaxX{ -numeric_limits<float>::infinity() };
					_float fMinX{ numeric_limits<float>::infinity() };

					_float fMaxY{ -numeric_limits<float>::infinity() };
					_float fMinY{ numeric_limits<float>::infinity() };

					_float fMaxZ{ -numeric_limits<float>::infinity() };
					_float fMinZ{ numeric_limits<float>::infinity() };

					vector<shared_ptr<CMesh>> vecMeshes = pObject->GetModel()->GetMeshes();
					
					for (auto pMesh : vecMeshes)
					{

						
						vector<_float3> vecVerticesPos = *(static_pointer_cast<CVIBuffer>(pMesh)->GetVerticesPos());
						
						for (const auto& vVertexPos : vecVerticesPos)
						{
							_float3 vVertexWorldPos = SimpleMath::Vector3::Transform(vVertexPos, matObjectWorld);

							fMaxX = max(fMaxX, vVertexWorldPos.x);
							fMaxY = max(fMaxY, vVertexWorldPos.y);
							fMaxZ = max(fMaxZ, vVertexWorldPos.z);

							fMinX = min(fMinX, vVertexWorldPos.x);
							fMinY = min(fMinY, vVertexWorldPos.y);
							fMinZ = min(fMinZ, vVertexWorldPos.z);
						}
						
					}
					_float3 vCenterPoint{ (fMaxX + fMinX) * 0.5f, (fMaxY + fMinY) * 0.5f, (fMaxZ + fMinZ) * 0.5f };
					OutBinaryFile.write(reinterpret_cast<const char*>(&vCenterPoint), sizeof(_float3));


					_float fRadius{};
					fRadius = max(fRadius, vCenterPoint.x - fMinX);
					fRadius = max(fRadius, vCenterPoint.y - fMinY);
					fRadius = max(fRadius, vCenterPoint.z - fMinZ);
					OutBinaryFile.write(reinterpret_cast<const char*>(&fRadius), sizeof(_float));
					
					

				}
			}

			/////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////

			// 1. �ش� Area�� ������ �׷� ������Ʈ ��
			vector<shared_ptr<CGroupObject>>& vecGroupObjects = pArea->GetGroupObjects();

			_uint iNumGroupObjects = static_cast<_uint>(vecGroupObjects.size());
			OutBinaryFile.write(reinterpret_cast<const char*>(&iNumGroupObjects), sizeof(_uint));

			// 2. �� �׷� ������Ʈ�� ���� �̸�, ��� �׸��� ��� ������Ʈ ��
			for (auto pGroupObject : vecGroupObjects)
			{
				// �̸�
				_char szNumGroupObjectName[MAX_PATH] = "";
				strcpy_s(szNumGroupObjectName, pGroupObject->GetGroupObjectName().c_str());
				OutBinaryFile.write(szNumGroupObjectName, sizeof(szNumGroupObjectName));

				// ���
				_float4x4 matGroupObjectWorld = pGroupObject->GetTransform()->GetWorldMatrix();
				OutBinaryFile.write(reinterpret_cast<const char*>(&matGroupObjectWorld), sizeof(_float4x4));

				// ��� ������Ʈ�� ��
				vector<shared_ptr<CEnvironmentObject>>& vecMemberObjects = pGroupObject->GetMemberObjects();
				_uint iNumMemberObjects = static_cast<_uint>(vecMemberObjects.size());
				OutBinaryFile.write(reinterpret_cast<const char*>(&iNumMemberObjects), sizeof(_uint));

				for (const auto pMemberObject : vecMemberObjects)
				{
					// �ɹ� ������Ʈ�� �̸�
					_char szMemberObjectModelKey[MAX_PATH] = "";
					strcpy_s(szMemberObjectModelKey, pMemberObject->GetModelKey().c_str());
					OutBinaryFile.write(szMemberObjectModelKey, sizeof(szMemberObjectModelKey));

					// �ɹ� ������Ʈ�� ���� ���
					_float4x4 matMemberObjectWorld = pMemberObject->GetTransform()->GetWorldMatrix();
					OutBinaryFile.write(reinterpret_cast<const char*>(&matMemberObjectWorld), sizeof(_float4x4));

					_float fMaxX{ -numeric_limits<float>::infinity() };
					_float fMinX{ numeric_limits<float>::infinity() };

					_float fMaxY{ -numeric_limits<float>::infinity() };
					_float fMinY{ numeric_limits<float>::infinity() };

					_float fMaxZ{ -numeric_limits<float>::infinity() };
					_float fMinZ{ numeric_limits<float>::infinity() };

					vector<shared_ptr<CMesh>> vecMeshes = pMemberObject->GetModel()->GetMeshes();

					for (auto pMesh : vecMeshes)
					{

						// ��ȯ ��� = ���������Ʈ ���� * �׷� ������Ʈ ����
						vector<_float3> vecVerticesPos = *(static_pointer_cast<CVIBuffer>(pMesh)->GetVerticesPos());

						for (const auto& vVertexPos : vecVerticesPos)
						{
							_float4x4 matTransformWolrd = matMemberObjectWorld * matGroupObjectWorld;

							_float3 vVertexWorldPos = SimpleMath::Vector3::Transform(vVertexPos, matTransformWolrd);

							fMaxX = max(fMaxX, vVertexWorldPos.x);
							fMaxY = max(fMaxY, vVertexWorldPos.y);
							fMaxZ = max(fMaxZ, vVertexWorldPos.z);

							fMinX = min(fMinX, vVertexWorldPos.x);
							fMinY = min(fMinY, vVertexWorldPos.y);
							fMinZ = min(fMinZ, vVertexWorldPos.z);
						}

					}

					_float3 vCenterPoint{ (fMaxX + fMinX) * 0.5f, (fMaxY + fMinY) * 0.5f, (fMaxZ + fMinZ) * 0.5f };
					OutBinaryFile.write(reinterpret_cast<const char*>(&vCenterPoint), sizeof(_float3));


					_float fRadius{};
					fRadius = max(fRadius, vCenterPoint.x - fMinX);
					fRadius = max(fRadius, vCenterPoint.y - fMinY);
					fRadius = max(fRadius, vCenterPoint.z - fMinZ);
					OutBinaryFile.write(reinterpret_cast<const char*>(&fRadius), sizeof(_float));



				}
			}
		}



		//if (FAILED(m_pTerrainTool.lock()->SaveTerrain(OutBinaryFile)))
		//	return E_FAIL;



		//if (FAILED(m_pObjectTool.lock()->SaveObjects(OutBinaryFile)))
		//	return E_FAIL;

		OutBinaryFile.close();
	}
	else // ���̳ʸ� ���� ����
	{
		MSG_BOX("WriteBinary Failed!");
		return E_FAIL;
	}

	MSG_BOX("WriteBinary Success!");

	return S_OK;
}

void CMapTool::EditArea()
{
	if (nullptr == m_pMap.lock())
		ImGui::SetNextItemOpen(false);

	//if (ImGui::TreeNodeEx("Edit Map", ImGuiTreeNodeFlags_DefaultOpen))
	//{
	if (ImGui::CollapsingHeader("Area"))
	{
		if (nullptr != m_pMap.lock())
		{
#pragma region Create Area
			if (ImGui::TreeNodeEx("Create Area", ImGuiTreeNodeFlags_DefaultOpen))
			{
				m_strCreateAreaName.resize(MAX_PATH);
				ImGui::PushItemWidth(200);
				ImGui::InputText("##Create Area Name", &m_strCreateAreaName[0], m_strCreateAreaName.size());
				m_strCreateAreaName.resize(m_strCreateAreaName.find_last_not_of('\0') + 1);  // ���ڿ��� ������ ���� ���ķ� �߶���ϴ�.
				ImGui::SameLine();
				ImGui::Text("Area Name");

				if (ImGui::Button("Create Area", ImVec2(200, 19)))
				{
					m_pMap.lock()->AddArea(CArea::Create(m_strCreateAreaName));
					m_strCreateAreaName.clear();
				}
			ImGui::TreePop();
			}

#pragma endregion // Create Area
			if (ImGui::TreeNodeEx("Edit Area", ImGuiTreeNodeFlags_DefaultOpen))
			{
				EditModeArea();

				ViewModeArea();

				ChangedModeArea();

			ImGui::TreePop();
			}
		}
		else
			MSG_BOX("Unable to Access: Map Nullptr!");

		//ImGui::TreePop();
		
	}

}

void CMapTool::EditModeArea()
{

#pragma region Change Edit Mode Area
	m_bIsChangedEditModeArea = false;

	vector<shared_ptr<CArea>>& vecAreas = m_pMap.lock()->GetAreas();
	_uint iNumAreas = static_cast<_uint>(vecAreas.size());

	vector<string> strAreaNames{};
	for (const auto pArea : vecAreas)
		strAreaNames.push_back(pArea->GetAreaName());
	SelectedComboListIndex("##Selected Edit Mode Area", strAreaNames, m_iSelectedEditModeAreaIndex); ImGui::SameLine();
	ImGui::Text("Edit Mode Name");

	// Edit Mode�� Area�� ����Ǿ��ٸ�!
	if (m_iCurEditModeAreaIndex != m_iSelectedEditModeAreaIndex)
	{
		m_bIsChangedEditModeArea = true;
		// Area Type ��ü.
		vecAreas[m_iCurEditModeAreaIndex]->SetType(CArea::TYPE_WAIT);
		MANAGER->GetObjectTool()->ResetEdge(vecAreas[m_iCurEditModeAreaIndex]);
		vecAreas[m_iSelectedEditModeAreaIndex]->SetType(CArea::TYPE_EDIT);
		m_iCurEditModeAreaIndex = m_iSelectedEditModeAreaIndex;
	}
#pragma endregion // Change Edit Mode Area

	if (ImGui::Button("Delete", ImVec2(96, 19)))
	{
		// ���� ���� ����� ������� ��� ���Ҹ� �����, ���� �����
		string strDefault = "Default";
		_uint iDefaultAreaIndex{};

		vector<shared_ptr<CArea>>& vecAreas = m_pMap.lock()->GetAreas();

		_uint iNumAreas = static_cast<_uint>(vecAreas.size());

		if (0 == iNumAreas)
		{
			vecAreas.push_back(CArea::Create(strDefault));
			m_iSelectedEditModeAreaIndex = 0;
			vecAreas[m_iSelectedEditModeAreaIndex]->SetType(CArea::TYPE_EDIT);
			m_iCurEditModeAreaIndex = m_iSelectedEditModeAreaIndex;
		}
		else
		{
			for (_uint iCntAreas = 0; iCntAreas < iNumAreas; iCntAreas++)
			{
				if (strDefault == vecAreas[iCntAreas]->GetAreaName())
				{
					iDefaultAreaIndex = iCntAreas;
					break;
				}
			}

			if (iDefaultAreaIndex == m_iSelectedEditModeAreaIndex)
				MSG_BOX("Failed to Delete : Current Area is Default!");
			else
			{
				// ���� ������ �� ��������


				vecAreas[m_iSelectedEditModeAreaIndex]->SetActive(false);
				vecAreas.erase(vecAreas.begin() + m_iSelectedEditModeAreaIndex);
				m_iSelectedEditModeAreaIndex = iDefaultAreaIndex;
				vecAreas[m_iSelectedEditModeAreaIndex]->SetType(CArea::TYPE_EDIT);
				m_iCurEditModeAreaIndex = m_iSelectedEditModeAreaIndex;
			}
		}
	}
		
	ImGui::SameLine();

	if (ImGui::Button("Reset", ImVec2(96, 19)))
	{
		vector<shared_ptr<CArea>>& vecAreas = m_pMap.lock()->GetAreas();

		string strDefault = "Default";
		_uint iNumAreas = static_cast<_uint>(vecAreas.size());

		if (0 == iNumAreas)
		{
			vecAreas.push_back(CArea::Create(strDefault));
			m_iSelectedEditModeAreaIndex = 0;
			vecAreas[m_iSelectedEditModeAreaIndex]->SetType(CArea::TYPE_EDIT);
			m_iCurEditModeAreaIndex = m_iSelectedEditModeAreaIndex;
		}
		else
			vecAreas[m_iSelectedEditModeAreaIndex]->GetObjects().clear();
	}

}

void CMapTool::ViewModeArea()
{
	m_bIsChangedViewModeArea = false;

	

	_uint iIndex{};

	ImGuiChildFlags_ ChildFlags = ImGuiChildFlags_Border;
	ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_None | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar;
	//_float fSizeX = ImGui::GetContentRegionAvail().x * 0.49f;
	//_float fSizeY = ImGui::GetContentRegionAvail().y * 0.49f;
	_float fSizeX = ImGui::GetContentRegionAvail().x * 0.49f;

	vector<shared_ptr<CArea>>& vecAreas = m_pMap.lock()->GetAreas();
	vector<shared_ptr<CArea>> vecViewModeAreas{};
	vector<shared_ptr<CArea>> vecWaitModeAreas{};
	
	for (const auto pArea : vecAreas)
	{
		switch (pArea->GetType())
		{
		case CArea::TYPE_EDIT:
			break;

		case CArea::TYPE_VIEW:
			vecViewModeAreas.push_back(pArea);
			break;

		case CArea::TYPE_WAIT:
			vecWaitModeAreas.push_back(pArea);
			break;
		}
	}

	vector<string> vecCurViewModeAreaNames{};
	for (const auto pArea : vecViewModeAreas)
		vecCurViewModeAreaNames.push_back(pArea->GetAreaName());
	
	if (vecCurViewModeAreaNames.size() != m_vecCurViewModeAreaNames.size())
		m_bIsChangedViewModeArea = true;
	else
	{
		sort(vecCurViewModeAreaNames.begin(), vecCurViewModeAreaNames.end());
		sort(m_vecCurViewModeAreaNames.begin(), m_vecCurViewModeAreaNames.end());

		if (false == equal(vecCurViewModeAreaNames.begin(), vecCurViewModeAreaNames.end(), m_vecCurViewModeAreaNames.begin()))
			m_bIsChangedViewModeArea = true;
	}

	m_vecCurViewModeAreaNames = vecCurViewModeAreaNames;
	
	ImGui::BeginChild("##Show View Mode Area List", ImVec2(fSizeX, 150), ChildFlags, WindowFlags);
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::Text("View Mode");
			ImGui::EndMenuBar();
		}

		_uint iNumViewModeArea = static_cast<_uint>(vecViewModeAreas.size());
		static _uint iCurIndex{};
		//if (ImGui::BeginListBox("##View Mode Area List", ImVec2(fSizeX - 15.f, fSizeY - 14.f)))
		if (ImGui::BeginListBox("##View Mode Area List", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))

		{
			for (_uint iCntViewModeArea = 0; iCntViewModeArea < iNumViewModeArea; iCntViewModeArea++)
			{
				_bool bIsSelected = (iIndex == iCntViewModeArea);
				if (ImGui::Selectable(vecViewModeAreas[iCntViewModeArea]->GetAreaName().c_str(), bIsSelected))
				{
					iIndex = iCntViewModeArea;
					vecViewModeAreas[iIndex]->SetType(CArea::TYPE_WAIT);
				}
				if (bIsSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
		ImGui::EndChild();
	}

	ImGui::SameLine();

	ImGui::BeginChild("##Show Wait Mode Area List", ImVec2(fSizeX, 150), ChildFlags, WindowFlags);
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::Text("Wait Mode");
			ImGui::EndMenuBar();
		}
		
		_uint iNumWaitModeArea = static_cast<_uint>(vecWaitModeAreas.size());
		static _uint iCurIndex{};
		//if (ImGui::BeginListBox("##Wait Mode Area List", ImVec2(fSizeX - 15.f, fSizeY - 14.f)))
		if (ImGui::BeginListBox("##Wait Mode Area List", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))

		{
			for (_uint iCntWaitModeArea = 0; iCntWaitModeArea < iNumWaitModeArea; iCntWaitModeArea++)
			{
				_bool bIsSelected = (iIndex == iCntWaitModeArea);
				if (ImGui::Selectable(vecWaitModeAreas[iCntWaitModeArea]->GetAreaName().c_str(), bIsSelected))
				{
					iIndex = iCntWaitModeArea;
					vecWaitModeAreas[iIndex]->SetType(CArea::TYPE_VIEW);

				}
				if (bIsSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
		ImGui::EndChild();
	}

}

void CMapTool::ChangedModeArea()
{
	vector<shared_ptr<CArea>>& vecAreas = m_pMap.lock()->GetAreas();

	map<string, vector<_float4x4>> mapInstObjMats{};

	for (const auto pArea : vecAreas)
	{
		if (CArea::TYPE_EDIT == pArea->GetType() && m_bIsChangedEditModeArea)
		{
			map<_uint, weak_ptr<CEnvironmentObject>>& mapObjsID = m_pObjectTool.lock()->GetObjectsID();
			mapObjsID.clear();
			map<string, vector<shared_ptr<CEnvironmentObject>>>& mapObjects = vecAreas[m_iCurEditModeAreaIndex]->GetObjects();
			for (const auto& pair : mapObjects)
			{
				for (const auto& pObject : pair.second)
					mapObjsID.emplace(pObject->GetObjID(), pObject);
			}

			vector<shared_ptr<CGroupObject>>& vecGroupObjects = vecAreas[m_iCurEditModeAreaIndex]->GetGroupObjects();
			for (auto pGroupObject : vecGroupObjects)
			{
				vector<shared_ptr<CEnvironmentObject>>& vecObjects = pGroupObject->GetMemberObjects();
				for (auto pObject : vecObjects)
				{
					mapObjsID.emplace(pObject->GetObjID(), pObject);
				}
			}
		}

		else if (CArea::TYPE_VIEW == pArea->GetType() && m_bIsChangedViewModeArea)
		{
			map<string, vector<shared_ptr<CEnvironmentObject>>>& mapObjects = pArea->GetObjects();
			for (const auto& pair : mapObjects)
			{
				auto iter = mapInstObjMats.find(pair.first);

				if (mapInstObjMats.end() == iter)
				{
					mapInstObjMats.emplace(pair.first, vector<_float4x4>());
					for (const auto pObject : pair.second)
						mapInstObjMats[pair.first].push_back(pObject->GetTransform()->GetWorldMatrix());
				}
				else
				{
					for (const auto pObject : pair.second)
						iter->second.push_back(pObject->GetTransform()->GetWorldMatrix());
				}
			}

			vector<shared_ptr<CGroupObject>>& vecGroupObjects = pArea->GetGroupObjects();
			for (const auto pGroupObject : vecGroupObjects)
			{
				_float4x4 matGroupObjectWorld = pGroupObject->GetTransform()->GetWorldMatrix();
				vector<shared_ptr<CEnvironmentObject>>& vecMemberObjects = pGroupObject->GetMemberObjects();

				for (const auto pMemberObject : vecMemberObjects)
				{
					string strMemberObjectModelName = pMemberObject->GetModelKey();
					
					_float4x4 matMemberObjectWorld = pMemberObject->GetTransform()->GetWorldMatrix();

					auto iter = mapInstObjMats.find(strMemberObjectModelName);
					
					if (mapInstObjMats.end() == iter)
					{
						mapInstObjMats.emplace(strMemberObjectModelName, vector<_float4x4>());
						mapInstObjMats[strMemberObjectModelName].push_back(matMemberObjectWorld * matGroupObjectWorld);
					}
					else
					{
						mapInstObjMats[strMemberObjectModelName].push_back(matMemberObjectWorld * matGroupObjectWorld);
					}

				}
			}



		}

	}

	if (true == m_bIsChangedViewModeArea)
	{
		vector<shared_ptr<CEnvironmentObject>> vecInstancingObjects{};
		for (const auto& pair : mapInstObjMats)
			vecInstancingObjects.push_back(CEnvironmentObject::Create(pair.first, pair.second));

		m_pMap.lock()->SetInstancingObjects(vecInstancingObjects);
	}
}




HRESULT CMapTool::ShowMapTransform()
{
	ImGui::SeparatorText("Map Transform");

#pragma region Map Scale
	ImGui::PushItemWidth(75);
	m_vScale = m_pMapTransform->GetScaled();
	ImGui::InputFloat("X##ScaleX", &m_vScale.x, 0.f, 0.f, "%.2f"); ImGui::SameLine();
	ImGui::InputFloat("Y##ScaleY", &m_vScale.y, 0.f, 0.f, "%.2f"); ImGui::SameLine();
	ImGui::InputFloat("Z##ScaleZ", &m_vScale.z, 0.f, 0.f, "%.2f");
	m_pMapTransform->SetScaling(m_vScale);
#pragma endregion

#pragma region Map Rotation
	_float3 vRight = m_pMapTransform->GetState(CTransform::STATE_RIGHT);
	_float3 vUp = m_pMapTransform->GetState(CTransform::STATE_UP);
	_float3 vLook = m_pMapTransform->GetState(CTransform::STATE_LOOK);

	vRight.Normalize();
	vUp.Normalize();
	vLook.Normalize();

	_float3 vBasisX(1.f, 0.f, 0.f);
	_float3 vBasisY(0.f, 1.f, 0.f);
	_float3 vBasisZ(0.f, 0.f, 1.f);

	_float fDegreesX = XMConvertToDegrees(acos(vRight.Dot(vBasisX)));
	_float fDegreesY = XMConvertToDegrees(acos(vUp.Dot(vBasisY)));
	_float fDegreesZ = XMConvertToDegrees(acos(vLook.Dot(vBasisZ)));

	ImGui::InputFloat("X##RotationX", &fDegreesX, 0.f, 0.f, "%.2f"); ImGui::SameLine();
	ImGui::InputFloat("Y##RotationY", &fDegreesY, 0.f, 0.f, "%.2f"); ImGui::SameLine();
	ImGui::InputFloat("Z##RotationZ", &fDegreesZ, 0.f, 0.f, "%.2f");
	// Degrees -> Radians
	m_vRotation.x = XMConvertToRadians(fDegreesX);
	m_vRotation.y = XMConvertToRadians(fDegreesY);
	m_vRotation.z = XMConvertToRadians(fDegreesZ);
	m_pMapTransform->RotationAll(m_vRotation.x, m_vRotation.y, m_vRotation.z);
#pragma endregion

#pragma region Map Position
	m_vTranslation = m_pMapTransform->GetState(CTransform::STATE_POSITION);
	ImGui::InputFloat("X##TranslationX", &m_vTranslation.x, 0.f, 0.f, "%.2f"); ImGui::SameLine();
	ImGui::InputFloat("Y##TranslationY", &m_vTranslation.y, 0.f, 0.f, "%.2f"); ImGui::SameLine();
	ImGui::InputFloat("Z##TranslationZ", &m_vTranslation.z, 0.f, 0.f, "%.2f");
	m_pMapTransform->SetState(CTransform::STATE_POSITION, m_vTranslation);
#pragma endregion

	return S_OK;
}

void CMapTool::ResourceExtract(SAVE _eSaveType)
{
	
	set<string> setModelKey{};
	vector<string> vecModelName{};
	vector<string> vecTextureName{};

	// �ش� Map�� Area ���� ����.
	vector<shared_ptr<CArea>> vecSaveAreas{};

	if (SAVE_PART == _eSaveType)
	{
		vector<shared_ptr<CArea>>& vecAreas = m_pMap.lock()->GetAreas();
		for (const auto pArea : vecAreas)
		{
			if ("Default" == pArea->GetAreaName())
			{
				vecSaveAreas.push_back(pArea);
				continue;
			}

			if (CArea::TYPE_VIEW == pArea->GetType())
			{
				vecSaveAreas.push_back(pArea);
			}
		}
	}
	else // (SAVE_FULL == _eSaveType)
	{
		vecSaveAreas = m_pMap.lock()->GetAreas();
	}

	//vector<shared_ptr<CArea>>& vecAreas = m_pMap->GetAreas();

	// Map�� Area �� ��ŭ �ݺ�!
	for (const auto& pArea : vecSaveAreas)
	{
		//if (SAVE_PART == _eSaveType)
		//	if (CArea::TYPE_VIEW != pArea->GetType())
		//		continue;

		// Area�� Object Model �� ��ŭ �ݺ�!
		map<string, vector<shared_ptr<CEnvironmentObject>>>& mapObjects = pArea->GetObjects();
		for (const auto& pair : mapObjects)
		{
			auto iter = setModelKey.find(pair.first);
			if (setModelKey.end() == iter) // Ű���� ���ٸ�.
			{
				setModelKey.emplace(pair.first);
				vecModelName.push_back(pair.first);

				// Model�� Material �� ��ŭ �ݺ�!
				vector<shared_ptr<CMaterial>> vecMaterials = *(pair.second.front()->GetModel()->GetMaterial());
				for (auto pMaterial : vecMaterials)
				{
					// Material�� Texture �� ��ŭ �ݺ�!
					unordered_map<_uint, string> mapTextures = pMaterial->GetTextures();
					for (auto TextureName : mapTextures)
						vecTextureName.push_back(TextureName.second);
				}
			}
		}


		vector<shared_ptr<CGroupObject>>& vecGroupObjects = pArea->GetGroupObjects();
		for (const auto pGroupObject : vecGroupObjects)
		{
			_float4x4 matGroupObjectWorld = pGroupObject->GetTransform()->GetWorldMatrix();
			vector<shared_ptr<CEnvironmentObject>>& vecMemberObjects = pGroupObject->GetMemberObjects();

			for (const auto pMemberObject : vecMemberObjects)
			{
				string strMemberObjectModelName = pMemberObject->GetModelKey();

				auto iter = setModelKey.find(strMemberObjectModelName);
				if (setModelKey.end() == iter) // Ű���� ���ٸ�.
				{
					setModelKey.emplace(strMemberObjectModelName);
					vecModelName.push_back(strMemberObjectModelName);

					// Model�� Material �� ��ŭ �ݺ�!
					vector<shared_ptr<CMaterial>> vecMaterials = *(pMemberObject->GetModel()->GetMaterial());
					for (auto pMaterial : vecMaterials)
					{
						// Material�� Texture �� ��ŭ �ݺ�!
						unordered_map<_uint, string> mapTextures = pMaterial->GetTextures();
						for (auto TextureName : mapTextures)
							vecTextureName.push_back(TextureName.second);
					}
				}

			}
		}
	}

	CopyModelFile(m_strOriginModelFilePath, vecModelName);
	CopyModelTextureFile(m_strOriginModelTextureFilePath, vecTextureName);

	// Terrain Save Box�� üũ �Ǿ��ִ��� Ȯ��.

	//shared_ptr<CTerrain> pTerrain = static_pointer_cast<CTerrain>(GAMEINSTANCE->GetGameObject(GAMEINSTANCE->GetCurrentLevel(), TEXT("Layer_Terrain")));
	//string strTerrainTexture = pTerrain->GetBaseDiffuseTextureName();

	//CopyMapFile();
	//CopyTerrainTextureFile(m_strOriginTerrainTextureFilePath, strTerrainTexture);

	
}

void CMapTool::CopyModelFile(string _strModelFilePath, const vector<string>& _vecModelName)
{
	filesystem::path basePath(_strModelFilePath);

	for (const auto& entry : filesystem::directory_iterator(basePath))
	{
		if (entry.is_directory())
			CopyModelFile(entry.path().string(), _vecModelName);
		else if (entry.is_regular_file())
		{
			for (const auto& ModelName : _vecModelName)
			{
				if (ModelName == entry.path().filename().stem().string())
				{
					filesystem::path CopyModelPath(m_strCopyModelFilePath + ModelName + ".dat");
					filesystem::copy(entry.path(), CopyModelPath, filesystem::copy_options::overwrite_existing);
				}
			}
		}
	}
}

void CMapTool::CopyModelTextureFile(string _strModelTextureFilePath, const vector<string>& _vecModelTextureName)
{
	for (const auto& strTextureName : _vecModelTextureName)
	{
		filesystem::path OriginTexturePathDDS = _strModelTextureFilePath + strTextureName + ".dds";
		filesystem::path OriginTexturePathPNG = _strModelTextureFilePath + strTextureName + ".png";

		filesystem::path CopyTexturePath = m_strCopyModelTextureFilePath + strTextureName;

		if (filesystem::exists(OriginTexturePathDDS))
		{
			CopyTexturePath += ".dds";
			filesystem::copy(OriginTexturePathDDS, CopyTexturePath, filesystem::copy_options::overwrite_existing);
		}
		else if (filesystem::exists(OriginTexturePathPNG))
		{
			CopyTexturePath += ".png";
			filesystem::copy(OriginTexturePathPNG, CopyTexturePath, filesystem::copy_options::overwrite_existing);
		}
	}
}

void CMapTool::CopyMapFile()
{
	filesystem::path OriginSelectedMapFilePath = m_strOriginMapDataFilePath + m_vecLoadMapDataFile[m_SelectedLoadMapDataFileIndex];
	filesystem::path CopySelectedMapFilePath = m_strCopyMapDataFilePath + m_vecLoadMapDataFile[m_SelectedLoadMapDataFileIndex];
	filesystem::copy(OriginSelectedMapFilePath, CopySelectedMapFilePath, filesystem::copy_options::overwrite_existing);
}

void CMapTool::CopyTerrainTextureFile(string _strTerrainTextureFilePath, string _strTerrainTexture)
{
	filesystem::path basePath(_strTerrainTextureFilePath);

	for (const auto& entry : filesystem::directory_iterator(basePath))
	{
		if (entry.is_directory())
			CopyTerrainTextureFile(entry.path().string(), _strTerrainTexture);
		else if (entry.is_regular_file())
		{

			if (_strTerrainTexture == entry.path().filename().stem().string())
			{
				filesystem::path CopyTerrainTextureFilePath(m_strCopyTerrainTextureFilePath + entry.path().filename().string());
				filesystem::copy(entry.path(), CopyTerrainTextureFilePath, filesystem::copy_options::overwrite_existing);
				return;
			}

		}
	}






	filesystem::path OriginSelectedMapFilePath = m_strOriginMapDataFilePath + m_vecLoadMapDataFile[m_SelectedLoadMapDataFileIndex];
	filesystem::path CopySelectedMapFilePath = m_strCopyMapDataFilePath + m_vecLoadMapDataFile[m_SelectedLoadMapDataFileIndex];
	filesystem::copy(OriginSelectedMapFilePath, CopySelectedMapFilePath, filesystem::copy_options::overwrite_existing);
}

void CMapTool::MakeList(string _strMapPath)
{
	filesystem::path basePath(_strMapPath);

	for (const auto& entry : filesystem::directory_iterator(basePath))
	{
		if (entry.is_directory())
			MakeList(entry.path().string());
		else
			m_vecLoadMapDataFile.push_back(entry.path().filename().string());
	}
}

void CMapTool::SelectedComboListIndex(string _strListName, vector<string>& _vecList, _uint& _iSelectedIndex)
{
	_uint iNumListSize = static_cast<_uint>(_vecList.size());

	if (0 == iNumListSize)
	{
		if (ImGui::BeginCombo("##Empty ComboBox", "Is Empty!"))
		{
			ImGui::EndCombo();

			_iSelectedIndex = 0;
		}

		return;
	}

	if (ImGui::BeginCombo(_strListName.c_str(), _vecList[_iSelectedIndex].c_str()))
	{
		for (_uint iCntListSize = 0; iCntListSize < iNumListSize; iCntListSize++)

		{
			bool bIsSelected = (_iSelectedIndex == iCntListSize);
			if (ImGui::Selectable(_vecList[iCntListSize].c_str(), bIsSelected))
				_iSelectedIndex = iCntListSize;

			if (true == bIsSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
}

string CMapTool::IODialog(IO _IOType)
{
	/*
	OPENFILENAME FILENAME; // ���� ��ȭ ���ڸ� ���� ����ü�� �����մϴ�.
	WCHAR szFile[260]; // ������ ������ �̸��� ������ �迭�� �����մϴ�.

	ZeroMemory(&FILENAME, sizeof(FILENAME));

	FILENAME.lStructSize = sizeof(FILENAME);
	FILENAME.hwndOwner = g_hWnd;
	FILENAME.lpstrFile = szFile;
	FILENAME.lpstrFile[0] = '\0'; // ���� �̸��� ���ϴ�.
	FILENAME.nMaxFile = sizeof(szFile); // �ִ� ���� �̸� ���� ����.
	FILENAME.lpstrFilter = L"All Files\0*.*\0";
	FILENAME.nFilterIndex = 1; // ���� ������ �ʱ� ���� �ε����� �����մϴ�.
	FILENAME.lpstrFileTitle = NULL; // ������� �ʴ� �ʵ��̹Ƿ� NULL�� �����մϴ�.
	FILENAME.nMaxFileTitle = 0; // ������� �ʴ� �ʵ��̹Ƿ� 0���� �����մϴ�.
	FILENAME.lpstrInitialDir = NULL; // �ʱ� ���丮�� �������� �ʽ��ϴ�.
	FILENAME.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	// ������ �����ϴ� ��ο��� �ϸ�, ������ �̹� ������ ��� ����� ���θ� ���� ��ȭ ���ڸ� ǥ���մϴ�.
	// ���� ��ȭ ���� ����

	// ���� ���� ��ȭ ���ڸ� ���� ����ڰ� ������ �����ϰ�
	// ������ �Ϸ��ϸ� ������ ������ ��ΰ� FILENAME.lpstrFile�� ����ǰ�,
	// �Լ��� TRUE�� ��ȯ�մϴ�.
	if (true == GetSaveFileName(&FILENAME))
	{
		// ������ ������ ����� FILENAME.lpstrFile�� �޽��� �ڽ��� ǥ���մϴ�.
		// NULL�� �޽��� �ڽ��� �θ� �����찡 ������ �ǹ��ϸ�,
		// "File Selected"�� �޽��� �ڽ��� �����Դϴ�.
		// MB_OK�� �޽��� �ڽ��� Ȯ�� ��ư�� ǥ�õ��� �ǹ��մϴ�.
		MessageBox(NULL, FILENAME.lpstrFile, L"File Selected", MB_OK);
	}

	wstring wstrFilePath = FILENAME.lpstrFile;
	string strFilePath(wstrFilePath.begin(), wstrFilePath.end());

	return strFilePath;
	FILENAME.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;



	OPENFILENAME FILENAME;
	WCHAR szFile[260];

	ZeroMemory(&FILENAME, sizeof(FILENAME));

	FILENAME.lStructSize = sizeof(FILENAME);
	FILENAME.hwndOwner = g_hWnd;
	FILENAME.lpstrFile = szFile;
	FILENAME.lpstrFile[0] = '\0'; // ���� �̸��� ���ϴ�.
	FILENAME.nMaxFile = sizeof(szFile); // �ִ� ���� �̸� ���� ����.
	FILENAME.lpstrFilter = L"All Files\0*.*\0";
	FILENAME.nFilterIndex = 1; // ���� ������ �ʱ� ���� �ε����� �����մϴ�.
	FILENAME.lpstrFileTitle = NULL;
	FILENAME.nMaxFileTitle = 0;
	FILENAME.lpstrInitialDir = NULL;


	// ���� �б� ��ȭ ���� ����
	if (true == GetOpenFileName(&FILENAME))
	{
		// ������ ������ ����� FILENAME.lpstrFile�� �޽��� �ڽ��� ǥ���մϴ�.
		MessageBox(NULL, FILENAME.lpstrFile, L"File Selected", MB_OK);
	}
	else
	{
		MessageBox(NULL, L"No file selected.", L"File Selection", MB_OK);
	}

	wstring wstrFilePath = FILENAME.lpstrFile;
	string strFilePath(wstrFilePath.begin(), wstrFilePath.end());

	return strFilePath;
	*/
	return "a";

}

void CMapTool::AddObject(shared_ptr<CEnvironmentObject> _pObject)
{
	if (nullptr == m_pMap.lock())
		return;
	m_pMap.lock()->AddObject(_pObject);
	//vecAreas[m_iSelectedEditArea]->AddObject(_pObject);
}

void CMapTool::AddGroupObject(shared_ptr<CGroupObject> _pGroupObject)
{
	if (nullptr == m_pMap.lock())
		return;
	m_pMap.lock()->AddGroupObject(_pGroupObject);
}

shared_ptr<CArea> CMapTool::GetCerrentEditModeArea()
{
	// ���ʿ� ���� ������ ������Ʈ ���Ⱑ �ȵ�.

	vector<shared_ptr<CArea>> vecAreas = m_pMap.lock()->GetAreas();

	string strDefault = "Default";

	if (0 == vecAreas.size())
	{
		vecAreas.push_back(CArea::Create(strDefault));
		m_iSelectedEditModeAreaIndex = 0;
		vecAreas[m_iSelectedEditModeAreaIndex]->SetType(CArea::TYPE_EDIT);
		m_iCurEditModeAreaIndex = m_iSelectedEditModeAreaIndex;
	}

	return m_pMap.lock()->GetAreas()[m_iCurEditModeAreaIndex];
}

shared_ptr<CMapTool> CMapTool::Create()
{
	shared_ptr<CMapTool> pInstance = WRAPPING(CMapTool)();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMapTool");
		pInstance.reset();
	}

	return pInstance;
}
