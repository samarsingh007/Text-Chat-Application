/**
 * @samarpra_assignment1
 * @author  Samar Pratap Singh <samarpra@buffalo.edu>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This contains the main function. Add further description here....
 */
#include<bits/stdc++.h>

#include <fcntl.h>

#include "../include/global.h"
#include "../include/logger.h"
#include "../include/events.h"


using namespace std;
#define v_string vector<string>   

struct Info_Client {
	int list_id, port_number;
	bool is_Logged_in;
	string name_of_host, IP_address,status;
	char re[50];
	string port;
	int num_msg_sent;
	int num_msg_rcv;
    int socketfd;
	bool operator<(const Info_Client &rhs) const {
    	return port_number < rhs.port_number;
  }
  v_string buffer;
  v_string User_blocked;
};

string port,host_name,ip;
int serverfd, clientfd = 0;
bool loggedIn = false;

vector<Info_Client> Data_Client;
/**
 * main function
 *
 * @param  argc Number of arguments
 * @param  argv The argument list
 * @return 0 EXIT_SUCCESS
 */


Info_Client* newClientInfo(int socketfd, string name_of_host, string ip, int port_number) {
  Info_Client* client = new Info_Client;
  client->socketfd = socketfd;
  client->name_of_host = name_of_host;
  client->IP_address = ip;
  client->port_number = port_number;

  	string ans = "";
	while(port_number) {
		int x = port_number%10;
		char y = (char) (x + '0');
		ans = y + ans;
		port_number = port_number/10;
	}
  client->port = ans;
  client->is_Logged_in = true;
  client->status = "Logged-In";
  return client;
}

Info_Client* getClientData(int clientfd) {
	int i = 0;
	while(i<Data_Client.size()){
		if(Data_Client[i].socketfd != clientfd) {
		}
		else{
			return &Data_Client[i];
		}
		i++;
	}

	return NULL;
}

Info_Client* getClientData(string ip) {
	int i = 0;
	while(i<Data_Client.size()){
		if(Data_Client[i].IP_address != ip)
		{  						
	
		 } 
		 else{
			return &Data_Client[i];
		 }
		 i++; 						
	}
	return NULL;
}

Info_Client* getClientData(string ip, string port) {
int i=0;
	while(i<Data_Client.size()) {
		if(Data_Client[i].IP_address == ip && Data_Client[i].port == port) return &Data_Client[i];
	i++;}

	return NULL;
}

v_string load_prm(string msg, char sep) {
	v_string params;
	string dummy = "";
	int i=0;
	while(i<msg.size()) {
		if(msg[i] != sep) {
			dummy += msg[i];
		} else {
			
			params.push_back(dummy);
			dummy = "";
		}
		i++;
	}

	params.push_back(dummy);
	return params;
}

// CHECKING IF IP EXISTS
bool CheckIP(string str) {
	char end[256];
  	end[0] = 0;
	int a = -1; 
	int b = -1;
	int c = -1; 
	int d = -1;
	int z,r = sscanf(str.c_str(), "%d.%d.%d.%d%s", &a, &b, &c, &d, end);
  	int y;
  	if (r == 4 && !end[0] && a >= 0 && a <= 255 && b >= 0 ) {
	if 

	(b <= 255 && c >= 0 && c <= 255 && d >= 0 && d <= 255) 
		{return 1;
  	}} 
	else {
    	return -1;
  	}
}

int CheckPort(string str) {
	char end[256];
	end[0] = 0;
	int a = -1,r = sscanf(str.c_str(), "%d%s", &a, end);;
	if (r == 1) {
		  if (r =!end[0] && a > 0 && a <= 65535){ 
		return 1;
	}}
	else {
		return -1;
	}
}
//----------------------------------------------------------------------------------------------------------
bool checkIfIPBlocked(Info_Client* cd, string ip) {

  for (int i = 0; i < cd->User_blocked.size(); i++) {
    if (cd->User_blocked[i] == ip) {
      return true;
    }
  }
  return false;
}
//-----------------------------------------------------------------------------------------------------------

void getHostNameIpAndPort(char *myPort) {
	port = myPort;
	char myHostName[1024],buffer[256];
	gethostname(myHostName, sizeof(myHostName)-1);
	host_name = myHostName;



    size_t bufLen = 256;


    int udp_socketfd = socket(AF_INET,SOCK_DGRAM,0);
    const char *kGoogleDnsIp = "8.8.8.8";
    uint16_t kDnsPort = 53;
    struct sockaddr_in googleServInfo;
    memset(&googleServInfo, 0, sizeof(googleServInfo));
    googleServInfo.sin_family = AF_INET;
    googleServInfo.sin_addr.s_addr = inet_addr(kGoogleDnsIp);
    googleServInfo.sin_port = htons(kDnsPort);

    if(connect(udp_socketfd, (struct sockaddr *) &googleServInfo, sizeof(googleServInfo)) != -1){

	} else{
		cse4589_print_and_log("Failed to connect with the Google server");
    }

    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    if(getsockname(udp_socketfd, (struct sockaddr *)&name, &namelen) != -1){

	} else{
        cse4589_print_and_log("Failed to get the socket name of the host\n");
    }

    const char *myIp = inet_ntop(AF_INET, &name.sin_addr, buffer, bufLen);
    ip = myIp;
    close(udp_socketfd);
	return;
}


std::string extractCommand(std::string full_cmd) {
	string answer = "";
	int i = 0;
	while(i<full_cmd.size()) {
		if(full_cmd[i] == ' ') return answer;
		answer = answer+full_cmd[i];
		i++;
	}

	return answer;
}	

void *get_in_addr(struct sockaddr *sa){
	if (sa->sa_family != AF_INET){

	} else{

		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


//-------------------------------------------------------------------------------------------------------------------------------------
//Client utilities from here.

void eventMsgRecv(vector<string> cmd, int cmdArgc, string orinmsg) {
  
}

void handleClientEvents(const char* buffer){
    string msg = buffer;
    v_string params = load_prm(msg, ' ');
    int paramCount = params.size();
    if (params[0] == "SEND" && paramCount >= 3) {
      	int nonMsgLen = params[0].length() + 2 + params[1].length();
		string newString = msg.substr(nonMsgLen, msg.length() - nonMsgLen);
  		string ip = params[1];

  		string event = "RECEIVED";
  		cse4589_print_and_log("[%s:SUCCESS]\n", event.c_str());
  		cse4589_print_and_log("msg from:%s\n[msg]:%s\n", ip.c_str(), newString.c_str());
  		cse4589_print_and_log("[%s:END]\n", event.c_str());
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------

int client(char **argv) {
	getHostNameIpAndPort(argv[2]);
	char buffer[65535];
	int sock_max = clientfd;


	fd_set lookuplist;
	

	while(TRUE) {
		FD_ZERO(&lookuplist);
		FD_SET(fileno(stdin), &lookuplist);
		FD_SET(sock_max, &lookuplist);
		sock_max = clientfd;
		memset(&buffer[0], 0, sizeof(buffer));

		int select_retr = select(sock_max+ 1, &lookuplist, NULL, NULL, NULL);

		if(select_retr >= 0)
		{

		} else{
			cse4589_print_and_log("selection failure \n");
			return -1;
		}

		if(FD_ISSET(fileno(stdin), &lookuplist)) {



			char *cmd_new_line = (char*) malloc(sizeof(char)*CMD_SIZE);
			memset(cmd_new_line, '\0', CMD_SIZE);
			
			if(fgets(cmd_new_line, CMD_SIZE-1, stdin) != NULL)
			{

			} else{
				cse4589_print_and_log("Please input a command\n");
			}

			std::string cmd_dummy(cmd_new_line, cmd_new_line + strlen(cmd_new_line)-1);
			std::string command = "";


			command = extractCommand(cmd_dummy);

			v_string cmd_prms = load_prm(cmd_dummy, ' ');

			if(checkForLower(command)) {
				cse4589_print_and_log("The command to be given in all capital letters\n");
			}

			if("AUTHOR" == command && cmd_prms.size() == 1) {
				cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
				cse4589_print_and_log("I, %s, have read and understood the course academic integrity policy.\n", "samarpra");
				cse4589_print_and_log("[%s:END]\n", command.c_str());
			}

			else if("IP" == command && cmd_prms.size() == 1) {
				cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
				cse4589_print_and_log("IP:%s\n", ip.c_str());
				cse4589_print_and_log("[%s:END]\n", command.c_str());
			}	

			else if("PORT" == command && cmd_prms.size() == 1) {
				cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
				cse4589_print_and_log("PORT:%s\n", port.c_str());
				cse4589_print_and_log("[%s:END]\n", command.c_str());
			}

			else if ("LIST" == command && cmd_prms.size() == 1 && loggedIn) {
				cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());

			int i = 0;
			while(i<Data_Client.size()) {


				cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", i+1, Data_Client[i].name_of_host.c_str(), Data_Client[i].IP_address.c_str(), Data_Client[i].port_number);
				i++;
				}


				cse4589_print_and_log("[%s:END]\n", command.c_str());
			}
			
			else if ("LOGIN" == command && cmd_prms.size() == 3) {

			if (CheckIP(ip) < 0 || CheckPort(port) < 0) {

				cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
				cse4589_print_and_log("[%s:END]\n", command.c_str());
				continue;
  			}

				struct addrinfo clues, *servAddrInfo;
				memset(&clues, 0, sizeof clues);
				clues.ai_family = AF_INET;
				clues.ai_socktype = SOCK_STREAM;

				string server_IP = "", server_port = "",tem = "";

				int wrd = 0;

				for (int it = 0;it < cmd_dummy.size();it++){
					if(cmd_dummy[it] == ' ') {
						if(wrd == 1) {
							server_IP = tem;
							tem = "";
							wrd++;

						} else if(wrd == 0) {
							wrd++;
							tem = "";
						}
					} else {
						tem = tem + cmd_dummy[it];
					}
				}

				server_port = tem;

				if (clientfd != 0) {
					
				}
				else
				{


getaddrinfo(server_IP.c_str(), server_port.c_str(), &clues, &servAddrInfo);
					clientfd = socket(servAddrInfo->ai_family, servAddrInfo->ai_socktype, servAddrInfo->ai_protocol);
					if (clientfd != -1){

					} else{
						cse4589_print_and_log("Error creating socket\n");
						continue;
					}

					if(connect(clientfd, servAddrInfo->ai_addr, servAddrInfo->ai_addrlen) >= 0){
						
					}
					else {
			cse4589_print_and_log("Login error, couldn't connect\n");
						continue;
					}
				}

				string msg = "LOGIN " + host_name + " " + ip + " " + port;
				send(clientfd, msg.c_str(), strlen(msg.c_str()), 0);

				v_string data;
				char temp[65535];
				int receive_size = recv(clientfd, temp, sizeof(temp), 0);
				if(receive_size >= 0){

				} else{ 

					cse4589_print_and_log("Recv Failed\n");
					continue;
				}
				Data_Client.clear();
				data = load_prm(temp, '-');

				// receive list
 

				int it = 0;

				for (it = 1; it < data.size()-1; it++) {
					v_string clientString;
					clientString = load_prm(data[it], ' ');
					if (clientString.size() < 1 || clientString[0] == "LOGINEND") {
						break;
					}
 

 

					Info_Client *clientInfo = newClientInfo(-1, clientString[0], clientString[1], atoi(clientString[2].c_str()));
					Data_Client.push_back(*clientInfo);
				}

				// receive cached msg
				while(it < data.size()-1) {
					v_string receivedMessages;
					receivedMessages = load_prm(data[it], ' ');
					if (data.size() < 1 || data[0] == "LOGINCACHEEND") {
						break;
					}
					handleClientEvents(data[it].c_str());
					it++;
				}

				loggedIn=true;
				cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
				cse4589_print_and_log("[%s:END]\n", command.c_str());
				fflush(stdout);
			}

			else if("REFRESH" == command && cmd_prms.size() ==1 && loggedIn) {
				send(clientfd, cmd_dummy.c_str(), strlen(cmd_dummy.c_str()), 0);

				v_string data;
				char temp[65535];
				int receive_size = recv(clientfd, temp, sizeof(temp), 0);
				if(receive_size >= 0){

				} else{ 
 

					cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
					cse4589_print_and_log("[%s:END]\n", command.c_str());
					continue;
				}

				Data_Client.clear();
				data = load_prm(temp, '-');

				// receive list
				int it = 0;

				if (data.size() >= 1 || data[0] == "REFRESHSUCCESS"){

				} else{
 

					cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
					cse4589_print_and_log("[%s:END]\n", command.c_str());
					continue;
				}


				for (it = 1; it < data.size()-1; it++) {
					v_string clientString;
					clientString = load_prm(data[it], ' ');
					if (clientString.size() < 1 || clientString[0] == "REFRESHEND") {
						break;
					}
 

 

					Info_Client *clientInfo = newClientInfo(-1, clientString[0], clientString[1], atoi(clientString[2].c_str()));
					Data_Client.push_back(*clientInfo);
				}

				data.clear();


				// cmdList(cmd);
				cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
				cse4589_print_and_log("[%s:END]\n", command.c_str());
			}

 
//----------------------------------------------------------------------------------------------------------------------------------------
			else if("SEND" == command && cmd_prms.size() >= 3 && loggedIn) {

					if (CheckIP(ip) < 0 || getClientData(ip) == NULL) {
						cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
						cse4589_print_and_log("[%s:END]\n", command.c_str());
						continue;
					}
					
					send(clientfd, cmd_dummy.c_str(), strlen(cmd_dummy.c_str()), 0);
					
					char temp[65535];
					v_string data;
					int recvSize = recv(clientfd, temp, sizeof temp, 0);
					string msg = temp;
					if (recvSize < 0) {
						cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
						cse4589_print_and_log("[%s:END]\n", command.c_str());
						continue;
					}

					data = load_prm(msg, '-');

					if (data[0] != "SENDSUCCESS" || data.size() < 1) {
						cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
						cse4589_print_and_log("[%s:END]\n", command.c_str());
						continue;
					}

					data.clear();

					cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
					cse4589_print_and_log("[%s:END]\n", command.c_str());
				}

			else if("BROADCAST" == command && cmd_prms.size() == 2 && loggedIn) {
				int status = fcntl(clientfd, F_SETFL, fcntl(clientfd, F_GETFL, 0) | O_NONBLOCK);
				send(clientfd, cmd_dummy.c_str(), strlen(cmd_dummy.c_str()), 0);

				char temp[65535];
				v_string data;
				int recvSize = recv(clientfd, temp, sizeof temp, 0);
				string msg = temp;
				cout<<msg<<endl;

				if(recvSize < 0) {
					cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
					cse4589_print_and_log("[%s:END]\n", command.c_str());
					continue;
				}

				data = load_prm(msg, '-');
				if(data[0] != "BROADCASTSUCCESS" || data.size() < 1) {
					cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
					cse4589_print_and_log("[%s:END]\n", command.c_str());
					continue;
				}

				data.clear();

				cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
				cse4589_print_and_log("[%s:END]\n", command.c_str());
				}

			else if("BLOCK" == command && cmd_prms.size() == 2 &&  loggedIn) {

				if (CheckIP(ip) < 0 || getClientData(ip) == NULL) {
					cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
					cse4589_print_and_log("[%s:END]\n", command.c_str());
    				continue;
  				}

  				string msg = command + " " + cmd_prms[1];
  				send(clientfd, msg.c_str(), strlen(msg.c_str()), 0);
  				// receive all response
  				char temp[65535];
  				v_string data;
  				int recvSize = recv(clientfd, temp, sizeof(temp), 0);
  				msg = temp;
  				if (recvSize < 0) {
					cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
					cse4589_print_and_log("[%s:END]\n", command.c_str());
    				continue;
  				}

  				data = load_prm(msg, '-');
  				if (data[0] != "BLOCKSUCCESS" || data.size() < 1) {
					cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
					cse4589_print_and_log("[%s:END]\n", command.c_str());
    				continue;
  				}

				data.clear();
  				cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
  				cse4589_print_and_log("[%s:END]\n", command.c_str());
			}

			else if("UNBLOCK" == command && cmd_prms.size() == 2 && loggedIn) {
				if (CheckIP(ip) < 0 || getClientData(ip) == NULL) {
					cse4589_print_and_log("[%s:ERROR]	\n", command.c_str());
					cse4589_print_and_log("[%s:END]\n", command.c_str());
    				continue;
  				}
  				string msg = command + " " + cmd_prms[1];
  				send(clientfd, msg.c_str(), strlen(msg.c_str()), 0);
  				// receive all response
  				char temp[65535];
  				vector<string> data;
  				int recvSize = recv(clientfd, temp, sizeof(temp), 0);
  				msg = temp;
  				if (recvSize < 0) {
					cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
					cse4589_print_and_log("[%s:END]\n", command.c_str());
    				continue;
  				}
  				data = load_prm(msg, '-');
  				if (data[0] != "UNBLOCKSUCCESS" || data.size() < 1) {
					cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
					cse4589_print_and_log("[%s:END]\n", command.c_str());
    				continue;
  				}

				data.clear();
  				cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
  				cse4589_print_and_log("[%s:END]\n", command.c_str());
			}

			else if ("LOGOUT" == command && cmd_prms.size() ==1 && loggedIn) {
				string msg = "LOGOUT " + ip + " " + port;
  				send(clientfd, msg.c_str(), strlen(msg.c_str()), 0);
  				loggedIn=false;
  				cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
  				cse4589_print_and_log("[%s:END]\n", command.c_str());
  				close(clientfd);
  				clientfd=0;
			}
//--------------------------------------------------------------------------------------------------------------------------------------------
 

			else if("EXIT" == command && cmd_prms.size() == 1) {
				string msg = "EXIT";
  				send(clientfd, msg.c_str(), strlen(msg.c_str()), 0);
  				cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
  				cse4589_print_and_log("[%s:END]\n", command.c_str());
  				exit(0); 
			}

			else {
				cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
				cse4589_print_and_log("[%s:END]\n", command.c_str());
			}

		} else if (FD_ISSET(clientfd, &lookuplist)) {
			if(recv(clientfd, buffer, sizeof(buffer), 0) != 0) {
				string msg = buffer;
   	 			v_string params = load_prm(msg, ' ');
    			int paramCount = params.size();

			} else {
					
			close(clientfd);
				clientfd = 0;
				
			}
		}
	}

	return 0;
}

void initializeServer(char **argv) {
	getHostNameIpAndPort(argv[2]);

	struct addrinfo clues, *addrInfo;
  	memset(&clues, 0, sizeof clues);  
	int status;
  	clues.ai_family = AF_INET;        
  	clues.ai_socktype = SOCK_STREAM;  
  	clues.ai_flags = AI_PASSIVE;

	// struct addrinfo *addrInfo;

  	if ((status = getaddrinfo(NULL, argv[2], &clues, &addrInfo)) == 0) {}
	else
	{
    	exit(1);
  	}

	int output = 1;
  	serverfd = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
	if(serverfd != -1) {
	}else{
		cse4589_print_and_log("Error creating socket\n");
		exit(-1);
	}

	if(setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (char*)&output, sizeof(output)) >=0) {
	}else{
		cse4589_print_and_log("Set sock output failed\n");
	}

	if(bind(serverfd, addrInfo->ai_addr, addrInfo->ai_addrlen) != -1) {
		}
		else{
		cse4589_print_and_log("Error binding socket to the given port\n");
		exit(-1);
  	}

	if(listen(serverfd, BACKLOG) >= 0) {
		}
		else{
		cse4589_print_and_log("Unable to listen to port.\n");
		exit(-1);
	} 
}

void handleServerEvents(const char *buffer, int new_clientfd) {
	string msg = buffer;
	v_string cmd_prms = load_prm(msg, ' ');
	int numberOfParams = cmd_prms.size();

//----------------------------------------------------------------------------------------------------------------------------------------
	if (cmd_prms[0] == "SEND" && numberOfParams >= 3) {
		string event = "RECEIVED";
  		Info_Client* sender_cd = getClientData(new_clientfd);
  		string senderIP = sender_cd->IP_address;
  		int nonMsgLen = cmd_prms[0].length() + 2 + cmd_prms[1].length();
  		string info = "SEND " + senderIP + " " + msg.substr(nonMsgLen, msg.length() - nonMsgLen);
  		string receiverIP = cmd_prms[1];
  		Info_Client* receiver_cd = getClientData(receiverIP);
  		if (receiver_cd != NULL && checkIfIPBlocked(receiver_cd, senderIP) == false) {
    		if (receiver_cd->status == "logged-in") {
      			send(receiver_cd->socketfd, info.c_str(), strlen(info.c_str()), 0);
    		} else {
      			receiver_cd->buffer.push_back(info);
    		}
    		receiver_cd->num_msg_rcv++;
  		}
  		sender_cd->num_msg_sent++;
	    string ack = "SENDSUCCESS-";
  		send(new_clientfd, ack.c_str(), strlen(ack.c_str()), 0);
  		info = msg.substr(nonMsgLen, msg.length() - nonMsgLen);
  		cse4589_print_and_log("[%s:SUCCESS]\n", event.c_str());
  		cse4589_print_and_log("msg from:%s, to:%s\n[msg]:%s\n", senderIP.c_str(), receiverIP.c_str(), info.c_str());
  		cse4589_print_and_log("[%s:END]\n", event.c_str());
  	} 
	else if(cmd_prms[0] == "BROADCAST" && numberOfParams >= 2) {
		string event = "RELAYED";
  		Info_Client* sender_cd = getClientData(new_clientfd);
  		string senderIP = sender_cd->IP_address;
  		int nonMsgLen = cmd_prms[0].length() + 1;
  		string info = "SEND " + senderIP + " " + msg.substr(nonMsgLen, msg.length() - nonMsgLen);

  		for (int i = 0; i < Data_Client.size(); i++) {
    		if (Data_Client[i].socketfd == new_clientfd) {
      			Data_Client[i].num_msg_sent++;
      			continue;
    		}
    		if(checkIfIPBlocked(&Data_Client[i], senderIP) == false){
      			if (Data_Client[i].is_Logged_in) {
        			send(Data_Client[i].socketfd, info.c_str(), strlen(info.c_str()), 0);
      			} else {
        			Data_Client[i].buffer.push_back(info);
      			}
      			Data_Client[i].num_msg_rcv++;
    		}
  		}

		string ack = "BROADCASTSUCCESS-";
  		send(clientfd, ack.c_str(), strlen(ack.c_str()), 0);

  		ack = msg.substr(nonMsgLen, msg.length() - nonMsgLen);
  		cse4589_print_and_log("[%s:SUCCESS]\n", event.c_str());
  		cse4589_print_and_log("msg from:%s, to:%s\n[msg]:%s\n", senderIP.c_str(), "255.255.255.255", msg.c_str());
  		cse4589_print_and_log("[%s:END]\n", event.c_str());
	}

	else if(cmd_prms[0] == "BLOCK" && numberOfParams == 2) {
		string destIP = cmd_prms[1];
		string msg;

  		Info_Client* dest_client_info = getClientData(destIP);
  		Info_Client* sender_client_info = getClientData(new_clientfd);

  		if (dest_client_info == NULL || checkIfIPBlocked(sender_client_info, destIP)) {

    		msg = "BLOCKFAIL-";
    		send(new_clientfd, msg.c_str(), strlen(msg.c_str()), 0);
			return;
  		}

  		sender_client_info->User_blocked.push_back(destIP);

  		msg = "BLOCKSUCCESS-";
  		if(send(new_clientfd, msg.c_str(), strlen(msg.c_str()), 0)<=0)
  		{
  			cse4589_print_and_log("\nSend failure from server in BLOCK case\n");//krishhh+
  		}
	}

	else if(cmd_prms[0] == "UNBLOCK" && numberOfParams == 2) {
		string destIP = cmd_prms[1];
  		string msg;
		Info_Client* dest_client_info = getClientData(destIP);
  		Info_Client* sender_client_info = getClientData(new_clientfd);

  		if (dest_client_info == NULL || !checkIfIPBlocked(sender_client_info, destIP)) {
    		msg = "UNBLOCKFAIL-";
    		if(send(new_clientfd, msg.c_str(), strlen(msg.c_str()), 0)<0)//krishhh++
    		{//krishhh+
    			  cse4589_print_and_log("\nSend failure from server in UNBLOCK case\n");//krishhh+
    		}//krishhh+
  		}
  		for (int i = 0; i < sender_client_info->User_blocked.size(); i++) {
    		if (sender_client_info->User_blocked[i] == destIP) {
				sender_client_info->User_blocked.erase(sender_client_info->User_blocked.begin() + i--);
    			break;
			}
		}

  			msg = "UNBLOCKSUCCESS-";
  			send(new_clientfd, msg.c_str(), strlen(msg.c_str()), 0);
	}

		else if(cmd_prms[0] == "LOGOUT" && numberOfParams == 3) {
			Info_Client* cd = getClientData(cmd_prms[1], cmd_prms[2]);
  			cd->status = "logged-out";
			cd->is_Logged_in = false;
		}
//--------------------------------------------------------------------------------------------------------------------------------------------

		if(cmd_prms[0] == "EXIT" && numberOfParams == 1) {
						int i = 0; 
				while (i< Data_Client.size()) {
					if (Data_Client[i].socketfd == new_clientfd) Data_Client.erase(Data_Client.begin() + i--);
					return;
					i++;
				}
		}
		else if (cmd_prms[0] == "REFRESH" && numberOfParams == 1) {
					string msg = "REFRESHSUCCESS-";
		int i = 0;
		while(i<Data_Client.size()) {
			if(Data_Client[i].is_Logged_in) {
				msg = msg + Data_Client[i].name_of_host + " " + Data_Client[i].IP_address + " " + Data_Client[i].port + "-";
			}
			i++;
		}
		msg += "REFRESHEND-";
		send(new_clientfd, msg.c_str(), strlen(msg.c_str()), 0);
		}

		else if(cmd_prms[0] == "FORCEEXIT" && numberOfParams == 1) {
			int i = 0;
			while (i < Data_Client.size()) {
				if (Data_Client[i].socketfd == clientfd) Data_Client.erase(Data_Client.begin() + i--);
    			return;
				i++;
  			}
		}
	}

int server(int argc, char **argv) {
	initializeServer(argv);
	struct sockaddr_in client_addr; 
	char buffer[65535];
	fd_set lookuplist;

	int clientSockets[5] = {0};
	int sock_max = serverfd;

	while(true) {
		FD_ZERO(&lookuplist);
		FD_SET(fileno(stdin), &lookuplist); /* Register STDIN */
		FD_SET(serverfd, &lookuplist);

 
		int i=0;
		while( i < 5) {
      		int client_socketfd = clientSockets[i];
      		if (client_socketfd > 0) FD_SET(client_socketfd, &lookuplist);
      		if (client_socketfd > sock_max) sock_max = client_socketfd;
			i++;
    	}

		memset(&buffer[0], 0, sizeof(buffer));
 

		int select_retr = select(sock_max+1, &lookuplist, NULL, NULL, NULL);

		if(select_retr >= 0) {

		}
		else{
			cse4589_print_and_log("select failed \n");
			exit(-1);
		}
		
		if(select_retr <= 0) {
		}
		else{
				if(FD_ISSET(fileno(stdin), &lookuplist)) {
 

					char *cmd_new_line = (char*) malloc(sizeof(char)*CMD_SIZE);
					memset(cmd_new_line, '\0', CMD_SIZE);
					if(fgets(cmd_new_line, CMD_SIZE-1, stdin) == NULL) {
 

						cse4589_print_and_log("Please input a command\n");
						exit(-1);
					}

					std::string cmd_dummy(cmd_new_line, cmd_new_line + strlen(cmd_new_line)-1);
					string command = ""; 
					command = extractCommand(cmd_dummy);
					v_string cmd_prms = load_prm(cmd_dummy, ' ');
 


					if(checkForLower(&command[0])) {
						cse4589_print_and_log("The command should be given in all capital letters\n");
					}

					if(checkForLower(&command[0])) {
						cse4589_print_and_log("The command should be given in all capital letters\n");
					}

					if("AUTHOR" != command || cmd_prms.size() != 1) {

					}
					else{
						cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
						cse4589_print_and_log("I, %s, have read and understood the course academic integrity policy.\n", "samarpra");
						cse4589_print_and_log("[%s:END]\n", command.c_str());
					}

					if ("IP" != command || cmd_prms.size() != 1) {

					}
					else{
						cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
						cse4589_print_and_log("IP:%s\n", ip.c_str());
						cse4589_print_and_log("[%s:END]\n", command.c_str());
					}		

					if ("PORT" != command || cmd_prms.size() != 1) {

					}
					else{
						cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
						cse4589_print_and_log("PORT:%s\n", port.c_str());
						cse4589_print_and_log("[%s:END]\n", command.c_str());
					}

					if ("LIST" != command || cmd_prms.size() != 1) {
						}
						else{

						cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());


						
						//Get a sorted List of Clients

						int i = 0;
						while(i < Data_Client.size())
						{
 

						cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", i + 1, Data_Client[i].name_of_host.c_str(), Data_Client[i].IP_address.c_str(), Data_Client[i].port_number);
						i++;
						}
						cse4589_print_and_log("[%s:END]\n", command.c_str());
					}

					if("EXIT" == command && cmd_prms.size() == 1) {
						string msg = "EXIT";
						send(clientfd, msg.c_str(), strlen(msg.c_str()), 0);
						cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
						cse4589_print_and_log("[%s:END]\n", command.c_str());
						exit(0); 
					}

					

//---------------------------------------------------------------------------------------------------------------------------------------------------
					if("BLOCKED" == command && cmd_prms.size() == 2) {
  						Info_Client* clientInfo = getClientData(cmd_prms[1]);

  						cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
  						if(clientInfo != NULL) {
  							for (int i = 0; i < clientInfo->User_blocked.size(); i++) {
    							Info_Client* hd = getClientData(clientInfo->User_blocked[i]);
    							cse4589_print_and_log("%-5d%-35s%-20s%-8d\n",i+1, hd->name_of_host.c_str(), hd->IP_address.c_str(), hd->port_number);
  							}
						}
	
						cse4589_print_and_log("[%s:END]\n", command.c_str());
					}

					if("STATISTICS" == command && cmd_prms.size() == 1) {
						vector<string> cmd_prms = load_prm(cmd_dummy, ' ');
						cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
						for(int i=0; i<Data_Client.size(); i++) {
							cse4589_print_and_log("%-5d%-35s%-8d%-8d%-8s\n", i+1, Data_Client[i].name_of_host.c_str(), Data_Client[i].num_msg_sent, Data_Client[i].num_msg_rcv, Data_Client[i].status.c_str());
						}
						cse4589_print_and_log("[%s:END]\n", command.c_str());
					}
//---------------------------------------------------------------------------------------------------------------------------------------------------

					free(cmd_new_line);
				} else if(FD_ISSET(serverfd, &lookuplist)) { 
					socklen_t caddr_len = sizeof(client_addr);
					int new_clientfd = accept(serverfd, (struct sockaddr *)&client_addr, &caddr_len);
					if(new_clientfd >= 0) {
						}
					else{
						cse4589_print_and_log("Accept failed.\n");
					}
					
					int i = 0;
					while (i < 5) {
						if(clientSockets[i] != 0) {
							
						}else{
							clientSockets[i] = new_clientfd;
							break;
						}
						i++;
					}

					//New Login
					recv(new_clientfd, buffer, sizeof(buffer), 0);
					string a;
					string msg = buffer;
					v_string login_parameters = load_prm(msg, ' ');
					

					if(login_parameters[0] == "LOGIN" && login_parameters.size() != 4){
					}
					else {
						Info_Client *clientInfo = getClientData(new_clientfd);
						if(clientInfo != NULL) {
							clientInfo->is_Logged_in = true;
						} 
						else {
							clientInfo = newClientInfo(new_clientfd, login_parameters[1], login_parameters[2], atoi(login_parameters[3].c_str()));
							Data_Client.push_back(*clientInfo);
							std::sort(Data_Client.begin(), Data_Client.end());
						}

						msg = "LOGINSUCCESS-";
						int i = 0;
						while(i<Data_Client.size()) {
							if(Data_Client[i].is_Logged_in) {
								msg = msg + Data_Client[i].name_of_host + " " + Data_Client[i].IP_address + " " + Data_Client[i].port + "-";
							}
							i++;
						}
						msg += "LOGINEND-";

						while(i < clientInfo->buffer.size()){
							msg = msg + clientInfo->buffer[i] + "-";
							++i;
						}
						clientInfo->buffer.clear();
						msg += "LOGINCACHEEND-";
						send(new_clientfd, msg.c_str(), strlen(msg.c_str()), 0);
					}
				} else { 
					int i = 0;
					while (i < 5) {
						int  new_clientfd = clientSockets[i];
						if (FD_ISSET(new_clientfd, &lookuplist)) {
						
							if (recv(new_clientfd, buffer, sizeof(buffer), 0) != 0) {
								handleServerEvents(buffer, new_clientfd);
							} else { 
								handleServerEvents("FORCEEXIT", new_clientfd);
								close(new_clientfd);
								clientSockets[i] = 0;
							}
						}
						i++;
					}
				}
			}
		}
	}	


int main(int argc, char **argv) {
	cse4589_init_log(argv[2]);


	fclose(fopen(LOGFILE, "w"));

	// If there are insufficient arguments present 
	if(argc >=3) {
	} 

	else {
		return -1;
	}

	// To initialize as a server 
	if(strcmp(argv[1],"s") != 0){
	} 
	else{	
		server(argc, argv);
	}

	// To initialize as a new client
	if(strcmp(argv[1],"c") != 0){

	} else{	
		client(argv);
	}
	return 0;
}