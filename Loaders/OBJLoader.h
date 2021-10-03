#ifndef _OBJ_LOADER_H_
#define _OBJ_LOADER_H_

#include "../Core/Maths/Vectors/Vec3/Vec3.h"
#include "../Core/Maths/Vectors/Vec4/Vec4.h"
#include "../Mesh/Model/Model.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

using namespace engine::core::maths;


Model* ExtractModel(Device* device, LPCWSTR filePath);

#endif