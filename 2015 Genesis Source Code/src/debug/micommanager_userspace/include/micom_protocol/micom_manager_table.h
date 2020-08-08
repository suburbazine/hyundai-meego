/*
 * micom_manager_table.h
 *
 *  Created on: 2011. 9. 3.
 *      Author: yongwoo1.park
 */

#ifndef MICOM_MANAGER_TABLE_H_
#define MICOM_MANAGER_TABLE_H_

#include <QObject>

typedef enum E_CmdDirection{
	nRequest,
	nResponse
} T_CmdDirection;


typedef struct{
	quint16				command;
	quint16				data_size;
	quint8				bus_index;
	quint8				cmd_name_index;
	bool				need_response;
} S_CmdTable;

typedef struct{
	QString				service_name;
	QString				interface_name;
	QString				method_name;
} S_BusDefinition;

extern const S_CmdTable 			g_CmdTable[];
extern const S_BusDefinition		g_BusDefinition[];
extern const char *					g_CmdNameTable[];

#define GET_COMMAND_DIRECTION(cmd)	\
		(cmd & 0x8000)? nResponse:nRequest;

#define IS_EVENT_MESSAGE(cmd) \
		(cmd & 0x0300) == 0x0300

#define GET_SERVICE_NAME(index) \
		(qPrintable( g_BusDefinition[g_CmdTable[index].bus_index].service_name))

#define GET_METHOD_NAME(index) \
		(qPrintable( g_BusDefinition[g_CmdTable[index].bus_index].method_name))

#define GET_INTERFACE_NAME(index) \
		(qPrintable( g_BusDefinition[g_CmdTable[index].bus_index].interface_name))

#define GET_COMMAND_NAME(index) \
		( g_CmdNameTable[g_CmdTable[index].cmd_name_index])

#define GET_ARG_COUNT(index) \
		(g_CmdTable[index].data_size)

#define NEED_RESPONSE(index) \
		(g_CmdTable[index].need_response)

#endif /* MICOM_MANAGER_TABLE_H_ */
