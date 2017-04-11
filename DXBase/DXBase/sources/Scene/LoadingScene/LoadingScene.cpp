#include "LoadingScene.h"
#include "../../ResourceLoader/ResourceLoader.h"

#include "../../Define.h"
#include "../../ResourceLoader/Movie.h"

#include "../../Renderer/NumberTexture.h"

LoadingScene::LoadingScene(SceneDataKeeper* keeper){
}

LoadingScene::~LoadingScene(){
	ResourceLoader::GetInstance().clearResources();
}

void LoadingScene::start(){

	//ここにローディング専用のリソースを読み込む
	ResourceLoader::GetInstance().loadAnimation(AnimationID::LORDING_SLIME_ANM, "./resources/sprite/slimeanmColor.png", Vector2(256, 256), 8, 4, 2);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::LOADING, "./resources/sprite/loading_anim.png", Vector2(623.0f, 120.0f), 3, 8, 0);
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBERS_TEX, "./resources/sprite/num.png");
	// アニメーション生成
	mLoadAnim.add_anim(0, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::LOADING));
	mLoadPuyo.add_anim(0, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::LORDING_SLIME_ANM));
	mLoadAnim.change_param(0);
	mLoadPuyo.change_param(0);
	//動画読み込みが非同期読み込みに対応していないため
	//動画

	//非同期読み込みを開始
	SetUseASyncLoadFlag(TRUE);
	
	//プレイヤー関連のロード
	load_player_res();
	load_enemy_res();
	load_boss_res();
	load_ui_res();

	ResourceLoader::GetInstance().loadTexture(TextureID::KIRIKABU_TEX, "./resources/sprite/kirikabu.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::HUKIDASI_TEX, "./resources/sprite/hukidasi.png");

	ResourceLoader::GetInstance().loadAnimation(AnimationID::SCENE_CHANGE_SLIME_ANM, "./resources/sprite/slimeanm.png", Vector2(256, 256), 8, 4, 2);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::ITEM_ANM, "./resources/sprite/itemanm.png", Vector2(96, 96),10,6,0);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::ITEM_MINUS_ANM, "./resources/sprite/itemanm2.png", Vector2(96, 96), 10, 6, 0);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::DOOR_CLOSE, "./resources/sprite/DoorAnimClose.png", Vector2(320, 384), 6, 4, 1);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::DOOR_OPEN, "./resources/sprite/DoorAnimOpen.png", Vector2(320, 384), 6, 4, 2);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::HATENA, "./resources/sprite/hatena.png", Vector2(256, 256), 8, 5, 1);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::TEXT_STAGECLEAR_ANM_TEX, "./resources/sprite/Text/stageclear_anim.png", Vector2(640, 200), 6, 7, 2);
	ResourceLoader::GetInstance().loadTexture(TextureID::PUYO_TEST_TEX, "./resources/sprite/en1.png");

	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE1_1_TEX, "./resources/sprite/BackGraund/Stage1-1.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE1_2_TEX, "./resources/sprite/BackGraund/Stage1-2.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE1_3_TEX, "./resources/sprite/BackGraund/Stage1-3.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE1_4_TEX, "./resources/sprite/BackGraund/Stage1-4.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE1_5_TEX, "./resources/sprite/BackGraund/Stage1-5.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE1_6_1_TEX, "./resources/sprite/BackGraund/Stage1-6-1.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE1_6_2_TEX, "./resources/sprite/BackGraund/Stage1-6-2.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE1_7_TEX, "./resources/sprite/BackGraund/Stage1-7.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE1_8_TEX, "./resources/sprite/BackGraund/Stage1-8.png");

	//ResourceLoader::GetInstance().loadTexture(TextureID::MGR_BACKSTAGE1_1_TEX, "./resources/sprite/BackGraund/BackMgrTex/Stage1-1.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::MGR_BACKSTAGE1_2_TEX, "./resources/sprite/BackGraund/BackMgrTex/Stage1-2.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::MGR_BACKSTAGE1_3_TEX, "./resources/sprite/BackGraund/BackMgrTex/Stage1-3.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::MGR_BACKSTAGE1_4_TEX, "./resources/sprite/BackGraund/BackMgrTex/Stage1-4.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::MGR_BACKSTAGE1_5_TEX, "./resources/sprite/BackGraund/BackMgrTex/Stage1-5.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::MGR_BACKSTAGE1_6_1_TEX, "./resources/sprite/BackGraund/BackMgrTex/Stage1-6-1.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::MGR_BACKSTAGE1_6_2_TEX, "./resources/sprite/BackGraund/BackMgrTex/Stage1-6-2.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::MGR_BACKSTAGE1_7_TEX, "./resources/sprite/BackGraund/BackMgrTex/Stage1-7.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::MGR_BACKSTAGE1_8_TEX, "./resources/sprite/BackGraund/BackMgrTex/Stage1-8.png");

	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE2_1_TEX, "./resources/sprite/BackGraund/Stage2-1.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE2_2_TEX, "./resources/sprite/BackGraund/Stage2-2.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE2_3_TEX, "./resources/sprite/BackGraund/Stage2-3.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE2_4_TEX, "./resources/sprite/BackGraund/Stage2-4.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE2_5_TEX, "./resources/sprite/BackGraund/Stage2-5.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE2_6_TEX, "./resources/sprite/BackGraund/Stage2-6.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE2_7_TEX, "./resources/sprite/BackGraund/Stage2-7.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE2_8_TEX, "./resources/sprite/BackGraund/Stage2-8.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE2_TOP1_TEX, "./resources/sprite/BackGraund/Stage2-Top1.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE2_TOP2_TEX, "./resources/sprite/BackGraund/Stage2-Top2.png");
	//ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE2_TOP3_TEX, "./resources/sprite/BackGraund/Stage2-Top3.png");

	ResourceLoader::GetInstance().loadTexture(TextureID::GAMEOVER_TEX, "./resources/sprite/GameOver.png");

	ResourceLoader::GetInstance().loadTexture(TextureID::MOVIEBACK_TEX, "./resources/sprite/Tutoreal/MovieBack.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TUTOREAL_BACK_TEX, "./resources/sprite/Tutoreal/tutorealFlame.png");

	ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE4_1_TEX, "./resources/sprite/BackGraund/Stage4-1.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::BACKSTAGE4_2_TEX, "./resources/sprite/BackGraund/Stage4-2.png");
	
	ResourceLoader::GetInstance().loadTexture(TextureID::PAUSE_BACK_TEX, "./resources/sprite/pauseBack.png");

	ResourceLoader::GetInstance().loadTexture(TextureID::TUTOREAL1_1_TXT_TEX, "./resources/sprite/Tutoreal/TutorealText1.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TUTOREAL2_1_TXT_TEX, "./resources/sprite/Tutoreal/TutorealText2.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TUTOREAL3_1_TXT_TEX, "./resources/sprite/Tutoreal/TutorealText3.png");

	ResourceLoader::GetInstance().loadTexture(TextureID::TUTOREAL_CLER_TEX, "./resources/sprite/Tutoreal/TutorealCler.png");


	ResourceLoader::GetInstance().loadTexture(TextureID::BOSS_GAUGE_IN_TEX, "./resources/sprite/BossGaugeIn.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::BOSS_GAUGE_OUT_TEX, "./resources/sprite/BossGaugeOut.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::BOSS_WING_TEX, "./resources/sprite/actor/boss/effect/wing.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::ENEMY_NEEDLE_TEX, "./resources/sprite/actor/enemy/Needle.png");


	ResourceLoader::GetInstance().loadTexture(TextureID::CHIP1_TEX, "./resources/sprite/chip1.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::CHIP2_TEX, "./resources/sprite/chip2.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::SEGMENT_TRI_TEX, "./resources/sprite/segchip.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::SEGMENT_TRI2_TEX, "./resources/sprite/segchip2.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::BLACK_BACK_TEX, "./resources/sprite/blackBack.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TITLE_BACK_TEX, "./resources/sprite/TitleBack.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TITLE_BACK2_TEX, "./resources/sprite/TitleBack2.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TITLE_BACK4_TEX, "./resources/sprite/TitleBack4.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TITLE_BACK_ALPHA_TEX, "./resources/sprite/TitleBackAlpha.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEAM_LOGO_TEX, "./resources/sprite/Logo.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::FLOOR_STAGE1_TEX, "./resources/sprite/Block/floor1.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::FLOOR_STAGE2_TEX, "./resources/sprite/Block/floor2.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::FLOOR_STAGE3_TEX, "./resources/sprite/Block/floor3.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::FLOOR_STAGE4_TEX, "./resources/sprite/Block/floor4.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::FLOOR_SUB_TEX, "./resources/sprite/Block/floorsub.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::FLOOR_WOOD_TEX, "./resources/sprite/Block/wood.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::FLOOR_STONE_TEX, "./resources/sprite/Block/stone.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_TITLE_TEX, "./resources/sprite/Text/title.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_CREDIT_TEX, "./resources/sprite/Text/credit.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_GAMEEND_TEX, "./resources/sprite/Text/gameend.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_GAMESTART_TEX, "./resources/sprite/Text/gamestart.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_MARU_TEX, "./resources/sprite/Text/maru.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_MENUBACK_TEX, "./resources/sprite/Text/menuback.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_TUTORIAL_TEX, "./resources/sprite/Text/tutorial.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_GAMEOVER_TEX, "./resources/sprite/Text/gameover.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_RETRY_TEX, "./resources/sprite/Text/retry.png");

	ResourceLoader::GetInstance().loadTexture(TextureID::CREDIT_TEX, "./resources/sprite/creditScene.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::PAUSE_CIRCLE_TEX, "./resources/sprite/text/pause_menu.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::PAUSE_START_TEX, "./resources/sprite/text/pause_start.png");

	ResourceLoader::GetInstance().loadTexture(TextureID::DOOR_STAY_TEX, "./resources/sprite/DoorStay.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::DOOR_OPEN_TEX, "./resources/sprite/DoorOpen.png");

	ResourceLoader::GetInstance().loadTexture(TextureID::STAGE_01_TEX, "./resources/sprite/Stage01.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::STAGE_02_TEX, "./resources/sprite/Stage02.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::STAGE_03_TEX, "./resources/sprite/Stage03.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::STAGE_04_TEX, "./resources/sprite/Stage04.png");

	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_ZERO_TEX, "./resources/sprite/num0.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_ONE_TEX, "./resources/sprite/num1.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_TWO_TEX, "./resources/sprite/num2.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_THREE_TEX, "./resources/sprite/num3.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_FOUR_TEX, "./resources/sprite/num4.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_FIVE_TEX, "./resources/sprite/num5.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_SIX_TEX, "./resources/sprite/num6.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_SEVEN_TEX, "./resources/sprite/num7.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_EIGHT_TEX, "./resources/sprite/num8.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_NINE_TEX, "./resources/sprite/num9.png");
	
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_R_ZERO_TEX, "./resources/sprite/num0_r.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_R_ONE_TEX, "./resources/sprite/num1_r.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_R_TWO_TEX, "./resources/sprite/num2_r.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_R_THREE_TEX, "./resources/sprite/num3_r.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_R_FOUR_TEX, "./resources/sprite/num4_r.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_R_FIVE_TEX, "./resources/sprite/num5_r.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_R_SIX_TEX, "./resources/sprite/num6_r.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_R_SEVEN_TEX, "./resources/sprite/num7_r.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_R_EIGHT_TEX, "./resources/sprite/num8_r.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::NUMBER_R_NINE_TEX, "./resources/sprite/num9_r.png");

	ResourceLoader::GetInstance().loadTexture(TextureID::ITEM_TEX, "./resources/sprite/item.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::ITEM_MINUS_TEX, "./resources/sprite/item2.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::VASE_TEX, "./resources/sprite/vase.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::GOAL_TEX, "./resources/sprite/goal.png");

	ResourceLoader::GetInstance().loadTexture(TextureID::HAPPA1_1_TEX, "./resources/sprite/Happa/Happa1-1.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::HAPPA1_2_TEX, "./resources/sprite/Happa/Happa1-2.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::HAPPA3_1_TEX, "./resources/sprite/Happa/Happa3-1.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::HAPPA3_2_TEX, "./resources/sprite/Happa/Happa3-2.png");

	ResourceLoader::GetInstance().loadTexture(TextureID::BOARD_NULL_TEX, "./resources/sprite/kanban0.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::BOARD_STAGE2_TEX, "./resources/sprite/kanban01.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::BOARD_STAGE3_TEX, "./resources/sprite/kanban02.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::BOARD_STAGE4_TEX, "./resources/sprite/kanban03.png");
	
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_COMBO2_TEX, "./resources/sprite/result/2combo.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_COMBO3_TEX, "./resources/sprite/result/3combo.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_COMBO4_TEX, "./resources/sprite/result/4combo.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_COMBO5_TEX, "./resources/sprite/result/5combo.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_CHARGETIME_TEX, "./resources/sprite/result/charge_time.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_GETSTAR_TEX, "./resources/sprite/result/get_star.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_MAXCOMBO_TEX, "./resources/sprite/result/max_combo.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_MINUS_MULT_TEX, "./resources/sprite/result/minus.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_MULTIPLY_TEX, "./resources/sprite/result/Multiply.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_NoENEMY_DEATH_TEX, "./resources/sprite/result/noenemydeath.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_STAR_COUNT_TEX, "./resources/sprite/result/yellowstar_count.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_DAMAGE_COUNT_TEX, "./resources/sprite/result/sprit_count.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_STICK_COUNT_TEX, "./resources/sprite/result/stick_count.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_TOTAL_TEX, "./resources/sprite/result/totle.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_WALK_BLOCK_COUNT_TEX, "./resources/sprite/result/walk_blockcount.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_RANK_A_TEX, "./resources/sprite/result/rank_a.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_RANK_B_TEX, "./resources/sprite/result/rank_b.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TEXT_RANK_C_TEX, "./resources/sprite/result/rank_c.png");

	ResourceLoader::GetInstance().loadTexture(TextureID::COMBO_TEX, "./resources/sprite/combo.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::THANK_TEX, "./resources/sprite/Thank.png");

	ResourceLoader::GetInstance().loadTexture(TextureID::TUTO_SKIP_TEX, "./resources/sprite/Text/tutoreal_skip.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TUTO_YES_TEX, "./resources/sprite/Text/YES.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::TUTO_NO_TEX, "./resources/sprite/Text/NO.png");
	load_bgm_res();
	load_se_res();

	//非同期読み込み終了
	SetUseASyncLoadFlag(FALSE);
	allCount_ = GetASyncLoadNum();
	now_ = 0.0f;
	//---Updateテスト用---
	mPosition = SCREEN_SIZE - Vector2(623, 120);
	isEnd_ = false;
	mAngle = 0;
	//---------
}

void LoadingScene::update(){
	mAngle += 10.0f*Time::GetInstance().deltaTime();
	mLoadAnim.update(Time::GetInstance().deltaTime());
	mLoadPuyo.update(Time::GetInstance().deltaTime());
	now_ = 100 - (int)(((float)GetASyncLoadNum() / (float)allCount_)*100.0f);
	//読み込み処理が終わっていたら
	if (GetASyncLoadNum() == 0 && ProcessMessage() == 0) {
		isEnd_ = true;
	}
}

void LoadingScene::draw() const{
	mLoadAnim.draw(mPosition - Vector2(0, 1) * 20, Vector2::Zero, 1.0f);
	NumberTexture num = NumberTexture(TextureID::NUMBERS_TEX, 96, 96);
	num.draw2(mPosition - Vector2(106, -26), now_, 3, Vector3(255, 255, 255), 0.3f);
	mLoadPuyo.draw(mPosition - Vector2(128, 48), Vector2::Zero, 0.5f);
}

void LoadingScene::end(){
}

bool LoadingScene::isEnd() const{
	return isEnd_;
}

Scene LoadingScene::next() const{
	return Scene::Logo;
}

void LoadingScene::load_player_res(){
	//butty
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_BUTTY_IDLE,		"./resources/sprite/actor/player/butty/b_idle.png",			256, 8, 4, 1);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_BUTTY_MOVE,		"./resources/sprite/actor/player/butty/b_move.png",			256, 8, 4, 1);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_BUTTY_HOLD,		"./resources/sprite/actor/player/butty/b_hold.png",			256, 8, 4, 1);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_BUTTY_SWIM,		"./resources/sprite/actor/player/butty/b_swim.png",			256, 8, 4, 1);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_BUTTY_SWIM_TURN,"./resources/sprite/actor/player/butty/b_swimturn.png",		256, 8, 2, 0);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_BUTTY_DEATH,	"./resources/sprite/actor/player/butty/b_death.png",		256, 8, 2, 0);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_BUTTY_DAMAGE,	"./resources/sprite/actor/player/butty/b_damage.png",		256, 8, 2, 0);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_BUTTY_DIV_IDLE, "./resources/sprite/actor/player/butty/b_division_idle.png",256, 8, 2, 1);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_BUTTY_DIV_MOVE, "./resources/sprite/actor/player/butty/b_division_move.png",256, 8, 2, 5);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_BUTTY_TURN,		"./resources/sprite/actor/player/butty/b_turn.png",			256, 8, 2, 5);
	//retty
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_RETTY_IDLE,		"./resources/sprite/actor/player/retty/r_idle.png",			256, 8, 4, 1);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_RETTY_MOVE,		"./resources/sprite/actor/player/retty/r_move.png",			256, 8, 4, 1);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_RETTY_HOLD,		"./resources/sprite/actor/player/retty/r_hold.png",			256, 8, 4, 1);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_RETTY_SWIM,		"./resources/sprite/actor/player/retty/r_swim.png",			256, 8, 4, 1);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_RETTY_SWIM_TURN,"./resources/sprite/actor/player/retty/r_swimturn.png",		256, 8, 2, 0);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_RETTY_DEATH,	"./resources/sprite/actor/player/retty/r_death.png",		256, 8, 2, 0);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_RETTY_DAMAGE,	"./resources/sprite/actor/player/retty/r_damage.png",		256, 8, 2, 0);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_RETTY_DIV_IDLE, "./resources/sprite/actor/player/retty/r_division_idle.png",256, 8, 2, 1);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_RETTY_DIV_MOVE, "./resources/sprite/actor/player/retty/r_division_move.png",256, 8, 2, 5);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_RETTY_TURN,		"./resources/sprite/actor/player/retty/r_turn.png",			256, 8, 2, 5);

	//エフェクト
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_EFFECT_SEP_MOVE, "./resources/sprite/actor/effect/Separation_move.png", 256, 7, 9, 6);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_EFFECT_SEP_EXP, "./resources/sprite/actor/effect/Separation.png", 256, 8, 6, 0);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_EFFECT_SHOUGEKI, "./resources/sprite/actor/effect/large_cell_256_256_syougeki_sun2.png", 256, 5, 5, 0);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_EFFECT_POWER_UP, "./resources/sprite/actor/effect/PawerUP.png", 192, 5, 5, 2);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::PLAYER_EFFECT_STAR_POWER_UP, "./resources/sprite/actor/effect/star_pawerUP.png", 256, 5, 11, 0);
}

void LoadingScene::load_enemy_res(){
	ResourceLoader::GetInstance().loadAnimation(AnimationID::ENEMY_EGGENEMY_WALK_TEX, "./resources/sprite/actor/enemy/Enemy01_Walk.png", 256, 8, 4, 1);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::ENEMY_EGGENEMY_WALKTURN_TEX, "./resources/sprite/actor/enemy/Enemy01_Walkturn.png", 256, 8, 2);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::ENEMY_EGGENEMY_DISCORVER_TEX, "./resources/sprite/actor/enemy/Enemy01_discorver.png", 256, 8, 2);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::ENEMY_EGGENEMY_ATTACK_TEX, "./resources/sprite/actor/enemy/Enemy01_Attack.png", 256, 8, 2);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::ENEMY_EGGENEMY_ATTACKTURN_TEX, "./resources/sprite/actor/enemy/Enemy01_Attackturn.png", 256, 4, 2);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::ENEMY_EGGENEMY_DAMAGE_TEX, "./resources/sprite/actor/enemy/Enemy01_damege.png", 256, 8, 2);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::ENEMY_FLYINGENEMY_WALK_TEX, "./resources/sprite/actor/enemy/Enemy02_walk.png", 256, 8, 3, 3);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::ENEMY_FLYINGENEMY_ATTACK_TEX, "./resources/sprite/actor/enemy/Enemy02_attack.png", 256, 8, 2, 5);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::ENEMY_FLYINGENEMY_DAMAGE_TEX, "./resources/sprite/actor/enemy/Enemy02_damege.png", 256, 8, 3, 3);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::ENEMY_NEEDLEENEMY_TEX, "./resources/sprite/actor/enemy/Enemy03.png", 256, 8, 2);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::ENEMY_WALLMOVEENEMY_TEX, "./resources/sprite/actor/enemy/Enemy04_Left.png", 256, 8, 4, 1);
	// エフェクト
	ResourceLoader::GetInstance().loadAnimation(AnimationID::ENEMY_EFFECT_COLLIDE_TEX, "./resources/sprite/actor/enemy/effect/syoutotu.png", 256, 5, 5);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::ENEMY_EFFECT_DEAD_TEX, "./resources/sprite/actor/enemy/effect/Enemy_Dead.png", Vector2(280, 800), 10, 12);
}

void LoadingScene::load_boss_res(){
	ResourceLoader::GetInstance().loadAnimation(AnimationID::BOSS_WAIT_TEX, "./resources/sprite/actor/boss/Boss_wait.png", 256, 8, 4, 1);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::BOSS_WAIT_TURN_TEX, "./resources/sprite/actor/boss/Boss_wait_turn.png", 256, 8, 2);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::BOSS_JUMP_UP_TEX, "./resources/sprite/actor/boss/Boss_jumpUP.png", 256, 8, 3, 3);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::BOSS_JUMP_DOWN_TEX, "./resources/sprite/actor/boss/Boss_jumpDOWN.png", 256, 8, 4);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::BOSS_WALLATTACK_DASH_TEX, "./resources/sprite/actor/boss/Boss_dash.png", 256, 8, 2, 5);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::BOSS_WALLATTACK_DASHJUMP_TEX, "./resources/sprite/actor/boss/Boss_dashjump.png", 256, 8, 2, 4);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::BOSS_WALLATTACK_DASHJUMP_STOP_TEX, "./resources/sprite/actor/boss/Boss_dashjump_stop.png", 256, 8, 3, 3);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::BOSS_BREATH_DYSFUNCTION_TEX, "./resources/sprite/actor/boss/Boss_3_dysfunction.png", 256, 8, 4);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::BOSS_PIYO_TEX, "./resources/sprite/actor/boss/Boss_piyo.png", 256, 8, 4);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::BOSS_DAMAGE_TEX, "./resources/sprite/actor/boss/Boss_damege.png", Vector2(260, 256), 7, 2, 2);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::BOSS_DAMAGE_BOKO_TEX, "./resources/sprite/actor/boss/Boss_damage_boko.png", 256, 8, 4);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::BOSS_DEATH_TEX, "./resources/sprite/actor/boss/Boss_deth.png", 256, 8, 4);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::BOSS_EFFECT_ATTACK_TEX, "./resources/sprite/actor/boss/effect/boss_attack_star.png", 256, 5, 3);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::BOSS_EFFECT_BOKO_TEX, "./resources/sprite/actor/boss/effect/boss_boko.png", 512, 5, 6);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::BOSS_EFFECT_PIYO_TEX, "./resources/sprite/actor/boss/effect/piyori.png", 256, 8, 7);
	ResourceLoader::GetInstance().loadAnimation(AnimationID::BOSS_EFFECT_ENTRY_SIGN_TEX, "./resources/sprite/actor/boss/effect/chance_anim.png", Vector2(320, 321), 6, 4);
}


void LoadingScene::load_ui_res() {
	ResourceLoader::GetInstance().loadTexture(TextureID::HURRY_TEX, "./resources/sprite/UI/HurryUP.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::BUTTON_R1_PUSH_TEX, "./resources/sprite/UI/button/R1_push.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::BUTTON_L1_PUSH_TEX, "./resources/sprite/UI/button/L1_push.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::BUTTON_R1_UP_TEX, "./resources/sprite/UI/button/R1_up.png");
	ResourceLoader::GetInstance().loadTexture(TextureID::BUTTON_L1_UP_TEX, "./resources/sprite/UI/button/L1_up.png");
}

void LoadingScene::load_bgm_res(){
	ResourceLoader::GetInstance().loadSound(SoundID::BGM_MENU, "./resources/sounds/bgm/game_menuBGM.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::BGM_STAGE_123, "./resources/sounds/bgm/stage1,2,3_BGM.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::BGM_STAGE_4, "./resources/sounds/bgm/stage4_BGM.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::BGM_STAGE_5, "./resources/sounds/bgm/stage5_BGM.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::BGM_STAGECLEAR, "./resources/sounds/menuse/stageClear.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::BGM_GAMEOVER, "./resources/sounds/menuse/gameover.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::BGM_INTER, "./resources/sounds/makuma_BGM.mp3");

}

void LoadingScene::load_se_res(){
	ResourceLoader::GetInstance().loadSound(SoundID::SE_BUDDY, "./resources/sounds/buddy.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_REDDY, "./resources/sounds/reddy.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_KAIWA, "./resources/sounds/kaiwa.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_BUNRI, "./resources/sounds/bunri.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_NOBI, "./resources/sounds/nobi.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_NOBI_FULL, "./resources/sounds/nobi_full.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_PUYON, "./resources/sounds/puyon.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_SHOT, "./resources/sounds/shot.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_SYOUTOTU, "./resources/sounds/syoutotu.mp3");
	// 敵
	ResourceLoader::GetInstance().loadSound(SoundID::SE_ENEMY_HAKKEN, "./resources/sounds/enemy/enemy_hakkenn.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_ENEMY_HITSTOP, "./resources/sounds/enemy/hitstop.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_ENEMY_DEAD, "./resources/sounds/enemy/kill_effect.mp3");
	// ボス
	ResourceLoader::GetInstance().loadSound(SoundID::SE_BOSS_CHAKUCHI, "./resources/sounds/boss/boss_chakuchi.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_BOSS_DAMAGE, "./resources/sounds/boss/boss_dame.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_BOSS_JUMP, "./resources/sounds/boss/boss_jump.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_BOSS_WALLATTACK, "./resources/sounds/boss/boss2_attack.mp3");
	SetCreateSoundPitchRate(600.0f);
	SetCreateSoundTimeStretchRate(3.0f);
	ResourceLoader::GetInstance().loadSound(SoundID::SE_BOSS_DEAD, "./resources/sounds/boss/boss_dame.mp3");
	SetCreateSoundTimeStretchRate(1.0f);
	SetCreateSoundPitchRate(0.0f);
	ResourceLoader::GetInstance().loadSound(SoundID::SE_BOSS_WIND, "./resources/sounds/boss/boss_wind.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_BOSS_POKO, "./resources/sounds/boss/boss_poko.mp3");
	// ミニボス
	SetCreateSoundPitchRate(600.0f * 1.5f);
	SetCreateSoundTimeStretchRate(0.85f);
	ResourceLoader::GetInstance().loadSound(SoundID::SE_MINIBOSS_CRY, "./resources/sounds/boss/boss_dame.mp3");
	SetCreateSoundTimeStretchRate(1.0f);
	SetCreateSoundPitchRate(0.0f);

	ResourceLoader::GetInstance().loadSound(SoundID::SE_GETCOIN, "./resources/sounds/coin.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_GETMINUSCOIN, "./resources/sounds/minuscoin.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_RESULT_ROLL, "./resources/sounds/score_up.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_RESULT_SHOW, "./resources/sounds/score_den.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_RESULT_SHOWMAX, "./resources/sounds/score_den.mp3");
	
	ResourceLoader::GetInstance().loadSound(SoundID::SE_RESULT_RANKC, "./resources/sounds/rank01.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_RESULT_RANKB, "./resources/sounds/rank02.mp3");
	ResourceLoader::GetInstance().loadSound(SoundID::SE_RESULT_RANKA, "./resources/sounds/rank03.mp3");

}
