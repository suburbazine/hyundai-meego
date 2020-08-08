/*!
 *      @date   2011/09/08
 *      @file   dhwr_result.h
 *      @author shchoi
 *
 *      Copyright (c) 2011 by DIOTEK Co.,LTD. All rights reserved.
 */

#ifndef __DHWR_RESULT_OBJECT_H__
#define __DHWR_RESULT_OBJECT_H__ 1

#include "dhwr_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ResultObject ResultObject APIs
 * @ingroup  Main
 * @brief    인식된 결과를 저장하는 ResultObject 관련 함수
 */

/*@{*/

//! Object Handle
typedef const void* DHWRResultObject;
//! Line Handle
typedef const void* DHWRResultLine;
//! Block Handle 
typedef const void* DHWRResultBlock;

//! 블럭이 인식된 상태
typedef enum {
  //! 완성되지 않았음
  kNoneComplete = 0,
  //! 낱자가 완성 되었음
  kCharacterComplete,
  //! 단어가 완성 되었음
  kWordComplete
} DHWRCompleteType;

//!  블럭의 정보를 저장
typedef struct tagDHWRBlockInfo {
  /*! 
   *  @brief 완성 상태
   *  @see DHWRCompleteType 
   */
  int complete;
  //! Stroke의 index(Zero base)
  int stroke;
  //! 인식 점수
  int score;
} DHWRBlockInfo;

/*!
 *  @brief 결과 오브젝트를 생성한다
 *  @return DHWRResultObject
 */
DHWR_API DHWRResultObject DHWRCreateResultObject(void);

/*!
 *  @brief 결과 오브젝트를 제거한다
 *  @param [in] result 결과 오브젝트
 *  @return void
 */
DHWR_API void DHWRDestroyResultObject(DHWRResultObject result);

/*!
 *  @brief 결과 값의 라인 크기를 가져온다
 *  @param [in] result 결과 오브젝트
 *  @return Line Size
 */
DHWR_API unsigned int DHWRGetLineSize(DHWRResultObject result);

/*!
 *  @brief 지정된 인덱스의 라인 결과를 가져온다
 *  @param [in] result 결과 오브젝트
 *  @param [in] index 라인 인덱스
 *  @return DHWRResultLine
 */
DHWR_API DHWRResultLine DHWRGetLine(DHWRResultObject result, const unsigned int index);

/*!
 *  @brief 라인 결과 값의 블럭 크기를 가져온다
 *  @param [in] line 라인 결과 값
 *  @return Block Size
 */
DHWR_API unsigned int DHWRGetBlockSize(DHWRResultLine line);

/*!
 *  @brief 라인 결과값의 지정된 인덱스의 블럭 결과를 가져온다
 *  @param [in] line 라인
 *  @param [in] index 블럭 인덱스
 *  @return DHWRResultBlock
 */
DHWR_API DHWRResultBlock DHWRGetBlock(DHWRResultLine line, const unsigned int index);

/*!
 *  @brief 블럭 결과값의 정보를 얻어온다 (완성여부, 처리된 획 인덱스, 인식 점수)
 *  @param [in] block 블럭 결과 값
 *  @param [out] info 블럭정보
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned int DHWRGetBlockInfo(DHWRResultBlock block, DHWRBlockInfo* info);

/*!
 *  @brief 블럭 결과 값에서 후보 크기를 가져온다.
 *  @param [in] block 블럭 결과 값
 *  @return Candidate Size
 */
DHWR_API unsigned int DHWRGetCandidateSize(DHWRResultBlock block);

/*!
 *  @brief 블럭 결과 값에서 인식 후보를 가져온다.
 *  @param [in] block 블럭 결과 값
 *  @param [in] index 후보 인덱스
 *  @param [out] length 인식후보 문자열 크기
 *  @return 인식 후보 문자열
 */
DHWR_API const unsigned short* DHWRGetCandidate(DHWRResultBlock block,
  const unsigned int index, unsigned int* length);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif // __DHWR_RESULT_OBJECT_H__
