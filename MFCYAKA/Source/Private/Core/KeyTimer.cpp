#include "stdafx.h"

#include "Core/KeyTimer.h"
using namespace YAKA;

#define DEFAULT_DURATION 50

KeyTimer::KeyTimer( int x, int y )
	: x( x ), y( y ), duration( DEFAULT_DURATION ), timer( 0 )
{

}

KeyTimer::~KeyTimer()
{

}

KeyTimer::KeyTimer( int x, int y, float duration )
	: x( x ), y( y ), duration( duration )
{

}

void KeyTimer::Update( float deltaTime )
{
	this->timer += deltaTime;
}