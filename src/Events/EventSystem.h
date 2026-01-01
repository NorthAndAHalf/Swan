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

// TODO: Change queue event function to use the new dual queue system

enum class Layer
{
    DEBUG = 0,
    UI = 1,
    GAME = 2
};

class TypeIdentifier {
public:
    template<typename T>
    static uint32_t get_id() {
        static const uint32_t id = m_Counter++;
        return id;
    }
private:
    inline static std::atomic<uint32_t> m_Counter{ 1 };
};

struct EventDelegate {
    using StubFunc = void(*)(void* instance, Event& event);

    void* instance = nullptr;
    StubFunc stub = nullptr;

    void operator()(Event& e) const { if (stub) stub(instance, e); }
};

class DelegateBucket 
{
public:
    void add_delegate(uint32_t type, EventDelegate d)
    {
        m_Pool[type].push_back(d);
    }

    void propagate_event(Event& e)
    {
        for (EventDelegate& delegate : m_Pool[e.get_type_id()])
        {
            delegate(e);
        }
    }

private:
    std::unordered_map<uint32_t, std::vector<EventDelegate>> m_Pool;
};

class EventSystem {
public:
    // --- Event Dispatching ---

    template<typename T, typename... Args>
    void queue_event(Args&&... args) {
        LinearBuffer& buffer = get_dispatch_buffer();
        auto& queue = get_dispatch_queue();

        void* address = buffer.allocate(sizeof(T));
        if (!address) {
            spdlog::critical("Event Buffer out of memory!");
            return;
        }

        T* e = new (address) T(std::forward<Args>(args)...);
        e->typeId = TypeIdentifier::get_id<T>();
        queue.push_back(e);
    }

    template<typename T, typename... Args>
    void fire_event(Args&&... args) {
        T e = T(std::forward<Args>(args)...);
        e.typeId = TypeIdentifier::get_id<T>();
        notify_subscribers(e);
    }

    void dispatch_queued_events() {
        std::vector<Event*>& queue = get_dispatch_queue();
        for (Event* e : queue) {
            notify_subscribers(*e);
            e->~Event();
        }
        swap_queues();
    }

private:
    void notify_subscribers(Event& e) {
        for (DelegateBucket& bucket : m_LayerBuckets) {
            bucket.propagate_event(e);
            if (e.handled) break;
        }
        m_GlobalBucket.propagate_event(e);
    }

public:
    // --- Subscribers ---

    template<typename T, typename Obj, void (Obj::* Func)(T&)>
    void subscribe(Layer layer, Obj* instance) {
        static_assert(std::is_base_of_v<Event, T>, "T must derive from Event");
        uint32_t id = TypeIdentifier::get_id<T>();

        EventDelegate delegate;
        delegate.instance = instance;
        delegate.stub = [](void* inst, Event& e) {
            (static_cast<Obj*>(inst)->*Func)(static_cast<T&>(e));
            };

        m_LayerBuckets[static_cast<size_t>(layer)].add_delegate(id, delegate);
    }

    template<typename T, typename Obj, void (Obj::* Func)(T&)>
    void subscribe_global(Obj* instance) {
        static_assert(std::is_base_of_v<Event, T>, "T must derive from Event");
        uint32_t id = TypeIdentifier::get_id<T>();

        EventDelegate delegate;
        delegate.instance = instance;
        delegate.stub = [](void* inst, Event& e) {
            (static_cast<Obj*>(inst)->*Func)(static_cast<T&>(e));
            };

        m_GlobalBucket.add_delegate(id, delegate);
    }

    template<typename T, void (*func)(T&)>
    void subscribe_static(Layer layer)
    {
        static_assert(std::is_base_of_v<Event, T>, "T must derive from Event");
        uint32_t id = TypeIdentifier::get_id<T>();
        EventDelegate delegate;
        delegate.instance = nullptr; // Passed to the stub, but not used in the lambda defined below
        delegate.stub = [](void*, Event& e) { func(static_cast<T&>(e)); };
        m_LayerBuckets[static_cast<size_t>(layer)].add_delegate(id, delegate);
    }

    template<typename T, void (*func)(T&)>
    void subscribe_static_global()
    {
        static_assert(std::is_base_of_v<Event, T>, "T must derive from Event");
        uint32_t id = TypeIdentifier::get_id<T>();
        EventDelegate delegate;
        delegate.instance = nullptr; // Passed to the stub, but not used in the lambda defined below
        delegate.stub = [](void*, Event& e) { func(static_cast<T&>(e)); };
        m_GlobalBucket.add_delegate(id, delegate);
    }

private:
    DelegateBucket m_GlobalBucket;
    std::array<DelegateBucket, 3> m_LayerBuckets;

    LinearBuffer m_EventBuffer1{ 1024 * 64 }; // 64KB
    LinearBuffer m_EventBuffer2{ 1024 * 64 }; // 64KB
    std::vector<Event*> m_EventQueue1;
    std::vector<Event*> m_EventQueue2;

    bool swapQueues;
    std::vector<Event*>& get_input_queue() { return swapQueues ? m_EventQueue1 : m_EventQueue2; }
    std::vector<Event*>& get_dispatch_queue() { return !swapQueues ? m_EventQueue1 : m_EventQueue2; }
    LinearBuffer& get_dispatch_buffer() { return !swapQueues ? m_EventBuffer1 : m_EventBuffer2; }

    void swap_queues()
    {
        auto& dispatchQueue = get_dispatch_queue();
        auto& dispatchBuffer = get_dispatch_buffer();

        dispatchQueue.clear();
        dispatchBuffer.reset();
        swapQueues = !swapQueues;
    }
};
