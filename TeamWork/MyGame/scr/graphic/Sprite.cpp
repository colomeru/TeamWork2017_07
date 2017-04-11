#include "Sprite.h"
#include "../game/WorkFolder.h"
#include <Dxlib.h>
#include <sstream>
#include <string>
#include <iomanip>

#define PI 3.14159265358979

Sprite::~Sprite()
{
	InitGraph();
}

// 初期化処理
void Sprite::Initialize()
{
	InitGraph();
	m_sprites.clear();
	m_splitsprites.clear();
}

// 通常画像の読み込み
void Sprite::Load(const std::string& filename, const SPRITE_ID& id)
{
	m_sprites[id] = LoadGraph((WorkFolder::Name + filename).c_str());
}

// 分割画像の読み込み
void Sprite::Load(const std::string& filename, const SPRITE_ID& id, int allnum, const Point& splitnum, const Point& size)
{
	int* handle = new int[allnum];
	LoadDivGraph((WorkFolder::Name + filename).c_str(), allnum, splitnum.x, splitnum.y, size.x, size.y, handle);
	for (int i = 0; i < allnum; ++i)
	{
		m_splitsprites[id].emplace_back(handle[i]);
	}
	delete[] handle;
}

void Sprite::DeleteAll()
{
	std::unordered_map<SPRITE_ID, int>::iterator spritr = m_sprites.begin();
	while (spritr != m_sprites.end()) {
		DeleteGraph(spritr->first);
		++spritr;
	}
	m_sprites.clear();
	m_splitsprites.clear();
}

// 通常画像の描画(アセット名、座標)
void Sprite::Draw(const SPRITE_ID& id, const Vector2& position)
{
	DrawRotaGraph3((int)position.x, (int)position.y, 0, 0,
		1.0f, 1.0f, 0.0, m_sprites[id], true, false);
}

// 通常画像の描画(アセット名、座標、アルファ値)
void Sprite::Draw(const SPRITE_ID& id, const Vector2& position, float alpha = 1.0f)
{
	AlphaBlend(alpha);
	DrawRotaGraph3((int)position.x, (int)position.y, 0, 0,
		1.0f, 1.0f, 0.0, m_sprites[id], true, false);
	DefaultBlend();
}

// 通常画像の描画(アセット名、座標、原点、拡大率)
void Sprite::Draw(const SPRITE_ID& id, const Vector2& position, const Vector2& origin = Vector2::Zero, const Vector2& scale = Vector2::One)
{
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, 0.0, m_sprites[id], true, false);
}

// 通常画像の描画(アセット名、座標、原点、アルファ値、拡大率)
void Sprite::Draw(const SPRITE_ID& id, const Vector2& position, const Vector2& origin = Vector2::Zero, float alpha = 1.0f, const Vector2& scale = Vector2::One)
{
	AlphaBlend(alpha);
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, 0.0, m_sprites[id], true, false);
	DefaultBlend();
}

// 通常画像の描画(アセット名、座標、原点、アルファ値、拡大率、回転率)
void Sprite::Draw(const SPRITE_ID& id, const Vector2& position, const Vector2& origin = Vector2::Zero, const Vector2& scale = Vector2::One, float angle = 0.0f)
{
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, angle * PI / 180.0, m_sprites[id], true, false);
}

// 通常画像の描画(アセット名、座標、原点、アルファ値、拡大率、回転率)
void Sprite::Draw(const SPRITE_ID& id, const Vector2& position, const Vector2& origin = Vector2::Zero, float alpha = 1.0f, const Vector2& scale = Vector2::One, float angle = 0.0f)
{
	AlphaBlend(alpha);
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, angle * PI / 180.0, m_sprites[id], true, false);
	DefaultBlend();
}

// 通常画像の描画(アセット名、座標、透過、反転) 
void Sprite::Draw(const SPRITE_ID& id, const Vector2& position, bool trans = true, bool turn = false)
{
	DrawRotaGraph3((int)position.x, (int)position.y, 0, 0,
		1.0, 1.0, 0.0, m_sprites[id], trans, turn);
}

// 通常画像の描画(アセット名、座標、アルファ値、透過、反転)
void Sprite::Draw(const SPRITE_ID& id, const Vector2& position, float alpha = 1.0f, bool trans = true, bool turn = false)
{
	AlphaBlend(alpha);
	DrawRotaGraph3((int)position.x, (int)position.y, 0, 0,
		1.0, 1.0, 0.0, m_sprites[id], trans, turn);
	DefaultBlend();
}

// 通常画像の描画(アセット名、座標、原点、拡大率、透過、反転)
void Sprite::Draw(const SPRITE_ID& id, const Vector2& position, const Vector2& origin = Vector2::Zero, const Vector2& scale = Vector2::One, bool trans = true, bool turn = false)
{
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, 0.0, m_sprites[id], trans, turn);
}

// 通常画像の描画(アセット名、座標、原点、アルファ値、拡大率、透過、反転)
void Sprite::Draw(const SPRITE_ID& id, const Vector2& position, const Vector2& origin = Vector2::Zero, float alpha = 1.0f, const Vector2& scale = Vector2::One, bool trans = true, bool turn = false)
{
	AlphaBlend(alpha);
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, 0.0, m_sprites[id], trans, turn);
	DefaultBlend();
}

// 通常画像の描画(アセット名、座標、原点、拡大率、回転率、透過、反転)
void Sprite::Draw(const SPRITE_ID& id, const Vector2& position, const Vector2& origin = Vector2::Zero, const Vector2& scale = Vector2::One, float angle = 0.0f, bool trans = true, bool turn = false)
{
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, (double)angle * PI / 180.0, m_sprites[id], trans, turn);
}

// 通常画像の描画(アセット名、座標、原点、アルファ値、拡大率、回転率、透過、反転)
void Sprite::Draw(const SPRITE_ID& id, const Vector2& position, const Vector2& origin = Vector2::Zero, float alpha = 1.0f, const Vector2& scale = Vector2::One, float angle = 0.0f, bool trans = true, bool turn = false)
{
	AlphaBlend(alpha);
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, (double)angle * PI / 180.0, m_sprites[id], trans, turn);
	DefaultBlend();
}

// 通常画像の描画(アセット名、座標、表示矩形サイズ、原点、アルファ値、拡大率、回転率、透過、反転)
void Sprite::Draw(const SPRITE_ID& id, const Vector2& position, const Point& size, const Vector2& origin = Vector2::Zero, float alpha = 1.0f, const Vector2& scale = Vector2::One, float angle = 0.0f, bool trans = true, bool turn = false)
{
	int handle = DerivationGraph(0, 0, size.x, size.y, m_sprites[id]);
	AlphaBlend(alpha);
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, (double)angle * PI / 180.0, handle, trans, turn);
	DeleteGraph(handle);
	DefaultBlend();
}

// 通常画像の描画(アセット名、座標、表示矩形サイズ、原点、アルファ値、拡大率、回転率、透過、反転)
void Sprite::Draw(const SPRITE_ID& id, const Vector2& position, const RECT& rect, const Vector2& origin = Vector2::Zero, float alpha = 1.0f, const Vector2& scale = Vector2::One, float angle = 0.0f, bool trans = true, bool turn = false)
{
	int handle = DerivationGraph(rect.left, rect.top, rect.right - rect.left, rect.bottom, m_sprites[id]);
	AlphaBlend(alpha);
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, (double)angle * PI / 180.0, handle, trans, turn);
	DeleteGraph(handle);
	DefaultBlend();
}

void Sprite::DrawBlend(const SPRITE_ID& id, const Vector2& position, const Vector2& origin, const Vector2& scale, float angle, const int& blendmode)
{
	SetDrawBlendMode(blendmode, 255);
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, (double)angle * PI / 180.0, m_sprites[id], true, false);
	SetDrawBlendMode(BLEND_MODE::NoBlend, 255);
}

// 分割画像の描画(アセット名、座標)
void Sprite::SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame)
{
	DrawRotaGraph3((int)position.x, (int)position.y, 0, 0,
		1.0, 1.0, 0.0, m_splitsprites[id][frame], true, false);
}

// 分割画像の描画(アセット名、座標、アルファ値)
void Sprite::SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, float alpha = 1.0f)
{
	AlphaBlend(alpha);
	DrawRotaGraph3((int)position.x, (int)position.y, 0, 0,
		1.0, 1.0, 0.0, m_splitsprites[id][frame], true, false);
	DefaultBlend();
}

// 分割画像の描画(アセット名、座標、原点、拡大率)
void Sprite::SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, const Vector2& origin = Vector2::Zero, const Vector2& scale = Vector2::One)
{
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, 0.0, m_splitsprites[id][frame], true, false);
}

// 分割画像の描画(アセット名、座標、原点、アルファ値、拡大率)
void Sprite::SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, const Vector2& origin = Vector2::Zero, float alpha = 1.0f, const Vector2& scale = Vector2::One)
{
	AlphaBlend(alpha);
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, 0.0, m_splitsprites[id][frame], true, false);
	DefaultBlend();
}

// 分割画像の描画(アセット名、座標、原点、アルファ値、拡大率、回転率)
void Sprite::SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, const Vector2& origin = Vector2::Zero, const Vector2& scale = Vector2::One, float angle = 0.0f)
{
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, angle * PI / 180.0, m_splitsprites[id][frame], true, false);
}

// 分割画像の描画(アセット名、座標、原点、アルファ値、拡大率、回転率)
void Sprite::SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, const Vector2& origin = Vector2::Zero, float alpha = 1.0f, const Vector2& scale = Vector2::One, float angle = 0.0f)
{
	AlphaBlend(alpha);
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, angle * PI / 180.0, m_splitsprites[id][frame], true, false);
	DefaultBlend();
}

// 分割画像の描画(アセット名、座標、透過、反転) 
void Sprite::SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, bool trans = true, bool turn = false)
{
	DrawRotaGraph3((int)position.x, (int)position.y, 0, 0,
		1.0, 1.0, 0.0, m_splitsprites[id][frame], trans, turn);
}

// 分割画像の描画(アセット名、座標、アルファ値、透過、反転)
void Sprite::SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, float alpha = 1.0f, bool trans = true, bool turn = false)
{
	AlphaBlend(alpha);
	DrawRotaGraph3((int)position.x, (int)position.y, 0, 0,
		1.0, 1.0, 0.0, m_splitsprites[id][frame], trans, turn);
	DefaultBlend();
}

// 分割画像の描画(アセット名、座標、原点、拡大率、透過、反転)
void Sprite::SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, const Vector2& origin = Vector2::Zero, const Vector2& scale = Vector2::One, bool trans = true, bool turn = false)
{
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, 0.0, m_splitsprites[id][frame], trans, turn);
}

// 分割画像の描画(アセット名、座標、原点、アルファ値、拡大率、透過、反転)
void Sprite::SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, const Vector2& origin = Vector2::Zero, float alpha = 1.0f, const Vector2& scale = Vector2::One, bool trans = true, bool turn = false)
{
	AlphaBlend(alpha);
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, 0.0, m_splitsprites[id][frame], trans, turn);
	DefaultBlend();
}

// 分割画像の描画(アセット名、座標、原点、拡大率、回転率、透過、反転)
void Sprite::SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, const Vector2& origin = Vector2::Zero, const Vector2& scale = Vector2::One, float angle = 0.0f, bool trans = true, bool turn = false)
{
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, (double)angle * PI / 180.0, m_splitsprites[id][frame], trans, turn);
}

// 分割画像の描画(アセット名、座標、原点、アルファ値、拡大率、回転率、透過、反転)
void Sprite::SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, const Vector2& origin = Vector2::Zero, float alpha = 1.0f, const Vector2& scale = Vector2::One, float angle = 0.0f, bool trans = true, bool turn = false)
{
	AlphaBlend(alpha);
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, (double)angle * PI / 180.0, m_splitsprites[id][frame], trans, turn);
	DefaultBlend();
}

// 分割画像の描画(アセット名、座標、表示コマ番号、表示矩形サイズ、原点、アルファ値、拡大率、回転率、透過、反転)
void Sprite::SplitDraw(const SPRITE_ID& id, const Vector2& position, int frame, const Point& size, const Vector2& origin = Vector2::Zero, float alpha = 1.0f, const Vector2& scale = Vector2::One, float angle = 0.0f, bool trans = true, bool turn = false)
{
	/*Point graphsize;
	GetGraphSize(m_splitsprites[id][frame], &graphsize.x, &graphsize.y);*/
	int handle = DerivationGraph(0, 0, size.x, size.y, m_splitsprites[id][frame]);
	AlphaBlend(alpha);
	DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,
		(double)scale.x, (double)scale.y, (double)angle * PI / 180.0, handle, trans, turn);
	DeleteGraph(handle);
	DefaultBlend();
}

// 指定の桁番目の表示コマ番号を取得する（数字、欲しい桁番目の表示コマ番号、最大桁数）
int Sprite::NumberFrame(int number, int digit, int maxdigit)
{
	// ５桁に揃えたstring型に変換
	std::ostringstream ost;
	ost << std::setfill('0') << std::setw(maxdigit) << number;
	const std::string& str = ost.str();

	std::vector<char> score;
	score.clear();

	// １文字ずつvectorに格納
	for (auto& s : str)
	{
		score.push_back(s);
	}

	// 設定された桁番目にあるスコアをセットする
	return score[maxdigit - digit] - '0';
}

// ファイル名の'.'から後を取り除く
std::string Sprite::Split(std::string filename)
{
	std::string id;
	std::getline(
		std::stringstream(filename),
		id,
		'.'
	);
	return id;
}

// 画像にアルファブレンド処理を施す
void Sprite::AlphaBlend(float alpha)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255.0f * alpha));
}

// 画像のブレンド処理をデフォルトにする
void Sprite::DefaultBlend()
{
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

bool Sprite::IsASyncLoad(const SPRITE_ID& id)
{
	return CheckHandleASyncLoad(m_sprites[id]) == FALSE;
}