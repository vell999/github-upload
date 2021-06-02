#ifndef I_SCHEDULER_H
#define I_SCHEDULER_H

#include <memory> // std::shared_ptr
#include "i_scheduler_task.hpp"
#include "thread.hpp"

namespace smarthome
{

typedef std::shared_ptr<ISchedulerTask> SPISchedulerTask;
class IScheduler : public advcpp::IRunnable
{
public:
    virtual ~IScheduler() noexcept = default;

    virtual void Insert(SPISchedulerTask a_iSchedulerTask) = 0;

    virtual void Run() noexcept = 0;

    virtual void Shutdown() = 0;

protected:
    IScheduler() = default;
    IScheduler(const IScheduler& a_rhs) = default;
    IScheduler& operator = (const IScheduler& a_rhs) = default;

};

} // namespace smarthome

#endif // I_SCHEDULER_H