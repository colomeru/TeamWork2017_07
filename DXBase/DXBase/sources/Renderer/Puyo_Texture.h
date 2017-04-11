#pragma once

#include "../Math/Math.h"
#include <vector>

class Puyo_Texture {
public:
	struct NodeData{
	public:
		NodeData() {}

		NodeData(Vector2 position, Vector2 velocity, Vector2 accelerate, float mass = 1.0f, Vector2 spring = Vector2::One, Vector2 dumping = Vector2::One) {
			position_ = position;
			velocity_ = velocity;
			accelerate_ = accelerate;
			mass_ = mass;
			spring_ = spring;
			dumping_ = dumping;
		}
	public:
		Vector2 get_pos()const { return position_; }
		Vector2 get_vel()const { return velocity_; }
		Vector2 get_acc()const { return accelerate_; }
		float   get_mas()const { return mass_; }
		Vector2 get_spr()const { return spring_; }
		Vector2 get_dmp()const { return dumping_; }

	public:
		Vector2 position_;
		Vector2 velocity_;
		Vector2 accelerate_;
		float mass_;
		Vector2 spring_;
		Vector2 dumping_;


		//Vector2 vec_left_top;
		//Vector2 vec_right_top;
		//Vector2 vec_right_bot;
		//Vector2 vec_left_bot;
	};
public:
	Puyo_Texture();
	Puyo_Texture(int id, int column = 1, int row = 1);
	NodeData get_node_data(int x, int y);
private:
	void divide_size();
	void divide_texture();
	void init_node_data();
public:
	void trans_node_pos();
	void calc_pos();
	void calc_acc();
	void draw() const;
private:
	Vector2 position_;
	Vector2 origin_;
	Vector2 velocity_;
	Vector2 accelerate_;
	int id_;
	int column_;
	int row_;
	int size_x, size_y;
	Vector2 divide_size_;
	std::vector<std::vector<NodeData>> node_datas_;

	//•ªŠ„Œã‚ÌidŠi”[—p‘½ŽŸŒ³”z—ñ
	std::vector<std::vector<int>> sprite_ids_;
};