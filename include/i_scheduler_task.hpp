#ifndef I_SCHEDULER_TASK_H
#define I_SCHEDULER_TASK_H

#include "time.hpp"

namespace smarthome
{

class ISchedulerTask
{
public:
    virtual ~ISchedulerTask() noexcept = default;

    virtual bool Execute() = 0;

    virtual advcpp::Nano GetRunTime() const = 0;

    virtual void SetRunTime(advcpp::Nano a_nano) = 0;

    virtual advcpp::Nano GetFrequency() const = 0;    

protected:
    ISchedulerTask() = default;
    ISchedulerTask(const ISchedulerTask& a_rhs) = default;
    ISchedulerTask& operator = (const ISchedulerTask& a_rhs) = default;

};

} // namespace smarthome

#endif // I_SCHEDULER_TASK_H