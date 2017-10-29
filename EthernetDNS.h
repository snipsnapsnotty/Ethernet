#if !defined(__ETHERNET_DNS_H__)
#define __ETHERNET_DNS_H__ 1

extern "C" {
   #include <inttypes.h>
}

typedef uint8_t byte;

typedef enum _DNSState_t {
   DNSStateIdle,
   DNSStateQuerySent
} DNSState_t;

typedef enum _DNSError_t {
   DNSTryLater = 3,
   DNSNothingToDo = 2,
   DNSSuccess  = 1,
   DNSInvalidArgument = -1,
   DNSOutOfMemory = -2,
   DNSSocketError = -3,
   DNSAlreadyProcessingQuery = -4,
   DNSNotFound = -5,
   DNSServerError = -6,
   DNSTimedOut = -7
} DNSError_t;

typedef DNSError_t DNSError;

typedef struct _DNSDataInternal_t {
   uint8_t  serverIpAddr[4];
   uint32_t xid;
   uint32_t lastQueryFirstXid;
} DNSDataInternal_t;

class EthernetDNSClass
{
private:
   DNSDataInternal_t    _dnsData;
   int                  _socket;
   DNSState_t           _state;
   unsigned long        _lastSendMillis;
   
   DNSError_t _sendDNSQueryPacket(const char* hostName);
   
   int _startDNSSession();
   int _closeDNSSession();
public:
   EthernetDNSClass();
   ~EthernetDNSClass();
   
   void setDNSServer(const byte dnsServerIpAddr[4]);
   
   DNSError_t resolveHostName(const char* hostName, byte ipAddr[4]);
   
   DNSError_t sendDNSQuery(const char* hostName);
   DNSError_t pollDNSReply(byte ipAddr[4]);
};

extern EthernetDNSClass EthernetDNS;

#endif // __ETHERNET_DNS_H__
