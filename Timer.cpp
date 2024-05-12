#include "Timer.h"
#include "CommonFunc.h"
Timer::Timer()
{
	start_tick_ = 0;
	paused_tick_ = 0;
	is_paused_ = 0;
	is_started_ = 0;
}

Timer::~Timer()
{

}

void Timer::start()
{
	is_started_ = 1;
	is_paused_ = 0;
	start_tick_ = SDL_GetTicks();
}
void Timer::stop()
{
	is_paused_ = 0;
	is_started_ = 0;
}

int Timer::getTicks()
{
	if(is_started_ == 1)
	{
		if(is_paused_ == 1)
		{
			return paused_tick_;
		}
		else return SDL_GetTicks() - start_tick_;
	}
	return 0;
}
