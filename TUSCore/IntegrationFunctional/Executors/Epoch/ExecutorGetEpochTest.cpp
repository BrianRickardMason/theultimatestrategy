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

#include "../../../IntegrationCommon/Helpers/Scenarios/Epoch/ScenarioCreateEpoch.hpp"
#include "../../../IntegrationCommon/Helpers/Scenarios/Epoch/ScenarioGetEpoch.hpp"
#include "../../../IntegrationCommon/Helpers/Scenarios/User/ScenarioCreateUser.hpp"
#include "../../../IntegrationCommon/Helpers/Scenarios/World/ScenarioCreateWorld.hpp"
#include "../../Helpers/IntegrationFunctionalTest.hpp"
#include "../../Helpers/XmlRPCClient/ClientSynchronous/ClientSynchronous.hpp"

using namespace IntegrationCommon::Helpers::Scenarios::Epoch;
using namespace IntegrationCommon::Helpers::Scenarios::User;
using namespace IntegrationCommon::Helpers::Scenarios::World;
using namespace IntegrationCommon::Helpers::Scenarios;
using namespace boost::assign;
using namespace std;

/**
 * Integration functional tests of: ExecutorGetEpoch.
 */
TEST_F(IntegrationFunctionalTest, GetEpoch_WorldDoesNotExist)
{
    IClientShrPtr client(new Client(m_io_service, "localhost", "2222"));

    m_scenarios = list_of
        (IScenarioShrPtr(new ScenarioCreateUser(
            client,
            IScenarioActionShrPtr(new ScenarioCreateUserActionSuccess("Login", "Password")),
            IScenarioVerificationShrPtr(new ScenarioCreateUserVerificationUserHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioGetEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioGetEpochActionSuccess("Login", "Password", "World")),
            IScenarioVerificationShrPtr(new ScenarioGetEpochVerificationWorldDoesNotExist))));

    for (vector<IScenarioShrPtr>::iterator it = m_scenarios.begin(); it != m_scenarios.end(); ++it)
    {
        ASSERT_STREQ("", (*it)->execute());
    }
}

TEST_F(IntegrationFunctionalTest, GetEpoch_WorldDoesExist_EpochDoesNotExist)
{
    IClientShrPtr client(new Client(m_io_service, "localhost", "2222"));

    m_scenarios = list_of
        (IScenarioShrPtr(new ScenarioCreateUser(
            client,
            IScenarioActionShrPtr(new ScenarioCreateUserActionSuccess("Login", "Password")),
            IScenarioVerificationShrPtr(new ScenarioCreateUserVerificationUserHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateWorld(
            client,
            IScenarioActionShrPtr(new ScenarioCreateWorldActionSuccess("modbot", "modbotpass", "World")),
            IScenarioVerificationShrPtr(new ScenarioCreateWorldVerificationWorldHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioGetEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioGetEpochActionSuccess("Login", "Password", "World")),
            IScenarioVerificationShrPtr(new ScenarioGetEpochVerificationEpochHasNotBeenGot))));

    for (vector<IScenarioShrPtr>::iterator it = m_scenarios.begin(); it != m_scenarios.end(); ++it)
    {
        ASSERT_STREQ("", (*it)->execute());
    }
}

TEST_F(IntegrationFunctionalTest, GetEpoch_WorldDoesExist_EpochDoesExist)
{
    IClientShrPtr client(new Client(m_io_service, "localhost", "2222"));

    m_scenarios = list_of
        (IScenarioShrPtr(new ScenarioCreateUser(
            client,
            IScenarioActionShrPtr(new ScenarioCreateUserActionSuccess("Login", "Password")),
            IScenarioVerificationShrPtr(new ScenarioCreateUserVerificationUserHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateWorld(
            client,
            IScenarioActionShrPtr(new ScenarioCreateWorldActionSuccess("modbot", "modbotpass", "World")),
            IScenarioVerificationShrPtr(new ScenarioCreateWorldVerificationWorldHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioCreateEpochActionSuccess("modbot", "modbotpass", "World", "Epoch")),
            IScenarioVerificationShrPtr(new ScenarioCreateEpochVerificationEpochHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioGetEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioGetEpochActionSuccess("Login", "Password", "World")),
            IScenarioVerificationShrPtr(new ScenarioGetEpochVerificationEpochHasBeenGot))));

    for (vector<IScenarioShrPtr>::iterator it = m_scenarios.begin(); it != m_scenarios.end(); ++it)
    {
        ASSERT_STREQ("", (*it)->execute());
    }
}

TEST_F(IntegrationFunctionalTest, GetEpoch_InvalidRequest)
{
    IClientShrPtr client(new Client(m_io_service, "localhost", "2222"));

    m_scenarios = list_of
        (IScenarioShrPtr(new ScenarioGetEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioGetEpochActionInvalidRequest("Login", "Password", "World")),
            IScenarioVerificationShrPtr(new ScenarioGetEpochVerificationInvalidRequest))));

    for (vector<IScenarioShrPtr>::iterator it = m_scenarios.begin(); it != m_scenarios.end(); ++it)
    {
        ASSERT_STREQ("", (*it)->execute());
    }
}
