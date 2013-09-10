#include "BoolArrayDataIo.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "globalInclude.h"


BoolArrayDataIo::BoolArrayDataIo(const char* s, bool write, const bool* data, int n):DataIo(s, write)
{
	//DataIo(s, write);
	if(write)
	{
		this->data = new bool[n];
		memcpy(this->data, data, n*sizeof(bool));
		this->n = n;
		this->write();
	}
	else
		read();
}

BoolArrayDataIo::~BoolArrayDataIo()
{
	delete[] data;
	data = 0;
	n = 0;
}

void BoolArrayDataIo::read()
{
	printf("entering\n");
	n = countWords(filename);
	printf("words: %d\n", n);

	FILE* in;
	printf("file: %s\n", filename);
	in = fopen(filename, "r");
	if(!in)
	{
		errorInfo();
		exit(1);
	}
	printf("tags\n");
	data = new bool[n];
	int state;
	for (int i = 0; i < n; i++)
	{
		fscanf(in, "%d", &state);
		if(state == 0)
			data[i] = false;
		else
			data[i] = true;
	}
	fclose(in);
}

void BoolArrayDataIo::write()
{
	FILE* out;
	out = fopen(filename, "w");
	if(!out)
	{
		errorInfo();
		exit(1);
	}
	for(int i = 0; i < n; i ++)
	{
		if(data[i])
			fprintf(out, "1 ");
		else
			fprintf(out, "0 ");
	}
	fclose(out);
}