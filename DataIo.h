#ifndef _LIAN_DATA_IO
#define _LIAN_DATA_IO

#include "stdio.h"


class DataIo
{
public:
	DataIo(const char* s, bool write){
		writing = write; 
		filename = s;
	}

	void errorInfo(){
		printf("error occured in %s when ", filename);
		if(writing)
			printf("writing\n");
		else
			printf("reading\n");
	}

protected:
	virtual void read() = 0;
	virtual void write() = 0;	
	const char* filename;
	bool writing;
};

#endif
