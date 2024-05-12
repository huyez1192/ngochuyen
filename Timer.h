#pragma once

class Timer
{
public:
	Timer();
	~Timer();

	void start();
	void stop();// het over

	int getTicks();

	bool is_started() {return is_started_;}
	bool is_paused() {return is_paused_;}

private:
	int start_tick_;
	int paused_tick_;//thoi diem paused
	bool is_paused_;
	bool is_started_;
};
