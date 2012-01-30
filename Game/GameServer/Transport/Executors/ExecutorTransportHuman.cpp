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

#include <Game/GameServer/Common/Constants.hpp>
#include <Game/GameServer/Transport/Executors/ExecutorTransportHuman.hpp>
#include <Language/Interface/ReplyBuilder.hpp>
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include <log4cpp/Category.hh>

using namespace GameServer::Persistence;
using namespace boost;
using namespace log4cpp;
using namespace std;

namespace Game
{

ExecutorTransportHuman::ExecutorTransportHuman(
    Server::IContextShrPtr const a_context
)
    : Executor(a_context)
{
}

void ExecutorTransportHuman::logExecutorStart() const
{
// TODO: Replace with Poco::Logger. Category::getInstance("Category").infoStream() << "Starting the ExecutorTransportHuman.";
}

bool ExecutorTransportHuman::getParameters(
    Language::ICommand::Handle a_request
)
{
    m_login = a_request->getLogin();
    m_password = a_request->getPassword();
    m_settlement_name_source = a_request->getParam("settlement_name_source");
    m_settlement_name_destination = a_request->getParam("settlement_name_destination");
    m_key = a_request->getParam("humankey");
    m_value_volume = a_request->getParam("volume");

    return true;
}

bool ExecutorTransportHuman::processParameters()
{
    try
    {
        m_volume = boost::lexical_cast<unsigned int>(m_value_volume);

        return true;
    }
    catch (std::range_error)
    {
        return false;
    }
}

bool ExecutorTransportHuman::authorize(
    IPersistenceShrPtr a_persistence
) const
{
    GameServer::Authorization::IAuthorizeUserToSettlementOperatorShrPtr authorize_operator =
        m_operator_abstract_factory->createAuthorizeUserToSettlementOperator();

    bool authorized_source(false), authorized_destination(false);

    // The transaction lifetime.
    {
        IConnectionShrPtr connection = a_persistence->getConnection();
        ITransactionShrPtr transaction = a_persistence->getTransaction(connection);

        GameServer::Authorization::AuthorizeUserToSettlementOperatorExitCode const exit_code =
            authorize_operator->authorizeUserToSettlement(transaction, m_user->getLogin(), m_settlement_name_source);

        if (exit_code.ok())
        {
            transaction->commit();
        }

        authorized_source = exit_code.m_authorized;
    }

    // The transaction lifetime.
    {
        IConnectionShrPtr connection = a_persistence->getConnection();
        ITransactionShrPtr transaction = a_persistence->getTransaction(connection);

        GameServer::Authorization::AuthorizeUserToSettlementOperatorExitCode const exit_code =
            authorize_operator->authorizeUserToSettlement(
                transaction, m_user->getLogin(), m_settlement_name_destination);

        if (exit_code.ok())
        {
            transaction->commit();
        }

        authorized_destination = exit_code.m_authorized;
    }

    return authorized_source && authorized_destination;
}

bool ExecutorTransportHuman::epochIsActive(
    IPersistenceShrPtr a_persistence
) const
{
    GameServer::Epoch::IGetEpochBySettlementNameOperatorShrPtr epoch_operator =
        m_operator_abstract_factory->createGetEpochBySettlementNameOperator();

    // The transaction lifetime.
    {
        IConnectionShrPtr connection = a_persistence->getConnection();
        ITransactionShrPtr transaction = a_persistence->getTransaction(connection);

        GameServer::Epoch::GetEpochBySettlementNameOperatorExitCode const exit_code =
            epoch_operator->getEpochBySettlementName(transaction, m_settlement_name_source);

        if (exit_code.ok())
        {
            transaction->commit();
        }

        return exit_code.m_epoch ? exit_code.m_epoch->getActive() : false;
    }
}

bool ExecutorTransportHuman::verifyWorldConfiguration(
    IPersistenceShrPtr a_persistence
) const
{
    return true;
}

Language::ICommand::Handle ExecutorTransportHuman::perform(
    IPersistenceShrPtr a_persistence
) const
{
    GameServer::Transport::ITransportHumanOperatorShrPtr transport_human_operator =
        m_operator_abstract_factory->createTransportHumanOperator();

    // The transaction lifetime.
    {
        IConnectionShrPtr connection = a_persistence->getConnection();
        ITransactionShrPtr transaction = a_persistence->getTransaction(connection);

        GameServer::Transport::TransportHumanOperatorExitCode const exit_code =
            transport_human_operator->transportHuman(transaction, m_settlement_name_source,
                m_settlement_name_destination, m_key, m_volume);

        if (exit_code.ok())
        {
            transaction->commit();
        }

        return produceReply(exit_code);
    }
}

Language::ICommand::Handle ExecutorTransportHuman::getBasicReply(
    unsigned int const a_status
) const
{
    Language::ReplyBuilder reply_builder;

    return reply_builder.buildTransportHumanReply(a_status);
}

Language::ICommand::Handle ExecutorTransportHuman::produceReply(
    GameServer::Transport::TransportHumanOperatorExitCode const & a_exit_code
) const
{
    Language::ReplyBuilder reply_builder;

    switch (a_exit_code.m_exit_code)
    {
        case GameServer::Transport::TRANSPORT_HUMAN_OPERATOR_EXIT_CODE_DESTINATION_SETTLEMENT_DOES_NOT_EXIST:
            return reply_builder.buildTransportHumanReply(REPLY_STATUS_OK,
                       TRANSPORT_HUMAN_DESTINATION_SETTLEMENT_DOES_NOT_EXIST);

        case GameServer::Transport::TRANSPORT_HUMAN_OPERATOR_EXIT_CODE_HUMAN_HAS_BEEN_TRANSPORTED:
            return reply_builder.buildTransportHumanReply(REPLY_STATUS_OK, TRANSPORT_HUMAN_HUMAN_HAS_BEEN_TRANSPORTED);

        case GameServer::Transport::TRANSPORT_HUMAN_OPERATOR_EXIT_CODE_NOT_ENOUGH_HUMANS:
            return reply_builder.buildTransportHumanReply(REPLY_STATUS_OK, TRANSPORT_HUMAN_NOT_ENOUGH_HUMANS);

        case GameServer::Transport::TRANSPORT_HUMAN_OPERATOR_EXIT_CODE_SETTLEMENTS_ARE_NOT_FROM_THE_SAME_LAND:
            return reply_builder.buildTransportHumanReply(REPLY_STATUS_OK,
                       TRANSPORT_HUMAN_SETTLEMENTS_ARE_NOT_FROM_THE_SAME_LAND);

        case GameServer::Transport::TRANSPORT_HUMAN_OPERATOR_EXIT_CODE_SOURCE_SETTLEMENT_DOES_NOT_EXIST:
            return reply_builder.buildTransportHumanReply(REPLY_STATUS_OK,
                       TRANSPORT_HUMAN_SOURCE_SETTLEMENT_DOES_NOT_EXIST);

        case GameServer::Transport::TRANSPORT_HUMAN_OPERATOR_EXIT_CODE_TRYING_TO_TRANSPORT_ZERO_HUMANS:
            return reply_builder.buildTransportHumanReply(REPLY_STATUS_OK,
                       TRANSPORT_HUMAN_TRYING_TO_TRANSPORT_ZERO_HUMANS);

        case GameServer::Transport::TRANSPORT_HUMAN_OPERATOR_EXIT_CODE_UNEXPECTED_ERROR:
            return reply_builder.buildTransportHumanReply(REPLY_STATUS_OK, TRANSPORT_HUMAN_UNEXPECTED_ERROR);

        default:
            return reply_builder.buildTransportHumanReply(REPLY_STATUS_OK,
                       METAMESSAGE_EVEN_MORE_UNEXPECTED_ERROR_UNKNOWN_EXIT_CODE);
    }
}

} // namespace Game
