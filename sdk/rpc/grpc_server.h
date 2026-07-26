/**
 * @file grpc_server.h
 * @brief gRPC服务端封装，支持双向流式通信
 * @author yAlgo Team
 * @date 2025-12-07
 */

#pragma once

#include "rpc_exports.h"
#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <thread>

namespace yalgo {
namespace rpc {

/**
 * @brief gRPC服务器封装类
 * 
 * 封装 grpc::Server 的生命周期管理，支持：
 * - 注册多个 grpc::Service 实现
 * - server-streaming 和 bidi-streaming 模式
 * - 自动日志集成（通过 yalgo::log）
 */
class RPC_API GrpcServer {
public:
    /**
     * @brief 构造函数
     * @param address 绑定地址，格式如 "0.0.0.0:50051"
     */
    explicit GrpcServer(const std::string& address);

    /**
     * @brief 析构函数，自动停止服务器
     */
    ~GrpcServer();

    // 禁止拷贝和移动
    GrpcServer(const GrpcServer&) = delete;
    GrpcServer& operator=(const GrpcServer&) = delete;

    /**
     * @brief 注册gRPC服务实现
     * @param service 服务实现指针（生命周期由调用者管理）
     */
    void registerService(grpc::Service* service);

    /**
     * @brief 启动服务器（阻塞当前线程）
     * @return true 启动成功，false 启动失败
     */
    bool start();

    /**
     * @brief 非阻塞启动服务器（在后台线程运行）
     * @return true 启动成功，false 启动失败
     */
    bool startAsync();

    /**
     * @brief 停止服务器
     */
    void stop();

    /**
     * @brief 等待服务器停止（阻塞直到服务器关闭）
     */
    void waitForShutdown();

    /**
     * @brief 检查服务器是否正在运行
     * @return true 正在运行，false 已停止
     */
    bool isRunning() const;

    /**
     * @brief 获取服务器监听地址
     * @return 地址字符串
     */
    const std::string& getAddress() const;

    /**
     * @brief 设置服务器构建选项
     * @param option 构建选项
     */
    void setBuilderOptions(const grpc::ServerBuilder& option);

private:
    std::string address_;
    std::vector<grpc::Service*> services_;
    std::unique_ptr<grpc::Server> server_;
    std::unique_ptr<std::thread> serverThread_;
    bool running_;
};

} // namespace rpc
} // namespace yalgo
