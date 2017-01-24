#pragma once
#include <cstdint>
#include <cstdlib>
#include <cstring>
namespace ofl
{
	/** a package header is allways!!!! 4*4Bytes long **/
struct PackageHeader
{
	uint32_t size;
	uint32_t type;
	uint32_t other;
	uint32_t stop;
};


class Package
{
public:
	PackageHeader header;
	uint32_t reserved;
	void* payload;

	Package()
	{
		header = {0,0,0,0};
		reserved = 0;
		payload = nullptr;
	}
	Package(const PackageHeader& h)
	{
		header = h;
	}

};

	void* pack(void* data,uint32_t len,uint32_t type, uint32_t* reslen)
	{
		*reslen= len+16;
		char* res = (char*) malloc(*reslen);
		PackageHeader ph ={len,type,0,~(0u)};
		memcpy(res,&ph,16);
		memcpy(res+16,data,len);
		return res;
	}


}
