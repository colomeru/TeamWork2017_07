#include "EndingScene.h"
#include "../time/Time.h"
#include "../camera/Camera.h"
#include "../conv/DXConverter.h"
#include "../graphic/Sprite.h"
#include "../input/Keyboard.h"
#include <cmath>
#include <math.h>
#include "../math/MathHelper.h"
#include "../stageGenerator/Stage1/Stage1.h"
#include "../input//GamePad.h"
#include "../game//Random.h"


EndingScene::EndingScene() :
	nextScene_(Scene::Title)
{
	// ���[���h����
	world_ = std::make_shared<World>();
	// �C�x���g���X�i�[�o�^
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});
}

EndingScene::~EndingScene()
{
}

void EndingScene::Initialize()
{
	isEnd_ = false;

	Camera::GetInstance().SetRange(0.1f, 10000.0f);
	Camera::GetInstance().SetViewAngle(60.0f);
	Camera::GetInstance().Up.Set(Vector3::Up);
	Camera::GetInstance().Position.Set(Vector3(0.0f, 0.0f, -10.0f));
	Camera::GetInstance().Target.Set(Vector3(0.0f, 0.0f, 0.0f));
	Camera::GetInstance().Update();

	size = 1.0f;
	horizontal = 0.0f;
	vertical = 0.0f;
	alpha = 1.0f;
	cAlpha = 1.0f;
	aAlpha = 1.0f;
	particlePos = Vector2(200.0f, 200.0f);
	circlePos = Vector2(400.0f, 200.0f);
	turn = false;
	rotDirection = true;
	r1 = rot;
	r2 = 0.0f;
	v1 = Vector2::Zero;
	v2 = Vector2::Zero;
	f = 1.0f;

	arrowSize = Sprite::GetInstance().GetSize(SPRITE_ID::ARROW_SPRITE);
	particleSize = Sprite::GetInstance().GetSize(SPRITE_ID::TEST_SPRITE);
	circleSize = Sprite::GetInstance().GetSize(SPRITE_ID::CIRCLE_SPRITE);

	stageManager.Add(Stage::Stage1, std::make_shared<Stage1>(world_.get(), std::string("Stage1")));
	stageManager.SetStage(Stage::Stage1);

	//�U��q
	for (int i = 0; i < 8; i++)
	{
		neckLen[i] = Random::GetInstance().Range(360.0f, 362.0f);
	}

	fx = 200.0f;
	fy = 100.0f;
	rot = 0.0f;
	rot_spd = 0.0f;
	r = 32.0f;
	vec = 0;
	length = neckLen[vec];
	g = 0.5f;
	friction = 0.995f;
	vec = 0;
	lineRot[0] = -90.0f;
	stageLen = stageManager.GetStageSize(Stage::Stage1).x;
	meterLen = 800.0f;
	meterPos = Vector2(200.0f, 100.0f);

	dRot = 0.0f;
	dRot_spd = 0.0f;
}

void EndingScene::Update()
{
	// �X�V
	world_->Update();

	// �I��
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;
	Camera::GetInstance().Position.Set(Vector3(0.0f, 0.0f, -10.0f));
	Camera::GetInstance().Target.Set(Vector3(0.0f, 0.0f, 0.0f));
	Camera::GetInstance().Update();

	//�l�p�̓����蔻��
	centerPos = Vector2(particlePos.x + particleSize.x / 2, particlePos.y + particleSize.y / 2);

	absH = abs(centerPos.x - arrowPos.x);
	absV = abs(centerPos.y - arrowPos.y);

	distanceH = particleSize.x / 2;
	distanceV = particleSize.y / 2;

	if (absH < distanceH && absV < distanceV) alpha = 0.5f;
	else alpha = 1.0f;

	//�~�̓����蔻��
	cCenterPos = Vector2(circlePos.x + circleSize.x / 2, circlePos.y + circleSize.y / 2);
	absC = cCenterPos - arrowPos;
	distanceC = sqrt((absC.x * absC.x) + (absC.y * absC.y));
	if (distanceC < circleSize.x / 2) cAlpha = 0.5f;
	else cAlpha = 1.0f;


	Pendulum();
	Double(spherePos);
	
}

void EndingScene::Draw() const
{
	DrawFormatString(0, 00, GetColor(255, 255, 255), "EndingScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);
	DrawFormatString(0, 40, GetColor(255, 255, 255), "���W %f %f", arrowPos.x, arrowPos.y);
	DrawFormatString(0, 60, GetColor(255, 255, 255), "X:%f Y:%f", absH, absV);
	DrawFormatString(0, 100, GetColor(255, 255, 255), "rot:%f", rot);
	DrawFormatString(0, 120, GetColor(255, 255, 255), "rot_spd:%f", rot_spd);
	DrawFormatString(0, 140, GetColor(255, 255, 255), "friction:%f", friction);
	DrawFormatString(0, 180, GetColor(255, 255, 255), "vec:%d", vec);
	DrawFormatString(0, 200, GetColor(255, 255, 255), "spherePos x:%f y:%f", spherePos.x, spherePos.y);
	for (int i = 0; i < 8; i++)
	{
		DrawFormatString(0, 220 + 20 * i, GetColor(255, 255, 255), "neckLen%d:%f", i, neckLen[i]);
	}
	DrawFormatString(0, 380, GetColor(255, 255, 255), "r:%f", r);
	DrawFormatString(0, 400, GetColor(255, 255, 255), "limit:%f", spdLimit);
	if (rotDirection && rot < 90) DrawFormatString(0, 420, GetColor(255, 255, 255), "���ɉ����ł��܂��I");
	else if (!rotDirection && rot > 90) DrawFormatString(0, 420, GetColor(255, 255, 255), "�E�ɉ����ł��܂��I");
	else DrawFormatString(0, 420, GetColor(255, 255, 255), "�����ł��܂���I");
	DrawFormatString(0, 440, GetColor(255, 255, 255), "dRot:%f", dRot);
	DrawFormatString(0, 460, GetColor(255, 255, 255), "dRot_spd:%f", dRot_spd);

	// �`��
	world_->Draw();


	//Sprite::GetInstance().Draw(SPRITE_ID::TEST2_SPRITE, Vector2(0, 400));
	//Sprite::GetInstance().Draw(SPRITE_ID::TEST_SPRITE, particlePos, alpha);
	//Sprite::GetInstance().Draw(SPRITE_ID::CIRCLE_SPRITE, circlePos, cAlpha);

	//Sprite::GetInstance().Draw(SPRITE_ID::ARROW_SPRITE, arrowPos, aAlpha);
	Sprite::GetInstance().Draw(SPRITE_ID::HITO_SPRITE, spherePos, Vector2(16, 32), Vector2::One, rot2 + lineRot[2]);
	DrawCircle(spherePos.x, spherePos.y, (int)r, GetColor(255, 255, 255), 0, 1);
	DrawCircle(fx, fy, 16, GetColor(255, 0, 0), 0, 1); //�x�_�ɉ~��\��
	DrawLine(inPos[0].x, inPos[0].y, outPos[0].x, outPos[0].y, GetColor(0, 255, 0), 1); //��
	DrawLine(inPos[1].x, inPos[1].y, outPos[1].x, outPos[1].y, GetColor(255, 0, 0), 1); //��
	DrawLine(inPos[2].x, inPos[2].y, outPos[2].x, outPos[2].y, GetColor(0, 0, 255), 1); //��
	DrawLine(inPos[3].x, inPos[3].y, outPos[3].x, outPos[3].y, GetColor(255, 255, 0), 1); //���F
	DrawLine(inPos[4].x, inPos[4].y, outPos[4].x, outPos[4].y, GetColor(255, 0, 255), 1); //��
	DrawLine(inPos[5].x, inPos[5].y, outPos[5].x, outPos[5].y, GetColor(0, 255, 255), 1); //���F
	DrawLine(inPos[6].x, inPos[6].y, outPos[6].x, outPos[6].y, GetColor(255, 165, 0), 1); //�I�����W
	DrawLine(inPos[7].x, inPos[7].y, outPos[7].x, outPos[7].y, GetColor(255, 255, 255), 1); //��

	DrawBox(meterPos.x, meterPos.y, meterPos.x + meterLen, meterPos.y + 20, GetColor(0, 255, 0), 1);
	Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, Vector2(spherePos.x * meterLen / stageLen + meterPos.x, meterPos.y), Vector2(32.0f, 32.0f), Vector2::One, 1.0f, turn);

	DrawLine(spherePos.x, spherePos.y, v1.x, v1.y, GetColor(0, 255, 0), 1);
	DrawLine(fx, fy, v2.x, v2.y, GetColor(0, 255, 0), 1);

	//��������Double
	Sprite::GetInstance().Draw(SPRITE_ID::HITO_SPRITE, doublePos, Vector2(16, 32), Vector2::One, rot2 + lineRot[2]);
	DrawCircle(doublePos.x, doublePos.y, (int)r, GetColor(255, 255, 255), 0, 1);
	DrawLine(spherePos.x, spherePos.y, doublePos.x, doublePos.y, GetColor(239, 117, 188), 1); //�s���N
	//DrawCircle(fx, fy, 16, GetColor(255, 0, 0), 0, 1); //�x�_�ɉ~��\��

}

bool EndingScene::IsEnd() const
{
	return isEnd_;
}

Scene EndingScene::Next() const
{
	return nextScene_;
}

void EndingScene::End()
{
	// ������
	world_->Clear();
}

void EndingScene::handleMessage(EventMessage message, void * param)
{
}

void EndingScene::Pendulum()
{
	//A�{�^���ŉ��������ɒ���
	float a;
	if (rotDirection) a = rot + 90.0f;
	else a = rot - 90.0f;
	v1 = Vector2(spherePos.x + MathHelper::Cos(a) * ((neckLen[vec] - r) * MathHelper::Abs(rot_spd)) / 2.0f, spherePos.y + MathHelper::Sin(a) * ((neckLen[vec] - r) * MathHelper::Abs(rot_spd)) / 2.0f);
	v2 = Vector2(fx + MathHelper::Cos(a) * ((neckLen[vec] - r) * MathHelper::Abs(rot_spd)) / 2.0f, fy + MathHelper::Sin(a) * ((neckLen[vec] - r) * MathHelper::Abs(rot_spd)) / 2.0f);
	if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM1))
	{
		spherePos += Vector2((v1.x - spherePos.x) * 0.1f, (v1.y - spherePos.y) * 0.1f + f);
		fx += (v2 - Vector2(fx, fy)).x * 0.1f;
		fy += (v2 - Vector2(fx, fy)).y * 0.1f + f;
		f += 0.5f;
		if (v1.y > 0 || v2.y > 0) rot_spd = -rot_spd;
	}
	if (GamePad::GetInstance().ButtonTriggerUp(PADBUTTON::NUM1)) f = 1.0f;
	if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM1)) return;

	//�U��q
	//���݂̏d��̈ʒu
	auto px = fx + MathHelper::Cos(rot) * length;
	auto py = fy + MathHelper::Sin(rot) * length;

	//�d�͈ړ��ʂ𔽉f�����d��̈ʒu
	auto vx = px - fx;
	auto vy = py - fy;
	auto t = -(vy * g) / (vx * vx + vy * vy);
	auto gx = px + t * vx;
	auto gy = py + g + t * vy;

	//�Q�̏d��̈ʒu�̊m�x��
	auto rDiff = MathHelper::ATan(gy - fy, gx - fx);

	//�p�x�����p���x�ɉ��Z
	auto sub = rDiff - rot;
	sub -= std::floor(sub / 360.0f) * 360.0f;
	if (sub < -180.0f) sub += 360.0f;
	if (sub > 180.0f) sub -= 360.0f;
	rot_spd += sub;

	//���C
	rot_spd *= friction;

	//�p�x�Ɋp���x�����Z
	rot += rot_spd;

	//�V�����d��̈ʒu
	px = fx + MathHelper::Cos(rot) * length;
	py = fy + MathHelper::Sin(rot) * length;

	//�d��̍��W
	spherePos.x = px;
	spherePos.y = py;

	//�p�x����
	rot2 = rot - 90.0f;

	//��̓����ƊO���̍��W�����߁A�O���̍��W���x�_�ɐݒ肷��
	for (int i = 0; i < 8; i++)
	{
		inPos[i].x = spherePos.x + MathHelper::Cos(lineRot[i] + rot2);
		inPos[i].y = spherePos.y + MathHelper::Sin(lineRot[i] + rot2);
		outPos[i].x = inPos[i].x + MathHelper::Cos(lineRot[i] + rot2) * neckLen[i];
		outPos[i].y = inPos[i].y + MathHelper::Sin(lineRot[i] + rot2) * neckLen[i];
		fulcrum[i] = outPos[i];
	}

	//1�t���[���O��rot�Ɣ�r
	r2 = rot;
	if (r1 < r2) //�E�ɉ���Ă����
	{
		rotDirection = true;
		r1 = rot;
	}
	else if (r1 > r2) //���ɉ���Ă����
	{
		rotDirection = false;
		r1 = rot;
	}

	//circle�̓����蔻��
	Vector2 absA = spherePos - arrowPos;
	float distanceS = sqrt((absA.x * absA.x) + (absA.y * absA.y));
	if (distanceS < 32.0f ||
		(rot_spd < 0 && !rotDirection && (Keyboard::GetInstance().KeyStateDown(KEYCODE::RIGHT) || GamePad::GetInstance().ButtonStateDown(PADBUTTON::RIGHT))) ||
		(rot_spd > 0 && rotDirection && (Keyboard::GetInstance().KeyStateDown(KEYCODE::LEFT) || GamePad::GetInstance().ButtonStateDown(PADBUTTON::LEFT))))
	{
		aAlpha = 0.5f; //circle�ɓ������Ă���Δ�����
		friction = 1.015f; //���C�����炷
		dFriction = 1.015f;
	}
	else
	{
		aAlpha = 1.0f; //circle�ɓ������Ă��Ȃ���Εs����
		friction = 0.985f; //���C��߂�
		dFriction = 0.985f;
	}

	if (spherePos.y < fy)
	{
		friction = 0.985;
	}

	//�X�s�[�h����
	spdLimit = sqrt(2 * g * (MathHelper::Sin(90.0f) * neckLen[vec])) / (neckLen[vec] * 0.02f);
	//spdLimit = 3.0f / (neckLen[vec] * ((neckLen[vec] * 800.0f) * 0.0002f));
	//rot_spd = 3.0f;
	rot_spd = MathHelper::Clamp(rot_spd, -spdLimit, spdLimit);

	//�x�_���ړ�
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::RSHIFT) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2))
	{
		vec += 1;
		if (vec > 7) vec = 0;
		length = neckLen[vec];
		fx = fulcrum[vec].x;
		fy = fulcrum[vec].y;
		lineRot[0] -= 45.0f;
		rot += 45.0f;
		turn = false;
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::LSHIFT) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM3))
	{
		vec -= 1;
		if (vec < 0) vec = 7;
		length = neckLen[vec];
		fx = fulcrum[vec].x;
		fy = fulcrum[vec].y;
		lineRot[0] += 45.0f;
		rot -= 45.0f;
		turn = true;
	}

	//45�x����8�����Ɏ��L�΂�
	for (int i = 1; i < 8; i++)
	{
		lineRot[i] = lineRot[i - 1] + 45.0f; //1�{�ڂ̎��45�x�������Ă���
	}

	//���_�Ő��K��
	if (rot > 270.0f) rot = -90.0f;
	if (rot < -90) rot = 270.0f;

	//LB/RB�ō��E�ǂ��炩�̎��L�΂��A�����Е��̎���k�܂���
	if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM6))
	{
		if (vec == 7)
		{
			if (neckLen[vec - 1] <= r + 20.0f) return; //����ȏ�Z���Ȃ�Ȃ�
			neckLen[0] += 5.0f;
			neckLen[vec - 1] -= 5.0f;
		}
		else
		{
			if ((vec != 0 && neckLen[vec - 1] <= r + 20.0f) || (vec == 0 && neckLen[7] <= r + 20.0f)) return;
			neckLen[vec + 1] += 5.0f;
			if (vec == 0) neckLen[7] -= 5.0f;
			else neckLen[vec - 1] -= 5.0f;
		}
	}
	else if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM5))
	{
		if (vec == 0)
		{
			if (neckLen[vec + 1] <= r + 20.0f) return;
			neckLen[7] += 5.0f;
			neckLen[vec + 1] -= 5.0f;
		}
		else
		{
			if ((vec != 7 && neckLen[vec + 1] <= r + 20.0f) || (vec == 7 && neckLen[0] <= r + 20.0f)) return;
			neckLen[vec - 1] += 5.0f;
			if (vec == 7) neckLen[0] -= 5.0f;
			else neckLen[vec + 1] -= 5.0f;
		}
	}

	if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM4))
	{
		rot_spd += spdLimit;
	}
}

void EndingScene::Double(Vector2 fPos)
{
	//�U��q
	//���݂̏d��̈ʒu

	auto px = fPos.x + MathHelper::Cos(dRot) * length;
	auto py = fPos.y + MathHelper::Sin(dRot) * length;

	//�d�͈ړ��ʂ𔽉f�����d��̈ʒu
	auto vx = px - fPos.x;
	auto vy = py - fPos.y;
	auto t = -(vy * g) / (vx * vx + vy * vy);
	auto gx = px + t * vx;
	auto gy = py + g + t * vy;

	//�Q�̏d��̈ʒu�̊m�x��
	auto rDiff = MathHelper::ATan(gy - fPos.y, gx - fPos.x);

	//�p�x�����p���x�ɉ��Z
	auto sub = rDiff - dRot;
	sub -= std::floor(sub / 360.0f) * 360.0f;
	if (sub < -180.0f) sub += 360.0f;
	if (sub > 180.0f) sub -= 360.0f;
	dRot_spd = dRot_spd + sub;

	//��̐U��q�Ƃ̊p�x�����p���x�ɉ��Z
	auto bind = 0.5f;
	auto dDiff = rot - dRot;
	auto dSub = rDiff - dRot;
	dSub -= std::floor(dSub / 360.0f) * 360.0f;
	if (dSub < -180.0f) dSub += 360.0f;
	if (dSub > 180.0f) dSub -= 360.0f;
	dRot_spd = dRot_spd + (dSub * bind);

	//���C
	//float dFriction;
	//if (doublePos.y < spherePos.y) dFriction = 0.985;
	//else dFriction = friction;
	dRot_spd *= dFriction;

	//�X�s�[�h����
	float dLimit = sqrt(2 * g * (MathHelper::Sin(90.0f) * neckLen[vec])) / (neckLen[vec] * 0.02f);
	dRot_spd = MathHelper::Clamp(dRot_spd, -dLimit, dLimit);

	//�p�x�Ɋp���x�����Z
	dRot += dRot_spd;
	//dRot += rot;

	//�V�����d��̈ʒu
	px = fPos.x + MathHelper::Cos(dRot + rot2) * length;
	py = fPos.y + MathHelper::Sin(dRot + rot2) * length;

	//�d��̍��W
	doublePos.x = px;
	doublePos.y = py;

	//���_�Ő��K��
	if (dRot > 270.0f) dRot = -90.0f;
	if (dRot < -90) dRot = 270.0f;

	//�x�_���ړ�
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::RSHIFT) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2))
	{
		dRot -= 45.0f;
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::LSHIFT) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM3))
	{
		dRot += 45.0f;
	}

	//�p�x����
	//rot2 = rot - 90.0f;

	//��̓����ƊO���̍��W�����߁A�O���̍��W���x�_�ɐݒ肷��
	//for (int i = 0; i < 8; i++)
	//{
	//	inPos[i].x = spherePos.x + MathHelper::Cos(lineRot[i] + rot2);
	//	inPos[i].y = spherePos.y + MathHelper::Sin(lineRot[i] + rot2);
	//	outPos[i].x = inPos[i].x + MathHelper::Cos(lineRot[i] + rot2) * neckLen[i];
	//	outPos[i].y = inPos[i].y + MathHelper::Sin(lineRot[i] + rot2) * neckLen[i];
	//	fulcrum[i] = outPos[i];
	//}

	//1�t���[���O��rot�Ɣ�r
	//r2 = rot;
	//if (r1 < r2) //�E�ɉ���Ă����
	//{
	//	rotDirection = true;
	//	r1 = rot;
	//}
	//else if (r1 > r2) //���ɉ���Ă����
	//{
	//	rotDirection = false;
	//	r1 = rot;
	//}

	//���E�L�[(���E�{�^��)�ŉ�����
	//if ((rot_spd < 0 && !rotDirection && (Keyboard::GetInstance().KeyStateDown(KEYCODE::RIGHT) || GamePad::GetInstance().ButtonStateDown(PADBUTTON::RIGHT))) ||
	//	(rot_spd > 0 && rotDirection && (Keyboard::GetInstance().KeyStateDown(KEYCODE::LEFT) || GamePad::GetInstance().ButtonStateDown(PADBUTTON::LEFT))))
	//{
	//	aAlpha = 0.5f; //circle�ɓ������Ă���Δ�����
	//	friction = 1.015f; //���C�����炷
	//}
	//else
	//{
	//	aAlpha = 1.0f; //circle�ɓ������Ă��Ȃ���Εs����
	//	friction = 0.985f; //���C��߂�
	//}
}