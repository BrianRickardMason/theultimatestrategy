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

#ifndef GAMESERVER_BUILDING_BUILDINGWITHVOLUME_HPP
#define GAMESERVER_BUILDING_BUILDINGWITHVOLUME_HPP

#include <GameServer/Building/BuildingWithVolumeRecord.hpp>
#include <GameServer/Configuration/Configurator/Building/IBuilding.hpp>
#include <Network/XmlRPCServer/IContext.hpp>

namespace GameServer
{
namespace Building
{

/**
 * @brief BuildingWithVolume.
 */
class BuildingWithVolume
{
public:
    /**
     * @brief Constructs the building with volume.
     *
     * @param a_context The context of the server.
     * @param a_key     The key of the building.
     * @param a_volume  The volume of the building.
     */
    BuildingWithVolume(
        IContextShrPtr              const a_context,
        Configuration::IBuildingKey const a_key,
        Volume                      const a_volume
    );

    /**
     * @brief Constructs the build with volume.
     *
     * @param a_context The context of the server.
     * @param a_record  The record of building with volume.
     */
    BuildingWithVolume(
        IContextShrPtr           const   a_context,
        BuildingWithVolumeRecord const & a_record
    );

    /**
     * @brief Gets the building.
     *
     * @return The building.
     */
    Configuration::IBuildingShrPtr getBuilding() const;

    /**
     * @brief Gets the volume of the building.
     *
     * @return The volume of the building.
     */
    Volume getVolume() const;

private:
    /**
     * @brief The building.
     */
    Configuration::IBuildingShrPtr m_building;

    /**
     * @brief The volume of the building.
     */
    Volume m_volume;
};

//@{
/**
 * @brief A useful typedef.
 */
typedef boost::shared_ptr<BuildingWithVolume> BuildingWithVolumeShrPtr;
typedef std::pair<Configuration::IBuildingKey, BuildingWithVolumeShrPtr> BuildingWithVolumePair;
typedef std::map<Configuration::IBuildingKey, BuildingWithVolumeShrPtr> BuildingWithVolumeMap;
//}@

} // namespace Building
} // namespace GameServer

#endif // GAMESERVER_BUILDING_BUILDINGWITHVOLUME_HPP
