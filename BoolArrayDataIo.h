#ifndef _LIAN_BOOL_ARRAY_DATA
#define _LIAN_BOOL_ARRAY_DATA

#include "DataIo.h"

class BoolArrayDataIo: DataIo
{
public:
	BoolArrayDataIo(const char* s, bool write=false, const bool* data=0, int n=0);
	~BoolArrayDataIo();

protected:
	void read();
	void write();	

public:
	bool* data;
	int n;
};

#endif