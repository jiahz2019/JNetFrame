
// @brief HttpServer测试

#include "src/jhz.h"

static jhz::Logger::ptr g_logger = JHZ_LOG_ROOT();

#define XX(...) #__VA_ARGS__

jhz::IOManager::ptr worker;

void run() {
    g_logger->setLevel(jhz::LogLevel::INFO);
    //jhz::http::HttpServer::ptr server(new jhz::http::HttpServer(true, worker.get(), jhz::IOManager::GetThis()));
    jhz::http::HttpServer::ptr server(new jhz::http::HttpServer(true));
    jhz::Address::ptr addr = jhz::Address::LookupAnyIPAddress("10.0.4.11:8080");
    JHZ_LOG_INFO(g_logger)<<addr->toString();
    while (!server->bind(addr)) {
        sleep(2);
    }
    auto sd = server->getServletDispatch();
    sd->addServlet("/jhz/xx", [](jhz::http::HttpRequest::ptr req, jhz::http::HttpResponse::ptr rsp, jhz::http::HttpSession::ptr session) {
        //rsp->setBody(req->toString());
        rsp->setBody("hello world");
        return 0;
    });

    sd->addGlobServlet("/jhz/*", [](jhz::http::HttpRequest::ptr req, jhz::http::HttpResponse::ptr rsp, jhz::http::HttpSession::ptr session) {
        rsp->setBody("Glob:\r\n" + req->toString());
        return 0;
    });

    sd->addGlobServlet("/jhzx/*", [](jhz::http::HttpRequest::ptr req, jhz::http::HttpResponse::ptr rsp, jhz::http::HttpSession::ptr session) {
        rsp->setBody(XX(<html>
                                <head><title> 404 Not Found</ title></ head>
                                <body>
                                <center><h1> 404 Not Found</ h1></ center>
                                <hr><center>
                                    nginx /
                                1.16.0 <
                            / center >
                            </ body>
                            </ html> < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >));
        return 0;
    });

    server->start();
}

int main(int argc, char **argv) {
    jhz::EnvMgr::GetInstance()->init(argc, argv);
    jhz::Config::LoadFromConfDir(jhz::EnvMgr::GetInstance()->getConfigPath());
    
    jhz::IOManager iom(1, true, "main");
    worker.reset(new jhz::IOManager(3, false, "worker"));
    iom.schedule(run);
    return 0;
}
