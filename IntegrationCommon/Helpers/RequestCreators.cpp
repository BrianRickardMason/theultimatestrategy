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

#include "RequestCreators.hpp"
#include <boost/make_shared.hpp>

using namespace Network::XmlRPCCommon::Request;
using namespace Network::XmlRPCCommon::Xml;
using namespace boost;
using namespace std;

RequestShrPtr createRequestBuildOrDestroyBuilding(
    unsigned short int const   a_id_request,
    unsigned int       const   a_id_user,
    string             const & a_password,
    unsigned int       const   a_id_holder_class,
    unsigned int       const   a_id_holder,
    unsigned int       const   a_id_building_class,
    unsigned int       const   a_id_building,
    unsigned int       const   a_volume
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(a_id_request);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idholderclass = parameters->appendNode("idholderclass");
    idholderclass->appendAttribute("type")->setValue("unsigned integer");
    idholderclass->appendAttribute("value")->setValue(a_id_holder_class);

    IXmlNodeShrPtr idholder = parameters->appendNode("idholder");
    idholder->appendAttribute("type")->setValue("unsigned integer");
    idholder->appendAttribute("value")->setValue(a_id_holder);

    IXmlNodeShrPtr idbuildingclass = parameters->appendNode("idbuildingclass");
    idbuildingclass->appendAttribute("type")->setValue("unsigned integer");
    idbuildingclass->appendAttribute("value")->setValue(a_id_building_class);

    IXmlNodeShrPtr idbuilding = parameters->appendNode("idbuilding");
    idbuilding->appendAttribute("type")->setValue("unsigned integer");
    idbuilding->appendAttribute("value")->setValue(a_id_building);

    IXmlNodeShrPtr volume = parameters->appendNode("volume");
    volume->appendAttribute("type")->setValue("unsigned integer");
    volume->appendAttribute("value")->setValue(a_volume);

    return request;
}

RequestShrPtr createRequestEcho()
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_ECHO);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    return request;
}

RequestShrPtr createRequestCreateUser(
    string const & a_login,
    string const & a_password
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_CREATE_USER);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr login = parameters->appendNode("login");
    login->appendAttribute("type")->setValue("string");
    login->appendAttribute("value")->setValue(a_login.c_str());

    IXmlNodeShrPtr password = parameters->appendNode("password");
    password->appendAttribute("type")->setValue("string");
    password->appendAttribute("value")->setValue(a_password.c_str());

    return request;
}

RequestShrPtr createRequestCreateWorld(
    unsigned int const   a_id_user,
    string       const & a_password,
    string       const & a_name
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_CREATE_WORLD);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr name = parameters->appendNode("name");
    name->appendAttribute("type")->setValue("string");
    name->appendAttribute("value")->setValue(a_name.c_str());

    return request;
}

RequestShrPtr createRequestCreateEpoch(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_world
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_CREATE_EPOCH);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr name = parameters->appendNode("idworld");
    name->appendAttribute("type")->setValue("unsigned integer");
    name->appendAttribute("value")->setValue(a_id_world);

    return request;
}

RequestShrPtr createRequestDeleteEpoch(
    unsigned int const a_id_world
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_DELETE_EPOCH);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr name = parameters->appendNode("idworld");
    name->appendAttribute("type")->setValue("unsigned integer");
    name->appendAttribute("value")->setValue(a_id_world);

    return request;
}

RequestShrPtr createRequestGetEpoch(
    unsigned int const a_id_world
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_GET_EPOCH);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr name = parameters->appendNode("idworld");
    name->appendAttribute("type")->setValue("unsigned integer");
    name->appendAttribute("value")->setValue(a_id_world);

    return request;
}

RequestShrPtr createRequestActivateEpoch(
    unsigned int const a_id_world
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_ACTIVATE_EPOCH);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr name = parameters->appendNode("idworld");
    name->appendAttribute("type")->setValue("unsigned integer");
    name->appendAttribute("value")->setValue(a_id_world);

    return request;
}

RequestShrPtr createRequestDeactivateEpoch(
    unsigned int const a_id_world
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_DEACTIVATE_EPOCH);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr name = parameters->appendNode("idworld");
    name->appendAttribute("type")->setValue("unsigned integer");
    name->appendAttribute("value")->setValue(a_id_world);

    return request;
}

RequestShrPtr createRequestFinishEpoch(
    unsigned int const a_id_world
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_FINISH_EPOCH);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr name = parameters->appendNode("idworld");
    name->appendAttribute("type")->setValue("unsigned integer");
    name->appendAttribute("value")->setValue(a_id_world);

    return request;
}

RequestShrPtr createRequestTickEpoch(
    unsigned int const a_id_world
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_TICK_EPOCH);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr name = parameters->appendNode("idworld");
    name->appendAttribute("type")->setValue("unsigned integer");
    name->appendAttribute("value")->setValue(a_id_world);

    return request;
}

RequestShrPtr createRequestCreateLand(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_world,
    unsigned int const   a_id_epoch,
    string       const & a_name
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_CREATE_LAND);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idworld = parameters->appendNode("idworld");
    idworld->appendAttribute("type")->setValue("unsigned integer");
    idworld->appendAttribute("value")->setValue(a_id_world);

    IXmlNodeShrPtr idepoch = parameters->appendNode("idepoch");
    idepoch->appendAttribute("type")->setValue("unsigned integer");
    idepoch->appendAttribute("value")->setValue(a_id_epoch);

    IXmlNodeShrPtr name = parameters->appendNode("name");
    name->appendAttribute("type")->setValue("string");
    name->appendAttribute("value")->setValue(a_name.c_str());

    return request;
}

RequestShrPtr createRequestDeleteLand(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_land
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_DELETE_LAND);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idland = parameters->appendNode("idland");
    idland->appendAttribute("type")->setValue("unsigned integer");
    idland->appendAttribute("value")->setValue(a_id_land);

    return request;
}

RequestShrPtr createRequestGetLandByIDLand(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_land
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_GET_LAND_BY_ID_LAND);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idland = parameters->appendNode("idland");
    idland->appendAttribute("type")->setValue("unsigned integer");
    idland->appendAttribute("value")->setValue(a_id_land);

    return request;
}

RequestShrPtr createRequestGetLandByName(
    unsigned int const   a_id_user,
    string       const & a_password,
    string       const & a_name
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_GET_LAND_BY_NAME);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idland = parameters->appendNode("name");
    idland->appendAttribute("type")->setValue("string");
    idland->appendAttribute("value")->setValue(a_name.c_str());

    return request;
}

RequestShrPtr createRequestGetLandsByIDWorld(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_world
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_GET_LANDS_BY_ID_WORLD);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idworld = parameters->appendNode("idworld");
    idworld->appendAttribute("type")->setValue("unsigned integer");
    idworld->appendAttribute("value")->setValue(a_id_world);

    return request;
}

RequestShrPtr createRequestCreateSettlement(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_land,
    string       const & a_name
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_CREATE_SETTLEMENT);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idland = parameters->appendNode("idland");
    idland->appendAttribute("type")->setValue("unsigned integer");
    idland->appendAttribute("value")->setValue(a_id_land);

    IXmlNodeShrPtr name = parameters->appendNode("name");
    name->appendAttribute("type")->setValue("string");
    name->appendAttribute("value")->setValue(a_name.c_str());

    return request;
}

RequestShrPtr createRequestDeleteSettlement(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_settlement
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_DELETE_SETTLEMENT);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idsettlement = parameters->appendNode("idsettlement");
    idsettlement->appendAttribute("type")->setValue("unsigned integer");
    idsettlement->appendAttribute("value")->setValue(a_id_settlement);

    return request;
}

RequestShrPtr createRequestGetSettlementByIDLandAndName(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_land,
    string       const & a_name
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_GET_SETTLEMENT_BY_ID_LAND_AND_NAME);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idland = parameters->appendNode("idland");
    idland->appendAttribute("type")->setValue("unsigned integer");
    idland->appendAttribute("value")->setValue(a_id_land);

    IXmlNodeShrPtr name = parameters->appendNode("name");
    name->appendAttribute("type")->setValue("string");
    name->appendAttribute("value")->setValue(a_name.c_str());

    return request;
}

RequestShrPtr createRequestGetSettlementByIDSettlement(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_settlement
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_GET_SETTLEMENT_BY_ID_SETTLEMENT);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idland = parameters->appendNode("idsettlement");
    idland->appendAttribute("type")->setValue("unsigned integer");
    idland->appendAttribute("value")->setValue(a_id_settlement);

    return request;
}

RequestShrPtr createRequestGetSettlements(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_land
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_GET_SETTLEMENTS_BY_ID_LAND);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idland = parameters->appendNode("idland");
    idland->appendAttribute("type")->setValue("unsigned integer");
    idland->appendAttribute("value")->setValue(a_id_land);

    return request;
}

RequestShrPtr createRequestBuildBuilding(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_holder_class,
    unsigned int const   a_id_holder,
    unsigned int const   a_id_building_class,
    unsigned int const   a_id_building,
    unsigned int const   a_volume
)
{
    return createRequestBuildOrDestroyBuilding(REQUEST_ID_BUILD_BUILDING, a_id_user, a_password, a_id_holder_class, a_id_holder, a_id_building_class, a_id_building, a_volume);
}

RequestShrPtr createRequestDestroyBuilding(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_holder_class,
    unsigned int const   a_id_holder,
    unsigned int const   a_id_building_class,
    unsigned int const   a_id_building,
    unsigned int const   a_volume
)
{
    return createRequestBuildOrDestroyBuilding(REQUEST_ID_DESTROY_BUILDING, a_id_user, a_password, a_id_holder_class, a_id_holder, a_id_building_class, a_id_building, a_volume);
}

RequestShrPtr createRequestGetBuilding(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_holder_class,
    unsigned int const   a_id_holder,
    unsigned int const   a_id_building_class,
    unsigned int const   a_id_building
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_GET_BUILDING);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idholderclass = parameters->appendNode("idholderclass");
    idholderclass->appendAttribute("type")->setValue("unsigned integer");
    idholderclass->appendAttribute("value")->setValue(a_id_holder_class);

    IXmlNodeShrPtr idholder = parameters->appendNode("idholder");
    idholder->appendAttribute("type")->setValue("unsigned integer");
    idholder->appendAttribute("value")->setValue(a_id_holder);

    IXmlNodeShrPtr idbuildingclass = parameters->appendNode("idbuildingclass");
    idbuildingclass->appendAttribute("type")->setValue("unsigned integer");
    idbuildingclass->appendAttribute("value")->setValue(a_id_building_class);

    IXmlNodeShrPtr idbuilding = parameters->appendNode("idbuilding");
    idbuilding->appendAttribute("type")->setValue("unsigned integer");
    idbuilding->appendAttribute("value")->setValue(a_id_building);

    return request;
}

RequestShrPtr createRequestGetBuildings(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_holder_class,
    unsigned int const   a_id_holder
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_GET_BUILDINGS);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idholderclass = parameters->appendNode("idholderclass");
    idholderclass->appendAttribute("type")->setValue("unsigned integer");
    idholderclass->appendAttribute("value")->setValue(a_id_holder_class);

    IXmlNodeShrPtr idholder = parameters->appendNode("idholder");
    idholder->appendAttribute("type")->setValue("unsigned integer");
    idholder->appendAttribute("value")->setValue(a_id_holder);

    return request;
}

RequestShrPtr createRequestEngageHuman(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_holder_class,
    unsigned int const   a_id_holder,
    unsigned int const   a_id_human_class,
    unsigned int const   a_id_human,
    unsigned int const   a_volume
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_ENGAGE_HUMAN);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idholderclass = parameters->appendNode("idholderclass");
    idholderclass->appendAttribute("type")->setValue("unsigned integer");
    idholderclass->appendAttribute("value")->setValue(a_id_holder_class);

    IXmlNodeShrPtr idholder = parameters->appendNode("idholder");
    idholder->appendAttribute("type")->setValue("unsigned integer");
    idholder->appendAttribute("value")->setValue(a_id_holder);

    IXmlNodeShrPtr idhumanclass = parameters->appendNode("idhumanclass");
    idhumanclass->appendAttribute("type")->setValue("unsigned integer");
    idhumanclass->appendAttribute("value")->setValue(a_id_human_class);

    IXmlNodeShrPtr idhuman = parameters->appendNode("idhuman");
    idhuman->appendAttribute("type")->setValue("unsigned integer");
    idhuman->appendAttribute("value")->setValue(a_id_human);

    IXmlNodeShrPtr volume = parameters->appendNode("volume");
    volume->appendAttribute("type")->setValue("unsigned integer");
    volume->appendAttribute("value")->setValue(a_volume);

    return request;
}

RequestShrPtr createRequestDismissHuman(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_holder_class,
    unsigned int const   a_id_holder,
    unsigned int const   a_id_human_class,
    unsigned int const   a_id_human,
    unsigned int const   a_experience,
    unsigned int const   a_volume
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_DISMISS_HUMAN);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idholderclass = parameters->appendNode("idholderclass");
    idholderclass->appendAttribute("type")->setValue("unsigned integer");
    idholderclass->appendAttribute("value")->setValue(a_id_holder_class);

    IXmlNodeShrPtr idholder = parameters->appendNode("idholder");
    idholder->appendAttribute("type")->setValue("unsigned integer");
    idholder->appendAttribute("value")->setValue(a_id_holder);

    IXmlNodeShrPtr idhumanclass = parameters->appendNode("idhumanclass");
    idhumanclass->appendAttribute("type")->setValue("unsigned integer");
    idhumanclass->appendAttribute("value")->setValue(a_id_human_class);

    IXmlNodeShrPtr idhuman = parameters->appendNode("idhuman");
    idhuman->appendAttribute("type")->setValue("unsigned integer");
    idhuman->appendAttribute("value")->setValue(a_id_human);

    IXmlNodeShrPtr experience = parameters->appendNode("experience");
    experience->appendAttribute("type")->setValue("unsigned integer");
    experience->appendAttribute("value")->setValue(a_experience);

    IXmlNodeShrPtr volume = parameters->appendNode("volume");
    volume->appendAttribute("type")->setValue("unsigned integer");
    volume->appendAttribute("value")->setValue(a_volume);

    return request;
}

RequestShrPtr createRequestGetHuman(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_holder_class,
    unsigned int const   a_id_holder,
    unsigned int const   a_id_human_class,
    unsigned int const   a_id_human,
    unsigned int const   a_experience
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_GET_HUMAN);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idholderclass = parameters->appendNode("idholderclass");
    idholderclass->appendAttribute("type")->setValue("unsigned integer");
    idholderclass->appendAttribute("value")->setValue(a_id_holder_class);

    IXmlNodeShrPtr idholder = parameters->appendNode("idholder");
    idholder->appendAttribute("type")->setValue("unsigned integer");
    idholder->appendAttribute("value")->setValue(a_id_holder);

    IXmlNodeShrPtr idhumanclass = parameters->appendNode("idhumanclass");
    idhumanclass->appendAttribute("type")->setValue("unsigned integer");
    idhumanclass->appendAttribute("value")->setValue(a_id_human_class);

    IXmlNodeShrPtr idhuman = parameters->appendNode("idhuman");
    idhuman->appendAttribute("type")->setValue("unsigned integer");
    idhuman->appendAttribute("value")->setValue(a_id_human);

    IXmlNodeShrPtr experience = parameters->appendNode("experience");
    experience->appendAttribute("type")->setValue("unsigned integer");
    experience->appendAttribute("value")->setValue(a_experience);

    return request;
}

RequestShrPtr createRequestGetHumans(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_holder_class,
    unsigned int const   a_id_holder
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_GET_HUMANS);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idholderclass = parameters->appendNode("idholderclass");
    idholderclass->appendAttribute("type")->setValue("unsigned integer");
    idholderclass->appendAttribute("value")->setValue(a_id_holder_class);

    IXmlNodeShrPtr idholder = parameters->appendNode("idholder");
    idholder->appendAttribute("type")->setValue("unsigned integer");
    idholder->appendAttribute("value")->setValue(a_id_holder);

    return request;
}

RequestShrPtr createRequestGetResource(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_holder_class,
    unsigned int const   a_id_holder,
    unsigned int const   a_id_resource
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_GET_RESOURCES);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr holderclass = parameters->appendNode("holderclass");
    holderclass->appendAttribute("type")->setValue("unsigned integer");
    holderclass->appendAttribute("value")->setValue(a_id_holder_class);

    IXmlNodeShrPtr idholder = parameters->appendNode("idholder");
    idholder->appendAttribute("type")->setValue("unsigned integer");
    idholder->appendAttribute("value")->setValue(a_id_holder);

    IXmlNodeShrPtr idresource = parameters->appendNode("idresource");
    idresource->appendAttribute("type")->setValue("unsigned integer");
    idresource->appendAttribute("value")->setValue(a_id_resource);

    return request;
}

RequestShrPtr createRequestGetResources(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_holder_class,
    unsigned int const   a_id_holder
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_GET_RESOURCES);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr holderclass = parameters->appendNode("idholderclass");
    holderclass->appendAttribute("type")->setValue("unsigned integer");
    holderclass->appendAttribute("value")->setValue(a_id_holder_class);

    IXmlNodeShrPtr idholder = parameters->appendNode("idholder");
    idholder->appendAttribute("type")->setValue("unsigned integer");
    idholder->appendAttribute("value")->setValue(a_id_holder);

    return request;
}

RequestShrPtr createRequestTurn(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_land
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_TURN);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idland = parameters->appendNode("idland");
    idland->appendAttribute("type")->setValue("unsigned integer");
    idland->appendAttribute("value")->setValue(a_id_land);

    return request;
}

RequestShrPtr createRequestTransportHuman(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_settlement_source,
    unsigned int const   a_id_settlement_destination,
    unsigned int const   a_id_human_class,
    unsigned int const   a_id_human,
    unsigned int const   a_experience,
    unsigned int const   a_volume
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_TRANSPORT_HUMAN);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idsettlementsource = parameters->appendNode("idsettlementsource");
    idsettlementsource->appendAttribute("type")->setValue("unsigned integer");
    idsettlementsource->appendAttribute("value")->setValue(a_id_settlement_source);

    IXmlNodeShrPtr idsettlementdestination = parameters->appendNode("idsettlementdestination");
    idsettlementdestination->appendAttribute("type")->setValue("unsigned integer");
    idsettlementdestination->appendAttribute("value")->setValue(a_id_settlement_destination);

    IXmlNodeShrPtr idhumanclass = parameters->appendNode("idhumanclass");
    idhumanclass->appendAttribute("type")->setValue("unsigned integer");
    idhumanclass->appendAttribute("value")->setValue(a_id_human_class);

    IXmlNodeShrPtr idhuman = parameters->appendNode("idhuman");
    idhuman->appendAttribute("type")->setValue("unsigned integer");
    idhuman->appendAttribute("value")->setValue(a_id_human);

    IXmlNodeShrPtr experience = parameters->appendNode("experience");
    experience->appendAttribute("type")->setValue("unsigned integer");
    experience->appendAttribute("value")->setValue(a_experience);

    IXmlNodeShrPtr volume = parameters->appendNode("volume");
    volume->appendAttribute("type")->setValue("unsigned integer");
    volume->appendAttribute("value")->setValue(a_volume);

    return request;
}

RequestShrPtr createRequestTransportResource(
    unsigned int const   a_id_user,
    string       const & a_password,
    unsigned int const   a_id_settlement_source,
    unsigned int const   a_id_settlement_destination,
    unsigned int const   a_id_resource,
    unsigned int const   a_volume
)
{
    RequestShrPtr request = make_shared<Request>();

    request->m_xml_document->appendNode("request")->appendAttribute("id")->setValue(REQUEST_ID_TRANSPORT_RESOURCE);
    IXmlNodeShrPtr parameters = request->m_xml_document->getNode("request")->appendNode("parameters");

    IXmlNodeShrPtr user_node = request->m_xml_document->getNode("request")->appendNode("user");

    user_node->appendNode("iduser")->appendAttribute("value")->setValue(a_id_user);
    user_node->appendNode("password")->appendAttribute("value")->setValue(a_password.c_str());

    IXmlNodeShrPtr idsettlementsource = parameters->appendNode("idsettlementsource");
    idsettlementsource->appendAttribute("type")->setValue("unsigned integer");
    idsettlementsource->appendAttribute("value")->setValue(a_id_settlement_source);

    IXmlNodeShrPtr idsettlementdestination = parameters->appendNode("idsettlementdestination");
    idsettlementdestination->appendAttribute("type")->setValue("unsigned integer");
    idsettlementdestination->appendAttribute("value")->setValue(a_id_settlement_destination);

    IXmlNodeShrPtr idresource = parameters->appendNode("idresource");
    idresource->appendAttribute("type")->setValue("unsigned integer");
    idresource->appendAttribute("value")->setValue(a_id_resource);

    IXmlNodeShrPtr volume = parameters->appendNode("volume");
    volume->appendAttribute("type")->setValue("unsigned integer");
    volume->appendAttribute("value")->setValue(a_volume);

    return request;
}
