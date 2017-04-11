#pragma once

#include <string>
#include <unordered_map>
#include "../math/Vector3.h"
#include "../math/Vector2.h"
#include "../math/Point.h"

class ShadowMap
{
public:
	ShadowMap() = default;
	~ShadowMap();

	///<summary>
	/// シャドウマップをセットする（sizeは2のn乗で設定する）
	///</summary>
	void Set(const std::string& handlename, Point& size);

	///<summary>
	/// シャドウマップをセットする（sizeは2のn乗で設定する）
	///</summary>
	void Set(const std::string& handlename, Point& size, Vector3& lightdirection);

	///<summary>
	/// シャドウマップを削除する
	///</summary>
	void Delete(const std::string& handlename);

	///<summary>
	/// 想定するライトの方向をセットする
	///</summary>
	void SetLightDirection(const std::string& handlename, const Vector3& lightdirection) const;

	///<summary>
	/// シャドウマップに描画する範囲を設定する
	///</summary>
	void SetRange(Vector3& minpos, Vector3& maxpos) const;

	void Enable(int slot) const ;

	void Disable(int slot) const ;

	///<summary>
	/// シャドウマップを開始する
	///</summary>
	void Begin() const ;

	///<summary>
	/// シャドウマップを終了する
	///</summary>
	void End() const ;

private:
	///<summary>
	/// 既に指定のシャドウマップが存在していたら、スローする
	///</summary>
	void AlreadyGrow(const std::string& handlename) const;

	///<summary>
	/// 指定のシャドウマップが存在していなければ、スローする
	///</summary>
	void NoneGrow(const std::string& handlename) const;

	std::unordered_map<std::string, int> m_shadowmaps;// シャドウマップ達を格納
};