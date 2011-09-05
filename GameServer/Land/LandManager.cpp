// Copyright (C) 2010 and 2011 Marcin Arkadiusz Skrobiranda.
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

#include "LandManager.hpp"

using namespace GameServer::Epoch;
using namespace GameServer::Persistency;
using namespace GameServer::User;
using namespace GameServer::World;
using namespace boost;
using namespace std;

namespace GameServer
{
namespace Land
{

LandManager::LandManager(
    ILandManagerAccessorAutPtr a_accessor
)
    : m_accessor(a_accessor)
{
}

bool LandManager::createLand(
    ITransactionShrPtr         a_transaction,
    IDUser             const & a_id_user,
    IDWorld            const & a_id_world,
    IDEpoch            const & a_id_epoch,
    string             const & a_name
) const
{
    try
    {
        m_accessor->insertRecord(a_transaction, a_id_user, a_id_world, a_id_epoch, a_name);

        return true;
    }
    catch (std::exception const & e)
    {
        return false;
    }
}

bool LandManager::deleteLand(
    ITransactionShrPtr         a_transaction,
    IDLand             const & a_id_land
) const
{
    try
    {
        m_accessor->deleteRecord(a_transaction, a_id_land);
        return true;
    }
    catch (std::exception const & e)
    {
        return false;
    }
}

LandShrPtr LandManager::getLand(
    ITransactionShrPtr         a_transaction,
    IDLand             const & a_id_land
) const
{
    return prepareResultGetLand(m_accessor->getRecord(a_transaction, a_id_land));
}

LandShrPtr LandManager::getLand(
    ITransactionShrPtr         a_transaction,
    string             const & a_name,
    IDUser             const & a_id_user
) const
{
    return prepareResultGetLand(m_accessor->getRecord(a_transaction, a_name, a_id_user));
}

LandShrPtr LandManager::getLand(
    ITransactionShrPtr         a_transaction,
    string             const & a_name,
    IDWorld            const & a_id_world
) const
{
    return prepareResultGetLand(m_accessor->getRecord(a_transaction, a_name, a_id_world));
}

LandMap LandManager::getLands(
    ITransactionShrPtr a_transaction
) const
{
    return prepareResultGetLands(m_accessor->getRecords(a_transaction));
}

LandMap LandManager::getLands(
    ITransactionShrPtr         a_transaction,
    IDUser             const & a_id_user
) const
{
    return prepareResultGetLands(m_accessor->getRecords(a_transaction, a_id_user));
}

LandMap LandManager::getLands(
    ITransactionShrPtr         a_transaction,
    IDWorld            const & a_id_world
) const
{
    return prepareResultGetLands(m_accessor->getRecords(a_transaction, a_id_world));
}

LandMap LandManager::getLands(
    ITransactionShrPtr         a_transaction,
    IDUser             const & a_id_user,
    IDWorld            const & a_id_world
) const
{
    return prepareResultGetLands(m_accessor->getRecords(a_transaction, a_id_user, a_id_world));
}

void LandManager::markGranted(
    ITransactionShrPtr         a_transaction,
    IDLand             const & a_id_land
) const
{
    m_accessor->markGranted(a_transaction, a_id_land);
}

LandShrPtr LandManager::prepareResultGetLand(
    LandRecordShrPtr a_record
) const
{
    return a_record ? make_shared<Land>(*a_record) : LandShrPtr();
}

LandMap LandManager::prepareResultGetLands(
    LandRecordMap a_records
) const
{
    LandMap result;

    for (LandRecordMap::iterator it = a_records.begin(); it != a_records.end(); ++it)
    {
        if (it->second)
        {
            LandShrPtr land = make_shared<Land>(*it->second);
            LandPair pair(it->second->getIDLand(), land);
            result.insert(pair);
        }
    }

    return result;
}

} // namespace Land
} // namespace GameServer
