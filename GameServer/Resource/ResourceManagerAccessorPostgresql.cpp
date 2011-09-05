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

#include "../Persistency/TransactionPostgresql.hpp"
#include "ResourceManagerAccessorPostgresql.hpp"

using namespace GameServer::Common;
using namespace GameServer::Persistency;
using namespace boost;
using namespace std;

namespace GameServer
{
namespace Resource
{

void ResourceManagerAccessorPostgresql::insertRecord(
    ITransactionShrPtr         a_transaction,
    IDHolder           const & a_id_holder,
    Key                const & a_key,
    Volume             const & a_volume
) const
{
    TransactionPostgresqlShrPtr transaction = shared_dynamic_cast<TransactionPostgresql>(a_transaction);
    pqxx::transaction<> & backbone_transaction = transaction->getBackboneTransaction();

    string query = "INSERT INTO "
                   + getTableName(a_id_holder)
                   + "(id_holder, id_resource, volume) VALUES("
                   + backbone_transaction.quote(a_id_holder.getValue2()) + ", "
                   + backbone_transaction.quote(a_key.getInternalKey().get<0>().getValue()) + ", "
                   + backbone_transaction.quote(a_volume) + ")";

    pqxx::result result = backbone_transaction.exec(query);
}

void ResourceManagerAccessorPostgresql::deleteRecord(
    ITransactionShrPtr         a_transaction,
    IDHolder           const & a_id_holder,
    Key                const & a_key
) const
{
    TransactionPostgresqlShrPtr transaction = shared_dynamic_cast<TransactionPostgresql>(a_transaction);
    pqxx::transaction<> & backbone_transaction = transaction->getBackboneTransaction();

    string query = "DELETE FROM "
                   + getTableName(a_id_holder)
                   + " WHERE id_holder = " + backbone_transaction.quote(a_id_holder.getValue2())
                   + " AND id_resource = " + backbone_transaction.quote(a_key.getInternalKey().get<0>().getValue());

    pqxx::result result = backbone_transaction.exec(query);
}

ResourceWithVolumeRecordShrPtr ResourceManagerAccessorPostgresql::getRecord(
    ITransactionShrPtr         a_transaction,
    IDHolder           const & a_id_holder,
    Key                const & a_key
) const
{
    TransactionPostgresqlShrPtr transaction = shared_dynamic_cast<TransactionPostgresql>(a_transaction);
    pqxx::transaction<> & backbone_transaction = transaction->getBackboneTransaction();

    string query = "SELECT volume FROM "
                   + getTableName(a_id_holder)
                   + " WHERE id_holder = " + backbone_transaction.quote(a_id_holder.getValue2())
                   + " AND id_resource = " + backbone_transaction.quote(a_key.getInternalKey().get<0>().getValue());

    pqxx::result result = backbone_transaction.exec(query);

    if (result.size() > 0)
    {
        Volume volume;
        result[0]["volume"].to(volume);
        return make_shared<ResourceWithVolumeRecord>(a_id_holder, a_key, volume);
    }
    else
    {
        return ResourceWithVolumeRecordShrPtr();
    }
}

ResourceWithVolumeRecordMap ResourceManagerAccessorPostgresql::getRecords(
    ITransactionShrPtr         a_transaction,
    IDHolder           const & a_id_holder
) const
{
    TransactionPostgresqlShrPtr transaction = shared_dynamic_cast<TransactionPostgresql>(a_transaction);
    pqxx::transaction<> & backbone_transaction = transaction->getBackboneTransaction();

    string query = "SELECT * FROM "
                   + getTableName(a_id_holder)
                   + " WHERE id_holder = " + backbone_transaction.quote(a_id_holder.getValue2());

    pqxx::result result = backbone_transaction.exec(query);

    ResourceWithVolumeRecordMap records;

    // Fake types for libpqxx.
    int integer;

    IDResource id_resource;
    Volume volume;

    for (pqxx::result::const_iterator it = result.begin(); it != result.end(); ++it)
    {
        id_resource = it["id_resource"].as(integer);
        volume = it["volume"].as(integer);

        Key key(id_resource);

        ResourceWithVolumeRecordShrPtr record = make_shared<ResourceWithVolumeRecord>(a_id_holder, key, volume);

        ResourceWithVolumeRecordPair pair(key, record);

        records.insert(pair);
    }

    return records;
}

void ResourceManagerAccessorPostgresql::increaseVolume(
    ITransactionShrPtr         a_transaction,
    IDHolder           const & a_id_holder,
    Key                const & a_key,
    Volume             const & a_volume
) const
{
    TransactionPostgresqlShrPtr transaction = shared_dynamic_cast<TransactionPostgresql>(a_transaction);
    pqxx::transaction<> & backbone_transaction = transaction->getBackboneTransaction();

    string query = "UPDATE "
                   + getTableName(a_id_holder)
                   + " SET volume = volume + " + backbone_transaction.quote(a_volume)
                   + " WHERE id_holder = " + backbone_transaction.quote(a_id_holder.getValue2())
                   + " AND id_resource = " + backbone_transaction.quote(a_key.getInternalKey().get<0>().getValue());

    pqxx::result result = backbone_transaction.exec(query);
}

void ResourceManagerAccessorPostgresql::decreaseVolume(
    ITransactionShrPtr         a_transaction,
    IDHolder           const & a_id_holder,
    Key                const & a_key,
    Volume             const & a_volume
) const
{
    TransactionPostgresqlShrPtr transaction = shared_dynamic_cast<TransactionPostgresql>(a_transaction);
    pqxx::transaction<> & backbone_transaction = transaction->getBackboneTransaction();

    string query = "UPDATE "
                   + getTableName(a_id_holder)
                   + " SET volume = volume - " + backbone_transaction.quote(a_volume)
                   + " WHERE id_holder = " + backbone_transaction.quote(a_id_holder.getValue2())
                   + " AND id_resource = " + backbone_transaction.quote(a_key.getInternalKey().get<0>().getValue());

    pqxx::result result = backbone_transaction.exec(query);
}

string ResourceManagerAccessorPostgresql::getTableName(
    IDHolder const & a_holder_class
) const
{
    return string("resources_settlement");
}

} // namespace Resource
} // namespace GameServer
