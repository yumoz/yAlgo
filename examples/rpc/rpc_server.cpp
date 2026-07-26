/**
 * @file rpc_server.cpp
 * @brief gRPC独立服务端示例
 * @author yAlgo Team
 * @date 2025-12-07
 *
 * 使用方式：./rpc_server [port]
 * 默认监听 0.0.0.0:50051
 */

#include "rpc/grpc_server.h"
#include "log/async_logger.h"
#include "echo_service.h"

#include <iostream>
#include <string>
#include <csignal>

static yalgo::rpc::GrpcServer* g_server = nullptr;

static void signalHandler(int sig) {
    std::cout << "\nReceived signal " << sig << ", shutting down..." << std::endl;
    if (g_server) g_server->stop();
}

int main(int argc, char* argv[]) {
    std::string port = "50051";
    if (argc > 1) port = argv[1];
    std::string address = "0.0.0.0:" + port;

    // 初始化日志
    yalgo::log::LogConfig config;
    config.log_file = "rpc_server.log";
    config.enable_console = true;
    config.enable_file = true;
    config.runtime_level = yalgo::log::LogLevel::INFO;
    yalgo::log::AsyncLogger::getInstance().init(config);

    std::cout << "=== gRPC Echo Server ===" << std::endl;
    std::cout << "Listening on " << address << std::endl;
    std::cout << "Press Ctrl+C to stop." << std::endl;

    // 注册信号处理
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    // 创建并启动服务器
    EchoServiceImpl service;
    yalgo::rpc::GrpcServer server(address);
    server.registerService(&service);
    g_server = &server;

    if (!server.start()) {
        std::cerr << "Failed to start server on " << address << std::endl;
        return 1;
    }

    std::cout << "Server started. Waiting for connections..." << std::endl;
    server.waitForShutdown();

    std::cout << "Server stopped." << std::endl;
    return 0;
}
