#pragma once

#include <map>
#include <vector>
#include <set>
#include <algorithm>
namespace ofl
{




template<unsigned int D=3, typename T=float>
class kd_node
{
	uint m_item;
	uint m_stride;
	kd_node<D,T>* m_left;
	kd_node<D,T>* m_right;
	uint m_axis;

	T m_value;
	T calc_dist2(const T* from, const T* to)
	{
		T sqd = 0 ;
		for(uint i  = 0 ; i < D;i++)
		{
			const auto q = from[i]-to[i];
			sqd +=  q*q;
		}
		return sqd;
	}
public:
	kd_node(std::vector<uint>& data, const T* base, uint stage, uint stride = D)
		:m_left(nullptr), m_right(nullptr), m_axis(stage)
	{
		m_axis = stage;
		m_stride =  stride;
		// a leaf node!
		if(data.size() == 1)
		{
			m_item = data.back();
			return;
		}
		// find median
		std::sort(data.begin(),data.end(), [base,stage,stride](const uint a, const uint b)
		{
			return base[a*stride+stage] < base[b*stride+stage];
		});

		uint median_index = static_cast<unsigned int>(data.size())/2u;
		T value = base[data[median_index]*m_stride+stage];

		std::vector<uint> left(data.size()/2+data.size()%2);
		std::vector<uint> right(data.size()/2);

		for(const uint& i : data)
		{
			if(base[i*m_stride+stage] <= value)
				left.push_back(i);
			else
				right.push_back(i);
		}


		m_left = new kd_node<D,T>(left,base,(stage+1)%D);
		m_right = new kd_node<D,T>(right,base,(stage+1)%D);
	}



	int find_nearest(const T* to, const T* base,T& dist, int curr)
	{

		bool is_left = to[m_axis] < m_value;

		if(m_left||m_right) // not a leave
		{
			// go left or right
			if(is_left)
				curr = m_left->find_nearest(to,base,dist,curr);
			else
				curr = m_right->find_nearest(to,base,dist,curr);

			// test if we have to check the other side as well
			if(dist >= (to[m_axis]-m_value)*(to[m_axis]-m_value))
			{
				if(!is_left)
					curr =  m_left->find_nearest(to,base,dist,curr);
				else
					curr = m_right->find_nearest(to,base,dist,curr);
			}

		}
		else // a leave
		{
			auto md = calc_dist2(to,base+m_item*m_stride);
			if(md < dist)
			{
				dist = md;
				curr = static_cast<int>(m_item);
			}
		}

		return curr;
	}
};

template<typename A=int,unsigned int D =3,typename T=float>
class kDTree
{
protected:
	kd_node<D,T>* m_root;
	const T* m_base;
	const A* m_attached;
public:
	void clear()
	{
		delete m_root;
	}

	void generate(const T*base,const A* attached, uint cnt,uint stride=D)
	{
		m_base = base;
		m_attached = attached;
		std::vector<uint> v(cnt) ; // vector with 100 ints.
		std::iota (std::begin(v), std::end(v), 0);
		m_root = new kd_node<D,T>(v,base,0,stride);
	}

	A find_nearest(const T* to, T* distance = nullptr) const
	{
		T d;

		int id = m_root->find_nearest(to,m_base,d,-1);
		if(distance)
			*distance = d;
		return m_attached[id];
	}
};



class kDTree3D : public kDTree<int,3,float>
{

public:
	void clear()
	{
		delete m_root;
	}

	void generate(const float* base, uint cnt)
	{
		m_base = base;
		std::vector<uint> v(cnt) ;
		std::iota (std::begin(v), std::end(v), 0);

		m_root = new kd_node<3,float>(v,base,0);
	}

	int find_nearest(const float* to, float* distance = nullptr) const
	{
		float d = -1;
		int id = m_root->find_nearest(to,m_base,d,-1);
		if(distance)
			*distance = d;
		return id;
	}
};

}

