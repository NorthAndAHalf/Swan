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

enum class Layer
{
    DEBUG = 0,
    UI = 1,
    GAME = 2
};

struct EventDelegate {
    using StubFunc = void(*)(void* instance, Event& event);

    void* instance = nullptr;
    StubFunc stub = nullptr;

    void operator()(Event& e) const { if (stub) stub(instance, e); }
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

class DelegateBucket {
    struct Entry {
        uint32_t typeId;
        EventDelegate delegate;
    };

    DelegateBucket(void* pool)
        : ptr(pool) {}

    void add_delegate()
    {

    }

private:
    void* ptr;
    uint32_t offset;
};

class EventSystem {
public:
    // --- Event Dispatching ---

    template<typename T, typename... Args>
    void queue_event(Args&&... args) {
        void* address = m_EventQueueBuffer.allocate(sizeof(T));
        if (!address) {
            spdlog::critical("Event Buffer out of memory!");
            return;
        }

        T* e = new (address) T(std::forward<Args>(args)...);
        e->typeId = TypeIdentifier::get_id<T>();
        m_EventQueue.push_back(e);
    }

    template<typename T, typename... Args>
    void fire_event(Args&&... args) {
        T e = T(std::forward<Args>(args)...);
        e.typeId = TypeIdentifier::get_id<T>();
        notify_subscribers(e);
    }

    void dispatch_queued_events() {
        for (Event* e : m_EventQueue) {
            notify_subscribers(*e);
            e->~Event();
        }
        m_EventQueue.clear();
        m_EventQueueBuffer.reset();
    }

private:
    void notify_subscribers(Event& e) {
        for (DelegateBucket& bucket : m_LayerBuckets) {
            bucket.propogate_event(e);
            if (e.handled) return;
        }
        m_GlobalBucket.propogate_event(e);
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

        m_LayerBuckets[static_cast<size_t>(layer)].add_handler(delegate, id);
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

        m_GlobalBucket.add_handler(delegate, id);
    }

    template<typename T, void (*func)(T&)>
    void subscribe_static(Layer layer)
    {
        static_assert(std::is_base_of_v<Event, T>, "T must derive from Event");
        uint32_t id = TypeIdentifier::get_id<T>();
        EventDelegate delegate;
        delegate.instance = nullptr; // Passed to the stub, but not used in the lambda defined below
        delegate.stub = [](void*, Event& e) { func(static_cast<T&>(e)); };
        m_LayerBuckets[static_cast<size_t>(layer)].add_handler(delegate, id);
    }

    template<typename T, void (*func)(T&)>
    void subscribe_static_global()
    {
        static_assert(std::is_base_of_v<Event, T>, "T must derive from Event");
        uint32_t id = TypeIdentifier::get_id<T>();
        EventDelegate delegate;
        delegate.instance = nullptr; // Passed to the stub, but not used in the lambda defined below
        delegate.stub = [](void*, Event& e) { func(static_cast<T&>(e)); };
        m_GlobalBucket.add_handler(delegate, id);
    }

private:
    LinearBuffer m_EventQueueBuffer{ 1024 * 64 }; // 64KB
    LinearBuffer m_DelegatePool{ 200 * 20 }; // 200 Delegate Entries (4KB)
    DelegateBucket m_GlobalBucket;
    std::array<DelegateBucket, 3> m_LayerBuckets;
    std::vector<Event*> m_EventQueue;
};