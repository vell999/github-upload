#include "new_fire_sensor.hpp"

#include "fire_event.hpp"

namespace smarthome
{
    
NewFireSensor::NewFireSensor(Location a_location, size_t a_maxNumEvents, advcpp::Nano a_frequency)
: m_location(a_location)
, m_maxNumEvents(a_maxNumEvents)
, m_frequency(a_frequency)
, m_numEventsSend(0)
, m_runTime(0)
{
}

void NewFireSensor::Connect(SharedPointer<ISystemConnectApi> a_systemConnectAPI)
{
    m_ptrReceiver = a_systemConnectAPI->GetReciver();

    a_systemConnectAPI->SubscribeScheduler(m_spNewFireSensor);    
}

bool NewFireSensor::Execute()
{
    SharedPointer<IEvent> fireEvent = createEvent();

    m_ptrReceiver->Receive(fireEvent);

    ++m_numEventsSend;

    return checkIfContinue();
}

advcpp::Nano NewFireSensor::GetRunTime() const
{
    return m_runTime;
}

void NewFireSensor::SetRunTime(advcpp::Nano a_nano)
{
    m_runTime = a_nano;
}

advcpp::Nano NewFireSensor::GetFrequency() const
{
    return m_frequency;
}

void NewFireSensor::SetSharedPointerPointer(std::shared_ptr<NewFireSensor> a_spNewFireSensor)
{
    m_spNewFireSensor = a_spNewFireSensor;
}

int NewFireSensor::GetNumEventsSend() const
{
    return m_numEventsSend.Value();
}

bool NewFireSensor::checkIfContinue() const
{
    return m_numEventsSend.Value() < m_maxNumEvents;
}

SharedPointer<IEvent> NewFireSensor::createEvent()
{
    Topic fireTopic;
    fireTopic.m_type.m_type = "fire sensor";
    fireTopic.m_location = m_location;

    TimeStamp timeEvent;
    asctime(gmtime(&(timeEvent.m_time)));

    Flames flame;
    flame.m_exist = true;
    flame.m_height = 100;

    SharedPointer<IEvent> fireEvent(new FireEvent(fireTopic, timeEvent, flame)); 

    return fireEvent;    
}

} // namespace smarthome


