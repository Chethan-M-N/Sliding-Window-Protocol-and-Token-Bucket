#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Tic : public cSimpleModule
{
  private:
    int seq = 0;
        int i;
        simtime_t timeout;
        cMessage *timeoutEvent;
        cMessage *message;

  protected:
    virtual void generateNewMessage();
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Tic);

void Tic::initialize()
{
    timeout = 0.1;
    timeoutEvent = new cMessage("timeoutEvent");
    generateNewMessage();
    scheduleAt(simTime()+timeout, timeoutEvent);
}

void Tic::handleMessage(cMessage *msg)
{
    if (msg == timeoutEvent)
        {
           generateNewMessage();
           scheduleAt(simTime()+timeout, timeoutEvent);
        }
    if(seq == 10)
    {
        for(i=0; i<10; i++)
        generateNewMessage();

    }
}
void Tic::generateNewMessage()
{
    char msgname[20];
    sprintf(msgname, "tic-%d",seq);
    cMessage *msg = new cMessage(msgname);
    send(msg,"out");
    seq=seq+1;
}

class Toc : public cSimpleModule
{
  private:

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Toc);

void Toc::initialize()
{
}

void Toc::handleMessage(cMessage *msg)
{
}

class Shaper : public cSimpleModule
{
  private:
    int len;
    int seq;
    cQueue queue;
    cQueue queue1;
    simtime_t timeout;  // timeout
    cMessage *timeoutEvent;  // holds pointer to the timeout self-message
    cMessage *message;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Shaper);

void Shaper::initialize()
{
    timeout = 0.05;
    len=5;
    timeoutEvent = new cMessage("timeoutEvent");
    scheduleAt(simTime()+timeout, timeoutEvent);
}

void Shaper::handleMessage(cMessage *msg)
{
    if(msg == timeoutEvent) // self time out to generate a tocken
         {
        if(queue1.getLength()<len)
        {
            queue1.insert(msg);
        }
            timeoutEvent = new cMessage("timeoutEvent");
            scheduleAt(simTime()+timeout, timeoutEvent);
        }
        else
         {
            queue.insert(msg);
         }

        if(!queue.isEmpty())
        {
            if(!queue1.isEmpty())
            {
                msg = (cMessage *)queue.pop();
                send(msg,"out");
                queue1.pop();
            }
        }
  }


