//
//#include"font.h"
//
//sprite*Font::img = NULL;
//float  Font::r = 0.0f;
//float  Font::g = 0.0f;
//float  Font::b = 0.0f;
//float  Font::a = 1.0f;
//
//void Font::RenderText(ID3D11DeviceContext*Context, const char*text, float x, float y, float w, float h, float rotDegree, sprite::ALIGN rotAlign)
//{
//	//画像を読み込んでいなければ終了
//	if (!img)return;
//
//	float tx,ty, tw, th;
//
//	//画像の切り取り幅&高さ(正方形マス前提)
//  //th=tw=img->TexDesc.Width/NUM_OF_1LINE;
//	th = tw = img1->GetTexDesc().width / NUM_OF_1LINE;
//
//	//テキスト描画
//	for (const char* p = text; *p;p++)
//	{
//		//切り抜き位置算出
//		tx = ((*p) & 0xF)*tx;
//		ty = (((*p) >> 4) & 0xF)*th;
//
//		//描画
//		img->render(Context, x, y, w, h, tx, ty, tw, th, .0f, r, g, b,a);
//
//		//一文字分ずらす
//		x += w;
//	}
//
//	
//
//
//}