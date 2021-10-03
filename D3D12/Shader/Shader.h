#ifndef _SHADER_H_
#define _SHADER_H_

#include "../d3dx12.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class Shader {

	public:

		Shader(LPCWSTR path, D3D12_SHADER_VISIBILITY visibility);
		Shader(const Shader& other);
		Shader(Shader&& other) noexcept;
		~Shader();

		LPCWSTR GetPath() const;
		ID3DBlob* GetPointer() const;
		D3D12_SHADER_BYTECODE GetbyteCode() const;
		D3D12_SHADER_VISIBILITY GetVisibility() const;

		Shader& operator=(const Shader& other);
		Shader& operator=(Shader&& other) noexcept;
		bool operator==(const Shader& other) const;
		bool operator!=(const Shader& other) const;

	private:

		LPCWSTR m_path;
		ID3DBlob* m_pointer;
		D3D12_SHADER_BYTECODE m_byteCode;
		D3D12_SHADER_VISIBILITY m_visibility;
};



#endif
