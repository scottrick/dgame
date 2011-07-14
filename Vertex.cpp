#include "Vertex.h"

#include <iostream>
using namespace std;

Vertex::Vertex()
{
	Init();
}

Vertex::~Vertex()
{

}

void Vertex::Init()
{
	m_vPos.x		= 0.0f;
	m_vPos.y		= 0.0f;
	m_vPos.z		= 0.0f;
	m_vNorm.x		= 0.0f;
	m_vNorm.y		= 0.0f;
	m_vNorm.z		= 0.0f; //no default normal
	m_vTex.x		= 0.0f;
	m_vTex.y		= 0.0f;
}

void Vertex::Print() const
{
	printf("vertex (%f, %f, %f)\n", m_vPos.x, m_vPos.y, m_vPos.z);
}