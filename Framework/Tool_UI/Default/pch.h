#pragma once

#define _HAS_STD_BYTE 0

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����
#include <windows.h>
// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <process.h>
#include <thread>
#include <mutex>
#include <functional>

#include <wrl.h>
using namespace Microsoft::WRL;

#include <d3d11.h>
#include <DirectXMath.h>
#include "SimpleMath.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

// C++ ǥ�� ���̺귯��
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <memory>
#include <functional>
#include <utility>
#include <tuple>

#include <fstream>
#include <filesystem>

#include <cmath>
#include <cstdlib>
#include <cassert>
#include <ctime>

using namespace std;
namespace fs = std::filesystem;
