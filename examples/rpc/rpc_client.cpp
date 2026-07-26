/**
 * @file rpc_client.cpp
 * @brief gRPC独立客户端示例
 * @author yAlgo Team
 * @date 2025-12-07
 *
 * 使用方式：./rpc_client [host:port]
 * 默认连接 localhost:50051
 */

#include "rpc/grpc_client.h"
#include "rpc/rpc_utils.h"
#include "log/async_logger.h"
#include "echo_service.h"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

// ============================================================
// 测试函数
// ============================================================

bool testUnary(const std::shared_ptr<grpc::Channel>& channel) {
    std::cout << "\n--- Test: Unary Echo ---" << std::endl;
    auto stub = echo::EchoService::NewStub(channel);
    grpc::ClientContext context;
    echo::EchoRequest request;
    echo::EchoResponse response;

    request.set_message("Hello gRPC");
    request.set_seq(1);

    grpc::Status status = stub->Echo(&context, request, &response);
    if (!status.ok()) {
        std::cerr << "  FAILED: " << status.error_message() << std::endl;
        return false;
    }
    std::cout << "  OK: seq=" << response.seq()
              << " msg='" << response.message() << "'"
              << " server=" << response.server_id() << std::endl;
    return true;
}

bool testClientStream(const std::shared_ptr<grpc::Channel>& channel) {
    std::cout << "\n--- Test: Client Streaming ---" << std::endl;
    auto stub = echo::EchoService::NewStub(channel);
    grpc::ClientContext context;
    echo::EchoResponse response;

    auto writer = stub->ClientStream(&context, &response);
    for (int i = 0; i < 3; i++) {
        echo::EchoRequest request;
        request.set_message("Stream message " + std::to_string(i));
        request.set_seq(i);
        writer->Write(request);
    }
    writer->WritesDone();
    grpc::Status status = writer->Finish();

    if (!status.ok()) {
        std::cerr << "  FAILED: " << status.error_message() << std::endl;
        return false;
    }
    std::cout << "  OK: " << response.message() << std::endl;
    return true;
}

bool testServerStream(const std::shared_ptr<grpc::Channel>& channel) {
    std::cout << "\n--- Test: Server Streaming ---" << std::endl;
    auto stub = echo::EchoService::NewStub(channel);
    grpc::ClientContext context;
    echo::EchoRequest request;
    request.set_message("Server stream request");
    request.set_seq(0);

    auto reader = stub->ServerStream(&context, request);
    echo::EchoResponse response;
    int count = 0;
    while (reader->Read(&response)) {
        std::cout << "  Received: seq=" << response.seq()
                  << " msg='" << response.message() << "'" << std::endl;
        count++;
    }
    grpc::Status status = reader->Finish();
    if (!status.ok()) {
        std::cerr << "  FAILED: " << status.error_message() << std::endl;
        return false;
    }
    std::cout << "  OK: received " << count << " messages" << std::endl;
    return count == 5;
}

bool testBidiStream(const std::shared_ptr<grpc::Channel>& channel) {
    std::cout << "\n--- Test: Bidi Streaming ---" << std::endl;
    auto stub = echo::EchoService::NewStub(channel);
    grpc::ClientContext context;

    auto stream = stub->BidiStream(&context);
    for (int i = 0; i < 3; i++) {
        echo::EchoRequest request;
        request.set_message("Bidi message " + std::to_string(i));
        request.set_seq(i);
        stream->Write(request);
    }
    stream->WritesDone();

    echo::EchoResponse response;
    int count = 0;
    while (stream->Read(&response)) {
        std::cout << "  Echo: seq=" << response.seq()
                  << " msg='" << response.message() << "'" << std::endl;
        count++;
    }
    grpc::Status status = stream->Finish();
    if (!status.ok()) {
        std::cerr << "  FAILED: " << status.error_message() << std::endl;
        return false;
    }
    std::cout << "  OK: echoed " << count << " messages" << std::endl;
    return count == 3;
}

// ============================================================
// 主函数
// ============================================================

int main(int argc, char* argv[]) {
    std::string target = "localhost:50051";
    if (argc > 1) target = argv[1];

    // 初始化日志
    yalgo::log::LogConfig config;
    config.log_file = "rpc_client.log";
    config.enable_console = true;
    config.enable_file = true;
    config.runtime_level = yalgo::log::LogLevel::INFO;
    yalgo::log::AsyncLogger::getInstance().init(config);

    std::cout << "=== gRPC Echo Client ===" << std::endl;
    std::cout << "Connecting to " << target << "..." << std::endl;

    // 创建客户端
    yalgo::rpc::GrpcClient client(target);
    if (!client.waitForReady(5)) {
        std::cerr << "Failed to connect to " << target << std::endl;
        return 1;
    }
    std::cout << "Connected." << std::endl;

    // 运行测试
    bool all_passed = true;
    all_passed &= testUnary(client.getChannel());
    all_passed &= testClientStream(client.getChannel());
    all_passed &= testServerStream(client.getChannel());
    all_passed &= testBidiStream(client.getChannel());

    // 输出结果
    std::cout << "\n=== Results ===" << std::endl;
    if (all_passed) {
        std::cout << "All tests PASSED!" << std::endl;
    } else {
        std::cerr << "Some tests FAILED!" << std::endl;
    }

    return all_passed ? 0 : 1;
}
