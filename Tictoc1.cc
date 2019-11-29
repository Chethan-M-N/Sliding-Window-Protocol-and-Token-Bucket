

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;

class Tic1 : public cSimpleModule
{
  private:
    int temp1;
    int i;
    simtime_t timeout;  // timeout
    cMessage *timeoutEvent;  // holds pointer to the timeout self-message
    int seq = 0;
    cMessage *message;  // message that has to be re-sent on timeout

 public:
    Tic1();
    virtual ~Tic1();

  protected:
    virtual void generateNewMessage();
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Tic1);

Tic1::Tic1()
{
    timeoutEvent = message = nullptr;
}
Tic1::~Tic1()
{
    cancelAndDelete(timeoutEvent);
    delete message;
}

void Tic1::initialize()
{
    timeout = 0.4;
    timeoutEvent = new cMessage("timeoutEvent");
    EV << "Sending initial message form tic \n";
    WATCH(seq);
    generateNewMessage();
    scheduleAt(simTime()+timeout, timeoutEvent);
}

void Tic1::handleMessage(cMessage *msg)
{
   if (msg == timeoutEvent)
        {
           EV << "Timeout expired, resending message and restarting timer\n";
           seq = seq - temp1;
           generateNewMessage();
           scheduleAt(simTime()+timeout, timeoutEvent);
        }
   else
       {
           temp1 = msg->par("WINSIZE").longValue();

           EV << "Received by handle message of tic : " << msg -> getName() << endl;
           delete msg;
           EV << "Timer cancelled.\n";
           cancelEvent(timeoutEvent);
           generateNewMessage();
           scheduleAt(simTime()+timeout, timeoutEvent);
       }
}

void Tic1::generateNewMessage()
{
    if(seq == 0)
    {
        int tempSeq = seq;
        char msgname[20];
        sprintf(msgname, "tic-%d\n", tempSeq);//tempSeq is not required it is optional
        cMessage *msg = new cMessage(msgname);
        msg->addPar("SEQ");
        msg->par("SEQ").setLongValue(tempSeq);
        EV <<"'Inside if loop of generate New Message tic side, SEQ value = " << msg->par("SEQ").longValue() << "\n";
        seq = seq + 1;
        //seq =seq%256;
        send(msg,"out");
    }
    else
    {
        for(i=0; i< temp1; i++)
        {
            char msgname[20];
            sprintf(msgname, "tic-%d\n", seq);
            cMessage *msg = new cMessage(msgname);
            msg->addPar("SEQ");
            msg->par("SEQ").setLongValue(seq);
            send(msg,"out");
            seq = seq + 1;
            //seq=seq%24;
        }
    }
}

class Toc1 : public cSimpleModule
{

  private:
    int windowsize;
    int temp;
    int counter = 0;
    cMessage *message;

  protected:
    virtual void handleMessage(cMessage *msg) override;
    virtual void initialize() override;
    virtual void generateNewMessage();
};

Define_Module(Toc1);

void Toc1::initialize()
{
    windowsize = par("window");
}

void Toc1::handleMessage(cMessage *msg)
{
   if (uniform(0, 1) < 0.1)
      {
            EV << "\"Losing\" message " << msg << endl;
            bubble("message lost");
            counter=0;
            delete msg;
      }
  else
      {
        temp = msg-> par("SEQ").longValue();
        EV << msg << " Received by toc handle message, Sending back an acknowledgment\n";
        delete msg;
        generateNewMessage();
      }
}
void Toc1::generateNewMessage()
{
if(temp == 0)
    {
        char msgname[20];
        sprintf(msgname,"toc window size is - %d\n",windowsize);
        cMessage *msg = new cMessage(msgname);
        msg->addPar("WINSIZE");
        msg->par("WINSIZE").setLongValue(windowsize);
        EV << "'Window size in toc is = " << msg->par("WINSIZE").longValue() << endl;
        send(msg, "out");
    }
else
    {
        if(counter == windowsize-1)
        {
            char msgname[20];
            sprintf(msgname,"toc ack successfully - %d message\n",windowsize);
            cMessage *msg = new cMessage(msgname);
            msg->addPar("WINSIZE");
            msg->par("WINSIZE").setLongValue(windowsize);
            send(msg, "out");
            counter = 0;
        }
        else
            if(temp%windowsize == 0)
            counter = 0;
            else
                counter++;
    }
}










