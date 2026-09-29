#pragma once

#include <concepts>
#include "Event.h"
#include "EventSystem.h"
#include <vector>
#include <functional>
#include <cstdint>

#define SW_EVENT_SUBSCRIBE(EventType, func) SubscribeListener<EventType>([this](const EventType& e) { func(e); })

struct CallbackToken
{
	CallbackToken(uint32_t _eventTypeId, uint64_t _callbackId) : eventTypeId(_eventTypeId), callbackId(_callbackId) {}
	uint32_t eventTypeId;
	uint64_t callbackId;
};

class EventListener
{
public:
	EventListener();
	virtual ~EventListener();

	// Listeners capture `this` in their callbacks, so they must never relocate
	// If they need to be in containers, use a deque, or wrap in a unique pointer
	EventListener(const EventListener&) = delete;
	EventListener& operator=(const EventListener&) = delete;
	EventListener(EventListener&&) = delete;
	EventListener& operator=(EventListener&&) = delete;

protected:
	template<typename T>
		requires std::derived_from<T, Event>
	void SubscribeListener(std::function<void(const T&)> callback)
	{
		uint32_t typeId;
		uint64_t callbackId;

		m_EventSystemRef.Subscribe<T>(std::move(callback), &typeId, &callbackId);
		
		m_CallbackTokens.push_back(CallbackToken(typeId, callbackId));
	}

private:
	EventSystem& m_EventSystemRef;
	std::vector<CallbackToken> m_CallbackTokens;
};