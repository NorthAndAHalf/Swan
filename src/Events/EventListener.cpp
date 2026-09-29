#include "EventListener.h"
#include "../Engine/Engine.h"

EventListener::EventListener()
	: m_EventSystemRef(Engine::Events())
{
}

EventListener::~EventListener()
{
	for (CallbackToken& cb : m_CallbackTokens)
	{
		m_EventSystemRef.Unsubscribe(cb.eventTypeId, cb.callbackId);
	}
}
