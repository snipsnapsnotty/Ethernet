#include <utility/EthernetCompat.h>

#if defined(__ETHERNET_COMPAT_DNS__)

#if defined(ARDUINO) && ARDUINO > 18   // Arduino 0019 or later

#include <utility/socket.h>
#include <utility/w5100.h>
#include <arduino.h>

#define TXBUF_BASE      0x4000
#define SMASK           0x07FF

const uint8_t ECSockClosed       = SnSR::CLOSED;
const uint8_t ECSnCrSockSend     = Sock_SEND;
const uint8_t ECSnCrSockRecv     = Sock_RECV;
const uint8_t ECSnMrUDP          = SnMR::UDP;
const uint8_t ECSnMrMulticast    = SnMR::MULTI;

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  inline static void initSS()    { DDRB  |=  _BV(4); };
  inline static void setSS()     { PORTB &= ~_BV(4); };
  inline static void resetSS()   { PORTB |=  _BV(4); };
#else
  inline static void initSS()    { DDRB  |=  _BV(2); };
  inline static void setSS()     { PORTB &= ~_BV(2); };
  inline static void resetSS()   { PORTB |=  _BV(2); };
#endif

uint16_t ethernet_compat_write_private(uint16_t _addr, uint8_t *_buf, uint16_t _len)
{
   for (int i=0; i<_len; i++) {
      setSS();    
      SPI.transfer(0xF0);
      SPI.transfer(_addr >> 8);
      SPI.transfer(_addr & 0xFF);
      _addr++;
      SPI.transfer(_buf[i]);
      resetSS();
   }
   return _len;
}

void ethernet_compat_init(uint8_t* macAddr, uint8_t* ipAddr, uint16_t rxtx_bufsize)
{
   W5100.init();
	W5100.setMACAddress(macAddr);
	W5100.setIPAddress(ipAddr);
}

uint8_t ethernet_compat_socket(int s, uint8_t proto, uint16_t port, uint8_t flag)
{
   return socket(s, proto, port, flag);
}

void ethernet_compat_close(int s)
{
   close(s);
}

uint16_t ethernet_compat_read_SnTX_WR(int socket)
{
   return W5100.readSnTX_WR(socket);
}

void ethernet_compat_write_data(int socket, uint8_t* src, uint8_t* dst, uint16_t len)
{
   uint16_t size;
   uint16_t dst_mask;
   uint16_t dst_ptr, dst_ptr_base;

   dst_mask = (uint16_t)dst & SMASK;
   dst_ptr_base = TXBUF_BASE + socket * W5100Class::SSIZE;
   dst_ptr = dst_ptr_base + dst_mask;

   if( (dst_mask + len) > W5100Class::SSIZE ) 
   {
 	size = W5100Class::SSIZE - dst_mask;
     ethernet_compat_write_private(dst_ptr, (uint8_t *) src, size);
     src += size;
 	  ethernet_compat_write_private(dst_ptr_base, (uint8_t *) src, len - size);
   } 
   else
     ethernet_compat_write_private(dst_ptr, (uint8_t *) src, len);
}

uint16_t ethernet_compat_read_SnRX_RSR(int socket)
{
   return W5100.readSnRX_RSR(socket);
}

uint16_t ethernet_compat_read_SnRX_RD(int socket)
{
   return W5100.readSnRX_RD(socket);
}

void ethernet_compat_read_data(int socket, uint16_t src, uint16_t dst, uint16_t len)
{
   W5100.read_data(socket, src, (volatile uint8_t *)dst, len);
}

uint8_t ethernet_compat_read_SnSr(int socket)
{
   return W5100.readSnSR(socket);
}

uint8_t ethernet_compat_read_SnCR(int socket)
{
   return W5100.readSnCR(socket);
}

void ethernet_compat_write_DHAR(int socket, uint8_t* macAddr)
{
   W5100.writeSnDHAR(socket, macAddr);
}

void ethernet_compat_write_SnDIPR(int socket, uint8_t* serverIpAddr)
{
   W5100.writeSnDIPR(socket, serverIpAddr);
}

void ethernet_compat_write_SnDPORT(int socket, uint16_t port)
{
   W5100.writeSnDPORT(socket, port);
}

void ethernet_compat_write_SnTX_WR(int socket, uint16_t ptr)
{
   W5100.writeSnTX_WR(socket, ptr);
}

void ethernet_compat_write_SnCR(int socket, uint8_t cmd)
{
   W5100.writeSnCR(socket, cmd);
}

void ethernet_compat_write_SnRX_RD(int socket, uint16_t ptr)
{
   W5100.writeSnRX_RD(socket, ptr);
}

void ethernet_compat_read_SIPR(uint8_t* dst)
{
   W5100.readSIPR(dst);
}
