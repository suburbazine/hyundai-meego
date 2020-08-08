#ifndef __LLDSTUB_H
#define __LLDSTUB_H

#include <stdint.h>

extern "C"
{
#include <mostns.h>
}
#include <IMAC.h>
#include <LLD.h>

typedef uint32_t LLDStubResult_t;
#define LLDSTUB_RESULT_OK		((LLDStubResult_t)0)
#define LLDSTUB_RESULT_FAIL		((LLDStubResult_t)0x80000000UL)

LLDStubResult_t LLDStubInitialize( IMAC * pMAC,	struct TNetServicesConfig::TLldConfig *lld_cfg_ptr );
void LLDStubTerminate();
void LLDStubServiceInterrupt();
void LLDStubInterruptDone();
eLLDInterface LLDStubGetInterfaceType();
// TODO: Maybe it is a good idea to add a query function for max packet size. This is one of two
// differences between MLB and I2C, which affects the upper layers.
// Currently, NetServices define this as a #define constant, which is uncomfortable, since the upper layer
// cannot be fully agnostic to the type of INIC interface.
// The second difference is, that in the MLB case, the Async sockets have to be maintained by NSW/K2LMOSTHigh
// which is not required in the I2C case. Another approach could be to move this down to LLDStub. This would
// better meet the idea of LLDStub, which is to encapsulate the differences of the different INIC interfaces.
// It might be sufficient to simply add a query function here, giving the type of interface (I2C, MLB).
// But there is a number of INIC interfaces out there, where we do not know, which additional requirements
// for the upper layers are imposed.


#endif
