//Winsock server with multi-client (multithreaded) handling.

//Use latest winsock (includes windows.h)
#include <windows.h>
#include <windns.h>
#include <ws2tcpip.h>
#include <iostream>
#include <winsock2.h>
#include <process.h>
#include <stdio.h>
#include <string>
#include <iomanip>
#include "infix_interator.h"

using namespace std;
//-----Prototypes and global vars
void ReverseIP(char* pIP);
unsigned int __stdcall ServClient(void*data);



// BEGIN!!!!
int main(int argc, _TCHAR* argv[]){

WSADATA wsaData;

int iResult; //Validity switch
sockaddr_in addr;
SOCKET sock,client;
addr.sin_family = AF_INET;

int PORT;
    printf("______________Project Z alpha 0.0.3____________\n");



addr.sin_port = htons(PORT); //Listen port

//addr.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDRS.c_str()); //Listen IP


//________________________________BEGIN________________________

//_____Startup check , if you have problems here you got problems everywur_____

iResult = WSAStartup(MAKEWORD(2,2),&wsaData);

if(iResult){

        printf("[X]WSA Startup failure..");
        return 0;
            }
 ///____________



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~DNS LOOKUP , HOLY SHIT THIS IS MESSY BUT IT WORKS!!!!! ~~~~~~~~~~~~~~~~~~~~~~
START:
 printf("\n-[Listen on (DNS)]: ");
string hostname;
struct hostent *he;
struct in_addr **addr_list;
int i;
cin >> hostname;
 printf("Resolving: %s ......\n", hostname.c_str());
he = gethostbyname(hostname.c_str()); //convert string to char array and look it up
if (!he)
  { puts("Host not found");
    goto START; //try again
  }else{
  addr_list = (struct in_addr **)he->h_addr_list;

  for(i = 0; addr_list[i] != NULL; i++) { //cycle thru possible hosts
         printf("Host Found:%s ", inet_ntoa(*addr_list[i]));
         //addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
        addr.sin_addr.S_un.S_addr = inet_addr(inet_ntoa(*addr_list[i])); //SET OUR FUCKING IP TO RESOLVED DNS !
         break;
    }
  }
/// ~~~~~~~~~~~~~~~~~~ DNS DONE ~~~~~~~~~~~~~~~

//__________Set Port_________

      printf("\n-[Listen on (PORT)]: ");
    cin >> PORT;
    addr.sin_port = htons(PORT);


 //  _________ Socket creation and check.   _________
sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
if(sock == INVALID_SOCKET){
        printf("[X]Invalid Socket..\n");
        return 0;

                        }
///_____________


//_____Bind +check on the ip and port so we can start to listen. If we get an error here the port or IP pay already be bided elsewhere_____
iResult = bind(sock,(sockaddr*)&addr,sizeof(sockaddr_in)); //bind throws 1 if error
if( iResult ){
    printf("[X]Bind failed...Already binded on port?\n");
    return 0;
}
///_______________


//_______BEGIN LISTENING + CHECK _____
iResult = listen(sock,SOMAXCONN); //Throws 1 if invalid like bind
if(iResult){
    printf("[X]Failed to listen..\n");
    return 0;
}
//____________

   printf("Listening on %s on port %d\n" ,inet_ntoa(*addr_list[i]),PORT);

//_________________HANDLE CONNECTIONS__________
while(client= accept(sock,0,0)) //There is a break in execution here
{
    if(client == INVALID_SOCKET)
    {
        //Dirty cunt , get out
        printf("[X]Client rejected , bad socket..\n");
        continue;
    }
    _beginthreadex(0,0,ServClient,(void*)&client,0,0); //Begin a new thread of execution passing in client socket and using servclient func, then go back to break to wait for more clients
}
//_____________________

return 0;

} //master main brace

//-------------------Declare ServClient , our smart ass multi thread handler-------------
unsigned int __stdcall ServClient(void *data){


SOCKET* pclient = (SOCKET*)data; //Assign our generic pointer data(casted to a sock pointer ) from func into a socket pointer
SOCKET Client = *pclient; //Derefrence into usable data
 printf("[*]Client Connected\n");

 char databuffer[200]; //Data buffer to get from this particular client

 while(recv(Client,databuffer,200,0)){

    printf("%s \t %d\n" ,databuffer, GetCurrentThreadId()); //Print out whatever our client spits at us , with an ID
                                }
                                return 0;


}

