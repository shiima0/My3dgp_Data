#pragma once
#include <d3d11.h>

class ResourceManager
{
private:
	static const int RESOURCE_MAX = 2;

	struct ResourceShaderView
	{
		int						     iRefNum;				    //データの参照数
		wchar_t					     path[256];					//ファイルパス
		ID3D11ShaderResourceView*    SRView;					//保存対象
		
		ResourceShaderView() :iRefNum(0), SRView(NULL) { path[0]= '\0'; }


		void Release(bool bForece = false){      
			if (iRefNum == 0)return;
			if (--iRefNum <= 0) bForece = true;//参照数が残っているなら解放しない     
			//解放処理
			if (bForece)
			{
				if (SRView)SRView->Release();
				SRView = NULL;
				iRefNum = 0;					//参照数は0
				path[0] = '\0';					//ファイルパスも消去
			}
		}

	};
	

	struct ResourceVertxShaders
	{
		int						     iRefNum;				    //データの参照数
		wchar_t					     path[256];					//ファイルパス
		ID3D11VertexShader*	         VShader;						//保存対象
		ID3D11InputLayout*           Layout;

		ResourceVertxShaders() :iRefNum(0), VShader(NULL),Layout(NULL) { path[0] = '\0'; }


		void Release(bool bForece = false) {
			if (iRefNum == 0)return;
			if (--iRefNum <= 0) bForece = true;//参照数が残っているなら解放しない     
											   //解放処理
			if (bForece)
			{
				if (VShader)VShader->Release();
				if (Layout)Layout->Release();
				Layout = NULL;
				VShader = NULL;
				iRefNum = 0;					//参照数は0
				path[0] = '\0';					//ファイルパスも消去
			}
		}


	};

	struct ResourecePixelShader
	{
		int						     iRefNum;				    //データの参照数
		wchar_t					     path[256];					//ファイルパス
		ID3D11PixelShader*           Pixel;						//保存対象

		ResourecePixelShader() :iRefNum(0), Pixel(NULL) { path[0] = '\0'; }


		void Release(bool bForece = false) {
			if (iRefNum ==0 )return;
			if (--iRefNum <= 0) bForece = true;//参照数が残っているなら解放しない     
											   //解放処理
			if (bForece)
			{
				if (Pixel)Pixel->Release();
				Pixel = NULL;
				iRefNum = 0;					//参照数は0
				path[0] = '\0';					//ファイルパスも消去
			}
		}


	};
	
	//コンストラクタ/デストラクタは外部で使用禁止(new/deleteできない)
	ResourceManager() {};
	~ResourceManager() {};

	static ResourceShaderView	SRViews[RESOURCE_MAX];
	static ResourceVertxShaders  VertexShaders[RESOURCE_MAX];
	static ResourecePixelShader   Pixels[RESOURCE_MAX];
	
public:
	static void Rleease();
	
	static  bool ResourceManager::LoadShaderResourceView(ID3D11Device*Device, const wchar_t*fileName, ID3D11ShaderResourceView**SRVviwe,D3D11_TEXTURE2D_DESC*TexDesc);
	static  bool ResourceManager::LoadPixelShader       (ID3D11Device*Device,  const char*csofileName, ID3D11PixelShader**Pixel);
	static  bool ResourceManager::LoadVertexShader		(ID3D11Device*Device, const char*csofileName,D3D11_INPUT_ELEMENT_DESC*InputElementDesc,int numElements,
		ID3D11VertexShader**Vertex,ID3D11InputLayout**InputLayout);


	static void ReleaseShaderResourceView(ID3D11ShaderResourceView * SRView);
	static void ReleaseVertexShader(ID3D11VertexShader * VertexShader, ID3D11InputLayout * InputLayout);
	static void ReleasePixelShader(ID3D11PixelShader*Pixel);

};
//-----------------ResourceManagerはここまで----------






