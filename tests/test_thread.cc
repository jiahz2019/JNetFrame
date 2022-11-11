
//@brief 线程模块测试

#include "src/jhz.h"

jhz::Logger::ptr g_logger = JHZ_LOG_ROOT();

int count = 0;
jhz::Mutex s_mutex;

void func1(void *arg) {
    JHZ_LOG_INFO(g_logger) << "name:" << jhz::Thread::GetName()
        << " this.name:" << jhz::Thread::GetThis()->getName()
        << " thread name:" << jhz::GetThreadName()
        << " id:" << jhz::GetThreadId()
        << " this.id:" << jhz::Thread::GetThis()->getId();
    JHZ_LOG_INFO(g_logger) << "arg: " << *(int*)arg;
    for(int i = 0; i < 10000; i++) {
        jhz::Mutex::Lock lock(s_mutex);
        ++count;
    }
}

int main(int argc, char *argv[]) {
    jhz::EnvMgr::GetInstance()->init(argc, argv);
    jhz::Config::LoadFromConfDir(jhz::EnvMgr::GetInstance()->getConfigPath());

    std::vector<jhz::Thread::ptr> thrs;
    int arg = 123456;
    for(int i = 0; i < 3; i++) {
        // 带参数的线程用std::bind进行参数绑定
        jhz::Thread::ptr thr(new jhz::Thread(std::bind(func1, &arg), "thread_" + std::to_string(i)));
        thrs.push_back(thr);
    }

    for(int i = 0; i < 3; i++) {
        thrs[i]->join();
    }
    
    JHZ_LOG_INFO(g_logger) << "count = " << count ;
    return 0;
}