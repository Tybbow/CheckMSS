/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiskow <tiskow@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 15:05:09 by tiskow            #+#    #+#             */
/*   Updated: 2018/08/24 16:28:36 by tiskow           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CheckMSS.h"

void	sendSyn(t_mss *mss)
{
	size_t port;
	struct sockaddr_in  dst;
	printf(COLOR_GREEN "[+] " COLOR_RESET "Envoie des paquets Syn...\n");

	dst.sin_family = AF_INET;
	dst.sin_addr.s_addr = inet_addr(mss->addr_dst);
	port = 1;
	while (port < 100)
	{
		mss->tcph->dest = htons(port);
		mss->tcph->check = 0;

		memcpy(&mss->psdheader.tcp, mss->tcph, sizeof(t_pshd));
		mss->tcph->check = csum((u_short *)&mss->psdheader, sizeof(t_pshd) + sizeof(t_otcp));
        mss->datagram[40] = 0x02;
        mss->datagram[41] = 0x04;
        mss->datagram[42] = 0x05;
        mss->datagram[43] = 0xb4;
		sendto(mss->socket, mss->datagram, sizeof(struct ip) + sizeof(t_tcp) + sizeof(t_otcp), 0, (struct sockaddr *)&dst, sizeof(dst));
		port++;
		usleep(30000);
	}
	
}
