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

#ifndef GAMESERVER_LAND_LANDRECORD_HPP
#define GAMESERVER_LAND_LANDRECORD_HPP

#include "../Epoch/IDEpoch.hpp"
#include "../User/IDUser.hpp"
#include "../World/IDWorld.hpp"
#include "IDLand.hpp"
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>

namespace GameServer
{
namespace Land
{

/**
 * @brief The land record.
 */
class LandRecord
{
public:
    /**
     * @brief Constructs the land record.
     *
     * @param a_id_user  The identifier of the user.
     * @param a_id_world The identifier of the world.
     * @param a_id_epoch The identifier of the epoch.
     * @param a_id_land  The identifier of the land.
     * @param a_name     The name of the land.
     * @param a_granted  The state of the "granted" value, whether or not humans and resources have been granted.
     */
    LandRecord(
        User::IDUser   const & a_id_user,
        World::IDWorld const & a_id_world,
        Epoch::IDEpoch const & a_id_epoch,
        IDLand         const & a_id_land,
        std::string    const & a_name,
        bool                   a_granted
    );

    /**
     * @brief Gets the identifier of the user.
     *
     * @return The identifier of the user.
     */
    User::IDUser const & getIDUser() const;

    /**
     * @brief Gets the identifier of the world.
     *
     * @return The identifier of the world.
     */
    World::IDWorld const & getIDWorld() const;

    /**
     * @brief Gets the identifier of the epoch.
     *
     * @return The identifier of the epoch.
     */
    Epoch::IDEpoch const & getIDEpoch() const;

    /**
     * @brief Gets the identifier of the land.
     *
     * @return The identifier of the land.
     */
    IDLand const & getIDLand() const;

    /**
     * @brief Gets the name of the land.
     *
     * @return The name of the land.
     */
    std::string const & getName() const;

    /**
     * @brief Gets the state of the "granted" value of the land.
     *
     * @return The state of the "granted" value of the land.
     */
    bool getGranted() const;

private:
    /**
     * @brief The identifier of the user.
     */
    User::IDUser const m_id_user;

    /**
     * @brief The identifier of the world.
     */
    World::IDWorld const m_id_world;

    /**
     * @brief The identifier of the epoch.
     */
    Epoch::IDEpoch const m_id_epoch;

    /**
     * @brief The identifier of the land.
     */
    IDLand const m_id_land;

    /**
     * @brief The name of the land.
     */
    std::string const m_name;

    /**
     * @brief Defines whether resources and humans has been granted to the land.
     *
     * True if resources and humans has been granted, false otherwise.
     */
    bool const m_granted;
};

/**
 * @brief The shared pointer of the record of the land.
 */
typedef boost::shared_ptr<LandRecord> LandRecordShrPtr;

/**
 * @brief The pair of the record of the land.
 */
typedef std::pair<IDLand, LandRecordShrPtr> LandRecordPair;

/**
 * @brief The map of the record of the land.
 */
typedef std::map<IDLand, LandRecordShrPtr> LandRecordMap;

} // namespace Land
} // namespace GameServer

#endif // GAMESERVER_LAND_LANDRECORD_HPP
