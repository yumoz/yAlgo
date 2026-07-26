/**
 * @file rpc_utils.h
 * @brief gRPC辅助工具函数和宏
 * @author yAlgo Team
 * @date 2025-12-07
 */

#pragma once

#include "rpc_exports.h"
#include <grpcpp/grpcpp.h>
#include <string>
#include <chrono>

namespace yalgo {
namespace rpc {

/**
 * @brief 创建不安全的gRPC channel（无TLS）
 * @param target 服务端地址，如 "localhost:50051"
 * @return channel指针
 */
RPC_API std::shared_ptr<grpc::Channel> createInsecureChannel(const std::string& target);

/**
 * @brief 创建带TLS的gRPC channel
 * @param target 服务端地址
 * @param caCertPath CA证书路径
 * @param clientCertPath 客户端证书路径（可选，用于mTLS）
 * @param clientKeyPath 客户端私钥路径（可选，用于mTLS）
 * @return channel指针
 */
RPC_API std::shared_ptr<grpc::Channel> createSecureChannel(
    const std::string& target,
    const std::string& caCertPath,
    const std::string& clientCertPath = "",
    const std::string& clientKeyPath = ""
);

/**
 * @brief 创建带自定义channel参数的不安全channel
 * @param target 服务端地址
 * @param args channel参数
 * @return channel指针
 */
RPC_API std::shared_ptr<grpc::Channel> createInsecureChannel(
    const std::string& target,
    const grpc::ChannelArguments& args
);

/**
 * @brief 设置gRPC调用超时时间
 * @param context 客户端上下文
 * @param timeout_ms 超时时间（毫秒）
 */
RPC_API void setTimeout(grpc::ClientContext& context, int timeout_ms);

/**
 * @brief 设置gRPC调用超时时间（chrono版本）
 * @param context 客户端上下文
 * @param duration 超时时长
 */
RPC_API void setTimeout(grpc::ClientContext& context, std::chrono::milliseconds duration);

/**
 * @brief 检查gRPC状态是否为OK
 * @param status gRPC状态
 * @return true 状态OK
 */
RPC_API bool isOk(const grpc::Status& status);

/**
 * @brief 获取gRPC状态的详细错误信息
 * @param status gRPC状态
 * @return 错误信息字符串
 */
RPC_API std::string getStatusDetail(const grpc::Status& status);

/**
 * @brief 日志拦截器
 * 
 * 自动记录gRPC调用的请求和响应信息。
 * 使用方法：创建实例并传入 GrpcClient 的构造函数。
 */
class RPC_API LogInterceptor {
public:
    virtual ~LogInterceptor() = default;

    /**
     * @brief 在调用前执行的操作（记录方法名）
     * @param method 方法名
     */
    virtual void beforeCall(const char* method);

    /**
     * @brief 在调用后执行的操作（记录状态）
     * @param method 方法名
     * @param status 调用状态
     */
    virtual void afterCall(const char* method, const grpc::Status& status);
};

} // namespace rpc
} // namespace yalgo

