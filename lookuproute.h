#ifndef __FIND__
#define __FIND__
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <arpa/inet.h>


//in_addr 一个unsigned int 用来保存ip地址

struct route
{
    struct route *next;
    struct in_addr ip4prefix;     // ipv4 address
    unsigned int prefixlen;       // mask
    struct nexthop *nexthop;
};

struct nexthop
{
   // struct nexthop *next;
   char *ifname;                  // interface name     
   unsigned int ifindex;          // interface index
   struct in_addr nexthopaddr;    // Nexthop address 
};

struct nextaddr
{
   char *ifname;
   struct in_addr ipv4addr;
   unsigned int prefixl;
};

struct route *route_table; 
int insert_route(unsigned int ip4prefix, unsigned int prefixlen, char *ifname, unsigned int ifindex, unsigned int nexthopaddr);
int lookup_route(struct in_addr dstaddr,struct nextaddr *nexthopinfo);
int delete_route(struct in_addr dstaddr,unsigned int prefixlen);

#endif
