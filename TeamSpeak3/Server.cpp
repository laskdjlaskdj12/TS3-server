//
//  Server.cpp
//  TeamSpeak3
//
//  Created by ace on 2016. 8. 5..
//  Copyright © 2016년 origin. All rights reserved.
//

#include "Server.hpp"


/*void Talk_Server::onCustomPacketEncryptEvent(char** dataToSend, unsigned int* sizeOfData)
{
    
}*/
 
//void Talk_Server::onCustomPacketDecryptEvent(char** dataReceived, unsigned int* dataReceivedSize)
void Talk_Server::showHelp()
{
    std::cout<<
}
void Talk_Server::emptyInputBuffer();
void Talk_Server::showVirtualServers();
void Talk_Server::showChannels(uint64 serverID);
void Talk_Server::showClients(uint64 serverID);
void Talk_Server::createDefaultChannelName(char *name);
void Talk_Server::enterName(char *name);
void Talk_Server::createChannel(uint64 serverID, const char *name);
void Talk_Server::deleteChannel(uint64 serverID);
void Talk_Server::renameChannel(uint64 serverID);
void Talk_Server::moveClient(uint64 serverID);
uint64 Talk_Server::createVirtualServer(const char* name, int port, unsigned int maxClients)
{
    uint64 serverid;
    if((error == ts3server_createVirtualServer(port, INADDR_ANY, name, <#const char *serverKeyPair#>, maxClients, &serverid)))
        }
uint64 Talk_Server::startVirtualServer();
void Talk_Server::editVirtualServer();
void Talk_Server::stopVirtualServer();
void Talk_Server::my_onClientStartTalkingEvent_callback();
virtual void Talk_Server::showexception();

Talk_Server::~Talk_Server();*/


Talk_Server::Talk_Server()
{
    
}
void Talk_Server::onClientConnected(uint64 serverID, anyID clientID, uint64 channelID, unsigned int* removeClientError)
{
    char* ClientName;
    unsigned int error;
    
    if((error = ts3server_getClientVariableAsString(serverID, clientID, CLIENT_NICKNAME, &ClientName))!=ERROR_ok)
    {
        char* errormessage;
        if(ts3server_getGlobalErrorMessage(error,&errormessage) == ERROR_ok)
        {
            std::cout<<"Error querying client nickname: "<<errormessage<<std::endl;
            ts3server_freeMemory(errormessage);
        }
        return;
    }
    std::cout<<"Client "<<ClientName<<" joined channel "<<(unsigned long long) channelID<<"on virtual server "<<(unsigned long long) serverID<<std::endl;
    
    
    /* Example: Kick clients with nickname "BlockMe from server */
    if(!strcmp(ClientName, "BlockMe")) {
        printf("Blocking bad client!\n");
        *removeClientError = ERROR_client_not_logged_in;  /* Give a reason */
    }
}

void Talk_Server::onClientDisconnected(uint64 serverID, anyID clientID, uint64 channelID){
    printf("Client %u left channel %llu on virtual server %llu\n", clientID, (unsigned long long)channelID, (unsigned long long)serverID);

}

void Talk_Server::onClientMoved(uint64 serverID, anyID clientID, uint64 oldChannelID, uint64 newChannelID){
    printf("Client %u moved from channel %llu to channel %llu on virtual server %llu\n", clientID, (unsigned long long)oldChannelID, (unsigned long long)newChannelID, (unsigned long long)serverID);
}

void Talk_Server::onChannelCreated(uint64 serverID, anyID invokerClientID, uint64 channelID){
    printf("Channel %llu created by %u on virtual server %llu\n", (unsigned long long)channelID, invokerClientID, (unsigned long long)serverID);
}

void Talk_Server::onChannelEdited(uint64 serverID, anyID invokerClientID, uint64 channelID){
    printf("Channel %llu edited by %u on virtual server %llu\n", (unsigned long long)channelID, invokerClientID, (unsigned long long)serverID);
}


void Talk_Server::onChannelDeleted(uint64 serverID, anyID invokerClientID, uint64 channelID){
    printf("Channel %llu deleted by %u on virtual server %llu\n", (unsigned long long)channelID, invokerClientID, (unsigned long long)serverID);
}

void Talk_Server::onServerTextMessageEvent(uint64 serverID, anyID invokerClientID, const char* textMessage)
{
    char* invokerNickname;
    unsigned int error;
    
    // Get invoker nickname
    if((error = ts3server_getClientVariableAsString(serverID, invokerClientID, CLIENT_NICKNAME, &invokerNickname)) != ERROR_ok) {
        printf("Error getting client nickname: %d\n", error);
        return;
    }
    
    std::cout<<"["<<invokerNickname<<"] : "<<textMessage<<std::endl;
    
    ts3server_freeMemory(invokerNickname);   //Release previously allocated memory
}
void Talk_Server::onChannelTextMessageEvent(uint64 serverID, anyID invokerClientID, uint64 targetChannelID, const char* textMessage)
{
    char* channelName;
    char* inVokeNickname;
    unsigned int error;
    
    if((error = ts3server_getClientVariableAsString(serverID, targetChannelID, ClientProperties::CLIENT_NICKNAME, &inVokeNickname))!=ERROR_ok){
        throw Talk_Server_Exception("Talk_Server::onChannelTextMessageEvent::ts3server_getClientVariableAsString", error);
    }
    
    if((error = ts3server_getChannelVariableAsString(serverID, targetChannelID, ChannelProperties::CHANNEL_NAME, &channelName))!=ERROR_ok){
        throw Talk_Server_Exception("Talk_Server::onChannelTextMessageEvent::ts3server_getChannelVariableAsString", error);
    }
    std::cout<<"["<<channelName<<"] 채널 ["<<inVokeNickname<<"] : "<<textMessage<<std::endl;
    ts3server_freeMemory(channelName);
    ts3server_freeMemory(inVokeNickname);
    
}
void Talk_Server::onUserLoggingMessageEvent(const char* logMessage, int logLevel, const char* logChannel, uint64 logID, const char* logTime, const char* completeLogString)
{
    if(logLevel == LogLevel_CRITICAL){
        std::cout<<"[Error] : "<<logMessage<<"[Channel]"<<logChannel<<std::endl;
        exit(1);
    }
}
#ifdef USE_VOICEDATAEVENT
void Talk_Server::onVoiceDataEvent(uint64 serverID, anyID clientID, unsigned char* voiceData, unsigned int voiceDataSize, unsigned int frequency)
{
    int fd;
    unsigned int error;
    char* name;
    
    if((error = ts3server_getClientVariableAsString(serverID,clientID, ClientProperties::CLIENT_NICKNAME, &name))!=ERROR_ok)
    {
        char* errormsg;
        ts3server_getGlobalErrorMessage(error, &errormsg);
        std::cout<<errormsg<<" : "<<error<<std::endl;
        ts3server_freeMemory(errormsg);
    }
    fd = open(name, _O_CREAT|_O_APPEND|_O_BINARY|_O_WRONLY,_S_IREAD|_S_IWRITE);
    if(fd == -1)
    {
        ts3server_freeMemory(name);
        throw Talk_Server_Exception("onVoiceDataEvent --> fd", -1);
    }
    _write(fd,voiceData,voiceDataSize);
    _close(fd);
    
    ts3server_freeMemory(name);
}
#endif
#ifdef CUSTOM_PASSWORDS
void Talk_Server::onClientPasswordEncrypt(uint64 serverID, const char* plaintext, char* encryptedText, int encryptedTextByteSize);
unsigned int Talk_Server::onCustomServerPasswordCheck(uint64 serverID, const struct ClientMiniExport* client, const char* password);
unsigned int Talk_Server::onCustomChannelPasswordCheck(uint64 serverID, const struct ClientMiniExport* client, uint64 channelID, const char* password);
#endif
void Talk_Server::onClientStartTalkingEvent(uint64 serverID, anyID clientID)
{
    std::cout<<"serverID : "<<serverID <<"is calling to client ID : "<<clientID<<std::endl;
    
}
void Talk_Server::onClientStopTalkingEvent(uint64 serverID, anyID clientID)
{
    std::cout<<"serverID : "<<serverID <<"is calling to client ID : "<<clientID;
}
void Talk_Server::onAccountingErrorEvent(uint64 serverID, unsigned int errorCode)
{
    uint64 error;
    char* message;
    
    if ((error = (ts3server_getGlobalErrorMessage(errorCode, &message)))!= ERROR_ok)
    {
        throw Talk_Server_Exception(message,errorCode);
    }
    std::cout<<"["<<serverID<<"]"<<" = "<<message<<std::endl;
    ts3server_freeMemory(message);
}

