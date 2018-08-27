/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sniffer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiskow <tiskow@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 11:42:03 by tiskow            #+#    #+#             */
/*   Updated: 2018/08/24 15:58:40 by tiskow           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CheckMSS.h"

void	process_packet(unsigned char *buffer, int size, t_mss *mss)
{
	struct iphdr *iph = (struct iphdr *)buffer;
	size_t totalmss;
	size_t iphdrlen;

	if (iph->protocol == 6)
	{
		iphdrlen = iph->ihl*4;
		struct tcphdr *tcph = (struct tcphdr *)(buffer + iphdrlen);
		if (tcph->syn == 1 && tcph->ack == 1 && iph->saddr == inet_addr(mss->addr_dst) && tcph->dest == mss->tcph->source)
		{
			totalmss = print_mss(&buffer[size - 4]);
			findMss(&mss, totalmss);
			printf(COLOR_GREEN "[+] " COLOR_RESET "Port Open: %d\n", ntohs(tcph->source));
			fflush(stdout);
		}
	}
}

void	start_sniffer(t_mss *mss)
{
	int sock_raw;
	socklen_t saddr_size;
	int data_size;
	struct sockaddr saddr;
	u_char buffer[4096];

	fflush(stdout);
	sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	if (sock_raw < 0)
	{
		printf(COLOR_RED "[+] " COLOR_RESET "Error Sniffer...\n");
		fflush(stdout);
		exit(1);
	}
	printf(COLOR_GREEN "[+] " COLOR_RESET "Sniffer en cours...\n");
	saddr_size = sizeof(saddr);
	while (1)
	{
		data_size = recvfrom(sock_raw, buffer, 4096, 0, &saddr, &saddr_size);
		if (data_size >= 0)
			process_packet(buffer, data_size, mss);
	}
}

void	receive_ack(t_mss *mss)
{
	start_sniffer(mss);
}

