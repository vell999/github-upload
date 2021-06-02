#include "system_api.hpp"

namespace smarthome 
{

SystemConnectApi::SystemConnectApi(SharedPointer<IEventReceiver> a_reciver, 
                                   SharedPointer<ISubscriberUpdator> a_updator,
                                   SharedPointer<IScheduler> a_scheduler)
: m_reciver(a_reciver)
, m_updator(a_updator)
, m_scheduler(a_scheduler)
{
}

SharedPointer<IEventReceiver> SystemConnectApi::GetReciver()
{
    return m_reciver;
}

void SystemConnectApi::Subscribe(const Topic & a_topic , SharedPointer<ISubscriber> a_subscribe)
{
   m_updator->Subscribe(a_topic, a_subscribe);
}

void SystemConnectApi::SubscribeScheduler(SPISchedulerTask a_iSchedulerTask)
{
    m_scheduler->Insert(a_iSchedulerTask);
}

} // namespace smarthome 
