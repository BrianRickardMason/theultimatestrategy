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

#include "../../GameServer/Land/LandManager.hpp"
#include "../Persistency/PersistencyDummy.hpp"
#include "LandManagerAccessorMock.hpp"

using namespace GameServer::Epoch;
using namespace GameServer::Land;
using namespace GameServer::Persistency;
using namespace GameServer::User;
using namespace GameServer::World;
using namespace boost;
using namespace std;

using testing::Return;
using testing::Throw;

/**
 * @brief A test class.
 */
class LandManagerTest
    : public testing::Test
{
protected:
    /**
     * @brief Constructs a test class.
     */
    LandManagerTest()
        : m_id_epoch_1(1),
          m_id_epoch_2(2),
          m_id_land_1(1),
          m_id_land_2(2),
          m_id_land_3(3),
          m_id_user_1(1),
          m_id_user_2(2),
          m_id_world_1(1),
          m_id_world_2(2)
    {
    }

    /**
     * @brief Compares the land with expected values.
     *
     * @param a_land     The land to be compared.
     * @param a_id_user  An expected identifier of the user.
     * @param a_id_world An expected identifier of the world.
     * @param a_id_epoch The expected identifier of the epoch.
     * @param a_id_land  An expected identifier of the land.
     * @param a_name     An expected name of the land.
     * @param a_granted  An expected state of the granted of the land.
     */
    void compareLand(
        LandShrPtr          a_land,
        IDUser      const & a_id_user,
        IDWorld     const & a_id_world,
        IDEpoch     const & a_id_epoch,
        IDLand      const & a_id_land,
        std::string const & a_name,
        bool                a_granted
    )
    {
        ASSERT_TRUE(a_id_user == a_land->getIDUser());
        ASSERT_TRUE(a_id_world == a_land->getIDWorld());
        ASSERT_TRUE(a_id_epoch == a_land->getIDEpoch());
        ASSERT_TRUE(a_id_land == a_land->getIDLand());
        ASSERT_STREQ(a_name.c_str(), a_land->getName().c_str());
        ASSERT_EQ(a_granted, a_land->getGranted());
    }

    /**
     * @brief Test constants identifiers of the epoch.
     */
    IDEpoch m_id_epoch_1,
            m_id_epoch_2;

    /**
     * @brief Test constants identifiers of the land.
     */
    IDLand m_id_land_1,
           m_id_land_2,
           m_id_land_3;

    /**
     * @brief Test constants identifiers of the user.
     */
    IDUser m_id_user_1,
           m_id_user_2;

    /**
     * @brief Test constants identifiers of the world.
     */
    IDWorld m_id_world_1,
            m_id_world_2;
};

TEST_F(LandManagerTest, LandManager)
{
    ILandManagerAccessorAutPtr accessor(new LandManagerAccessorMock);

    LandManager manager(accessor);
}

TEST_F(LandManagerTest, createLand_Success)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    EXPECT_CALL(*mock, insertRecord(transaction, m_id_user_1, m_id_world_1, m_id_epoch_1, "Land1"));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    ASSERT_TRUE(manager.createLand(transaction, m_id_user_1, m_id_world_1, m_id_epoch_1, "Land1"));
}

TEST_F(LandManagerTest, createLand_Failure)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    std::exception e;

    EXPECT_CALL(*mock, insertRecord(transaction, m_id_user_1, m_id_world_1, m_id_epoch_1, "Land1"))
    .WillOnce(Throw(e));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    ASSERT_FALSE(manager.createLand(transaction, m_id_user_1, m_id_world_1, m_id_epoch_1, "Land1"));
}

TEST_F(LandManagerTest, deleteLand_Success)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    EXPECT_CALL(*mock, deleteRecord(transaction, m_id_land_1));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    ASSERT_TRUE(manager.deleteLand(transaction, m_id_land_1));
}

TEST_F(LandManagerTest, deleteLand_Failure)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    std::exception e;

    EXPECT_CALL(*mock, deleteRecord(transaction, m_id_land_1))
    .WillOnce(Throw(e));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    ASSERT_FALSE(manager.deleteLand(transaction, m_id_land_1));
}

TEST_F(LandManagerTest, getLand_ByIDLand_LandDoesNotExist)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    EXPECT_CALL(*mock, getRecord(transaction, m_id_land_1))
    .WillOnce(Return(LandRecordShrPtr()));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    LandShrPtr land = manager.getLand(transaction, m_id_land_1);

    ASSERT_TRUE(land == NULL);
}

TEST_F(LandManagerTest, getLand_ByIDLand_LandDoesExist)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    EXPECT_CALL(*mock, getRecord(transaction, m_id_land_1))
    .WillOnce(Return(make_shared<LandRecord>(m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_1, "Land1", true)));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    LandShrPtr land = manager.getLand(transaction, m_id_land_1);

    ASSERT_TRUE(land != NULL);

    compareLand(land, m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_1, "Land1", true);
}

TEST_F(LandManagerTest, getLand_ByNameAndIDUser_LandDoesNotExist)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    EXPECT_CALL(*mock, getRecord(transaction, "Land1", m_id_user_1))
    .WillOnce(Return(LandRecordShrPtr()));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    LandShrPtr land = manager.getLand(transaction, "Land1", m_id_user_1);

    ASSERT_TRUE(land == NULL);
}

TEST_F(LandManagerTest, getLand_ByNameAndIDUser_LandDoesExist)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    EXPECT_CALL(*mock, getRecord(transaction, "Land1", m_id_user_1))
    .WillOnce(Return(make_shared<LandRecord>(m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_3, "Land1", true)));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    LandShrPtr land = manager.getLand(transaction, "Land1", m_id_user_1);

    ASSERT_TRUE(land != NULL);

    compareLand(land, m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_3, "Land1", true);
}

TEST_F(LandManagerTest, getLand_ByNameAndIDWorld_LandDoesNotExist)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    EXPECT_CALL(*mock, getRecord(transaction, "Land1", m_id_world_1))
    .WillOnce(Return(LandRecordShrPtr()));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    LandShrPtr land = manager.getLand(transaction, "Land1", m_id_world_1);

    ASSERT_TRUE(land == NULL);
}

TEST_F(LandManagerTest, getLand_ByNameAndIDWorld_LandDoesExist)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    EXPECT_CALL(*mock, getRecord(transaction, "Land1", m_id_world_1))
    .WillOnce(Return(make_shared<LandRecord>(m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_3, "Land1", true)));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    LandShrPtr land = manager.getLand(transaction, "Land1", m_id_world_1);

    ASSERT_TRUE(land != NULL);

    compareLand(land, m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_3, "Land1", true);
}

TEST_F(LandManagerTest, getLands_LandsDoNotExist)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    EXPECT_CALL(*mock, getRecords(transaction))
    .WillOnce(Return(LandRecordMap()));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    LandMap lands = manager.getLands(transaction);

    ASSERT_TRUE(lands.empty());
}

TEST_F(LandManagerTest, getLands_LandsDoExist_OneLand)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    LandRecordMap map;
    map.insert(make_pair(m_id_land_1, make_shared<LandRecord>(m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_1, "Land1", false)));

    EXPECT_CALL(*mock, getRecords(transaction))
    .WillOnce(Return(map));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    LandMap lands = manager.getLands(transaction);

    ASSERT_FALSE(lands.empty());

    ASSERT_EQ(1, lands.size());

    compareLand(lands[m_id_land_1], m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_1, "Land1", false);
}

TEST_F(LandManagerTest, getLands_LandsDoExist_ManyLands)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    LandRecordMap map;
    map.insert(make_pair(m_id_land_1, make_shared<LandRecord>(m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_1, "Land1", false)));
    map.insert(make_pair(m_id_land_2, make_shared<LandRecord>(m_id_user_2, m_id_world_2, m_id_epoch_2, m_id_land_2, "Land2", true)));

    EXPECT_CALL(*mock, getRecords(transaction))
    .WillOnce(Return(map));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    LandMap lands = manager.getLands(transaction);

    ASSERT_FALSE(lands.empty());

    ASSERT_EQ(2, lands.size());

    compareLand(lands[m_id_land_1], m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_1, "Land1", false);
    compareLand(lands[m_id_land_2], m_id_user_2, m_id_world_2, m_id_epoch_2, m_id_land_2, "Land2", true);
}

TEST_F(LandManagerTest, getLands_ByIDUser_LandsDoNotExist)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    EXPECT_CALL(*mock, getRecords(transaction, m_id_user_1))
    .WillOnce(Return(LandRecordMap()));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    LandMap lands = manager.getLands(transaction, m_id_user_1);

    ASSERT_TRUE(lands.empty());
}

TEST_F(LandManagerTest, getLands_ByIDUser_LandsDoExist_OneLand)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    LandRecordMap map;
    map.insert(make_pair(m_id_land_1, make_shared<LandRecord>(m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_1, "Land1", false)));

    EXPECT_CALL(*mock, getRecords(transaction, m_id_user_1))
    .WillOnce(Return(map));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    LandMap lands = manager.getLands(transaction, m_id_user_1);

    ASSERT_FALSE(lands.empty());

    ASSERT_EQ(1, lands.size());

    compareLand(lands[m_id_land_1], m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_1, "Land1", false);
}

TEST_F(LandManagerTest, getLands_ByIDUser_LandsDoExist_ManyLands)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    LandRecordMap map;
    map.insert(make_pair(m_id_land_1, make_shared<LandRecord>(m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_1, "Land1", false)));
    map.insert(make_pair(m_id_land_2, make_shared<LandRecord>(m_id_user_1, m_id_world_2, m_id_epoch_2, m_id_land_2, "Land2", true)));

    EXPECT_CALL(*mock, getRecords(transaction, m_id_user_1))
    .WillOnce(Return(map));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    LandMap lands = manager.getLands(transaction, m_id_user_1);

    ASSERT_FALSE(lands.empty());

    ASSERT_EQ(2, lands.size());

    compareLand(lands[m_id_land_1], m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_1, "Land1", false);
    compareLand(lands[m_id_land_2], m_id_user_1, m_id_world_2, m_id_epoch_2, m_id_land_2, "Land2", true);
}

TEST_F(LandManagerTest, getLands_ByIDWorld_LandsDoNotExist)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    EXPECT_CALL(*mock, getRecords(transaction, m_id_world_1))
    .WillOnce(Return(LandRecordMap()));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    LandMap lands = manager.getLands(transaction, m_id_world_1);

    ASSERT_TRUE(lands.empty());
}

TEST_F(LandManagerTest, getLands_ByIDWorld_LandsDoExist_OneLand)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    LandRecordMap map;
    map.insert(make_pair(m_id_land_1, make_shared<LandRecord>(m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_1, "Land1", false)));

    EXPECT_CALL(*mock, getRecords(transaction, m_id_world_1))
    .WillOnce(Return(map));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    LandMap lands = manager.getLands(transaction, m_id_world_1);

    ASSERT_FALSE(lands.empty());

    ASSERT_EQ(1, lands.size());

    compareLand(lands[m_id_land_1], m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_1, "Land1", false);
}

TEST_F(LandManagerTest, getLands_ByIDWorld_LandsDoExist_ManyLands)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    LandRecordMap map;
    map.insert(make_pair(m_id_land_1, make_shared<LandRecord>(m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_1, "Land1", false)));
    map.insert(make_pair(m_id_land_2, make_shared<LandRecord>(m_id_user_2, m_id_world_1, m_id_epoch_1, m_id_land_2, "Land2", true)));

    EXPECT_CALL(*mock, getRecords(transaction, m_id_world_1))
    .WillOnce(Return(map));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    LandMap lands = manager.getLands(transaction, m_id_world_1);

    ASSERT_FALSE(lands.empty());

    ASSERT_EQ(2, lands.size());

    compareLand(lands[m_id_land_1], m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_1, "Land1", false);
    compareLand(lands[m_id_land_2], m_id_user_2, m_id_world_1, m_id_epoch_1, m_id_land_2, "Land2", true);
}

TEST_F(LandManagerTest, getLands_ByIDUserAndWorld_LandsDoNotExist)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    EXPECT_CALL(*mock, getRecords(transaction, m_id_user_1, m_id_world_1))
    .WillOnce(Return(LandRecordMap()));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    LandMap lands = manager.getLands(transaction, m_id_user_1, m_id_world_1);

    ASSERT_TRUE(lands.empty());
}

TEST_F(LandManagerTest, getLands_ByIDUserAndIDWorld_LandsDoExist_OneLand)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    LandRecordMap map;
    map.insert(make_pair(m_id_land_1, make_shared<LandRecord>(m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_1, "Land1", false)));

    EXPECT_CALL(*mock, getRecords(transaction, m_id_user_1, m_id_world_1))
    .WillOnce(Return(map));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    LandMap lands = manager.getLands(transaction, m_id_user_1, m_id_world_1);

    ASSERT_FALSE(lands.empty());

    ASSERT_EQ(1, lands.size());

    compareLand(lands[m_id_land_1], m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_1, "Land1", false);
}

TEST_F(LandManagerTest, getLands_ByIDUserAndIDWorld_LandsDoExist_ManyLands)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    LandRecordMap map;
    map.insert(make_pair(m_id_land_1, make_shared<LandRecord>(m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_1, "Land1", false)));
    map.insert(make_pair(m_id_land_2, make_shared<LandRecord>(m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_2, "Land2", true)));

    EXPECT_CALL(*mock, getRecords(transaction, m_id_user_1, m_id_world_1))
    .WillOnce(Return(map));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    LandMap lands = manager.getLands(transaction, m_id_user_1, m_id_world_1);

    ASSERT_FALSE(lands.empty());

    ASSERT_EQ(2, lands.size());

    compareLand(lands[m_id_land_1], m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_1, "Land1", false);
    compareLand(lands[m_id_land_2], m_id_user_1, m_id_world_1, m_id_epoch_1, m_id_land_2, "Land2", true);
}

TEST_F(LandManagerTest, markGranted)
{
    ITransactionShrPtr transaction(new TransactionDummy);

    LandManagerAccessorMock * mock = new LandManagerAccessorMock;

    EXPECT_CALL(*mock, markGranted(transaction, m_id_land_1));

    ILandManagerAccessorAutPtr accessor(mock);

    LandManager manager(accessor);

    ASSERT_NO_THROW(manager.markGranted(transaction, m_id_land_1));
}
