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

#include "../../../GameServer/Human/Experience.hpp"
#include "../../../GameServer/Human/IDHuman.hpp"
#include "../../../IntegrationCommon/Helpers/Scenarios/Epoch/ScenarioActivateEpoch.hpp"
#include "../../../IntegrationCommon/Helpers/Scenarios/Epoch/ScenarioCreateEpoch.hpp"
#include "../../../IntegrationCommon/Helpers/Scenarios/Epoch/ScenarioDeactivateEpoch.hpp"
#include "../../../IntegrationCommon/Helpers/Scenarios/Land/ScenarioCreateLand.hpp"
#include "../../../IntegrationCommon/Helpers/Scenarios/Settlement/ScenarioCreateSettlement.hpp"
#include "../../../IntegrationCommon/Helpers/Scenarios/Transport/ScenarioTransportHuman.hpp"
#include "../../../IntegrationCommon/Helpers/Scenarios/User/ScenarioCreateUser.hpp"
#include "../../../IntegrationCommon/Helpers/Scenarios/World/ScenarioCreateWorld.hpp"
#include "../../Helpers/IntegrationFunctionalTest.hpp"
#include "../../Helpers/XmlRPCClient/ClientSynchronous/ClientSynchronous.hpp"

using namespace GameServer::Human;
using namespace IntegrationCommon::Helpers::Scenarios::Epoch;
using namespace IntegrationCommon::Helpers::Scenarios::Land;
using namespace IntegrationCommon::Helpers::Scenarios::Settlement;
using namespace IntegrationCommon::Helpers::Scenarios::Transport;
using namespace IntegrationCommon::Helpers::Scenarios::User;
using namespace IntegrationCommon::Helpers::Scenarios::World;
using namespace IntegrationCommon::Helpers::Scenarios;
using namespace boost::assign;
using namespace std;

/**
 * @brief The test class.
 */
class IntegrationFunctionalExecutorTransportHumanTest
    : public IntegrationFunctionalTest
{
protected:
    /**
     * @brief Constructs the test class.
     */
    IntegrationFunctionalExecutorTransportHumanTest()
        : m_scenario_action_invalid_request(
            new ScenarioTransportHumanActionInvalidRequest(
                1, "Password",
                1, 2,
                ID_HUMAN_WORKER_JOBLESS.getValue1(), ID_HUMAN_WORKER_JOBLESS.getValue2(), EXPERIENCE_NOVICE.getValue(),
                22)),
          m_scenario_action_invalid_range(
              new ScenarioTransportHumanActionSuccess(
                  1, "Password",
                  1, 2,
                  ID_HUMAN_WORKER_TEACHER.getValue1() + 1, ID_HUMAN_WORKER_TEACHER.getValue2() + 1, EXPERIENCE_ADVANCED.getValue() + 1,
                  22)),
          m_scenario_action_unauthenticated(
              new ScenarioTransportHumanActionSuccess(
                  1, "Password2",
                  1, 2,
                  ID_HUMAN_WORKER_TEACHER.getValue1(), ID_HUMAN_WORKER_TEACHER.getValue2(), EXPERIENCE_ADVANCED.getValue(),
                  22)),
          m_scenario_verification_invalid_request(new ScenarioTransportHumanVerificationInvalidRequest),
          m_scenario_verification_invalid_range(new ScenarioTransportHumanVerificationInvalidRange),
          m_scenario_verification_unauthenticted(new ScenarioTransportHumanVerificationUnauthenticated)
    {
    }

    /**
     * @brief Test constants: scenario actions.
     */
    IScenarioActionShrPtr m_scenario_action_invalid_request,
                          m_scenario_action_invalid_range,
                          m_scenario_action_unauthenticated;

    /**
     * @brief Test constants: scenario verifications.
     */
    IScenarioVerificationShrPtr m_scenario_verification_invalid_request,
                                m_scenario_verification_invalid_range,
                                m_scenario_verification_unauthenticted;
};

/**
 * Integration functional tests of: ExecutorTransportHuman.
 */
TEST_F(IntegrationFunctionalExecutorTransportHumanTest, InvalidRequest)
{
    IClientShrPtr client(new Client(m_io_service, "localhost", "2222"));

    m_scenarios = list_of
        (IScenarioShrPtr(new ScenarioTransportHuman(client, m_scenario_action_invalid_request, m_scenario_verification_invalid_request)));

    for (vector<IScenarioShrPtr>::iterator it = m_scenarios.begin(); it != m_scenarios.end(); ++it)
    {
        ASSERT_STREQ("", (*it)->execute());
    }
}

TEST_F(IntegrationFunctionalExecutorTransportHumanTest, InvalidRange)
{
    IClientShrPtr client(new Client(m_io_service, "localhost", "2222"));

    m_scenarios = list_of
        (IScenarioShrPtr(new ScenarioTransportHuman(client, m_scenario_action_invalid_range, m_scenario_verification_invalid_range)));

    for (vector<IScenarioShrPtr>::iterator it = m_scenarios.begin(); it != m_scenarios.end(); ++it)
    {
        ASSERT_STREQ("", (*it)->execute());
    }
}

TEST_F(IntegrationFunctionalExecutorTransportHumanTest, Unauthenticated)
{
    IClientShrPtr client(new Client(m_io_service, "localhost", "2222"));

    m_scenarios = list_of
        (IScenarioShrPtr(new ScenarioCreateUser(
            client,
            IScenarioActionShrPtr(new ScenarioCreateUserActionSuccess("Login", "Password")),
            IScenarioVerificationShrPtr(new ScenarioCreateUserVerificationUserHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateWorld(
            client,
            IScenarioActionShrPtr(new ScenarioCreateWorldActionSuccess(1, "Password", "World")),
            IScenarioVerificationShrPtr(new ScenarioCreateWorldVerificationWorldHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioCreateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioCreateEpochVerificationEpochHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioActivateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioActivateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioActivateEpochVerificationEpochHasBeenActivated))))
        (IScenarioShrPtr(new ScenarioCreateLand(
            client,
            IScenarioActionShrPtr(new ScenarioCreateLandActionSuccess(1, "Password", 1, 1, "Land1")),
            IScenarioVerificationShrPtr(new ScenarioCreateLandVerificationLandHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 1, "Settlement1")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioTransportHuman(client, m_scenario_action_unauthenticated, m_scenario_verification_unauthenticted)));

    for (vector<IScenarioShrPtr>::iterator it = m_scenarios.begin(); it != m_scenarios.end(); ++it)
    {
        ASSERT_STREQ("", (*it)->execute());
    }
}

TEST_F(IntegrationFunctionalExecutorTransportHumanTest, UnauthorizedToTheSourceSettlement)
{
    IClientShrPtr client(new Client(m_io_service, "localhost", "2222"));

    m_scenarios = list_of
        (IScenarioShrPtr(new ScenarioCreateUser(
            client,
            IScenarioActionShrPtr(new ScenarioCreateUserActionSuccess("Login1", "Password")),
            IScenarioVerificationShrPtr(new ScenarioCreateUserVerificationUserHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateUser(
            client,
            IScenarioActionShrPtr(new ScenarioCreateUserActionSuccess("Login2", "Password")),
            IScenarioVerificationShrPtr(new ScenarioCreateUserVerificationUserHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateWorld(
            client,
            IScenarioActionShrPtr(new ScenarioCreateWorldActionSuccess(1, "Password", "World")),
            IScenarioVerificationShrPtr(new ScenarioCreateWorldVerificationWorldHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioCreateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioCreateEpochVerificationEpochHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioActivateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioActivateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioActivateEpochVerificationEpochHasBeenActivated))))
        (IScenarioShrPtr(new ScenarioCreateLand(
            client,
            IScenarioActionShrPtr(new ScenarioCreateLandActionSuccess(1, "Password", 1, 1, "Land1")),
            IScenarioVerificationShrPtr(new ScenarioCreateLandVerificationLandHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateLand(
            client,
            IScenarioActionShrPtr(new ScenarioCreateLandActionSuccess(2, "Password", 1, 1, "Land2")),
            IScenarioVerificationShrPtr(new ScenarioCreateLandVerificationLandHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 1, "Settlement1")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 1, "Settlement2")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(2, "Password", 2, "Settlement3")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioTransportHuman(
            client,
            IScenarioActionShrPtr(new ScenarioTransportHumanActionSuccess(
                2, "Password",
                1, 3,
                ID_HUMAN_WORKER_TEACHER.getValue1(), ID_HUMAN_WORKER_TEACHER.getValue2(), EXPERIENCE_ADVANCED.getValue(),
                22)),
            IScenarioVerificationShrPtr(new ScenarioTransportHumanVerificationUnauthorized))));

    for (vector<IScenarioShrPtr>::iterator it = m_scenarios.begin(); it != m_scenarios.end(); ++it)
    {
        ASSERT_STREQ("", (*it)->execute());
    }
}

TEST_F(IntegrationFunctionalExecutorTransportHumanTest, UnauthorizedToTheDestinationSettlement)
{
    IClientShrPtr client(new Client(m_io_service, "localhost", "2222"));

    m_scenarios = list_of
        (IScenarioShrPtr(new ScenarioCreateUser(
            client,
            IScenarioActionShrPtr(new ScenarioCreateUserActionSuccess("Login1", "Password")),
            IScenarioVerificationShrPtr(new ScenarioCreateUserVerificationUserHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateUser(
            client,
            IScenarioActionShrPtr(new ScenarioCreateUserActionSuccess("Login2", "Password")),
            IScenarioVerificationShrPtr(new ScenarioCreateUserVerificationUserHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateWorld(
            client,
            IScenarioActionShrPtr(new ScenarioCreateWorldActionSuccess(1, "Password", "World")),
            IScenarioVerificationShrPtr(new ScenarioCreateWorldVerificationWorldHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioCreateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioCreateEpochVerificationEpochHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioActivateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioActivateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioActivateEpochVerificationEpochHasBeenActivated))))
        (IScenarioShrPtr(new ScenarioCreateLand(
            client,
            IScenarioActionShrPtr(new ScenarioCreateLandActionSuccess(1, "Password", 1, 1, "Land1")),
            IScenarioVerificationShrPtr(new ScenarioCreateLandVerificationLandHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateLand(
            client,
            IScenarioActionShrPtr(new ScenarioCreateLandActionSuccess(2, "Password", 1, 1, "Land2")),
            IScenarioVerificationShrPtr(new ScenarioCreateLandVerificationLandHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 1, "Settlement1")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 1, "Settlement2")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(2, "Password", 2, "Settlement3")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioTransportHuman(
            client,
            IScenarioActionShrPtr(new ScenarioTransportHumanActionSuccess(
                2, "Password",
                3, 1,
                ID_HUMAN_WORKER_TEACHER.getValue1(), ID_HUMAN_WORKER_TEACHER.getValue2(), EXPERIENCE_ADVANCED.getValue(),
                22)),
            IScenarioVerificationShrPtr(new ScenarioTransportHumanVerificationUnauthorized))));

    for (vector<IScenarioShrPtr>::iterator it = m_scenarios.begin(); it != m_scenarios.end(); ++it)
    {
        ASSERT_STREQ("", (*it)->execute());
    }
}

TEST_F(IntegrationFunctionalExecutorTransportHumanTest, EpochIsNotActive)
{
    IClientShrPtr client(new Client(m_io_service, "localhost", "2222"));

    m_scenarios = list_of
        (IScenarioShrPtr(new ScenarioCreateUser(
            client,
            IScenarioActionShrPtr(new ScenarioCreateUserActionSuccess("Login1", "Password")),
            IScenarioVerificationShrPtr(new ScenarioCreateUserVerificationUserHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateWorld(
            client,
            IScenarioActionShrPtr(new ScenarioCreateWorldActionSuccess(1, "Password", "World")),
            IScenarioVerificationShrPtr(new ScenarioCreateWorldVerificationWorldHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioCreateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioCreateEpochVerificationEpochHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioActivateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioActivateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioActivateEpochVerificationEpochHasBeenActivated))))
        (IScenarioShrPtr(new ScenarioCreateLand(
            client,
            IScenarioActionShrPtr(new ScenarioCreateLandActionSuccess(1, "Password", 1, 1, "Land1")),
            IScenarioVerificationShrPtr(new ScenarioCreateLandVerificationLandHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 1, "Settlement1")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 1, "Settlement2")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioDeactivateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioDeactivateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioDeactivateEpochVerificationEpochHasBeenDeactivated))))
        (IScenarioShrPtr(new ScenarioTransportHuman(
            client,
            IScenarioActionShrPtr(new ScenarioTransportHumanActionSuccess(
                1, "Password",
                1, 2,
                ID_HUMAN_WORKER_JOBLESS.getValue1(), ID_HUMAN_WORKER_JOBLESS.getValue2(), EXPERIENCE_NOVICE.getValue(),
                22)),
            IScenarioVerificationShrPtr(new ScenarioTransportHumanVerificationEpochIsNotActive))));

    for (vector<IScenarioShrPtr>::iterator it = m_scenarios.begin(); it != m_scenarios.end(); ++it)
    {
        ASSERT_STREQ("", (*it)->execute());
    }
}

TEST_F(IntegrationFunctionalExecutorTransportHumanTest, TryingToTransportZeroHumans)
{
    IClientShrPtr client(new Client(m_io_service, "localhost", "2222"));

    m_scenarios = list_of
        (IScenarioShrPtr(new ScenarioCreateUser(
            client,
            IScenarioActionShrPtr(new ScenarioCreateUserActionSuccess("Login1", "Password")),
            IScenarioVerificationShrPtr(new ScenarioCreateUserVerificationUserHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateWorld(
            client,
            IScenarioActionShrPtr(new ScenarioCreateWorldActionSuccess(1, "Password", "World")),
            IScenarioVerificationShrPtr(new ScenarioCreateWorldVerificationWorldHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioCreateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioCreateEpochVerificationEpochHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioActivateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioActivateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioActivateEpochVerificationEpochHasBeenActivated))))
        (IScenarioShrPtr(new ScenarioCreateLand(
            client,
            IScenarioActionShrPtr(new ScenarioCreateLandActionSuccess(1, "Password", 1, 1, "Land1")),
            IScenarioVerificationShrPtr(new ScenarioCreateLandVerificationLandHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 1, "Settlement1")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 1, "Settlement2")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioTransportHuman(
            client,
            IScenarioActionShrPtr(new ScenarioTransportHumanActionSuccess(
                1, "Password",
                1, 2,
                ID_HUMAN_WORKER_JOBLESS.getValue1(), ID_HUMAN_WORKER_JOBLESS.getValue2(), EXPERIENCE_NOVICE.getValue(),
                0)),
            IScenarioVerificationShrPtr(new ScenarioTransportHumanVerificationTryingToTransportZeroHumans))));

    for (vector<IScenarioShrPtr>::iterator it = m_scenarios.begin(); it != m_scenarios.end(); ++it)
    {
        ASSERT_STREQ("", (*it)->execute());
    }
}

//TEST_F(IntegrationFunctionalExecutorTransportHumanTest, SourceSettlementDoesNotExist)
//{
//    // TODO: Cannot be covered by IFTs.
//}

//TEST_F(IntegrationFunctionalExecutorTransportHumanTest, DestinationSettlementDoesNotExist)
//{
//    // TODO: Cannot be covered by IFTs.
//}

TEST_F(IntegrationFunctionalExecutorTransportHumanTest, SettlementsAreNotFromTheSameLand)
{
    IClientShrPtr client(new Client(m_io_service, "localhost", "2222"));

    m_scenarios = list_of
        (IScenarioShrPtr(new ScenarioCreateUser(
            client,
            IScenarioActionShrPtr(new ScenarioCreateUserActionSuccess("Login1", "Password")),
            IScenarioVerificationShrPtr(new ScenarioCreateUserVerificationUserHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateWorld(
            client,
            IScenarioActionShrPtr(new ScenarioCreateWorldActionSuccess(1, "Password", "World")),
            IScenarioVerificationShrPtr(new ScenarioCreateWorldVerificationWorldHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioCreateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioCreateEpochVerificationEpochHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioActivateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioActivateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioActivateEpochVerificationEpochHasBeenActivated))))
        (IScenarioShrPtr(new ScenarioCreateLand(
            client,
            IScenarioActionShrPtr(new ScenarioCreateLandActionSuccess(1, "Password", 1, 1, "Land1")),
            IScenarioVerificationShrPtr(new ScenarioCreateLandVerificationLandHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateLand(
            client,
            IScenarioActionShrPtr(new ScenarioCreateLandActionSuccess(1, "Password", 1, 1, "Land2")),
            IScenarioVerificationShrPtr(new ScenarioCreateLandVerificationLandHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 1, "Settlement1")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 2, "Settlement2")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioTransportHuman(
            client,
            IScenarioActionShrPtr(new ScenarioTransportHumanActionSuccess(
                1, "Password",
                1, 2,
                ID_HUMAN_WORKER_JOBLESS.getValue1(), ID_HUMAN_WORKER_JOBLESS.getValue2(), EXPERIENCE_NOVICE.getValue(),
                22)),
            IScenarioVerificationShrPtr(new ScenarioTransportHumanVerificationSettlementsAreNotFromTheSameLand))));

    for (vector<IScenarioShrPtr>::iterator it = m_scenarios.begin(); it != m_scenarios.end(); ++it)
    {
        ASSERT_STREQ("", (*it)->execute());
    }
}

TEST_F(IntegrationFunctionalExecutorTransportHumanTest, NotEnoughHumans)
{
    IClientShrPtr client(new Client(m_io_service, "localhost", "2222"));

    m_scenarios = list_of
        (IScenarioShrPtr(new ScenarioCreateUser(
            client,
            IScenarioActionShrPtr(new ScenarioCreateUserActionSuccess("Login1", "Password")),
            IScenarioVerificationShrPtr(new ScenarioCreateUserVerificationUserHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateWorld(
            client,
            IScenarioActionShrPtr(new ScenarioCreateWorldActionSuccess(1, "Password", "World")),
            IScenarioVerificationShrPtr(new ScenarioCreateWorldVerificationWorldHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioCreateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioCreateEpochVerificationEpochHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioActivateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioActivateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioActivateEpochVerificationEpochHasBeenActivated))))
        (IScenarioShrPtr(new ScenarioCreateLand(
            client,
            IScenarioActionShrPtr(new ScenarioCreateLandActionSuccess(1, "Password", 1, 1, "Land1")),
            IScenarioVerificationShrPtr(new ScenarioCreateLandVerificationLandHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 1, "Settlement1")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 1, "Settlement2")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioTransportHuman(
            client,
            IScenarioActionShrPtr(new ScenarioTransportHumanActionSuccess(
                1, "Password",
                1, 2,
                ID_HUMAN_WORKER_JOBLESS.getValue1(), ID_HUMAN_WORKER_JOBLESS.getValue2(), EXPERIENCE_NOVICE.getValue(),
                1001)),
            IScenarioVerificationShrPtr(new ScenarioTransportHumanVerificationNotEnoughHumans))));

    for (vector<IScenarioShrPtr>::iterator it = m_scenarios.begin(); it != m_scenarios.end(); ++it)
    {
        ASSERT_STREQ("", (*it)->execute());
    }
}

TEST_F(IntegrationFunctionalExecutorTransportHumanTest, HumanHasBeenTransportedOneHuman)
{
    IClientShrPtr client(new Client(m_io_service, "localhost", "2222"));

    m_scenarios = list_of
        (IScenarioShrPtr(new ScenarioCreateUser(
            client,
            IScenarioActionShrPtr(new ScenarioCreateUserActionSuccess("Login1", "Password")),
            IScenarioVerificationShrPtr(new ScenarioCreateUserVerificationUserHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateWorld(
            client,
            IScenarioActionShrPtr(new ScenarioCreateWorldActionSuccess(1, "Password", "World")),
            IScenarioVerificationShrPtr(new ScenarioCreateWorldVerificationWorldHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioCreateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioCreateEpochVerificationEpochHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioActivateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioActivateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioActivateEpochVerificationEpochHasBeenActivated))))
        (IScenarioShrPtr(new ScenarioCreateLand(
            client,
            IScenarioActionShrPtr(new ScenarioCreateLandActionSuccess(1, "Password", 1, 1, "Land1")),
            IScenarioVerificationShrPtr(new ScenarioCreateLandVerificationLandHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 1, "Settlement1")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 1, "Settlement2")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioTransportHuman(
            client,
            IScenarioActionShrPtr(new ScenarioTransportHumanActionSuccess(
                1, "Password",
                1, 2,
                ID_HUMAN_WORKER_JOBLESS.getValue1(), ID_HUMAN_WORKER_JOBLESS.getValue2(), EXPERIENCE_NOVICE.getValue(),
                1)),
            IScenarioVerificationShrPtr(new ScenarioTransportHumanVerificationHumanHasBeenTransported))));

    for (vector<IScenarioShrPtr>::iterator it = m_scenarios.begin(); it != m_scenarios.end(); ++it)
    {
        ASSERT_STREQ("", (*it)->execute());
    }
}

TEST_F(IntegrationFunctionalExecutorTransportHumanTest, HumanHasBeenTransportedManyHumans)
{
    IClientShrPtr client(new Client(m_io_service, "localhost", "2222"));

    m_scenarios = list_of
        (IScenarioShrPtr(new ScenarioCreateUser(
            client,
            IScenarioActionShrPtr(new ScenarioCreateUserActionSuccess("Login1", "Password")),
            IScenarioVerificationShrPtr(new ScenarioCreateUserVerificationUserHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateWorld(
            client,
            IScenarioActionShrPtr(new ScenarioCreateWorldActionSuccess(1, "Password", "World")),
            IScenarioVerificationShrPtr(new ScenarioCreateWorldVerificationWorldHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioCreateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioCreateEpochVerificationEpochHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioActivateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioActivateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioActivateEpochVerificationEpochHasBeenActivated))))
        (IScenarioShrPtr(new ScenarioCreateLand(
            client,
            IScenarioActionShrPtr(new ScenarioCreateLandActionSuccess(1, "Password", 1, 1, "Land1")),
            IScenarioVerificationShrPtr(new ScenarioCreateLandVerificationLandHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 1, "Settlement1")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 1, "Settlement2")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioTransportHuman(
            client,
            IScenarioActionShrPtr(new ScenarioTransportHumanActionSuccess(
                1, "Password",
                1, 2,
                ID_HUMAN_WORKER_JOBLESS.getValue1(), ID_HUMAN_WORKER_JOBLESS.getValue2(), EXPERIENCE_NOVICE.getValue(),
                501)),
            IScenarioVerificationShrPtr(new ScenarioTransportHumanVerificationHumanHasBeenTransported))));

    for (vector<IScenarioShrPtr>::iterator it = m_scenarios.begin(); it != m_scenarios.end(); ++it)
    {
        ASSERT_STREQ("", (*it)->execute());
    }
}

TEST_F(IntegrationFunctionalExecutorTransportHumanTest, HumanHasBeenTransportedMaxHumans)
{
    IClientShrPtr client(new Client(m_io_service, "localhost", "2222"));

    m_scenarios = list_of
        (IScenarioShrPtr(new ScenarioCreateUser(
            client,
            IScenarioActionShrPtr(new ScenarioCreateUserActionSuccess("Login1", "Password")),
            IScenarioVerificationShrPtr(new ScenarioCreateUserVerificationUserHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateWorld(
            client,
            IScenarioActionShrPtr(new ScenarioCreateWorldActionSuccess(1, "Password", "World")),
            IScenarioVerificationShrPtr(new ScenarioCreateWorldVerificationWorldHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioCreateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioCreateEpochVerificationEpochHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioActivateEpoch(
            client,
            IScenarioActionShrPtr(new ScenarioActivateEpochActionSuccess(1)),
            IScenarioVerificationShrPtr(new ScenarioActivateEpochVerificationEpochHasBeenActivated))))
        (IScenarioShrPtr(new ScenarioCreateLand(
            client,
            IScenarioActionShrPtr(new ScenarioCreateLandActionSuccess(1, "Password", 1, 1, "Land1")),
            IScenarioVerificationShrPtr(new ScenarioCreateLandVerificationLandHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 1, "Settlement1")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioCreateSettlement(
            client,
            IScenarioActionShrPtr(new ScenarioCreateSettlementActionSuccess(1, "Password", 1, "Settlement2")),
            IScenarioVerificationShrPtr(new ScenarioCreateSettlementVerificationSettlementHasBeenCreated))))
        (IScenarioShrPtr(new ScenarioTransportHuman(
            client,
            IScenarioActionShrPtr(new ScenarioTransportHumanActionSuccess(
                1, "Password",
                1, 2,
                ID_HUMAN_WORKER_JOBLESS.getValue1(), ID_HUMAN_WORKER_JOBLESS.getValue2(), EXPERIENCE_NOVICE.getValue(),
                1000)),
            IScenarioVerificationShrPtr(new ScenarioTransportHumanVerificationHumanHasBeenTransported))));

    for (vector<IScenarioShrPtr>::iterator it = m_scenarios.begin(); it != m_scenarios.end(); ++it)
    {
        ASSERT_STREQ("", (*it)->execute());
    }
}
