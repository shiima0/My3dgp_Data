//#pragma once
//#include "sprite.h"
//
//class Font
//{
//private:
//	
//	static const int NUM_OF_1LINE = 16;  //‰æ‘œ‚Ìˆê—ñ‚É•À‚ñ‚Å‚¢‚é•¶Žš”
//
//	static sprite* img;
//	static float r, g, b, a;
//	Font() {};
//	~Font() {};
//
//public:
//
//	static bool LoadImage(ID3D11Device*Device, const wchar_t*filename)
//	{
//		ReleaseImage();
//		img = new sprite(Device, filename);
//		return true;
//	}
//	static void ReleaseImage()
//	{
//		if (img)
//		{
//			delete img;
//			img = NULL;
//		}
//	}
//	static void SetColor(DWORD rgba)
//	{
//		float  r = ((rgba >> 24) & 0xFF) / 255.0f;
//		float  g = ((rgba >> 16) & 0xFF) / 255.0f;
//		float  b = ((rgba >> 8) & 0xFF)  / 255.0f;
//		float  a = ((rgba >> 0) & 0xFF)  / 255.0f;
//		SetColor(r, g, b, a);
//	}
//	static void SetColor(float r, float g, float b, float a)
//	{
//		Font::r = r;
//		Font::g = g;
//		Font::b = b;
//		Font::a = a;
//	}
//
//	static void RenderText(ID3D11DeviceContext*Context,
//		const char*text,
//		float x, float y, float w, float h,
//		float rotDegree, 
//		sprite::ALIGN rotAlign = sprite::ALIGN_CENTERMIDDLE);
//};
