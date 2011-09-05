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
#include "SettlementManagerAccessorPostgresql.hpp"

using namespace GameServer::Land;
using namespace GameServer::Persistency;
using namespace boost;
using namespace std;

namespace GameServer
{
namespace Settlement
{

IDSettlement SettlementManagerAccessorPostgresql::insertRecord(
    ITransactionShrPtr         a_transaction,
    IDLand             const & a_id_land,
    string             const & a_name
) const
{
    TransactionPostgresqlShrPtr transaction = shared_dynamic_cast<TransactionPostgresql>(a_transaction);
    pqxx::transaction<> & backbone_transaction = transaction->getBackboneTransaction();

    string query = "INSERT INTO settlements(id_land, name) VALUES("
                   + backbone_transaction.quote(a_id_land.getValue()) + ", "
                   + backbone_transaction.quote(a_name) + ")"
                   + "RETURNING id_settlement";

    pqxx::result result = backbone_transaction.exec(query);

    // Fake types for libpqxx.
    unsigned int unsigned_integer;

    IDSettlement id_settlement(result[0]["id_settlement"].as(unsigned_integer));

    return id_settlement;
}

void SettlementManagerAccessorPostgresql::deleteRecord(
    ITransactionShrPtr         a_transaction,
    IDSettlement       const & a_id_settlement
) const
{
    TransactionPostgresqlShrPtr transaction = shared_dynamic_cast<TransactionPostgresql>(a_transaction);
    pqxx::transaction<> & backbone_transaction = transaction->getBackboneTransaction();

    string query = "DELETE FROM settlements WHERE id_settlement = "
                   + backbone_transaction.quote(a_id_settlement.getValue());

    pqxx::result result = backbone_transaction.exec(query);
}

SettlementRecordShrPtr SettlementManagerAccessorPostgresql::getRecord(
    ITransactionShrPtr         a_transaction,
    IDSettlement       const & a_id_settlement
) const
{
    TransactionPostgresqlShrPtr transaction = shared_dynamic_cast<TransactionPostgresql>(a_transaction);
    pqxx::transaction<> & backbone_transaction = transaction->getBackboneTransaction();

    string query = "SELECT * FROM settlements WHERE id_settlement = "
                   + backbone_transaction.quote(a_id_settlement.getValue());

    return prepareResultGetRecord(backbone_transaction.exec(query));
}

SettlementRecordShrPtr SettlementManagerAccessorPostgresql::getRecord(
    ITransactionShrPtr         a_transaction,
    string             const & a_name,
    IDLand             const & a_id_land
) const
{
    TransactionPostgresqlShrPtr transaction = shared_dynamic_cast<TransactionPostgresql>(a_transaction);
    pqxx::transaction<> & backbone_transaction = transaction->getBackboneTransaction();

    string query = "SELECT * FROM settlements WHERE name = "
                   + backbone_transaction.quote(a_name)
                   + " AND id_land = " + backbone_transaction.quote(a_id_land.getValue());

    return prepareResultGetRecord(backbone_transaction.exec(query));
}

SettlementRecordMap SettlementManagerAccessorPostgresql::getRecords(
    ITransactionShrPtr a_transaction
) const
{
    TransactionPostgresqlShrPtr transaction = shared_dynamic_cast<TransactionPostgresql>(a_transaction);
    pqxx::transaction<> & backbone_transaction = transaction->getBackboneTransaction();

    string query = "SELECT * FROM settlements";

    return prepareResultGetRecords(backbone_transaction.exec(query));
}

SettlementRecordMap SettlementManagerAccessorPostgresql::getRecords(
    ITransactionShrPtr         a_transaction,
    IDLand             const & a_id_land
) const
{
    TransactionPostgresqlShrPtr transaction = shared_dynamic_cast<TransactionPostgresql>(a_transaction);
    pqxx::transaction<> & backbone_transaction = transaction->getBackboneTransaction();

    string query = "SELECT * FROM settlements WHERE id_land = "
                   + backbone_transaction.quote(a_id_land.getValue());

    return prepareResultGetRecords(backbone_transaction.exec(query));
}

SettlementRecordShrPtr SettlementManagerAccessorPostgresql::prepareResultGetRecord(
    pqxx::result const & a_result
) const
{
    // Fake types for libpqxx.
    unsigned int unsigned_integer;

    if (a_result.size() > 0)
    {
        IDLand id_land;
        IDSettlement id_settlement;
        string name;

        id_land       = a_result[0]["id_land"].as(unsigned_integer);
        id_settlement = a_result[0]["id_settlement"].as(unsigned_integer);
        a_result[0]["name"].to(name);

        return make_shared<SettlementRecord>(id_land, id_settlement, name);
    }
    else
    {
        return SettlementRecordShrPtr();
    }
}

SettlementRecordMap SettlementManagerAccessorPostgresql::prepareResultGetRecords(
    pqxx::result const & a_result
) const
{
    // Fake types for libpqxx.
    unsigned int unsigned_integer;

    IDLand id_land;
    IDSettlement id_settlement;
    string name;

    SettlementRecordMap records;

    for (pqxx::result::const_iterator it = a_result.begin(); it != a_result.end(); ++it)
    {
        id_land       = it["id_land"].as(unsigned_integer);
        id_settlement = it["id_settlement"].as(unsigned_integer);
        it["name"].to(name);

        SettlementRecordShrPtr record = make_shared<SettlementRecord>(id_land, id_settlement, name);
        SettlementRecordPair pair(id_settlement, record);
        records.insert(pair);
    }

    return records;
}

} // namespace Settlement
} // namespace GameServer
