#include "framework.h"
#include "ResourceManager.h"

#include <crtdbg.h>



bool framework::initialize(HWND hwnd)
{
	// CRT���������[�N���o�p
	//_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	// CRT���������[�N�ӏ����o
	//_CrtSetBreakAlloc(34756);
	//MessageBox(NULL, _T("�C���ނȁ[�I"), _T("���傭���傭���Ă邯�ǂ�΂������Ȃ��O�̐i��!"), MB_OK | MB_ICONHAND);

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

	//blend�C�j�V�����C�Y
	Blend::Initialize(Device);

	//sprites[0] = new sprite(Device, L"logos.jpg");
	//sprites[1] = new sprite(Device, L"n64.png");

	/*for (auto&p :sprites) {
		p = new sprite(Device, L"player-sprites.png");
	}
*/
	//particle = new sprite (Device, L"particle-smoke.png");
	
	//cube = new geometric_primitive(Device);


   // cube   = new geometric_Cube(Device);
	char* sdk_meshData[] = {
	 "000_cube.fbx",
	 "001_cube.fbx",
	 "002_cube.fbx",
	 "003_cube.fbx",
	 "004_cube.fbx",
	 "005_cube.fbx",
	"dummy_data.fbx",
	"danbo_atk.fbx",
	"ShachikuChan_ver3.fbx",
	};
	

	sdk = new skinned_mesh(Device, sdk_meshData[5]);

	return true;
}
framework::~framework() {
	
	
	if (sdk)delete sdk;
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
//    if (Device)Device->Release();
	
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
	float ClearColor[4] = { 0.2f, 0.7f, 0.0f, 1.0f }; //red,green,blue,alpha
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
	
	static Blend::BLEND_MODE mode = Blend::NONE;
	if (GetAsyncKeyState('O') & 0x01)		
		mode = (Blend::BLEND_MODE)((mode + (Blend::MODE_MAX - 1)) % Blend::MODE_MAX);
	if (GetAsyncKeyState('P') & 0x01)		
		mode = (Blend::BLEND_MODE)((mode + 1) % Blend::MODE_MAX);
	static float alpha = .0f;
	if (GetAsyncKeyState('K') < 0)
		alpha = (alpha -= 0.005f) < 0.0f ? 0.0f : alpha;
	if (GetAsyncKeyState('L') < 0)
		alpha = (alpha += 0.005f) > 1.0f ? 1.0f : alpha;

	Blend::Set(Context, Blend::NONE);
	//sprites[0]->render(Context, 0, 0, 960, 540, 0, 0, 1920, 1080, .0f, 1.0f, 1.0f, 1.0f, 1.0f);
	//Blend::Set(Context, mode);
	//sprites[1]->render(Context, 100, 100, 900, 877, 0, 0, 900, 877, .0f, 1.0f, 1.0f, 1.0f,alpha);
	

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
	

	const DirectX::XMFLOAT4 cpos(10, 5, -25,1);
	const DirectX::XMFLOAT4 ctarget(0, 0, 0, 1);

	static DirectX::XMFLOAT3 pos(0, 0, 0);
	static DirectX::XMFLOAT3 angle(0, 0, 0);
	static DirectX::XMFLOAT3 scale(1, 1, 1);

	static float fovY = DirectX::XMConvertToRadians(30.0f);
	
	static DirectX::XMFLOAT4 light_dir(0, 0, 1, 0);
	static DirectX::XMFLOAT4 Matrial_col(1, 1, 1, 1);

	/*angle.x += DirectX::XMConvertToRadians(30.0f)*elapsed_time;
	angle.y += DirectX::XMConvertToRadians(30.0f)*elapsed_time;
	angle.z += DirectX::XMConvertToRadians(30.0f)*elapsed_time;*/

	if (GetAsyncKeyState(VK_LEFT)   & 1)pos.x -= 0.2f;
	if (GetAsyncKeyState(VK_RIGHT)  & 1)pos.x += 0.2f;
	if (GetAsyncKeyState(VK_UP)		& 1)pos.y += 0.2f;
	if (GetAsyncKeyState(VK_DOWN)   & 1)pos.y -= 0.2f;
	
		//���[���h�ϊ�
	DirectX::XMMATRIX S, Rx, Ry, Rz, R, T, W;					//�����𐔊w�Ƃ��ł���!!Quaterni��!!������(�W���o�����b�N�𒼂����̊���)
	S  = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);   
	Rx = DirectX::XMMatrixRotationX(angle.x);
	Ry = DirectX::XMMatrixRotationY(angle.y);
	Rz = DirectX::XMMatrixRotationZ(angle.z);
	R  = Rz*Rx*Ry;
	T  = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	W  = S*R*T;  //�󋵂ɂ���ĕς��邱�Ƃ����邪8���������ł�����



				//�r���[�ϊ��s��
	DirectX::XMMATRIX  V;
	const DirectX::XMFLOAT4 Up(0, 1, 0, 0);
	DirectX::XMVECTOR cp, ct, up;
	cp = DirectX::XMLoadFloat4(&cpos);
	ct = DirectX::XMLoadFloat4(&ctarget);
	up = DirectX::XMLoadFloat4(&Up);
	V = DirectX::XMMatrixLookAtLH(cp, ct, up);

	//�v���W�F�N�V�����ϊ�
	DirectX::XMMATRIX P;
	static bool bPers = false;
	const float aspect = 1280 / (float)720;		//��ʂ̏c���Ŋ���
	if (GetAsyncKeyState('A') & 1)bPers = !bPers;
	if (!bPers)
		P = DirectX::XMMatrixOrthographicLH(2 * aspect, 2, 0.1f, 200.0f);
	else
		P = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, 0.1f, 200.0f); //fovY ����p

																			//�`��
	static bool bWareframe = false;

	if (GetAsyncKeyState('E') & 1)bWareframe = !bWareframe;

	DirectX::XMFLOAT4X4 w, wvp;
	DirectX::XMStoreFloat4x4(&w, W);
	DirectX::XMStoreFloat4x4(&wvp, W*V*P);
	
	//�`��

	 //cube->render(Context, wvp, w, light_dir, Matrial_col, bWareframe);
	//Cylinder->render(Context, wvp, w, light_dir, Matrial_col, bWareframe);
	sdk->render(Context, wvp, w, light_dir, Matrial_col, bWareframe,elapsed_time);
	
	SwapChain->Present(0, 0);

}

