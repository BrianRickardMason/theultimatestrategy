// Copyright (C) 2010, 2011 and 2012 Marcin Arkadiusz Skrobiranda.
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

#include <TUSIntegration/IntegrationCommon/Helpers/Commands/Land/LandCommands.hpp>
#include <TUSIntegration/IntegrationCommon/Helpers/RequestCreators.hpp>

using namespace Network::XmlRPCCommon::Reply;
using namespace Network::XmlRPCCommon::Request;
using namespace std;

namespace IntegrationCommon
{
namespace Helpers
{
namespace Commands
{
namespace Land
{

ReplyShrPtr CreateLand(
    IClientShrPtr       a_client,
    string        const a_login,
    string        const a_password,
    string        const a_world_name,
    string        const a_land_name
)
{
    RequestShrPtr request = createRequestCreateLand(a_login, a_password, a_world_name, a_land_name);

    return a_client->sendRequest(request);
}

ReplyShrPtr DeleteLand(
    IClientShrPtr       a_client,
    string        const a_login,
    string        const a_password,
    string        const a_land_name
)
{
    RequestShrPtr request = createRequestDeleteLand(a_login, a_password, a_land_name);

    return a_client->sendRequest(request);
}

ReplyShrPtr GetLand(
    IClientShrPtr       a_client,
    string        const a_login,
    string        const a_password,
    string        const a_land_name
)
{
    RequestShrPtr request = createRequestGetLand(a_login, a_password, a_land_name);

    return a_client->sendRequest(request);
}

ReplyShrPtr GetLands(
    IClientShrPtr       a_client,
    string        const a_login,
    string        const a_password
)
{
    RequestShrPtr request = createRequestGetLands(a_login, a_password);

    return a_client->sendRequest(request);
}

} // namespace Land
} // namespace Commands
} // namespace Helpers
} // namespace IntegrationCommon
