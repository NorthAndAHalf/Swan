#pragma once

#include "Core.h"
#include <algorithm>
#include <vector>
#include <algorithm>
#include "Events/Event.h"
#include <atomic>
#include <array>
#include <type_traits>
#include "Memory/LinearBuffer.h"
#include "spdlog/spdlog.h"

constexpr uint32_t EVENT_QUEUE_COUNT = 5000;

enum class Layer
{
    DEBUG = 0,
    ENGINE = 1,
    UI = 2,
    GAME = 3
};

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

struct EventDelegate
{
    using StubFunc = void(*)(void* instance, Event& event);

    void* instance = nullptr;
    StubFunc stub = nullptr;

    void operator()(Event& e) const { if (stub) stub(instance, e); }
};

class DelegateBucket 
{
public:
    void AddDelegate(uint32_t type, EventDelegate d)
    {
        m_Pool[type].push_back(d);
    }

    void PropagateEvent(Event& e)
    {
        for (EventDelegate& delegate : m_Pool[e.GetTypeId()])
        {
            delegate(e);
        }
    }

private:
    std::unordered_map<uint32_t, std::vector<EventDelegate>> m_Pool;
};

class EventSystem {
public:
    void Init()
    {
        spdlog::info("Allocating event buffers");

        // Prevent memory leak if init is called twice
        if (!m_EventQueue1) m_EventQueue1 = new Event* [EVENT_QUEUE_COUNT];
        if (!m_EventQueue2) m_EventQueue2 = new Event* [EVENT_QUEUE_COUNT];

        m_InputQueue = m_EventQueue1;
        m_DispatchQueue = m_EventQueue2;

        m_InputBuffer = &m_EventBuffer1;
        m_DispatchBuffer = &m_EventBuffer2;
    }

    ~EventSystem()
    {
        delete[] m_EventQueue1;
        delete[] m_EventQueue2;
    }

    // --- Event Dispatching ---

    template<typename T, typename... Args>
    void QueueEvent(Args&&... args) 
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

    template<typename T, typename... Args>
    void FireEvent(Args&&... args) 
    {
        T e = T(std::forward<Args>(args)...);
        e.typeId = TypeIdentifier::GetId<T>();
        NotifySubscribers(e);
    }

    void DispatchQueuedEvents() 
    {
        SwapQueues();
        for (unsigned int i = 0; i < m_DispatchQueueHead; i++)
        {
            Event* e = m_DispatchQueue[i];
            NotifySubscribers(*e);
            e->~Event();
        }
        ClearDispatchQueue();
    }

private:
    void NotifySubscribers(Event& e)
    {
        for (DelegateBucket& bucket : m_LayerBuckets) 
        {
            bucket.PropagateEvent(e);
            if (e.handled) break;
        }
        m_GlobalBucket.PropagateEvent(e);
    }

public:
    // --- Subscribers ---

    template<typename T, typename Obj, void (Obj::* Func)(T&)>
    void Subscribe(Layer layer, Obj* instance) {
        static_assert(std::is_base_of_v<Event, T>, "T must derive from Event");
        uint32_t id = TypeIdentifier::GetId<T>();

        EventDelegate delegate;
        delegate.instance = instance;
        delegate.stub = [](void* inst, Event& e) {
            (static_cast<Obj*>(inst)->*Func)(static_cast<T&>(e));
            };

        m_LayerBuckets[static_cast<size_t>(layer)].AddDelegate(id, delegate);
    }

    template<typename T, typename Obj, void (Obj::* Func)(T&)>
    void SubscribeGlobal(Obj* instance) {
        static_assert(std::is_base_of_v<Event, T>, "T must derive from Event");
        uint32_t id = TypeIdentifier::GetId<T>();

        EventDelegate delegate;
        delegate.instance = instance;
        delegate.stub = [](void* inst, Event& e) {
            (static_cast<Obj*>(inst)->*Func)(static_cast<T&>(e));
            };

        m_GlobalBucket.AddDelegate(id, delegate);
    }

    template<typename T, void (*func)(T&)>
    void SubscribeStatic(Layer layer)
    {
        static_assert(std::is_base_of_v<Event, T>, "T must derive from Event");
        uint32_t id = TypeIdentifier::GetId<T>();
        EventDelegate delegate;
        delegate.instance = nullptr; // Passed to the stub, but not used in the lambda defined below
        delegate.stub = [](void*, Event& e) { func(static_cast<T&>(e)); };
        m_LayerBuckets[static_cast<size_t>(layer)].AddDelegate(id, delegate);
    }

    template<typename T, void (*func)(T&)>
    void SubscribeStaticGlobal()
    {
        static_assert(std::is_base_of_v<Event, T>, "T must derive from Event");
        uint32_t id = TypeIdentifier::GetId<T>();
        EventDelegate delegate;
        delegate.instance = nullptr; // Passed to the stub, but not used in the lambda defined below
        delegate.stub = [](void*, Event& e) { func(static_cast<T&>(e)); };
        m_GlobalBucket.AddDelegate(id, delegate);
    }

private:
    DelegateBucket m_GlobalBucket;
    std::array<DelegateBucket, 4> m_LayerBuckets;

    LinearBuffer m_EventBuffer1{ 1024 * 64 }; // 64KB
    LinearBuffer m_EventBuffer2{ 1024 * 64 }; // 64KB

    Event** m_EventQueue1;
    Event** m_EventQueue2;

    uint32_t m_InputQueueHead = 0;
    Event** m_InputQueue;
    LinearBuffer* m_InputBuffer;

    uint32_t m_DispatchQueueHead = 0;
    Event** m_DispatchQueue;  
    LinearBuffer* m_DispatchBuffer;

    void SwapQueues()
    {
        Event** newInputQueue = m_DispatchQueue;
        Event** newDispatchQueue = m_InputQueue;

        LinearBuffer* newInputBuffer = m_DispatchBuffer;
        LinearBuffer* newDispatchBuffer = m_InputBuffer;

        m_InputQueue = newInputQueue;
        m_DispatchQueue = newDispatchQueue;

        m_InputBuffer = newInputBuffer;
        m_DispatchBuffer = newDispatchBuffer;

        m_DispatchQueueHead = m_InputQueueHead;
        m_InputQueueHead = 0; // Dispatch queue should always be 0 when queues are swapped
    }

    void ClearDispatchQueue()
    {
        m_DispatchQueueHead = 0;
        m_DispatchBuffer->reset();
    }
};
