#pragma once

#include "Core.h"
#include <vector>
#include "Events/Event.h"
#include <atomic>
#include <functional>
#include <queue>
#include <array>
#include <memory>
#include <type_traits>
#include "Memory/LinearBuffer.h"
#include "spdlog/spdlog.h"

// TODO: Replace std::function with a custom delegate; std::function can make large heap allocations, especially if the lambdas capture 'this'
using TypeErasedCallback = std::function<void(Event&)>;

enum class Layer : int {
    Debug = 0,
    Editor = 1,
    UI = 2,
    Game = 3,
};

// Use incremented ints to refer to Event types rather than actual types when storing event handlers, for performance
// Static int is not cross DLL safe, will need to be changed if cross DLL is needed eventually
class TypeIdentifier
{
public:
    template<typename T>
    static uint32_t get_id()
    {
        static const uint32_t id = m_Counter++;
        return id;
    }
private:
    inline static std::atomic<uint32_t> m_Counter{ 1 }; // Start at 1 as typeId is initialised to 0 in Event
};

class HandlerBucket
{
public:
    HandlerBucket() {}

    void add_handler(TypeErasedCallback&& callback, uint32_t id)
    {
        if (id >= m_Container.size()) m_Container.resize(id + 1);

        m_Container[id].push_back(std::move(callback));
    }

    template<typename T>
    void propogate_event(T& e)
    {
        uint32_t typeId = e.get_type_id();

        if (typeId < m_Container.size())
        {

            for (const auto& callback : m_Container[typeId])
            {
                callback(e);
            }
        }
    }

private:
    std::vector<std::vector<TypeErasedCallback>> m_Container;
};

class EventSystem {
public:

    template<typename T, typename... Args>
    void queue_event(Args&&... args)
    {
        void* address = m_Buffer.allocate(sizeof(T));
        if (!address)
        {
            spdlog::critical("Event Buffer out of memory!");
            return;
        }

        T* e = new (address) T(std::forward<Args>(args)...);
        e->typeId = TypeIdentifier::get_id<T>();

        m_EventQueue.push_back(e);
    }

    template<typename T, typename... Args>
    void fire_event(Args&&... args)
    {
        T e = T(std::forward<Args>(args)...);
        e.typeId = TypeIdentifier::get_id<T>();

        notify_subscribers(e);
    }

    void dispatch_queued_events()
    {
        for (Event* e : m_EventQueue)
        {
            notify_subscribers(*e);
            e->~Event();
        }

        m_EventQueue.clear();
        m_Buffer.reset();
    }

    template<typename T>
    void subscribe_global(std::function<void(T&)> callback)
    {
        SF_ASSERT((std::is_base_of<Event, T>::value), "T must derive from Event");
        uint32_t id = TypeIdentifier::get_id<T>();

        TypeErasedCallback wrapper = [callback](Event& event)
            {
                callback(static_cast<T&>(event));
            };

        m_GlobalBucket.add_handler(std::move(wrapper), id);
    }

    template<typename T>
    void subscribe_layer(Layer layer, std::function<void(T&)> callback)
    {
        SF_ASSERT((std::is_base_of<Event, T>::value), "T must derive from Event");
        uint32_t id = TypeIdentifier::get_id<T>();

        TypeErasedCallback wrapper = [callback](Event& event)
            {
                callback(static_cast<T&>(event));
            };

        m_LayerBuckets[static_cast<size_t>(layer)].add_handler(std::move(wrapper), id);
    }

private:
    void notify_subscribers(Event& e)
    {
        for (HandlerBucket& bucket: m_LayerBuckets)
        {
            bucket.propogate_event(e);

            if (e.handled) break;
        }

        m_GlobalBucket.propogate_event(e);
    }

    LinearBuffer m_Buffer{ 1024 * 64 }; // 64KB per frame (or per queue dispatch)

    HandlerBucket m_GlobalBucket;
    std::array<HandlerBucket, 4> m_LayerBuckets; // Magic Number, the amount of values in the Layer enum
    std::vector<Event*> m_EventQueue;
};

