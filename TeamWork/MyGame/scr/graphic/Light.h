#pragma once

#include <unordered_map>
#include "../math/Vector3.h"
#include "../math/Color.h"

class Light
{
public:

	///<summary>
	/// コンストラクタ（標準ライトを使用するか否か）
	///</summary>
	Light();
	explicit Light(bool value);
	~Light();

	///<summary>
	/// 初期化処理
	///</summary>
	void Initialize();

	///<summary>
	/// 通常ライトを使用するか否か
	///</summary>
	void UseStandardLight(bool value);

	///<summary>
	/// ライティング計算処理を使用するか否か
	///</summary>
	void UseLighting(bool value);

	///<summary>
	/// ディレクショナルライトをセットする（ハンドル名、ベクトル）
	///</summary>
	void SetDirectionalLight(const std::string& handlename, Vector3& vector);

	///<summary>
	/// ポイントライトをセットする（ハンドル名、座標、範囲、減衰値０、減衰値１、減衰値２）
	///</summary>
	void SetPointLight(const std::string& handlename, Vector3& position, float range, float atten0 = 0.0f, float atten1 = 0.01f, float atten2 = 0.0f);

	///<summary>
	/// スポットライトをセットする（ハンドル名、座標、ベクトル、ライトコーンの外側の角度、ライトコーンの内側の角度、範囲、減衰値０、減衰値１、減衰値２）
	///</summary>
	void SetSpotLight(const std::string& handlename, Vector3& position, Vector3& vector, float cone_outangle, float cone_inangle, float range, float atten0 = 0.0f, float atten1 = 0.01f, float atten2 = 0.0f);

	///<summary>
	/// セットしたライトを消去する
	///</summary>
	void DeleteLight(const std::string& handlename);

	///<summary>
	/// ライトの向きを設定する
	///</summary>
	void SetDirection(const std::string& handlename, Vector3& vector);

	///<summary>
	/// ライトの位置を設定する
	///</summary>
	void SetPosition(const std::string& handlename, Vector3& position);

	///<summary>
	/// ライトのディフューズカラーを設定する
	///</summary>
	void SetDiffuseColor(const std::string& handlename, Color& color);

	///<summary>
	/// ライトのスペキュラーカラーを設定する
	///</summary>
	void SetSpecularColor(const std::string& handlename, Color& color);

	///<summary>
	/// ライトのアンビエントカラーを設定する
	///</summary>
	void SetAmbientColor(const std::string& handlename, Color& color);

	///<summary>
	/// ライトのディフューズカラーを受け取る
	///</summary>
	Color GetDiffuseColor(const std::string& handlename);

	///<summary>
	/// ライトのスペキュラーカラーを受け取る
	///</summary>
	Color GetSpecularColor(const std::string& handlename);

	///<summary>
	/// ライトのアンビエントカラーを受け取る
	///</summary>
	Color GetAmbientColor(const std::string& handlename);

	///<summary>
	/// セットしたライトを有効にするか無効にするか設定する
	///</summary>
	void LightEnable(const std::string& handlename, bool value);

	///<summary>
	/// セットされているライトの数を取得する
	///</summary>
	int IsLightNumber();

	bool IsEnable(const std::string& name) const;

private:
	///<summary>
	/// 既に指定のライトが存在していたら、スローする
	///</summary>
	void AlreadyGrow(const std::string& handlename);

	///<summary>
	/// 指定のライトが存在していなければ、スローする
	///</summary>
	void NoneGrow(const std::string& handlename);

	std::unordered_map<std::string, int> m_lights;
};