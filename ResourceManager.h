#pragma once
#include <d3d11.h>

class ResourceManager
{
private:
	static const int RESOURCE_MAX = 2;

	struct ResourceShaderView
	{
		int						     iRefNum;				    //�f�[�^�̎Q�Ɛ�
		wchar_t					     path[256];					//�t�@�C���p�X
		ID3D11ShaderResourceView*    SRView;					//�ۑ��Ώ�
		
		ResourceShaderView() :iRefNum(0), SRView(NULL) { path[0]= '\0'; }


		void Release(bool bForece = false){      
			if (iRefNum == 0)return;
			if (--iRefNum <= 0) bForece = true;//�Q�Ɛ����c���Ă���Ȃ������Ȃ�     
			//�������
			if (bForece)
			{
				if (SRView)SRView->Release();
				SRView = NULL;
				iRefNum = 0;					//�Q�Ɛ���0
				path[0] = '\0';					//�t�@�C���p�X������
			}
		}

	};
	

	struct ResourceVertxShaders
	{
		int						     iRefNum;				    //�f�[�^�̎Q�Ɛ�
		wchar_t					     path[256];					//�t�@�C���p�X
		ID3D11VertexShader*	         VShader;						//�ۑ��Ώ�
		ID3D11InputLayout*           Layout;

		ResourceVertxShaders() :iRefNum(0), VShader(NULL),Layout(NULL) { path[0] = '\0'; }


		void Release(bool bForece = false) {
			if (iRefNum == 0)return;
			if (--iRefNum <= 0) bForece = true;//�Q�Ɛ����c���Ă���Ȃ������Ȃ�     
											   //�������
			if (bForece)
			{
				if (VShader)VShader->Release();
				if (Layout)Layout->Release();
				Layout = NULL;
				VShader = NULL;
				iRefNum = 0;					//�Q�Ɛ���0
				path[0] = '\0';					//�t�@�C���p�X������
			}
		}


	};

	struct ResourecePixelShader
	{
		int						     iRefNum;				    //�f�[�^�̎Q�Ɛ�
		wchar_t					     path[256];					//�t�@�C���p�X
		ID3D11PixelShader*           Pixel;						//�ۑ��Ώ�

		ResourecePixelShader() :iRefNum(0), Pixel(NULL) { path[0] = '\0'; }


		void Release(bool bForece = false) {
			if (iRefNum ==0 )return;
			if (--iRefNum <= 0) bForece = true;//�Q�Ɛ����c���Ă���Ȃ������Ȃ�     
											   //�������
			if (bForece)
			{
				if (Pixel)Pixel->Release();
				Pixel = NULL;
				iRefNum = 0;					//�Q�Ɛ���0
				path[0] = '\0';					//�t�@�C���p�X������
			}
		}


	};
	
	//�R���X�g���N�^/�f�X�g���N�^�͊O���Ŏg�p�֎~(new/delete�ł��Ȃ�)
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
//-----------------ResourceManager�͂����܂�----------






