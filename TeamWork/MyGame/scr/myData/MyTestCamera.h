//#pragma once
//#pragma once
//
//#include"../camera/Camera.h"
//#include"../actor/Actor.h"
//#include"../actor/player/Player.h"
//#include"../input/Keyboard.h"
//#include<map>
//
//class MyTestCamera :public Actor {
//public:
//	MyTestCamera() :Actor(nullptr) {
//		Init();
//
//	}
//	MyTestCamera(IWorld* world) : Actor(world), velo(Vector3::Zero), defCameraPos_(0.f, 100.f, 100.f) {//Actor(world, player.get()){
//
//		world_->EachActor(ACTOR_ID::SAMPLE_ACTOR, [&, this](const Actor& other) {
//			player_ = const_cast<Actor*>(&other);
//
//		});
//
//		Init();
//
//		Camera::GetInstance().SetRange(0.f, 1000.f);
//		Camera::GetInstance().SetViewAngle(60.f);
//		Camera::GetInstance().Up.Set(Vector3::Up);
//		Camera::GetInstance().Position.Set(player_->GetParameter().mat.Translation());
//		Camera::GetInstance().Target.Set(player_->GetParameter().mat.Translation());
//		Camera::GetInstance().Update();
//
//	};
//	~MyTestCamera() {
//
//	}
//	void Init() {
//		camPos = defCameraPos_;
//
//		cameraDefPosDatas_[KEYCODE::NUM1] = Vector3(0.f, 100.f, 100.f);
//		cameraDefPosDatas_[KEYCODE::NUM2] = Vector3(0.f, 100.f, -100.f);
//		cameraDefPosDatas_[KEYCODE::NUM3] = Vector3(0.f, -100.f, 100.f);
//		cameraDefPosDatas_[KEYCODE::NUM4] = Vector3(0.f, -100.f, -100.f);
//		cameraDefPosDatas_[KEYCODE::NUM5] = Vector3(0.f, 50.f, 50.f);
//		cameraDefPosDatas_[KEYCODE::NUM6] = Vector3(0.f, 50.f, 100.f);
//		cameraDefPosDatas_[KEYCODE::NUM7] = Vector3(0.f, 100.f, 50.f);
//		cameraDefPosDatas_[KEYCODE::NUM8] = Vector3(0.f, 0.f, 1.f);
//	}
//	virtual void Update() {
//
//		for (auto& dt : cameraDefPosDatas_){
//			if (Keyboard::GetInstance().KeyStateDown(dt.first)) {
//				camPos = dt.second;
//				break;
//			}
//		}
//		defCameraPos_ = camPos;
//
//		Spring();
//
//		Camera::GetInstance().Position.Set(position_);
//		Camera::GetInstance().Target.Set(target_);
//		Camera::GetInstance().Update();
//	};
//	virtual void Draw() const{
//
//		DrawFormatString(200, 200, GetColor(255, 255, 255), "%f:%f", position_.x, position_.z);
//		DrawFormatString(300, 300, GetColor(255, 255, 255), "%f:%f", target_.x, target_.z);
//		DrawFormatString(200, 500, GetColor(255, 255, 255), "%f:%f", drawp.x, drawp.z);
//
//	};
//
//private:
//	Vector3 Spring() {
//		//player�̍��W���擾
//		target_ = player_->GetPose().Translation();
//		//�Œ�l��Matrix���쐬����()
//		Matrix mat1 = Matrix::CreateWorld(defCameraPos_, -defCameraPos_, Vector3::Up);
//		//�J�����ʒu
//		Matrix world_mat = mat1*player_->GetPose();
//
//		float stiffness = 0.5f;
//		float friction = 0.2f;
//		float mass = 1.f;
//
//		//���݂�position����{������ׂ�position������
//		Vector3 stretch = (position_ - world_mat.Translation());
//		// �o�l�̗͂��v�Z
//		Vector3 force = -stiffness * stretch;
//		// �����x��ǉ�
//		Vector3 acceleration = force / mass;
//		// �ړ����x���v�Z
//		velo = friction * (velo + acceleration);
//		// ���W�̍X�V
//		position_ += velo;
//
//		drawp = stretch;
//
//		return velo;
//	};
//private:
//	Actor* player_;
//	Vector3 target_;
//
//	Vector3 velo;
//	Vector3 drawp;
//
//	Vector3 lastPos;
//
//	Vector3 defCameraPos_;
//
//	Vector3 camPos;
//	
//	std::map<UINT, Vector3> cameraDefPosDatas_;
//};
