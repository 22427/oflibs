
#include <cstdio>
#include <fstream>
#include <set>
#include <cmath>
#include <ofl_processing_graph.h>

using namespace ofl;



class IntSource : public Node<int>
{

	int i;
public:
	IntSource()
	{
		i = 0;
	}
	virtual ~IntSource();
	void doSth(Item<int> &t)
	{
		if(i<10)
		{
			t.payload() = i;
			t.flags() = Item<int>::F_NONE;
			i++;
		}
		else
		{
			kill();
		}
	}
};



class IntPlotter : public Node<int>
{

public:
	virtual ~IntPlotter();
	void doSth(Item<int> &t)
	{
		if(!(t.flags() & Item<int>::F_INVALID))
		{
			printf("got an integer: %d\n",t.payload());
			fflush(stdout);
		}
	}
};


class Sqrt : public TransformNode<int,float>
{

public:

	virtual ~Sqrt();
	void doSth(Item<int> &t)
	{
		Item<float> q;
		this->grab_right(q);
		if(!(q.flags() & Item<int>::F_INVALID) && !(t.flags() & Item<int>::F_INVALID))
		{
			q.payload() = sqrtf(static_cast<float>(t.payload()));
		}
		push_out(q);
	}
};

class FloatPlotter : public Node<float>
{

public:
	virtual ~FloatPlotter();
	void doSth(Item<float> &t)
	{
		if(!(t.flags() & Item<float>::F_INVALID))
		{
			printf("got an float: %f\n",t.payload());
			fflush(stdout);
		}

	}
};

int main(int , char** )
{
	IntSource src;
	IntPlotter plt;
	Sqrt s;
	FloatPlotter fplt;


	plt.getJoint()->linkInBetween(src.getJoint(),s.getLeft());
	s.getRight()->link(fplt.getJoint());

	Item<int> itm;
	src.getJoint()->insert(itm);

	Item<float> flt;
	s.getRight()->insert(flt);

	src.start();
//	plt.start();
//	s.start();
//	fplt.start();

	src.join();
	plt.join();
	s.join();
	fplt.join();
}

#define OFL_IMPLEMENTATION
#include <ofl_processing_graph.h>

IntSource::~IntSource(){}

FloatPlotter::~FloatPlotter(){}

Sqrt::~Sqrt(){}

IntPlotter::~IntPlotter(){}
