#include"framework.h"
#include"WICTextureLoader.h"
#include "sprite.h"
#include "ResourceManager.h"

bool ReadBinaryFile(const char*filename, BYTE**data, unsigned int &size) {
	//cso
	FILE*fp = 0;
	if (fopen_s(&fp, filename, "rb")) return false;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	*data = new  unsigned char[size];
	fread(*data, size, 1, fp);
	fclose(fp);

	return true;
}

ResourceManager::ResourceShaderView		  ResourceManager::SRViews[ResourceManager::RESOURCE_MAX];
ResourceManager::ResourceVertxShaders     ResourceManager::VertexShaders[ResourceManager::RESOURCE_MAX];
ResourceManager::ResourecePixelShader     ResourceManager::Pixels[ResourceManager::RESOURCE_MAX];

void ResourceManager::Rleease()
{
	for (int i = 0; i < RESOURCE_MAX; i++)
	{
		SRViews[i].Release(true);
		VertexShaders[i].Release(true);
		Pixels[i].Release(true);
	}
}

bool ResourceManager::LoadShaderResourceView(ID3D11Device*Device, const wchar_t*fileName, ID3D11ShaderResourceView**SRView, D3D11_TEXTURE2D_DESC*TexDesc)
{

	int no = -1;						//�V�f�[�^�ۑ���ԍ�
	ResourceShaderView*find = NULL;		//���������f�[�^or�V�f�[�^
	ID3D11Resource*Resource = NULL;
	//�f�[�^����
	for (int n = 0; n < RESOURCE_MAX; n++) {
		ResourceShaderView*p = &SRViews[n];


		//�f�[�^�������Ȃ疳��
		//�ŏ��Ɍ�����?�J���̈�Ȃ�A�Z�b�g�p�Ɋm��
		if (p->iRefNum == 0) {
			if (no == -1)no = n;
			continue;
		}
		//�t�@�C���p�X���Ⴄ�Ȃ�
		if (wcscmp(p->path, fileName) != 0) continue;

		//�����t�@�C�������݂���
		find = p;		//����
		p->SRView->GetResource(&Resource);   //Resource�̏���
		break;
	}
	//�f�[�^��������Ȃ�������=�V�K�ǂݍ���
	if (!find)
	{

		ResourceShaderView* p = &SRViews[no];
		HRESULT	hr = DirectX::CreateWICTextureFromFile(Device, fileName, &Resource, &(p->SRView));
		if (FAILED(hr)) {
			_ASSERTE(false, "FAILED LOAD TEXTURE");
			return false;
		}
		find = p;					          //����
		wcscpy_s(p->path, 256, fileName);    //���O�R�s�[
	}

	//�ŏI����(�Q�Ɠn���Ńf�[�^��Ԃ�)
	ID3D11Texture2D*texture2d;
	Resource->QueryInterface(&texture2d);  //�e�N�X�`���f�[�^�̏���

	texture2d->GetDesc(TexDesc);		   //�f�[�^�R�s�[
	*SRView = find->SRView;


	find->iRefNum++;					   //����ǉ�

	texture2d->Release();				   //�s�v�f�[�^���
	Resource->Release();

	return true;


}
bool ResourceManager::LoadVertexShader(ID3D11Device*Device, const char*csofileName, D3D11_INPUT_ELEMENT_DESC*InputElementDesc, int numElements, ID3D11VertexShader**VertexShader, ID3D11InputLayout**InputLayout)
{

	*VertexShader = NULL;
	*InputLayout = NULL;

	ResourceVertxShaders* find = NULL;		  //�V�f�[�^�ۑ���ԍ�
	int                   no = -1;			  //���������f�[�^or�V�f�[�^

	wchar_t   filename[256];
	size_t    stringSize = 0;
	mbstowcs_s(&stringSize, filename, csofileName, strlen(csofileName));

	//�f�[�^����
	for (int n = 0; n < RESOURCE_MAX; n++) {
		ResourceVertxShaders*p = &VertexShaders[n];


		if (p->iRefNum == 0)
		{
			if (no == -1)no = n;
			continue;
		}
		//�t�@�C���p�X���Ⴄ�Ȃ璎
		if (wcscmp(p->path, filename) != 0)continue;

		//�����t�@�C�������݂���
		find = p;
		break;
	}

	//�f�[�^��������Ȃ�������=�V�K�ǂݍ���
	if (!find)
	{
		ResourceVertxShaders*p = &VertexShaders[no];

		BYTE*data;
		unsigned int size;
		//�R���p�C���ς݃V�F�[�_�[�I�u�W�F�N�g�̓ǂݍ���
		if (!ReadBinaryFile(csofileName, &data, size))return false;
		//���_�V�F�[�_�[�I�u�W�F�N�g�̐���
		HRESULT hr = Device->CreateVertexShader(data, size, nullptr, &p->VShader);
		if (FAILED(hr)) {
			delete[] data;
			return false;
		}
		//���̓��C�A�E�g�쐬
		hr = Device->CreateInputLayout(InputElementDesc, numElements, data, size, &p->Layout);
		delete[]data;
		if (FAILED(hr)) {
			_ASSERTE(false, "CreateInputLayout failed");
			return false;
		}
		//�V�K�f�[�^
		find = p;
		wcscpy_s(find->path, 256, filename);    //���O�R�s�[
	}

	//�ŏI����(�Q�Ɠn���Ńf�[�^��Ԃ�)
	*VertexShader = find->VShader;
	*InputLayout = find->Layout;
	find->iRefNum++;

	return true;
}
bool ResourceManager::LoadPixelShader(ID3D11Device*Device, const char *csofileName, ID3D11PixelShader**Pixel)
{
	int no = -1;							//�V�f�[�^�ۑ���ԍ�
	ResourecePixelShader*find = NULL;		//���������f�[�^or�V�f�[�^

	wchar_t filename[256];
	size_t stringSize = 0;
	mbstowcs_s(&stringSize, filename, csofileName, strlen(csofileName));
	//�f�[�^����
	for (int n = 0; n < RESOURCE_MAX; n++) {
		ResourecePixelShader*p = &Pixels[n];


		//�f�[�^�������Ȃ疳��
		//�ŏ��Ɍ�����?�J���̈�Ȃ�A�Z�b�g�p�Ɋm��
		if (p->iRefNum == 0) {
			if (no == -1)no = n;
			continue;
		}
		//�t�@�C���p�X���Ⴄ�Ȃ�
		if (wcscmp(p->path, filename) != 0) continue;

		//�����t�@�C�������݂���
		find = p;		//����
		break;
	}
	//�f�[�^��������Ȃ�������=�V�K�ǂݍ���
	if (!find)
	{
		ResourecePixelShader*p = &Pixels[no];

		BYTE*data;
		unsigned int size;

		if (!ReadBinaryFile(csofileName, &data, size))return false;
		HRESULT hr = Device->CreatePixelShader(data, size, nullptr, &p->Pixel);
		delete[] data;
		if (FAILED(hr)) {
			return false;
		}

		//�V�K�f�[�^
		find = p;
		wcscpy_s(p->path, 256, filename);    //���O�R�s�[
	}

	//�ŏI����(�Q�Ɠn���Ńf�[�^��Ԃ�)
	*Pixel = find->Pixel;

	find->iRefNum++;

	return true;


}



void ResourceManager::ReleaseShaderResourceView(ID3D11ShaderResourceView*SRView) {

	if (!SRView) return;			//�����f�[�^�������Ȃ�I��

	for (int n = 0; n < RESOURCE_MAX; n++) {

		ResourceShaderView*p = &SRViews[n];

		//�f�[�^�������Ȃ疳��
		if (p->iRefNum == 0) continue;

		//�f�[�^���Ⴄ�Ȃ�
		if (SRView != p->SRView) continue;

		//�f�[�^�����݂���
		p->Release();   //�f�[�^���
		break;

	}
}

void ResourceManager::ReleaseVertexShader(ID3D11VertexShader * VertexShader, ID3D11InputLayout * InputLayout)
{
	if (!VertexShaders)return;
	if (!InputLayout)return;

	for (int n = 0; n < RESOURCE_MAX; n++) {

		ResourceVertxShaders*p = &VertexShaders[n];

		//�f�[�^�������Ȃ疳��
		if (p->iRefNum == 0) continue;

		//�f�[�^���Ⴄ�Ȃ�
		if (VertexShader != p->VShader) continue;
		if (InputLayout != p->Layout)continue;

		//�f�[�^�����݂���
		p->Release();   //�f�[�^���
		break;

	}
}

void ResourceManager::ReleasePixelShader(ID3D11PixelShader*Pixel) {

	if (!Pixel) return;   //�����f�[�^�������Ȃ�I��

	for (int n = 0; n < RESOURCE_MAX; n++) {

		ResourecePixelShader*p = &Pixels[n];

		//�f�[�^�������Ȃ疳��
		if (p->iRefNum == 0)continue;

		//�f�[�^���Ⴄ�Ȃ�
		if (Pixel != p->Pixel) continue;

		//�f�[�^�����݂���
		p->Release(); //�f�[�^���
		break;
	}

}

