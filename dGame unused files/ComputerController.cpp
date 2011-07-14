#include "ComputerController.h"

#include "FiringPlugin.h"
#include "MovementPlugin.h"

ComputerController::ComputerController()
{
	Init();
}

ComputerController::~ComputerController()
{
	SetFiringPlugin(0);
	SetMovementPlugin(0);
}

void ComputerController::FromBlock(Block *pBlock)
{
	
}

void ComputerController::Init()
{
	m_pFiringPlugin		= 0;
	m_pMovementPlugin	= 0;
}

void ComputerController::Print(int indent) const
{
	
}

void ComputerController::Refresh(int dwDeltaTime)
{
	
}

void ComputerController::SetFiringPlugin(FiringPlugin *pPlugin)
{
	if (m_pFiringPlugin != pPlugin)
	{
		if (m_pFiringPlugin)
		{
			m_pFiringPlugin->Release();
			m_pFiringPlugin = 0;
		}

		if (pPlugin)
		{
			m_pFiringPlugin = pPlugin;
			m_pFiringPlugin->AddRef();
		}
	}
}

void ComputerController::SetMovementPlugin(MovementPlugin *pPlugin)
{
	if (m_pMovementPlugin != pPlugin)
	{
		if (m_pMovementPlugin)
		{
			m_pMovementPlugin->Release();
			m_pMovementPlugin = 0;
		}

		if (pPlugin)
		{
			m_pMovementPlugin = pPlugin;
			m_pMovementPlugin->AddRef();
		}
	}
}

Block *ComputerController::ToBlock()
{
	return 0;
}	