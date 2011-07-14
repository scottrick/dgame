#include "UIEffect.h"

#include "Block.h"
#include "Effect.h"

UIEffect::UIEffect()
{
	Init();
}

UIEffect::~UIEffect()
{
	SetEffect(0);
}

void UIEffect::FromBlock(Block *pBlock)
{

}

void UIEffect::Init()
{
	m_pEffect = 0;
}

void UIEffect::Refresh(const float &fDeltaTime)
{
	if (m_pEffect)
	{
		m_pEffect->Refresh(fDeltaTime);
	}
}

void UIEffect::Render()
{
	if (m_pEffect)
	{
		m_pEffect->Render();
	}
}

void UIEffect::SetEffect(Effect *pEffect)
{
	if (m_pEffect != pEffect)
	{
		if (m_pEffect)
		{
			m_pEffect->Release();
			m_pEffect = 0;
		}

		if (pEffect)
		{
			m_pEffect = pEffect;
			m_pEffect->AddRef();
		}
	}
}

Block *UIEffect::ToBlock()
{
	return 0;
}