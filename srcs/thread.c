/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiskow <tiskow@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/20 16:06:21 by tiskow            #+#    #+#             */
/*   Updated: 2018/08/24 15:48:20 by tiskow           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CheckMSS.h"

pthread_t myThreadSniffer;

void	*threadSniffer(void *arg)
{
	t_mss *mss = (t_mss*)arg;
	receive_ack(mss);
	return (NULL);
}

void	*threadSendSyn(void *arg)
{
	t_mss *mss = (t_mss*)arg;
	sendSyn(mss);
	sleep(mss->timeout);
	pthread_cancel(myThreadSniffer);
	return (NULL);
}

void	start_thread(t_mss *mss)
{
	pthread_t myThreadSendSyn;

	pthread_create(&myThreadSniffer, NULL, threadSniffer, mss);
	pthread_create(&myThreadSendSyn, NULL, threadSendSyn, mss);

	pthread_join(myThreadSniffer, NULL);
	pthread_join(myThreadSendSyn, NULL);
}