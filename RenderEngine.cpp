// RenderEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#endif

#include "RHI/RHI.h"
#include "Mesh/Mesh/Mesh.h"
#include "Lights/Light.h"
#include "Camera/Camera.h"
#include "Inputs/Input.h"
#include <time.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

using namespace engine::input;
using namespace engine::input::keys;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	//       _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		   //RHI
	Rhi* rhi = new Rhi(3);
	Input* inputs = new Input();
	float translateSpeed = 5.0f;
	float rotSpeed = 100.0f;
	float freeCamRotSpeed = 50.0f;
	float meshMoveSpeed = 1.0f;
	float explosionSpeed = 1.0f;
	bool cursorLock = false;

	Vec3 lightAttenuationSettings[12]{ {1.0f, 0.7f, 1.8f}, {1.0f, 0.35f, 0.44f}, {1.0f, 0.22f, 0.20f},                   //dist = 7, 13, 20
										{1.0f, 0.12f, 0.07f}, {1.0f, 0.09f, 0.032f}, {1.0f, 0.07f, 0.017f},             //dist = 32, 50, 65
										{1.0f, 0.045f, 0.0075f}, {1.0f, 0.027f, 0.0028f}, {1.0f, 0.022f, 0.0019f},      // dist = 100, 160, 200
										{1.0f, 0.014f, 0.0007f}, {1.0f, 0.007f, 0.0002f}, {1.0f, 0.0014f, 0.000007f} };   // dist = 325, 600, 3250
	short int lightAttenuationSettingIndex = 4;
	std::vector<Light*> allLights;
	//allLights.push_back(new Light({ 0.0f, 0.0f, 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f, 1.0f }));
	//allLights.push_back(new Light({ 0.0f, 0.0f, -3.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }));
	allLights.push_back(new Light({ 0.0f, 3.0f, -4.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }));
	allLights.push_back(new Light({ 0.0f, 3.0f, 4.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }));
	allLights.push_back(new Light({ 0.0f, 7.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }));
	allLights.push_back(new Light({ 0.0f, -1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }));
	allLights.push_back(new Light({ 4.0f, 3.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.2f, 0.0f, 1.0f }));
	allLights.push_back(new Light({ -4.0f, 3.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f }));


	//allLights[0]->Rotate(Quaternion::Euler({ 90.0f, 0.0f, 0.0f }));
	//allLights[0]->SetCutOff(50.0f);

	//Window
	rhi->GenWindow(L"test", 800, 600, false, false, hInstance, nShowCmd);
	rhi->GetWindow(0)->SetClearColor(0.0f, 0.2f, 0.4f, 1.0f);

	clock_t beginFrame = clock();
	clock_t endFrame = clock();

	//Camera
	engine::camera::Camera mainCamera = engine::camera::Camera(engine::camera::EProjectType::PERSPECTIVE,
		rhi->GetWindow(0)->GetClientWidth(), rhi->GetWindow(0)->GetClientHeight(),
		0.1f, 1000.f, 60.f, { 0.f, 3.0f, -5.0f });


	//inputLayout
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
	inputLayoutDesc.NumElements = sizeof(inputLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	inputLayoutDesc.pInputElementDescs = inputLayout;

	//Model
	Model* cubeMod = rhi->GetModel(L"Cube");
	rhi->LoadFBX(L"Resources/Models/Ironman/ironman.fbx");
	Model* ironmanModel = rhi->GetModel(L"IronMan_Mesh");
	Model* sphereModel = rhi->LoadFBX(L"Resources/Models/Sphere/Sphere.fbx")[0];


	//Shaders
	Shader* vertexShader = rhi->GetShader(L"Resources/Shaders/Compiled/VertexShader.cso", D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX);
	Shader* pixelShader = rhi->GetShader(L"Resources/Shaders/Compiled/PixelShader.cso", D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL);
	Shader* PointLightPixelShader = rhi->GetShader(L"Resources/Shaders/Compiled/PointLightPixelShader.cso", D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL);
	Shader* MultiplePointLightPixelShader = rhi->GetShader(L"Resources/Shaders/Compiled/MultiplePointLightPixelShader.cso", D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL);
	Shader* MultiplePointLightWithoutMapPixelShader = rhi->GetShader(L"Resources/Shaders/Compiled/MultiplePointLightWithoutMapPixelShader.cso", D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL);
	Shader* goemetryShader = rhi->GetShader(L"Resources/Shaders/Compiled/GeometryShader.cso", D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_GEOMETRY);
	std::vector<Shader*> shaders;
	std::vector<Shader*> bumpShaders;
	std::vector<Shader*> noBumpShaders;
	shaders.push_back(vertexShader);
	shaders.push_back(pixelShader);
	bumpShaders.push_back(vertexShader);
	//bumpShaders.push_back(goemetryShader);
	bumpShaders.push_back(MultiplePointLightPixelShader);
	noBumpShaders.push_back(vertexShader);
	noBumpShaders.push_back(MultiplePointLightWithoutMapPixelShader);

	//Textures
	Texture* johnTravoltaireTex = rhi->GetTexture(L"Resources/Textures/JohnTravoltaire.jpg");
	Texture* ironmanTex = rhi->GetTexture(L"Resources/Textures/Ironman/ironman.dff.png");
	Texture* ironmanBumpTex = rhi->GetTexture(L"Resources/Textures/Ironman/ironman.norm.png");
	Texture* ironmanSpecTex = rhi->GetTexture(L"Resources/Textures/Ironman/ironman.spec.png");
	Texture* diceTex = rhi->GetTexture(L"Resources/Textures/Dice.png");
	Texture* nicolasCageTex = rhi->GetTexture(L"Resources/Textures/NicolasCage.jpg");
	Texture* greenTex = rhi->GetTexture(L"Resources/Textures/Green.png");
	std::vector<Texture*> johnTexture;
	std::vector<Texture*> ironmanTextures;
	std::vector<Texture*> diceTextures;
	std::vector<Texture*> greenTexture;
	std::vector<Texture*> nicolasCageTexture;
	johnTexture.push_back(johnTravoltaireTex);
	ironmanTextures.push_back(ironmanTex);
	ironmanTextures.push_back(ironmanBumpTex);
	ironmanTextures.push_back(ironmanSpecTex);
	diceTextures.push_back(diceTex);
	diceTextures.push_back(nicolasCageTex);
	greenTexture.push_back(greenTex);
	nicolasCageTexture.push_back(nicolasCageTex);

	//Transform
	Transform ironmanTransform;
	ironmanTransform.Translate({ 0.0f, 2.0f, 0.0f });
	ironmanTransform.SetRotation(Quaternion::Euler({ -90.0f, 180.0f, 0.0f }));

	std::vector<Transform> lightTransform(allLights.size());
	for (unsigned int i = 0; i < allLights.size(); ++i) {
		lightTransform[i].SetPosition(allLights[i]->GetTransform().GetPosition());
		lightTransform[i].SetRotation(allLights[i]->GetTransform().GetRotation());
		lightTransform[i].SetScale({ 0.1f, 0.1f, 0.1f });
	}
	std::vector<Transform> wallTransform(6);
	wallTransform[0].Translate({ 0.0f, -2.0f, 0.0f });
	wallTransform[0].SetScale({ 10.0f, 0.3f, 10.0f });
	wallTransform[1].Translate({ 5.0f, 3.0f, 0.0f });
	wallTransform[1].SetScale({ 0.3f, 10.0f, 10.0f });
	wallTransform[2].Translate({ -5.0f, 3.0f, 0.0f });
	wallTransform[2].SetScale({ 0.3f, 10.0f, 10.0f });
	wallTransform[3].Translate({ 0.0f, 8.0f, 0.0f });
	wallTransform[3].SetScale({ 10.0f, 0.3f, 10.0f });
	wallTransform[4].Translate({ 0.0f, 3.0f, 5.0f });
	wallTransform[4].SetScale({ 10.0f, 10.0f, 0.3f });
	wallTransform[5].Translate({ 0.0f, 3.0f, -5.0f });
	wallTransform[5].SetScale({ 10.0f, 10.0f, 0.3f });
	//ConstantBuffer
	ConstantBufferPerObject ironmanCB;
	ConstantBufferMaterial ironmanMatCB;
	ConstantbufferMultipleLight ironmanLightCb;
	std::vector<ConstantBufferPerObject> light1ModelCB(allLights.size());
	std::vector<ConstantBufferPerObject> wallCB(wallTransform.size());
	std::vector<ConstantBufferMaterial> wallMatCB(wallTransform.size());
	std::vector<ConstantbufferMultipleLight> wallLightCB(wallTransform.size());
	ConstantBufferGeometryShader gsCb;

	ironmanCB.wMat = ironmanTransform.GetWorldMat();
	ironmanCB.vMat = mainCamera.GetViewMat();
	ironmanCB.pMat = mainCamera.GetProjectMat();
	ironmanCB.mvp = ironmanCB.pMat * ironmanCB.vMat * ironmanCB.wMat;
	ironmanCB.invWMat = ironmanTransform.GetWorldMat().GetInverse();
	ironmanCB.cameraPos = mainCamera.GetPosition();
	ironmanMatCB.ambientColor = { 0.2f, 0.2f, 0.2f, 1.0f };
	ironmanMatCB.emissiveColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	ironmanMatCB.glossiness = 256.0f;
	for (unsigned int i = 0; i < allLights.size(); ++i) {

		light1ModelCB[i].wMat = lightTransform[i].GetWorldMat();
		light1ModelCB[i].vMat = mainCamera.GetViewMat();
		light1ModelCB[i].pMat = mainCamera.GetProjectMat();
		light1ModelCB[i].mvp = light1ModelCB[i].pMat * light1ModelCB[i].vMat * light1ModelCB[i].wMat;
		light1ModelCB[i].invWMat = lightTransform[i].GetWorldMat().GetInverse();
		light1ModelCB[i].cameraPos = mainCamera.GetPosition();
	}
	for (unsigned int i = 0; i < wallCB.size(); ++i) {

		wallCB[i].wMat = wallTransform[i].GetWorldMat();
		wallCB[i].vMat = mainCamera.GetViewMat();
		wallCB[i].pMat = mainCamera.GetProjectMat();
		wallCB[i].mvp = wallCB[i].pMat * wallCB[i].vMat * wallCB[i].wMat;
		wallCB[i].invWMat = wallTransform[i].GetWorldMat().GetInverse();
		wallCB[i].cameraPos = mainCamera.GetPosition();
		wallMatCB[i].ambientColor = { 0.2f, 0.2f, 0.2f, 1.0f };
		wallMatCB[i].emissiveColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		wallMatCB[i].glossiness = 256.0f;
	}

	gsCb.time = 0.0f;

	ConstantBuffer* ironmanCBPTR = new ConstantBuffer(rhi->GetDevice(), &ironmanCB, sizeof(ironmanCB), 0, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX);
	ConstantBuffer* ironmanMatCBPTR = new ConstantBuffer(rhi->GetDevice(), &ironmanMatCB, sizeof(ironmanMatCB), 1, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL);
	ConstantBuffer* ironmanLightCbPtr = new ConstantBuffer(rhi->GetDevice(), &ironmanLightCb, sizeof(ironmanLightCb), 2, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL);
	std::vector<ConstantBuffer*> lightModelCBPTR(allLights.size());
	for (unsigned int i = 0; i < allLights.size(); ++i)
		lightModelCBPTR[i] = new ConstantBuffer(rhi->GetDevice(), &light1ModelCB, sizeof(light1ModelCB), 0, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX);
	std::vector<ConstantBuffer*> wallCBPTR(wallTransform.size());
	std::vector<ConstantBuffer*> wallMatCBPTR(wallTransform.size());
	std::vector<ConstantBuffer*> wallLightCBPTR(wallTransform.size());
	for (unsigned int i = 0; i < wallCBPTR.size(); ++i) {

		wallCBPTR[i] = new ConstantBuffer(rhi->GetDevice(), &wallCB, sizeof(wallCB), 0, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX);
		wallMatCBPTR[i] = new ConstantBuffer(rhi->GetDevice(), &wallMatCB, sizeof(wallMatCB), 1, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL);
		wallLightCBPTR[i] = new ConstantBuffer(rhi->GetDevice(), &wallLightCB, sizeof(wallLightCB), 2, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL);
	}

	ConstantBuffer* gsCbPtr = new ConstantBuffer(rhi->GetDevice(), &gsCb, sizeof(gsCb), 3, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_GEOMETRY);
	std::vector<ConstantBuffer*> ironmanCbs;
	ironmanCbs.push_back(ironmanCBPTR);
	ironmanCbs.push_back(ironmanMatCBPTR);
	ironmanCbs.push_back(ironmanLightCbPtr);
	//ironmanCbs.push_back(gsCbPtr);
	std::vector<std::vector<ConstantBuffer*>> lightModelCbs;
	for (unsigned int i = 0; i < allLights.size(); ++i) {
		std::vector<ConstantBuffer*> temp;
		temp.push_back(lightModelCBPTR[i]);
		lightModelCbs.push_back(temp);
	}
	std::vector<std::vector<ConstantBuffer*>> wallCbs;
	for (unsigned int i = 0; i < wallTransform.size(); ++i) {
		std::vector<ConstantBuffer*> temp;
		temp.push_back(wallCBPTR[i]);
		temp.push_back(wallMatCBPTR[i]);
		temp.push_back(wallLightCBPTR[i]);
		wallCbs.push_back(temp);
	}

	//MeshRenderer
	MeshRenderer* ironmanMeshRenderer = rhi->GetMeshRenderer(&inputLayoutDesc, ironmanModel, bumpShaders, ironmanTextures, true, ironmanCbs);
	std::vector<MeshRenderer*> lag;
	//for (unsigned int i = 0; i < 200; ++i) {

	//	lag.push_back(rhi->GetMeshRenderer(&inputLayoutDesc, ironmanModel, bumpShaders, ironmanTextures, true, ironmanCbs));
	//	rhi->SetActive(lag[i], rhi->GetWindow(0), true);
	//}
	std::vector<MeshRenderer*> lightMeshRenderer(allLights.size());
	for (unsigned int i = 0; i < allLights.size(); ++i) {

		lightMeshRenderer[i] = rhi->GetMeshRenderer(&inputLayoutDesc, sphereModel, shaders, nicolasCageTexture, true, lightModelCbs[i]);
		rhi->SetActive(lightMeshRenderer[i], rhi->GetWindow(0), true);
	}
	std::vector<MeshRenderer*> wallMeshRenderer(wallTransform.size());
	std::vector<Mesh> wallMesh;
	for (unsigned int i = 0; i < wallTransform.size(); ++i) {

		wallMeshRenderer[i] = rhi->GetMeshRenderer(&inputLayoutDesc, cubeMod, noBumpShaders, diceTextures, true, wallCbs[i]);
		wallMesh.push_back(Mesh(wallMeshRenderer[i], wallTransform[i]));
		rhi->SetActive(wallMesh[i].GetMeshRenderer(), rhi->GetWindow(0), true);
	}

	//Meshes
	Mesh ironmanMesh(ironmanMeshRenderer, ironmanTransform);
	rhi->SetActive(ironmanMesh.GetMeshRenderer(), rhi->GetWindow(0), true);

	while (rhi->Update()) {

		endFrame = clock();

		float deltaTime = ((float)(endFrame - beginFrame)) / CLOCKS_PER_SEC;

		if (rhi->GetWindows().size() > 0 && rhi->GetWindow(0)->GetHasFocus()) {

			inputs->Update();

			//Quit
			if (inputs->IsKeyDown(EKeyCode::INPUT_KEY_ESCAPE))
				rhi->GetWindow(0)->CloseWindow();

			//Camera movement
			if (inputs->IsKey(EKeyCode::INPUT_KEY_W))
				mainCamera.SetPosition(Vec3::Lerp(mainCamera.GetPosition(),
													mainCamera.GetPosition() + mainCamera.GetRotation().GetRotatedVertex({ 0.0f, 0.0f, translateSpeed }),
													deltaTime));
			if (inputs->IsKey(EKeyCode::INPUT_KEY_A))
				mainCamera.SetPosition(Vec3::Lerp(mainCamera.GetPosition(),
													mainCamera.GetPosition() + mainCamera.GetRotation().GetRotatedVertex({ -translateSpeed, 0.0f, 0.0f }),
													deltaTime));
			if (inputs->IsKey(EKeyCode::INPUT_KEY_S))
				mainCamera.SetPosition(Vec3::Lerp(mainCamera.GetPosition(),
													mainCamera.GetPosition() + mainCamera.GetRotation().GetRotatedVertex({ 0.0f, 0.0f, -translateSpeed }),
													deltaTime));
			if (inputs->IsKey(EKeyCode::INPUT_KEY_D))
				mainCamera.SetPosition(Vec3::Lerp(mainCamera.GetPosition(),
													mainCamera.GetPosition() + mainCamera.GetRotation().GetRotatedVertex({ translateSpeed, 0.0f, 0.0f }),
													deltaTime));
			if (inputs->IsKey(EKeyCode::INPUT_KEY_LEFT_SHIFT))
				mainCamera.SetPosition(Vec3::Lerp(mainCamera.GetPosition(),
													mainCamera.GetPosition() + Vec3(0.0f, translateSpeed, 0.0f),
													deltaTime));
			if (inputs->IsKey(EKeyCode::INPUT_KEY_LEFT_CONTROL))
				mainCamera.SetPosition(Vec3::Lerp(mainCamera.GetPosition(),
													mainCamera.GetPosition() + Vec3(0.0f, -translateSpeed, 0.0f),
													deltaTime));

			//Camera rotation
			if (inputs->IsKey(EKeyCode::INPUT_KEY_NUM_PAD_2))
				mainCamera.SetRotation(Quaternion::Slerp(mainCamera.GetRotation(),
															mainCamera.GetRotation() * Quaternion::FromAxis({ rotSpeed, 0.0f, 0.0f }),
															deltaTime));
			if (inputs->IsKey(EKeyCode::INPUT_KEY_NUM_PAD_8))
				mainCamera.SetRotation(Quaternion::Slerp(mainCamera.GetRotation(),
															mainCamera.GetRotation() * Quaternion::FromAxis({ -rotSpeed, 0.0f, 0.0f }),
															deltaTime));
			if (inputs->IsKey(EKeyCode::INPUT_KEY_NUM_PAD_6))
				mainCamera.SetRotation(Quaternion::Slerp(mainCamera.GetRotation(),
															Quaternion::FromAxis({ 0.0f, rotSpeed, 0.0f }) * mainCamera.GetRotation(),
															deltaTime));
			if (inputs->IsKey(EKeyCode::INPUT_KEY_NUM_PAD_4))
				mainCamera.SetRotation(Quaternion::Slerp(mainCamera.GetRotation(),
															Quaternion::FromAxis({ 0.0f, -rotSpeed, 0.0f }) * mainCamera.GetRotation(),
															deltaTime));

			//Mesh movement
			if (inputs->IsKey(EKeyCode::INPUT_KEY_LEFT_ARROW))
				ironmanMesh.GetTransform().SetPosition(Vec3::Lerp(ironmanMesh.GetTransform().GetPosition(),
																	ironmanMesh.GetTransform().GetPosition() + Vec3(-meshMoveSpeed, 0.0f, 0.0f),
																	deltaTime));
			if (inputs->IsKey(EKeyCode::INPUT_KEY_RIGHT_ARROW))
				ironmanMesh.GetTransform().SetPosition(Vec3::Lerp(ironmanMesh.GetTransform().GetPosition(),
														ironmanMesh.GetTransform().GetPosition() + Vec3(meshMoveSpeed, 0.0f, 0.0f),
														deltaTime));
			if (inputs->IsKey(EKeyCode::INPUT_KEY_UP_ARROW))
				ironmanMesh.GetTransform().SetPosition(Vec3::Lerp(ironmanMesh.GetTransform().GetPosition(),
																	ironmanMesh.GetTransform().GetPosition() + Vec3(0.0f, 0.0f, meshMoveSpeed),
																	deltaTime));
			if (inputs->IsKey(EKeyCode::INPUT_KEY_DOWN_ARROW))
				ironmanMesh.GetTransform().SetPosition(Vec3::Lerp(ironmanMesh.GetTransform().GetPosition(),
																	ironmanMesh.GetTransform().GetPosition() + Vec3(0.0f, 0.0f, -meshMoveSpeed),
																	deltaTime));
			if (inputs->IsKey(EKeyCode::INPUT_KEY_RIGHT_SHIFT))
				ironmanMesh.GetTransform().SetPosition(Vec3::Lerp(ironmanMesh.GetTransform().GetPosition(),
																	ironmanMesh.GetTransform().GetPosition() + Vec3(0.0f, meshMoveSpeed, 0.0f),
																	deltaTime));
			if (inputs->IsKey(EKeyCode::INPUT_KEY_RIGHT_CONTROL))
				ironmanMesh.GetTransform().SetPosition(Vec3::Lerp(ironmanMesh.GetTransform().GetPosition(),
																	ironmanMesh.GetTransform().GetPosition() + Vec3(0.0f, -meshMoveSpeed, 0.0f),
																	deltaTime));

			//Spotlight test
			if (inputs->IsKeyDown(EKeyCode::INPUT_KEY_O))
				for (unsigned i = 0; i < allLights.size(); ++i)
					allLights[i]->SetCutOff(allLights[i]->GetInfo().cutOff + 1.0f);
			if (inputs->IsKeyDown(EKeyCode::INPUT_KEY_P))
				for (unsigned i = 0; i < allLights.size(); ++i)
					allLights[i]->SetCutOff(allLights[i]->GetInfo().cutOff - 1.0f);

			//Light attenuation test
			if (inputs->IsKeyDown(EKeyCode::INPUT_KEY_J)) {

				if (lightAttenuationSettingIndex != 11) {

					++lightAttenuationSettingIndex;
					for (unsigned i = 0; i < allLights.size(); ++i)
						allLights[i]->SetAttenuation(lightAttenuationSettings[lightAttenuationSettingIndex].m_x,
							lightAttenuationSettings[lightAttenuationSettingIndex].m_y,
							lightAttenuationSettings[lightAttenuationSettingIndex].m_z);
				}
			}
			if (inputs->IsKeyDown(EKeyCode::INPUT_KEY_K))
				if (lightAttenuationSettingIndex != 0) {

					--lightAttenuationSettingIndex;
					for (unsigned i = 0; i < allLights.size(); ++i)
						allLights[i]->SetAttenuation(lightAttenuationSettings[lightAttenuationSettingIndex].m_x,
							lightAttenuationSettings[lightAttenuationSettingIndex].m_y,
							lightAttenuationSettings[lightAttenuationSettingIndex].m_z);
				}

			//Display/Hide ironman
			if (inputs->IsKeyDown(EKeyCode::INPUT_KEY_T))
				rhi->SetActive(ironmanMeshRenderer, rhi->GetWindow(0), true);
			if (inputs->IsKeyDown(EKeyCode::INPUT_KEY_Y))
				rhi->SetActive(ironmanMeshRenderer, rhi->GetWindow(0), false);
			if (inputs->IsKeyDown(EKeyCode::INPUT_KEY_F))
				rhi->GetWindow(0)->ToggleFullscreen();

			//Change resolution
			if (inputs->IsKeyDown(EKeyCode::INPUT_KEY_F1))
				rhi->GetWindow(0)->SetResolution(800, 600);
			if (inputs->IsKeyDown(EKeyCode::INPUT_KEY_F2))
				rhi->GetWindow(0)->SetResolution(1280, 1024);
			if (inputs->IsKeyDown(EKeyCode::INPUT_KEY_F3))
				rhi->GetWindow(0)->SetResolution(1680, 1050);
			if (inputs->IsKeyDown(EKeyCode::INPUT_KEY_F4))
				rhi->GetWindow(0)->SetResolution(1920, 1080);

			//Update camera matrices
			float newWidth = rhi->GetWindow(0)->GetWindowWidth();
			float newheight = rhi->GetWindow(0)->GetWindowHeight();

			//Lock cursor to rotate camera with mouse
			Vec2 center{ round(static_cast<float>(newWidth / 2)), round(static_cast<float>(newheight / 2)) };
			if (inputs->IsKeyDown(EKeyCode::INPUT_KEY_L)) {

				inputs->SetMousePos(rhi->GetWindow(0)->GetHandle(), center);
				ShowCursor(cursorLock);
				cursorLock = !cursorLock;
			}

			if (cursorLock)
			{
				Vec2 pos{ inputs->GetMousePos(rhi->GetWindow(0)->GetHandle()) };
				Vec2 diff{ (pos - center) };

				inputs->SetMousePos(rhi->GetWindow(0)->GetHandle(), center);
				mainCamera.SetRotation(Quaternion::Slerp(mainCamera.GetRotation(),
															mainCamera.GetRotation() * Quaternion::FromAxis({ diff.m_y, 0.0f, 0.0f }),
															deltaTime * freeCamRotSpeed));
				mainCamera.SetRotation(Quaternion::Slerp(mainCamera.GetRotation(),
															Quaternion::FromAxis({ 0.0f, diff.m_x, 0.0f }) * mainCamera.GetRotation(),
															deltaTime * freeCamRotSpeed));
			}
			mainCamera.SetViewSize(newWidth, newheight, 0.1f, 1000.0f);
		}

		ironmanCB.wMat = ironmanMesh.GetTransform().GetWorldMat();
		ironmanCB.vMat = mainCamera.GetViewMat();
		ironmanCB.pMat = mainCamera.GetProjectMat();
		ironmanCB.mvp = ironmanCB.pMat * ironmanCB.vMat * ironmanCB.wMat;
		ironmanCB.invWMat = ironmanMesh.GetTransform().GetWorldMat().GetInverse();
		ironmanCB.cameraPos = mainCamera.GetPosition();
		for (unsigned int i = 0; i < allLights.size(); ++i) {

			lightTransform[i].SetPosition(allLights[i]->GetTransform().GetPosition());
			lightTransform[i].SetRotation(allLights[i]->GetTransform().GetRotation());
			light1ModelCB[i].wMat = lightTransform[i].GetWorldMat();
			light1ModelCB[i].vMat = mainCamera.GetViewMat();
			light1ModelCB[i].pMat = mainCamera.GetProjectMat();
			light1ModelCB[i].mvp = light1ModelCB[i].pMat * light1ModelCB[i].vMat * light1ModelCB[i].wMat;
			light1ModelCB[i].invWMat = lightTransform[i].GetWorldMat().GetInverse();
			light1ModelCB[i].cameraPos = mainCamera.GetPosition();
		}
		for (unsigned int i = 0; i < wallMesh.size(); ++i) {


			wallCB[i].wMat = wallMesh[i].GetTransform().GetWorldMat();
			wallCB[i].vMat = mainCamera.GetViewMat();
			wallCB[i].pMat = mainCamera.GetProjectMat();
			wallCB[i].mvp = wallCB[i].pMat * wallCB[i].vMat * wallCB[i].wMat;
			wallCB[i].invWMat = wallMesh[i].GetTransform().GetWorldMat().GetInverse();
			wallCB[i].cameraPos = mainCamera.GetPosition();
			wallLightCB[i] = wallMesh[i].FindNearestLights(allLights);
		}

		ironmanLightCb = ironmanMesh.FindNearestLights(allLights);
		gsCb.time += deltaTime * explosionSpeed;

		rhi->UpdateConstantBuffer(ironmanMesh.GetMeshRenderer()->GetConstantBuffers()[0], &ironmanCB, sizeof(ironmanCB));
		rhi->UpdateConstantBuffer(ironmanMesh.GetMeshRenderer()->GetConstantBuffers()[1], &ironmanMatCB, sizeof(ironmanMatCB));
		rhi->UpdateConstantBuffer(ironmanMesh.GetMeshRenderer()->GetConstantBuffers()[2], &ironmanLightCb, sizeof(ironmanLightCb));
		//rhi->UpdateConstantBuffer(ironmanMesh.GetMeshRenderer()->GetConstantBuffers()[3], &gsCb, sizeof(gsCb));
		for (unsigned int i = 0; i < allLights.size(); ++i)
			rhi->UpdateConstantBuffer(lightMeshRenderer[i]->GetConstantBuffers()[0], &light1ModelCB[i], sizeof(light1ModelCB[i]));
		for (unsigned int i = 0; i < wallMesh.size(); ++i) {

			rhi->UpdateConstantBuffer(wallMesh[i].GetMeshRenderer()->GetConstantBuffers()[0], &wallCB[i], sizeof(wallCB[i]));
			rhi->UpdateConstantBuffer(wallMesh[i].GetMeshRenderer()->GetConstantBuffers()[1], &wallMatCB[i], sizeof(wallMatCB[i]));
			rhi->UpdateConstantBuffer(wallMesh[i].GetMeshRenderer()->GetConstantBuffers()[2], &wallLightCB[i], sizeof(wallLightCB[i]));
		}

		beginFrame = clock();
	}

	delete ironmanCBPTR;
	delete ironmanMatCBPTR;
	delete ironmanLightCbPtr;
	SAFE_DELETE_VECTOR(lightModelCBPTR);
	SAFE_DELETE_VECTOR(wallCBPTR);
	SAFE_DELETE_VECTOR(wallMatCBPTR);
	SAFE_DELETE_VECTOR(wallLightCBPTR);
	delete gsCbPtr;
	SAFE_DELETE_VECTOR(allLights);
	delete inputs;
	delete rhi;


	return 0;
}

