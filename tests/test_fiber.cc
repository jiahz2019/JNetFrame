
// @brief 协程测试

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

    JHZ_LOG_INFO(g_logger) << "before run_in_fiber yield";
    jhz::Fiber::GetThis()->yield();
    JHZ_LOG_INFO(g_logger) << "after run_in_fiber yield";

    JHZ_LOG_INFO(g_logger) << "run_in_fiber end";
    // fiber结束之后会自动返回主协程运行
}

void test_fiber() {
    JHZ_LOG_INFO(g_logger) << "test_fiber begin";

    // 初始化线程主协程
    jhz::Fiber::GetThis();

    jhz::Fiber::ptr fiber(new jhz::Fiber(run_in_fiber, 0, false));
    JHZ_LOG_INFO(g_logger) << "use_count:" << fiber.use_count(); // 1

    JHZ_LOG_INFO(g_logger) << "before test_fiber resume";
    fiber->resume();
    JHZ_LOG_INFO(g_logger) << "after test_fiber resume";

    /** 
     * 关于fiber智能指针的引用计数为3的说明：
     * 一份在当前函数的fiber指针，一份在MainFunc的cur指针
     * 还有一份在在run_in_fiber的GetThis()结果的临时变量里
     */
    JHZ_LOG_INFO(g_logger) << "use_count:" << fiber.use_count(); // 3

    JHZ_LOG_INFO(g_logger) << "fiber status: " << fiber->getState(); // READY

    JHZ_LOG_INFO(g_logger) << "before test_fiber resume again";
    fiber->resume();
    JHZ_LOG_INFO(g_logger) << "after test_fiber resume again";

    JHZ_LOG_INFO(g_logger) << "use_count:" << fiber.use_count(); // 1
    JHZ_LOG_INFO(g_logger) << "fiber status: " << fiber->getState(); // TERM

    fiber->reset(run_in_fiber2); // 上一个协程结束之后，复用其栈空间再创建一个新协程
    fiber->resume();

    JHZ_LOG_INFO(g_logger) << "use_count:" << fiber.use_count(); // 1
    JHZ_LOG_INFO(g_logger) << "test_fiber end";
}

int main(int argc, char *argv[]) {
    jhz::EnvMgr::GetInstance()->init(argc, argv);
    jhz::Config::LoadFromConfDir(jhz::EnvMgr::GetInstance()->getConfigPath());

    jhz::SetThreadName("main_thread");
    JHZ_LOG_INFO(g_logger) << "main begin";

    std::vector<jhz::Thread::ptr> thrs;
    for (int i = 0; i < 2; i++) {
        thrs.push_back(jhz::Thread::ptr(
            new jhz::Thread(&test_fiber, "thread_" + std::to_string(i))));
    }

    for (auto i : thrs) {
        i->join();
    }

    JHZ_LOG_INFO(g_logger) << "main end";
    return 0;
}