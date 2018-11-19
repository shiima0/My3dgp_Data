#pragma once
#include <d3d11.h>
#include<math.h>
//�@Vector�N���X
class Vector {

private:
	
	

public:
	//�A�����o�ϐ�
	float x, y, z;

	//�B�f�t�H���g�R���X�g���N�^(�����,�S�Ẵ����o�ϐ���0�ŏ�����)
	Vector() :x(0), y(0), z(0) {}

	//�C�����t���R���X�g���N�^(v1=Vector(1,1,1);)
	Vector(float a, float b, float c): x(a), y(b), z(c) {}
	
	//�D�`�F�X�^�e�B�b�N�����o�֐��i�N���X���\�b�h�j(float l= Vector::Length( v1 );)
	static float Length(const Vector &v) {
		float l;

		l = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);

		return l;
		// return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z); ����ł�ok!
	}

	//�DB:�����o�֐�(�C���X�^���X���\�b�h)(float l = v1.Length();)
	float Length() const  {

		return sqrtf(x*x + y*y + z*z);
	}

	//�E�`�F�X�^�e�B�b�N�����o�֐��i�N���X���\�b�h�j(l = Vector::LengthSq( v1 );)
	static float LengthSq(const Vector &v) {

		return(v.x*v.x + v.y*v.y + v.z*v.z);
	}

	//�E�a�F�����o�֐��i�C���X�^���X���\�b�h�j(l = v1.LengthSq();)
	float LengthSq()const {
		return(x*x + y*y + z*z);
	}

	//�F�`�F�X�^�e�B�b�N�����o�֐��i�N���X���\�b�h�j(v2 = Vector::Scale( v1, 2.0f );)
	static Vector Scale(const Vector &v,const float a) {
		 
		Vector i;
		i.x=v.x*a;
		i.y=v.y*a;
		i.z=v.z*a;
		return i;
	}
	//�F�a�F�����o�֐��i�C���X�^���X���\�b�h�j(	v3 = v1.Scale( -1.0f );)
	  static Vector Scale(const float a) {
		  Vector i;
		  i.x = a;
		  i.y = a;
		  i.z = a;

		  return i;
		
	}
	//�G���K��(v1.Normalize(&v1,&v1);
	void Normalize(const Vector *a, const Vector *b) {
		
		float len = 1 / Length();

		x *= len;
		y *= len;
		z *= len;

	  }

	//�H�`�F�X�^�e�B�b�N�����o�֐��i�N���X���\�b�h�j(v3 = Vector::Add( v1,  v2 )
	static Vector Add (const Vector &v, const Vector &v2) {
		
		Vector i;
		i.x=v.x + v2.x;
		i.y=v.y + v2.y;
		i.z=v.z + v2.z;

		return i;
	}
	//�H�a�F�����o�֐��i�C���X�^���X���\�b�h�j(v3 = v1.Add( v2 ));
	 Vector Add(const Vector &v) {
		Vector v1;
		v1.x = x + v.x;
		v1.y = y + v.y;
		v1.z = z + v.z;

		return v1;
	}
	 //�I�`�F�X�^�e�B�b�N�����o�֐��i�N���X���\�b�h)(v3 = Vector::Subtract( v1,  v2 );
	 static Vector Subtract(const Vector &v, const Vector&v2) {
		 Vector i;
		 i.x = v.x - v2.x;
		 i.y = v.y - v2.y;
		 i.z = v.z - v2.z;
		 
		 return i;
	 }
	 //�I�a�F�����o�֐��i�C���X�^���X���\�b�h�j(v3 = v1.Subtract( v2 );
	 Vector Subtract(const Vector  &v) {
		 Vector v1;
		 v1.x = x - v.x;
		 v1.y = y - v.y;
		 v1.z = z - v.z;

		 return v1;
	 }
	 //�J�`�F�X�^�e�B�b�N�����o�֐��i�N���X���\�b�h�j	(dot = Vector::Dot( v1, v2 );
	 static float Dot(const Vector &v, const Vector v2) {
		 FLOAT dot;
		 dot = (v.x*v2.x) + (v.y*v2.y) + (v.z*v2.z);
	     return dot;
	 }
	 //�J�a�F�����o�֐��i�C���X�^���X���\�b�h�j(dot = v2.Dot( v3 );
	 float Dot(const Vector &v) {
		 FLOAT dot;
		 dot = (x*v.x) + (y*v.y) + (z*v.z);
		 return dot;
	 }
	 //�K�`�F�X�^�e�B�b�N�����o�֐��i�N���X���\�b�h�j(v3 = Vector::Cross( v1, v2 );
	 static Vector Cross(const Vector &v, const Vector v2) {
		 Vector v1;
		 v1.x = v.y*v2.z - v.z*v2.y;
		 v1.y = v.z*v2.x - v.x*v2.z;
		 v1.z = v.x*v2.y - v.y*v2.x;

		 return v1;
	 }
	 //�K�a�F�����o�֐��i�C���X�^���X���\�b�h�j(v3 = v1.Cross( v2 );
	 Vector Cross(const Vector &v) {
		 Vector v1;
		 v1.x = y*v.z - z*v.y;
		 v1.y = z*v.x - x*v.z;
		 v1.z = x*v.y - y*v.x;

		 return v1;
	 }
	 //�L���Z�q�̃I�[�o�[���[�h�i���̂P)
	 //�L�`�F�t�x�N�g�� (v2 = -v1;)
	 Vector operator- () {
		 Vector v1;
		 v1.x = -x;
		 v1.y = -y;
		 v1.z = -z;

		 return v1;
	 }
	 //�L�a�F�X�J���[�{(v2 = v1 * 0.5f;)
	 Vector operator*(float f) {
		 Vector v1;
		 v1.x = x*f;
		 v1.y = y*f;
		 v1.z = z*f;

		 return v1;
	 }
	 //�L�b�F�X�J���[�{(v1 *= 0.5f;)
	 Vector operator*=(float f) {
		 Vector v1;
		 v1.x = x *= f;
		 v1.y = y *= f;
		 v1.z = z *= f;

		 return v1;
	 }
	 //�L�c�F�X�J���[�{(v2 = v1 / 2.0f;)
	 Vector operator/(float f) {
		 Vector v1;
		 v1.x = x / f;
		 v1.y = y / f;
		 v1.z = z / f;

		 return v1;
	 }
	 //�L�d�F�X�J���[�{(v1 /= 0.5f;)
	 Vector operator/=(float f) {
		 Vector v1;
		 v1.x = x /= f;
		 v1.y = y /= f;
		 v1.z = z /= f;

		 return v1;
	 }

	//�M���Z�q�̃I�[�o�[���[�h�i���̂Q�j
	//�M�`�F���Z(v3 = v1 + v2;)
	 Vector operator+(const Vector &v) {
		 Vector v1;
		 v1.x = x + v.x;
		 v1.y = y + v.y;
		 v1.z = z + v.z;

		 return v1;
	 }
	 //�M�a�F���Z(v1 += v3;)
	 Vector operator+=(const Vector &v) {
		 Vector v1;
		 v1.x = x += v.x;
		 v1.y = y += v.y;
		 v1.z = z += v.z;

		 return v1;
	 }
	 //�M�b�F���Z(	v3 = v1 - v2;)
	 Vector operator-(const Vector &v) {
		 Vector v1;
		 v1.x = x - v.x;
		 v1.y = y - v.y;
		 v1.z = z - v.z;

		 return v1;
	 }
	 //�M�c�F���Z(v2 -= v3;)
	 Vector operator-=(const Vector &v) {
		 Vector v1;
		 v1.x = x -= v.x;
		 v1.y = y -= v.y;
		 v1.z = z -= v.z;

		 return v1;
	 }
	 //�M�d�F����(dot = v1 * v2;)
	 float operator*(const Vector &v) {
		 float i;
		 i = (x*v.x) + (y*v.y) + (z*v.z);

		 return i;

		
	 }
	 //�N�c:����
	 bool operator==(const Vector &v) {
		 
		 if( x != v.x) return false;
		 if( y != v.y) return false;
		 if( z != v.z)  return false;
		  return true;
		
	 }
	 //�N�d�F�s����
	 bool operator!=(const Vector &v) {

		 if (x == v.x) return false;
		 if (y == v.y) return false;
		 if (z == v.z)  return false;
		 return true;
	 }

};

Vector operator*(float f,const Vector &v) {
	Vector v1;

	v1.x = f*v.x;
	v1.y = f*v.y;
	v1.z = f*v.z;

	return v1;
}




