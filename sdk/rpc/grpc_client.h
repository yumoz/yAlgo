/**
 * @file grpc_client.h
 * @brief gRPC客户端封装，支持四种调用模式
 * @author yAlgo Team
 * @date 2025-12-07
 */

#pragma once

#include "rpc_exports.h"
#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include <functional>

namespace yalgo {
namespace rpc {

/**
 * @brief gRPC客户端封装类
 * 
 * 封装 grpc::Channel 的创建和管理，支持：
 * - unary 调用
 * - server-streaming 调用
 * - client-streaming 调用
 * - bidi-streaming 调用
 */
class RPC_API GrpcClient {
public:
    /**
     * @brief 构造函数
     * @param target 服务端地址，格式如 "localhost:50051"
     */
    explicit GrpcClient(const std::string& target);

    /**
     * @brief 构造函数（带自定义channel参数）
     * @param target 服务端地址
     * @param args channel构建参数
     */
    GrpcClient(const std::string& target, const grpc::ChannelArguments& args);

    /**
     * @brief 析构函数
     */
    ~GrpcClient();

    /**
     * @brief 获取底层gRPC channel
     * @return channel指针
     */
    std::shared_ptr<grpc::Channel> getChannel() const;

    /**
     * @brief 检查channel连接状态
     * @return 连接状态
     */
    grpc_connectivity_state getChannelState() const;

    /**
     * @brief 等待channel就绪
     * @param deadline_secs 等待超时时间（秒）
     * @return true 就绪，false 超时
     */
    bool waitForReady(int deadline_secs = 5);

    /**
     * @brief 设置自定义metadata
     * @param key 元数据键
     * @param value 元数据值
     */
    void setMetadata(const std::string& key, const std::string& value);

    /**
     * @brief 清除所有自定义metadata
     */
    void clearMetadata();

    /**
     * @brief 获取目标地址
     * @return 地址字符串
     */
    const std::string& getTarget() const;

private:
    std::string target_;
    std::shared_ptr<grpc::Channel> channel_;
    grpc::ClientContext baseContext_;
    std::vector<std::pair<std::string, std::string>> metadata_;
};

} // namespace rpc
} // namespace yalgo
