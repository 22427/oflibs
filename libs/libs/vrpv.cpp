#include "vrpv.h"

namespace vrpv {


////// Screen //////////////////////////////////////////////////////////////////

vec4 Screen::corner(const int i) const
{
	return m_wall_space * m_corners[i];
}

Screen::Screen(const vec4 blc, const vec4 brc, const vec4 &tlc)
{

	m_corners[BL] = blc;
	m_corners[BR] = brc;
	m_corners[TL] = tlc;

	vec4 x = normalize(m_corners[BR]-m_corners[BL]);
	vec4 y = normalize(m_corners[TL]-m_corners[BL]);
	vec4 z = cross(x,y);


	m_wall_space = mat4(x,y,z,vec4(0,0,0,1));
	m_inv_wall_space = inverse(m_wall_space);

	for(int i = 0 ; i<3;i++)
	{
		m_corners[i] = m_inv_wall_space*m_corners[i];
	}
}

void Screen::calculate_projection_and_view(
		const vec4 &eye,
		const float near,
		const float far,
		mat4 &projection_matrix,
		mat4 &view_matrix) const
{

	vec4 corner[3];

	// eye to wallspace
	vec4 eyepos_wall= m_inv_wall_space * eye;

	// calculate the frustum
	for (int i=0; i<3; i++)
	{
		float f;

		corner[i]= m_corners[i] - eyepos_wall;
		f= near / corner[i][2];
		corner[i][0] *= f;
		corner[i][1] *= f;
	}

	// generate projection matrix
	projection_matrix = frustum(
				corner[0][0],
			corner[1][0],
			corner[0][1],
			corner[2][1], near, far);

	// generate view matrix
	view_matrix =   translate(m_wall_space,eye*-1.0f);
}

////// ScreenArrangement ///////////////////////////////////////////////////////

Screen *ScreenArrangement::addScreen(const Screen scrn, const std::string &name)
{
	m_screens.push_back(scrn);
	m_by_name[name] = &m_screens.back();
	return &m_screens.back();
}

const Screen *ScreenArrangement::getScreen(const int id) const
{
	if(id >= (int)m_screens.size())
		return nullptr;
	return &m_screens[id];
}

const Screen *ScreenArrangement::getScreen(const std::string &name)
{
	if(m_by_name.find(name) == m_by_name.end())
		return nullptr;
	return m_by_name[name];
}

unsigned int ScreenArrangement::countScreens() const {return m_screens.size();}

bool ScreenArrangement::loadScreens(const std::string &path)
{
	std::ifstream file (path);
	if(!file.is_open())
		return false;

	std::string line;
	while(std::getline(file,line))
	{
		stru::trim(line);
		if(line.empty()|| line.at(0)=='#') continue;

		vec4 bl,br,tl;
		std::string name = line.substr(0,line.find(' '));
		line = line.substr(line.find(' ')+1);

		bl = read_from_string(line);
		br = read_from_string(line);
		tl = read_from_string(line);

		this->addScreen(Screen(bl,br,tl),name);
	}

	file.close();
	return true;
}

bool ScreenArrangement::saveScreens(const std::string &path)
{
	std::ofstream file (path);
	if(!file.is_open())
		return false;

	for(const std::pair<std::string,Screen*>& sp : m_by_name)
	{
		const Screen& s = *(sp.second);
		file<<sp.first;
		for(int i =0  ; i< 3; i++)
			file<<" "<<to_string(s.corner(i));
		file<<"\n";
	}
	file.close();
	return true;
}
}
