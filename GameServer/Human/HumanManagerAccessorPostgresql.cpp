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
#include "HumanManagerAccessorPostgresql.hpp"

using namespace GameServer::Common;
using namespace GameServer::Persistency;
using namespace boost;
using namespace std;

namespace GameServer
{
namespace Human
{

void HumanManagerAccessorPostgresql::insertRecord(
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
                   + "(id_holder, id_human_class, id_human, experience, volume) VALUES("
                   + backbone_transaction.quote(a_id_holder.getValue2()) + ", "
                   + backbone_transaction.quote(a_key.getInternalKey().get<0>().getValue1()) + ", "
                   + backbone_transaction.quote(a_key.getInternalKey().get<0>().getValue2()) + ", "
                   + backbone_transaction.quote(a_key.getInternalKey().get<1>().getValue()) + ", "
                   + backbone_transaction.quote(a_volume) + ")";

    pqxx::result result = backbone_transaction.exec(query);
}

void HumanManagerAccessorPostgresql::deleteRecord(
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
                   + " AND id_human_class = " + backbone_transaction.quote(a_key.getInternalKey().get<0>().getValue1())
                   + " AND id_human = " + backbone_transaction.quote(a_key.getInternalKey().get<0>().getValue2())
                   + " AND experience = " + backbone_transaction.quote(a_key.getInternalKey().get<1>().getValue());

    pqxx::result result = backbone_transaction.exec(query);
}

HumanWithVolumeRecordShrPtr HumanManagerAccessorPostgresql::getRecord(
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
                   + " AND id_human_class = " + backbone_transaction.quote(a_key.getInternalKey().get<0>().getValue1())
                   + " AND id_human = " + backbone_transaction.quote(a_key.getInternalKey().get<0>().getValue2())
                   + " AND experience = " + backbone_transaction.quote(a_key.getInternalKey().get<1>().getValue());

    pqxx::result result = backbone_transaction.exec(query);

    if (result.size() > 0)
    {
        Volume volume;
        result[0]["volume"].to(volume);
        return make_shared<HumanWithVolumeRecord>(a_id_holder, a_key, volume);
    }
    else
    {
        return HumanWithVolumeRecordShrPtr();
    }
}

HumanWithVolumeRecordMap HumanManagerAccessorPostgresql::getRecords(
    ITransactionShrPtr         a_transaction,
    IDHolder           const & a_id_holder,
    IDHuman            const & a_id_human
) const
{
    TransactionPostgresqlShrPtr transaction = shared_dynamic_cast<TransactionPostgresql>(a_transaction);
    pqxx::transaction<> & backbone_transaction = transaction->getBackboneTransaction();

    string query = "SELECT * FROM "
                   + getTableName(a_id_holder)
                   + " WHERE id_holder = " + backbone_transaction.quote(a_id_holder.getValue2())
                   + " AND id_human_class = " + backbone_transaction.quote(a_id_human.getValue1())
                   + " AND id_human = " + backbone_transaction.quote(a_id_human.getValue2());

    return prepareResultGetRecords(backbone_transaction.exec(query), a_id_holder);
}

HumanWithVolumeRecordMap HumanManagerAccessorPostgresql::getRecords(
    ITransactionShrPtr         a_transaction,
    IDHolder           const & a_id_holder
) const
{
    TransactionPostgresqlShrPtr transaction = shared_dynamic_cast<TransactionPostgresql>(a_transaction);
    pqxx::transaction<> & backbone_transaction = transaction->getBackboneTransaction();

    string query = "SELECT * FROM "
                   + getTableName(a_id_holder)
                   + " WHERE id_holder = " + backbone_transaction.quote(a_id_holder.getValue2());

    return prepareResultGetRecords(backbone_transaction.exec(query), a_id_holder);
}

void HumanManagerAccessorPostgresql::increaseVolume(
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
                   + " AND id_human_class = " + backbone_transaction.quote(a_key.getInternalKey().get<0>().getValue1())
                   + " AND id_human = " + backbone_transaction.quote(a_key.getInternalKey().get<0>().getValue2())
                   + " AND experience = " + backbone_transaction.quote(a_key.getInternalKey().get<1>().getValue());

    pqxx::result result = backbone_transaction.exec(query);
}

void HumanManagerAccessorPostgresql::decreaseVolume(
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
                   + " AND id_human_class = " + backbone_transaction.quote(a_key.getInternalKey().get<0>().getValue1())
                   + " AND id_human = " + backbone_transaction.quote(a_key.getInternalKey().get<0>().getValue2())
                   + " AND experience = " + backbone_transaction.quote(a_key.getInternalKey().get<1>().getValue());

    pqxx::result result = backbone_transaction.exec(query);
}

HumanWithVolumeRecordMap HumanManagerAccessorPostgresql::prepareResultGetRecords(
    pqxx::result const & a_result,
    IDHolder     const & a_id_holder
) const
{
    // Fake types for libpqxx.
    unsigned short int unsigned_short_integer;

    // Create a result map.
    HumanWithVolumeRecordMap records;

    // Prepare types for the values to be written to.
    Volume volume;

    for (pqxx::result::const_iterator it = a_result.begin(); it != a_result.end(); ++it)
    {
        // Get the values.
        IDHuman id_human(it["id_human_class"].as(unsigned_short_integer), it["id_human"].as(unsigned_short_integer));
        Experience experience(it["experience"].as(unsigned_short_integer));
        it["volume"].to(volume);

        // Create a human key.
        Key key(id_human, experience);

        // Create a corresponding record.
        HumanWithVolumeRecordShrPtr record = make_shared<HumanWithVolumeRecord>(a_id_holder, key, volume);

        // Create a pair.
        HumanWithVolumeRecordPair pair(key, record);

        // Add record to the result.
        records.insert(pair);
    }

    return records;
}

string HumanManagerAccessorPostgresql::getTableName(
    IDHolder const & a_id_holder
) const
{
    return string("humans_settlement");
}

} // namespace Human
} // namespace GameServer
