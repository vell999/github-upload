#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <queue> // std::priority_queue
#include <memory> // std::shared_ptr
#include "i_scheduler.hpp"
#include "i_scheduler_task.hpp"
#include "atomic_value.hpp"

namespace smarthome
{

typedef bool (*CompareTask)(SPISchedulerTask, SPISchedulerTask);
typedef std::shared_ptr<ISchedulerTask> SPISchedulerTask;
typedef std::priority_queue<SPISchedulerTask, std::vector<SPISchedulerTask>, CompareTask > TasksQueue;

class Scheduler : public IScheduler
{
public:
    Scheduler();

    Scheduler(const Scheduler& a_rhs) = delete;  
    Scheduler& operator = (const Scheduler& a_rhs) = delete;
    virtual ~Scheduler() noexcept = default;

    virtual void Insert(SPISchedulerTask a_iSchedulerTask);

    virtual void Run() noexcept;

    virtual void Shutdown();

private:
    static const size_t OPEN_FOR_BUSINESS = 1;
    static const size_t CLOSE_FOR_BUSINESS = 0;
    advcpp::Nano clacRunTime(advcpp::Nano a_frequncy) const;
    void setNewRunTime(SPISchedulerTask& a_iSchedulerTask);
    void sleepUntilReady(advcpp::Nano a_runTime) const;
    SPISchedulerTask popTask();

private:
    advcpp::AtomicValue<int> m_flagShutdown;
    TasksQueue m_tasksQueue;

};

bool CompareNano(SPISchedulerTask a_lhs, SPISchedulerTask a_rhs);

} // namespace smarthome

#endif // SCHEDULER_H