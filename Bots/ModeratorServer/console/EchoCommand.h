
#ifndef _ECHOCOMMAND_H_
#define _ECHOCOMMAND_H_

// needed for implementing ICommand and ICommandCreator
#include "include/ICommand.h"
#include "include/ACommand.h"
#include "include/ICommandCreator.h"

// needed by the EchoCommand itself
#include "Console.h"

/**
 * Command that echoes the parameters on the console
 */
class EchoCommand: public ACommand {
public:
    EchoCommand(Console& aConsole,const ICommand::Params& aParams)
        : ACommand(aParams), mConsole(aConsole){}
    
    virtual int execute()const{
        if(mParameters.count() == 2){
            mConsole.echo(mParameters[1]);
        } else {
            mConsole.echo(mParameters.getParameterString());
        }
        return 0;
    }
    
private:
    Console& mConsole;
};


class EchoCommandCreator: public ICommandCreator {
public:
    EchoCommandCreator(Console& aConsole): mConsole(aConsole){}
    
    virtual ICommand::SingleHandle create(const ICommand::Params& aParams){
        return ICommand::SingleHandle( new EchoCommand(mConsole, aParams) );
    }

    virtual std::string getDescription()const{
        return "echoes the parameters on the console";
    }
private:
    Console& mConsole;
};

#endif
