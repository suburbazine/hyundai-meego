/*!
 *      @date   2011/09/08
 *      @file   dhwr_ink.h
 *      @author shchoi
 *
 *      Copyright (c) 2011 by DIOTEK Co.,LTD. All rights reserved.
 */

#ifndef __DHWR_INK_OBJECT_H__
#define __DHWR_INK_OBJECT_H__ 1

#include "dhwr_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup  InkObject InkObject APIs
 * @ingroup   Main
 * @brief     좌표를 관리하는 InkObject관련 함수
 */

/*@{*/

//! Ink Type 
typedef enum {
  //! Pen Type
  kInkPen = 0,
  //! Erase Type
  kInkErase,
} DHWRInkType;

//! Ink Object Handle
typedef const void* DHWRInkObject;

/*!
 *  @brief 잉크 오브젝트를 생성한다
 *  @return DHWRInkObject
 */
DHWR_API DHWRInkObject DHWRCreateInkObject(void);

/*!
 *  @brief 잉크 오브젝트를 제거한다
 *  @param [in] ink 잉크 오브젝트
 *  @return void
 */
DHWR_API void DHWRDestroyInkObject(DHWRInkObject ink);

/*!
 *  @brief 잉크의 종류를 설정한다 (펜, 지우개)
 *  @param [in] ink 잉크 오브젝트
 *  @param [in] type 펜 타입
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned long DHWRSetInkType(DHWRInkObject ink, const DHWRInkType type);

/*!
 *  @brief 지우개획의 너비를 설정한다. 설정 이후의 지우개는 설정한 두께로 동작함
 *  @param [in] ink 잉크 오브젝트
 *  @param [in] thick 지우개 두께 값(반지름)
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned long DHWRSetEraseThick(DHWRInkObject ink, const unsigned int thick);

/*!
 *  @brief 인식엔진에서 인식할 터치 좌표(잉크 좌표)를 추가함
 *  @param [in] ink 잉크 오브젝트
 *  @param [in] x x좌표
 *  @param [in] y y좌표
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned long DHWRAddPoint(DHWRInkObject ink, const int x, const int y);

/*!
 *  @brief TouchUp 이벤트에 의해 한 획의 입력을 마쳤을 때 호출함
 *  @param [in] ink 잉크 오브젝트
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned long DHWREndStroke(DHWRInkObject ink);

/*!
 *  @brief 입력한 좌표 데이터를 초기화함
 *  @param [in] ink 잉크 오브젝트
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned long DHWRInkClear(DHWRInkObject ink);

/*!
 *  @brief 입력된 잉크 인덱스 값에 해당하는 잉크 좌표값을 받아옴
 *  @param [in] ink 잉크 오브젝트
 *  @param [in] index 받아오고자 하는 잉크의 인덱스
 *  @param [out] x 해당 잉크의 x좌표값
 *  @param [out] y 해당 잉크의 y좌표값
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned long DHWRGetInkPoint(DHWRInkObject ink, const int index,
  int* x, int* y);

/*!
 *  @brief 현재까지 입력된 잉크의 카운트
 *  @param [in] ink 잉크 오브젝트
 *  @param [out] count 잉크 카운트
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned long DHWRGetInkCount(DHWRInkObject ink, int* count);

/*!
 *  @brief 현재 잉크로 부터 선택적으로 스트로크를 취해 새로운 잉크 생성
 *  @param [in] obj 잉크 오브젝트
 *  @param [in] start 복사할 시작 스트로크 인덱스
 *  @param [in] end 복사할 마지막 스트로크 인덱스
 *  @param [in] 복사할 라인 인덱스
 *  @return DHWRInkObject
 */
DHWR_API DHWRInkObject DHWRCopyInk(DHWRInkObject obj, const int start, const int end, const int line);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif // __DHWR_INK_OBJECT_H__
