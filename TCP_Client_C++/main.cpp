/*
 * main.c
 * 
 * Copyright 2018  <pi@raspberrypi>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <iostream>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

using namespace std;

int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent * server;
char buffer[255];




void error(const char* msg)
{
    perror(msg);
    exit(0);
}


void ConnectToServer()
{
    portno = 1410;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Error opening socket");
    }
    server = gethostbyname("192.168.43.191");
    if (server == NULL)
    {

        cout << "No such host" << endl;
        exit(0);

    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr,
         (char*)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    cout << "laczymy " << endl;
    if (connect(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
        error("ERROR connecting");

}

cout << "po polaczeniu" << endl;
}

void WriteToServer(char* message, int size)
{

    n = write(sockfd, message, size);
}

void ReadDFromServer(){
	n = read(sockfd,buffer,255);
	
	if(n < 0)
	error("Error reading from socket");
	printf("%s\n",buffer);
}

int main(int argc, char** argv)
{	
    ConnectToServer();
    wiringPiSetup();
    int dataAvaible = -1;
    int fid = serialOpen("/dev/ttyS0", 9600);
    sleep(3);

    if (fid < 0)
    {
        cout << " Nie udalo sie otworzyc portu" << endl;
    }
    else
    {
        cout << "Udalo otworzyc sie port" << endl;
    }

    while (true)
    {
        while (dataAvaible <= 0)
        {
            //Czekamy na dane
            dataAvaible = serialDataAvail(fid);
        }


        char* message = new char[dataAvaible];

        int i = 0;
        for (; i < dataAvaible; i++)
        {
            int currentCharacter = serialGetchar(fid);
            message[i] = currentCharacter;

        }
        message[i + 1] = '\0';

        //cout << message << endl;
        WriteToServer(message, i);

        delete message;
        //ReadDFromServer(); 




    }
    serialClose(fid);
    return 0;
}

