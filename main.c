//#include "analyseip.h"
#include "checksum.h"
#include "lookuproute.h"
#include "arpfind.h"
#include "sendetherip.h"
#include "recvroute.h"
#include <pthread.h>

#define IP_HEADER_LEN sizeof(struct ip)
#define ETHER_HEADER_LEN sizeof(struct ether_header)


//接收路由信息的线程
void *thr_fn(void *arg)
{
	int st = 0;
	struct selfroute *selfrt; 
	selfrt = (struct selfroute*)malloc(sizeof(struct selfroute));
	memset(selfrt, 0, sizeof(struct selfroute));

	//get if.name
	struct if_nameindex *head, *ifni;
	ifni = if_nameindex();
  	head = ifni;
	char *ifname;

	// add-24 del-25
	while(1)
	{
		st = static_route_get(selfrt);
		if(st == 0)
		{
			if(selfrt->cmdnum == 24)
			{
				while(ifni->if_index != 0) {
					if(ifni->if_index==selfrt->ifindex)
					{
						printf("if_name is %s\n", ifni->if_name);
							ifname= ifni->if_name;
						break;
					}
					ifni++;
				}

				{
					//插入到路由表里
				}
			}
			else if(selfrt->cmdnum == 25)
			{
				//从路由表里删除路由
			}
		}

	}

}

int main()	
{
	char skbuf[1514];
	char data[1480];
	int recvfd,datalen;
	int recvlen;		
	pthread_t tid;

	//创建raw socket套接字
	if((recvfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP))) == -1)	
	{
		printf("recvfd() error\n");
		return -1;
	}	
	
	//路由表初始化
	route_table = (struct route*)malloc(sizeof(struct route));
	if(route_table == NULL){
			printf("malloc error!!\n");
			return -1;
	}
	memset(route_table, 0, sizeof(struct route));


	{
		
	//调用添加函数insert_route往路由表里添加直连路由
		insert_route(inet_addr("192.168.1.4"), 24, "eth0", if_nametoindex("eth0"), inet_addr("192.168.3.1"));
	
	}

	//创建线程去接收路由信息
	int pd;
	pd = pthread_create(&tid, NULL, thr_fn, NULL);


	while(1)
	{
		//接收ip数据包模块
		recvlen = recv(recvfd, skbuf, sizeof(skbuf), 0);
		//接受ether网头


		if(recvlen > 0)
		{
			struct ether_header *eth_header = (struct ether_header *)skbuf;
			struct ip *ip_recv_header = (struct ip *)(skbuf + sizeof(struct ether_header));
					
			//192.168.1.10是测试服务器的IP，现在测试服务器IP是192.168.1.10到192.168.1.80.
			//使用不同的测试服务器要进行修改对应的IP。然后再编译。
			//192.168.6.2是测试时候ping的目的地址。与静态路由相对应。
 			if(ip_recv_header->ip_src.s_addr == inet_addr("192.168.1.10") && ip_recv_header->ip_dst.s_addr == inet_addr("192.168.6.2") )
			{
				//分析打印ip数据包的源和目的ip地址
			//	analyseIP(ip_recvpkt);

				memset(data, 0, 1480);			
				int s;
				for(s=0; s<1480; s++)
				{
					data[s]=skbuf[s+34];
				}

				 
					// 校验计算模块
					// struct _iphdr *iphead;

					// iphead=(struct _iphdr *)malloc(sizeof(struct _iphdr));
					
					

					//调用校验函数check_sum，成功返回0
					int c = check_sum((unsigned short *)ip_recv_header, ip_recv_header->ip_len * 4);

				
					if(c == 0){
							printf("checksum is ok!!\n");
					}else{
							printf("checksum is error !!\n");
							return -1;
					}


					//查找路由表，获取下一跳ip地址和出接口模块
					struct nextaddr *nexthopinfo;
					nexthopinfo = (struct nextaddr *)malloc(sizeof(struct nextaddr));
					memset(nexthopinfo, 0, sizeof(struct nextaddr));
					
					//调用查找路由函数lookup_route，获取下一跳ip地址和出接口
					lookup_route(ip_recv_header->ip_dst, nexthopinfo);
					

					
					//arp find
					struct arpmac *srcmac;
					srcmac = (struct arpmac*)malloc(sizeof(struct arpmac));
					memset(srcmac, 0, sizeof(struct arpmac));
					
					//调用arpGet获取下一跳的mac地址	
					arpGet(srcmac, nexthopinfo->ifname, "192.168.3.1");
					

					// send ether icmp
					
					// 调用ip_transmit函数   填充数据包，通过原始套接字从查表得到的出接口(比如网卡2)将数据包发送出去
					// 将获取到的下一跳接口信息存储到存储接口信息的结构体ifreq里，通过ioctl获取出接口的mac地址作为数据包的源mac地址
					// 封装数据包：
					// <1>.根据获取到的信息填充以太网数据包头，以太网包头主要需要源mac地址、目的mac地址、以太网类型eth_header->ether_type = htons(ETHERTYPE_IP);
					// <2>.再填充ip数据包头，对其进行校验处理；
					// <3>.然后再填充接收到的ip数据包剩余数据部分，然后通过raw socket发送出去

				//修改以太网包头
					// ether_header->ether_dhost = ;
					// ether_header->ether_shost = ;
				// 修改ip包头
					ip_recv_header->ip_ttl--;
					ip_recv_header->ip_sum++;

				// 发送
					int sendfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW);
					struct sockaddr_ll sadr_ll;
					sadr_ll.sll_ifindex = srcmac->index; 	 // index of next hop 
					sadr_ll.sll_halen = ETH_ALEN;
					  // mac_addr_to is the result of arp query 
					memcpy(sadr_ll.sll_addr, srcmac->mac, ETH_ALEN);
					
					// length should be equal to the length you receive from raw socket 
					int result = -1;
					if ((result = sendto(sendfd, skbuf, 1514, 0, 
						(const struct sockaddr *)&sadr_ll, sizeof(struct sockaddr_ll))) == -1){
						// send error 
						printf("package sending succeeded\n");
					 } else {
						// send succeed
						printf("package sending failed\n"); 
					 }
					close(sendfd);
					
			}
		
			

		}
	}

	close(recvfd);	
	return 0;
}

