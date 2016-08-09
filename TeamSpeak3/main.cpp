//
//  main.cpp
//  TeamSpeak3
//
//  Created by ace on 2016. 8. 5..
//  Copyright © 2016년 origin. All rights reserved.
//

#include <iostream>
#include "Server.hpp"
void mac(int a)
{
    int mac1;
    mac1+=a;
}
int main(int argc, const char * argv[]) {
    
    struct ServerLibFunctions func;
    unsigned int error;
    uint64 serverID;
    uint64* ids;
    char* version;
    unsigned int Switch = 0;
    unsigned int unknownInput = 0;
    
    
    try {
        if(argc <2){
            std::cout<<"argument parameter is missing use ./TeamSepak3 -h"<<std::endl;
            std::cout<<"./TeamSpeak3 <Logging_file_loaction> <ServerName> <port> <MaxClient>";
            return 0;
        }
        
        memset(&func, 0, sizeof(struct ServerLibFunctions));
        
        Talk_Server *talk = new Talk_Server;
        typedef struct test
        {
          // typedef void (*)vall(int) = 0;
           typedef void (*valloc12)(int) = 0;
            
        }test12;
        test12 test123;
        test123.valloc12 = mac;
        
        func.
        func.onAccountingErrorEvent = talk->onAccountingErrorEvent;
        func.onVoiceDataEvent           = talk->onVoiceDataEvent;
        func.onClientStartTalkingEvent  = talk->onClientStartTalkingEvent;
        func.onClientStopTalkingEvent = talk->onClientStopTalkingEvent;
        func.onClientConnected = talk->onClientConnected;
        func.onClientDisconnected = talk->onClientDisconnected;
        func.onClientMoved = talk->onClientMoved;
        func.onChannelCreated = talk->onChannelCreated;
        func.onChannelEdited = talk->onChannelEdited;
        func.onChannelDeleted = talk->onChannelDeleted;
        func.onServerTextMessageEvent = talk->onServerTextMessageEvent;
        func.onChannelTextMessageEvent  = talk->onChannelTextMessageEvent;
        func.onUserLoggingMessageEvent  = talk->onUserLoggingMessageEvent;
        func.onCustomPacketEncryptEvent = NULL;  // Not used by your application
        func.onCustomPacketDecryptEvent = NULL;
        
        
        if((error =ts3server_initServerLib(&func, LogType_CONSOLE|LogType_FILE|LogType_USERLOGGING, argv[2] = NULL))!= ERROR_ok){
            char* errormessage;
            if(ts3server_getGlobalErrorMessage(error, &errormessage)==ERROR_ok) std::cout<<"ts3Server_initServerLib "<<errormessage<<std::endl;
            throw Talk_Server_Exception("ts3server_initServerLib",error);
        }
        
        std::cout<<"TeamSpeakServer is running..."<<std::endl;
        
        if((error = ts3server_getServerLibVersion(&version))!=ERROR_ok) throw Talk_Server_Exception("ts3server_getServerLibVersion", error);
        
        std::cout<<"Server Lib Verseion = "<<version<<" "<<std::endl;
        
        serverID = talk->createVirtualServer(argv[3], atoi(argv[4]), atoi(argv[5]));
        
      /*while(!Switch)
        {
            
        }*/
        
        
        //setting message when client is connecting server
        if((error = ts3server_setVirtualServerVariableAsString(serverID, VIRTUALSERVER_WELCOMEMESSAGE, "Hello TeamSpeak 3")) != ERROR_ok) {
            throw Talk_Server_Exception("Error setting server welcome messag", error);
        }
        
        //setting server password string
        if((error = ts3server_setVirtualServerVariableAsString(serverID,VIRTUALSERVER_PASSWORD,"secret"))!=ERROR_ok){
            throw Talk_Server_Exception("ts3server_setVirtualServerVariableAsString", error);
        }
        
        //flushing this setting
        if((error = ts3server_flushVirtualServerVariable(serverID)) !=ERROR_ok)
        {
            throw Talk_Server_Exception("ts3server_flushVirtualServerVariable", error);
        }
        
        std::cout<<"====Command Line ===="<<std::endl;
        talk->showHelp();
        
        Switch == 1 ? Switch=0:Switch=1;
        while(!Switch)
        {
            int c;
            if(unknownInput == 0) {
                printf("\nEnter Command (h for help)> ");
            }
            unknownInput = 0;
            c = getchar();
            std::cin.ignore();
            switch (c) {
                case 'q':
                    printf("\nShutting down server...\n");
                    Switch =1;
                    break;
                case 'h':
                    talk->showHelp();
                    break;
                case 'v':
                    talk->showVirtualServers();
                    break;
                case 'c':
                    talk->showChannels(serverID);
                    break;
                case 'l':
                    talk->showClients(serverID);
                    break;
                case 'n':
                {
                    char name[BUFSIZ];
                    talk->createDefaultChannelName(name);
                    talk->createChannel(serverID, name);
                    break;
                }
                case 'N':
                {
                    char name[BUFSIZ];
                    talk->emptyInputBuffer();
                    talk->enterName(name);
                    talk->createChannel(serverID, name);
                    break;
                }
                case 'd':
                    talk->deleteChannel(serverID);
                    break;
                case 'r':
                    talk->renameChannel(serverID);
                    break;
                case 'm':
                    talk->moveClient(serverID);
                    break;
                case 'C':
                    talk->startVirtualServer();
                    break;
                case 'E':
                    talk->editVirtualServer();
                    break;
                case 'S':
                    talk->stopVirtualServer();
                    break;
                default:
                    unknownInput = 1;
            }
            sleep(50);
        }
        if((error = ts3server_getVirtualServerList(&ids))!=ERROR_ok) throw Talk_Server_Exception("ts3server_getVirtualServerList", error);
        else
        {
            for(int i = 0; ids[i]; i++)
            {
                if((error = ts3server_stopVirtualServer(ids[i])) != ERROR_ok)
                {
                    std::cout<<"Error : Stopping virtualserver : "<<error<<std::endl;
                    break;
                }
            }
        }
        if((error = ts3server_destroyServerLib())!=ERROR_ok) throw Talk_Server_Exception("ts3server_destroyServerLib", error);
    } catch (Talk_Server_Excpetion_Handler &e) {
        e.showexception();
        return e.getshowerror();
    }
    
    ts3server_freeMemory(version);
    std::cout<<"Stopping Virtual Server...."<<std::endl;
    
}