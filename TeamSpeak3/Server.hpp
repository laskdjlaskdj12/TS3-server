//
//  Server.hpp
//  TeamSpeak3
//
//  Created by ace on 2016. 8. 5..
//  Copyright © 2016년 origin. All rights reserved.
//

#ifndef Server_hpp
#define Server_hpp

#ifdef _Windows
#define _CRT_SECURE_NO_WARINGES
#include <Windows.h>
#else
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <iostream>

#include <teamspeak/public_definitions.h>
#include <teamspeak/public_errors.h>
#include <teamspeak/serverlib_publicdefinitions.h>
#include <teamspeak/serverlib.h>
#include "id_io.h"

#define USE_VOICEDATAEVENT

#define MAX_CLIENTS 8

#ifdef _WINDOWS
#define SLEEP(x) Sleep(x)
#else
#define SLEEP(x) usleep(x*1000)
#endif

#ifdef USE_VOICEDATAEVENT
#ifdef _WINDOWS
#include <io.h>
#else  /* Unix compatibility */
#include <unistd.h>
#define _open open
#define _write write
#define _close close
#define _O_CREAT O_CREAT
#define _O_WRONLY O_WRONLY
#define _S_IREAD S_IREAD
#define _S_IWRITE S_IWRITE
#define _O_APPEND O_APPEND
#define _O_BINARY 0
#endif
#include <fcntl.h>
#include <sys/stat.h>
#endif

#define CHECK_ERROR(x) if((error = x) != ERROR_ok) { goto on_error; }


class Talk_Server
{
public:
    Talk_Server();
    static void onClientConnected(uint64 serverID, anyID clientID, uint64 channelID, unsigned int* removeClientError);
    static void onClientDisconnected(uint64 serverID, anyID clientID, uint64 channelID);
    static void onClientMoved(uint64 serverID, anyID clientID, uint64 oldChannelID, uint64 newChannelID);
    static void onChannelCreated(uint64 serverID, anyID invokerClientID, uint64 channelID);
    static void onChannelEdited(uint64 serverID, anyID invokerClientID, uint64 channelID);
    static void onChannelDeleted(uint64 serverID, anyID invokerClientID, uint64 channelID);
    static void onServerTextMessageEvent(uint64 serverID, anyID invokerClientID, const char* textMessage);
    static void onChannelTextMessageEvent(uint64 serverID, anyID invokerClientID, uint64 targetChannelID, const char* textMessage);
    static void onUserLoggingMessageEvent(const char* logMessage, int logLevel, const char* logChannel, uint64 logID, const char* logTime, const char* completeLogString);
#ifdef USE_VOICEDATAEVENT
    static void onVoiceDataEvent(uint64 serverID, anyID clientID, unsigned char* voiceData, unsigned int voiceDataSize, unsigned int frequency);
#endif 
    
#ifdef CUSTOM_PASSWORDS
    static void onClientPasswordEncrypt(uint64 serverID, const char* plaintext, char* encryptedText, int encryptedTextByteSize);
    static unsigned int onCustomServerPasswordCheck(uint64 serverID, const struct ClientMiniExport* client, const char* password);
    static unsigned int onCustomChannelPasswordCheck(uint64 serverID, const struct ClientMiniExport* client, uint64 channelID, const char* password);
#endif
    static void onClientStartTalkingEvent(uint64 serverID, anyID clientID);
    static void onClientStopTalkingEvent(uint64 serverID, anyID clientID);
    static void onAccountingErrorEvent(uint64 serverID, unsigned int errorCode);
    static void onCustomPacketEncryptEvent(char** dataToSend, unsigned int* sizeOfData);
    static void onCustomPacketDecryptEvent(char** dataReceived, unsigned int* dataReceivedSize);
    static void showHelp();
    static void emptyInputBuffer();
    static void showVirtualServers();
    static void showChannels(uint64 serverID);
    static void showClients(uint64 serverID);
    static void createDefaultChannelName(char *name);
    static void enterName(char *name);
    static void createChannel(uint64 serverID, const char *name);
    static void deleteChannel(uint64 serverID);
    static void renameChannel(uint64 serverID);
    static void moveClient(uint64 serverID);
    static uint64 createVirtualServer(const char* name, int port, unsigned int maxClients);
    static uint64 startVirtualServer();
    static void editVirtualServer();
    static void stopVirtualServer();
    static void my_onClientStartTalkingEvent_callback();
    
    ~Talk_Server();
private:
    uint64 serverid;
    anyID clientid;
    uint64 channelid;
    static class Talk_Server *instance;
    
};
class Talk_Server_Excpetion_Handler
{
public:
    virtual void showexception() = 0;
    virtual int getshowerror() = 0;
};
class Talk_Server_Exception:public Talk_Server
{
public:
    Talk_Server_Exception(std::string location, unsigned int exception);
    virtual void showexception();
    virtual int getshowerror();
private:
    std::string exception;
};


#endif /* Server_hpp */
