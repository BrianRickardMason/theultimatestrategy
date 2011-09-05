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

#ifndef GAMESERVER_SETTLEMENT_SETTLEMENTACCESSORPOSTGRESQL_HPP
#define GAMESERVER_SETTLEMENT_SETTLEMENTACCESSORPOSTGRESQL_HPP

#include "ISettlementManagerAccessor.hpp"
#include <pqxx/result.hxx>

namespace GameServer
{
namespace Settlement
{

/**
 * @brief A PostgreSQL settlement manager accessor.
 */
class SettlementManagerAccessorPostgresql
    : public ISettlementManagerAccessor
{
public:
    /**
     * @brief Inserts a settlement record.
     *
     * @param a_transaction A transaction.
     * @param a_id_land     An identifier of the land.
     * @param a_name        A name of the settlement.
     */
    virtual IDSettlement insertRecord(
        Persistency::ITransactionShrPtr         a_transaction,
        Land::IDLand                    const & a_id_land,
        std::string                     const & a_name
    ) const;

    /**
     * @brief Deletes a settlement record.
     *
     * @param a_transaction   A transaction.
     * @param a_id_settlement An identifier of the settlement.
     */
    virtual void deleteRecord(
        Persistency::ITransactionShrPtr         a_transaction,
        IDSettlement                    const & a_id_settlement
    ) const;

    /**
     * @brief Gets the record of settlement.
     *
     * @param a_transaction   The transaction.
     * @param a_id_settlement The identifier of the settlement.
     *
     * @return The record of the settlement, null if not found.
     */
    virtual SettlementRecordShrPtr getRecord(
        Persistency::ITransactionShrPtr         a_transaction,
        IDSettlement                    const & a_id_settlement
    ) const;

    /**
     * @brief Gets the record of settlement.
     *
     * @param a_transaction   The transaction.
     * @param a_id_settlement The identifier of the settlement.
     *
     * @return The record of the settlement, null if not found.
     */
    virtual SettlementRecordShrPtr getRecord(
        Persistency::ITransactionShrPtr         a_transaction,
        std::string                     const & a_name,
        Land::IDLand                    const & a_id_land
    ) const;

    /**
     * @brief Gets records of settlement.
     *
     * @param a_transaction The transaction.
     *
     * @return A map of settlement records, an empty map if not found.
     */
    virtual SettlementRecordMap getRecords(
        Persistency::ITransactionShrPtr a_transaction
    ) const;

    /**
     * @brief Gets records of settlement.
     *
     * @param a_transaction The transaction.
     * @param a_id_land     The identifier of the land.
     *
     * @return A map of settlement records, an empty map if not found.
     */
    virtual SettlementRecordMap getRecords(
        Persistency::ITransactionShrPtr         a_transaction,
        Land::IDLand                    const & a_id_land
    ) const;

private:
    /**
     * @brief Prepares the result for getRecord* methods.
     *
     * @param a_result A postgresql result.
     *
     * @return A settlement record.
     */
    SettlementRecordShrPtr prepareResultGetRecord(
        pqxx::result const & a_result
    ) const;

    /**
     * @brief Prepares the result for getRecords* methods.
     *
     * @param a_result A postgresql result.
     *
     * @return A map of settlement records.
     */
    SettlementRecordMap prepareResultGetRecords(
        pqxx::result const & a_result
    ) const;
};

} // namespace Settlement
} // namespace GameServer

#endif // GAMESERVER_SETTLEMENT_SETTLEMENTACCESSORPOSTGRESQL_HPP
