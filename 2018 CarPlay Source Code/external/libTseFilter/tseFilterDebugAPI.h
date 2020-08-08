#ifndef FILTER_DEBUG_API_H_
#define FILTER_DEBUG_API_H_

/*
 *    @file        <tseFilterDebugAPI.h>
 *    @brief       Filter Debug API header of LG Electronics Preprocessor
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
/* --------------------------------------------------------------- */

#include "tseFilterDebug.h"
#include "tseFilter_priv.h"

int setupDebugMode(FilterContext *ctx);
void unsetDebugMode(FilterContext *ctx);

/* --------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif /* C++ */

#endif /* FILTER_DEBUG_API_H_ */
