#include "lookuproute.h"


int insert_route(unsigned int ip4prefix, unsigned int prefixlen, char *ifname, unsigned int ifindex, unsigned int nexthopaddr){

	struct route *new_route = (struct route *)malloc(sizeof(struct route));
	new_route->next = NULL;
	new_route->ip4prefix = *(struct in_addr *)(&ip4prefix);
	new_route->prefixlen = prefixlen;

	struct nexthop *new_hop = (struct nexthop *)malloc(sizeof(struct nexthop));
	new_hop->ifname = ifname;
	new_hop->ifindex = ifindex;
	new_hop->nexthopaddr = *(struct in_addr *)(&nexthopaddr);

	new_route->nexthop = new_hop;

	if(route_table != NULL){
		new_route->next = route_table->next;
	}
	route_table->next = new_route;
	return 0;
}

// 最长前缀匹配
int lookup_route(struct in_addr dstaddr, struct nextaddr *nexthopinfo){

	struct route *p_route = route_table->next;
	struct route *match_route = NULL;
	int max_prefix_len = 0;

	while(p_route != NULL){
		if(dstaddr.s_addr == p_route->ip4prefix.s_addr){
			if(p_route->prefixlen > max_prefix_len){
				max_prefix_len = p_route->prefixlen;
				match_route = p_route;
			}
		}
		p_route = p_route->next;
	}

	if(max_prefix_len > 0 && match_route != NULL){
			nexthopinfo->ifname = match_route->nexthop->ifname;
			nexthopinfo->ipv4addr = match_route->nexthop->nexthopaddr;
			nexthopinfo->prefixl = match_route->prefixlen;
			return 0;	
	}
	return -1;
}

int delete_route(struct in_addr dstaddr, unsigned int prefixlen){
	struct route *p_route_last = route_table;
	struct route *p_route = route_table->next;
	while(p_route != NULL){
		if(dstaddr.s_addr == p_route->ip4prefix.s_addr && prefixlen == p_route->prefixlen){
			p_route_last->next = p_route->next;

			free(p_route->nexthop);
			free(p_route);
			return 0;
		}
		p_route_last = p_route;
		p_route = p_route->next;
	}
	return -1;
}

