#ifndef MapGenerator
#define Mapgenerator
#include"../FileReader/CsvReader.h"
#include"../Actor/Base/Actor.h"
#include"../World/IWorld.h"
#include"../Actor/Base/ActorGroup.h"
#include"MapChip.h"
#include"../World/IWorldPtr.h"
#include<map>
#include"MoveFloorUpDown.h"
#include"MoveFloorCenterUpDown.h"
#include"MoveFloorRightLeft.h"
#include"MoveFloorCenterRightLeft.h"
#include"TurnFloor.h"
#include"MovelessFloor.h"
#include"MovelessFloorInvis.h"
#include"NavChip.h"
#include"TranslessTurnFloor.h"
#include"StageClearPoint.h"
#include"GameOverPoint.h"
#include"CollidelessFloor.h"
#include"SegmentFloor.h"
#include"SticklessFloor.h"
#include"BossAreaFloor.h"
#include"../Actor/Person/Enemy/ImportEnemys.h"
#include"../Actor/Person/Enemy/Bosses/MiniBoss/MediumBoss/ImportMediumBosses.h"
#include"../ResourceLoader/ResourceLoader.h"
#include "../Actor/Door/Door.h"
#include "MovelessFloorBreak.h"
#include "../Actor/Tubo/Tubo.h"
#include"../Actor/Item/Items.h"
#include"../Actor/Item/BadItems.h"
#include"../Field/ItemSpawnFloor.h"


class MapGenerator {
private:
	enum pointSetState {
		FF,//どちらもそのまま
		TF,//始点のみ補正
		FT,//終点のみ補正
		TT//どちらも補正
	};
public:
	//マップを生成するためのクラス　world:マップを追加する対象(IWorld*)
	MapGenerator(IWorld* world) :reader_() {
		world_ = world;

		//world_->addActor(ActorGroup::Field, std::make_shared<MapChip>(world_, Vector2(rowN*CHIPSIZE, colN*CHIPSIZE)));

		stagetexes[0] = TextureID::FLOOR_STAGE4_TEX;
		stagetexes[1] = TextureID::FLOOR_STAGE1_TEX;
		stagetexes[2] = TextureID::FLOOR_STAGE2_TEX;
		stagetexes[3] = TextureID::FLOOR_STAGE3_TEX;
		stagetexes[4] = TextureID::FLOOR_STAGE4_TEX;

		segtexes[0] = TextureID::SEGMENT_TRI_TEX;
		segtexes[1] = TextureID::SEGMENT_TRI_TEX;
		segtexes[3] = TextureID::SEGMENT_TRI_TEX;


		segtexes[2] = TextureID::SEGMENT_TRI_TEX;
		segtexes[4] = TextureID::SEGMENT_TRI2_TEX;


		for (int i = 0; i < 20; i++) {
			segmentChecker[100 + i] = false;
		}

	}
	Vector2 findStartPoint(std::string fileName) {
		reader_.load(fileName);
		for (int rowN = 0; rowN < reader_.rows(); rowN++)
		{
			for (int colN = 0; colN < reader_.columns(rowN); colN++)
			{
				if (reader_.geti(rowN, colN) == 999) {
					return Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
				}
			}
		}
		return START_POS;
	}
	Vector2 GetCellSize() {
		return Vector2(static_cast<float>(colSize), static_cast<float>(rowSize));
	}

	//縦のセル数を返す
	int GetRowSize() {
		return rowSize;
	}
	//横のセル数を返す
	int GetColumnSize() {
		return colSize;
	}
	//ファイル名(拡張子まで書く事)から、マップを生成する fileName:マップ用の.csv 0:生成しない 1:MapChipを生成
	void create(std::string fileName, int colDef = 0, int rowDef = 0, int stagenum = 0)
	{
		reader_.load(fileName);

		rowSize = reader_.rows();
		colSize = reader_.columns();
		std::vector<std::vector<bool>> baseBlockConnectChecker;
		std::vector<std::vector<bool>> sticklessBlockConnectChecker;
		std::vector<std::vector<bool>> bossBlockConnectChecker;
		std::vector<std::vector<bool>> fWoodBlockConnectChecker;
		std::vector<std::vector<bool>> fS4BlockConnectChecker;
		std::vector<std::vector<bool>> fSubBlockConnectChecker;

		baseBlockConnectChecker.resize(rowSize);
		sticklessBlockConnectChecker.resize(rowSize);
		bossBlockConnectChecker.resize(rowSize);
		fWoodBlockConnectChecker.resize(rowSize);
		fS4BlockConnectChecker.resize(rowSize);
		fSubBlockConnectChecker.resize(rowSize);

		for (int i = 0; i < (int)baseBlockConnectChecker.size();i++) {
			baseBlockConnectChecker[i].resize(colSize);
			sticklessBlockConnectChecker[i].resize(colSize);
			bossBlockConnectChecker[i].resize(colSize);
			fWoodBlockConnectChecker[i].resize(colSize);
			fS4BlockConnectChecker[i].resize(colSize);
			fSubBlockConnectChecker[i].resize(colSize);
		}
		
		//OutputDebugString(std::to_string(baseBlockConnectChecker[0].size()).c_str());

		int rowCountDef = rowDef - 1;
		int colCountDef = colDef - 1;

		if (colDef == 0 && rowDef == 0) rowCountDef = colCountDef = 0;

		for (int rowN = rowDef; rowN < (reader_.rows() - (rowCountDef)); rowN++)
		{
			for (int colN = colDef; colN < (reader_.columns(rowN) - (colCountDef)); colN++)
			{

				typeid(chips[(reader_.geti(rowN, colN))]).raw_name();

				if (reader_.geti(rowN, colN) == 0) continue;

				if (reader_.geti(rowN, colN) == 1) {//テンプレートを使って複数種類生成出来るようにする
													//world_->addActor(ActorGroup::Field,std::make_shared<MovelessFloor>(chips[(reader_.geti(rowN, colN))], world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					//world_->addActor(ActorGroup::Field, std::make_shared<MovelessFloor>(
					//	ResourceLoader::GetInstance().getTextureID(stagetexes[stagenum]), world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					
					baseBlockConnectChecker[rowN][colN] = true;

					continue;
				}
				if (reader_.geti(rowN, colN) == 2) {
					world_->addActor(ActorGroup::Field, std::make_shared<MoveFloorUpDown>(
						ResourceLoader::GetInstance().getTextureID(stagetexes[stagenum]), world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 3) {
					world_->addActor(ActorGroup::Field, std::make_shared<MoveFloorRightLeft>(
						ResourceLoader::GetInstance().getTextureID(stagetexes[stagenum]), world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 4) {
					world_->addActor(ActorGroup::Field, std::make_shared<TurnFloor>(
						ResourceLoader::GetInstance().getTextureID(stagetexes[stagenum]), world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 6) {
					world_->addActor(ActorGroup::Field, std::make_shared<TranslessTurnFloor>(
						ResourceLoader::GetInstance().getTextureID(stagetexes[stagenum]), world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 7) {
					world_->addActor(ActorGroup::Field, std::make_shared<StageClearPoint>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 8) {
					world_->addActor(ActorGroup::Field, std::make_shared<GameOverPoint>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 9) {
					//world_->addActor(ActorGroup::Field, std::make_shared<SticklessFloor>(
					//	ResourceLoader::GetInstance().getTextureID(TextureID::FLOOR_STONE_TEX), world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
						
					sticklessBlockConnectChecker[rowN][colN] = true;

						continue;
				}
				if (reader_.geti(rowN, colN) == 10) {
					//world_->addActor(ActorGroup::Field, std::make_shared<BossAreaFloor>(
					//	ResourceLoader::GetInstance().getTextureID(stagetexes[stagenum]), world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					bossBlockConnectChecker[rowN][colN] = true;
					continue;
				}
				if (reader_.geti(rowN, colN) == 11) {
					world_->addActor(ActorGroup::Field, std::make_shared<MoveFloorCenterUpDown>(
						ResourceLoader::GetInstance().getTextureID(stagetexes[stagenum]), world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 12) {
					world_->addActor(ActorGroup::Field, std::make_shared<MoveFloorCenterRightLeft>(
						ResourceLoader::GetInstance().getTextureID(stagetexes[stagenum]), world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 13) {
					world_->addActor(ActorGroup::Field, std::make_shared<MovelessFloor>(
						ResourceLoader::GetInstance().getTextureID(stagetexes[stagenum]), world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 14) {
					world_->addActor(ActorGroup::Field, std::make_shared<MovelessFloorInvis>(
						ResourceLoader::GetInstance().getTextureID(stagetexes[stagenum]), world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					continue;
				}

				if (reader_.geti(rowN, colN) == 15) {
					world_->addActor(ActorGroup::Item, std::make_shared<BadItems>(
						world_, Vector2(colN*CHIPSIZE + 48, rowN*CHIPSIZE + 48)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 16) {
					world_->addActor(ActorGroup::Field, std::make_shared<ItemSpawnFloor>(
						ResourceLoader::GetInstance().getTextureID(stagetexes[stagenum]),world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					itemCount++;
					continue;
				}
				if (reader_.geti(rowN, colN) == 17) {
					world_->addActor(ActorGroup::Item, std::make_shared<Items>(
						world_, Vector2(colN*CHIPSIZE + 48, rowN*CHIPSIZE + 48)));
					itemCount++;
					continue;
				}
				if (reader_.geti(rowN, colN) == 18) {
					world_->addActor(ActorGroup::Tubo, std::make_shared<Tubo>(
						world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE - 1.0f)));
					continue;
				}

				if (reader_.geti(rowN, colN) == 19) {
					world_->addActor(ActorGroup::Field, std::make_shared<MovelessFloorBreak>(
						ResourceLoader::GetInstance().getTextureID(TextureID::CHIP1_TEX), world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 20) {
					world_->addActor(ActorGroup::Field, std::make_shared<MovelessFloor>(
						ResourceLoader::GetInstance().getTextureID(TextureID::FLOOR_WOOD_TEX), world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 21) {
					//world_->addActor(ActorGroup::Field,std::make_shared<MovelessFloor>(chips[(reader_.geti(rowN, colN))], world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					world_->addActor(ActorGroup::Field, std::make_shared<MovelessFloor>(
						ResourceLoader::GetInstance().getTextureID(TextureID::FLOOR_STAGE4_TEX), world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 22) {
					//world_->addActor(ActorGroup::Field,std::make_shared<MovelessFloor>(chips[(reader_.geti(rowN, colN))], world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					//world_->addActor(ActorGroup::Field, std::make_shared<MovelessFloor>(
					//	ResourceLoader::GetInstance().getTextureID(TextureID::FLOOR_SUB_TEX), world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					fSubBlockConnectChecker[rowN][colN] = true;
					continue;
				}
				if (reader_.geti(rowN, colN) == 30) {
					world_->addActor(ActorGroup::Field, std::make_shared<CollidelessFloor>(ResourceLoader::GetInstance().getTextureID(TextureID::CHIP2_TEX), world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 50) {
					// クリボーエネミー
					world_->addActor(ActorGroup::Enemy, std::make_shared<FloorTurnEnemy>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2), -1.0f));
					itemCount++;
					continue;
				}
				if (reader_.geti(rowN, colN) == 51) {
					// クリボーエネミー
					world_->addActor(ActorGroup::Enemy, std::make_shared<FloorTurnEnemy>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2)));
					itemCount++;
					continue;
				}
				if (reader_.geti(rowN, colN) == 52) {
					// ゴルドエネミー
					world_->addActor(ActorGroup::Enemy, std::make_shared<WallTrunEnemy>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 53) {
					// ゴルドエネミー
					world_->addActor(ActorGroup::Enemy, std::make_shared<WallTrunEnemy>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2), Vector2::Left));
					continue;
				}
				if (reader_.geti(rowN, colN) == 54) {
					// ハネクリボーエネミー
					world_->addActor(ActorGroup::Enemy, std::make_shared<FlyingEnemy>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2), -1.0f));
					itemCount++;
					continue;
				}
				if (reader_.geti(rowN, colN) == 55) {
					// ハネクリボーエネミー
					world_->addActor(ActorGroup::Enemy, std::make_shared<FlyingEnemy>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2)));
					itemCount++;
					continue;
				}
				if (reader_.geti(rowN, colN) == 56) {
					// 壁移動エネミー
					world_->addActor(ActorGroup::Enemy, std::make_shared<WallMoveEnemy>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 57) {
					// 壁移動エネミー
					world_->addActor(ActorGroup::Enemy, std::make_shared<WallMoveEnemy>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2), Vector2(1.f, 1.f)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 58) {
					world_->addActor(ActorGroup::Enemy, std::make_shared<NeedleEnemy>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2), 0.0f));
					continue;
				}
				if (reader_.geti(rowN, colN) == 59) {
					world_->addActor(ActorGroup::Enemy, std::make_shared<NeedleEnemy>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2), 180.0f));
					continue;
				}
				if (reader_.geti(rowN, colN) == 60) {
					world_->addActor(ActorGroup::Enemy, std::make_shared<NeedleEnemy>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2), 90.0f));
					continue;
				}
				if (reader_.geti(rowN, colN) == 61) {
					world_->addActor(ActorGroup::Enemy, std::make_shared<NeedleEnemy>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2), 270.0f));
					continue;
				}
				if (reader_.geti(rowN, colN) == 62) {
					// 中ボス１
					world_->addActor(ActorGroup::Enemy, std::make_shared<Stage1MiniBoss>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 63) {
					// 中ボス２
					world_->addActor(ActorGroup::Enemy, std::make_shared<Stage2MiniBoss>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 64) {
					// 中ボス３
					world_->addActor(ActorGroup::Enemy, std::make_shared<Stage3MiniBoss>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 65) {
					world_->addActor(ActorGroup::Enemy, std::make_shared<BaseBoss>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 66) {
					// 壁移動エネミー
					world_->addActor(ActorGroup::Enemy, std::make_shared<WallMoveEnemy>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2) + Vector2::One * (CHIPSIZE - 64.0f) / 2));
					continue;
				}
				if (reader_.geti(rowN, colN) == 67) {
					// 壁移動エネミー
					world_->addActor(ActorGroup::Enemy, std::make_shared<WallMoveEnemy>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2) + Vector2::One * (CHIPSIZE - 64.0f) / 2, Vector2(1.f, 1.f)));
					continue;
				}
				if (reader_.geti(rowN, colN) == 68) {
					// スクロールポイント
					world_->addActor(ActorGroup::Effect, std::make_shared<BossScrollPoint>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2) + Vector2::One * (CHIPSIZE - 64.0f) / 2));
					continue;
				}
				if (reader_.geti(rowN, colN) == 70) {
					world_->addActor(ActorGroup::Field, std::make_shared<Door>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					continue;
				}
				/*if (reader_.geti(rowN, colN) == 67) {
				world_->addActor(ActorGroup::Enemy, std::make_shared<BossHeart>(world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE) + (Vector2::One*CHIPSIZE / 2)));
				}*/
				if (reader_.geti(rowN, colN) == 100) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 101) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 102) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 103) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 104) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 105) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 106) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 107) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 108) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 109) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 110) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, FF);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					//if (segmentChecker[reader_.geti(rowN, colN)]) {
					//	segmentChecker[reader_.geti(rowN, colN)] = false;
					//	world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(world_, segmentStartPoints[reader_.geti(rowN, colN)], Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
					//}
					//else {
					//	segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
					//	segmentChecker[reader_.geti(rowN, colN)] = true;
					//}
					continue;
				}
				if (reader_.geti(rowN, colN) == 111) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, FF);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 112) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, FF);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 113) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, FF);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 114) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, FF);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 115) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, FF);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 116) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, FF);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 117) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, FF);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 118) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, FF);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 119) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, FF);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 120) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 121) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 122) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 123) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 124) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 125) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 126) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 127) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 128) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
				if (reader_.geti(rowN, colN) == 129) {
					if (segmentChecker[reader_.geti(rowN, colN)]) {
						segmentChecker[reader_.geti(rowN, colN)] = false;
						Vector2 startPoint, endPoint;
						SetSegmentPoint(rowN, colN, startPoint, endPoint, TT);
						world_->addActor(ActorGroup::Field, std::make_shared<SegmentFloor>(ResourceLoader::GetInstance().getTextureID(segtexes[stagenum]), world_, startPoint, endPoint));
					}
					else {
						segmentStartPoints[reader_.geti(rowN, colN)] = Vector2(colN*CHIPSIZE, rowN*CHIPSIZE);
						segmentChecker[reader_.geti(rowN, colN)] = true;
					}
					continue;
				}
			}
		}
		
		CreateFloor(baseBlockConnectChecker, stagenum, 1);
		CreateFloor(sticklessBlockConnectChecker, stagenum, 9);
		CreateFloor(bossBlockConnectChecker, stagenum, 10);
		CreateFloor(fWoodBlockConnectChecker, stagenum, 20);
		CreateFloor(fS4BlockConnectChecker, stagenum, 21);
		CreateFloor(fSubBlockConnectChecker, stagenum, 22);
	}

	int getItemCount() {
		return itemCount;
	}

private:
	void CreateFloor(std::vector<std::vector<bool>> baseBlockConnectChecker,int stagenum,int blockType) {
		int blockCount;
		bool isFirst;
		Vector2 startPosition;

		std::vector<std::vector<bool>> posit;

		
		for (int i = 0; i < (int)baseBlockConnectChecker.size(); i++) {

			blockCount = 0;
			isFirst = true;

			for (int x = 0; x < (int)baseBlockConnectChecker[i].size(); x++) {
				if (baseBlockConnectChecker[i][x]) {

					baseBlockConnectChecker[i][x] = false;
					blockCount++;
					if (isFirst)startPosition = Vector2(CHIPSIZE*x, CHIPSIZE*i);

					isFirst = false;
				}
				else if (blockCount>0) {
					if (blockCount == 1) {
						blockCount = 0;
						isFirst = true;
						baseBlockConnectChecker[(int)(startPosition.y / CHIPSIZE)][(int)(startPosition.x / CHIPSIZE)] = true;
						continue;
					}
					//world_->addActor(ActorGroup::Field, std::make_shared<MovelessFloor>(
						//ResourceLoader::GetInstance().getTextureID(stagetexes[stagenum]), world_, startPosition, blockCount, 1));
					CreateChip(blockType, stagenum, startPosition, blockCount,1);
					blockCount = 0;
					isFirst = true;
				}
				if (x == baseBlockConnectChecker[0].size() - 1 && blockCount != 0)
				{
					if (blockCount == 1) {
						blockCount = 0;
						isFirst = true;
						baseBlockConnectChecker[(int)(startPosition.y / CHIPSIZE)][(int)(startPosition.x / CHIPSIZE)] = true;
						continue;
					}

					//world_->addActor(ActorGroup::Field, std::make_shared<MovelessFloor>(
					//	ResourceLoader::GetInstance().getTextureID(stagetexes[stagenum]), world_, startPosition, blockCount, 1));
					CreateChip(blockType, stagenum, startPosition, blockCount, 1);

					blockCount = 0;
					isFirst = true;

				}
			}
		}
		for (int i = 0; i < (int)baseBlockConnectChecker[0].size(); i++) {

			blockCount = 0;
			isFirst = true;

			for (int x = 0; x < (int)baseBlockConnectChecker.size(); x++) {
				if (baseBlockConnectChecker[x][i]) {
					baseBlockConnectChecker[x][i] = false;
					blockCount++;
					if (isFirst)startPosition = Vector2(CHIPSIZE*i, CHIPSIZE*x);

					isFirst = false;
				}
				else if (blockCount>0) {

					//world_->addActor(ActorGroup::Field, std::make_shared<MovelessFloor>(
					//	ResourceLoader::GetInstance().getTextureID(stagetexes[stagenum]), world_, startPosition, 1, blockCount));
					CreateChip(blockType, stagenum, startPosition, 1,blockCount);

					blockCount = 0;
					isFirst = true;
				}
				if (x == baseBlockConnectChecker.size() - 1 && blockCount != 0)
				{

					//world_->addActor(ActorGroup::Field, std::make_shared<MovelessFloor>(
					//	ResourceLoader::GetInstance().getTextureID(stagetexes[stagenum]), world_, startPosition, 1, blockCount));
					CreateChip(blockType, stagenum, startPosition, 1, blockCount);

					blockCount = 0;
					isFirst = true;

				}
			}
		}

	}

	void CreateChip(int num,int stagenum,Vector2 position, int width=1,int height=1, Vector2 endPosition=Vector2::Zero) {
		if (num == 1) {
			world_->addActor(ActorGroup::Field, std::make_shared<MovelessFloor>(
				ResourceLoader::GetInstance().getTextureID(stagetexes[stagenum]), world_, position, width, height));
			return;
		}
		if (num == 9) {
			world_->addActor(ActorGroup::Field, std::make_shared<SticklessFloor>(
				ResourceLoader::GetInstance().getTextureID(TextureID::FLOOR_STONE_TEX), world_, position, width, height));
		}
		if (num == 10) {
			world_->addActor(ActorGroup::Field, std::make_shared<BossAreaFloor>(
				ResourceLoader::GetInstance().getTextureID(stagetexes[stagenum]), world_,position,width,height));
		}
		if (num == 20) {
			world_->addActor(ActorGroup::Field, std::make_shared<MovelessFloor>(
				ResourceLoader::GetInstance().getTextureID(TextureID::FLOOR_WOOD_TEX), world_, position, width, height));
			return;
		}
		if (num == 21) {
			//world_->addActor(ActorGroup::Field,std::make_shared<MovelessFloor>(chips[(reader_.geti(rowN, colN))], world_, Vector2(colN*CHIPSIZE, rowN*CHIPSIZE)));
			world_->addActor(ActorGroup::Field, std::make_shared<MovelessFloor>(
				ResourceLoader::GetInstance().getTextureID(TextureID::FLOOR_STAGE4_TEX), world_, position, width, height));
			return;
		}

		if (num == 22) {
			world_->addActor(ActorGroup::Field, std::make_shared<MovelessFloor>(
				ResourceLoader::GetInstance().getTextureID(TextureID::FLOOR_SUB_TEX), world_, position, width, height));
		}
	}

	void SetSegmentPoint(int rowN, int colN, Vector2& startPoint, Vector2& endPoint, pointSetState state)
	{
		startPoint = segmentStartPoints[reader_.geti(rowN, colN)];
		endPoint = Vector2(static_cast<float>(colN), static_cast<float>(rowN)) * CHIPSIZE;

		Vector2 startSet, endSet;

		startSet = Vector2::Zero;
		endSet = Vector2(CHIPSIZE, 0);

		if (segmentStartPoints[reader_.geti(rowN, colN)].x < colN*CHIPSIZE) {
			startPoint = segmentStartPoints[reader_.geti(rowN, colN)] + startSet;
			endPoint = Vector2(static_cast<float>(colN), static_cast<float>(rowN)) * CHIPSIZE + endSet;
		}
		else if (segmentStartPoints[reader_.geti(rowN, colN)].x > colN*CHIPSIZE)
		{
			startPoint = Vector2(static_cast<float>(colN), static_cast<float>(rowN)) * CHIPSIZE + startSet;
			endPoint = segmentStartPoints[reader_.geti(rowN, colN)] + endSet;
		}
	}
private:
	//CSVのデータを引き出すためのクラス
	CsvReader reader_;
	//ゲーム本体、マップを追加する対象
	IWorld* world_;
	//追加したいマップチップはここに入れる
	std::map<int, MapChip> chips;
	std::map<int, bool> segmentChecker;
	std::map<int, Vector2> segmentStartPoints;
	std::map<int, TextureID> stagetexes;
	std::map<int, TextureID> segtexes;

	int rowSize;
	int colSize;
	int itemCount;
};

#endif // !MapGenerator