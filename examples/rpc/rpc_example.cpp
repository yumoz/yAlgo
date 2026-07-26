/**
 * @file rpc_example.cpp
 * @brief gRPC双向流式通信示例
 * @author yAlgo Team
 * @date 2025-12-07
 * 
 * 演示内容：
 * 1. 启动一个gRPC服务器（在后台线程）
 * 2. 客户端连接服务器
 * 3. 执行四种调用模式：unary、client-streaming、server-streaming、bidi-streaming
 * 4. 停止服务器
 */

#include "rpc/grpc_server.h"
#include "rpc/grpc_client.h"
#include "rpc/rpc_utils.h"
#include "log/async_logger.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <string>

// 生成的proto头文件
#include "echo.pb.h"
#include "echo.grpc.pb.h"

// ============================================================
// Echo服务实现
// ============================================================

class EchoServiceImpl final : public echo::EchoService::Service {
public:
    // Unary调用：返回相同消息
    grpc::Status Echo(grpc::ServerContext* context,
                      const echo::EchoRequest* request,
                      echo::EchoResponse* response) override {
        response->set_message(request->message());
        response->set_seq(request->seq());
        response->set_server_id("echo-server-1");
        
        YLOG_INFO("Echo received: seq=%d msg='%s'", request->seq(), request->message().c_str());
        return grpc::Status::OK;
    }

    // Client-streaming：接收所有消息后返回汇总
    grpc::Status ClientStream(grpc::ServerContext* context,
                              grpc::ServerReader<echo::EchoRequest>* reader,
                              echo::EchoResponse* response) override {
        echo::EchoRequest request;
        int count = 0;
        std::string last_msg;
        
        while (reader->Read(&request)) {
            last_msg = request.message();
            count++;
            YLOG_INFO("ClientStream received: seq=%d", request.seq());
        }
        
        response->set_message("Received " + std::to_string(count) + " messages, last: " + last_msg);
        response->set_seq(count);
        response->set_server_id("echo-server-1");
        
        return grpc::Status::OK;
    }

    // Server-streaming：对一个请求返回多个响应
    grpc::Status ServerStream(grpc::ServerContext* context,
                              const echo::EchoRequest* request,
                              grpc::ServerWriter<echo::EchoResponse>* writer) override {
        for (int i = 0; i < 5; i++) {
            echo::EchoResponse response;
            response.set_message(request->message() + " [response " + std::to_string(i) + "]");
            response.set_seq(i);
            response.set_server_id("echo-server-1");
            writer->Write(response);
            YLOG_INFO("ServerStream sent: seq=%d", i);
        }
        return grpc::Status::OK;
    }

    // Bidi-streaming：实时回显每个收到的消息
    grpc::Status BidiStream(grpc::ServerContext* context,
                            grpc::ServerReaderWriter<echo::EchoResponse, echo::EchoRequest>* stream) override {
        echo::EchoRequest request;
        while (stream->Read(&request)) {
            echo::EchoResponse response;
            response.set_message("echo: " + request.message());
            response.set_seq(request.seq());
            response.set_server_id("echo-server-1");
            stream->Write(response);
            YLOG_INFO("BidiStream echo: seq=%d msg='%s'", request.seq(), request.message().c_str());
        }
        return grpc::Status::OK;
    }
};

// ============================================================
// 测试函数
// ============================================================

const std::string SERVER_ADDRESS = "localhost:50051";

/**
 * @brief 测试unary调用
 */
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
        std::cerr << "Unary failed: " << status.error_message() << std::endl;
        return false;
    }
    
    std::cout << "  Response: seq=" << response.seq() 
              << " msg='" << response.message() << "'" 
              << " server=" << response.server_id() << std::endl;
    return true;
}

/**
 * @brief 测试client-streaming
 */
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
        std::cerr << "ClientStream failed: " << status.error_message() << std::endl;
        return false;
    }
    
    std::cout << "  Response: " << response.message() << std::endl;
    return true;
}

/**
 * @brief 测试server-streaming
 */
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
        std::cerr << "ServerStream failed: " << status.error_message() << std::endl;
        return false;
    }
    
    std::cout << "  Total received: " << count << " messages" << std::endl;
    return count == 5;
}

/**
 * @brief 测试bidi-streaming
 */
bool testBidiStream(const std::shared_ptr<grpc::Channel>& channel) {
    std::cout << "\n--- Test: Bidi Streaming ---" << std::endl;
    
    auto stub = echo::EchoService::NewStub(channel);
    grpc::ClientContext context;
    
    auto stream = stub->BidiStream(&context);
    
    // 发送3条消息
    for (int i = 0; i < 3; i++) {
        echo::EchoRequest request;
        request.set_message("Bidi message " + std::to_string(i));
        request.set_seq(i);
        stream->Write(request);
    }
    
    // 关闭写入端
    stream->WritesDone();
    
    // 读取所有响应
    echo::EchoResponse response;
    int count = 0;
    while (stream->Read(&response)) {
        std::cout << "  Echo: seq=" << response.seq() 
                  << " msg='" << response.message() << "'" << std::endl;
        count++;
    }
    
    grpc::Status status = stream->Finish();
    if (!status.ok()) {
        std::cerr << "BidiStream failed: " << status.error_message() << std::endl;
        return false;
    }
    
    std::cout << "  Total echoed: " << count << " messages" << std::endl;
    return count == 3;
}

// ============================================================
// 主函数
// ============================================================

int main() {
    // 初始化日志
    yalgo::log::LogConfig config;
    config.log_file = "rpc_example.log";
    config.enable_console = true;
    config.enable_file = true;
    config.runtime_level = yalgo::log::LogLevel::INFO;
    yalgo::log::AsyncLogger::getInstance().init(config);
    
    std::cout << "=== gRPC RPC Example ===" << std::endl;
    
    // 1. 创建并启动服务器
    std::cout << "\n[1] Starting gRPC server on " << SERVER_ADDRESS << "..." << std::endl;
    
    EchoServiceImpl service;
    yalgo::rpc::GrpcServer server(SERVER_ADDRESS);
    server.registerService(&service);
    
    if (!server.startAsync()) {
        std::cerr << "Failed to start server!" << std::endl;
        return 1;
    }
    
    // 等待服务器启动
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // 2. 创建客户端
    std::cout << "\n[2] Creating gRPC client..." << std::endl;
    yalgo::rpc::GrpcClient client(SERVER_ADDRESS);
    
    // 等待连接就绪
    if (!client.waitForReady(3)) {
        std::cerr << "Failed to connect to server!" << std::endl;
        server.stop();
        return 1;
    }
    std::cout << "  Connected to server." << std::endl;
    
    // 3. 运行测试
    std::cout << "\n[3] Running tests..." << std::endl;
    
    bool all_passed = true;
    all_passed &= testUnary(client.getChannel());
    all_passed &= testClientStream(client.getChannel());
    all_passed &= testServerStream(client.getChannel());
    all_passed &= testBidiStream(client.getChannel());
    
    // 4. 停止服务器
    std::cout << "\n[4] Stopping server..." << std::endl;
    server.stop();
    
    // 5. 输出结果
    std::cout << "\n=== Results ===" << std::endl;
    if (all_passed) {
        std::cout << "All tests PASSED!" << std::endl;
    } else {
        std::cerr << "Some tests FAILED!" << std::endl;
    }
    
    return all_passed ? 0 : 1;
}
