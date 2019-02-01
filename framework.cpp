#include "framework.h"
#include "ResourceManager.h"
#include"DXShader.h"
#include<math.h>
#include <crtdbg.h>

//	半球ライティング+平行光
ID3D11VertexShader* HSL_Vertex;
ID3D11PixelShader*  HSL_Pixel;
ID3D11InputLayout*  HSL_Layout;

//映り込み
ID3D11VertexShader* TEST_Vertex;
ID3D11PixelShader*  TEST_Pixel;
ID3D11InputLayout*  TEST_Layout;



bool framework::initialize(HWND hwnd)
{


	// CRTメモリリーク検出用
	//_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	// CRTメモリリーク箇所検出
	//_CrtSetBreakAlloc(15654);
	//MessageBox(NULL, _T("韻踏むなー！"), _T("ちょくちょく見てるけどやばそうだなお前の進捗!"), MB_OK | MB_ICONHAND);

	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;
	HINSTANCE               hInst = NULL;
	HWND                    hWnd = NULL;


#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{

		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);


	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;  //DXG1

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		D3D_DRIVER_TYPE   driverType = driverTypes[driverTypeIndex];
		D3D_FEATURE_LEVEL       featureLevel = D3D_FEATURE_LEVEL_11_0;
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &SwapChain, &Device, &featureLevel, &Context);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return false;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return  false;
	pBackBuffer->Release();
	hr = Device->CreateRenderTargetView(pBackBuffer, NULL, &RenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return  false;


 // Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = SCREEN_WIDTH;
	descDepth.Height = SCREEN_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = Device->CreateTexture2D(&descDepth, NULL, &DepthStencil);
	
	if (FAILED(hr))
		return  false;

	// Create the depth stencil view
	
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = Device->CreateDepthStencilView(DepthStencil, &descDSV, &DepthStenciView);
	if (FAILED(hr))
		return  false;

	//blendイニシャライズ
	Blend::Initialize(Device);

	


	sprites[0] = new sprite(Device, L"hdri1.jpeg");
	sprites[0]->SetSlot(Context, 1);
	
	sprites[1] = new sprite(Device, L"hdri1.jpeg");

	/*for (auto&p :sprites) {
		p = new sprite(Device, L"player-sprites.png");
	}
*/
	//particle = new sprite (Device, L"particle-smoke.png");
	
	cube = new geometric_Cube(Device);

	char* sdk_meshData[] = {
	 "./fbx/000_cube.fbx",
	 "./fbx/001_cube.fbx",
	 "./fbx/002_cube.fbx",
	 "./fbx/003_cube.fbx",
	 "./fbx/004_cube.fbx",
	 "./fbx/005_cube.fbx",
	"./fbx/dummy_data.fbx",
	"./fbx/danbo_atk.fbx",
	"./fbx/danbo_taiki.fbx",
	"./fbx/danbo_fly.fbx",
	"./fbx/ShachikuChan_ver3.fbx",
	
	};
	
	
	
	
	sdk[0] = new skinned_mesh(Device, sdk_meshData[0]);
	sdk[1] = new skinned_mesh(Device, sdk_meshData[8]);
	


	D3D11_INPUT_ELEMENT_DESC HSL_layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },

		{ "WEIGHTS"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "BONES"	, 0, DXGI_FORMAT_R32G32B32A32_SINT, 0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	UINT HSL_numElements = ARRAYSIZE(HSL_layout);

	ResourceManager::LoadVertexShader(Device, "skinnd_mesh_vs.cso", HSL_layout, HSL_numElements, &HSL_Vertex, &HSL_Layout);
	ResourceManager::LoadPixelShader(Device, "Shader_test_ps.cso", &HSL_Pixel);

	ResourceManager::LoadVertexShader(Device, "skinnd_mesh_vs.cso", HSL_layout, HSL_numElements, &TEST_Vertex, &TEST_Layout);
	ResourceManager::LoadPixelShader(Device, "Lighting_ps.cso", &TEST_Pixel);

	return true;
}
framework::~framework() {
	
	for (int i = 0; i < 10; i++) {
		if (sdk[i])delete sdk[i];
	}
	
	for (auto&p : sprites) {
		delete p;
		
	}


	Blend::Release();
	ResourceManager::Rleease();

	if (DepthStenciView)DepthStenciView->Release();
	if (DepthStencil)DepthStencil->Release();
	if (RenderTargetView)RenderTargetView->Release();
	if (SwapChain)SwapChain->Release();
	if (Context)Context->Release();

	
}
void framework::update(float elapsed_time/*Elapsed seconds from last frame*/)
{
	
}
void framework::render(float elapsed_time/*Elapsed seconds from last frame*/)
{
	

	

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	Context->RSSetViewports(1, &vp);


	// Just clear the backbuffer
	float ClearColor[4] = { 0.8f, 0.0f, 0.4f, 1.0f }; //red,green,blue,alpha
	Context->ClearRenderTargetView(RenderTargetView, ClearColor);


	//
	// Clear the depth buffer to 1.0 (max depth)
	//
	Context->ClearDepthStencilView(DepthStenciView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//static float angle=.0f;
	Context->OMSetRenderTargets(1, &RenderTargetView,DepthStenciView);
	//sprites[0]->render(Context,570,240,140,240,1120,720,140,240,angle,1,1,1,1);
	/*angle += 0.05f;
	
	angle += 6.0f*elapsed_time;
	float x = 0;
	float y = 0;
	for (auto&p : sprites) {
		p->render(Context, x, static_cast<int>(y) % 720, 64, 64, 0, 0, 140, 240, angle, 1.0f, 0.0f, 0.0f, 1.0f);  
		x += 32;
		if (x > 1280 - 64)
		{
			x = 0;
			y += 24;
		}
	}*/
	static float alpha = .0f;
	DirectX::XMFLOAT4 rgba (1, 1, 1, alpha);
	
	static Blend::BLEND_MODE mode = Blend::NONE;
	if (GetAsyncKeyState('O') & 0x01)		
		mode = (Blend::BLEND_MODE)((mode + (Blend::MODE_MAX - 1)) % Blend::MODE_MAX);
	if (GetAsyncKeyState('P') & 0x01)		
		mode = (Blend::BLEND_MODE)((mode + 1) % Blend::MODE_MAX);

	if (GetAsyncKeyState('K') < 0)
		alpha = (alpha -= 0.005f) < 0.0f ? 0.0f : alpha;
	if (GetAsyncKeyState('L') < 0)
		alpha = (alpha += 0.005f) > 1.0f ? 1.0f : alpha;

	Blend::Set(Context, Blend::NONE);
	//sprites[0]->render(Context, 0, 0, 960, 540, 0, 0, 1920, 1080, .0f, rgba);
	//Blend::Set(Context, mode);
	//sprites[1]->render(Context, 0, 0, 1280, 720, 0, 0, 600, 300, .0f, rgba);
	sprites[1]->render(Context, 0, 0, 1280, 720, 0, 0, 4096, 2048, .0f, rgba);

	

	//static DirectX::XMFLOAT2 sprite_position[1024] = {};//screen space
	//static float timer = 0; //0-4(sec)
	//timer += elapsed_time;
	//if (timer > 4.0f) {
	//	for (auto&p : sprite_position) {
	//		float a = ((float)rand()) / RAND_MAX*360.0f;//angle(degree)
	//		float r = ((float)rand()) / RAND_MAX*256.0f;//radius(screen space)
	//		p.x = cosf(a*0.01745f)*r;
	//		p.y = sinf(a*0.01745f)*r;
	//	}
	//	timer = 0;
	//}
	//Blend::Set(Context, Blend::ADD);
	//for (auto&p : sprite_position) {
	//	particle->render(Context, p.x + 256, p.y + 256, 128, 128, 0, 0, 420, 420,
	//		angle * 4, 0.2f, 0.05f*timer, 0.01f*timer, fabsf(sinf(3.141592f*timer*0.5f*0.5f)));
	//}
	
	

	const DirectX::XMFLOAT4 cpos(10, 10, -25,1);  
	const DirectX::XMFLOAT4 ctarget(0, 0, 0, 1);

	static DirectX::XMFLOAT3 pos(0, -0.5, 0);
	//仮
	static DirectX::XMFLOAT3 pos2(+1.0, -0.3, 0);
	//
	static DirectX::XMFLOAT3 angle(-1.25, 0, 0);
	static DirectX::XMFLOAT3 scale(0.01, 0.01, 0.01);

	static float fovY = DirectX::XMConvertToRadians(30.0f);
	
	static DirectX::XMFLOAT4 light_dir(0, 0, 1, 0);
//	static DirectX::XMFLOAT4 light_dir(1.0, -1.0, -1.0, 0);
	static DirectX::XMFLOAT4 Matrial_col(1, 1, 1, 1);

	//angle.x += DirectX::XMConvertToRadians(30.0f)*elapsed_time;
	//angle.y += DirectX::XMConvertToRadians(30.0f)*elapsed_time;
	angle.z += DirectX::XMConvertToRadians(30.0f)*elapsed_time;
	
	/*if (GetAsyncKeyState(VK_LEFT) & 1)pos.x -= 0.2f;
	if (GetAsyncKeyState(VK_RIGHT)  & 1)pos.x += 0.2f;
	if (GetAsyncKeyState(VK_UP)		& 1)pos.y += 0.2f;
	if (GetAsyncKeyState(VK_DOWN)   & 1)pos.y -= 0.2f;*/
	
		//ワールド変換
	DirectX::XMMATRIX S, Rx, Ry, Rz, R, T, W,W2;					//ここを数学とかでした!!Quaterni恩!!化する(ジンバルロックを直そうの巻き)
	S  = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);   
	Rx = DirectX::XMMatrixRotationX(angle.x);
	Ry = DirectX::XMMatrixRotationY(angle.y);
	Rz = DirectX::XMMatrixRotationZ(angle.z);
	R  = Rz*Rx*Ry;
	T  = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	W  = S*R*T;  //状況によって変えることがあるが8割り方これでいける

	//仮
	S = DirectX::XMMatrixScaling(0.3, 0.3, 0.3);
	T = DirectX::XMMatrixTranslation(pos2.x, pos2.y, pos2.z);
	W2 = S*R*T;


				//ビュー変換行列
	DirectX::XMMATRIX  V;
	const DirectX::XMFLOAT4 Up(0, 1, 0, 0);
	DirectX::XMVECTOR cp, ct, up;
	cp = DirectX::XMLoadFloat4(&cpos);
	ct = DirectX::XMLoadFloat4(&ctarget);
	up = DirectX::XMLoadFloat4(&Up);
	V = DirectX::XMMatrixLookAtLH(cp, ct, up);

	//プロジェクション変換
	DirectX::XMMATRIX P;
	static bool bPers = false;
	const float aspect = 1280 / (float)720;		//画面の縦横で割る
	if (GetAsyncKeyState('A') & 1)bPers = !bPers;
	if (!bPers)
		P = DirectX::XMMatrixOrthographicLH(2 * aspect, 2, 0.1f, 200.0f);
	else
		P = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, 0.1f, 200.0f); //fovY 視野角

																			//描画
	static bool bWareframe = false;

	if (GetAsyncKeyState('E') & 1)bWareframe = !bWareframe;

	DirectX::XMFLOAT4X4 w,w2, wvp,wvp2;
	
	DirectX::XMStoreFloat4x4(&w, W);
	DirectX::XMStoreFloat4x4(&wvp, W*V*P);

	DirectX::XMStoreFloat4x4(&w2, W2);
	DirectX::XMStoreFloat4x4(&wvp2, W2*V*P);
	//描画

//	 cube->render(Context, wvp, w, light_dir, Matrial_col, bWareframe);
	//Cylinder->render(Context, wvp, w, light_dir, Matrial_col, bWareframe);


	static ID3D11VertexShader* vs = nullptr;
	static ID3D11InputLayout*  il = nullptr;
	static ID3D11PixelShader*  ps = nullptr;
	//	デフォルト
	if (GetAsyncKeyState('0') & 0x01) {
		vs = nullptr;
		il = nullptr;
		ps = nullptr;
	}
	//	半球ライティング+平行光
	if (GetAsyncKeyState('1') & 0x01) {
		vs = HSL_Vertex;
		il = HSL_Layout;
		ps = HSL_Pixel;
	}
	//	映り込み
	static bool bg = false;
	if (GetAsyncKeyState('2') & 0x01) {
		vs = TEST_Vertex;
		il = TEST_Layout;
		ps = TEST_Pixel;
		
	}
	
		
	static bool box_draw = false;
	if (GetAsyncKeyState('B') & 0x01) {
		box_draw = !box_draw;
	}
	if (box_draw) {
		
		sdk[0]->render(Context, wvp2, w2, light_dir, Matrial_col, bWareframe, elapsed_time, vs, il, ps);
		
	}
	
	sdk[1]->render(Context, wvp, w, light_dir, Matrial_col, bWareframe, elapsed_time, vs, il, ps);
	
	SwapChain->Present(0, 0);

}

