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
//	//�摜��ǂݍ���ł��Ȃ���ΏI��
//	if (!img)return;
//
//	float tx,ty, tw, th;
//
//	//�摜�̐؂��蕝&����(�����`�}�X�O��)
//  //th=tw=img->TexDesc.Width/NUM_OF_1LINE;
//	th = tw = img1->GetTexDesc().width / NUM_OF_1LINE;
//
//	//�e�L�X�g�`��
//	for (const char* p = text; *p;p++)
//	{
//		//�؂蔲���ʒu�Z�o
//		tx = ((*p) & 0xF)*tx;
//		ty = (((*p) >> 4) & 0xF)*th;
//
//		//�`��
//		img->render(Context, x, y, w, h, tx, ty, tw, th, .0f, r, g, b,a);
//
//		//�ꕶ�������炷
//		x += w;
//	}
//
//	
//
//
//}