#ifndef NEW_FIRE_SENSOR
#define NEW_FIRE_SENSOR

#include <memory> // std::shared_ptr
#include "i_agent.hpp"
#include "i_scheduler_task.hpp"
#include "atomic_value.hpp"
#include "i_event.hpp"
#include "shared_pointer.hpp"

namespace smarthome
{    
    
class NewFireSensor : public IAgent , public ISchedulerTask
{
public:
    NewFireSensor(Location a_location, size_t a_maxNumEvents, advcpp::Nano a_frequency);

    ~NewFireSensor() = default;
    NewFireSensor(const NewFireSensor& a_rhs) = delete;
    NewFireSensor& operator = (const NewFireSensor& a_rhs) = delete;

    virtual void Connect(SharedPointer<ISystemConnectApi> a_systemConnectAPI);

    virtual bool Execute();

    virtual advcpp::Nano GetRunTime() const;

    virtual void SetRunTime(advcpp::Nano a_nano);

    virtual advcpp::Nano GetFrequency() const; 

    void SetSharedPointerPointer(std::shared_ptr<NewFireSensor> a_spNewFireSensor);

    int GetNumEventsSend() const;

private:
    bool checkIfContinue() const;
    SharedPointer<IEvent> createEvent();

private:    
    Location m_location;      
    SharedPointer<IEventReceiver> m_ptrReceiver;
    std::shared_ptr<NewFireSensor> m_spNewFireSensor;
    size_t m_maxNumEvents;
    AtomicValue<int> m_numEventsSend;
    advcpp::Nano m_runTime;
    advcpp::Nano m_frequency;

};

} // namespace smarthome

#endif // NEW_FIRE_SENSOR