#pragma once

#include "CommonFunc.h"
#include "BaseObject.h"


class Character: public BaseObject
{
public:
	Character()
	{
		y_val_ = 0;
		frame_cur_ = 0;
		on_ground_ = 1;
		rect_.x = 100;
		rect_.y = BASE;
		status_ = RUN;
		pre_status_ = RUN;
		up = 0;
	}
	~Character(){;}

	enum STATUS
	{
		RUN = 0,
		JUMP = 1
	};
	void Reset()
	{
		Character tmp;
		*this = tmp;
	}
	void Show(SDL_Renderer* des)
	{
		if(pre_status_ != status_) frame_cur_ = 0;
		else frame_cur_++;
		pre_status_ = status_;
//		cout<<status_<<"\n";
		//cout<<frame_cur_<<"\n";
		string path;
		if(status_ == RUN)
		{
			if(frame_cur_ == 5*6) frame_cur_ = 0;
			path = "character/dog-" + to_string(frame_cur_/5) + ".png";
		}
		else if(status_ == JUMP)
		{
			path = "character/dog-4.png";
		}

		LoadImg(path, des);

		SDL_Rect renderQuad = {rect_.x, rect_.y, rect_.w, rect_.h};

		SDL_RenderCopyEx(des, p_object_, NULL, &renderQuad, 0, NULL, SDL_FLIP_NONE);
	}
	void HandleInputAction(SDL_Event e)
	{
		if(e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_SPACE:
				up = 1;
				break;

			default:
				break;
			}
		}

	}
	void Logic()
	{
		if(on_ground_ == 0) status_ = JUMP;
		else status_ = RUN;
		y_val_ += GRAV;
		if(y_val_ >= CAP_SPEED)
		{
			y_val_ = CAP_SPEED;
		}
//		if(y_val_ < -3 && !up )
//		{
//			y_val_ = -3;
//		}
		if(up == 1)
		{
			if(on_ground_ == 1)
			{
				y_val_ += -JUMP_SPEED;
			}

			on_ground_ = 0;
			up = 0;
		}
		rect_.y += y_val_;
		if(rect_.y > BASE)
		{
			rect_.y = BASE;
			y_val_ = 0;
			on_ground_ = 1;
		}
		else on_ground_ = 0;
	}
// private
	float y_val_;
	int status_;
	int pre_status_;
	int frame_cur_;
	bool on_ground_;
	bool up;
};

