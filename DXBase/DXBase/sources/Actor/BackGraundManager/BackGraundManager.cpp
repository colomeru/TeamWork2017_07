#include "BackGraundManager.h"
#include "../Person/Player/Player.h"
#include "../../Actor/Person/Player/PlayerBody.h"
#include "../../World/IWorld.h"
#include "../../Game/Time.h"
#include "../../Define.h"
BackGraundManager::BackGraundManager(IWorld * world) :
	stageFlag(true),
	mWorld(world),
	konohaTimer(0.0f),
	konohaRandTime(1.0f),
	bossCount(60.0f),
	bossTimer(0.0f),
	bossFlag(false),
	bossTurn(true),
	bossTurnCount(0.0f),
	bossTurnTime(0.0f)
{
	////プレイヤー変換
	//mPlayer = dynamic_cast<PlayerBody*>(world->findActor("Player").get());
	//mFloor = dynamic_cast<Player*>(world->findActor("Player").get());
	//ボスアニメーション生成
	if (bossFlag) {
		anim = SceneChangeBossAnm();
		childanim = SceneChangeBossAnm(4.0f);
		mChildFlag = rand() % 2;
	}

	bossPos = Vector2(SCREEN_SIZE.x + 128.0f, SCREEN_SIZE.y / 3);
	mBossChilds.push_back(Vector2(-64, -64));
	mBossChilds.push_back(Vector2(-90, 20));
	mBossChilds.push_back(Vector2(-256, 0));
	mBossChilds.push_back(Vector2(-89, 70));
	mBossChilds.push_back(Vector2(-189, -30));
	mBossChilds.push_back(Vector2(-200, 96));
	mBossChilds.push_back(Vector2(-230, -55));
}

BackGraundManager::BackGraundManager() :
	mWorld(nullptr)
{
}

BackGraundManager::~BackGraundManager()
{
}

void BackGraundManager::SetBackGraund(TextureID id1, TextureID id2, float heightY, bool frontGraund, bool stage2, const Vector2& scale)
{
	BackGraundState backState;
	backState.stage2 = stage2;
	//サイズを追加
	backState.size = ResourceLoader::GetInstance().GetTextureSize(id1)*scale;
	Vector2 size = backState.size;
	//手前に表示するか
	backState.frontGraundFlag = frontGraund;
	backState.scale = scale;
	//ポジションと番号を設定
	IndexPos indexPos;
	indexPos.index = ResourceLoader::GetInstance().getTextureID(id1);
	indexPos.position = Vector2(0, heightY*scale.y);
	backState.indexPos.push_back(indexPos);
	indexPos.index = ResourceLoader::GetInstance().getTextureID(id2);
	indexPos.position = Vector2(size.x, heightY*scale.y);
	backState.indexPos.push_back(indexPos);
	//入れる
	backStates.push_back(backState);
}

void BackGraundManager::SetTateBackGraund(TextureID id1, TextureID id2, const Vector2& scale)
{
	BackGraundState backState;
	//サイズを追加
	backState.size = ResourceLoader::GetInstance().GetTextureSize(id1);
	Vector2 size = backState.size;
	IndexPos indexPos;
	indexPos.index = ResourceLoader::GetInstance().getTextureID(id1);
	indexPos.position = Vector2::Zero;
	backState.indexPos.push_back(indexPos);
	indexPos.index = ResourceLoader::GetInstance().getTextureID(id2);
	indexPos.position = Vector2(0.0f, -size.y);
	backState.indexPos.push_back(indexPos);
	//入れる
	backStates.push_back(backState);
}

void BackGraundManager::SetTateYokoBackGraund(TextureID id)
{
	//サイズ取得
	tateYokoState.size = ResourceLoader::GetInstance().GetTextureSize(id);
	Vector2 size = tateYokoState.size;

	IndexPos indexPos;
	indexPos.index = ResourceLoader::GetInstance().getTextureID(id);

	indexPos.position = Vector2(size.x, 0.0f);
	tateYokoState.indexPos.push_back(indexPos);
	indexPos.position = Vector2(-size.x, 0.0f);
	tateYokoState.indexPos.push_back(indexPos);
	indexPos.position = Vector2(size.x, size.y);
	tateYokoState.indexPos.push_back(indexPos);
	indexPos.position = Vector2(-size.x, size.y);
	tateYokoState.indexPos.push_back(indexPos);

}

void BackGraundManager::SetUpBackGraund(TextureID id, int layer,bool flag)
{
	BackGraundState state;

	//サイズを追加
	state.size = ResourceLoader::GetInstance().GetTextureSize(id);
	Vector2 size = state.size;
	state.layer = layer;

	IndexPos indexPos;
	indexPos.index = ResourceLoader::GetInstance().getTextureID(id);
	state.frontGraundFlag = flag;
	indexPos.position = Vector2::Zero;
	state.indexPos.push_back(indexPos);
	indexPos.position = Vector2(0, -size.y);
	state.indexPos.push_back(indexPos);
	indexPos.position = Vector2(size.x, -size.y);
	state.indexPos.push_back(indexPos);
	indexPos.position = Vector2(size.x, 0);
	state.indexPos.push_back(indexPos);

	upBackStates.push_back(state);
}

void BackGraundManager::SetDownBackGraund(TextureID id)
{
	//idを追加
	downBackStates.id = ResourceLoader::GetInstance().getTextureID(id);
	//サイズを追加
	downBackStates.size = ResourceLoader::GetInstance().GetTextureSize(id);
	Vector2 size = downBackStates.size;

	IndexPos indexPos;
	indexPos.index = ResourceLoader::GetInstance().getTextureID(id);
	indexPos.position = Vector2(Vector2(0, size.y));
	downBackStates.indexPos.push_back(indexPos);
	indexPos.position = Vector2(Vector2(0, size.y * 2));
	downBackStates.indexPos.push_back(indexPos);
	indexPos.position = Vector2(Vector2(size.x, size.y));
	downBackStates.indexPos.push_back(indexPos);
	indexPos.position = Vector2(Vector2(size.x, size.y * 2));
	downBackStates.indexPos.push_back(indexPos);


}

void BackGraundManager::AllDeleteBackGraund()
{
	backStates.clear();
}

void BackGraundManager::Update(float deltatime, bool title)
{
	//木の葉アップデート
	if (!title)
		konohaUpdate();
	//要素内が何もなかったりプレイヤーがnullだったらリターン
	if (backStates.empty()) return;
	//背景階層の数を取得
	float layerNum = backStates.size();
	//地面系
	//コピーを使わないために＆を付けて参照する(for内でiの中身を弄る場合に使う)
	for (auto& i : backStates)
	{
		for (auto& j : i.indexPos)
		{
			if (!title) {
				//プレイヤーベクトル加算
				if (i.stage2)
					j.position -= Vector2((mWorld->GetInvVelo()*(1.0f / layerNum)).x, mWorld->GetInvVelo().y);
				else
					j.position -= mWorld->GetInvVelo()*1.0f / layerNum;
			}
			else {
				j.position -= Vector2(10.0f, 0.0f)*(1.0f / layerNum);
			}
			//地面テクスチャサイズ
			Vector2 size = i.size;
			//x軸のループ
			if (j.position.x <= -size.x)
				j.position.x = size.x + i.size.x + j.position.x;
			else if (j.position.x >= size.x)
				j.position.x = -size.x - i.size.x + j.position.x;
			//地上が見えているか
			if (j.position.y <= -size.y)
				stageFlag = false;
			else
				stageFlag = true;
		}
		layerNum--;
	}

	//空系

	for (auto & i : upBackStates) {
		//地面テクスチャサイズ
		Vector2 size = i.size;
		for (auto& j : i.indexPos) {
			//プレイヤー速度加算(タイトル用もある)
			if (!title)
				j.position -= mWorld->GetInvVelo()*(1.0f / i.layer);
			else
				j.position -= Vector2(10, 0)*(1.0f / layerNum);

			//x軸のループ
			if (j.position.x <= -size.x)
				j.position.x = size.x + size.x + j.position.x;
			else if (j.position.x >= size.x)
				j.position.x = -size.x - size.x + j.position.x;
			//Y軸のループ
			if (j.position.y <= -size.y)
				j.position.y = size.y + size.y + j.position.y;
			else if (j.position.y >= size.y)
				j.position.y = -size.y - size.y + j.position.y;
		}
	}

	//for (auto& i : upBackStates.indexPos)
	//{
	//	//プレイヤー速度加算(タイトル用もある)
	//	if (!title)
	//		i.position -= Vector2((mWorld->GetInvVelo()*(1.0f / upBackStates.layer)).x, mWorld->GetInvVelo().y);
	//	else
	//		i.position -= Vector2(10, 0)*(1.0f / layerNum);
	//	//x軸のループ
	//	if (i.position.x <= -size.x)
	//		i.position.x = size.x + size.x + i.position.x;
	//	else if (i.position.x >= size.x)
	//		i.position.x = -size.x - size.x + i.position.x;
	//	if (stageFlag)
	//	{
	//		//Y軸のループ
	//		if (i.position.y <= -size.y)
	//			i.position.y = size.y + size.y + i.position.y;
	//		else if (i.position.y >= size.y)
	//			i.position.y = -size.y - size.y + i.position.y;
	//	}
	//}
	//Vector2 size = downBackStates.size;
	////地下系
	//for (auto& i : downBackStates.indexPos)
	//{
	//	//プレイヤー速度加算
	//	if (!title)
	//		i.position += -mWorld->GetInvVelo();
	//	//x軸のループ
	//	if (i.position.x <= -size.x)
	//		i.position.x = size.x + size.x + i.position.x;
	//	else if (i.position.x >= size.x)
	//		i.position.x = -size.x - size.x + i.position.x;
	//	//Y軸のループ(地上が見えていなかった場合)
	//	if (!stageFlag)
	//	{
	//		if (i.position.y <= -size.y)
	//			i.position.y = size.y + size.y + i.position.y;
	//		else if (i.position.y >= size.y)
	//			i.position.y = -size.y - size.y + i.position.y;
	//	}
	//}
	if (!title&&bossFlag)
		BossUpdate();
}

void BackGraundManager::TateUpdate(float deltaTime)
{
	//要素内が何もなかったりプレイヤーがnullだったらリターン
	if (backStates.empty()) return;
	//背景階層の数を取得
	float layerNum = backStates.size();
	//地面系
	//コピーを使わないために＆を付けて参照する(for内でiの中身を弄る場合に使う)
	for (auto& i : backStates)
	{
		for (auto& j : i.indexPos)
		{
			//プレイヤーベクトル加算
			j.position += -mWorld->GetInvVelo()*(1.0f / layerNum);
			//地面テクスチャサイズ
			Vector2 size = i.size;
			//x軸のループ
			if (j.position.y <= -size.y)
				j.position.y = size.y + i.size.y + j.position.y;
			else if (j.position.y >= size.y)
				j.position.y = -size.y - i.size.y + j.position.y;
		}
		layerNum--;
	}
	////縦でのスクロールの場合横背景のx軸ループはない
	//for (auto& i : tateYokoState.indexPos)
	//{
	//	Vector2 size = tateYokoState.size;
	//	//プレイヤー速度加算
	//	i.position += -mWorld->GetInvVelo();
	//	//Y軸のループ(地上が見えていなかった場合)
	//	if (i.position.y <= -size.y)
	//		i.position.y = size.y + size.y + i.position.y;
	//	else if (i.position.y >= size.y)
	//		i.position.y = -size.y - size.y + i.position.y;
	//}
}

void BackGraundManager::Draw(bool title) const
{
	int wari = 1;
	if (title) wari = 2;

	//空の描写
	int count = 0;
	for (auto& i : upBackStates)
	{
		count++;
		if (!i.frontGraundFlag) {
			for (auto& j : i.indexPos)
			{
				DrawGraph(j.position.x, j.position.y, j.index, true);
				if (count == 1 && bossFlag) {
					if (mChildFlag == 1) {
						for (const auto& i : mBossChilds) {
							childanim.draw_e(bossPos + i, Vector2::Zero, 0.2f, 0.0f);
						}
						anim.drawTurn(bossPos, Vector2::Zero, 0.75f, 0.0f, Vector3(255, 255, 255), bossTurnFlag);
					}
					else
						anim.draw_e(bossPos, Vector2::Zero, 0.75f, 0.0f);
				}
			}
		}
	}
	//地上の描写
	for (auto& i : backStates)
	{
		for (auto& j : i.indexPos)
		{
			if (!i.frontGraundFlag)
				DrawRotaGraph(j.position.x + i.size.x / 2, j.position.y + i.size.y/wari, i.scale.x, 0, j.index, true);
		}
	}
	for (auto& i : upBackStates) {
		if (i.frontGraundFlag) {
			for (auto& j : i.indexPos) {
				DrawGraph(j.position.x, j.position.y, j.index, true);
			}
		}
	}

	////地下の描写
	//for (auto& i : downBackStates.indexPos)
	//{
	//	DrawGraph(i.position.x, i.position.y, i.index, true);
	//}
	//縦横のスクロール
	for (auto& i : tateYokoState.indexPos)
	{
		DrawGraph(i.position.x, i.position.y, i.index, true);
	}
}

void BackGraundManager::BackDraw() const
{
	//木の葉の描写
	for (auto& i : konohaStates) {
		DrawGraph(i.position.x + i.lerpPosition.x, i.position.y + i.lerpPosition.y, i.index, true);
	}

	//地上の描写
	for (auto& i : backStates)
	{
		for (auto& j : i.indexPos)
		{
			if (i.frontGraundFlag)
				DrawRotaGraph(j.position.x + i.size.x / 2, j.position.y + i.size.y, i.scale.x, 0, j.index, true);
		}
	}
}

void BackGraundManager::Spring(Vector2 & pos, Vector2 & resPos, Vector2 & velo, float stiffness, float friction, float mass)
{
	// バネの伸び具合を計算
	Vector2 stretch = (pos - resPos);
	// バネの力を計算
	Vector2 force = -stiffness * stretch;
	// 加速度を追加
	Vector2 acceleration = force / mass;
	// 移動速度を計算
	velo = friction * (velo + acceleration);

	pos = pos + velo;
}

void BackGraundManager::AddKonoha(const TextureID& id)
{
	konohaIds.push_back(id);
}

void BackGraundManager::BossFlag(bool flag)
{
	bossFlag = flag;
}

void BackGraundManager::konohaUpdate()
{
	//木の葉の挙動
	if (konohaIds.empty()) return;
	konohaTimer += Time::GetInstance().deltaTime();
	if (konohaTimer >= konohaRandTime) {
		//この葉の出現位置を設定
		int randNum = rand() % konohaIds.size();
		int randPosX = rand() % (int)(SCREEN_SIZE.x + 2048.0f);
		int randPosY = rand() % 256;
		konohaRandTime = rand() % 3;
		randPosX -= 1024.0f;
		randPosY -= 128.0f;
		//木の葉ステータスに代入
		KonohaState state;
		state.index = ResourceLoader::GetInstance().getTextureID(konohaIds[randNum]);
		state.position = Vector2(randPosX, randPosY);
		konohaStates.push_back(state);
		konohaTimer = 0.0f;
	}



	for (auto& i : konohaStates) {
		i.lerpTimer += 0.5f*Time::GetInstance().deltaTime();
		i.velo.y += 7.0f; Time::GetInstance().deltaTime();
		i.velo -= mWorld->GetInvVelo();
		float x = MathHelper::Lerp(-128, 128, MathHelper::Sin(MathHelper::Lerp(0, 180, i.lerpTimer)));
		float y = MathHelper::Lerp(-128, 64, MathHelper::Sin(MathHelper::Lerp(0, 180, i.lerpTimer)));
		i.lerpPosition = Vector2(x, y) + i.velo;
	}
}

void BackGraundManager::BossUpdate()
{
	bossTimer += Time::GetInstance().deltaTime();

	bossPos.x += 170.0f*Time::GetInstance().deltaTime();
	anim.Turn();
	anim.setIdle();
	childanim.Turn();
	childanim.setIdle();
	if (bossPos.x >= SCREEN_SIZE.x + 256.0f && (bossTimer >= bossCount)) {
		bossPos.x = -128.0f;
		mChildFlag = rand() % 3;
		bossTurnTime = GetRand(3) + 5;
		bossTimer = 0.0f;
		bossTurnCount = 0.0f;
	}

	bossTurnFlag = true;
	if (bossTimer >= bossTurnTime) {
		bossTurnCount += Time::GetInstance().deltaTime();
		bossTurnFlag = false;
		if (bossTurnCount >= 4.0f) {
			bossTurnFlag = true;
		}
	}

	bossPos += -mWorld->GetInvVelo() / 20.0f;

	anim.update_e(Time::GetInstance().deltaTime());
	childanim.update_e(Time::GetInstance().deltaTime()*3.0f);
}
