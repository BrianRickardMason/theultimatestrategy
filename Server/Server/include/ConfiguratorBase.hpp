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

#ifndef SERVER_CONFIGURATORBASE_HPP
#define SERVER_CONFIGURATORBASE_HPP

#include <Poco/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Server/Server/include/IConfigurator.hpp>
#include <Server/Server/include/IConfiguratorBase.hpp>

namespace Server
{

class ConfiguratorBase
    : public IConfiguratorBase
{
public:
    ConfiguratorBase(
        IConfiguratorShrPtr const a_configurator
    );

    virtual bool configure();

    virtual unsigned short int getFamineDeathFactor()     const;
    virtual unsigned short int getHumanExperienceFactor() const;
    virtual unsigned short int getHumanReproduceFactor()  const;
    virtual unsigned short int getPovertyDismissFactor()  const;

private:
    bool loadXml();
    bool parseXml();

    IConfiguratorShrPtr const mConfigurator;

    Poco::AutoPtr<Poco::XML::Document> mBaseXml;

    unsigned short int mFamineDeathFactor;
    unsigned short int mHumanExperienceFactor;
    unsigned short int mHumanReproduceFactor;
    unsigned short int mPovertyDismissFactor;
};

} // namespace Server;

#endif // SERVER_CONFIGURATORBASE_HPP
