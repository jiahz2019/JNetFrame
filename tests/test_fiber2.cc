
//@brief 协程测试，用于演示非对称协程

#include "src/fiber.h"
#include "src/jhz.h"
#include <string>
#include <vector>

jhz::Logger::ptr g_logger = JHZ_LOG_ROOT();

void run_in_fiber2() {
    JHZ_LOG_INFO(g_logger) << "run_in_fiber2 begin";
    JHZ_LOG_INFO(g_logger) << "run_in_fiber2 end";
}

void run_in_fiber() {
    JHZ_LOG_INFO(g_logger) << "run_in_fiber begin";

    /**
     * 非对称协程，子协程不能创建并运行新的子协程，下面的操作是有问题的，
     * 子协程再创建子协程，原来的主协程就跑飞了
     */
    jhz::Fiber::ptr fiber(new jhz::Fiber(run_in_fiber2, 0, false));
    fiber->resume();

    JHZ_LOG_INFO(g_logger) << "run_in_fiber end";
}

int main(int argc, char *argv[]) {
    jhz::EnvMgr::GetInstance()->init(argc, argv);
    jhz::Config::LoadFromConfDir(jhz::EnvMgr::GetInstance()->getConfigPath());

    JHZ_LOG_INFO(g_logger) << "main begin";

    jhz::Fiber::GetThis();

    jhz::Fiber::ptr fiber(new jhz::Fiber(run_in_fiber, 0, false));
    fiber->resume();

    JHZ_LOG_INFO(g_logger) << "main end";
    return 0;
}