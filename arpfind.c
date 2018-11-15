#include "arpfind.h"

int arpGet(struct arpmac *srcmac, char *ifname, char *ipStr)  
{  
	struct arpreq arp_req;
	struct sockaddr_in *sin;
	sin = (struct sockaddr_in *)&(arp_req.arp_pa);
	memset(&arp_req, 0, sizeof(arp_req)); 

	sin->sin_family = AF_INET;
	sin->sin_addr.s_addr = inet_addr(ipStr);
	strncpy(arp_req.arp_dev, ifname, IF_NAMESIZE - 1);

	int arp_fd = socket(AF_INET, SOCK_DGRAM, 0);
	int ret = ioctl(arp_fd, SIOCGARP, &arp_req);

	if (ret < 0) {
		fprintf(stderr, "Get ARP entry failed for %s @%s : %s\n", ipStr, ifname, strerror(errno));
		exit(EXIT_FAILURE); 
	}

	if (arp_req.arp_flags & ATF_COM){
		//entry found
		unsigned char mac[6];
		
		memcpy(mac, (unsigned char *)arp_req.arp_ha.sa_data, 6);
		srcmac->mac = mac;
		srcmac->index = if_nametoindex(ifname);
		
		printf("Destination MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	}else{
		return -1;
	}

	close(arp_fd);
    return 0;  
}

int fromInterfaceGetMac(char *ifname, char *mac_a){
	struct ifreq ifr;
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	strncpy(ifr.ifr_name, ifname, IF_NAMESIZE);
	if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == 0) { 
		memcpy(mac_a, ifr.ifr_hwaddr.sa_data, 6); 
	} else {
		printf("from interface get mac address error\n");
	}

	close(sockfd);  
	                                                                                                        
	
}
                                                                                                          
                                                                                                            
                                                                                                              
