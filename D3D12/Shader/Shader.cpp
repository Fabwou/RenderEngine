#include "Shader.h"
#include <D3Dcompiler.h>

Shader::Shader(LPCWSTR path, D3D12_SHADER_VISIBILITY visibility) : 
	m_path{ path },
	m_pointer{ nullptr },
	m_byteCode{},
	m_visibility{ visibility }
{

	HRESULT hr;

	hr = D3DReadFileToBlob(path, &m_pointer);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to load Vertex Shader",
			L"Error", MB_OK);
	}

	m_byteCode.BytecodeLength = m_pointer->GetBufferSize();
	m_byteCode.pShaderBytecode = m_pointer->GetBufferPointer();

}

Shader::Shader(const Shader& other):
	m_path{ other.m_path },
	m_pointer{ other.m_pointer },
	m_byteCode{ other.m_byteCode },
	m_visibility{ other.m_visibility }
{}

Shader::Shader(Shader&& other) noexcept:
	m_path{ other.m_path },
	m_pointer{ other.m_pointer },
	m_byteCode{ other.m_byteCode },
	m_visibility{ other.m_visibility }
{}

Shader::~Shader()
{}

LPCWSTR Shader::GetPath() const { return m_path; }
ID3DBlob* Shader::GetPointer() const { return m_pointer; }
D3D12_SHADER_BYTECODE Shader::GetbyteCode() const { return m_byteCode; }
D3D12_SHADER_VISIBILITY Shader::GetVisibility() const { return m_visibility; }


Shader& Shader::operator=(const Shader& other) {
	m_path = other.m_path;
	m_pointer = other.m_pointer;
	m_byteCode = other.m_byteCode;
	m_visibility = other.m_visibility;

	return *this;
}
Shader& Shader::operator=(Shader&& other) noexcept {
	m_path = other.m_path;
	m_pointer = other.m_pointer;
	m_byteCode = other.m_byteCode;
	m_visibility = other.m_visibility;

	return *this;
}

bool Shader::operator==(const Shader& other) const {

	return (wcscmp(m_path, other.m_path) == 0) && (m_visibility == other.GetVisibility());

}

bool Shader::operator!=(const Shader& other) const {

	return (wcscmp(m_path, other.m_path) != 0) || (m_visibility != other.m_visibility);

}