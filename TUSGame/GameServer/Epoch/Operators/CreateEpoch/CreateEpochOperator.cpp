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

#include <TUSGame/GameServer/Epoch/Operators/CreateEpoch/CreateEpochOperator.hpp>

using namespace GameServer::Persistence;
using namespace GameServer::World;
using namespace std;

namespace GameServer
{
namespace Epoch
{

CreateEpochOperator::CreateEpochOperator(
    IEpochPersistenceFacadeShrPtr a_epoch_persistence_facade,
    IWorldPersistenceFacadeShrPtr a_world_persistence_facade
)
    : m_epoch_persistence_facade(a_epoch_persistence_facade),
      m_world_persistence_facade(a_world_persistence_facade)
{
}

CreateEpochOperatorExitCode CreateEpochOperator::createEpoch(
    ITransactionShrPtr       a_transaction,
    string             const a_world_name,
    string             const a_epoch_name
) const
{
    try
    {
        // Verify if the world exists.
        if (!m_world_persistence_facade->getWorld(a_transaction, a_world_name))
        {
            return CreateEpochOperatorExitCode(CREATE_EPOCH_OPERATOR_EXIT_CODE_WORLD_DOES_NOT_EXIST);
        }

        bool const result = m_epoch_persistence_facade->createEpoch(a_transaction, a_world_name, a_epoch_name);

        return (result) ? CreateEpochOperatorExitCode(CREATE_EPOCH_OPERATOR_EXIT_CODE_EPOCH_HAS_BEEN_CREATED)
                        : CreateEpochOperatorExitCode(CREATE_EPOCH_OPERATOR_EXIT_CODE_EPOCH_HAS_NOT_BEEN_CREATED);
    }
    catch (...)
    {
        return CreateEpochOperatorExitCode(CREATE_EPOCH_OPERATOR_EXIT_CODE_UNEXPECTED_ERROR);
    }
}

} // namespace Epoch
} // namespace GameServer
