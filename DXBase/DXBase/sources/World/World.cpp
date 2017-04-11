#include "World.h"

#include "../Field/Field.h"
#include "../Define.h"

// コンストラクタ
World::World(SceneDataKeeper* keeper) :
	listener_([](EventMessage, void*) {}),
	deltaTime_(0.0f),
	is_clear_(false),
	isEntered_(false),
	isLetOuted_(false),
	isStopTime_(false),
	isMBossStage_(false),
	mNoPlayerMove(false),itemCount_(0),getCount_(0),
	playerScreenPos_(PLAYER_SCREEN_POSITION),
	keeper_(keeper){
}

// 更新
void World::update(float deltaTime) {
	//field_->update(deltaTime);
	auto player = findActor("Player");
	if (player != nullptr) {
		inv(playerMat_);
		playerMat_ = player->getPose();
	}
	deltaTime_ = deltaTime;
	if (isStopTime_) deltaTime = 0.0f;
	actors_.update(deltaTime);

	//camera_->update(deltaTime);
	//light_->update(deltaTime);
}

// 描画
void World::draw() const {
	field_->draw(inv_);
	//camera_->draw();
	//light_->draw();
	actors_.draw();
}

// メッセージ処理
void World::handleMessage(EventMessage message, void* param) {
	// ワールドのメッセージ処理
	listener_(message, param);
	// アクターのメッセージ処理
	actors_.handleMessage(message, param);
	//camera_->handleMessage(message, param);
	//light_->handleMessage(message, param);
}

// フィールドの追加
void World::addField(const FieldPtr& field) {
	field_ = field;
}

// カメラの追加
void World::addCamera(const ActorPtr& camera) {
	camera_ = camera;
}

// ライトの追加
void World::addLight(const ActorPtr& light) {
	light_ = light;
}

// アクターの追加
void World::addActor(ActorGroup group, const ActorPtr& actor) {
	actors_.addActor(group, actor);
}

void World::addUIActor(const ActorUIPtr & actor)
{
	actors_.addUIActor(actor);
}
// アクターの検索
ActorPtr World::findActor(const std::string& name) {
	return actors_.findActor(name);
}
//アクター検索(list返し)
std::forward_list<ActorPtr> World::findActors(const ActorGroup group)
{
	return actors_.GetActors(group);
}

// フィールドの取得
FieldPtr World::getField() const {
	return field_;
}


unsigned int World::fieldHandle() const {
	return field_->modelHandle();
}

// メッセージの送信
void World::sendMessage(EventMessage message, void* param) {
	handleMessage(message, param);
}

// イベントリスナーの追加
void World::addEventMessageListener(
	std::function<void(EventMessage, void*)> listener) {
	listener_ = listener;
}

bool World::is_clear() {
	return is_clear_;
}

void World::clear(bool clear) {
	is_clear_ = clear;
}

// ボスの口に入ったかを返します
bool World::isEntered()
{
	return isEntered_;
}

// ボスの口から脱出したかを返します
bool World::isLetOuted()
{
	return isLetOuted_;
}

// ボスの口の侵入脱出を設定します
void World::setEntry(const bool isEntry, const bool isLetOut)
{
	isEntered_ = isEntry;
	isLetOuted_ = isLetOut;
}

// タイマが止まっているかを返します
bool World::isStopTime()
{
	return isStopTime_;
}

// タイマを0にするかを設定します
void World::setIsStopTime(const bool isTime)
{
	isStopTime_ = isTime;
}

// デルタタイムを取得します(元のタイムクラスから取得)
float World::getDeltaTime()
{
	return deltaTime_;
}

// 中ボスのステージ内に入ったかを設定します
void World::setIsMBossStage(bool isStage)
{
	isMBossStage_ = isStage;
}

bool World::isMBossStage()
{
	return isMBossStage_;
}

void World::SetScroolJudge(const Vector2& scroolJudge,const Vector2& scroolMinPos,const Vector2& scroolMaxPos, bool flag)
{
	scrool_.scroolJudge = scroolJudge;
	scrool_.scroolStopMin = scroolMinPos;
	scrool_.scroolStopMax = scroolMaxPos;
	//if (!flag)
	//	InitializeInv(scroolMinPos);
	//else
	//	InitializeInv(scroolMaxPos);
}

ScroolJudge World::GetScroolJudge() {
	return scrool_;
}

void World::inv(const Matrix& mat) {
	ScroolJudge scrool = GetScroolJudge();
	//1フレーム前の座標
	mPrePos = Vector2(inv_.Translation().x, inv_.Translation().y);
	//スクロールストップ処理
	Matrix playerMat;
	playerMat = mat;

	float clampPosX = MathHelper::Clamp(playerMat.Translation().x, scrool.scroolStopMin.x, scrool.scroolStopMax.x);
	float clampPosY = MathHelper::Clamp(playerMat.Translation().y, scrool.scroolStopMin.y, scrool.scroolStopMax.y);
	if (scrool.scroolJudge.x == 0)
		clampPosX = playerScreenPos_.x;
	if (scrool.scroolJudge.y == 0)
		clampPosY = playerScreenPos_.y;
	playerMat.Translation(Vector3(clampPosX, clampPosY, 0.0f));

	//行くべき位置を設定(matrix版)
	resInv_ = Matrix::Invert(playerMat) *
		Matrix::CreateTranslation(Vector3(playerScreenPos_.x, playerScreenPos_.y));
	//行くべき位置を設定
	Vector2 resPos = Vector2(resInv_.Translation().x, resInv_.Translation().y);
	Vector2 pos = Vector2(inv_.Translation().x, inv_.Translation().y);

	Spring(pos, resPos, velo,0.2f);
	//補正された移動マトリックス代入
	inv_ = Matrix::CreateTranslation(Vector3(
		pos.x*scrool.scroolJudge.x,
		pos.y*scrool.scroolJudge.y,
		0.0f));

	//1フレーム後の座標
	mCurPos = Vector2(inv_.Translation().x, inv_.Translation().y);
	//移動量を計算
	mVelo = mPrePos - mCurPos;
	mVelo = Vector2(mVelo.x*scrool.scroolJudge.x, mVelo.y * scrool.scroolJudge.y);

}

Matrix World::InitializeInv(Vector2 position)
{
	//1フレーム前の座標
	mPrePos = Vector2(inv_.Translation().x, inv_.Translation().y);
	Matrix playerMat;
	ScroolJudge scrool = GetScroolJudge();
	playerMat.Translation(Vector3(position.x, position.y, 0.0f));

	inv_ = Matrix::Invert(playerMat) *
		Matrix::CreateTranslation(Vector3(playerScreenPos_.x, playerScreenPos_.y));
	resInv_ = Matrix::Invert(playerMat) *
		Matrix::CreateTranslation(Vector3(playerScreenPos_.x, playerScreenPos_.y));

	//1フレーム後の座標
	mCurPos = Vector2(inv_.Translation().x, inv_.Translation().y);
	//移動量を計算
	//mVelo = mPrePos - mCurPos;
	return inv_;
}

void World::SetScroolPos(const Vector2 & pos)
{
	playerMat_.Translation(Vector3(pos.x, pos.y, 0));
}

void World::Spring(Vector2 & pos, Vector2 & resPos, Vector2 & velo, float stiffness, float friction, float mass) const
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


