/**
 * @file echo_service.h
 * @brief Echo gRPC服务定义（server/client共用）
 * @author yAlgo Team
 * @date 2025-12-07
 */

#pragma once

#include "echo.pb.h"
#include "echo.grpc.pb.h"
#include "log/async_logger.h"

class EchoServiceImpl final : public echo::EchoService::Service {
public:
    grpc::Status Echo(grpc::ServerContext* context,
                      const echo::EchoRequest* request,
                      echo::EchoResponse* response) override {
        response->set_message(request->message());
        response->set_seq(request->seq());
        response->set_server_id("echo-server-1");
        YLOG_INFO("Echo received: seq=%d msg='%s'", request->seq(), request->message().c_str());
        return grpc::Status::OK;
    }

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
