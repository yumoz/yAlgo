/**
 * @file rpc_utils.cpp
 * @brief gRPC辅助工具函数实现
 * @author yAlgo Team
 * @date 2025-12-07
 */

#include "rpc_utils.h"
#include "log/async_logger.h"
#include <sstream>

namespace yalgo {
namespace rpc {

std::shared_ptr<grpc::Channel> createInsecureChannel(const std::string& target) {
    return grpc::CreateChannel(target, grpc::InsecureChannelCredentials());
}

std::shared_ptr<grpc::Channel> createSecureChannel(
    const std::string& target,
    const std::string& caCertPath,
    const std::string& clientCertPath,
    const std::string& clientKeyPath
) {
    grpc::SslCredentialsOptions sslOpts;
    sslOpts.pem_root_certs = caCertPath; // 实际应读取文件内容

    if (!clientCertPath.empty() && !clientKeyPath.empty()) {
        sslOpts.pem_private_key = clientKeyPath;
        sslOpts.pem_cert_chain = clientCertPath;
    }

    auto creds = grpc::SslCredentials(sslOpts);
    return grpc::CreateChannel(target, creds);
}

std::shared_ptr<grpc::Channel> createInsecureChannel(
    const std::string& target,
    const grpc::ChannelArguments& args
) {
    return grpc::CreateCustomChannel(target, grpc::InsecureChannelCredentials(), args);
}

void setTimeout(grpc::ClientContext& context, int timeout_ms) {
    context.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds(timeout_ms));
}

void setTimeout(grpc::ClientContext& context, std::chrono::milliseconds duration) {
    context.set_deadline(std::chrono::system_clock::now() + duration);
}

bool isOk(const grpc::Status& status) {
    return status.ok();
}

std::string getStatusDetail(const grpc::Status& status) {
    std::ostringstream oss;
    oss << "gRPC status: code=" << static_cast<int>(status.error_code())
        << ", message=" << status.error_message()
        << ", detail=" << status.error_details();
    return oss.str();
}

// LogInterceptor 实现

void LogInterceptor::beforeCall(const char* method) {
    YLOG_DEBUG("RPC call starting: %s", method);
}

void LogInterceptor::afterCall(const char* method, const grpc::Status& status) {
    if (status.ok()) {
        YLOG_DEBUG("RPC call completed OK: %s", method);
    } else {
        YLOG_WARN("RPC call failed: %s status=%d", method, static_cast<int>(status.error_code()));
    }
}

} // namespace rpc
} // namespace yalgo
