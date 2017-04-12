#include "Stage.h"
#include "../../graphic/Model.h"

Stage::Stage(IWorld * world)
	:Actor(world)
{
}

Stage::~Stage()
{
}

void Stage::Update()
{
}

void Stage::Draw() const
{
	Model::GetInstance().Draw(MODEL_ID::PLANE_MODEL, Vector3::Zero);
}

void Stage::OnUpdate()
{
}

void Stage::OnCollide(Actor * other, CollisionParameter colpara)
{
}

void Stage::OnMessage(EventMessage message, void * param)
{
}
