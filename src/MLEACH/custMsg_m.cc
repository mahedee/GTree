//
// Generated file, do not edit! Created by opp_msgc 4.5 from custMsg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "custMsg_m.h"

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

Register_Class(custMsg);

custMsg::custMsg(const char *name, int kind) : ::cPacket(name,kind)
{
    this->sourceId_var = 0;
    this->messageId_var = 0;
    this->ackMsgId_var = 0;
    this->senderId_var = 0;
    this->intendedReceiverId_var = 0;
    this->packetGenerateTime_var = 0;
    this->packetReachToSinkTime_var = 0;
    this->isRelayMsg_var = 0;
    this->packetReachTime_var = 0;
    this->overheadDelay_var = 0;
}

custMsg::custMsg(const custMsg& other) : ::cPacket(other)
{
    copy(other);
}

custMsg::~custMsg()
{
}

custMsg& custMsg::operator=(const custMsg& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void custMsg::copy(const custMsg& other)
{
    this->sourceId_var = other.sourceId_var;
    this->messageId_var = other.messageId_var;
    this->ackMsgId_var = other.ackMsgId_var;
    this->senderId_var = other.senderId_var;
    this->intendedReceiverId_var = other.intendedReceiverId_var;
    this->packetGenerateTime_var = other.packetGenerateTime_var;
    this->packetReachToSinkTime_var = other.packetReachToSinkTime_var;
    this->isRelayMsg_var = other.isRelayMsg_var;
    this->packetReachTime_var = other.packetReachTime_var;
    this->overheadDelay_var = other.overheadDelay_var;
}

void custMsg::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->sourceId_var);
    doPacking(b,this->messageId_var);
    doPacking(b,this->ackMsgId_var);
    doPacking(b,this->senderId_var);
    doPacking(b,this->intendedReceiverId_var);
    doPacking(b,this->packetGenerateTime_var);
    doPacking(b,this->packetReachToSinkTime_var);
    doPacking(b,this->isRelayMsg_var);
    doPacking(b,this->packetReachTime_var);
    doPacking(b,this->overheadDelay_var);
}

void custMsg::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->sourceId_var);
    doUnpacking(b,this->messageId_var);
    doUnpacking(b,this->ackMsgId_var);
    doUnpacking(b,this->senderId_var);
    doUnpacking(b,this->intendedReceiverId_var);
    doUnpacking(b,this->packetGenerateTime_var);
    doUnpacking(b,this->packetReachToSinkTime_var);
    doUnpacking(b,this->isRelayMsg_var);
    doUnpacking(b,this->packetReachTime_var);
    doUnpacking(b,this->overheadDelay_var);
}

int custMsg::getSourceId() const
{
    return sourceId_var;
}

void custMsg::setSourceId(int sourceId)
{
    this->sourceId_var = sourceId;
}

int custMsg::getMessageId() const
{
    return messageId_var;
}

void custMsg::setMessageId(int messageId)
{
    this->messageId_var = messageId;
}

int custMsg::getAckMsgId() const
{
    return ackMsgId_var;
}

void custMsg::setAckMsgId(int ackMsgId)
{
    this->ackMsgId_var = ackMsgId;
}

int custMsg::getSenderId() const
{
    return senderId_var;
}

void custMsg::setSenderId(int senderId)
{
    this->senderId_var = senderId;
}

int custMsg::getIntendedReceiverId() const
{
    return intendedReceiverId_var;
}

void custMsg::setIntendedReceiverId(int intendedReceiverId)
{
    this->intendedReceiverId_var = intendedReceiverId;
}

double custMsg::getPacketGenerateTime() const
{
    return packetGenerateTime_var;
}

void custMsg::setPacketGenerateTime(double packetGenerateTime)
{
    this->packetGenerateTime_var = packetGenerateTime;
}

double custMsg::getPacketReachToSinkTime() const
{
    return packetReachToSinkTime_var;
}

void custMsg::setPacketReachToSinkTime(double packetReachToSinkTime)
{
    this->packetReachToSinkTime_var = packetReachToSinkTime;
}

bool custMsg::getIsRelayMsg() const
{
    return isRelayMsg_var;
}

void custMsg::setIsRelayMsg(bool isRelayMsg)
{
    this->isRelayMsg_var = isRelayMsg;
}

double custMsg::getPacketReachTime() const
{
    return packetReachTime_var;
}

void custMsg::setPacketReachTime(double packetReachTime)
{
    this->packetReachTime_var = packetReachTime;
}

double custMsg::getOverheadDelay() const
{
    return overheadDelay_var;
}

void custMsg::setOverheadDelay(double overheadDelay)
{
    this->overheadDelay_var = overheadDelay;
}

class custMsgDescriptor : public cClassDescriptor
{
  public:
    custMsgDescriptor();
    virtual ~custMsgDescriptor();

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

Register_ClassDescriptor(custMsgDescriptor);

custMsgDescriptor::custMsgDescriptor() : cClassDescriptor("custMsg", "cPacket")
{
}

custMsgDescriptor::~custMsgDescriptor()
{
}

bool custMsgDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<custMsg *>(obj)!=NULL;
}

const char *custMsgDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int custMsgDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 10+basedesc->getFieldCount(object) : 10;
}

unsigned int custMsgDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<10) ? fieldTypeFlags[field] : 0;
}

const char *custMsgDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "sourceId",
        "messageId",
        "ackMsgId",
        "senderId",
        "intendedReceiverId",
        "packetGenerateTime",
        "packetReachToSinkTime",
        "isRelayMsg",
        "packetReachTime",
        "overheadDelay",
    };
    return (field>=0 && field<10) ? fieldNames[field] : NULL;
}

int custMsgDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceId")==0) return base+0;
    if (fieldName[0]=='m' && strcmp(fieldName, "messageId")==0) return base+1;
    if (fieldName[0]=='a' && strcmp(fieldName, "ackMsgId")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderId")==0) return base+3;
    if (fieldName[0]=='i' && strcmp(fieldName, "intendedReceiverId")==0) return base+4;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetGenerateTime")==0) return base+5;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetReachToSinkTime")==0) return base+6;
    if (fieldName[0]=='i' && strcmp(fieldName, "isRelayMsg")==0) return base+7;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetReachTime")==0) return base+8;
    if (fieldName[0]=='o' && strcmp(fieldName, "overheadDelay")==0) return base+9;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *custMsgDescriptor::getFieldTypeString(void *object, int field) const
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
        "double",
        "double",
        "bool",
        "double",
        "double",
    };
    return (field>=0 && field<10) ? fieldTypeStrings[field] : NULL;
}

const char *custMsgDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int custMsgDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    custMsg *pp = (custMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string custMsgDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    custMsg *pp = (custMsg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSourceId());
        case 1: return long2string(pp->getMessageId());
        case 2: return long2string(pp->getAckMsgId());
        case 3: return long2string(pp->getSenderId());
        case 4: return long2string(pp->getIntendedReceiverId());
        case 5: return double2string(pp->getPacketGenerateTime());
        case 6: return double2string(pp->getPacketReachToSinkTime());
        case 7: return bool2string(pp->getIsRelayMsg());
        case 8: return double2string(pp->getPacketReachTime());
        case 9: return double2string(pp->getOverheadDelay());
        default: return "";
    }
}

bool custMsgDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    custMsg *pp = (custMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setSourceId(string2long(value)); return true;
        case 1: pp->setMessageId(string2long(value)); return true;
        case 2: pp->setAckMsgId(string2long(value)); return true;
        case 3: pp->setSenderId(string2long(value)); return true;
        case 4: pp->setIntendedReceiverId(string2long(value)); return true;
        case 5: pp->setPacketGenerateTime(string2double(value)); return true;
        case 6: pp->setPacketReachToSinkTime(string2double(value)); return true;
        case 7: pp->setIsRelayMsg(string2bool(value)); return true;
        case 8: pp->setPacketReachTime(string2double(value)); return true;
        case 9: pp->setOverheadDelay(string2double(value)); return true;
        default: return false;
    }
}

const char *custMsgDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *custMsgDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    custMsg *pp = (custMsg *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


