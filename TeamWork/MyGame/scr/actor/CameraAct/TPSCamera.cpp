//#include "TPSCamera.h"
//
//TPSCamera::TPSCamera() :Actor(nullptr) {
//
//}
//
//TPSCamera::TPSCamera(IWorld* world) : Actor(world), velo(Vector3::Zero) {//Actor(world, player.get()){
//
//	world_->EachActor(ACTOR_ID::SAMPLE_ACTOR, [&, this](const Actor& other) {
//		player_ = const_cast<Actor*>(&other);
//
//	});
//	Camera::GetInstance().SetRange(0.f, 1000.f);
//	Camera::GetInstance().SetViewAngle(60.f);
//	Camera::GetInstance().Up.Set(Vector3::Up);
//	Camera::GetInstance().Position.Set(player_->GetParameter().mat.Translation());
//	Camera::GetInstance().Target.Set(player_->GetParameter().mat.Translation());
//	Camera::GetInstance().Update();
//
//}
//
//TPSCamera::~TPSCamera() {
//
//}
//
//void TPSCamera::Update() {
//	Spring();
//
//	Camera::GetInstance().Position.Set(position_);
//	Camera::GetInstance().Target.Set(target_);
//	Camera::GetInstance().Update();
//}
//
//void TPSCamera::Draw() const {
//
//	DrawFormatString(200, 200, GetColor(255, 255, 255), "%f:%f", position_.x, position_.z);
//	DrawFormatString(300, 300, GetColor(255, 255, 255), "%f:%f", target_.x, target_.z);
//	DrawFormatString(200, 500, GetColor(255, 255, 255), "%f:%f", drawp.x, drawp.z);
//
//}
//
//Vector3 TPSCamera::Spring() {
//	//playerの座標を取得
//	target_ = player_->GetPose().Translation();
//	//固定値のMatrixを作成する()
//	Matrix mat1 = Matrix::CreateWorld(Vector3(0.f, 100.f, 100.f), Vector3(0, -100.f, -100.f), Vector3::Up);
//	//カメラ位置
//	Matrix world_mat = mat1*player_->GetPose();
//
//	
//	float stiffness = 0.5f;
//	float friction = 0.2f;
//	float mass = 1.f;
//	
//	//現在のpositionから本来いるべきpositionを引く
//	Vector3 stretch = (position_ - world_mat.Translation());
//	// バネの力を計算
//	Vector3 force = -stiffness * stretch;
//	// 加速度を追加
//	Vector3 acceleration = force / mass;
//	// 移動速度を計算
//	velo = friction * (velo + acceleration);
//	// 座標の更新
//	position_ += velo;
//
//	drawp = stretch;
//
//	return velo;
//}
