#pragma once

#include "Core.h"
#include <vector>
#include "Events/Event.h"
#include <atomic>
#include <functional>
#include <queue>
#include <memory>
#include <type_traits>

enum class Layer : int {
    Editor = 0,
    UI = 100,
    Game = 200,
    Debug = 300
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
    using TypeErasedCallback = std::function<void(Event&)>;

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

        if (e.typeId >= m_GlobalBucket.size()) return;

        for (const auto& callback : m_GlobalBucket[e.typeId])
        {
            callback(e);
        }
    }

    void dispatch_queued_events()
    {
        while (!m_EventQueue.empty()) 
        {
            std::unique_ptr<Event>& e = m_EventQueue.front();

            if (e->typeId < m_GlobalBucket.size())
            {

                for (const auto& callback : m_GlobalBucket[e->typeId])
                {
                    callback((Event&)*e.get());
                }
            }
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

        if (id >= m_GlobalBucket.size()) m_GlobalBucket.resize(id + 1);

        m_GlobalBucket[id].push_back(wrapper);
    }

    template<typename T>
    void subscribe_layer(Layer layer, std::function<void(T)> callback)
    {

    }

private:
    std::vector<std::vector<TypeErasedCallback>> m_GlobalBucket;

    std::queue<std::unique_ptr<Event>> m_EventQueue;
};

