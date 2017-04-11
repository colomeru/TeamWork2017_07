#include "ShadowMap.h"
#include "../conv/DXConverter.h"

ShadowMap::~ShadowMap()
{
	
}

// シャドウマップをセットする（sizeは2のn乗で設定する）
void ShadowMap::Set(const std::string& handlename, Point& size)
{
	AlreadyGrow(handlename);
	m_shadowmaps[handlename] = MakeShadowMap(size.x, size.y);
}

// シャドウマップをセットする（sizeは2のn乗で設定する）
void ShadowMap::Set(const std::string& handlename, Point& size, Vector3& lightdirection)
{
	AlreadyGrow(handlename);
	m_shadowmaps[handlename] = MakeShadowMap(size.x, size.y);
	SetShadowMapLightDirection(m_shadowmaps[handlename], VGet(lightdirection.x, lightdirection.y, lightdirection.z));
}

// シャドウマップを削除する
void ShadowMap::Delete(const std::string& handlename)
{
	NoneGrow(handlename);
	DeleteShadowMap(m_shadowmaps[handlename]);
	m_shadowmaps.erase(handlename);
}

// 想定するライトの方向をセットする
void ShadowMap::SetLightDirection(const std::string& handlename, const Vector3& lightdirection) const
{
	NoneGrow(handlename);
	SetShadowMapLightDirection(m_shadowmaps.at(handlename), VGet(lightdirection.x, lightdirection.y, lightdirection.z));
}

// シャドウマップに描画する範囲を設定する
void ShadowMap::SetRange(Vector3& minpos, Vector3& maxpos) const
{
	for (const auto& shadow : m_shadowmaps)
		SetShadowMapDrawArea(shadow.second, VGet(minpos.x, minpos.y, minpos.z), VGet(maxpos.x, maxpos.y, maxpos.z));
}

void ShadowMap::Enable(int slot) const
{
	for (auto& shadow : m_shadowmaps)
		SetUseShadowMap(slot, shadow.second);
}

void ShadowMap::Disable(int slot) const
{
	for (auto& shadow : m_shadowmaps)
		SetUseShadowMap(slot, -1);
}

// シャドウマップを開始する
void ShadowMap::Begin() const
{
	for (auto& shadow : m_shadowmaps)
		ShadowMap_DrawSetup(shadow.second);
}

// シャドウマップを終了する
void ShadowMap::End() const
{
	ShadowMap_DrawEnd();
}

// 既に指定のシャドウマップが存在していたら、スローする
void ShadowMap::AlreadyGrow(const std::string& handlename) const
{
	if (m_shadowmaps.find(handlename) != m_shadowmaps.end())
	{
		throw std::string(handlename + "シャドウマップをもうひとつ生成しようとしました");
	}
}

// 指定のシャドウマップが存在していなければ、スローする
void ShadowMap::NoneGrow(const std::string& handlename) const
{
	if (m_shadowmaps.find(handlename) == m_shadowmaps.end())
	{
		throw std::string(handlename + "シャドウマップは存在していません");
	}
}