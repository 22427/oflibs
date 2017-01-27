#pragma once

#include <sstream>
#include <vector>
#include <mutex>
#include <map>
#include <thread>
#include "vmath.h"
#include "socket.h"
#include "stru.h"

namespace ofl {

class Target
{
protected:
	unsigned int m_id;
	unsigned int m_frame_no;
public:
	unsigned int id()const {return m_id;}
	unsigned int frame_no()const {return m_frame_no;}
	Target(unsigned int id = 0, unsigned int frame_no = 0)
		:m_id(id),m_frame_no(frame_no)
	{}
	virtual const vec4& position() const = 0;
	virtual void transform(const mat4& T) = 0;
};



class Target3DOF : public Target
{
protected:
	vec4 m_position;

public:
	Target3DOF(unsigned int id = 0, unsigned int frame_no = 0,
			   const vec4& pos =vec4(0)
			   );
	virtual void transform(const mat4& T);

	const vec4& position() const;
};


class Target6DOF : public Target3DOF
{
protected:
	mat4 m_orientation;
public:
	Target6DOF(unsigned int id = 0,
			   unsigned int frame_no = 0,
			   const vec4& pos = vec4(0),
			   const mat4& orientation = mat4(0));
	void transform(const mat4& T);

	const mat4& orientation() const;
};

class TargetFlystick : public Target6DOF
{
protected:
	std::vector<unsigned char> m_button_state;
public:
	enum ButtonState
	{
		released = 0,
		pressed,
	};
	TargetFlystick(unsigned int id = 0,
				   unsigned int frame_no =0 ,
				   std::vector<unsigned char> b = std::vector<unsigned char>(8,0),
				   const vec4& pos = vec4(0),
				   const mat4& orientation = mat4(0));

	ButtonState button(const unsigned int id) const;
};


class TrackingData
{
	std::vector<Target3DOF> m_t3d;
	std::mutex m_t3d_mtx;
	std::map<uint32_t,Target6DOF> m_t6d;
	std::mutex m_t6d_mtx;
	std::map<uint32_t,Target6DOF> m_t6di;
	std::mutex m_t6di_mtx;
	std::map<uint32_t,TargetFlystick> m_tfly;
	std::mutex m_tfly_mtx;

	mat4 m_transformation;
	unsigned int m_curr_frame_no;

	bool m_goon;
	std::mutex m_goon_mtx;

	std::thread m_thread;

	Socket m_sock;

	void lock_all();

	void unlock_all();


	void loop();

public:
	TrackingData(
			const std::string& address,
			const std::string& port);
	~TrackingData();
	void start();

	void stop();

	void setTransformation(const mat4& mat);

	std::vector<Target3DOF> get3ds();
	Target6DOF get6d(const unsigned int id);
	Target6DOF get6di(const unsigned int id);
	Target6DOF getFlystick(const unsigned int id);


};

}
