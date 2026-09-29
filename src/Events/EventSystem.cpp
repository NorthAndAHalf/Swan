#include "EventSystem.h"

EventSystem::EventSystem()
{
    spdlog::info("Allocating event buffers");

    // Prevent memory leak if the constructor is called twice
    if (!m_EventQueue1) m_EventQueue1 = new Event * [EVENT_QUEUE_COUNT];
    if (!m_EventQueue2) m_EventQueue2 = new Event * [EVENT_QUEUE_COUNT];

    m_InputQueue = m_EventQueue1;
    m_DispatchQueue = m_EventQueue2;

    m_InputBuffer = &m_EventBuffer1;
    m_DispatchBuffer = &m_EventBuffer2;
}

EventSystem::~EventSystem()
{
    delete[] m_EventQueue1;
    delete[] m_EventQueue2;
}

void EventSystem::DispatchQueuedEvents()
{
    SwapQueues();
    for (unsigned int i = 0; i < m_DispatchQueueHead; i++)
    {
        Event* e = m_DispatchQueue[i];
        NotifySubscribers(*e);
        e->~Event();
    }
    ClearDispatchQueue();
    FlushPendingSubscriptions();
    FlushPendingUnsubscribes();
    ResetCache();
}

// Subscriptions and unsubscriptions are deferred to the end of the frame
// Because we do not want the vectors to be resized during iteration (i.e. a subscription is made in an handler function)
void EventSystem::Unsubscribe(uint32_t typeId, uint64_t callbackId)
{
    auto matches = [callbackId](const Callback& cb) { return cb.id == callbackId; };

    // Live callbacks: mark for deferred deletion
    auto liveIt = m_CallbackMap.find(typeId);
    if (liveIt != m_CallbackMap.end())
    {
        auto& live = liveIt->second;
        auto it = std::find_if(live.begin(), live.end(), matches);
        if (it != live.end())
        {
            it->markedForDeletion = true;
            m_DirtyEventTypes.push_back(typeId);
            return;
        }
    }

    // Edge case: handler is subscribed and unsubscribed in the same frame, drop from pending
    auto pendingIt = m_PendingSubscriptions.find(typeId);
    if (pendingIt != m_PendingSubscriptions.end())
    {
        auto& pending = pendingIt->second;
        auto it = std::find_if(pending.begin(), pending.end(), matches);
        if (it != pending.end())
        {
            pending.erase(it);
            return;
        }
    }

    spdlog::error("Tried to unsubscribe unrecognised event callback ID");
}

void EventSystem::NotifySubscribers(Event& e)
{
    auto& group = m_CallbackMap[e.GetTypeId()];

    for (const Callback& callback : group)
    {
        if (!callback.markedForDeletion)
            callback.func(e);
    }
}

void EventSystem::SwapQueues()
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

void EventSystem::ClearDispatchQueue()
{
    m_DispatchQueueHead = 0;
    m_DispatchBuffer->reset();
}

void EventSystem::FlushPendingSubscriptions()
{
    for (auto& [typeId, pending] : m_PendingSubscriptions)
    {
        if (pending.empty()) continue;

        auto& live = m_CallbackMap[typeId];
        live.insert(live.end(),
            std::make_move_iterator(pending.begin()),
            std::make_move_iterator(pending.end()));
        pending.clear();
    }
}

void EventSystem::FlushPendingUnsubscribes()
{
    for (auto typeId : m_DirtyEventTypes)
    {
        auto newEnd = std::remove_if(m_CallbackMap[typeId].begin(), m_CallbackMap[typeId].end(),
            [](const Callback& cb)
            {
                return cb.markedForDeletion;
            });
        m_CallbackMap[typeId].erase(newEnd, m_CallbackMap[typeId].end());
    }
}

void EventSystem::ResetCache()
{
    m_DirtyEventTypes.clear();
}
