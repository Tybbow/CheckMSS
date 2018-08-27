/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiskow <tiskow@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/20 15:43:49 by tiskow            #+#    #+#             */
/*   Updated: 2018/08/24 15:53:13 by tiskow           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CheckMSS.h"

char	*getIPLocal()
{
	struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1)
        return (NULL);
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr != NULL && ifa->ifa_addr->sa_family == AF_INET)
        {
            if (!strcmp(ifa->ifa_name, "en0") || !strcmp(ifa->ifa_name, "eth0") || 
            !strcmp(ifa->ifa_name, "enp0s3"))
            {
                freeifaddrs(ifaddr);
                return (inet_ntoa(((struct sockaddr_in *)ifa->ifa_addr)->sin_addr));
            }
        }
    }
    return (NULL);
}

unsigned short csum(unsigned short *ptr, int nbytes) 
{
    register long sum;
    unsigned short oddbyte;
    register short answer;
 
    sum=0;
    while(nbytes>1) {
        sum+=*ptr++;
        nbytes-=2;
    }
    if(nbytes==1) {
        oddbyte=0;
        *((unsigned char *)&oddbyte)=*(unsigned char *)ptr;
        sum+=oddbyte;
    }
 
    sum = (sum>>16)+(sum & 0xffff);
    sum = sum + (sum>>16);
    answer=(short)~sum;
     
    return(answer);
}

size_t    print_mss(u_char *buffer)
{
    size_t mss;

    mss = 0;
    if (buffer[0] == 2)
    {
        mss = buffer[2] * 256;
        mss += buffer[3];
    }
	return (mss);
}

void	findMss(t_mss **mss, size_t value)
{
	if (value < (*mss)->totalmss)
		(*mss)->totalmss = value;
}
