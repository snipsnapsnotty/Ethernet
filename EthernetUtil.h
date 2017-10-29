#if !defined(__ETHERNET_UTIL_H__)
#define __ETHERNET_UTIL_H__

#define __ETHERNET_UTIL_DNS__

#include <inttypes.h>

extern uint16_t ethutil_htons(unsigned short hostshort);
extern uint32_t ethutil_htonl(unsigned long hostlong);
extern uint16_t ethutil_ntohs(unsigned short netshort);
extern uint32_t ethutil_ntohl(unsigned long netlong);

#endif // __ETHERNET_UTIL_H__
