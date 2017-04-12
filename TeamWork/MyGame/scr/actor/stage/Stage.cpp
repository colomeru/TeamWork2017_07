#include "Stage.h"
#include "../../graphic/Model.h"

Stage::Stage(IWorld * world)
	:Actor(world)
{
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
}

Stage::~Stage()
{
}

void Stage::Update()
{
}

void Stage::Draw() const
{
	Model::GetInstance().Draw(MODEL_ID::PLANE_MODEL, Vector3(0.0f, -10.0f, 0.0f));
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
