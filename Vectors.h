#pragma once
#include <d3d11.h>
#include<math.h>
//�@Vectorクラス
class Vector {

private:
	
	

public:
	//�Aメンバ変数
	float x, y, z;

	//�Bデフォルトコンストラクタ(今回は,全てのメンバ変数を0で初期化)
	Vector() :x(0), y(0), z(0) {}

	//�C引数付きコンストラクタ(v1=Vector(1,1,1);)
	Vector(float a, float b, float c): x(a), y(b), z(c) {}
	
	//�DＡ：スタティックメンバ関数（クラスメソッド）(float l= Vector::Length( v1 );)
	static float Length(const Vector &v) {
		float l;

		l = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);

		return l;
		// return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z); これでもok!
	}

	//�DB:メンバ関数(インスタンスメソッド)(float l = v1.Length();)
	float Length() const  {

		return sqrtf(x*x + y*y + z*z);
	}

	//�EＡ：スタティックメンバ関数（クラスメソッド）(l = Vector::LengthSq( v1 );)
	static float LengthSq(const Vector &v) {

		return(v.x*v.x + v.y*v.y + v.z*v.z);
	}

	//�EＢ：メンバ関数（インスタンスメソッド）(l = v1.LengthSq();)
	float LengthSq()const {
		return(x*x + y*y + z*z);
	}

	//�FＡ：スタティックメンバ関数（クラスメソッド）(v2 = Vector::Scale( v1, 2.0f );)
	static Vector Scale(const Vector &v,const float a) {
		 
		Vector i;
		i.x=v.x*a;
		i.y=v.y*a;
		i.z=v.z*a;
		return i;
	}
	//�FＢ：メンバ関数（インスタンスメソッド）(	v3 = v1.Scale( -1.0f );)
	  static Vector Scale(const float a) {
		  Vector i;
		  i.x = a;
		  i.y = a;
		  i.z = a;

		  return i;
		
	}
	//�G正規化(v1.Normalize(&v1,&v1);
	void Normalize(const Vector *a, const Vector *b) {
		
		float len = 1 / Length();

		x *= len;
		y *= len;
		z *= len;

	  }

	//�HＡ：スタティックメンバ関数（クラスメソッド）(v3 = Vector::Add( v1,  v2 )
	static Vector Add (const Vector &v, const Vector &v2) {
		
		Vector i;
		i.x=v.x + v2.x;
		i.y=v.y + v2.y;
		i.z=v.z + v2.z;

		return i;
	}
	//�HＢ：メンバ関数（インスタンスメソッド）(v3 = v1.Add( v2 ));
	 Vector Add(const Vector &v) {
		Vector v1;
		v1.x = x + v.x;
		v1.y = y + v.y;
		v1.z = z + v.z;

		return v1;
	}
	 //�IＡ：スタティックメンバ関数（クラスメソッド)(v3 = Vector::Subtract( v1,  v2 );
	 static Vector Subtract(const Vector &v, const Vector&v2) {
		 Vector i;
		 i.x = v.x - v2.x;
		 i.y = v.y - v2.y;
		 i.z = v.z - v2.z;
		 
		 return i;
	 }
	 //�IＢ：メンバ関数（インスタンスメソッド）(v3 = v1.Subtract( v2 );
	 Vector Subtract(const Vector  &v) {
		 Vector v1;
		 v1.x = x - v.x;
		 v1.y = y - v.y;
		 v1.z = z - v.z;

		 return v1;
	 }
	 //�JＡ：スタティックメンバ関数（クラスメソッド）	(dot = Vector::Dot( v1, v2 );
	 static float Dot(const Vector &v, const Vector v2) {
		 FLOAT dot;
		 dot = (v.x*v2.x) + (v.y*v2.y) + (v.z*v2.z);
	     return dot;
	 }
	 //�JＢ：メンバ関数（インスタンスメソッド）(dot = v2.Dot( v3 );
	 float Dot(const Vector &v) {
		 FLOAT dot;
		 dot = (x*v.x) + (y*v.y) + (z*v.z);
		 return dot;
	 }
	 //�KＡ：スタティックメンバ関数（クラスメソッド）(v3 = Vector::Cross( v1, v2 );
	 static Vector Cross(const Vector &v, const Vector v2) {
		 Vector v1;
		 v1.x = v.y*v2.z - v.z*v2.y;
		 v1.y = v.z*v2.x - v.x*v2.z;
		 v1.z = v.x*v2.y - v.y*v2.x;

		 return v1;
	 }
	 //�KＢ：メンバ関数（インスタンスメソッド）(v3 = v1.Cross( v2 );
	 Vector Cross(const Vector &v) {
		 Vector v1;
		 v1.x = y*v.z - z*v.y;
		 v1.y = z*v.x - x*v.z;
		 v1.z = x*v.y - y*v.x;

		 return v1;
	 }
	 //�L演算子のオーバーロード（その１)
	 //�LＡ：逆ベクトル (v2 = -v1;)
	 Vector operator- () {
		 Vector v1;
		 v1.x = -x;
		 v1.y = -y;
		 v1.z = -z;

		 return v1;
	 }
	 //�LＢ：スカラー倍(v2 = v1 * 0.5f;)
	 Vector operator*(float f) {
		 Vector v1;
		 v1.x = x*f;
		 v1.y = y*f;
		 v1.z = z*f;

		 return v1;
	 }
	 //�LＣ：スカラー倍(v1 *= 0.5f;)
	 Vector operator*=(float f) {
		 Vector v1;
		 v1.x = x *= f;
		 v1.y = y *= f;
		 v1.z = z *= f;

		 return v1;
	 }
	 //�LＤ：スカラー倍(v2 = v1 / 2.0f;)
	 Vector operator/(float f) {
		 Vector v1;
		 v1.x = x / f;
		 v1.y = y / f;
		 v1.z = z / f;

		 return v1;
	 }
	 //�LＥ：スカラー倍(v1 /= 0.5f;)
	 Vector operator/=(float f) {
		 Vector v1;
		 v1.x = x /= f;
		 v1.y = y /= f;
		 v1.z = z /= f;

		 return v1;
	 }

	//�M演算子のオーバーロード（その２）
	//�MＡ：加算(v3 = v1 + v2;)
	 Vector operator+(const Vector &v) {
		 Vector v1;
		 v1.x = x + v.x;
		 v1.y = y + v.y;
		 v1.z = z + v.z;

		 return v1;
	 }
	 //�MＢ：加算(v1 += v3;)
	 Vector operator+=(const Vector &v) {
		 Vector v1;
		 v1.x = x += v.x;
		 v1.y = y += v.y;
		 v1.z = z += v.z;

		 return v1;
	 }
	 //�MＣ：減算(	v3 = v1 - v2;)
	 Vector operator-(const Vector &v) {
		 Vector v1;
		 v1.x = x - v.x;
		 v1.y = y - v.y;
		 v1.z = z - v.z;

		 return v1;
	 }
	 //�MＤ：減算(v2 -= v3;)
	 Vector operator-=(const Vector &v) {
		 Vector v1;
		 v1.x = x -= v.x;
		 v1.y = y -= v.y;
		 v1.z = z -= v.z;

		 return v1;
	 }
	 //�MＥ：内積(dot = v1 * v2;)
	 float operator*(const Vector &v) {
		 float i;
		 i = (x*v.x) + (y*v.y) + (z*v.z);

		 return i;

		
	 }
	 //�NＤ:等号
	 bool operator==(const Vector &v) {
		 
		 if( x != v.x) return false;
		 if( y != v.y) return false;
		 if( z != v.z)  return false;
		  return true;
		
	 }
	 //�NＥ：不等号
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




