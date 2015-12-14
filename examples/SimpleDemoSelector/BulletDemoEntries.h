
#ifndef BULLET_DEMO_ENTRIES_H
#define BULLET_DEMO_ENTRIES_H

#include "Bullet3AppSupport/BulletDemoInterface.h"
#include "CoordinateSystemDemo.h"
#include "RenderInstancingDemo.h"



struct DemoEntry
{
	int									m_menuLevel;
	const char*							m_name;
	BulletDemoInterface::CreateFunc*	m_createFunc;
	int									m_scenarioIndex;
};



static DemoEntry allDemoEntries[]=
{

    {0,"Basic Concepts",0},
    {1,"Basis Frame", &CoordinateSystemDemo::CreateFunc},
    {0,"Rendering",0},
    {1,"Instanced Shapes", &RenderInstancingDemo::CreateFunc},
};

#include <stdio.h>


static void saveCurrentDemoEntry(int currentEntry,const char* startFileName)
{
	FILE* f = fopen(startFileName,"w");
	if (f)
	{
		fprintf(f,"%d\n",currentEntry);
		fclose(f);
	}
};

static int loadCurrentDemoEntry(const char* startFileName)
{
	int currentEntry= 0;
	FILE* f = fopen(startFileName,"r");
	if (f)
	{
		int result;
		result = fscanf(f,"%d",&currentEntry);
		if (result)
		{
			return currentEntry;
		}
		fclose(f);
	}
	return 0;
};

#endif//BULLET_DEMO_ENTRIES_H

