#pragma once

#include "Core.h"
#include <vector>
#include "Events/Event.h"
#include <atomic>
#include <functional>
#include <queue>
#include <map>
#include <memory>
#include <type_traits>

using TypeErasedCallback = std::function<void(Event&)>;

enum class Layer : int {
    Debug = 0,
    Editor = 100,
    UI = 200,
    Game = 300,
};

// Use incremented ints to refer to Event types rather than actual types when storing event handlers, for performance
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

class EventSystem {
public:

    template<typename T, typename... Args>
    void queue_event(Args&&... args)
    {
        std::unique_ptr<T> e = std::make_unique<T>(std::forward<Args>(args)...);
        e->typeId = TypeIdentifier::get_id<T>();
        m_EventQueue.push(std::move(e));
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
        while (!m_EventQueue.empty()) 
        {
            std::unique_ptr<Event>& e = m_EventQueue.front();
            notify_subscribers((Event&)*e.get());

            m_EventQueue.pop();
        }
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

        m_LayerBuckets[layer].add_handler(std::move(wrapper), id);
    }

private:
    void notify_subscribers(Event& e)
    {
        for (auto& pair : m_LayerBuckets)
        {
            HandlerBucket& bucket = pair.second;
            bucket.propogate_event(e);

            if (e.handled) break;
        }

        m_GlobalBucket.propogate_event(e);
    }

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
            if (e.typeId < m_Container.size())
            {

                for (const auto& callback : m_Container[e.typeId])
                {
                    callback(e);
                }
            }
        }

    private:
        std::vector<std::vector<TypeErasedCallback>> m_Container;
    };

    HandlerBucket m_GlobalBucket;
    std::map<Layer, HandlerBucket> m_LayerBuckets;

    std::queue<std::unique_ptr<Event>> m_EventQueue;
};

