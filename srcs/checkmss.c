/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CheckMSS.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiskow <tiskow@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 09:41:27 by tiskow            #+#    #+#             */
/*   Updated: 2018/08/24 16:00:54 by tiskow           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CheckMSS.h"

int     usage()
{
    printf("Usage :\t./checkmss [-d destination addr] [-s source addr]\n");
    printf("\t\t[-t timeout] [-o all|top]\n");
    return (0);
}

int		checkOpt(int ac, char **av, t_mss **mss)
{
	int i;
	
	i = 1;
	(*mss)->timeout = 3;
	(*mss)->checkport = 2;
	strcpy((*mss)->addr_src, getIPLocal());
	memset((*mss)->addr_dst, 0, 20);
	while (i < ac)
	{
		if (!strcmp(av[i], "-s"))
			strcpy((*mss)->addr_src, av[i + 1]);
		if (!strcmp(av[i], "-d"))
			strcpy((*mss)->addr_dst, av[i + 1]);
		if (!strcmp(av[i], "-t"))
			(*mss)->timeout = atoi(av[i + 1]);
		if (!strcmp(av[i], "-o"))
		{
			if (!strcmp(av[i + 1], "all"))
				(*mss)->checkport = 1;
			if (!strcmp(av[i + 1], "top"))
				(*mss)->checkport = 2;
		}
        if (!strcmp(av[i], "-h"))
            return (0);
		i++;
	}
	if (strlen((*mss)->addr_dst) > 0)
		return (1);
	return (0);
}

void	launch_checkMSS(t_mss **mss)
{
	printf("\nCheckMSS By Tybbow - v 1.0\n\n");
	printf(COLOR_GREEN "[+] " COLOR_RESET "Adresse source : %s\n", (*mss)->addr_src);
	printf(COLOR_GREEN "[+] " COLOR_RESET "Adresse destination : %s\n", (*mss)->addr_dst);
	printf(COLOR_GREEN "[+] " COLOR_RESET "Timeout : %lu\n", (*mss)->timeout);
	printf(COLOR_GREEN "[+] " COLOR_RESET "Option : %d\n\n", (*mss)->checkport);

	int one = 1;
    const int *val = &one;
	(*mss)->socket = socket(AF_INET, SOCK_RAW , IPPROTO_TCP);
    if((*mss)->socket < 0)
    {
        printf (COLOR_RED "[-] " COLOR_RESET "Error creating socket. Error number : %d. \n\tError message : %s \n" , errno , strerror(errno));
        exit(0);
    }
    else
        printf(COLOR_GREEN "[+] " COLOR_RESET "Socket created.\n");
    if (setsockopt ((*mss)->socket, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
    {
        printf ("Error setting IP_HDRINCL. Error number : %d . Error message : %s \n" , errno , strerror(errno));
        exit(0);
    }
	initMSS(mss);
	start_thread(*mss);
	printf("\n"COLOR_GREEN "[+] " COLOR_RESET "Value MSS : %lu\n", (*mss)->totalmss);
    free((*mss)->iph);
}

t_mss	*initStruct()
{
	t_mss *mss;

	mss = NULL;
	mss = malloc(sizeof(t_mss));
	if (!mss)
		return (NULL);
	memset(mss->datagram, 0, 4096);
	mss->iph = (t_ip *)mss->datagram;
	mss->tcph = (t_tcp *)(mss->datagram + sizeof(struct ip));
	return (mss);
}

int		main(int ac, char **av)
{
	t_mss 	*mss;

	mss = initStruct();
	if(mss && checkOpt(ac, av, &mss))
		launch_checkMSS(&mss);
	else
		return (usage());
	return (0);

}
