#include "track.h"
#include "stru.h"

Target3DOF::Target3DOF(unsigned int id, unsigned int frame_no, const vec4 &pos)
	:Target(id, frame_no)
{
	m_position = pos;
}

void Target3DOF::transform(const mat4 &T)
{
	m_position = T*m_position;
}

const vec4 &Target3DOF::position() const
{
	return m_position;
}

Target6DOF::Target6DOF(unsigned int id,
					   unsigned int frame_no,
					   const vec4 &pos,
					   const mat4 &orientation)
	: Target3DOF(id,frame_no,pos)
{
	m_orientation = mat4(orientation);
}

void Target6DOF::transform(const mat4 &T)
{
	m_orientation = T*m_orientation;
	m_position = T*m_position;
}

const mat4 &Target6DOF::orientation() const
{
	return m_orientation;
}

TargetFlystick::TargetFlystick(unsigned int id, unsigned int frame_no,
							   std::vector<unsigned char> b,
							   const vec4 &pos,
							   const mat4 &orientation)
	: Target6DOF(id,frame_no,pos,orientation), m_button_state (b)
{}

TargetFlystick::ButtonState TargetFlystick::button(const unsigned int id) const
{
	return (ButtonState)m_button_state[id];
}

void TrackingData::lock_all()
{
	m_t3d_mtx.lock();
	m_t6d_mtx.lock();
	m_t6di_mtx.lock();
	m_tfly_mtx.lock();
}

void TrackingData::unlock_all()
{
	m_t3d_mtx.unlock();
	m_t6d_mtx.unlock();
	m_t6di_mtx.unlock();
	m_tfly_mtx.unlock();
}



std::vector<float> readARTBlob(stru::Tokenizer& tkn)
{
	std::vector<float> res;
	stru::Tokenizer t(tkn.getToken(']'));
	t.skipOverAll("["+t.whitespaces);

	std::string seperators = ","+t.whitespaces;
	float f;
	while(t.getTokenAs(f,seperators))
	{
		res.push_back(f);
	}
	return res;
}

mat4 mat4_from_array(const std::vector<float> &vec)
{
	mat4 res(1);
	int s = sqrt(vec.size());
	s = std::min(s,4);
	for(int i =0 ; i<s;i++)
	{
		for( int j = 0 ; j <s;j++)
		{
			res[i][j] = vec[i*s+j];
		}
	}
	return res;
}

Target6DOF read6d(stru::Tokenizer &tkn, int frame_no)
{
	std::vector<float> head = readARTBlob(tkn);
	std::vector<float> pos_eul = readARTBlob(tkn);
	mat4 mat = mat4_from_array(readARTBlob(tkn));

	return Target6DOF(head[0],
			frame_no,
			vec4(pos_eul[0],pos_eul[1],pos_eul[2]),
			mat);

}

Target3DOF  read3d(stru::Tokenizer &tkn, int frame_no)
{
	std::vector<float> head = readARTBlob(tkn);
	std::vector<float> pos = readARTBlob(tkn);

	return Target3DOF(head[0],
			frame_no,
			vec4(pos[0],pos[1],pos[2]));
}

TargetFlystick  read6df(stru::Tokenizer &tkn, int frame_no)
{
	std::vector<float> head = readARTBlob(tkn);
	std::vector<float> pos_eul = readARTBlob(tkn);
	mat4 mat = mat4_from_array(readARTBlob(tkn));

	std::vector<unsigned char> buttons;
	uint8_t b = head[2];
	for(unsigned int i =0 ; i<8;i++ )
	{
		buttons.push_back(b&(1<<i));
	}
	return TargetFlystick(head[0],frame_no,buttons,
			vec4(pos_eul[0],pos_eul[1],pos_eul[2]),
			mat);
}


void TrackingData::loop()
{
	m_goon_mtx.lock();

	int buffer_size = 8*1024;
	char* buffer = (char*)malloc(buffer_size);

	while(m_goon)
	{
		m_goon_mtx.unlock();
		int read = m_sock.read_data(buffer,buffer_size);
		if(read < buffer_size)
			buffer[read] = 0;

		std::stringstream strm(buffer);
		std::string l;

		while(std::getline(strm,l))
		{
			stru::Tokenizer line(l);
			std::string lt = line.getToken(' ');
			int cnt = 0;
			line.getTokenAs(cnt," ");

			if(lt == "fr")
			{
				m_curr_frame_no = cnt;
			}
			else if(lt == "6d")
			{
				m_t6d_mtx.lock();
				for (int i = 0 ; i< cnt; i++)
				{
					Target6DOF target = read6d(line,m_curr_frame_no);
					target.transform(m_transformation);
					m_t6d[target.id()] = target;
				}
				m_t6d_mtx.unlock();
			}
			else if(lt == "6di")
			{
				m_t6di_mtx.lock();
				for (int i = 0 ; i< cnt; i++)
				{
					Target6DOF target = read6d(line,m_curr_frame_no);
					target.transform(m_transformation);
					m_t6di[target.id()] = target;
				}
				m_t6di_mtx.unlock();
			}
			else if(lt == "3d")
			{
				m_t6d_mtx.lock();
				m_t3d.clear();
				for (int i = 0 ; i< cnt; i++)
				{
					Target3DOF target = read3d(line,m_curr_frame_no);
					target.transform(m_transformation);
					m_t3d.push_back(target);
				}
				m_t6d_mtx.unlock();

			}
			else if(lt == "6df")
			{
				m_tfly_mtx.lock();
				for (int i = 0 ; i< cnt; i++)
				{
					TargetFlystick target = read6df(line,m_curr_frame_no);
					target.transform(m_transformation);
					m_tfly[target.id()] = target;
				}
				m_tfly_mtx.unlock();
			}
		}

		m_goon_mtx.lock();
	}
	m_goon_mtx.unlock();
}

TrackingData::TrackingData(const std::string &address, const std::string &port)
	:m_sock(address,port,SOCK_DGRAM)
{

	m_goon = false;
	m_curr_frame_no = 0;
	m_sock.bind();
}

TrackingData::~TrackingData()
{
	stop();
	m_sock.close();
}

void TrackingData::start()
{
	m_goon_mtx.lock();
	m_goon = true;
	m_goon_mtx.unlock();
	if(!m_thread.joinable())
		m_thread = std::thread(&TrackingData::loop,this);
}

void TrackingData::stop()
{
	m_goon_mtx.lock();
	m_goon = false;
	m_goon_mtx.unlock();
	m_thread.join();
}

void TrackingData::setTransformation(const mat4 &mat)
{
	lock_all();
	m_transformation = mat;
	unlock_all();
}

std::vector<Target3DOF> TrackingData::get3ds()
{
	std::lock_guard<std::mutex>lock(m_t3d_mtx);
	return m_t3d;
}

Target6DOF TrackingData::get6d(const unsigned int id)
{
	std::lock_guard<std::mutex>lock(m_t6d_mtx);
	return m_t6d[id];
}

Target6DOF TrackingData::get6di(const unsigned int id)
{
	std::lock_guard<std::mutex>lock(m_t6di_mtx);
	return m_t6di[id];
}

Target6DOF TrackingData::getFlystick(const unsigned int id)
{
	std::lock_guard<std::mutex>lock(m_tfly_mtx);
	return m_tfly[id];
}

