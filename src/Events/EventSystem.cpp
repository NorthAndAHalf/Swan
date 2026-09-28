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
}

void EventSystem::NotifySubscribers(Event& e)
{
    auto group = m_CallbackMap[e.GetTypeId()];

    for (std::function callback : group)
    {
        callback(e);
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
