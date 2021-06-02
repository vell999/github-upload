#include "scheduler.hpp"

#include <chrono> // system_clock

namespace smarthome
{

Scheduler::Scheduler()
: m_flagShutdown(OPEN_FOR_BUSINESS)
, m_tasksQueue(CompareNano)
{
}

void Scheduler::Insert(SPISchedulerTask a_iSchedulerTask)
{
    setNewRunTime(a_iSchedulerTask);

    m_tasksQueue.push(a_iSchedulerTask);
}

void Scheduler::Run() noexcept
{
    while(m_flagShutdown.Value() == OPEN_FOR_BUSINESS)
    {
        if(m_tasksQueue.size() > 0)
        {
            SPISchedulerTask task = popTask();
            sleepUntilReady(task->GetRunTime());
            bool continueExe = task->Execute();
            if(continueExe)
            {
               m_tasksQueue.push(task); 
            }
        }
    }
}

void Scheduler::Shutdown()
{
    m_flagShutdown.Set(CLOSE_FOR_BUSINESS);
}

advcpp::Nano Scheduler::clacRunTime(advcpp::Nano a_frequncy) const
{
    return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) + a_frequncy;
}

void Scheduler::setNewRunTime(SPISchedulerTask& a_iSchedulerTask)
{
    advcpp::Nano frequncy = a_iSchedulerTask->GetFrequency();

    advcpp::Nano runTime = clacRunTime(frequncy);

    a_iSchedulerTask->SetRunTime(runTime);
}

void Scheduler::sleepUntilReady(advcpp::Nano a_runTime) const
{
    advcpp::Nano now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    if(a_runTime - now > 0)
    {
        sleep(a_runTime - now);
    }
}

SPISchedulerTask Scheduler::popTask()
{
    SPISchedulerTask task = m_tasksQueue.top();

    m_tasksQueue.pop();

    return task;
}

bool CompareNano(SPISchedulerTask a_lhs, SPISchedulerTask a_rhs)
{
    return a_lhs->GetRunTime() > a_rhs->GetRunTime();
}

} // namespace smarthome
