// ��ɾ�	�ɼ�			���� ������ �ִ� ��ġ			�纻 ������ ������ ��ġ

del /q ..\..\..\Client\Bin\DataFiles\MapData\*
del /q ..\..\..\Client\Bin\Resources\Level_Dungeon\Textures\Terrain\*

del /q ..\..\..\Client\Bin\Resources\Level_Dungeon\Models\ModelTexture\Environment\*
del /q ..\..\..\Client\Bin\Resources\Level_Dungeon\Models\NonAnim\Environment\*

xcopy		/y/s		.\Map\*						..\..\..\Client\Bin\DataFiles\MapData\
xcopy		/y/s		.\TerrainTexture\*			..\..\..\Client\Bin\Resources\Level_Dungeon\Textures\Terrain\
xcopy		/y/s		.\ModelTexture\*			..\..\..\Client\Bin\Resources\Level_Dungeon\Models\ModelTexture\Environment\
xcopy		/y/s		.\Model\*					..\..\..\Client\Bin\Resources\Level_Dungeon\Models\NonAnim\Environment\