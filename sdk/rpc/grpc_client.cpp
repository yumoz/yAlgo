/**
 * @file grpc_client.cpp
 * @brief gRPC客户端封装实现
 * @author yAlgo Team
 * @date 2025-12-07
 */

#include "grpc_client.h"
#include "log/async_logger.h"

namespace yalgo {
namespace rpc {

GrpcClient::GrpcClient(const std::string& target)
    : target_(target) {
    channel_ = grpc::CreateChannel(target, grpc::InsecureChannelCredentials());
    YLOG_INFO("gRPC client created for target: %s", target.c_str());
}

GrpcClient::GrpcClient(const std::string& target, const grpc::ChannelArguments& args)
    : target_(target) {
    channel_ = grpc::CreateCustomChannel(target, grpc::InsecureChannelCredentials(), args);
    YLOG_INFO("gRPC client created for target: %s", target.c_str());
}

GrpcClient::~GrpcClient() {
}

std::shared_ptr<grpc::Channel> GrpcClient::getChannel() const {
    return channel_;
}

grpc_connectivity_state GrpcClient::getChannelState() const {
    return channel_->GetState(true);
}

bool GrpcClient::waitForReady(int deadline_secs) {
    auto deadline = std::chrono::system_clock::now() + std::chrono::seconds(deadline_secs);
    return channel_->WaitForConnected(deadline);
}

void GrpcClient::setMetadata(const std::string& key, const std::string& value) {
    metadata_.emplace_back(key, value);
}

void GrpcClient::clearMetadata() {
    metadata_.clear();
}

const std::string& GrpcClient::getTarget() const {
    return target_;
}

} // namespace rpc
} // namespace yalgo
