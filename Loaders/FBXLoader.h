#ifndef _FBX_LOADER_H_
#define _FBX_LOADER_H_

#include "../Mesh/Model/Model.h"
#include "../D3D12/Device/Device.h"
#include "OpenFBX/ofbx.h"
#include <algorithm>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class FBXLoader {

public:

	FBXLoader(Device* device);
	~FBXLoader();
	std::vector<Model*> ExtractFile(LPCWSTR fileName);

private:
	Device* m_device;
	std::vector<Model*> m_returnedModels;
	ofbx::IScene* m_scene;

	bool saveAsOBJ(ofbx::IScene& scene, const char* path);

};

#endif
