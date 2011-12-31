// Copyright (C) 2010, 2011 and 2012 Marcin Arkadiusz Skrobiranda.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of the project nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

#ifndef NETWORK_XMLRPCSERVER_REQUEST_EXECUTORS_EXECUTORTRANSPORTRESOURCE_HPP
#define NETWORK_XMLRPCSERVER_REQUEST_EXECUTORS_EXECUTORTRANSPORTRESOURCE_HPP

#include <TUSGame/GameServer/Resource/Volume.hpp>
#include <TUSGame/GameServer/Transport/Operators/TransportResource/TransportResourceOperatorExitCode.hpp>
#include <TUSServer/Network/XmlRPCServer/Request/Executors/Executor.hpp>

namespace Network
{
namespace XmlRPCServer
{
namespace Request
{
namespace Executors
{

/**
 * @brief The ExecutorTransportResource executor.
 *
 * <?xml version=\"1.0\"?>
 * <request id=\"REQUEST_ID_TRANSPORT_RESOURCE\">
 *     <user>
 *         <login    value=\"#\" />
 *         <password value=\"#\" />
 *     </user>
 *     <parameters>
 *         <settlement_name_source      value=\"#\" />
 *         <settlement_name_destination value=\"#\" />
 *         <resourcekey                 value=\"#\" />
 *         <volume                      value=\"#\" />
 *     </parameters>
 * </request>
 *
 * <?xml version=\"1.0\"?>
 * <reply id=\"REPLY_ID_TRANSPORT_RESOURCE">
 *     <status value=\"#\" />
 *     <parameters>
 *         <message value=\"#\" />
 *     </parameters>
 * </reply>
 */
class ExecutorTransportResource
    : public Executor
{
public:
    /**
     * @brief Ctor.
     *
     * @param a_context The context of the server.
     */
    ExecutorTransportResource(
        IContextShrPtr const a_context
    );

private:
    /**
     * @brief Logs the start of the executor.
     */
    virtual void logExecutorStart() const;

    /**
     * @brief Gets parameters from the request.
     *
     * @param a_request The request.
     *
     * @return True if all parameters have been got, false otherwise.
     */
    virtual bool getParameters(
        XmlRPCCommon::Request::RequestShrPtr a_request
    );

    /**
     * @brief Process parameters from the request.
     *
     * @return True if all parameters have been processed, false otherwise.
     */
    virtual bool processParameters();

    /**
     * @brief Authorizes the user.
     *
     * @param a_persistence The persistence.
     *
     * @return True if user has been authorized, false otherwise.
     */
    virtual bool authorize(
        GameServer::Persistence::IPersistenceShrPtr a_persistence
    ) const;

    /**
     * @brief Verifies whether the epoch is active.
     *
     * @param a_persistence The persistence.
     *
     * @return True if the epoch is active, false otherwise.
     */
    virtual bool epochIsActive(
        GameServer::Persistence::IPersistenceShrPtr a_persistence
    ) const;

    /**
     * @brief Verifies whether the world configuration allows an action.
     *
     * @param a_persistence The persistence.
     *
     * @return True if the action is allowed, false otherwise.
     */
    virtual bool verifyWorldConfiguration(
        GameServer::Persistence::IPersistenceShrPtr a_persistence
    ) const;

    /**
     * @brief Performs the main operation.
     *
     * @return The reply.
     */
    virtual XmlRPCCommon::Reply::ReplyShrPtr perform(
        GameServer::Persistence::IPersistenceShrPtr a_persistence
    ) const;

    /**
     * @brief Produces the basic reply with a given status.
     *
     * @param a_status The status of the reply.
     *
     * @return The reply.
     */
    virtual XmlRPCCommon::Reply::ReplyShrPtr getBasicReply(
        unsigned int const a_status
    ) const;

    /**
     * @brief Produces the reply.
     *
     * @param a_exit_code The exit code.
     *
     * @return The reply.
     */
    XmlRPCCommon::Reply::ReplyShrPtr produceReply(
        GameServer::Transport::TransportResourceOperatorExitCode const & a_exit_code
    ) const;

    /**
     * @brief The key of the resource.
     */
    std::string m_key;

    /**
     * @brief The value of the volume.
     */
    unsigned int m_value_volume;

    /**
     * @brief The name of the source settlement.
     */
    std::string m_settlement_name_source;

    /**
     * @brief The name of the destination settlement.
     */
    std::string m_settlement_name_destination;

    /**
     * @brief The volume.
     */
    GameServer::Resource::Volume m_volume;
};

} // namespace Executors
} // namespace Request
} // namespace XmlRPCServer
} // namespace Network

#endif // NETWORK_XMLRPCSERVER_REQUEST_EXECUTORS_EXECUTORTRANSPORTRESOURCE_HPP
