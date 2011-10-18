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

#include "../../../../GameServer/Common/ManagerAbstractFactoryPostgresql.hpp"
#include "../../../../GameServer/Human/Operators/EngageHuman/EngageHumanOperatorFactory.hpp"
#include "../../../../GameServer/Settlement/Operators/CreateSettlement/CreateSettlementOperatorFactory.hpp"
#include "../../../ComponentTest.hpp"
#include "../../../Helpers/Constants.hpp"
#include <boost/assign.hpp>

using namespace GameServer::Building;
using namespace GameServer::Common;
using namespace GameServer::Cost;
using namespace GameServer::Epoch;
using namespace GameServer::Human;
using namespace GameServer::Land;
using namespace GameServer::Persistence;
using namespace GameServer::Property;
using namespace GameServer::Resource;
using namespace GameServer::Settlement;
using namespace GameServer::User;
using namespace GameServer::World;
using namespace boost;
using namespace std;

namespace R = GameServer::Resource;

/**
 * @brief A test class.
 */
class EngageHumanOperatorTest
    : public ComponentTest
{
protected:
    /**
     * @brief Constructs the test class.
     */
    EngageHumanOperatorTest()
        : m_epoch_name("Epoch"),
          m_login("Login"),
          m_world_name("World"),
          m_land_name_1("Land1"),
          m_land_name_2("Land2"),
          m_settlement_name_1("Settlement1"),
          m_settlement_name_2("Settlement2"),
          m_settlement_name_3("Settlement3"),
          m_settlement_name_4("Settlement5"),
          m_id_holder_11(ID_HOLDER_CLASS_SETTLEMENT, m_settlement_name_1),
          m_id_holder_12(ID_HOLDER_CLASS_SETTLEMENT, m_settlement_name_2),
          m_id_holder_21(ID_HOLDER_CLASS_SETTLEMENT, m_settlement_name_3),
          m_id_holder_4(ID_HOLDER_CLASS_SETTLEMENT, m_settlement_name_4),
          m_manager_abstract_factory(new ManagerAbstractFactoryPostgresql),
          m_building_persistence_facade(m_manager_abstract_factory->createBuildingPersistenceFacade()),
          m_cost_manager(m_manager_abstract_factory->createCostManager()),
          m_epoch_persistence_facade(m_manager_abstract_factory->createEpochPersistenceFacade()),
          m_human_persistence_facade(m_manager_abstract_factory->createHumanPersistenceFacade()),
          m_land_persistence_facade(m_manager_abstract_factory->createLandPersistenceFacade()),
          m_property_manager(m_manager_abstract_factory->createPropertyManager()),
          m_resource_persistence_facade(m_manager_abstract_factory->createResourcePersistenceFacade()),
          m_user_persitence_facade(m_manager_abstract_factory->createUserPersistenceFacade()),
          m_world_persistence_facade(m_manager_abstract_factory->createWorldPersistenceFacade()),
          m_create_settlement_operator(CreateSettlementOperatorFactory::createCreateSettlementOperator(m_manager_abstract_factory)),
          m_engage_human_operator(EngageHumanOperatorFactory::createEngageHumanOperator(m_manager_abstract_factory))
    {
        {
            IConnectionShrPtr connection = m_persistence.getConnection();
            ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

            m_user_persitence_facade->createUser(transaction, "Login", "Password");

            m_world_persistence_facade->createWorld(transaction, m_world_name);

            m_epoch_persistence_facade->createEpoch(transaction, m_world_name, m_epoch_name);

            m_land_persistence_facade->createLand(transaction, m_login, m_world_name, m_land_name_1);
            m_land_persistence_facade->createLand(transaction, m_login, m_world_name, m_land_name_2);

            m_create_settlement_operator->createSettlement(transaction, m_land_name_1, m_settlement_name_1);
            m_create_settlement_operator->createSettlement(transaction, m_land_name_1, m_settlement_name_2);
            m_create_settlement_operator->createSettlement(transaction, m_land_name_2, m_settlement_name_3);

            transaction->commit();
        }
    }

    /**
     * @brief Compares a resource set with expected volumes.
     *
     * @param a_resource_set The resource set to be compared.
     * @param a_volumes      A vector of expected volumes.
     *
     * TODO: Consider moving to helpers.
     */
    void compareResourceSet(
        ResourceSet       const & a_resource_set,
        vector<R::Volume> const & a_vector
    )
    {
        ResourceWithVolumeMap resources_map = a_resource_set.getMap();

        for (ResourceWithVolumeMap::iterator it = resources_map.begin(); it != resources_map.end(); ++it)
        {
            ASSERT_EQ(a_vector.at(it->second->getIDResource().getValue() - 1), it->second->getVolume());
        }
    }

    /**
     * @brief Asserts that database remains untouched.
     */
    void assertDatabaseUntouched()
    {
        vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
        vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);

        {
            IConnectionShrPtr connection = m_persistence.getConnection();
            ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

            ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
            ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
            ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

            compareResourceSet(resource_set_11, expected_volumes_1);
            compareResourceSet(resource_set_12, expected_volumes_2);
            compareResourceSet(resource_set_21, expected_volumes_1);

            ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
            ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
            ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

            ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
            ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
        }
    }

    /**
     * @brief Test constants: the name of the epoch.
     */
    string m_epoch_name;

    /**
     * @brief Test constants: the login of the user.
     */
    string m_login;

    /**
     * @brief Test constants: the name of the world.
     */
    string m_world_name;

    /**
     * @brief Test constants: the names of the lands.
     */
    string m_land_name_1,
           m_land_name_2;

    /**
     * @brief Test constants: the names of the settlements.
     */
    string m_settlement_name_1,
           m_settlement_name_2,
           m_settlement_name_3,
           m_settlement_name_4;

    /**
     * @brief Test constants identifiers of a holder.
     */
    IDHolder m_id_holder_11,
             m_id_holder_12,
             m_id_holder_21,
             m_id_holder_4;

    /**
     * @brief The abstract factory of managers.
     */
    IManagerAbstractFactoryShrPtr m_manager_abstract_factory;

    //@{
    /**
     * @brief Persistence facades used in tests.
     */
    IBuildingPersistenceFacadeShrPtr m_building_persistence_facade;
    ICostManagerShrPtr               m_cost_manager;
    IEpochPersistenceFacadeShrPtr    m_epoch_persistence_facade;
    IHumanPersistenceFacadeShrPtr    m_human_persistence_facade;
    ILandPersistenceFacadeShrPtr     m_land_persistence_facade;
    IPropertyManagerShrPtr           m_property_manager;
    IResourcePersistenceFacadeShrPtr m_resource_persistence_facade;
    IUserPersistenceFacadeShrPtr     m_user_persitence_facade;
    IWorldPersistenceFacadeShrPtr    m_world_persistence_facade;
    //}@

    /**
     * @brief Operators used in tests.
     */
    ICreateSettlementOperatorShrPtr m_create_settlement_operator;
    IEngageHumanOperatorShrPtr      m_engage_human_operator;
};

/**
 * Component tests of: EngageHumanOperator::engageHuman.
 */
TEST_F(EngageHumanOperatorTest, engageHuman_TryingToEngageZeroHumans)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_TRYING_TO_ENGAGE_ZERO_HUMANS,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_WORKER_DRUID_NOVICE, 0).m_exit_code);
    }

    assertDatabaseUntouched();
}

TEST_F(EngageHumanOperatorTest, engageHuman_HolderDoesNotExist)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_NOT_ENOUGH_JOBLESS,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_4, KEY_WORKER_DRUID_NOVICE, 1).m_exit_code);
    }

    assertDatabaseUntouched();
}

TEST_F(EngageHumanOperatorTest, engageHuman_HumanIsNotEngageable)
{
    for (KeyVec::const_iterator it = HUMAN_IS_NOT_ENGAGEABLE.begin(); it != HUMAN_IS_NOT_ENGAGEABLE.end(); ++it)
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_HUMAN_IS_NOT_ENGAGEABLE,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, *it, 1).m_exit_code);
    }

    assertDatabaseUntouched();
}

TEST_F(EngageHumanOperatorTest, engageHuman_ZeroJobless)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_human_persistence_facade->subtractHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE, 1000);

        transaction->commit();
    }

    vector<IDHolder> ids = assign::list_of(IDHolder(ID_HOLDER_CLASS_SETTLEMENT, m_settlement_name_1))
                                          (IDHolder(ID_HOLDER_CLASS_SETTLEMENT, m_settlement_name_2));

    for (vector<IDHolder>::const_iterator it = ids.begin(); it != ids.end(); ++it)
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_NOT_ENOUGH_JOBLESS,
                  m_engage_human_operator->engageHuman(transaction, *it, KEY_WORKER_DRUID_NOVICE, 1).m_exit_code);
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_1);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_11).empty());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_NotEnoughJobless)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_human_persistence_facade->subtractHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE, 999);

        transaction->commit();
    }

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_NOT_ENOUGH_JOBLESS,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_WORKER_DRUID_NOVICE, 2).m_exit_code);
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_1);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());


        ASSERT_EQ(1, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_ZeroResources)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_human_persistence_facade->addHuman(transaction, m_id_holder_12, KEY_WORKER_JOBLESS_NOVICE, 1000);

        // TODO: Subtract resource set. The same in build building and destroy building operators.
        m_resource_persistence_facade->subtractResource(transaction, m_id_holder_11, KEY_RESOURCE_COAL, 1000);
        m_resource_persistence_facade->subtractResource(transaction, m_id_holder_11, KEY_RESOURCE_FOOD, 10000);
        m_resource_persistence_facade->subtractResource(transaction, m_id_holder_11, KEY_RESOURCE_GOLD, 10000);
        m_resource_persistence_facade->subtractResource(transaction, m_id_holder_11, KEY_RESOURCE_IRON, 1000);
        m_resource_persistence_facade->subtractResource(transaction, m_id_holder_11, KEY_RESOURCE_MANA, 1000);
        m_resource_persistence_facade->subtractResource(transaction, m_id_holder_11, KEY_RESOURCE_ROCK, 1000);
        m_resource_persistence_facade->subtractResource(transaction, m_id_holder_11, KEY_RESOURCE_WOOD, 1000);

        transaction->commit();
    }

    vector<IDHolder> ids = assign::list_of(IDHolder(ID_HOLDER_CLASS_SETTLEMENT, m_settlement_name_1))
                                          (IDHolder(ID_HOLDER_CLASS_SETTLEMENT, m_settlement_name_2));

    for (vector<IDHolder>::const_iterator it = ids.begin(); it != ids.end(); ++it)
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_NOT_ENOUGH_RESOURCES,
                  m_engage_human_operator->engageHuman(transaction, *it, KEY_WORKER_DRUID_NOVICE, 1).m_exit_code);
    }

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_human_persistence_facade->subtractHuman(transaction, m_id_holder_12, KEY_WORKER_JOBLESS_NOVICE, 1000);

        // TODO: Subtract resource set. The same in build building and destroy building operators.
        m_resource_persistence_facade->addResource(transaction, m_id_holder_11, KEY_RESOURCE_COAL, 1000);
        m_resource_persistence_facade->addResource(transaction, m_id_holder_11, KEY_RESOURCE_FOOD, 10000);
        m_resource_persistence_facade->addResource(transaction, m_id_holder_11, KEY_RESOURCE_GOLD, 10000);
        m_resource_persistence_facade->addResource(transaction, m_id_holder_11, KEY_RESOURCE_IRON, 1000);
        m_resource_persistence_facade->addResource(transaction, m_id_holder_11, KEY_RESOURCE_MANA, 1000);
        m_resource_persistence_facade->addResource(transaction, m_id_holder_11, KEY_RESOURCE_ROCK, 1000);
        m_resource_persistence_facade->addResource(transaction, m_id_holder_11, KEY_RESOURCE_WOOD, 1000);

        transaction->commit();
    }

    // Test assertions. TODO: Resources are not being checked correctly.
    assertDatabaseUntouched();
}

TEST_F(EngageHumanOperatorTest, engageHuman_NotEnoughResources_AllResources)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        // TODO: Subtract resource set. The same in build building and destroy building operators.
        m_resource_persistence_facade->subtractResource(transaction, m_id_holder_11, KEY_RESOURCE_COAL, 999);
        m_resource_persistence_facade->subtractResource(transaction, m_id_holder_11, KEY_RESOURCE_FOOD, 9999);
        m_resource_persistence_facade->subtractResource(transaction, m_id_holder_11, KEY_RESOURCE_GOLD, 9999);
        m_resource_persistence_facade->subtractResource(transaction, m_id_holder_11, KEY_RESOURCE_IRON, 999);
        m_resource_persistence_facade->subtractResource(transaction, m_id_holder_11, KEY_RESOURCE_MANA, 999);
        m_resource_persistence_facade->subtractResource(transaction, m_id_holder_11, KEY_RESOURCE_ROCK, 999);
        m_resource_persistence_facade->subtractResource(transaction, m_id_holder_11, KEY_RESOURCE_WOOD, 999);

        transaction->commit();
    }

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_NOT_ENOUGH_RESOURCES,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_WORKER_DRUID_NOVICE, 2).m_exit_code);
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);
    vector<R::Volume> expected_volumes_3 = assign::list_of(1)(1)(1)(1)(1)(1)(1);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_3);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_NotEnoughResources_SomeResources)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_resource_persistence_facade->subtractResource(transaction, m_id_holder_11, KEY_RESOURCE_COAL, 999);
        m_resource_persistence_facade->subtractResource(transaction, m_id_holder_11, KEY_RESOURCE_FOOD, 9999);
        m_resource_persistence_facade->subtractResource(transaction, m_id_holder_11, KEY_RESOURCE_WOOD, 999);

        transaction->commit();
    }

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_NOT_ENOUGH_RESOURCES,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_WORKER_DRUID_NOVICE, 2).m_exit_code);
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);
    vector<R::Volume> expected_volumes_3 = assign::list_of(1)(1)(10000)(1000)(1000)(1000)(1);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_3);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_NotEnoughResources_OneResource)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_resource_persistence_facade->subtractResource(transaction, m_id_holder_11, KEY_RESOURCE_ROCK, 999);

        transaction->commit();
    }

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_NOT_ENOUGH_RESOURCES,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_WORKER_DRUID_NOVICE, 2).m_exit_code);
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);
    vector<R::Volume> expected_volumes_3 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1)(1000);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_3);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_NotEnoughPlaceInBuildings_ZeroBuildings)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_NOT_ENOUGH_BUILDINGS,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE, 1).m_exit_code);
    }

    assertDatabaseUntouched();
}

TEST_F(EngageHumanOperatorTest, engageHuman_NotEnoughPlaceInBuildings_TooFewBuildings)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_building_persistence_facade->addBuilding(transaction, m_id_holder_11, KEY_REGULAR_BARRACKS, 1);

        transaction->commit();
    }

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_NOT_ENOUGH_BUILDINGS,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE, 11).m_exit_code);
    }

    assertDatabaseUntouched();
}

TEST_F(EngageHumanOperatorTest, engageHuman_NotEnoughPlaceInBuildings_OneBuilding_TheSameTypeOfHumanOccupying)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_building_persistence_facade->addBuilding(transaction, m_id_holder_11, KEY_REGULAR_BARRACKS, 1);

        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE, 5);

        transaction->commit();
    }

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_NOT_ENOUGH_BUILDINGS,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE, 6).m_exit_code);
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_1);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(2, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
        ASSERT_EQ(5, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_NotEnoughPlaceInBuildings_OneBuilding_OneTypeOfHumanOccupying)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_building_persistence_facade->addBuilding(transaction, m_id_holder_11, KEY_REGULAR_BARRACKS, 1);

        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_HORSEMAN_ADVANCED, 5);

        transaction->commit();
    }

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_NOT_ENOUGH_BUILDINGS,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE, 6).m_exit_code);
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_1);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(2, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
        ASSERT_EQ(5, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_HORSEMAN_ADVANCED)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_NotEnoughPlaceInBuildings_OneBuilding_ManyTypesOfHumansOccupying)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_building_persistence_facade->addBuilding(transaction, m_id_holder_11, KEY_REGULAR_BARRACKS, 1);

        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_ADVANCED, 3);
        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_HORSEMAN_NOVICE, 1);
        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_INFANTRYMAN_NOVICE, 2);

        transaction->commit();
    }

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_NOT_ENOUGH_BUILDINGS,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE, 6).m_exit_code);
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_1);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(4, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
        ASSERT_EQ(3, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_ADVANCED)->getVolume());
        ASSERT_EQ(1, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_HORSEMAN_NOVICE)->getVolume());
        ASSERT_EQ(2, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_INFANTRYMAN_NOVICE)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_NotEnoughPlaceInBuildings_ManyBuildings)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_building_persistence_facade->addBuilding(transaction, m_id_holder_11, KEY_REGULAR_BARRACKS, 3);

        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE, 3);
        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_ADVANCED, 7);
        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_HORSEMAN_NOVICE, 6);
        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_HORSEMAN_ADVANCED, 4);
        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_INFANTRYMAN_NOVICE, 2);
        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_INFANTRYMAN_ADVANCED, 5);

        transaction->commit();
    }

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_NOT_ENOUGH_BUILDINGS,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE, 6).m_exit_code);
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_1);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(7, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
        ASSERT_EQ(3, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE)->getVolume());
        ASSERT_EQ(7, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_ADVANCED)->getVolume());
        ASSERT_EQ(6, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_HORSEMAN_NOVICE)->getVolume());
        ASSERT_EQ(4, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_HORSEMAN_ADVANCED)->getVolume());
        ASSERT_EQ(2, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_INFANTRYMAN_NOVICE)->getVolume());
        ASSERT_EQ(5, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_INFANTRYMAN_ADVANCED)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_BuildingNotRequired_Engaged_One)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_HUMAN_HAS_BEEN_ENGAGED,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_WORKER_DRUID_NOVICE, 1).m_exit_code);

        transaction->commit();
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);
    vector<R::Volume> expected_volumes_3 = assign::list_of(990)(9990)(9990)(990)(990)(990)(990);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_3);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(2, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(999, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
        ASSERT_EQ(1, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_DRUID_NOVICE)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_BuildingNotRequired_Engaged_Some)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_HUMAN_HAS_BEEN_ENGAGED,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_WORKER_DRUID_NOVICE, 63).m_exit_code);

        transaction->commit();
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);
    vector<R::Volume> expected_volumes_3 = assign::list_of(370)(9370)(9370)(370)(370)(370)(370);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_3);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(2, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(937, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
        ASSERT_EQ(63, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_DRUID_NOVICE)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_BuildingNotRequired_Engaged_Max_OnResources)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_HUMAN_HAS_BEEN_ENGAGED,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_WORKER_DRUID_NOVICE, 100).m_exit_code);

        transaction->commit();
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);
    vector<R::Volume> expected_volumes_3 = assign::list_of(0)(9000)(9000)(0)(0)(0)(0);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_3);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(2, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(900, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
        ASSERT_EQ(100, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_DRUID_NOVICE)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_BuildingNotRequired_Engaged_Max_OnJobless)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_human_persistence_facade->subtractHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE, 930);

        transaction->commit();
    }

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_HUMAN_HAS_BEEN_ENGAGED,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_WORKER_DRUID_NOVICE, 70).m_exit_code);

        transaction->commit();
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);
    vector<R::Volume> expected_volumes_3 = assign::list_of(300)(9300)(9300)(300)(300)(300)(300);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_3);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(70, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_DRUID_NOVICE)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_BuildingRequired_Engaged_One)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_building_persistence_facade->addBuilding(transaction, m_id_holder_11, KEY_REGULAR_BARRACKS, 1);

        transaction->commit();
    }

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_HUMAN_HAS_BEEN_ENGAGED,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE, 1).m_exit_code);

        transaction->commit();
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);
    vector<R::Volume> expected_volumes_3 = assign::list_of(990)(9990)(9990)(990)(990)(990)(990);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_3);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(2, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(999, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
        ASSERT_EQ(1, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_BuildingRequired_Engaged_Some)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_building_persistence_facade->addBuilding(transaction, m_id_holder_11, KEY_REGULAR_BARRACKS, 1);

        transaction->commit();
    }

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_HUMAN_HAS_BEEN_ENGAGED,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE, 6).m_exit_code);

        transaction->commit();
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);
    vector<R::Volume> expected_volumes_3 = assign::list_of(940)(9940)(9940)(940)(940)(940)(940);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_3);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(2, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(994, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
        ASSERT_EQ(6, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_BuildingRequired_Engaged_Max_OnResources)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_building_persistence_facade->addBuilding(transaction, m_id_holder_11, KEY_REGULAR_BARRACKS, 11);

        transaction->commit();

    }

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_HUMAN_HAS_BEEN_ENGAGED,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE, 100).m_exit_code);

        transaction->commit();
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);
    vector<R::Volume> expected_volumes_3 = assign::list_of(0)(9000)(9000)(0)(0)(0)(0);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_3);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(2, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(900, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
        ASSERT_EQ(100, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_BuildingRequired_Engaged_Max_OnJobless)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_human_persistence_facade->subtractHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE, 930);

        m_building_persistence_facade->addBuilding(transaction, m_id_holder_11, KEY_REGULAR_BARRACKS, 11);

        transaction->commit();
    }

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_HUMAN_HAS_BEEN_ENGAGED,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE, 70).m_exit_code);

        transaction->commit();
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);
    vector<R::Volume> expected_volumes_3 = assign::list_of(300)(9300)(9300)(300)(300)(300)(300);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_3);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(70, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_BuildingRequired_Engaged_Max_OnBuildings_TheSameTypeOfHumanOccupying)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_building_persistence_facade->addBuilding(transaction, m_id_holder_11, KEY_REGULAR_BARRACKS, 3);

        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE, 24);

        transaction->commit();
    }

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_HUMAN_HAS_BEEN_ENGAGED,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE, 6).m_exit_code);

        transaction->commit();
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);
    vector<R::Volume> expected_volumes_3 = assign::list_of(940)(9940)(9940)(940)(940)(940)(940);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_3);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(2, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(994, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
        ASSERT_EQ(30, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_BuildingRequired_Engaged_Max_OnBuildings_OneTypeOfHumanOccupying)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_building_persistence_facade->addBuilding(transaction, m_id_holder_11, KEY_REGULAR_BARRACKS, 3);

        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_INFANTRYMAN_NOVICE, 24);

        transaction->commit();
    }

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_HUMAN_HAS_BEEN_ENGAGED,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE, 6).m_exit_code);

        transaction->commit();
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);
    vector<R::Volume> expected_volumes_3 = assign::list_of(940)(9940)(9940)(940)(940)(940)(940);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_3);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(3, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(994, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
        ASSERT_EQ(6, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE)->getVolume());
        ASSERT_EQ(24, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_INFANTRYMAN_NOVICE)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}

TEST_F(EngageHumanOperatorTest, engageHuman_BuildingRequired_Engaged_Max_OnBuildings_ManyTypesOfHumansOccupying)
{
    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        m_building_persistence_facade->addBuilding(transaction, m_id_holder_11, KEY_REGULAR_BARRACKS, 3);

        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE, 2);
        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_ADVANCED, 6);
        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_HORSEMAN_NOVICE, 5);
        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_HORSEMAN_ADVANCED, 4);
        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_INFANTRYMAN_NOVICE, 2);
        m_human_persistence_facade->addHuman(transaction, m_id_holder_11, KEY_SOLDIER_INFANTRYMAN_ADVANCED, 5);

        transaction->commit();
    }

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ASSERT_EQ(ENGAGE_HUMAN_OPERATOR_EXIT_CODE_HUMAN_HAS_BEEN_ENGAGED,
                  m_engage_human_operator->engageHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE, 6).m_exit_code);

        transaction->commit();
    }

    vector<R::Volume> expected_volumes_1 = assign::list_of(1000)(10000)(10000)(1000)(1000)(1000)(1000);
    vector<R::Volume> expected_volumes_2 = assign::list_of(0)(0)(0)(0)(0)(0)(0);
    vector<R::Volume> expected_volumes_3 = assign::list_of(940)(9940)(9940)(940)(940)(940)(940);

    {
        IConnectionShrPtr connection = m_persistence.getConnection();
        ITransactionShrPtr transaction = m_persistence.getTransaction(connection);

        ResourceSet resource_set_11 = m_resource_persistence_facade->getResources(transaction, m_id_holder_11);
        ResourceSet resource_set_12 = m_resource_persistence_facade->getResources(transaction, m_id_holder_12);
        ResourceSet resource_set_21 = m_resource_persistence_facade->getResources(transaction, m_id_holder_21);

        compareResourceSet(resource_set_11, expected_volumes_3);
        compareResourceSet(resource_set_12, expected_volumes_2);
        compareResourceSet(resource_set_21, expected_volumes_1);

        ASSERT_EQ(7, m_human_persistence_facade->getHumans(transaction, m_id_holder_11).size());
        ASSERT_TRUE(m_human_persistence_facade->getHumans(transaction, m_id_holder_12).empty());
        ASSERT_EQ(1, m_human_persistence_facade->getHumans(transaction, m_id_holder_21).size());

        ASSERT_EQ(994, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
        ASSERT_EQ(8, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_NOVICE)->getVolume());
        ASSERT_EQ(6, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_ARCHER_ADVANCED)->getVolume());
        ASSERT_EQ(5, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_HORSEMAN_NOVICE)->getVolume());
        ASSERT_EQ(4, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_HORSEMAN_ADVANCED)->getVolume());
        ASSERT_EQ(2, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_INFANTRYMAN_NOVICE)->getVolume());
        ASSERT_EQ(5, m_human_persistence_facade->getHuman(transaction, m_id_holder_11, KEY_SOLDIER_INFANTRYMAN_ADVANCED)->getVolume());
        ASSERT_EQ(1000, m_human_persistence_facade->getHuman(transaction, m_id_holder_21, KEY_WORKER_JOBLESS_NOVICE)->getVolume());
    }
}
