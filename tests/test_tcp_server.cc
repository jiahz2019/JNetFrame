
// @brief TcpServer类测试

#include "src/jhz.h"

static jhz::Logger::ptr g_logger = JHZ_LOG_ROOT();

/**
 * @brief 自定义TcpServer类，重载handleClient方法
 */
class MyTcpServer : public jhz::TcpServer {
protected:
    virtual void handleClient(jhz::Socket::ptr client) override;
};

void MyTcpServer::handleClient(jhz::Socket::ptr client) {
    JHZ_LOG_INFO(g_logger) << "new client: " << client->toString();
    static std::string buf;
    buf.resize(4096);
    client->recv(&buf[0], buf.length()); // 这里有读超时，由tcp_server.read_timeout配置项进行配置，默认120秒
    JHZ_LOG_INFO(g_logger) << "recv: " << buf;
    client->close();
}

void run() {
    jhz::TcpServer::ptr server(new MyTcpServer); // 内部依赖shared_from_this()，所以必须以智能指针形式创建对象
    auto addr = jhz::Address::LookupAny("0.0.0.0:8080");
    JHZ_ASSERT(addr);
    std::vector<jhz::Address::ptr> addrs;
    addrs.push_back(addr);

    std::vector<jhz::Address::ptr> fails;
    while(!server->bind(addrs, fails)) {
        sleep(2);
    }
    
    JHZ_LOG_INFO(g_logger) << "bind success, " << server->toString();

    server->start();
}

void extraWork(){
    for(int i=1;i<4;i++){
        JHZ_LOG_INFO(g_logger) << "work has been finished" <<" i is "<<i;
        sleep(3);
    }
}
int main(int argc, char *argv[]) {
    jhz::EnvMgr::GetInstance()->init(argc, argv);
    jhz::Config::LoadFromConfDir(jhz::EnvMgr::GetInstance()->getConfigPath());

    jhz::IOManager iom(2);
    iom.schedule(&run);
    iom.schedule(&extraWork);
    return 0;
}