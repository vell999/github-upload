#include "mu_test.h"

#include "scheduler.hpp"
#include "system_api.hpp"
#include "i_subscribers_list_notifier.hpp"
#include "new_fire_sensor.hpp"
#include "events_queue.hpp"
#include "event_dispatcher.hpp"
#include "receiver.hpp"
#include "i_system_api.hpp"
#include "thread.hpp"


using namespace std;
using namespace advcpp;
using namespace smarthome;

class SubscribersListNotifier : public ISubscribersListNotifier
{
public:

    SubscribersListNotifier();

    virtual void Notifi(SharedPointer<IEvent> a_event);
    virtual void ShutDown();
    int GetNumEvents();

private:
    AtomicValue<int> m_num;

};

SubscribersListNotifier::SubscribersListNotifier()
: m_num(0)
{ 
}

void SubscribersListNotifier::ShutDown()
{
}

void SubscribersListNotifier::Notifi(SharedPointer<IEvent>)
{
    ++m_num;    
    std::cout << m_num.Value() << std::endl;
}

int SubscribersListNotifier::GetNumEvents()
{
    return m_num.Value();
}


class SystemConnectApiTest : public ISystemConnectApi
{
public:
    SystemConnectApiTest(SharedPointer<IEventReceiver> a_receviver, SharedPointer<IScheduler> a_scheduler);

    virtual SharedPointer<IEventReceiver> GetReciver();

    virtual void Subscribe(const Topic & a_topic, SharedPointer<ISubscriber> a_subscriber);

    virtual void SubscribeScheduler(SPISchedulerTask a_iSchedulerTask);

private:
    SharedPointer<IEventReceiver> m_receviver;
    SharedPointer<IScheduler> m_scheduler; 

};

SystemConnectApiTest::SystemConnectApiTest(SharedPointer<IEventReceiver> a_receviver, SharedPointer<IScheduler> a_scheduler)
: m_receviver(a_receviver)
, m_scheduler(a_scheduler)
{
}

void SystemConnectApiTest::Subscribe(const Topic & , SharedPointer<ISubscriber> )
{
}

SharedPointer<IEventReceiver> SystemConnectApiTest::GetReciver()
{
    return m_receviver;
}

void SystemConnectApiTest::SubscribeScheduler(SPISchedulerTask a_iSchedulerTask)
{
    m_scheduler->Insert(a_iSchedulerTask);
}

/*
void Factory(std::vector<SharedPointer<IAgent> > & a_agents, size_t numOfTopics, vector<SharedPointer<FireSensor> >& a_sensors)
{
	
    Location location;
    location.m_floor = 4;
    location.m_room = 1;	
    size_t numEvents = 1;

    size_t numControllers = numOfTopics;

    while(numControllers-- >0)
    {
        SharedPointer<SprinklerController> fireController(new SprinklerController(location));
        fireController->SetSharedPointer(fireController);
        a_agents.push_back(fireController);
    }

    size_t numSensors = numOfTopics;

    while(numSensors-- >0)
    {
        SharedPointer<FireSensor> fireSensor(new FireSensor(location, numEvents));
        a_agents.push_back(fireSensor);
        a_sensors.push_back(fireSensor);
    }
}

UNIT(connect_everyone_test_big_number)

	// AgentContainer agentVector;

    // vector<SharedPointer<FireSensor> > fireSensors;

	// Factory(agentVector.GetStorage(), 1, fireSensors);

	// SharedPointer<SubscribersStorage> sStorage(new SubscribersStorage);
	
	// Broadcast bcast(*sStorage);
	
	// EventsQueue eventsStorage(1000);

    // const size_t numThreads = 1;

    // EventDispatcher eventDispatcher(eventsStorage, bcast, numThreads);

   	// SharedPointer<Receiver> receiver(new Receiver(eventsStorage));
	
	// Connector ConnectEveryone(receiver, sStorage);
	
	// ConnectEveryone.Connect(agentVector);

    // eventDispatcher.Resume();
	
	// eventDispatcher.ShutDown();
	// bcast.ShutDown();

	// ASSERT_PASS();
END_UNIT

UNIT(only_receiver)

    SubscribersListNotifier subscribersListNotifier;

    const size_t capacityEventSrotage = 1000;

	EventsQueue eventsStorage(capacityEventSrotage);

    const size_t numThreads = 5;

    EventDispatcher eventDispatcher(eventsStorage, subscribersListNotifier, numThreads);

   	SharedPointer<Receiver> receiver(new Receiver(eventsStorage));

    SharedPointer<SystemConnectApi> subscribeForm(new SystemConnectApi(receiver));

    Location location;
    location.m_floor = 4;
    location.m_room = 1;	
    size_t numEvents = 300;

    size_t numSensors = 1;

    vector<SharedPointer<FireSensor> > sensors;

    while(numSensors-- >0)
    {
        SharedPointer<FireSensor> fireSensor(new FireSensor(location, numEvents));
        fireSensor->Connect(subscribeForm);
        sensors.push_back(fireSensor);
    }

    eventDispatcher.Resume();

    eventDispatcher.ShutDown();

    ASSERT_PASS();

END_UNIT
*/

UNIT(scheduler)

    SubscribersListNotifier subscribersListNotifier;

    const size_t capacityEventSrotage = 1000;

	EventsQueue eventsStorage(capacityEventSrotage);

    const size_t numThreads = 1;    

    EventDispatcher eventDispatcher(eventsStorage, subscribersListNotifier, numThreads);

   	SharedPointer<Receiver> spReceiver(new Receiver(eventsStorage));

    SharedPointer<Scheduler> spScheduler(new Scheduler());

    Thread threadScheduler(&*spScheduler);   

    SharedPointer<SystemConnectApiTest> spSystemConnectApiTest(new SystemConnectApiTest(spReceiver, spScheduler));       

    Location location;
    location.m_floor = 4;
    location.m_room = 1;	
    size_t maxNumEvents = 2;
    advcpp::Nano frequncy(1);

    size_t numSensors = 1;

    vector<std::shared_ptr<NewFireSensor> > sensors;

    while(numSensors-- >0)
    {
        std::shared_ptr<NewFireSensor> fireSensor(new NewFireSensor(location, maxNumEvents, frequncy));
        fireSensor->SetSharedPointerPointer(fireSensor);
        fireSensor->Connect(spSystemConnectApiTest);
        sensors.push_back(fireSensor);
    }

    eventDispatcher.Resume();

    eventDispatcher.ShutDown();

    spScheduler->Shutdown();

    threadScheduler.Join();

    std::cout << "sensors[0]->GetNumEventsSend() " << sensors[0]->GetNumEventsSend() << '\n';
    std::cout << "subscribersListNotifier.GetNumEvents() " << subscribersListNotifier.GetNumEvents() << '\n';

    ASSERT_THAT(sensors[0]->GetNumEventsSend() == subscribersListNotifier.GetNumEvents());

END_UNIT

TEST_SUITE(amit_test)

    // IGNORE_TEST(connect_everyone_test_big_number)
    // TEST(only_receiver)
    TEST(scheduler)

END_SUITE
