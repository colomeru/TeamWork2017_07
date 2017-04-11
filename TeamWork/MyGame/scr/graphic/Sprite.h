#pragma once

#include "../math/Vector2.h"
#include "../math/Point.h"
#include "../game/ID.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "BlendMode.h"


class Sprite
{
private:
	Sprite() = default;
	~Sprite();
public:
	static Sprite &GetInstance(){
		static Sprite s;
		return s;
	}

	///<summary>
	/// 初期化処理
	///</summary>
	void Initialize();

	///<summary>
	/// 通常画像の読み込み(アセット名、IDの指定)
	///</summary>
	void Load(const std::string& filename, const SPRITE_ID& id);
	///<summary>
	/// 分割画像の読み込み(アセット名、IDの指定、分割数、横と縦の分割数、1コマの大きさ)
	///</summary>
	void Load(const std::string& filename, const SPRITE_ID& id, int allnum, const Point& splitnum, const Point& size);
	///<summary>
	/// 読み込んだ画像をすべて削除する
	///</summary>
	void DeleteAll();
	///<summary>
	/// 通常画像の描画(アセット名、座標)
	///</summary>
	void Draw(const SPRITE_ID& id, const Vector2& position);
	///<summary>
	/// 通常画像の描画(アセット名、座標、アルファ値)
	///</summary>
	void Draw(const SPRITE_ID& id, const Vector2& position, float alpha);
	///<summary>
	/// 通常画像の描画(アセット名、座標、原点、拡大率)
	///</summary>
	void Draw(const SPRITE_ID& id, const Vector2& position, const Vector2& origin, const Vector2& scale);
	///<summary>
	/// 通常画像の描画(アセット名、座標、原点、アルファ値、拡大率)
	///</summary>
	void Draw(const SPRITE_ID& id, const Vector2& position, const Vector2& origin, float alpha, const Vector2& scale);
	///<summary>
	/// 通常画像の描画(アセット名、座標、原点、拡大率、回転率)
	///</summary>
	void Draw(const SPRITE_ID& id, const Vector2& position, const Vector2& origin, const Vector2& scale, float angle);
	///<summary>
	/// 通常画像の描画(アセット名、座標、原点、アルファ値、拡大率、回転率)
	///</summary>
	void Draw(const SPRITE_ID& id, const Vector2& position, const Vector2& origin, float alpha, const Vector2& scale, float angle);
	///<summary>
	/// 通常画像の描画(アセット名、座標、透過、反転)
	///</summary>
	void Draw(const SPRITE_ID& id, const Vector2& position, bool trans, bool turn);
	///<summary>
	/// 通常画像の描画(アセット名、座標、アルファ値、透過、反転)
	///</summary>
	void Draw(const SPRITE_ID& id, const Vector2& position, float alpha, bool trans, bool turn);
	///<summary>
	/// 通常画像の描画(アセット名、座標、原点、拡大率、透過、反転)
	///</summary>
	void Draw(const SPRITE_ID& id, const Vector2& position, const Vector2& origin, const Vector2& scale, bool trans, bool turn);
	///<summary>
	/// 通常画像の描画(アセット名、座標、原点、アルファ値、拡大率、透過、反転)
	///</summary>
	void Draw(const SPRITE_ID& id, const Vector2& position, const Vector2& origin, float alpha, const Vector2& scale, bool trans, bool turn);
	///<summary>
	/// 通常画像の描画(アセット名、座標、原点、拡大率、回転率、透過、反転)
	///</summary>
	void Draw(const SPRITE_ID& id, const Vector2& position, const Vector2& origin, const Vector2& scale, float angle, bool trans, bool turn);
	///<summary>
	/// 通常画像の描画(アセット名、座標、原点、アルファ値、拡大率、回転率、透過、反転)
	///</summary>
	void Draw(const SPRITE_ID& id, const Vector2& position, const Vector2& origin, float alpha, const Vector2& scale, float angle, bool trans, bool turn);
	///<summary>
	/// 通常画像の描画(アセット名、座標、表示矩形サイズ、原点、アルファ値、拡大率、回転率、透過、反転)
	///</summary>
	void Draw(const SPRITE_ID& id, const Vector2& position, const Point& size, const Vector2& origin, float alpha, const Vector2& scale, float angle, bool trans, bool turn);
	///<summary>
	///<summary>
	/// 通常画像の描画(アセット名、座標、表示矩形サイズ、原点、アルファ値、拡大率、回転率、透過、反転)
	///</summary>
	void Draw(const SPRITE_ID& id, const Vector2& position, const RECT& rect, const Vector2& origin, float alpha, const Vector2& scale, float angle, bool trans, bool turn);
	///<summary>

	///<summary>
	/// ブレンド画像の描画(アセット名、座標、原点、拡大率、回転率、ブレンドモード)
	///</summary>
	void DrawBlend(const SPRITE_ID& id, const Vector2& position, const Vector2& origin, const Vector2& scale, float angle, const int& blendmode);

	/// 分割画像の描画(アセット名、座標、表示コマ番号)
	///</summary>
	void SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame);
	///<summary>
	/// 分割画像の描画(アセット名、座標、表示コマ番号、アルファ値)
	///</summary>
	void SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, float alpha);
	///<summary>
	/// 分割画像の描画(アセット名、座標、表示コマ番号、原点、拡大率)
	///</summary>
	void SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, const Vector2& origin, const Vector2& scale);
	///<summary>
	/// 分割画像の描画(アセット名、座標、表示コマ番号、原点、アルファ値、拡大率)
	///</summary>
	void SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, const Vector2& origin, float alpha, const Vector2& scale);
	///<summary>
	/// 分割画像の描画(アセット名、座標、表示コマ番号、原点、拡大率、回転率)
	///</summary>
	void SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, const Vector2& origin, const Vector2& scale, float angle);
	///<summary>
	/// 分割画像の描画(アセット名、座標、表示コマ番号、原点、アルファ値、拡大率、回転率)
	///</summary>
	void SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, const Vector2& origin, float alpha, const Vector2& scale, float angle);
	///<summary>
	/// 分割画像の描画(アセット名、座標、表示コマ番号、透過、反転)
	///</summary>
	void SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, bool trans, bool turn);
	///<summary>
	/// 分割画像の描画(アセット名、座標、表示コマ番号、アルファ値、透過、反転)
	///</summary>
	void SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, float alpha, bool trans, bool turn);
	///<summary>
	/// 分割画像の描画(アセット名、座標、表示コマ番号、原点、拡大率、透過、反転)
	///</summary>
	void SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, const Vector2& origin, const Vector2& scale, bool trans, bool turn);
	///<summary>
	/// 分割画像の描画(アセット名、座標、表示コマ番号、原点、アルファ値、拡大率、透過、反転)
	///</summary>
	void SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, const Vector2& origin, float alpha, const Vector2& scale, bool trans, bool turn);
	///<summary>
	/// 分割画像の描画(アセット名、座標、表示コマ番号、原点、拡大率、回転率、透過、反転)
	///</summary>
	void SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, const Vector2& origin, const Vector2& scale, float angle, bool trans, bool turn);
	///<summary>
	/// 分割画像の描画(アセット名、座標、表示コマ番号、原点、アルファ値、拡大率、回転率、透過、反転)
	///</summary>
	void SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, const Vector2& origin, float alpha, const Vector2& scale, float angle, bool trans, bool turn);
	///<summary>
	/// 分割画像の描画(アセット名、座標、表示コマ番号、表示矩形サイズ、原点、アルファ値、拡大率、回転率、透過、反転)
	///</summary>
	void SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, const Point& size, const Vector2& origin, float alpha, const Vector2& scale, float angle, bool trans, bool turn);

	///<summary>
	/// 指定の桁番目の表示コマ番号を取得する（数字、欲しい桁番目の表示コマ番号、最大桁数）
	///</summary>
	static int NumberFrame(int number, int digit, int maxdigit);

	bool IsASyncLoad(const SPRITE_ID& id);

	int GetCount()
	{
		return static_cast<int>(m_sprites.size());
	}

	///<summary>
	/// 画像のサイズを取得する
	///</summary>
	Point GetSize(const SPRITE_ID& id) const
	{
		Point size;
		GetGraphSize(m_sprites.at(id), &size.x, &size.y);
		return size;
	}

private:
	///<summary>
	/// ファイル名の'.'から後を取り除く
	///</summary>
	std::string Split(std::string filename);
	///<summary>
	/// 画像にアルファブレンド処理を施す
	///</summary>
	void AlphaBlend(float alpha);
	///<summary>
	/// 画像のブレンド処理をデフォルトにする
	///</summary>
	void DefaultBlend();

private:
	// 通常画像
	std::unordered_map<SPRITE_ID, int> m_sprites;
	// 分割画像
	std::unordered_map<SPRITE_ID, std::vector<int>> m_splitsprites;
};