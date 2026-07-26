/**
 * @file test_rpc.cpp
 * @brief RPC模块单元测试
 * @author yAlgo Team
 * @date 2025-12-07
 */

#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>

#include "rpc/grpc_server.h"
#include "rpc/grpc_client.h"
#include "rpc/rpc_utils.h"
#include "rpc/version.h"

// ============================================================
// 测试辅助：简单的Echo服务
// ============================================================

#include "echo.pb.h"
#include "echo.grpc.pb.h"

class TestEchoService final : public echo::EchoService::Service {
public:
    grpc::Status Echo(grpc::ServerContext* context,
                      const echo::EchoRequest* request,
                      echo::EchoResponse* response) override {
        response->set_message(request->message());
        response->set_seq(request->seq());
        response->set_server_id("test-server");
        return grpc::Status::OK;
    }
};

// ============================================================
// 测试用例
// ============================================================

const std::string TEST_ADDRESS = "localhost:50060";

/**
 * @brief 测试版本宏定义
 */
bool test_versionMacros() {
    if (YALGO_RPC_MAJOR_VERSION != 1) return false;
    if (YALGO_RPC_MINOR_VERSION != 0) return false;
    if (YALGO_RPC_PATCH_VERSION != 0) return false;
    if (std::string(YALGO_RPC_VERSION_STRING) != "1.0.0") return false;
    return true;
}

/**
 * @brief 测试GrpcServer创建和启动
 */
bool test_serverStartStop() {
    yalgo::rpc::GrpcServer server(TEST_ADDRESS);
    
    TestEchoService service;
    server.registerService(&service);
    
    // 测试同步启动
    if (!server.start()) return false;
    if (!server.isRunning()) return false;
    if (server.getAddress() != TEST_ADDRESS) return false;
    
    // 停止
    server.stop();
    if (server.isRunning()) return false;
    
    return true;
}

/**
 * @brief 测试GrpcServer异步启动
 */
bool test_serverAsyncStart() {
    std::string addr = "localhost:50061";
    yalgo::rpc::GrpcServer server(addr);
    
    TestEchoService service;
    server.registerService(&service);
    
    // 测试异步启动
    if (!server.startAsync()) return false;
    if (!server.isRunning()) return false;
    
    // 等待服务器就绪
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    server.stop();
    if (server.isRunning()) return false;
    
    return true;
}

/**
 * @brief 测试GrpcClient创建
 */
bool test_clientCreation() {
    yalgo::rpc::GrpcClient client(TEST_ADDRESS);
    
    if (client.getTarget() != TEST_ADDRESS) return false;
    if (!client.getChannel()) return false;
    
    return true;
}

/**
 * @brief 测试channel创建工具函数
 */
bool test_channelCreation() {
    auto channel = yalgo::rpc::createInsecureChannel("localhost:50062");
    if (!channel) return false;
    
    grpc::ChannelArguments args;
    auto channel2 = yalgo::rpc::createInsecureChannel("localhost:50063", args);
    if (!channel2) return false;
    
    return true;
}

/**
 * @brief 测试超时设置
 */
bool test_timeoutSetting() {
    grpc::ClientContext context;
    yalgo::rpc::setTimeout(context, 1000);
    // 无异常即通过
    return true;
}

/**
 * @brief 测试状态检查工具
 */
bool test_statusCheck() {
    grpc::Status okStatus = grpc::Status::OK;
    if (!yalgo::rpc::isOk(okStatus)) return false;
    
    grpc::Status errStatus(grpc::StatusCode::NOT_FOUND, "not found");
    if (yalgo::rpc::isOk(errStatus)) return false;
    
    std::string detail = yalgo::rpc::getStatusDetail(errStatus);
    if (detail.find("code=5") == std::string::npos) return false;
    if (detail.find("not found") == std::string::npos) return false;
    
    return true;
}

/**
 * @brief 测试metadata设置
 */
bool test_metadataSetting() {
    yalgo::rpc::GrpcClient client(TEST_ADDRESS);
    
    client.setMetadata("authorization", "Bearer token123");
    client.setMetadata("x-custom", "value");
    client.clearMetadata();
    // 无异常即通过
    return true;
}

/**
 * @brief 测试LogInterceptor
 */
bool test_logInterceptor() {
    yalgo::rpc::LogInterceptor interceptor;
    
    // 测试beforeCall和afterCall不抛异常
    interceptor.beforeCall("test/method");
    interceptor.afterCall("test/method", grpc::Status::OK);
    interceptor.afterCall("test/method", grpc::Status(grpc::StatusCode::INTERNAL, "error"));
    
    return true;
}

/**
 * @brief 测试完整的server-client通信
 */
bool test_fullCommunication() {
    std::string addr = "localhost:50064";
    
    // 启动服务器
    yalgo::rpc::GrpcServer server(addr);
    TestEchoService service;
    server.registerService(&service);
    
    if (!server.start()) return false;
    
    // 等待服务器就绪
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    // 创建客户端
    yalgo::rpc::GrpcClient client(addr);
    if (!client.waitForReady(3)) {
        server.stop();
        return false;
    }
    
    // 发送请求
    auto stub = echo::EchoService::NewStub(client.getChannel());
    grpc::ClientContext context;
    echo::EchoRequest request;
    echo::EchoResponse response;
    
    request.set_message("test message");
    request.set_seq(42);
    
    grpc::Status status = stub->Echo(&context, request, &response);
    
    server.stop();
    
    if (!status.ok()) return false;
    if (response.message() != "test message") return false;
    if (response.seq() != 42) return false;
    if (response.server_id() != "test-server") return false;
    
    return true;
}

// ============================================================
// 主函数 - 运行所有RPC模块测试
// ============================================================

int main() {
    std::cout << "Running rpc module unit tests..." << std::endl;
    
    bool all_passed = true;
    
    auto runTest = [&](const char* name, bool (*testFunc)()) {
        bool result = testFunc();
        if (!result) {
            std::cerr << "  FAIL: " << name << std::endl;
        }
        all_passed &= result;
    };
    
    runTest("versionMacros", test_versionMacros);
    runTest("serverStartStop", test_serverStartStop);
    runTest("serverAsyncStart", test_serverAsyncStart);
    runTest("clientCreation", test_clientCreation);
    runTest("channelCreation", test_channelCreation);
    runTest("timeoutSetting", test_timeoutSetting);
    runTest("statusCheck", test_statusCheck);
    runTest("metadataSetting", test_metadataSetting);
    runTest("logInterceptor", test_logInterceptor);
    runTest("fullCommunication", test_fullCommunication);
    
    if (all_passed) {
        std::cout << "All rpc tests passed!" << std::endl;
        return 0;
    } else {
        std::cerr << "Some rpc tests failed!" << std::endl;
        return 1;
    }
}
