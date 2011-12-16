
#ifndef _MODERATORCONTEXT_H_
#define _MODERATORCONTEXT_H_

#include <memory>

#include "IModeratorContext.h"

class BotConnectionConfiguration;
class ModeratorServerConfiguration;

class ModeratorContextBuilder;

class ModeratorContext: public IModeratorContext{
public:
    virtual const IBotConnectionConfiguration& getBotConnectionConf()const;
    virtual const IModeratorServerConfiguration& getModeratorServerConf()const;
    
    void setModeratorServerConf( std::auto_ptr<ModeratorServerConfiguration> aConf );
    void setBotConnectionConf( std::auto_ptr<BotConnectionConfiguration> aConf );
private:
    friend class ModeratorContextBuilder;
    
    std::auto_ptr<ModeratorServerConfiguration> mServerConf;
    std::auto_ptr<BotConnectionConfiguration> mBotConnectionConf;
};

#endif
