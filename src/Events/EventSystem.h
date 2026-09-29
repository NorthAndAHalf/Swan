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

struct Callback
{
    Callback(std::function<void(const Event&)> _func, uint64_t _id) 
        : func(_func), id(_id) {}

    std::function<void(const Event&)> func;
    uint64_t id;
    uint8_t markedForDeletion = false;
};

class EventSystem {

    friend class EventListener;

public:
    EventSystem();
    ~EventSystem();

    void DispatchQueuedEvents();

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

    // Subscribe and unsubscribe are only called from friend EventListeners, which wraps these in protected functions to abstract away callback ID management
    template<typename T>
        requires std::derived_from<T, Event>
    void Subscribe(std::function<void(const T&)> func, uint32_t* typeId, uint64_t* callbackId)
    {
        uint32_t id = TypeIdentifier::GetId<T>();

        m_PendingSubscriptions[id].push_back(
            Callback(
                [func](const Event& e)
                {
                    func(static_cast<const T&>(e));
                },
                m_NextCallbackId
            ));

        *typeId = id;
        *callbackId = m_NextCallbackId++;
    }
    void Unsubscribe(uint32_t typeId, uint64_t callbackId);

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

    std::unordered_map<uint32_t, std::vector<Callback>> m_CallbackMap;
    std::unordered_map<uint32_t, std::vector<Callback>> m_PendingSubscriptions;
    std::vector<uint32_t> m_DirtyEventTypes;

    uint64_t m_NextCallbackId = 0;

    void SwapQueues();
    void ClearDispatchQueue();
    void FlushPendingSubscriptions();
    void FlushPendingUnsubscribes();
    void ResetCache();
};
