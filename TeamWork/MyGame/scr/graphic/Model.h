#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include "../conv/DXConverter.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../math/Color.h"
#include "../math/Matrix.h"
#include "../game/ID.h"
#include "../math/Point.h"
#include "BlendMode.h"

class Model
{
private:
	Model() = default;
	~Model();

public:
	static Model &GetInstance(){
		static Model m;
		return m;
	}

	///<summary>
	/// 初期化処理
	///</summary>
	void Initialize();

	///<summary>
	/// ３Ｄモデルを読み込む（モデルID）
	///</summary>
	void Load(const std::string& filename, const MODEL_ID& id, bool async = false, int param = 0, bool collinfo = false, bool reference = false, std::function<void(int)> end = nullptr);
	///<summary>
	/// ２Ｄ画像を読み込む（モデルID）
	///</summary>
	void Load2D(const std::string& filename, const MODEL_ID& id);
	///<summary>
	/// 分割２Ｄ画像を読み込む(モデルID、分割数、横と縦の分割数、1コマの大きさ)
	///</summary>
	void Load2D(const std::string& filename, const MODEL_ID& id, int allnum, const Point& splitnum, const Point& size);
	///<summary>
	/// 読み込んだモデル、2D画像をすべて削除する
	///</summary>
	void DeleteAll();
	///<summary>
	/// 読み込んだモデル、2D画像を削除する
	///</summary>
	void Delete(const MODEL_ID& id);



	///<summary>
	/// ３Ｄモデルを描画する（モデルID、座標）
	///</summary>
	void Draw(const MODEL_ID& id, const Vector3& position);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、座標、透過度）
	///</summary>
	void Draw(const MODEL_ID& id, const Vector3& position, float alpha);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、座標、回転量）
	///</summary>
	void Draw(const MODEL_ID& id, const Vector3& position, Vector3 rotate);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、座標、回転量、拡大率）
	///</summary>
	void Draw(const MODEL_ID& id, const Vector3& position, Vector3 rotate, const Vector3& scale);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、座標、透過度、回転量）
	///</summary>
	void Draw(const MODEL_ID& id, const Vector3& position, float alpha, Vector3 rotate);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、座標、透過度、回転量、拡大率）
	///</summary>
	void Draw(const MODEL_ID& id, const Vector3& position, float alpha, Vector3 rotate, const Vector3& scale);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、座標、ディフューズカラー）
	///</summary>
	void Draw(const MODEL_ID& id, const Vector3& position, const Color& diffusecolor);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、座標、ディフューズカラー、スペキュラーカラー）
	///</summary>
	void Draw(const MODEL_ID& id, const Vector3& position, const Color& diffusecolor, const Color& specularcolor);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、座標、透過度、回転量、拡大率、ディフューズカラー、スペキュラーカラー）
	///</summary>
	void Draw(const MODEL_ID& id, const Vector3& position, float alpha, Vector3 rotate, const Vector3& scale, const Color& diffusecolor, const Color& specularcolor);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、座標、描画フラグ）
	///</summary>
	void Draw(const MODEL_ID& id, const Vector3& position, bool trans);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、座標、透過度、描画フラグ）
	///</summary>
	void Draw(const MODEL_ID& id, const Vector3& position, float alpha, bool trans);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、座標、回転量、描画フラグ）
	///</summary>
	void Draw(const MODEL_ID& id, const Vector3& position, Vector3 rotate, bool trans);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、座標、回転量、拡大率、描画フラグ）
	///</summary>
	void Draw(const MODEL_ID& id, const Vector3& position, Vector3 rotate, const Vector3& scale, bool trans);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、座標、透過度、回転量、描画フラグ）
	///</summary>
	void Draw(const MODEL_ID& id, const Vector3& position, float alpha, Vector3 rotate, bool trans);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、座標、透過度、回転量、拡大率、描画フラグ）
	///</summary>
	void Draw(const MODEL_ID& id, const Vector3& position, float alpha, Vector3 rotate, const Vector3& scale, bool trans);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、座標、ディフューズカラー、描画フラグ）
	///</summary>
	void Draw(const MODEL_ID& id, const Vector3& position, const Color& diffusecolor, bool trans);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、座標、ディフューズカラー、スペキュラーカラー、描画フラグ）
	///</summary>
	void Draw(const MODEL_ID& id, const Vector3& position, const Color& diffusecolor, const Color& specularcolor, bool trans);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、座標、透過度、回転量、拡大率、ディフューズカラー、スペキュラーカラー、描画フラグ）
	///</summary>
	void Draw(const MODEL_ID& id, const Vector3& position, float alpha, Vector3 rotate, const Vector3& scale, const Color& diffusecolor, const Color& specularcolor, bool trans);
	

	///<summary>
	/// ３Ｄモデルを描画する（モデルID、マトリックス、透過度、ディフューズカラー、スペキュラーカラー、描画フラグ）
	///</summary>
	void Draw(const MODEL_ID& id, const Matrix& mat, float alpha, const Color& diffusecolor, const Color& specularcolor, bool trans);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、マトリックス、透過度、ディフューズカラー、スペキュラーカラー）
	///</summary>
	void Draw(const MODEL_ID& id, const Matrix& mat, float alpha, const Color& diffusecolor, const Color& specularcolor);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、マトリックス、透過度、ディフューズカラー、描画フラグ）
	///</summary>
	void Draw(const MODEL_ID& id, const Matrix& mat, float alpha, const Color& diffusecolor, bool trans);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、マトリックス、透過度、ディフューズカラー）
	///</summary>
	void Draw(const MODEL_ID& id, const Matrix& mat, float alpha, const Color& diffusecolor);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、マトリックス、透過度、描画フラグ）
	///</summary>
	void Draw(const MODEL_ID& id, const Matrix& mat, float alpha, bool trans);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、マトリックス、透過度）
	///</summary>
	void Draw(const MODEL_ID& id, const Matrix& mat, float alpha);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、マトリックス、描画フラグ）
	///</summary>
	void Draw(const MODEL_ID& id, const Matrix& mat, bool trans);
	///<summary>
	/// ３Ｄモデルを描画する（モデルID、マトリックス）
	///</summary>
	void Draw(const MODEL_ID& id, const Matrix& mat);
	///<summary>
	/// ３Ｄモデルを描画する（モデルハンドル、マトリックス）
	///</summary>
	void Draw(const int& handle, const Matrix& mat);

	///<summary>
	/// ３Ｄ空間に２Ｄ画像を描画する（モデルID、座標、表示コマ番号、横のサイズ）
	///</summary>
	void Draw2D(const MODEL_ID& id, const Vector3& position, int frame, float size);
	///<summary>
	/// ３Ｄ空間に２Ｄ画像を描画する（モデルID、座標、表示コマ番号、横のサイズ、中心座標）
	///</summary>
	void Draw2D(const MODEL_ID& id, const Vector3& position, int frame, float size, const Vector2& origin);
	///<summary>
	/// ３Ｄ空間に２Ｄ画像を描画する（モデルID、座標、表示コマ番号、横のサイズ、中心座標、回転量）
	///</summary>
	void Draw2D(const MODEL_ID& id, const Vector3& position, int frame, float size, const Vector2& origin, float angle);
	///<summary>
	/// ３Ｄ空間に２Ｄ画像を描画する（モデルID、座標、表示コマ番号、横のサイズ、中心座標、回転量、透過度）
	///</summary>
	void Draw2D(const MODEL_ID& id, const Vector3& position, int frame, float size, const Vector2& origin, float angle, float alpha);
	///<summary>
	/// ３Ｄ空間に２Ｄ画像を描画する（モデルID、座標、表示コマ番号、横のサイズ、中心座標、回転量、透過度、描画フラグ）
	///</summary>
	void Draw2D(const MODEL_ID& id, const Vector3& position, int frame, float size, const Vector2& origin, float angle, float alpha, bool trans);
	///<summary>
	/// ３Ｄ空間に２Ｄ画像を描画する（モデルID、座標、表示コマ番号、横のサイズ、中心座標、回転量、透過度、描画フラグ、反転）
	///</summary>
	void Draw2D(const MODEL_ID& id, const Vector3& position, int frame, float size, const Vector2& origin, float angle, float alpha, bool trans, bool turn);
	
	///<summary>
	/// ３Ｄ空間に２Ｄ画像をブレンドしながら描画する（モデルID、座標、サイズ、ブレンドモード、ブレンドの強さ(アルファ値)）
	///</summary>
	void Draw2DBlend(const MODEL_ID& id, const Vector3& position, float size, int blend_mode, int alpha);
	///<summary>
	/// ３Ｄ空間に２Ｄ画像をブレンドしながら描画する（モデルID、座標、表示コマ番号、サイズ、ブレンドモード、ブレンドの強さ(アルファ値)）
	///</summary>
	void Draw2DBlend(const MODEL_ID& id, const Vector3& position, int frame, float size, int blend_mode, int alpha);
	///<summary>
	/// ３Ｄ空間に２Ｄ画像をブレンドしながら描画する（モデルID、座標、サイズ、中心座標、回転量、ブレンドモード、ブレンドの強さ(アルファ値)）
	///</summary>
	void Draw2DBlend(const MODEL_ID& id, const Vector3& position, float size, const Vector2& origin, float angle, int blend_mode, int alpha);
	///<summary>
	/// ３Ｄ空間に２Ｄ画像をブレンドしながら描画する（モデルID、座標、表示コマ番号、サイズ、中心座標、回転量、ブレンドモード、ブレンドの強さ(アルファ値)）
	///</summary>
	void Draw2DBlend(const MODEL_ID& id, const Vector3& position, int frame, float size, const Vector2& origin, float angle, int blend_mode, int alpha);
	///<summary>
	/// ３Ｄ空間に２Ｄ画像をブレンドしながら描画する（モデルID、座標、サイズ、中心座標、回転量、反転、ブレンドモード、ブレンドの強さ(アルファ値)）
	///</summary>
	void Draw2DBlend(const MODEL_ID& id, const Vector3& position, float size, const Vector2& origin, float angle, bool turn, int blend_mode, int alpha);
	///<summary>
	/// ３Ｄ空間に２Ｄ画像をブレンドしながら描画する（モデルID、座標、表示コマ番号、サイズ、中心座標、回転量、反転、ブレンドモード、ブレンドの強さ(アルファ値)）
	///</summary>
	void Draw2DBlend(const MODEL_ID& id, const Vector3& position, int frame, float size, const Vector2& origin, float angle, bool turn, int blend_mode, int alpha);


	///<summary>
	/// ブレンドモードを変えずに、３Ｄ空間に２Ｄ画像をブレンドしながら描画する（モデルID、座標、サイズ、ブレンドの強さ(アルファ値)）
	///</summary>
	void Draw2DBlendNoChange(const MODEL_ID& id, const Vector3& position, float size, int alpha);
	///<summary>
	/// ブレンドモードを変えずに、３Ｄ空間に２Ｄ画像をブレンドしながら描画する（モデルID、座標、表示コマ番号、サイズ、ブレンドの強さ(アルファ値)）
	///</summary>
	void Draw2DBlendNoChange(const MODEL_ID& id, const Vector3& position, int frame, float size, int alpha);
	///<summary>
	/// ブレンドモードを変えずに、３Ｄ空間に２Ｄ画像をブレンドしながら描画する（モデルID、座標、サイズ、中心座標、回転量、ブレンドの強さ(アルファ値)）
	///</summary>
	void Draw2DBlendNoChange(const MODEL_ID& id, const Vector3& position, float size, const Vector2& origin, float angle, int alpha);
	///<summary>
	/// ブレンドモードを変えずに、３Ｄ空間に２Ｄ画像をブレンドしながら描画する（モデルID、座標、表示コマ番号、サイズ、中心座標、回転量、ブレンドの強さ(アルファ値)）
	///</summary>
	void Draw2DBlendNoChange(const MODEL_ID& id, const Vector3& position, int frame, float size, const Vector2& origin, float angle, int alpha);
	///<summary>
	/// ブレンドモードを変えずに、３Ｄ空間に２Ｄ画像をブレンドしながら描画する（モデルID、座標、サイズ、中心座標、回転量、反転、ブレンドの強さ(アルファ値)）
	///</summary>
	void Draw2DBlendNoChange(const MODEL_ID& id, const Vector3& position, float size, const Vector2& origin, float angle, bool turn, int alpha);
	///<summary>
	/// ブレンドモードを変えずに、３Ｄ空間に２Ｄ画像をブレンドしながら描画する（モデルID、座標、表示コマ番号、サイズ、中心座標、回転量、反転、ブレンドの強さ(アルファ値)）
	///</summary>
	void Draw2DBlendNoChange(const MODEL_ID& id, const Vector3& position, int frame, float size, const Vector2& origin, float angle, bool turn, int alpha);
	

	///<summary>
	/// モデルハンドルを受け取る
	///</summary>
	int GetHandle(const MODEL_ID& id);
	///<summary>
	/// ３Ｄモデルの物理演算モードをセットする
	///</summary>
	void SetPhysics(int mode);

	bool CollisionLine(const MODEL_ID& id, const Vector3& start, const Vector3& end, Vector3* normal = nullptr, Vector3* position = nullptr);
	bool CollisionSphere(const MODEL_ID& id, const Vector3& center, float radius, Vector3* position = nullptr, Vector3* normal = nullptr);
	bool RefreshInfo(const MODEL_ID& id, const Vector3& p, const Vector3& s);
	/* 非同期時のロードチェック */
	bool CheckASyncModel();
	bool CheckASyncBillboard();
	bool IsASyncLoad(const MODEL_ID& id);

	int GetCount();

	VECTOR GetMin(const MODEL_ID& id, int index  = 0);
	VECTOR GetMax(const MODEL_ID& id, int index = 0);

private:
	std::string Split(std::string filename);
	
	///<summary>
	/// 画像にアルファブレンド処理を施す
	///</summary>
	void AlphaBlend(float alpha);

	///<summary>
	/// 画像のブレンド処理をデフォルトにする
	///</summary>
	void DefaultBlend();

	class ModelHandle
	{
	public:
		explicit ModelHandle(int cmp);
		~ModelHandle();
		ModelHandle(const ModelHandle&) = delete;
		ModelHandle& operator = (const ModelHandle&) = delete;
		ModelHandle(ModelHandle&&) = delete;
		ModelHandle& operator = (ModelHandle&&) = delete;

		/* 読み込み */
		void Load(const std::string& filename, bool async = false, bool collinfo = false, bool reference = false, std::function<void(int)> end = nullptr);
		/* 描画 */
		void Draw();
		/* 削除 */
		void Delete();
		/* 非同期時に設定したものを設定する */
		void Enable();
		/* ハンドル取得 */
		int GetHangle()
		{
			return handle;
		}

		bool CheckASync();

		void End()
		{
			isInit = true;
		}

	private:
		const int param;
		int handle;
		bool isInit;
		bool isCol;
		bool isRef;
		std::function<void(int)> endcallback;
	};

	using HandlePtr = std::shared_ptr < ModelHandle > ;
	using HandlePair = std::pair < MODEL_ID, HandlePtr >;
	std::unordered_map<MODEL_ID, HandlePtr> m_models;
	
	std::unordered_map<MODEL_ID, std::vector<int>> m_sprites;

	//現在選択中のブレンドモード
	int currentBlendMode;
};