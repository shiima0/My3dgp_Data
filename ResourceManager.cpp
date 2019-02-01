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

	int no = -1;						//新データ保存先番号
	ResourceShaderView*find = NULL;		//見つかったデータor新データ
	ID3D11Resource*Resource = NULL;
	//データ検索
	for (int n = 0; n < RESOURCE_MAX; n++) {
		ResourceShaderView*p = &SRViews[n];


		//データが無いなら無視
		//最初に見つけた?開き領域なら、セット用に確保
		if (p->iRefNum == 0) {
			if (no == -1)no = n;
			continue;
		}
		//ファイルパスが違うなら
		if (wcscmp(p->path, fileName) != 0) continue;

		//同盟ファイルが存在した
		find = p;		//発見
		p->SRView->GetResource(&Resource);   //Resourceの準備
		break;
	}
	//データが見つからなかったら=新規読み込み
	if (!find)
	{

		ResourceShaderView* p = &SRViews[no];
		HRESULT	hr = DirectX::CreateWICTextureFromFile(Device, fileName, &Resource, &(p->SRView));
		if (FAILED(hr)) {
			_ASSERTE(false, "FAILED LOAD TEXTURE");
			return false;
		}
		find = p;					          //発見
		wcscpy_s(p->path, 256, fileName);    //名前コピー
	}

	//最終処理(参照渡しでデータを返す)
	ID3D11Texture2D*texture2d;
	Resource->QueryInterface(&texture2d);  //テクスチャデータの準備

	texture2d->GetDesc(TexDesc);		   //データコピー
	*SRView = find->SRView;


	find->iRefNum++;					   //数を追加

	texture2d->Release();				   //不要データ解放
	Resource->Release();

	return true;


}
bool ResourceManager::LoadVertexShader(ID3D11Device*Device, const char*csofileName, D3D11_INPUT_ELEMENT_DESC*InputElementDesc, int numElements, ID3D11VertexShader**VertexShader, ID3D11InputLayout**InputLayout)
{

	*VertexShader = NULL;
	*InputLayout = NULL;

	ResourceVertxShaders* find = NULL;		  //新データ保存先番号
	int                   no = -1;			  //見つかったデータor新データ

	wchar_t   filename[256];
	size_t    stringSize = 0;
	mbstowcs_s(&stringSize, filename, csofileName, strlen(csofileName));

	//データ検索
	for (int n = 0; n < RESOURCE_MAX; n++) {
		ResourceVertxShaders*p = &VertexShaders[n];


		if (p->iRefNum == 0)
		{
			if (no == -1)no = n;
			continue;
		}
		//ファイルパスが違うなら虫
		if (wcscmp(p->path, filename) != 0)continue;

		//同名ファイルが存在した
		find = p;
		break;
	}

	//データが見つからなかったら=新規読み込み
	if (!find)
	{
		ResourceVertxShaders*p = &VertexShaders[no];

		BYTE*data;
		unsigned int size;
		//コンパイル済みシェーダーオブジェクトの読み込み
		if (!ReadBinaryFile(csofileName, &data, size))return false;
		//頂点シェーダーオブジェクトの生成
		HRESULT hr = Device->CreateVertexShader(data, size, nullptr, &p->VShader);
		if (FAILED(hr)) {
			delete[] data;
			return false;
		}
		//入力レイアウト作成
		hr = Device->CreateInputLayout(InputElementDesc, numElements, data, size, &p->Layout);
		delete[]data;
		if (FAILED(hr)) {
			_ASSERTE(false, "CreateInputLayout failed");
			return false;
		}
		//新規データ
		find = p;
		wcscpy_s(find->path, 256, filename);    //名前コピー
	}

	//最終処理(参照渡しでデータを返す)
	*VertexShader = find->VShader;
	*InputLayout = find->Layout;
	find->iRefNum++;

	return true;
}
bool ResourceManager::LoadPixelShader(ID3D11Device*Device, const char *csofileName, ID3D11PixelShader**Pixel)
{
	int no = -1;							//新データ保存先番号
	ResourecePixelShader*find = NULL;		//見つかったデータor新データ

	wchar_t filename[256];
	size_t stringSize = 0;
	mbstowcs_s(&stringSize, filename, csofileName, strlen(csofileName));
	//データ検索
	for (int n = 0; n < RESOURCE_MAX; n++) {
		ResourecePixelShader*p = &Pixels[n];


		//データが無いなら無視
		//最初に見つけた?開き領域なら、セット用に確保
		if (p->iRefNum == 0) {
			if (no == -1)no = n;
			continue;
		}
		//ファイルパスが違うなら
		if (wcscmp(p->path, filename) != 0) continue;

		//同盟ファイルが存在した
		find = p;		//発見
		break;
	}
	//データが見つからなかったら=新規読み込み
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

		//新規データ
		find = p;
		wcscpy_s(p->path, 256, filename);    //名前コピー
	}

	//最終処理(参照渡しでデータを返す)
	*Pixel = find->Pixel;

	find->iRefNum++;

	return true;


}



void ResourceManager::ReleaseShaderResourceView(ID3D11ShaderResourceView*SRView) {

	if (!SRView) return;			//検索データが無いなら終了

	for (int n = 0; n < RESOURCE_MAX; n++) {

		ResourceShaderView*p = &SRViews[n];

		//データが無いなら無視
		if (p->iRefNum == 0) continue;

		//データが違うなら
		if (SRView != p->SRView) continue;

		//データが存在した
		p->Release();   //データ解放
		break;

	}
}

void ResourceManager::ReleaseVertexShader(ID3D11VertexShader * VertexShader, ID3D11InputLayout * InputLayout)
{
	if (!VertexShaders)return;
	if (!InputLayout)return;

	for (int n = 0; n < RESOURCE_MAX; n++) {

		ResourceVertxShaders*p = &VertexShaders[n];

		//データが無いなら無視
		if (p->iRefNum == 0) continue;

		//データが違うなら
		if (VertexShader != p->VShader) continue;
		if (InputLayout != p->Layout)continue;

		//データが存在した
		p->Release();   //データ解放
		break;

	}
}

void ResourceManager::ReleasePixelShader(ID3D11PixelShader*Pixel) {

	if (!Pixel) return;   //検索データが無いなら終了

	for (int n = 0; n < RESOURCE_MAX; n++) {

		ResourecePixelShader*p = &Pixels[n];

		//データが無いなら無視
		if (p->iRefNum == 0)continue;

		//データが違うなら
		if (Pixel != p->Pixel) continue;

		//データが存在した
		p->Release(); //データ解放
		break;
	}

}

