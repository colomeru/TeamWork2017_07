#include "Puyo_Texture_K.h"
#include "../Game/Time.h"
#include "../Math/MathHelper.h"
#include "../Input/InputMgr.h"
#include "../Math/Matrix.h"
#include "../Math/Vector3.h"
#include "../Actor/Base/ActorGroup.h"
#include "../Actor/Person/Player/PuyoCollision.h"
//一応正方形で考える
const int SplitSize = 32;

PuyoTextureK::PuyoTextureK(IWorld* world, TextureID tex, Vector2 pos, Vector2 scale, float rotate) :
	mWorld(world),
	textureIndex(ResourceLoader::GetInstance().getTextureID(tex)),
	textureSize(ResourceLoader::GetInstance().GetTextureSize(tex)),
	mPosition(pos - textureSize / 2),
	mScale(scale),
	mRotate(rotate),
	mCenter(Vector2::Zero),
	mAlpha(0.0f),
	mColor(Vector3::Zero),
	mColorVelo(Vector3::Zero),
	mResColor(Vector3::Zero),
	mIsHold(false)
{
	loopX = static_cast<int>(textureSize.x / SplitSize);
	loopY = static_cast<int>(textureSize.y / SplitSize);
	//動かない頂点をセット
	PuyoVertexSetInit();
	//グラフィックの設定
	PuyoGraphSplit();
	//グラフィックに頂点を設定
	PuyoGraphVertex();

	for (int y = 0; y <= loopY; y++)
	{
		for (int x = 0; x <= loopX; x++)
		{
			ActorPtr puyo = std::make_shared<PuyoCollision>(mWorld, commonVertexH[x][y].position, Vector2(x, y), mPosition);
			mWorld->addActor(ActorGroup::PuyoVertex, puyo);
			puyoCols.push_back(puyo);
		}
	}
	time = 0.0f;
}

PuyoTextureK::~PuyoTextureK()
{
	for (auto& i : puyoCols) {
		i->dead();
	}
}

void PuyoTextureK::SetPosition(Vector2 pos, Vector2 scale, float rotate, Vector2 center)
{
	mPosition = pos;
	mScale = scale;
	mRotate = rotate;
	mCenter = center;
}
void PuyoTextureK::PuyoPlayerPos(Vector2 pos1, Vector2 pos2,Vector3 color, bool isHold)
{
	mPlayerPos1 = pos1;
	mPlayerPos2 = pos2;
	mIsHold = isHold;
	mResColor = color;
}
void PuyoTextureK::PuyoUpdate()
{
	if (!mIsHold)mResColor = Vector3(255,255,255);
	//カラー変更
	Vector3::Spring(mColor, mResColor, mColorVelo, 0.2f, 0.5f, 2.0f);

	if(mAlpha<=1.0f)
	mAlpha += Time::GetInstance().deltaTime();
	for (auto& i : mWorld->findActors(ActorGroup::PuyoVertex)) {
		PuyoCollision* puyoCol = dynamic_cast<PuyoCollision*>(i.get());
		Vector2 hairetu = puyoCol->GetArrayState();
		if (/*(Vector2::Distance(mPlayerPos1, commonVertexHNoCol[(int)hairetu.x][(int)hairetu.y].position) <= 64.0f*2.5f ||
			Vector2::Distance(mPlayerPos2, commonVertexHNoCol[(int)hairetu.x][(int)hairetu.y].position) <= 64.0f*2.5f)&&
			mIsHold*/false) {
			commonVertexH[(int)hairetu.x][(int)hairetu.y].colWallVec = 0.0f;
		}
		else
			commonVertexH[(int)hairetu.x][(int)hairetu.y].colWallVec = puyoCol->GetVec();
		puyoCol->SetPos(commonVertexHNoCol[(int)hairetu.x][(int)hairetu.y].position, mPlayerPos1,mPlayerPos2);
	}

	//頂点のバネ系を毎フレーム更新
	for (int y = 0; y <= loopY; y++)
	{
		for (int x = 0; x <= loopX; x++)
		{
			Vector2 pos = Vector2(x*SplitSize, y*SplitSize);
			//回転とスケールのマトリクスを作成
			Matrix mat;
			mat =
				Matrix::CreateTranslation(Vector3(pos.x - textureSize.x / 2, pos.y - textureSize.y / 2, 0.0f))*
				Matrix::CreateRotationZ(mRotate)*
				Matrix::CreateTranslation(Vector3(textureSize.x / 2, textureSize.y / 2, 0.0f));

			//commonVertexH[x][y].colWallVec = Vector2::Normalize(commonVertexH[x][y].colWallVec);
			//Matrix veloMat = Matrix::CreateTranslation(Vector3(commonVertexH[x][y].velocity.x, commonVertexH[x][y].velocity.y, 0))*
			//	Matrix::CreateRotationZ(mRotate);

			//全頂点の時間を進める(振動が伝わる時間も配慮)
			//commonVertexH[x][y].vibrationTimer += 1.0f*Time::GetInstance().deltaTime();
			//正規化
			commonVertexH[x][y].velocity = commonVertexH[x][y].velocity.Normalize();
			//numをClamp
			//if(!commonVertexH[x][y].PowerFlag)
			//commonVertexH[x][y].num=MathHelper::Clamp(commonVertexH[x][y].num, -commonVertexH[x][y].maxPower, commonVertexH[x][y].maxPower);
			//if (commonVertexH[x][y].vibrationTimer >= commonVertexH[x][y].vibrationTime&&commonVertexH[x][y].power!=0) {
			//	commonVertexH[x][y].num += commonVertexH[x][y].power;
			//	commonVertexH[x][y].power = 0.0f;
			//}
			commonVertexH[x][y].time += 1000.0f*Time::GetInstance().deltaTime();
			MathHelper::Spring(commonVertexH[x][y].num, commonVertexH[x][y].resNum, commonVertexH[x][y].veloNum, 0.15f, 0.5f, 1.5f);

			commonVertexHNoCol[x][y].position = Vector2((MathHelper::Sin(commonVertexH[x][y].time)*commonVertexH[x][y].num)*
				commonVertexH[x][y].velocity.x + mat.Translation().x - textureSize.x / 2,

				(MathHelper::Sin(commonVertexH[x][y].time)*commonVertexH[x][y].num)*
				commonVertexH[x][y].velocity.y + mat.Translation().y - textureSize.y / 2) + mPosition;

			commonVertexH[x][y].springResPos =
				Vector2((MathHelper::Sin(commonVertexH[x][y].time)*commonVertexH[x][y].num)*
					commonVertexH[x][y].velocity.x + mat.Translation().x - textureSize.x / 2,

					(MathHelper::Sin(commonVertexH[x][y].time)*commonVertexH[x][y].num)*
					commonVertexH[x][y].velocity.y + mat.Translation().y - textureSize.y / 2) + mPosition + commonVertexH[x][y].colWallVec; /*+ (commonVertexH[x][y].colWallVec*commonVertexH[x][y].colWallPower)*/;

			Vector2::Spring(commonVertexH[x][y].position, commonVertexH[x][y].springResPos, commonVertexH[x][y].springVelocity, 0.4f);

			//commonVertexH[x][y].position.x =
			//	(MathHelper::Sin(commonVertexH[x][y].time)*commonVertexH[x][y].num)*
			//	commonVertexH[x][y].velocity.x + mat.Translation().x - textureSize.x / 2;
			//commonVertexH[x][y].position.y =
			//	(MathHelper::Sin(commonVertexH[x][y].time)*commonVertexH[x][y].num)*
			//	commonVertexH[x][y].velocity.y + mat.Translation().y - textureSize.y / 2;
		}
	}


	//頂点情報を毎フレーム更新
	PuyoVertexSet();
}

void PuyoTextureK::PuyoDraw()
{
	//色変更
	SetDrawBright(mColor.x, mColor.y, mColor.z);
	//描写
	for (int y = 0; y < loopY; y++)
	{
		for (int x = 0; x < loopX; x++)
		{
			VertexPos vp = spriteVertexH[x][y];
			float alpha = MathHelper::Lerp(0.0f, 255.0f, mAlpha);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawModiGraphF(
				vp.LeftTopPos.x, vp.LeftTopPos.y,
				vp.RightTopPos.x, vp.RightTopPos.y,
				vp.RightDownPos.x, vp.RightDownPos.y,
				vp.LeftDownPos.x, vp.LeftDownPos.y,
				spriteIndexsH[x][y], TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	//色を戻す
	SetDrawBright(255, 255, 255);
	//for (int y = 0; y <= loopY; y++)
	//{
	//	for (int x = 0; x <= loopX; x++)
	//	{
	//		DrawCircle(commonVertexH[x][y].position.x, commonVertexH[x][y].position.y, 3, GetColor(0, 0, 255));
	//	}
	//}

	//DrawCircle(mPlayerPos1.x, mPlayerPos1.y, 2, GetColor(255, 255, 0));
	//DrawCircle(mPlayerPos2.x, mPlayerPos2.y, 2, GetColor(255, 255, 0));

	//DrawFormatString(500,128, GetColor(255, 255, 255), "座標:%f,%f",test.x,test.y);


	//for (auto i : mWorld->findActors(ActorGroup::PuyoVertex)) {
	//	DrawCircle(i->getPosition().x,i->getPosition().y, 8, GetColor(0, 0, 255));
	//}
}

void PuyoTextureK::PuyoGraphSplit()
{
	//分割した画像情報を代入
	for (int y = 0; y < loopY; y++)
	{
		for (int x = 0; x < loopX; x++)
		{
			//右上の座標
			Vector2 splitLeftUp;
			splitLeftUp = Vector2(x*SplitSize, y*SplitSize);
			spriteIndexsH[x][y] = DerivationGraph(splitLeftUp.x, splitLeftUp.y, SplitSize, SplitSize, textureIndex);
		}
	}
}

void PuyoTextureK::PuyoGraphVertex()
{
	//画像に頂点を打つ
	for (int y = 0; y <= loopY; y++)
	{
		for (int x = 0; x <= loopX; x++)
		{
			commonVertexH[x][y].position = Vector2(x*SplitSize, y*SplitSize) + mPosition;
		}
	}
	PuyoVertexSet();
}

void PuyoTextureK::PuyoVertexSet()
{
	//頂点情報を分割画像にいれる
	for (int y = 0; y < loopY; y++)
	{
		for (int x = 0; x < loopX; x++)
		{
			VertexPos vertex;
			vertex.LeftTopPos = commonVertexH[x][y].position;
			vertex.LeftDownPos = commonVertexH[x][y + 1].position;
			vertex.RightTopPos = commonVertexH[x + 1][y].position;
			vertex.RightDownPos = commonVertexH[x + 1][y + 1].position;
			spriteVertexH[x][y] = vertex;
		}
	}
}
void PuyoTextureK::PuyoVertexSetInit()
{
	//画像に頂点を打つ
	for (int y = 0; y <= loopY; y++)
	{
		for (int x = 0; x <= loopX; x++)
		{
			commonVertexHNoMove[x][y].position = Vector2(x*SplitSize, y*SplitSize);
		}
	}
	//何も影響しない頂点を更新
	for (int y = 0; y <= loopY; y++)
	{
		for (int x = 0; x <= loopX; x++)
		{
			commonVertexHNoCol[x][y].position = Vector2(x*SplitSize, y*SplitSize) + (mPosition);
		}
	}
}
void PuyoTextureK::PuyoAddPower(Vector2 pos, Vector2 velo)
{
	//float Amplitude = 15.0f;
	////遠くに行くほどふり幅が小さい
	//float AmplitudeVib = 1.0f;
	//float count = 0;

	//Vector2 sevePos = Vector2(999, 999);
	//int x_ = 0;
	//int y_ = 0;
	////設定された一番近い配列の場所を調べる
	//for (int y = 0; y <= loopY; y++)
	//{
	//	for (int x = 0; x <= loopX; x++)
	//	{
	//		if ((commonVertexHNoMove[x][y].position - pos).Length() <= (sevePos - pos).Length())
	//		{
	//			sevePos = commonVertexHNoMove[x][y].position;
	//			x_ = x;
	//			y_ = y;
	//		}
	//	}
	//}

	//for (int y = 0; y <= 15; y++)
	//{
	//	AmplitudeVib = y + 1;
	//	count = 0;
	//	for (int x = 0; x <= 15; x++)
	//	{
	//		if (count >= AmplitudeVib)
	//			AmplitudeVib++;
	//		count++;

	//		if ((x + x_) <= 16 && (y + y_) <= 16 &&
	//			(x + x_) >= 0 && (y + y_) >= 0)
	//		{
	//			commonVertexH[x + x_][y + y_].vibrationTimer = 0.0f;
	//			commonVertexH[x + x_][y + y_].vibrationTime = AmplitudeVib / 100.0f;
	//			commonVertexH[x + x_][y + y_].velocity += velo;
	//			commonVertexH[x + x_][y + y_].resNum = 0.0f;
	//			//commonVertexH[x + x_][y + y_].num += 20.0f / AmplitudeVib;
	//			commonVertexH[x + x_][y + y_].power = 100.0f / AmplitudeVib;
	//			commonVertexH[x + x_][y + y_].PowerFlag = true;
	//		}
	//		if ((x + x_) <= 16 && (y_ - y) <= 16 &&
	//			(x + x_) >= 0 && (y_ - y) >= 0)
	//		{
	//			commonVertexH[x + x_][y_ - y].vibrationTimer = 0.0f;
	//			commonVertexH[x + x_][y_ - y].vibrationTime = AmplitudeVib / 100.0f;
	//			commonVertexH[x + x_][y_ - y].velocity += velo;
	//			commonVertexH[x + x_][y_ - y].resNum = 0.0f;
	//			//commonVertexH[x + x_][y_ - y].num += 20.0f / AmplitudeVib;
	//			commonVertexH[x + x_][y_ - y].power = 100.0f / AmplitudeVib;
	//			commonVertexH[x + x_][y_ - y].PowerFlag = true;
	//		}
	//		if ((x_ - x) <= 16 && (y_ - y) <= 16 &&
	//			(x_ - x) >= 0 && (y_ - y) >= 0)
	//		{
	//			commonVertexH[x_ - x][y_ - y].vibrationTimer = 0.0f;
	//			commonVertexH[x_ - x][y_ - y].vibrationTime = AmplitudeVib / 100.0f;
	//			commonVertexH[x_ - x][y_ - y].velocity += velo;
	//			commonVertexH[x_ - x][y_ - y].resNum = 0.0f;
	//			//commonVertexH[x_ - x][y_ - y].num += 20.0f / AmplitudeVib;
	//			commonVertexH[x_ - x][y_ - y].power = 100.0f / AmplitudeVib;
	//			commonVertexH[x_ - x][y_ - y].PowerFlag = true;
	//		}
	//		if ((x_ - x) <= 16 && (y_ + y) <= 16 &&
	//			(x_ - x) >= 0 && (y_ + y) >= 0)
	//		{
	//			//commonVertexH[x_ - x][y_ + y].amplitude = Amplitude / AmplitudeVib;
	//			commonVertexH[x_ - x][y_ + y].vibrationTimer = 0.0f;
	//			commonVertexH[x_ - x][y_ + y].vibrationTime = AmplitudeVib / 100.0f;
	//			commonVertexH[x_ - x][y_ + y].velocity += velo;
	//			commonVertexH[x_ - x][y_ + y].resNum = 0.0f;
	//			//commonVertexH[x_ - x][y_ + y].num += 20.0f/AmplitudeVib;
	//			commonVertexH[x_ - x][y_ + y].power = 100.0f / AmplitudeVib;
	//			commonVertexH[x_ - x][y_ + y].PowerFlag = true;
	//		}
	//	}
	//}
}






void PuyoTextureK::PuyoAddPowerDx(Vector2 pos, Vector2 velo)
{
	//float Amplitude = 15.0f;
	////遠くに行くほどふり幅が小さい
	//float AmplitudeVib = 1.0f;
	//float count = 0;

	//Vector2 sevePos = Vector2(999, 999);
	//int x_ = 0;
	//int y_ = 0;
	////設定された一番近い配列の場所を調べる
	//for (int y = 0; y <= loopY; y++)
	//{
	//	for (int x = 0; x <= loopX; x++)
	//	{
	//		if ((commonVertexHNoMove[x][y].position - pos).Length() <= (sevePos - pos).Length())
	//		{
	//			sevePos = commonVertexHNoMove[x][y].position;
	//			x_ = x;
	//			y_ = y;
	//		}
	//	}
	//}

	//for (int y = 0; y < loopY / 2; y++)
	//{
	//	AmplitudeVib = y + 1;
	//	count = 0;
	//	for (int x = 0; x < loopX / 2; x++)
	//	{
	//		if (count >= AmplitudeVib)
	//			AmplitudeVib++;
	//		count++;

	//		if ((x + x_) <= 17 && (y + y_) <= 17 &&
	//			(x + x_) >= 0 && (y + y_) >= 0)
	//		{
	//			commonVertexH[x + x_][y + y_].vibrationTimer = 0.0f;
	//			//commonVertexH[x + x_][y + y_].vibrationTime += AmplitudeVib / 1000.0f;
	//			commonVertexH[x + x_][y + y_].time = 0.0f;
	//			commonVertexH[x + x_][y + y_].velocity += velo;
	//			commonVertexH[x + x_][y + y_].resNum = 0.0f;
	//			commonVertexH[x + x_][y + y_].maxPower = (velo.Length() * 40);
	//			commonVertexH[x + x_][y + y_].num += velo.Length();
	//			commonVertexH[x + x_][y + y_].PowerFlag = false;
	//			commonVertexH[x + x_][y + y_].power = 0.0f;
	//		}
	//		if ((x + x_) <= 17 && (y_ - y) <= 17 &&
	//			(x + x_) >= 0 && (y_ - y) >= 0)
	//		{
	//			commonVertexH[x + x_][y_ - y].vibrationTimer = 0.0f;
	//			//commonVertexH[x + x_][y_ - y].vibrationTime += AmplitudeVib / 1000.0f;
	//			commonVertexH[x + x_][y_ - y].time = 0.0f;
	//			commonVertexH[x + x_][y_ - y].velocity += velo;
	//			commonVertexH[x + x_][y_ - y].resNum = 0.0f;
	//			commonVertexH[x + x_][y_ - y].maxPower = (velo.Length() * 40);
	//			commonVertexH[x + x_][y_ - y].num += velo.Length();
	//			commonVertexH[x + x_][y_ - y].PowerFlag = false;
	//			commonVertexH[x + x_][y_ - y].power = 0.0f;
	//		}
	//		if ((x_ - x) <= 17 && (y_ - y) <= 17 &&
	//			(x_ - x) >= 0 && (y_ - y) >= 0)
	//		{
	//			commonVertexH[x_ - x][y_ - y].vibrationTimer = 0.0f;
	//			//commonVertexH[x_ - x][y_ - y].vibrationTime += AmplitudeVib / 1000.0f;
	//			commonVertexH[x_ - x][y_ - y].time = 0.0f;
	//			commonVertexH[x_ - x][y_ - y].velocity += velo;
	//			commonVertexH[x_ - x][y_ - y].resNum = 0.0f;
	//			commonVertexH[x_ - x][y_ - y].maxPower = (velo.Length()*40.0f);
	//			commonVertexH[x_ - x][y_ - y].num += velo.Length();
	//			commonVertexH[x_ - x][y_ - y].PowerFlag = false;
	//			commonVertexH[x_ - x][y_ - y].power = 0.0f;
	//		}
	//		if ((x_ - x) <= 16 && (y_ + y) <= 16 &&
	//			(x_ - x) >= 0 && (y_ + y) >= 0)
	//		{
	//			commonVertexH[x_ - x][y_ + y].vibrationTimer = 0.0f;
	//			//commonVertexH[x_ - x][y_ + y].vibrationTime += AmplitudeVib / 1000.0f;
	//			commonVertexH[x_ - x][y_ + y].time = 0.0f;
	//			commonVertexH[x_ - x][y_ + y].velocity += velo;
	//			commonVertexH[x_ - x][y_ + y].resNum = 0.0f;
	//			commonVertexH[x_ - x][y_ + y].maxPower = (velo.Length()*40.0f);
	//			commonVertexH[x_ - x][y_ + y].num += velo.Length();
	//			commonVertexH[x_ - x][y_ + y].PowerFlag = false;
	//			commonVertexH[x_ - x][y_ + y].power = 0.0f;
	//		}
	//	}
	//}
}

void PuyoTextureK::PuyoAddPowerEx(Vector2 vec, Vector2 velo, float power, float eikyo)
{
	int x_ = 0;
	int y_ = 0;
	Vector2 kakeru = vec*100.0f;
	{
		Vector2 texCen = Vector2(textureSize.x / 2, textureSize.y / 2);
		Vector2 vecc = kakeru + texCen;
		Vector2 sevePos = Vector2(9999, 9999);

		//設定された一番近い配列の場所を調べる
		for (int y = 0; y <= loopY; y++)
			for (int x = 0; x <= loopX; x++)
			{
				if (Vector2::Distance(commonVertexHNoMove[x][y].position, vecc) <= Vector2::Distance(sevePos, vecc))
				{
					sevePos = commonVertexHNoMove[x][y].position;
					x_ = x;
					y_ = y;
				}
			}
	}
	for (int y = -3; y <= 3; y++)
	{
		for (int x = -3; x <= 3; x++)
		{
			if (x_ + x <= loopX&& y_ + y <= loopY&&y_ + y >= 0 && x_ + x >= 0)
				PuyoAddPowerDxSub(x_ + x, y_ + y, velo, power, eikyo);
		}
	}
	//DrawCircle(commonVertexH[x_][y_].position.x + mPosition.x, commonVertexH[x_][y_].position.y + mPosition.y, 5, GetColor(255, 0, 0));
	//PuyoAddPowerDxSub(x_, y_, velo, power);
	//DrawFormatString(500, 128, GetColor(255, 255, 255), "velo:%f,%f", velo.x, velo.y);
}
void PuyoTextureK::PuyoTimerZero()
{
	for (int y = 0; y <= loopY; y++)
	{
		for (int x = 0; x <= loopX; x++)
		{
			commonVertexH[x][y].time = 0.0f;
		}
	}
}
//仕様変更版
void PuyoTextureK::PuyoAddPowerDxSub(int x_, int y_, Vector2 velo, float power, float eikyo)
{
	Vector2 pos = commonVertexHNoMove[x_][y_].position;
	//if (InputMgr::GetInstance().IsKeyOn(KeyCode::N))
	//{
	//	testRes -= 50.0f*Time::GetInstance().deltaTime();
	//}
	for (int y = 0; y <= loopY; y++)
	{
		for (int x = 0; x <= loopX; x++)
		{
			float dis = Vector2::Distance(pos, commonVertexHNoMove[x][y].position);
			commonVertexH[x][y].time = 0.0f;
			if (dis <= eikyo)
			{
				dis++;
				commonVertexH[x][y].resNum = 0.0f;
				commonVertexH[x][y].num += (power)*Time::GetInstance().deltaTime();
				commonVertexH[x][y].velocity += velo;
			}
		}
	}

}