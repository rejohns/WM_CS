	//from TTT
	
	// old code
	/*
	// preliminary setting up of players
	listen(listen_stream,1);
	length_stream = sizeof(peer_stream);
	if ((player1 = accept(listen_stream, (struct sockaddr *)&peer_stream, &length_stream)) < 0) {
     	perror("TTT:accept");
    	exit(-1);
  	}
	
	wtsock(player1, "wait");
	
	listen(listen_stream, 1);
	length_stream = sizeof(peer_stream);
	if ((player2 = accept(listen_stream, (struct sockaddr *)&peer_stream, &length_stream)) < 0){
		perror("TTT: accept");
		exit(-1);
	}
	
	wtsock(player2, "wait");
	// end preliminary setting up of players
	
	// correspondence
	wtsock(player1, "go");
	
	recv = player1;
	send = player2;
	while(1) {
		
		msg[0] = '\0';
		
  		while (count < MSGSIZE) {
    		if (read(recv, &msg, MSGSIZE) == MSGSIZE);
    			count += MSGSIZE;
    	}
    	count = 0;
  		
		wtsock(send, msg);
  		
  		tmp  = send;
  		send = recv;
  		recv = tmp;
  		
  	}
  	*/
  	
  	// FROM tttc
  	
  			/*
		while(1) {
			
			msg[0] = '\0';
			
  			while (count < MSGSIZE) {
	  			if (read(sock, &recv, MSGSIZE) == MSGSIZE) {
    				count += MSGSIZE;
    			}
    		}
    		printf("%s", recv);
  			putchar('\n');
  			count = 0;
  			
			scanf("%s", msg);
		
  			wtsock(sock, msg);
  			
  		}
  		*/