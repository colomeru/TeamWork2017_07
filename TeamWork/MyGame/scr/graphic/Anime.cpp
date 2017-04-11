#include "Anime.h"
#include "Model.h"

void Anime::AttachAnime(const MODEL_ID& id, int animestate)
{
	m_animes[id][animestate] = MV1AttachAnim(Model::GetInstance().GetHandle(id), animestate, -1, FALSE);
	if (m_animes[id][animestate] == -1)
		throw std::string("アタッチに失敗しました\n");
}

void Anime::RemoveAnime(const MODEL_ID& id, int animestate)
{
	MV1DetachAnim(Model::GetInstance().GetHandle(id), m_animes[id][animestate]);
}

void Anime::PlayAnime(const MODEL_ID& id, int animestate, float animetime)
{
	MV1SetAttachAnimTime(Model::GetInstance().GetHandle(id), m_animes[id][animestate], animetime);
}

float Anime::GetTotalTime(const MODEL_ID& id, int animestate)
{
	return MV1GetAttachAnimTotalTime(Model::GetInstance().GetHandle(id), m_animes[id][animestate]);
}

void Anime::PhysicsReset(const MODEL_ID& id)
{
	MV1PhysicsResetState(Model::GetInstance().GetHandle(id));
}

void Anime::PhysicsUpdate(const MODEL_ID& id, float time)
{
	MV1PhysicsCalculation(Model::GetInstance().GetHandle(id), time);
}