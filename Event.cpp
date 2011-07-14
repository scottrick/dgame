#include "Event.h"

Event::Event()
{
	Init();
}

Event::~Event()
{

}

void Event::Init()
{
	m_fTimeTillExecution = 0.0f;
}

void Event::Update(float fDeltaTime)
{
	m_fTimeTillExecution -= fDeltaTime;

	if (m_fTimeTillExecution <= 0.0f)
	{
		Execute();
	}	
}