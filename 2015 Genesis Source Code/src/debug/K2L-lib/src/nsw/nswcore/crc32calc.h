/*=== K2L GmbH ===============================================================*/
/**
\file
            This file contains the implementation of the \ref CRC32
            functionality.
\note       Content of note. If no note is necessary, remove the line. 

\par        COPYRIGHT (c) 2000-2011 by K2L GmbH
            All rights reserved.

\par        File History  
\code      
Date        Author Scope
--------------------------------------------------------------------------------
2011-10-18  K2L Initial version.
\endcode */

/*=============================================================================
Copyright © 2011 K2L GmbH (“K2L”). All rights reserved.
Please make sure that all information within a document marked as ‘Confidential’
or ‘Restricted Access’ is handled solely in accordance with the agreement
pursuant to which it is provided, and is not reproduced or disclosed to others
without the prior written consent of K2L. The confidential ranking of a document
can be found in the footer of every page. This document supersedes and replaces
all information previously supplied. The technical information in this document
loses its validity with the next edition. Although the information is believed
to be accurate, no responsibility is assumed for inaccuracies. Specifications
and other documents mentioned in this document are subject to change without
notice. K2L reserves the right to make changes to this document and to the
products at any time without notice. Neither the provision of this information
nor the sale of the described products conveys any licenses under any patent
rights or other intellectual property rights of K2L or others. The products
may contain design defects or errors known as anomalies, including but not
necessarily limited to any which may be identified in this document, which may
cause the product to deviate from published descriptions. Anomalies are
described in errata sheets available upon request. K2L products are not
designed, intended, authorized or warranted for use in any life support or
other application where product failure could cause or contribute to personal
injury or severe property damage. Any and all such uses without prior written
approval of an officer of K2L will be fully at your own risk. The K2L logo is
a trademark of K2L. Other names mentioned may be trademarks of their respective
holders.

K2L disclaims and excludes any and all warranties, including without limitation
any and all implied warranties of merchantability, fitness for a particular
purpose, title, and against infringement and the like, and any and all
warranties arising from any course of dealing or usage of trade. In no event
shall K2L be liable for any direct, incidental, indirect, special, punitive,
or consequential damages; or for lost data, profits, savings or revenues of
any kind; regardless of the form of action, whether based on contract; tort;
negligence of K2L or others; strict liability; breach of warranty; or
otherwise; whether or not any remedy of buyer is held to have failed of its
essential purpose, and whether or not K2L has been advised of the possibility
of such damages.
===HEADER END================================================================*/

#ifndef _CRC32CALC_H_
#define _CRC32CALC_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>


void crc32Init ();
uint32_t crc32Calc (const uint8_t *block, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif /* _CRC32CALC_H_ */
