//
// Generated file, do not edit! Created by opp_msgc 4.5 from modules/messages/WaveShortMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "WaveShortMessage_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

Register_Class(WaveShortMessage);

WaveShortMessage::WaveShortMessage(const char *name, int kind) : ::cPacket(name,kind)
{
    this->wsmVersion_var = 0;
    this->securityType_var = 0;
    this->channelNumber_var = 0;
    this->dataRate_var = 1;
    this->priority_var = 3;
    this->psid_var = 0;
    this->psc_var = "Service with some Data";
    this->wsmLength_var = 0;
    this->wsmData_var = "Some Data";
    this->senderAddress_var = 0;
    this->recipientAddress_var = -1;
    this->serial_var = 0;
    this->timestamp_var = 0;
    this->speed_var = 0;
    this->expireTime1_var = 0;
    this->hopLimit_var = 1;
    this->flag_var = 0;
    this->originalAddress_var = -1;
    this->userPriority_var = 0;
    this->nextForwardAddress_var = 0;
    this->psid2_var = 0;
    this->flag2_var = 0;
    this->userPriority2_var = 0;
    this->sch2_var = 0;
    this->dst2_var = -1;
    this->hopLimit2_var = 1;
    this->timeStamp2_var = 0;
    this->expireTime2_var = 0;
}

WaveShortMessage::WaveShortMessage(const WaveShortMessage& other) : ::cPacket(other)
{
    copy(other);
}

WaveShortMessage::~WaveShortMessage()
{
}

WaveShortMessage& WaveShortMessage::operator=(const WaveShortMessage& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void WaveShortMessage::copy(const WaveShortMessage& other)
{
    this->wsmVersion_var = other.wsmVersion_var;
    this->securityType_var = other.securityType_var;
    this->channelNumber_var = other.channelNumber_var;
    this->dataRate_var = other.dataRate_var;
    this->priority_var = other.priority_var;
    this->psid_var = other.psid_var;
    this->psc_var = other.psc_var;
    this->wsmLength_var = other.wsmLength_var;
    this->wsmData_var = other.wsmData_var;
    this->senderAddress_var = other.senderAddress_var;
    this->recipientAddress_var = other.recipientAddress_var;
    this->serial_var = other.serial_var;
    this->senderPos_var = other.senderPos_var;
    this->timestamp_var = other.timestamp_var;
    this->direction_var = other.direction_var;
    this->speed_var = other.speed_var;
    this->expireTime1_var = other.expireTime1_var;
    for (unsigned int i=0; i<10; i++)
        this->carlist_var[i] = other.carlist_var[i];
    for (unsigned int i=0; i<10; i++)
        this->routeTable_var[i] = other.routeTable_var[i];
    this->hopLimit_var = other.hopLimit_var;
    this->flag_var = other.flag_var;
    this->originalAddress_var = other.originalAddress_var;
    this->userPriority_var = other.userPriority_var;
    this->nextForwardAddress_var = other.nextForwardAddress_var;
    this->psid2_var = other.psid2_var;
    this->flag2_var = other.flag2_var;
    this->userPriority2_var = other.userPriority2_var;
    this->sch2_var = other.sch2_var;
    this->dst2_var = other.dst2_var;
    this->hopLimit2_var = other.hopLimit2_var;
    this->timeStamp2_var = other.timeStamp2_var;
    this->expireTime2_var = other.expireTime2_var;
}

void WaveShortMessage::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->wsmVersion_var);
    doPacking(b,this->securityType_var);
    doPacking(b,this->channelNumber_var);
    doPacking(b,this->dataRate_var);
    doPacking(b,this->priority_var);
    doPacking(b,this->psid_var);
    doPacking(b,this->psc_var);
    doPacking(b,this->wsmLength_var);
    doPacking(b,this->wsmData_var);
    doPacking(b,this->senderAddress_var);
    doPacking(b,this->recipientAddress_var);
    doPacking(b,this->serial_var);
    doPacking(b,this->senderPos_var);
    doPacking(b,this->timestamp_var);
    doPacking(b,this->direction_var);
    doPacking(b,this->speed_var);
    doPacking(b,this->expireTime1_var);
    doPacking(b,this->carlist_var,10);
    doPacking(b,this->routeTable_var,10);
    doPacking(b,this->hopLimit_var);
    doPacking(b,this->flag_var);
    doPacking(b,this->originalAddress_var);
    doPacking(b,this->userPriority_var);
    doPacking(b,this->nextForwardAddress_var);
    doPacking(b,this->psid2_var);
    doPacking(b,this->flag2_var);
    doPacking(b,this->userPriority2_var);
    doPacking(b,this->sch2_var);
    doPacking(b,this->dst2_var);
    doPacking(b,this->hopLimit2_var);
    doPacking(b,this->timeStamp2_var);
    doPacking(b,this->expireTime2_var);
}

void WaveShortMessage::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->wsmVersion_var);
    doUnpacking(b,this->securityType_var);
    doUnpacking(b,this->channelNumber_var);
    doUnpacking(b,this->dataRate_var);
    doUnpacking(b,this->priority_var);
    doUnpacking(b,this->psid_var);
    doUnpacking(b,this->psc_var);
    doUnpacking(b,this->wsmLength_var);
    doUnpacking(b,this->wsmData_var);
    doUnpacking(b,this->senderAddress_var);
    doUnpacking(b,this->recipientAddress_var);
    doUnpacking(b,this->serial_var);
    doUnpacking(b,this->senderPos_var);
    doUnpacking(b,this->timestamp_var);
    doUnpacking(b,this->direction_var);
    doUnpacking(b,this->speed_var);
    doUnpacking(b,this->expireTime1_var);
    doUnpacking(b,this->carlist_var,10);
    doUnpacking(b,this->routeTable_var,10);
    doUnpacking(b,this->hopLimit_var);
    doUnpacking(b,this->flag_var);
    doUnpacking(b,this->originalAddress_var);
    doUnpacking(b,this->userPriority_var);
    doUnpacking(b,this->nextForwardAddress_var);
    doUnpacking(b,this->psid2_var);
    doUnpacking(b,this->flag2_var);
    doUnpacking(b,this->userPriority2_var);
    doUnpacking(b,this->sch2_var);
    doUnpacking(b,this->dst2_var);
    doUnpacking(b,this->hopLimit2_var);
    doUnpacking(b,this->timeStamp2_var);
    doUnpacking(b,this->expireTime2_var);
}

int WaveShortMessage::getWsmVersion() const
{
    return wsmVersion_var;
}

void WaveShortMessage::setWsmVersion(int wsmVersion)
{
    this->wsmVersion_var = wsmVersion;
}

int WaveShortMessage::getSecurityType() const
{
    return securityType_var;
}

void WaveShortMessage::setSecurityType(int securityType)
{
    this->securityType_var = securityType;
}

int WaveShortMessage::getChannelNumber() const
{
    return channelNumber_var;
}

void WaveShortMessage::setChannelNumber(int channelNumber)
{
    this->channelNumber_var = channelNumber;
}

int WaveShortMessage::getDataRate() const
{
    return dataRate_var;
}

void WaveShortMessage::setDataRate(int dataRate)
{
    this->dataRate_var = dataRate;
}

int WaveShortMessage::getPriority() const
{
    return priority_var;
}

void WaveShortMessage::setPriority(int priority)
{
    this->priority_var = priority;
}

int WaveShortMessage::getPsid() const
{
    return psid_var;
}

void WaveShortMessage::setPsid(int psid)
{
    this->psid_var = psid;
}

const char * WaveShortMessage::getPsc() const
{
    return psc_var.c_str();
}

void WaveShortMessage::setPsc(const char * psc)
{
    this->psc_var = psc;
}

int WaveShortMessage::getWsmLength() const
{
    return wsmLength_var;
}

void WaveShortMessage::setWsmLength(int wsmLength)
{
    this->wsmLength_var = wsmLength;
}

const char * WaveShortMessage::getWsmData() const
{
    return wsmData_var.c_str();
}

void WaveShortMessage::setWsmData(const char * wsmData)
{
    this->wsmData_var = wsmData;
}

int WaveShortMessage::getSenderAddress() const
{
    return senderAddress_var;
}

void WaveShortMessage::setSenderAddress(int senderAddress)
{
    this->senderAddress_var = senderAddress;
}

int WaveShortMessage::getRecipientAddress() const
{
    return recipientAddress_var;
}

void WaveShortMessage::setRecipientAddress(int recipientAddress)
{
    this->recipientAddress_var = recipientAddress;
}

int WaveShortMessage::getSerial() const
{
    return serial_var;
}

void WaveShortMessage::setSerial(int serial)
{
    this->serial_var = serial;
}

Coord& WaveShortMessage::getSenderPos()
{
    return senderPos_var;
}

void WaveShortMessage::setSenderPos(const Coord& senderPos)
{
    this->senderPos_var = senderPos;
}

simtime_t WaveShortMessage::getTimestamp() const
{
    return timestamp_var;
}

void WaveShortMessage::setTimestamp(simtime_t timestamp)
{
    this->timestamp_var = timestamp;
}

Coord& WaveShortMessage::getDirection()
{
    return direction_var;
}

void WaveShortMessage::setDirection(const Coord& direction)
{
    this->direction_var = direction;
}

double WaveShortMessage::getSpeed() const
{
    return speed_var;
}

void WaveShortMessage::setSpeed(double speed)
{
    this->speed_var = speed;
}

double WaveShortMessage::getExpireTime1() const
{
    return expireTime1_var;
}

void WaveShortMessage::setExpireTime1(double expireTime1)
{
    this->expireTime1_var = expireTime1;
}

unsigned int WaveShortMessage::getCarlistArraySize() const
{
    return 10;
}

CarMove& WaveShortMessage::getCarlist(unsigned int k)
{
    if (k>=10) throw cRuntimeError("Array of size 10 indexed by %lu", (unsigned long)k);
    return carlist_var[k];
}

void WaveShortMessage::setCarlist(unsigned int k, const CarMove& carlist)
{
    if (k>=10) throw cRuntimeError("Array of size 10 indexed by %lu", (unsigned long)k);
    this->carlist_var[k] = carlist;
}

unsigned int WaveShortMessage::getRouteTableArraySize() const
{
    return 10;
}

RouteEntry& WaveShortMessage::getRouteTable(unsigned int k)
{
    if (k>=10) throw cRuntimeError("Array of size 10 indexed by %lu", (unsigned long)k);
    return routeTable_var[k];
}

void WaveShortMessage::setRouteTable(unsigned int k, const RouteEntry& routeTable)
{
    if (k>=10) throw cRuntimeError("Array of size 10 indexed by %lu", (unsigned long)k);
    this->routeTable_var[k] = routeTable;
}

int WaveShortMessage::getHopLimit() const
{
    return hopLimit_var;
}

void WaveShortMessage::setHopLimit(int hopLimit)
{
    this->hopLimit_var = hopLimit;
}

int WaveShortMessage::getFlag() const
{
    return flag_var;
}

void WaveShortMessage::setFlag(int flag)
{
    this->flag_var = flag;
}

int WaveShortMessage::getOriginalAddress() const
{
    return originalAddress_var;
}

void WaveShortMessage::setOriginalAddress(int originalAddress)
{
    this->originalAddress_var = originalAddress;
}

int WaveShortMessage::getUserPriority() const
{
    return userPriority_var;
}

void WaveShortMessage::setUserPriority(int userPriority)
{
    this->userPriority_var = userPriority;
}

int WaveShortMessage::getNextForwardAddress() const
{
    return nextForwardAddress_var;
}

void WaveShortMessage::setNextForwardAddress(int nextForwardAddress)
{
    this->nextForwardAddress_var = nextForwardAddress;
}

int WaveShortMessage::getPsid2() const
{
    return psid2_var;
}

void WaveShortMessage::setPsid2(int psid2)
{
    this->psid2_var = psid2;
}

int WaveShortMessage::getFlag2() const
{
    return flag2_var;
}

void WaveShortMessage::setFlag2(int flag2)
{
    this->flag2_var = flag2;
}

int WaveShortMessage::getUserPriority2() const
{
    return userPriority2_var;
}

void WaveShortMessage::setUserPriority2(int userPriority2)
{
    this->userPriority2_var = userPriority2;
}

int WaveShortMessage::getSch2() const
{
    return sch2_var;
}

void WaveShortMessage::setSch2(int sch2)
{
    this->sch2_var = sch2;
}

int WaveShortMessage::getDst2() const
{
    return dst2_var;
}

void WaveShortMessage::setDst2(int dst2)
{
    this->dst2_var = dst2;
}

int WaveShortMessage::getHopLimit2() const
{
    return hopLimit2_var;
}

void WaveShortMessage::setHopLimit2(int hopLimit2)
{
    this->hopLimit2_var = hopLimit2;
}

simtime_t WaveShortMessage::getTimeStamp2() const
{
    return timeStamp2_var;
}

void WaveShortMessage::setTimeStamp2(simtime_t timeStamp2)
{
    this->timeStamp2_var = timeStamp2;
}

double WaveShortMessage::getExpireTime2() const
{
    return expireTime2_var;
}

void WaveShortMessage::setExpireTime2(double expireTime2)
{
    this->expireTime2_var = expireTime2;
}

class WaveShortMessageDescriptor : public cClassDescriptor
{
  public:
    WaveShortMessageDescriptor();
    virtual ~WaveShortMessageDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(WaveShortMessageDescriptor);

WaveShortMessageDescriptor::WaveShortMessageDescriptor() : cClassDescriptor("WaveShortMessage", "cPacket")
{
}

WaveShortMessageDescriptor::~WaveShortMessageDescriptor()
{
}

bool WaveShortMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<WaveShortMessage *>(obj)!=NULL;
}

const char *WaveShortMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int WaveShortMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 32+basedesc->getFieldCount(object) : 32;
}

unsigned int WaveShortMessageDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISCOMPOUND,
        FD_ISARRAY | FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<32) ? fieldTypeFlags[field] : 0;
}

const char *WaveShortMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "wsmVersion",
        "securityType",
        "channelNumber",
        "dataRate",
        "priority",
        "psid",
        "psc",
        "wsmLength",
        "wsmData",
        "senderAddress",
        "recipientAddress",
        "serial",
        "senderPos",
        "timestamp",
        "direction",
        "speed",
        "expireTime1",
        "carlist",
        "routeTable",
        "hopLimit",
        "flag",
        "originalAddress",
        "userPriority",
        "nextForwardAddress",
        "psid2",
        "flag2",
        "userPriority2",
        "sch2",
        "dst2",
        "hopLimit2",
        "timeStamp2",
        "expireTime2",
    };
    return (field>=0 && field<32) ? fieldNames[field] : NULL;
}

int WaveShortMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='w' && strcmp(fieldName, "wsmVersion")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "securityType")==0) return base+1;
    if (fieldName[0]=='c' && strcmp(fieldName, "channelNumber")==0) return base+2;
    if (fieldName[0]=='d' && strcmp(fieldName, "dataRate")==0) return base+3;
    if (fieldName[0]=='p' && strcmp(fieldName, "priority")==0) return base+4;
    if (fieldName[0]=='p' && strcmp(fieldName, "psid")==0) return base+5;
    if (fieldName[0]=='p' && strcmp(fieldName, "psc")==0) return base+6;
    if (fieldName[0]=='w' && strcmp(fieldName, "wsmLength")==0) return base+7;
    if (fieldName[0]=='w' && strcmp(fieldName, "wsmData")==0) return base+8;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderAddress")==0) return base+9;
    if (fieldName[0]=='r' && strcmp(fieldName, "recipientAddress")==0) return base+10;
    if (fieldName[0]=='s' && strcmp(fieldName, "serial")==0) return base+11;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderPos")==0) return base+12;
    if (fieldName[0]=='t' && strcmp(fieldName, "timestamp")==0) return base+13;
    if (fieldName[0]=='d' && strcmp(fieldName, "direction")==0) return base+14;
    if (fieldName[0]=='s' && strcmp(fieldName, "speed")==0) return base+15;
    if (fieldName[0]=='e' && strcmp(fieldName, "expireTime1")==0) return base+16;
    if (fieldName[0]=='c' && strcmp(fieldName, "carlist")==0) return base+17;
    if (fieldName[0]=='r' && strcmp(fieldName, "routeTable")==0) return base+18;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopLimit")==0) return base+19;
    if (fieldName[0]=='f' && strcmp(fieldName, "flag")==0) return base+20;
    if (fieldName[0]=='o' && strcmp(fieldName, "originalAddress")==0) return base+21;
    if (fieldName[0]=='u' && strcmp(fieldName, "userPriority")==0) return base+22;
    if (fieldName[0]=='n' && strcmp(fieldName, "nextForwardAddress")==0) return base+23;
    if (fieldName[0]=='p' && strcmp(fieldName, "psid2")==0) return base+24;
    if (fieldName[0]=='f' && strcmp(fieldName, "flag2")==0) return base+25;
    if (fieldName[0]=='u' && strcmp(fieldName, "userPriority2")==0) return base+26;
    if (fieldName[0]=='s' && strcmp(fieldName, "sch2")==0) return base+27;
    if (fieldName[0]=='d' && strcmp(fieldName, "dst2")==0) return base+28;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopLimit2")==0) return base+29;
    if (fieldName[0]=='t' && strcmp(fieldName, "timeStamp2")==0) return base+30;
    if (fieldName[0]=='e' && strcmp(fieldName, "expireTime2")==0) return base+31;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *WaveShortMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "string",
        "int",
        "string",
        "int",
        "int",
        "int",
        "Coord",
        "simtime_t",
        "Coord",
        "double",
        "double",
        "CarMove",
        "RouteEntry",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "simtime_t",
        "double",
    };
    return (field>=0 && field<32) ? fieldTypeStrings[field] : NULL;
}

const char *WaveShortMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int WaveShortMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    WaveShortMessage *pp = (WaveShortMessage *)object; (void)pp;
    switch (field) {
        case 17: return 10;
        case 18: return 10;
        default: return 0;
    }
}

std::string WaveShortMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    WaveShortMessage *pp = (WaveShortMessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getWsmVersion());
        case 1: return long2string(pp->getSecurityType());
        case 2: return long2string(pp->getChannelNumber());
        case 3: return long2string(pp->getDataRate());
        case 4: return long2string(pp->getPriority());
        case 5: return long2string(pp->getPsid());
        case 6: return oppstring2string(pp->getPsc());
        case 7: return long2string(pp->getWsmLength());
        case 8: return oppstring2string(pp->getWsmData());
        case 9: return long2string(pp->getSenderAddress());
        case 10: return long2string(pp->getRecipientAddress());
        case 11: return long2string(pp->getSerial());
        case 12: {std::stringstream out; out << pp->getSenderPos(); return out.str();}
        case 13: return double2string(pp->getTimestamp());
        case 14: {std::stringstream out; out << pp->getDirection(); return out.str();}
        case 15: return double2string(pp->getSpeed());
        case 16: return double2string(pp->getExpireTime1());
        case 17: {std::stringstream out; out << pp->getCarlist(i); return out.str();}
        case 18: {std::stringstream out; out << pp->getRouteTable(i); return out.str();}
        case 19: return long2string(pp->getHopLimit());
        case 20: return long2string(pp->getFlag());
        case 21: return long2string(pp->getOriginalAddress());
        case 22: return long2string(pp->getUserPriority());
        case 23: return long2string(pp->getNextForwardAddress());
        case 24: return long2string(pp->getPsid2());
        case 25: return long2string(pp->getFlag2());
        case 26: return long2string(pp->getUserPriority2());
        case 27: return long2string(pp->getSch2());
        case 28: return long2string(pp->getDst2());
        case 29: return long2string(pp->getHopLimit2());
        case 30: return double2string(pp->getTimeStamp2());
        case 31: return double2string(pp->getExpireTime2());
        default: return "";
    }
}

bool WaveShortMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    WaveShortMessage *pp = (WaveShortMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setWsmVersion(string2long(value)); return true;
        case 1: pp->setSecurityType(string2long(value)); return true;
        case 2: pp->setChannelNumber(string2long(value)); return true;
        case 3: pp->setDataRate(string2long(value)); return true;
        case 4: pp->setPriority(string2long(value)); return true;
        case 5: pp->setPsid(string2long(value)); return true;
        case 6: pp->setPsc((value)); return true;
        case 7: pp->setWsmLength(string2long(value)); return true;
        case 8: pp->setWsmData((value)); return true;
        case 9: pp->setSenderAddress(string2long(value)); return true;
        case 10: pp->setRecipientAddress(string2long(value)); return true;
        case 11: pp->setSerial(string2long(value)); return true;
        case 13: pp->setTimestamp(string2double(value)); return true;
        case 15: pp->setSpeed(string2double(value)); return true;
        case 16: pp->setExpireTime1(string2double(value)); return true;
        case 19: pp->setHopLimit(string2long(value)); return true;
        case 20: pp->setFlag(string2long(value)); return true;
        case 21: pp->setOriginalAddress(string2long(value)); return true;
        case 22: pp->setUserPriority(string2long(value)); return true;
        case 23: pp->setNextForwardAddress(string2long(value)); return true;
        case 24: pp->setPsid2(string2long(value)); return true;
        case 25: pp->setFlag2(string2long(value)); return true;
        case 26: pp->setUserPriority2(string2long(value)); return true;
        case 27: pp->setSch2(string2long(value)); return true;
        case 28: pp->setDst2(string2long(value)); return true;
        case 29: pp->setHopLimit2(string2long(value)); return true;
        case 30: pp->setTimeStamp2(string2double(value)); return true;
        case 31: pp->setExpireTime2(string2double(value)); return true;
        default: return false;
    }
}

const char *WaveShortMessageDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 12: return opp_typename(typeid(Coord));
        case 14: return opp_typename(typeid(Coord));
        case 17: return opp_typename(typeid(CarMove));
        case 18: return opp_typename(typeid(RouteEntry));
        default: return NULL;
    };
}

void *WaveShortMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    WaveShortMessage *pp = (WaveShortMessage *)object; (void)pp;
    switch (field) {
        case 12: return (void *)(&pp->getSenderPos()); break;
        case 14: return (void *)(&pp->getDirection()); break;
        case 17: return (void *)(&pp->getCarlist(i)); break;
        case 18: return (void *)(&pp->getRouteTable(i)); break;
        default: return NULL;
    }
}


