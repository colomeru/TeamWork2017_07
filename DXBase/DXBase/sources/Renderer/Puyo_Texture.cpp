#include "Puyo_Texture.h"

const float KINETICE_MIN = 0.00001f;

Puyo_Texture::Puyo_Texture():
	id_(-1), column_(0), row_(0), divide_size_(Vector2::Zero){
}

Puyo_Texture::Puyo_Texture(int id, int column, int row):
id_(id), column_(column), row_(row), divide_size_(Vector2::Zero){

	GetGraphSize(id_, &size_x, &size_y);
	Vector2 size = Vector2(size_x, size_y);

	//分割した際の個々の画像サイズ
	divide_size_ = Vector2(size.x / row_, size.y / column_);

	//divide_size();
	divide_texture();
	init_node_data();
}

Puyo_Texture::NodeData Puyo_Texture::get_node_data(int x, int y){
	return node_datas_[x][y];
}

void Puyo_Texture::divide_size(){
	GetGraphSize(id_, &size_x, &size_y);
	Vector2 size = Vector2(size_x, size_y);

	//分割した際の個々の画像サイズ
	divide_size_ = Vector2(size.x / row_, size.y / column_);
}

void Puyo_Texture::divide_texture(){
	//配列の確保
	sprite_ids_ = std::vector<std::vector<int>>(column_, std::vector<int>(row_));
	//分割後のidを格納
	for (int i = 0; i < column_; ++i) {
		for (int j = 0; j < row_; ++j) {
			sprite_ids_[i][j] = DerivationGraph(divide_size_.x * j, divide_size_.y * i, divide_size_.x, divide_size_.y, id_);
		}
	}
}

void Puyo_Texture::init_node_data(){
	//配列の確保
	node_datas_ = std::vector<std::vector<NodeData>>(column_, std::vector<NodeData>(row_));
	//分割後のidを格納
	for (int i = 0; i < column_; ++i) {
		for (int j = 0; j < row_; ++j) {
			node_datas_[i][j] = NodeData(Vector2(divide_size_.x * j, divide_size_.y * i), Vector2::Zero, Vector2::Zero);
		}
	}
}

void Puyo_Texture::trans_node_pos(){
	node_datas_[3][3] = NodeData(node_datas_[3][3].get_pos() + Vector2(10,10), node_datas_[3][3].get_vel(), node_datas_[3][3].get_acc());

}

void Puyo_Texture::calc_pos(){
	for (int i = 0; i < column_; ++i) {
		for (int j = 0; j < row_; ++j) {

			NodeData* main = &node_datas_[i][j];

			//速度
			//node_datas_[i][j] = NodeData(main.get_pos(), main.get_vel() + main.get_acc(), main.get_acc());

			main->velocity_ += main->accelerate_;

			Vector2 energy = Vector2(std::pow(main->get_vel().x, 2), std::pow(main->get_vel().y, 2)) * main->get_mas() * 0.5f;

			if (energy.x < KINETICE_MIN) main->velocity_.x = 0; main->accelerate_.x = 0;
			if (energy.y < KINETICE_MIN) main->velocity_.y = 0; main->accelerate_.y = 0;

			//座標
			//node_datas_[i][j] = NodeData(main.get_pos() + main.get_vel(), main.get_vel(), main.get_acc());

			main->position_ += main->velocity_;
		}
	}
}

void Puyo_Texture::calc_acc() {
	for (int i = 0; i < column_; ++i) {
		for (int j = 0; j < row_; ++j) {

			NodeData main = node_datas_[i][j];

			NodeData left = NodeData();
			NodeData right = NodeData();
			NodeData upper = NodeData();
			NodeData lower = NodeData();
			
			if (j == 0) left = node_datas_[i][j];
			else left = node_datas_[i][j - 1];

			if(j == 9)right = node_datas_[i][j];
			else right = node_datas_[i][j + 1];

			if (i == 0) upper = node_datas_[i][j];
			else upper = node_datas_[i - 1][j];

			if (i == 9)lower = node_datas_[i][j];
			else lower = node_datas_[i + 1][j];

			float dump = left.get_dmp().x + right.get_dmp().x + upper.get_dmp().y + lower.get_dmp().y;

			//拘束無し
			Vector2 f = Vector2::Zero;
			f.x = left.get_spr().x  * (main.get_pos().x - left.get_pos().x  - divide_size_.x)
				+ right.get_spr().x * (main.get_pos().x - right.get_pos().x + divide_size_.x)
				+ upper.get_spr().y * (main.get_pos().x - upper.get_pos().x)
				+ lower.get_spr().y * (main.get_pos().x - lower.get_pos().x)
				+ dump * main.get_vel().x;
			f.y = left.get_spr().x  * (main.get_pos().y - left.get_pos().y)
				+ right.get_spr().x * (main.get_pos().y - right.get_pos().y)
				+ upper.get_spr().y * (main.get_pos().y - upper.get_pos().y - divide_size_.y)
				+ lower.get_spr().y * (main.get_pos().y - lower.get_pos().y + divide_size_.y)
				+ dump * main.get_vel().y;


			//拘束有り
			//f = Vector2::Zero;
			accelerate_ = -f / main.get_mas();
			node_datas_[i][j] = NodeData(main.get_pos(), main.get_vel(), accelerate_);
		}
	}
}

void Puyo_Texture::draw() const {
	for (int i = 0; i < column_ - 1; ++i) {
		for (int j = 0; j < row_ - 1; ++j) {

			Vector2 left_top = node_datas_[i][j].get_pos();
			Vector2 right_top = node_datas_[i][j + 1].get_pos();
			Vector2 right_bot = node_datas_[i + 1][j + 1].get_pos();
			Vector2 left_bot = node_datas_[i + 1][j].get_pos();
			DrawModiGraph(left_top.x, left_top.y, right_top.x, right_top.y, right_bot.x, right_bot.y, left_bot.x, left_bot.y, sprite_ids_[i][j], TRUE);
		}
	}
}

/**** 変数 ************************************************/
//int texHandle;
//int size_x, size_y;
//Vector2 div_size_;
//Vector2 vib_center_;	//振動中心
//Vector2 position_;		//座標
//float time_;						//経過時間
//float angular_freq_;				//角振動数
//float amplitude_;					//振幅
//Vector2 phase_;						//位相
//
//float spring_constant_;				//ばね定数
//float attenuation_constant_;		//減衰定数
//float natural_length_;				//自然長
//float mass_;						//質量
//float p_;						//質量
//float e_;						//質量
//
//Vector2 velocity_;
//
//Vector2 accelarate_;
//
//Vector2 vec_left_top;
//Vector2 vec_right_top;
//Vector2 vec_right_bot;
//Vector2 vec_left_bot;
//
////分割後のid格納用多次元配列
//std::vector<std::vector<int>> sprites_;
//
//int column_ = 80;
//int row_ = 80;
//
//int ModelHandle;
//int GradTexHandle;

/****** init ****************************************************************/
//texHandle = LoadGraph("resources/sprite/001.jpg");
//GetGraphSize(texHandle, &size_x, &size_y);

////分割した際の個々の画像サイズ
//div_size_ = Vector2(size_x / row_, size_y / column_);

////配列の確保
//sprites_ = std::vector<std::vector<int>>(column_, std::vector<int>(row_));
////分割後のidを格納
//for (int i = 0; i < column_; ++i) {
//	for (int j = 0; j < row_; ++j) {
//		float src_x = size_x / row_ * j;
//		float src_y = size_y / column_ * i;
//		sprites_[i][j] = DerivationGraph(src_x, src_y, div_size_.x, div_size_.y, texHandle);
//	}
//}

//vib_center_ = div_size_ * 2;

//position_.x = div_size_.x * 4;
//position_.y = div_size_.y * 2;

//velocity_ = Vector2::Zero;
//accelarate_ = Vector2::Zero;

//natural_length_ = div_size_.x * 2;


//spring_constant_ = 10.0f;
//attenuation_constant_ = 0.5f;
//mass_ = 0.3f;
//angular_freq_ = std::sqrtf(spring_constant_ / mass_);
//p_ = attenuation_constant_ / 2 / mass_;
//e_ = 2.7f;
//time_ = 0;

//amplitude_ = div_size_.x * 3;

//phase_ = position_;

//puyo_ = Puyo_Texture(texHandle, 10, 10);


/**** update ***************************************/
//position_.x = div_size_.x * 4 + velocity_.x;
//position_.y = div_size_.y * 2 + velocity_.y;
//velocity_.x = amplitude_ * angular_freq_ * std::cos(angular_freq_ * time_ + phase_.x);
//velocity_.y = amplitude_ * angular_freq_ * std::cos(angular_freq_ * time_ + phase_.y);


//position_.x = div_size_.x * 4 + amplitude_ * std::pow(e_ ,-p_ * time_) * std::sin(angular_freq_ * time_ + phase_.x);
//position_.y = div_size_.y * 2 + amplitude_ * std::pow(e_, -p_ * time_) * std::sin(angular_freq_ * time_ + phase_.y);


//position_.x += velocity_.x * time_;
////position_y_ += velocity_y_ * time_;

//velocity_.x += accelarate_.x * time_;
////velocity_y_ += accelarate_y_ * time_;

//accelarate_.x = -spring_constant_ * ((position_.x - vib_center_.x) - natural_length_);
////accelarate_y_ = 0;

//DrawGraph(position_.x, position_.y, sprites_[2][2], FALSE);
////DrawGraph(vib_center_.x, vib_center_.y, sprites_[2][2], FALSE);

//DrawFormatString(550, 150, GetColor(255, 255, 255), "%d, %d", (int)position_.x, (int)position_.y);


//puyo_.calc_acc();
//puyo_.calc_pos();

//if (InputMgr::GetInstance().IsKeyDown(KeyCode::SPACE)) puyo_.trans_node_pos();

//DrawFormatString(550, 150, GetColor(255, 255, 255), "%d, %d", (int)(puyo_.get_node_data(3, 3).get_pos().x), (int)(puyo_.get_node_data(3, 3).get_pos().y));
//DrawFormatString(550, 175, GetColor(255, 255, 255), "%d, %d", (int)(puyo_.get_node_data(3, 3).get_acc().x), (int)(puyo_.get_node_data(3, 3).get_acc().y));
//DrawFormatString(550, 200, GetColor(255, 255, 255), "%d, %d", (int)(puyo_.get_node_data(2, 2).get_acc().x), (int)(puyo_.get_node_data(2, 2).get_acc().y));

/**** draw ***************************/
//puyo_.draw();
