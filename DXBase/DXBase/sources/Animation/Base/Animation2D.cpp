#include "Animation2D.h"

using namespace std;

Animation2D::Animation2D(const ActionType& type) :
	id_(-1), frame_(0), timer_(0),
	curr_anim_(-1), norm_anim_(-1), turn_anim_(-1), 
	curr_speed_(1), norm_speed_(1), turn_speed_(2),
	type_(type), type_stock_(ActionType::Right) {
}

Animation2D::~Animation2D()
{
}


void Animation2D::add_anim(const int & id, const std::vector<int>& anims) {
	sprites_[id] = anims;
}

void Animation2D::change_param(const int& anim_num, const float& speed) {
	//再生速度を変更
	norm_speed_ = speed;
	if (turn_anim_ == -1)curr_speed_ = norm_speed_;
	//アニメーションが同じ場合はreturn
	if (norm_anim_ == anim_num)return;
	//指定したアニメーションに変更
	norm_anim_ = anim_num;
	if (turn_anim_ == -1)curr_anim_ = norm_anim_;
	//再生時間をリセット
	timer_ = 0;
}

void Animation2D::change_dir_type(const int& anim_num, const ActionType& type) {
	if (type_stock_ == type)return;
	if (turn_anim_ != -1)return;
	turn_anim_ = anim_num;
	curr_anim_ = turn_anim_;
	turn_speed_ = 2.0f;
	curr_speed_ = turn_speed_;
	type_stock_ = type;
}

void Animation2D::back_to_pre_motion() {
	if (turn_anim_ != -1 && end_anim()) {
		turn_anim_ = -1;
		curr_anim_ = norm_anim_;
		curr_speed_ = norm_speed_;
		type_ = type_stock_;
	}
}

bool Animation2D::end_anim() {
	return frame_ >= sprites_[curr_anim_].size() - 2;
}

void Animation2D::update(float deltaTime) {
	back_to_pre_motion();

	frame_ = static_cast<int>(timer_) % sprites_[curr_anim_].size();
	id_ = sprites_[curr_anim_][frame_];
	//更新処理
	timer_ += deltaTime * curr_speed_ * 60.0f / sprites_[curr_anim_].size() * 10;

}

void Animation2D::draw(const Vector2& position, const Vector2& origin, const float& scale, const float& degree, const Vector3& color) const {
	draw(position, origin, Vector2::One * scale, degree, color);
}

void Animation2D::draw(const Vector2& position, const Vector2& origin, const Vector2& scale, const float& degree, const Vector3& color) const {
	SetDrawBright(static_cast<int>(color.x), static_cast<int>(color.y), static_cast<int>(color.z));
	//度数法→弧度法に変換
	float radian = MathHelper::ToRadians(degree);
	if (type_ == ActionType::Right)DrawRotaGraph3(static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(origin.x), static_cast<int>(origin.y), scale.x, scale.y, radian, id_, TRUE, FALSE);
	if (type_ == ActionType::Left) DrawRotaGraph3(static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(origin.x), static_cast<int>(origin.y), scale.x, scale.y, radian, id_, TRUE, TRUE);
	SetDrawBright(255, 255, 255);
}

void Animation2D::drawTurn(const Vector2 & position, const Vector2 & origin, const float & scale, const float & degree, const Vector3 & color, bool reversal) const
{
	SetDrawBright(static_cast<int>(color.x), static_cast<int>(color.y), static_cast<int>(color.z));
	//度数法→弧度法に変換
	float radian = MathHelper::ToRadians(degree);
	DrawRotaGraph3(static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(origin.x), static_cast<int>(origin.y), scale, scale, radian, id_, TRUE, reversal);
	SetDrawBright(255, 255, 255);

}


