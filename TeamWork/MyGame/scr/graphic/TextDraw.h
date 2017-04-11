#pragma once

#include <Dxlib.h>
#include <string>
#include "../math/Vector2.h"
#include "../math/Point.h"
#include "../math/Vector3.h"

class TextDraw
{
public:
	TextDraw() = default;
	~TextDraw() = default;

	/// <summary>
	/// 文字列を画面上の座標(0,0)に白文字で描画する
	/// </summary>
	static void Draw(const std::string& s);

	/// <summary>
	/// 文字列を白文字で描画する ( 座標 , 文字列 )
	/// </summary>
	static void Draw(const Vector2& pos, const std::string& s);
	/// <summary>
	/// 文字列を描画する ( 座標 , 色(0,0,0)～(255,255,255) , 文字列 )
	/// </summary>
	static void Draw(const Vector2& pos, const Vector3& color, const std::string& s);
	/// <summary>
	/// 文字列を描画する ( 座標 , 色(0,0,0)～(255,255,255) , 文字サイズ , 文字列 )
	/// </summary>
	static void Draw(const Vector2& pos, const Vector3& color, const float& size, const std::string& s);
	/// <summary>
	/// 文字列を描画する ( 座標 , 色(0,0,0)～(255,255,255) , 文字サイズ , 文字列 )
	/// </summary>
	static void Draw(const Vector2& pos, const Vector3& color, const double& size, const std::string& s);
	/// <summary>
	/// 文字列を描画する ( 座標 , 色(0,0,0)～(255,255,255) , 文字サイズX , 文字サイズY , 文字列 )
	/// </summary>
	static void Draw(const Vector2& pos, const Vector3& color, const float& sizeX, const float& sizeY, const std::string& s);
	/// <summary>
	/// 文字列を描画する ( 座標 , 色(0,0,0)～(255,255,255) , 文字サイズX , 文字サイズY , 文字列 )
	/// </summary>
	static void Draw(const Vector2& pos, const Vector3& color, const double& sizeX, const double& sizeY, const std::string& s);


	/// <summary>
	/// 文字列を白文字で描画する ( 座標 , 文字列 )
	/// </summary>
	static void Draw(const Point& pos, const std::string& s);
	/// <summary>
	/// 文字列を描画する ( 座標 , 色(0,0,0)～(255,255,255) , 文字列 )
	/// </summary>
	static void Draw(const Point& pos, const Vector3& color, const std::string& s);
	/// <summary>
	/// 文字列を描画する ( 座標 , 色(0,0,0)～(255,255,255) , 文字サイズ , 文字列 )
	/// </summary>
	static void Draw(const Point& pos, const Vector3& color, const float& size, const std::string& s);
	/// <summary>
	/// 文字列を描画する ( 座標 , 色(0,0,0)～(255,255,255) , 文字サイズ , 文字列 )
	/// </summary>
	static void Draw(const Point& pos, const Vector3& color, const double& size, const std::string& s);
	/// <summary>
	/// 文字列を描画する ( 座標 , 色(0,0,0)～(255,255,255) , 文字サイズX , 文字サイズY , 文字列 )
	/// </summary>
	static void Draw(const Point& pos, const Vector3& color, const float& sizeX, const float& sizeY, const std::string& s);
	/// <summary>
	/// 文字列を描画する ( 座標 , 色(0,0,0)～(255,255,255) , 文字サイズX , 文字サイズY , 文字列 )
	/// </summary>
	static void Draw(const Point& pos, const Vector3& color, const double& sizeX, const double& sizeY, const std::string& s);
};