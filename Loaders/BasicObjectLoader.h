#ifndef _BASIC_OBJECT_LOADER_H_
#define _BASIC_OBJECT_LOADER_H_

#include "../Mesh/Model/Model.h"
#include "../D3D12/Device/Device.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

	Model* LoadCube(Device* device);

	void ComputeTangents(std::vector<Vertex>& vertices);
	void ComputeBitangents(std::vector<Vertex>& vertices);
	void ComputeTangentsAndBitangents(std::vector<Vertex>& vertices);


#endif
