/**
 * @file grpc_server.cpp
 * @brief gRPC服务端封装实现
 * @author yAlgo Team
 * @date 2025-12-07
 */

#include "grpc_server.h"
#include "log/async_logger.h"
#include <sstream>

namespace yalgo {
namespace rpc {

GrpcServer::GrpcServer(const std::string& address)
    : address_(address)
    , running_(false) {
}

GrpcServer::~GrpcServer() {
    stop();
}

void GrpcServer::registerService(grpc::Service* service) {
    if (service) {
        services_.push_back(service);
    }
}

bool GrpcServer::start() {
    if (running_) {
        YLOG_WARN("RPC server already running on %s", address_.c_str());
        return false;
    }

    grpc::ServerBuilder builder;
    builder.AddListeningPort(address_, grpc::InsecureServerCredentials());

    for (auto* service : services_) {
        builder.RegisterService(service);
    }

    server_ = builder.BuildAndStart();
    if (!server_) {
        YLOG_ERROR("Failed to start RPC server on %s", address_.c_str());
        return false;
    }

    running_ = true;
    YLOG_INFO("RPC server started on %s", address_.c_str());
    return true;
}

bool GrpcServer::startAsync() {
    if (running_) {
        YLOG_WARN("RPC server already running on %s", address_.c_str());
        return false;
    }

    if (!start()) {
        return false;
    }

    serverThread_ = std::make_unique<std::thread>([this]() {
        server_->Wait();
        running_ = false;
    });

    return true;
}

void GrpcServer::stop() {
    if (!running_) {
        return;
    }

    YLOG_INFO("Stopping RPC server on %s", address_.c_str());
    
    if (server_) {
        server_->Shutdown();
    }

    if (serverThread_ && serverThread_->joinable()) {
        serverThread_->join();
    }

    running_ = false;
    YLOG_INFO("RPC server stopped on %s", address_.c_str());
}

void GrpcServer::waitForShutdown() {
    if (server_) {
        server_->Wait();
    }
}

bool GrpcServer::isRunning() const {
    return running_;
}

const std::string& GrpcServer::getAddress() const {
    return address_;
}

void GrpcServer::setBuilderOptions(const grpc::ServerBuilder& option) {
    // 注意：此方法需要在start()之前调用
    // 当前实现中builder在start()内部创建，此方法保留供未来扩展
    YLOG_WARN("setBuilderOptions called - implementation pending refactoring");
}

} // namespace rpc
} // namespace yalgo
