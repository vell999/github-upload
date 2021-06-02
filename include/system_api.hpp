#ifndef SUBSCRIBE_FORM_H
#define SUBSCRIBE_FORM_H

#include "i_system_api.hpp"
#include "i_subscribers_updator.hpp"
#include "i_scheduler.hpp"

namespace smarthome
{

typedef std::shared_ptr<ISchedulerTask> SPISchedulerTask;
class SystemConnectApi : public ISystemConnectApi
{
public:
    SystemConnectApi(SharedPointer<IEventReceiver> a_reciver,
                     SharedPointer<ISubscriberUpdator> a_updator,
                     SharedPointer<IScheduler> a_scheduler);

    virtual SharedPointer<IEventReceiver> GetReciver();
    
    virtual void Subscribe(const Topic & a_topic, SharedPointer<ISubscriber> a_subscriber);

    virtual void SubscribeScheduler(SPISchedulerTask a_iSchedulerTask);    

private:
    SharedPointer<IEventReceiver> m_reciver;
    SharedPointer<ISubscriberUpdator> m_updator;    
    SharedPointer<IScheduler> m_scheduler;    

};

} // namespace smarthome

#endif // SUBSCRIBE_FORM_H
