#include "Model.h"
#include "../game/WorkFolder.h"
#include "../math/Point.h"
#include <Dxlib.h>
#include <sstream>
#include <algorithm>

#define PI 3.14159265358979f

Model::ModelHandle::ModelHandle(int cmp) :
param(cmp), handle(0),
isInit(false), isCol(false), isRef(false)
{

}
Model::ModelHandle::~ModelHandle()
{
	Delete();
}

void Model::Initialize()
{
	MV1InitModel();
	m_models.clear();
	m_sprites.clear();
	currentBlendMode = BLEND_MODE::NoBlend;
}

void Model::ModelHandle::Load(const std::string& filename, bool async, bool collinfo, bool reference, std::function<void(int)> end)
{
	handle = MV1LoadModel((WorkFolder::Name + filename).c_str());
	endcallback = end;

	if (!async)
	{
		if (handle == -1)
			throw std::string(filename + "の読み込みに失敗しました\n");

		MV1SetMaterialDrawAlphaTestAll(handle, TRUE, DX_CMP_GREATER, param);

		if (collinfo)
		{
			DxLib::MV1SetupCollInfo(handle, 0);
		}

		if (reference)
		{
			DxLib::MV1SetupReferenceMesh(handle, 0, FALSE);
		}
	}
	else
	{
		isCol = collinfo;
		isRef = reference;
	}
}
void Model::ModelHandle::Draw()
{
	MV1DrawModel(handle);
}
void Model::ModelHandle::Delete()
{
	if (handle < 1)
	{
		MV1DeleteModel(handle);
		handle = 0;
	}
}
void Model::ModelHandle::Enable()
{
	if (!isInit)
	{
		MV1SetMaterialDrawAlphaTestAll(handle, TRUE, DX_CMP_GREATER, param);

		if (isCol)
		{
			DxLib::MV1SetupCollInfo(handle, 0);
		}
		if (isRef)
		{
			DxLib::MV1SetupReferenceMesh(handle, 0, FALSE);
		}

		if (endcallback != nullptr)
		{
			endcallback(handle);
		}
	}
}
bool Model::ModelHandle::CheckASync()
{
	return CheckHandleASyncLoad(handle) == FALSE;
}

Model::~Model()
{
	m_sprites.clear();
	m_models.clear();
	MV1InitModel();
}

// ３Ｄモデルを読み込む（アセット名）
void Model::Load(const std::string& filename, const MODEL_ID& id, bool async, int param, bool colinfo, bool reference, std::function<void(int)> end)
{
	auto model = std::make_shared<ModelHandle>(param);
	model->Load(filename, async, colinfo, reference, end);
	m_models[id] = model;
}

// ２Ｄ画像を読み込む（アセット名）
void Model::Load2D(const std::string& filename, const MODEL_ID& id)
{
	m_sprites[id].push_back(LoadGraph((WorkFolder::Name + filename).c_str()));
	if (m_sprites[id][0] == -1)
		throw std::string(id + "の読み込みに失敗しました\n");
}

// 分割２Ｄ画像の読み込み(アセット名、分割数、横と縦の分割数、1コマの大きさ)
void Model::Load2D(const std::string& filename, const MODEL_ID& id, int allnum, const Point& splitnum, const Point& size)
{
	int* handle = new int[allnum];
	LoadDivGraph((WorkFolder::Name + filename).c_str(), allnum, splitnum.x, splitnum.y, size.x, size.y, handle);
	for (int i = 0; i < allnum; ++i)
	{
		m_sprites[id].emplace_back(handle[i]);
	}
	delete[] handle;
}

void Model::DeleteAll()
{
	std::unordered_map<MODEL_ID, HandlePtr>::iterator mitr = m_models.begin();
	while (mitr != m_models.end()) {
		MV1DeleteModel(mitr->first);
		++mitr;
	}
	m_models.clear();

	std::unordered_map<MODEL_ID, std::vector<int>>::iterator sitr = m_sprites.begin();
	while (sitr != m_sprites.end()) {
		DeleteGraph(sitr->first);
		++sitr;
	}
	m_sprites.clear();
}

void Model::Delete(const MODEL_ID& id)
{
	MV1DeleteModel(m_models.at(id)->GetHangle());
}

void Model::Draw(const MODEL_ID& id, const Vector3& position)
{
	auto handle = m_models.at(id)->GetHangle();
	MV1SetPosition(handle, VGet(position.x, position.y, position.z));
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Vector3& position, float alpha)
{
	auto handle = m_models.at(id)->GetHangle();
	MV1SetPosition(handle, VGet(position.x, position.y, position.z));
	MV1SetOpacityRate(handle, alpha);
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Vector3& position, Vector3 rotate)
{
	rotate = rotate * PI / 180.0f;

	auto handle = m_models.at(id)->GetHangle();
	MV1SetPosition(handle, VGet(position.x, position.y, position.z));
	MV1SetRotationXYZ(handle, VGet(rotate.x, rotate.y, rotate.z));
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Vector3& position, Vector3 rotate, const Vector3& scale)
{
	rotate = rotate * PI / 180.0f;

 	auto handle = m_models.at(id)->GetHangle();
	MV1SetPosition(handle, VGet(position.x, position.y, position.z));
	MV1SetRotationXYZ(handle, VGet(rotate.x, rotate.y, rotate.z));
	MV1SetScale(handle, VGet(scale.x, scale.y, scale.z));
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Vector3& position, float alpha, Vector3 rotate)
{
	rotate = rotate * PI / 180.0f;

	auto handle = m_models.at(id)->GetHangle();
	MV1SetPosition(handle, VGet(position.x, position.y, position.z));
	MV1SetOpacityRate(handle, alpha);
	MV1SetRotationXYZ(handle, VGet(rotate.x, rotate.y, rotate.z));
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Vector3& position, float alpha, Vector3 rotate, const Vector3& scale)
{
	rotate = rotate * PI / 180.0f;

	auto handle = m_models.at(id)->GetHangle();
	MV1SetPosition(handle, VGet(position.x, position.y, position.z));
	MV1SetOpacityRate(handle, alpha);
	MV1SetRotationXYZ(handle, VGet(rotate.x, rotate.y, rotate.z));
	MV1SetScale(handle, VGet(scale.x, scale.y, scale.z));
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Vector3& position, const Color& diffusecolor)
{
	auto handle = m_models.at(id)->GetHangle();
	auto diffuse = diffusecolor * 255.0f;
	MV1SetPosition(handle, VGet(position.x, position.y, position.z));
	MV1SetDifColorScale(handle, GetColorF(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Vector3& position, const Color& diffusecolor, const Color& specularcolor)
{
	auto handle = m_models.at(id)->GetHangle();
	auto diffuse = diffusecolor * 255.0f;
	auto specular = specularcolor * 255.0f;
	MV1SetPosition(handle, VGet(position.x, position.y, position.z));
	MV1SetDifColorScale(handle, GetColorF(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
	MV1SetSpcColorScale(handle, GetColorF(specular.r, specular.g, specular.b, specular.a));
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Vector3& position, float alpha, Vector3 rotate, const Vector3& scale, const Color& diffusecolor, const Color& specularcolor)
{
	rotate = rotate * PI / 180.0f;

	auto handle = m_models.at(id)->GetHangle();	
	auto diffuse = diffusecolor * 255.0f;
	auto specular = specularcolor * 255.0f;

	MV1SetPosition(handle, VGet(position.x, position.y, position.z));
	MV1SetOpacityRate(handle, alpha);
	MV1SetRotationXYZ(handle, VGet(rotate.x, rotate.y, rotate.z));
	MV1SetScale(handle, VGet(scale.x, scale.y, scale.z));
	MV1SetDifColorScale(handle, GetColorF(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
	MV1SetSpcColorScale(handle, GetColorF(specular.r, specular.g, specular.b, specular.a));
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Vector3& position, bool trans)
{
	auto handle = m_models.at(id)->GetHangle();
	MV1SetPosition(handle, VGet(position.x, position.y, position.z));
	MV1SetVisible(handle, trans);
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Vector3& position, float alpha, bool trans)
{
	auto handle = m_models.at(id)->GetHangle();
	MV1SetPosition(handle, VGet(position.x, position.y, position.z));
	MV1SetOpacityRate(handle, alpha);
	MV1SetVisible(handle, trans);
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Vector3& position, Vector3 rotate, bool trans)
{
	rotate = rotate * PI / 180.0f;

	auto handle = m_models.at(id)->GetHangle();
	MV1SetPosition(handle, VGet(position.x, position.y, position.z));
	MV1SetRotationXYZ(handle, VGet(rotate.x, rotate.y, rotate.z));
	MV1SetVisible(handle, trans);
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Vector3& position, Vector3 rotate, const Vector3& scale, bool trans)
{
	rotate = rotate * PI / 180.0f;

	auto handle = m_models.at(id)->GetHangle();
	MV1SetPosition(handle, VGet(position.x, position.y, position.z));
	MV1SetRotationXYZ(handle, VGet(rotate.x, rotate.y, rotate.z));
	MV1SetScale(handle, VGet(scale.x, scale.y, scale.z));
	MV1SetVisible(handle, trans);
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Vector3& position, float alpha, Vector3 rotate, bool trans)
{
	rotate = rotate * PI / 180.0f;

	auto handle = m_models.at(id)->GetHangle();
	MV1SetPosition(handle, VGet(position.x, position.y, position.z));
	MV1SetOpacityRate(handle, alpha);
	MV1SetRotationXYZ(handle, VGet(rotate.x, rotate.y, rotate.z));
	MV1SetVisible(handle, trans);
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Vector3& position, float alpha, Vector3 rotate, const Vector3& scale, bool trans)
{
	rotate = rotate * PI / 180.0f;

	auto handle = m_models.at(id)->GetHangle();
	MV1SetPosition(handle, VGet(position.x, position.y, position.z));
	MV1SetOpacityRate(handle, alpha);
	MV1SetRotationXYZ(handle, VGet(rotate.x, rotate.y, rotate.z));
	MV1SetScale(handle, VGet(scale.x, scale.y, scale.z));
	MV1SetVisible(handle, trans);
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Vector3& position, const Color& diffusecolor, bool trans)
{
	auto handle = m_models.at(id)->GetHangle();
	auto diffuse = diffusecolor * 255.0f;
	MV1SetPosition(handle, VGet(position.x, position.y, position.z));
	MV1SetDifColorScale(handle, GetColorF(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
	MV1SetVisible(handle, trans);
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Vector3& position, const Color& diffusecolor, const Color& specularcolor, bool trans)
{
	auto handle = m_models.at(id)->GetHangle();
	auto diffuse = diffusecolor * 255.0f;
	auto specular = specularcolor * 255.0f;

	MV1SetPosition(handle, VGet(position.x, position.y, position.z));
	MV1SetDifColorScale(handle, GetColorF(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
	MV1SetSpcColorScale(handle, GetColorF(specular.r, specular.g, specular.b, specular.a));
	MV1SetVisible(handle, trans);
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Vector3& position, float alpha, Vector3 rotate, const Vector3& scale, const Color& diffusecolor, const Color& specularcolor, bool trans)
{
	rotate = rotate * PI / 180.0f;

	auto handle = m_models.at(id)->GetHangle();
	auto diffuse = diffusecolor * 255.0f;
	auto specular = specularcolor * 255.0f;
	
	MV1SetPosition(handle, VGet(position.x, position.y, position.z));
	MV1SetRotationXYZ(handle, VGet(rotate.x, rotate.y, rotate.z));
	MV1SetScale(handle, VGet(scale.x, scale.y, scale.z));
	MV1SetOpacityRate(handle, alpha);
	MV1SetDifColorScale(handle, GetColorF(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
	MV1SetSpcColorScale(handle, GetColorF(specular.r, specular.g, specular.b, specular.a));
	MV1SetVisible(handle, trans);
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Matrix& mat, float alpha, const Color& diffusecolor, const Color& specularcolor, bool trans)
{
	auto handle = m_models.at(id)->GetHangle();
	auto diffuse = diffusecolor * 255.0f;
	auto specular = specularcolor * 255.0f;

	MV1SetMatrix(handle, DXConverter::GetInstance().ToMATRIX(mat));
	MV1SetOpacityRate(handle, alpha);
	MV1SetDifColorScale(handle, GetColorF(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
	MV1SetSpcColorScale(handle, GetColorF(specular.r, specular.g, specular.b, specular.a));
	MV1SetVisible(handle, trans);
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Matrix& mat, float alpha, const Color& diffusecolor, const Color& specularcolor)
{
	auto handle = m_models.at(id)->GetHangle();
	auto diffuse = diffusecolor * 255.0f;
	auto specular = specularcolor * 255.0f;
	
	MV1SetMatrix(handle, DXConverter::GetInstance().ToMATRIX(mat));
	MV1SetOpacityRate(handle, alpha);
	MV1SetDifColorScale(handle, GetColorF(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
	MV1SetSpcColorScale(handle, GetColorF(specular.r, specular.g, specular.b, specular.a));
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Matrix& mat, float alpha, const Color& diffusecolor, bool trans)
{
	auto handle = m_models.at(id)->GetHangle();
	auto diffuse = diffusecolor * 255.0f;
	
	MV1SetMatrix(handle, DXConverter::GetInstance().ToMATRIX(mat));
	MV1SetOpacityRate(handle, alpha);
	MV1SetDifColorScale(handle, GetColorF(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
	MV1SetVisible(handle, trans);
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Matrix& mat, float alpha, const Color& diffusecolor)
{
	auto handle = m_models.at(id)->GetHangle();
	auto diffuse = diffusecolor * 255.0f;
	
	MV1SetMatrix(handle, DXConverter::GetInstance().ToMATRIX(mat));
	MV1SetOpacityRate(handle, alpha);
	MV1SetDifColorScale(handle, GetColorF(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Matrix& mat, float alpha, bool trans)
{
	auto handle = m_models.at(id)->GetHangle();
	MV1SetMatrix(handle, DXConverter::GetInstance().ToMATRIX(mat));
	MV1SetOpacityRate(handle, alpha);
	MV1SetVisible(handle, trans);
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Matrix& mat, float alpha)
{
	auto handle = m_models.at(id)->GetHangle();
	MV1SetMatrix(handle, DXConverter::GetInstance().ToMATRIX(mat));
	MV1SetOpacityRate(handle, alpha);
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Matrix& mat, bool trans)
{
	auto handle = m_models.at(id)->GetHangle();
	MV1SetMatrix(handle, DXConverter::GetInstance().ToMATRIX(mat));
	MV1SetVisible(handle, trans);
	MV1DrawModel(handle);
}

void Model::Draw(const MODEL_ID& id, const Matrix& mat)
{
	auto handle = m_models.at(id)->GetHangle();
	MV1SetMatrix(handle, DXConverter::GetInstance().ToMATRIX(mat));
	MV1DrawModel(handle);
}

void Model::Draw(const int& handle, const Matrix& mat)
{
	MV1SetMatrix(handle, DXConverter::GetInstance().ToMATRIX(mat));
	MV1DrawModel(handle);
}

// ３Ｄ空間に２Ｄ画像を描画する（アセット名、座標、表示コマ番号、横のサイズ）
void Model::Draw2D(const MODEL_ID& id, const Vector3& position, int frame, float size)
{
	DrawBillboard3D(VGet(position.x, position.y, position.z), 0.5f, 0.5f, size, 0.0f, m_sprites[id][frame], true);
}

// ３Ｄ空間に２Ｄ画像を描画する（アセット名、座標、表示コマ番号、横のサイズ、中心座標）
void Model::Draw2D(const MODEL_ID& id, const Vector3& position, int frame, float size, const Vector2& origin)
{
	DrawBillboard3D(VGet(position.x, position.y, position.z), origin.x, origin.y, size, 0.0f, m_sprites[id][frame], true);
}

// ３Ｄ空間に２Ｄ画像を描画する（アセット名、座標、表示コマ番号、横のサイズ、中心座標、回転量）
void Model::Draw2D(const MODEL_ID& id, const Vector3& position, int frame, float size, const Vector2& origin, float angle)
{
	DrawBillboard3D(VGet(position.x, position.y, position.z), origin.x, origin.y, size, angle, m_sprites[id][frame], true);
}

// ３Ｄ空間に２Ｄ画像を描画する（アセット名、座標、表示コマ番号、横のサイズ、中心座標、回転量、透過度）
void Model::Draw2D(const MODEL_ID& id, const Vector3& position, int frame, float size, const Vector2& origin, float angle, float alpha)
{
	AlphaBlend(alpha);
	DrawBillboard3D(VGet(position.x, position.y, position.z), origin.x, origin.y, size, angle, m_sprites[id][frame], true);
	DefaultBlend();
}

// ３Ｄ空間に２Ｄ画像を描画する（アセット名、座標、表示コマ番号、横のサイズ、中心座標、回転量、透過度、描画フラグ）
void Model::Draw2D(const MODEL_ID& id, const Vector3& position, int frame, float size, const Vector2& origin, float angle, float alpha, bool trans)
{
	AlphaBlend(alpha);
	DrawBillboard3D(VGet(position.x, position.y, position.z), origin.x, origin.y, size, angle, m_sprites[id][frame], trans);
	DefaultBlend();
}

// ３Ｄ空間に２Ｄ画像を描画する（アセット名、座標、表示コマ番号、横のサイズ、中心座標、回転量、透過度、描画フラグ、反転）
void Model::Draw2D(const MODEL_ID& id, const Vector3& position, int frame, float size, const Vector2& origin, float angle, float alpha, bool trans, bool turn)
{
	AlphaBlend(alpha);
	DrawBillboard3D(VGet(position.x, position.y, position.z), origin.x, origin.y, size, angle, m_sprites[id][frame], trans, turn);
	DefaultBlend();
}

void Model::Draw2DBlend(const MODEL_ID& id, const Vector3& position, float size, int blend_mode, int alpha)
{
	SetDrawBlendMode(blend_mode, alpha);
	DrawBillboard3D(VGet(position.x, position.y, position.z), 0.5f, 0.5f, size, 0.0f, m_sprites[id][0], true);
	SetDrawBlendMode(BLEND_MODE::NoBlend, 0);
}

void Model::Draw2DBlend(const MODEL_ID& id, const Vector3& position, int frame, float size, int blend_mode, int alpha)
{
	SetDrawBlendMode(blend_mode, alpha);
	DrawBillboard3D(VGet(position.x, position.y, position.z), 0.5f, 0.5f, size, 0.0f, m_sprites[id][frame], true);
	SetDrawBlendMode(BLEND_MODE::NoBlend, 0);
}

void Model::Draw2DBlend(const MODEL_ID& id, const Vector3& position, float size, const Vector2& origin, float angle, int blend_mode, int alpha)
{
	SetDrawBlendMode(blend_mode, alpha);
	DrawBillboard3D(VGet(position.x, position.y, position.z), origin.x, origin.y, size, angle, m_sprites[id][0], true);
	SetDrawBlendMode(BLEND_MODE::NoBlend, 0);
}
void Model::Draw2DBlend(const MODEL_ID& id, const Vector3& position, int frame, float size, const Vector2& origin, float angle, int blend_mode, int alpha)
{
	SetDrawBlendMode(blend_mode, alpha);
	DrawBillboard3D(VGet(position.x, position.y, position.z), origin.x, origin.y, size, angle, m_sprites[id][frame], true);
	SetDrawBlendMode(BLEND_MODE::NoBlend, 0);
}
void Model::Draw2DBlend(const MODEL_ID& id, const Vector3& position, float size, const Vector2& origin, float angle, bool turn, int blend_mode, int alpha)
{
	SetDrawBlendMode(blend_mode, alpha);
	DrawBillboard3D(VGet(position.x, position.y, position.z), origin.x, origin.y, size, angle, m_sprites[id][0], true, turn);
	SetDrawBlendMode(BLEND_MODE::NoBlend, 0);
}
void Model::Draw2DBlend(const MODEL_ID& id, const Vector3& position, int frame, float size, const Vector2& origin, float angle, bool turn, int blend_mode, int alpha)
{
	SetDrawBlendMode(blend_mode, alpha);
	DrawBillboard3D(VGet(position.x, position.y, position.z), origin.x, origin.y, size, angle, m_sprites[id][frame], true, turn);
	SetDrawBlendMode(BLEND_MODE::NoBlend, 0);
}


void Model::Draw2DBlendNoChange(const MODEL_ID& id, const Vector3& position, float size, int alpha)
{
	DrawBillboard3D(VGet(position.x, position.y, position.z), 0.5f, 0.5f, size, 0.0f, m_sprites[id][0], true);
}

void Model::Draw2DBlendNoChange(const MODEL_ID& id, const Vector3& position, int frame, float size, int alpha)
{
	DrawBillboard3D(VGet(position.x, position.y, position.z), 0.5f, 0.5f, size, 0.0f, m_sprites[id][frame], true);
}

void Model::Draw2DBlendNoChange(const MODEL_ID& id, const Vector3& position, float size, const Vector2& origin, float angle, int alpha)
{
	DrawBillboard3D(VGet(position.x, position.y, position.z), origin.x, origin.y, size, angle, m_sprites[id][0], true);
}
void Model::Draw2DBlendNoChange(const MODEL_ID& id, const Vector3& position, int frame, float size, const Vector2& origin, float angle, int alpha)
{
	DrawBillboard3D(VGet(position.x, position.y, position.z), origin.x, origin.y, size, angle, m_sprites[id][frame], true);
}
void Model::Draw2DBlendNoChange(const MODEL_ID& id, const Vector3& position, float size, const Vector2& origin, float angle, bool turn, int alpha)
{
	DrawBillboard3D(VGet(position.x, position.y, position.z), origin.x, origin.y, size, angle, m_sprites[id][0], true, turn);
}
void Model::Draw2DBlendNoChange(const MODEL_ID& id, const Vector3& position, int frame, float size, const Vector2& origin, float angle, bool turn, int alpha)
{
	DrawBillboard3D(VGet(position.x, position.y, position.z), origin.x, origin.y, size, angle, m_sprites[id][frame], true, turn);
}


// モデルハンドルを受け取る
int Model::GetHandle(const MODEL_ID& id)
{
	return m_models.at(id)->GetHangle();
}

// ３Ｄモデルの物理演算モードをセットする
void Model::SetPhysics(int mode)
{
	MV1SetLoadModelUsePhysicsMode(mode);
}

// ファイル名の'.'から後を取り除く
std::string Model::Split(std::string filename)
{
	std::string name;
	std::getline(
		std::stringstream(filename),
		name,
		'.'
		);
	return name;
}

// 画像にアルファブレンド処理を施す
void Model::AlphaBlend(float alpha)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255.0f * alpha));
}

// 画像のブレンド処理をデフォルトにする
void Model::DefaultBlend()
{
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

bool Model::CollisionLine(const MODEL_ID& id, const Vector3& start, const Vector3& end, Vector3* normal, Vector3* position)
{
	auto handle = m_models.at(id)->GetHangle();
	auto info = DxLib::MV1CollCheck_Line(handle, 0, VGet(start.x, start.y, start.z), VGet(end.x, end.y, end.z));

	if (info.HitFlag == TRUE)
	{
		if (normal != nullptr)
		{
			normal->x = info.Normal.x;
			normal->y = info.Normal.y;
			normal->z = info.Normal.z;
		}

		if (position != nullptr)
		{
			position->x = info.HitPosition.x;
			position->y = info.HitPosition.y;
			position->z = info.HitPosition.z;
		}
	}

	return info.HitFlag == TRUE;
}
bool Model::CollisionSphere(const MODEL_ID& id, const Vector3& center, float radius, Vector3* position, Vector3* normal)
{
	auto V = VGet(center.x, center.y, center.z);
	auto handle = m_models.at(id)->GetHangle();
	auto info = DxLib::MV1CollCheck_Sphere(handle, 0, V, radius);

	auto collide = info.HitNum > 0;

	if (collide)
	{
		if (info.HitNum == 1)
		{
			const auto& Dim = info.Dim[0];
			if (position != nullptr)
			{
				position->x = Dim.HitPosition.x;
				position->y = Dim.HitPosition.y;
				position->z = Dim.HitPosition.z;
			}
			if (normal != nullptr)
			{
				normal->x = Dim.Normal.x;
				normal->y = Dim.Normal.y;
				normal->z = Dim.Normal.z;
			}
		}
		else
		{
			for (auto i = 0; i < info.HitNum; ++i)
			{
				const auto& Dim = info.Dim[i];
				/* 中心位置、中間法線を求める */
				if (i > 0)
				{
					if (position != nullptr)
					{
						*position += DXConverter::GetInstance().ToVector3(Dim.HitPosition);
						*position *= 0.5f;
					}
					if (normal != nullptr)
					{
						*normal += DXConverter::GetInstance().ToVector3(Dim.Normal);
						normal->Normalize();
					}
				}
				/* 初期代入 */
				else
				{
					if (position != nullptr)
					{
						position->x = Dim.HitPosition.x;
						position->y = Dim.HitPosition.y;
						position->z = Dim.HitPosition.z;
					}
					if (normal != nullptr)
					{
						normal->x = Dim.Normal.x;
						normal->y = Dim.Normal.y;
						normal->z = Dim.Normal.z;
					}
				}
			}
		}
	}

	/* 破棄 */
	DxLib::MV1CollResultPolyDimTerminate(info);

	return collide;
}
bool Model::RefreshInfo(const MODEL_ID& id, const Vector3& p, const Vector3& s)
{
	/* メッシュの情報を更新する（当たり判定対象ごと、回転はない） */
	auto handle = m_models.at(id)->GetHangle();
	DxLib::MV1SetPosition(handle, VGet(p.x, p.y, p.z));
	DxLib::MV1SetScale(handle, VGet(s.x, s.y, s.z));
	/* 衝突情報の更新 */
	return DxLib::MV1RefreshCollInfo(handle, 0) == 0;
}

bool Model::CheckASyncModel()
{
	/* モデル */
	return std::all_of(m_models.begin(), m_models.end(),
		[] (const HandlePair& m)
	{
		/* ハンドルの非同期読み込みが完了しているか */
		if (m.second->CheckASync())
		{
			/* 読み込みが完了していたらマテリアルのαテストを有効にする */
			m.second->Enable();
			m.second->End();
			return true;
		}
		return false;
	});
}

bool Model::CheckASyncBillboard()
{
	/* ビルボード */
	return std::all_of(m_sprites.begin(), m_sprites.end(),
		[] (const std::pair<MODEL_ID, std::vector<int>>& s)
	{
		return std::all_of(s.second.begin(), s.second.end(),
			[] (int handle)
		{
			return CheckHandleASyncLoad(handle) == FALSE;
		});
	});
}

bool Model::IsASyncLoad(const MODEL_ID& id)
{
	return m_models.at(id)->CheckASync();
}

int Model::GetCount()
{
	return static_cast<int>(m_models.size());
}

VECTOR Model::GetMin(const MODEL_ID& id, int index)
{
	return MV1GetMeshMinPosition(m_models.at(id)->GetHangle(), index);
}

VECTOR Model::GetMax(const MODEL_ID& id, int index)
{
	return MV1GetMeshMaxPosition(m_models.at(id)->GetHangle(), index);
}