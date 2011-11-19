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

#include <GameServer/Configuration/Configurator/Building/Building.hpp>

using namespace GameServer::Resource;
using namespace std;

namespace GameServer
{
namespace Configuration
{

Building::Building(
    IBuildingKey                                         const   a_key,
    string                                               const   a_class,
    string                                               const   a_name,
    unsigned int                                         const   a_capacity,
    std::map<IResourceKey, GameServer::Resource::Volume> const & a_costs_building
)
    : m_key(a_key),
      m_class(a_class),
      m_name(a_name),
      m_capacity(a_capacity),
      m_costs_building(a_costs_building)
{
}

IBuildingKey Building::getKey() const
{
    return m_key;
}

string Building::getClass() const
{
    return m_class;
}

string Building::getName() const
{
    return m_name;
}

unsigned int Building::getCapacity() const
{
    return m_capacity;
}

ResourceSet Building::getCostsBuilding() const
{
    ResourceWithVolumeMap map;

    for (std::map<IResourceKey, Volume>::const_iterator it = m_costs_building.begin();
         it != m_costs_building.end();
         ++it)
    {
        ResourceWithVolumeShrPtr resource(new ResourceWithVolume(it->first, it->second));

        map[it->first] = resource;
    }

    return ResourceSet(map);
}

} // namespace Configuration
} // namespace GameServer
