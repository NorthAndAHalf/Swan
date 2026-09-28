#pragma once

#include "Core.h"
#include <algorithm>
#include <vector>
#include <functional>
#include "Events/Event.h"
#include <atomic>
#include <array>
#include <type_traits>
#include "Memory/LinearBuffer.h"
#include "spdlog/spdlog.h"
#include <concepts>

#define SW_BIND_CALLBACK(type, func) std::bind(&type::func, this, std::placeholders::_1)   

constexpr uint32_t EVENT_QUEUE_COUNT = 5000;

// Static counter will not work cross DLL, so will need to be refactored if cross DLL compilation becomes required
class TypeIdentifier 
{
public:
    template<typename T>
    static uint32_t GetId() {
        static const uint32_t id = m_Counter++;
        return id;
    }
private:
    inline static std::atomic<uint32_t> m_Counter{ 1 };
};

class EventSystem {
public:
    EventSystem();
    ~EventSystem();

    void DispatchQueuedEvents();

    template<typename T>
    requires std::derived_from<T, Event>
    void Subscribe(std::function<void(const T&)> func)
    {
        uint32_t id = TypeIdentifier::GetId<T>();

        m_CallbackMap[id].push_back(
            [func](const Event& e)
            {
                func(static_cast<const T&>(e));
            });
    }

    // Need to implement an ID system to support subscriptions
    template<typename T>
        requires std::derived_from<T, Event>
    void Unsubscribe(std::function<void(const T&)> func)
    {
        spdlog::error("Event unsubscriptions are not yet supported");
    }

    template<typename T, typename... Args>
    void FireEvent(Args&&... args)
    {
        if (m_InputQueueHead >= EVENT_QUEUE_COUNT)
        {
            spdlog::error("Event queue overflow");
            return;
        }

        void* address = m_InputBuffer->allocate(sizeof(T), alignof(T));
        if (!address)
        {
            spdlog::error("Event Buffer out of memory!");
            return;
        }

        T* e = new (address) T(std::forward<Args>(args)...);
        e->typeId = TypeIdentifier::GetId<T>();
        m_InputQueue[m_InputQueueHead] = e;
        m_InputQueueHead++;
    }

private:
    void NotifySubscribers(Event& e);

private:

    LinearBuffer m_EventBuffer1{ 1024 * 64 }; // 64KB
    LinearBuffer m_EventBuffer2{ 1024 * 64 }; // 64KB

    Event** m_EventQueue1 = nullptr;
    Event** m_EventQueue2 = nullptr;

    uint32_t m_InputQueueHead = 0;
    Event** m_InputQueue;
    LinearBuffer* m_InputBuffer;

    uint32_t m_DispatchQueueHead = 0;
    Event** m_DispatchQueue;  
    LinearBuffer* m_DispatchBuffer;

    std::unordered_map<uint32_t, std::vector<std::function<void(const Event&)>>> m_CallbackMap;

    void SwapQueues();
    void ClearDispatchQueue();
};
