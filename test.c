#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#define MAX_MACHINE_NUM 1000
#define MAX_MESSAGE_LEN 2048

typedef struct Message
{
    int dataId;
    int machineId;
}Message;

typedef struct Machine
{
    int machineId;
    int messageCount;
    int contribution;
    Message* messageQueue[MAX_MESSAGE_LEN];
}Machine;

Machine** g_Machines = NULL;
int g_MachinesNumber = 0;

//数据存在返回0， 数据不存在返回1；
int isMessageExisted(Machine* targetMachine, int dataId)
{
    for (int i = 0; i < (targetMachine->messageCount); ++i)
    {
        if (targetMachine->messageQueue[i]->dataId == dataId)
        {
            return 0;
        }
    }
    return 1;
}

Message* FindMessage(Machine* targetMachine, int dataId)
{
    for (int i = 0; i < (targetMachine->messageCount); ++i)
    {
        if (targetMachine->messageQueue[i]->dataId == dataId)
        {
            return targetMachine->messageQueue[i];
        }
    }
    return NULL;
}
//迭代的给每个消息传递者都+10；
void AddContribution(int machine)
{
    Machine* currentMachine = g_Machines[machine];
    currentMachine->contribution += 10;
    return;
}
void AddMessage(Machine* targetMachine, int messageSender, int dataId)
{
    int queueCount = targetMachine->messageCount;
    int machineId = targetMachine->machineId;
    targetMachine->messageQueue[queueCount]->dataId = dataId;
    targetMachine->messageQueue[queueCount]->machineId = messageSender;
    targetMachine->messageCount++;
}

void AddContributionToAll(Machine* currentMachine, Message* message){
    //所有消息的转发者
    while (message->machineId != currentMachine->machineId){
        AddContribution(message->machineId);
        currentMachine = g_Machines[message->machineId];
        FindMessage(currentMachine,message->dataId);
    }
    //消息生成者也+10
    AddContribution(message->machineId);
    return;
}

int DataMachinceSystem(int num)
{
    g_Machines = (Machine **)malloc(sizeof(Machine*) * num);
    for (int i = 0; i < num; i ++)
    {
        g_Machines[i] = (Machine *)malloc(sizeof(Machine));
        g_Machines[i][0].machineId = i + 1;
        g_Machines[i][0].messageCount = 0;
        g_Machines[i][0].contribution = 0;
    }
    g_MachinesNumber = num;
    return -1;
}
int TransferData(int machineA, int machineB, int dataId)
{
    Machine* tmpMachineA = g_Machines[machineA];
    Machine* tmpMachineB = g_Machines[machineB];
    Message* tmpMessage = NULL;
    //发送方检查是否有该消息，没有的话加入到MachineA的消息队列
    if (isMessageExisted(tmpMachineA, dataId) == 1)
    {
        AddMessage(tmpMachineA, tmpMachineA->machineId, dataId);
    }
    //接收方检查是否有该消息，没有的话加入到MachineB的消息队列,并进行逻辑运算
    if (isMessageExisted(tmpMachineB, dataId) == 1)
    {
        tmpMessage = FindMessage(tmpMachineA, dataId);
        if (tmpMessage != NULL)
        {
            AddMessage(machineB, tmpMessage->machineId, dataId);
        }
        AddContributionToAll(tmpMachineA,tmpMessage);
        return 1;
    }
    else
    {
        return 0;
    }
}
int TransferDataToAll(int machine, int dataId)
{
    int receivedMachineCount = 0;
    for (int i = 0; i < g_MachinesNumber; ++i)
    {
        if (machine != i)
        {
            receivedMachineCount += TransferData(machine, i, dataId);
        }
    }
    return receivedMachineCount;
}
int QueryContribution(int machine){
    return g_Machines[machine]->contribution;
}
void FreeMachines(){
    free(g_Machines);
    return;

}

int main (){

    char* test_commands[] = {"DataMachineSystem", "transferData", "transferDataToAll", "queryContribution"};
    int test_symbols[][3] ={{3}, {1, 2, 17}, {2, 29}, {2}}; 
    int test_result[4] = {0};
    for (int i = 0; i < 4; ++i){
        int result = 0;
        if (strcmp(test_commands[i], "DataMachineSystem") == 0)
        {
            result = DataMachinceSystem(test_symbols[i][0]);
        }
        else if (strcmp(test_commands[i], "transferData") == 0)
        {
            result = TransferData(test_symbols[i][0], test_symbols[i][1], test_symbols[2]);
        }
        else if (strcmp(test_commands[i], "transferDataToAll") == 0)
        {
            result = TransfeiDataToAll(test_symbols[i][0], test_symbols[i][1]);
        }
        else if (strcmp(test_commands[i], "queryContribution") == 0)
        {
            result = QueryContribution(test_symbols[i][0]);
        }
        test_result[i] = result;
    }
    printf("Hello World");
    return 0;
}