/*
    File:       iAp2AppFrameworkCommon.h
    Package:    LG iAP2 Application Framework
    Author:     Yongil Park (yongil.park@lge.com)
    Copyright © [2014-2016] by LG Electronics Inc.

    This program or software including the accompanying associated documentation
    (“Software”) is the proprietary software of LG Electronics Inc. and or its
    licensors, and may only be used, duplicated, modified or distributed pursuant
    to the terms and conditions of a separate written license agreement between you
    and LG Electronics Inc. (“Authorized License”). Except as set forth in an
    Authorized License, LG Electronics Inc. grants no license (express or implied),
    rights to use, or waiver of any kind with respect to the Software, and LG
    Electronics Inc. expressly reserves all rights in and to the Software and all
    intellectual property therein. If you have no Authorized License, then you have
    no rights to use the Software in any ways, and should immediately notify LG
    Electronics Inc. and discontinue all use of the Software.
*/

#ifndef IAP2APPFRAMEWORKCOMMON_H_
#define IAP2APPFRAMEWORKCOMMON_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

//===========================================================================================================================
//    Definitions
//===========================================================================================================================

typedef unsigned char BOOL; // #LGE_CHANGE fix for StaticAnalysis. yongil.park

#ifndef TRUE
#define TRUE   1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define kIap2NoErr                        0        //! No error occurred.
//#define kIap2InProgressErr                1        //! Operation in progress.

// Generic error codes are in the range -6700 to -6779.
#define kIap2GenericErrBase            -6700    //! Starting error code for all generic errors.

#define kIap2UnknownErr                -6700    //! Unknown error occurred.
#define kIap2OptionErr                -6701    //! Option was not acceptable.
#define kIap2SelectorErr                -6702    //! Selector passed in is invalid or unknown.
#define kIap2ExecutionStateErr        -6703    //! Call made in the wrong execution state (e.g. called at interrupt time).
#define kIap2PathErr                    -6704    //! Path is invalid, too long, or otherwise not usable.
#define kIap2ParamErr                    -6705    //! Parameter is incorrect, missing, or not appropriate.
#define kIap2UserRequiredErr            -6706    //! User interaction is required.
#define kIap2CommandErr                -6707    //! Command invalid or not supported.
#define kIap2IDErr                    -6708    //! Unknown, invalid, or inappropriate identifier.
#define kIap2StateErr                    -6709    //! Not in appropriate state to perform operation.
#define kIap2RangeErr                    -6710    //! Index is out of range or not valid.
#define kIap2RequestErr                -6711    //! Request was improperly formed or not appropriate.
#define kIap2ResponseErr                -6712    //! Response was incorrect or out of sequence.
#define kIap2ChecksumErr                -6713    //! Checksum does not match the actual data.
#define kIap2NotHandledErr            -6714    //! Operation was not handled (or not handled completely).
#define kIap2VersionErr                -6715    //! Version is not correct or not compatible.
#define kIap2SignatureErr                -6716    //! Signature did not match what was expected.
#define kIap2FormatErr                -6717    //! Unknown, invalid, or inappropriate file/data format.
#define kIap2NotInitializedErr        -6718    //! Action request before needed services were initialized.
#define kIap2AlreadyInitializedErr    -6719    //! Attempt made to initialize when already initialized.
#define kIap2NotInUseErr                -6720    //! Object not in use (e.g. cannot abort if not already in use).
#define kIap2AlreadyInUseErr            -6721    //! Object is in use (e.g. cannot reuse active param blocks).
#define kIap2TimeoutErr                -6722    //! Timeout occurred.
#define kIap2CanceledErr                -6723    //! Operation canceled (successful cancel).
#define kIap2AlreadyCanceledErr        -6724    //! Operation has already been canceled.
#define kIap2CannotCancelErr            -6725    //! Operation could not be canceled (maybe already done or invalid).
#define kIap2DeletedErr                -6726    //! Object has already been deleted.
#define kIap2NotFoundErr                -6727    //! Something was not found.
#define kIap2NoMemoryErr                -6728    //! Not enough memory was available to perform the operation.
#define kIap2NoResourcesErr            -6729    //! Resources unavailable to perform the operation.
#define kIap2DuplicateErr                -6730    //! Duplicate found or something is a duplicate.
#define kIap2ImmutableErr                -6731    //! Entity is not changeable.
#define kIap2UnsupportedDataErr        -6732    //! Data is unknown or not supported.
#define kIap2IntegrityErr                -6733    //! Data is corrupt.
#define kIap2IncompatibleErr            -6734    //! Data is not compatible or it is in an incompatible format.
#define kIap2UnsupportedErr            -6735    //! Feature or option is not supported.
#define kIap2UnexpectedErr            -6736    //! Error occurred that was not expected.
#define kIap2ValueErr                    -6737    //! Value is not appropriate.
#define kIap2NotReadableErr            -6738    //! Could not read or reading is not allowed.
#define kIap2NotWritableErr            -6739    //! Could not write or writing is not allowed.
#define    kIap2BadReferenceErr            -6740    //! An invalid or inappropriate reference was specified.
#define    kIap2FlagErr                    -6741    //! An invalid, inappropriate, or unsupported flag was specified.
#define    kIap2MalformedErr                -6742    //! Something was not formed correctly.
#define    kIap2SizeErr                    -6743    //! Size was too big, too small, or not appropriate.
#define    kIap2NameErr                    -6744    //! Name was not correct, allowed, or appropriate.
#define    kIap2NotPreparedErr            -6745    //! Device or service is not ready.
#define    kIap2ReadErr                    -6746    //! Could not read.
#define    kIap2WriteErr                    -6747    //! Could not write.
#define    kIap2MismatchErr                -6748    //! Something does not match.
#define    kIap2DateErr                    -6749    //! Date is invalid or out-of-range.
#define    kIap2UnderrunErr                -6750    //! Less data than expected.
#define    kIap2OverrunErr                -6751    //! More data than expected.
#define    kIap2EndingErr                -6752    //! Connection, session, or something is ending.
#define    kIap2ConnectionErr            -6753    //! Connection failed or could not be established.
#define    kIap2AuthenticationErr        -6754    //! Authentication failed or is not supported.
#define    kIap2OpenErr                    -6755    //! Could not open file, pipe, device, etc.
#define    kIap2TypeErr                    -6756    //! Incorrect or incompatible type (e.g. file, data, etc.).
#define    kIap2SkipErr                    -6757    //! Items should be or was skipped.
#define    kIap2NoAckErr                    -6758    //! No acknowledge.
#define    kIap2CollisionErr                -6759    //! Collision occurred (e.g. two on bus at same time).
#define    kIap2BackoffErr                -6760    //! Backoff in progress and operation intentionally failed.
#define    kIap2AddressErr                -6761    //! Bad address or no acknowledge of address.
#define    kIap2InternalErr                -6762    //! An error internal to the implementation occurred.
#define    kIap2NoSpaceErr                -6763    //! Not enough space to perform operation.
#define    kIap2CountErr                    -6764    //! Count is incorrect.
#define    kIap2EndOfDataErr                -6765    //! Reached the end of the data (e.g. recv returned 0).
#define    kIap2WouldBlockErr            -6766    //! Would need to block to continue (e.g. non-blocking read/write).
#define    kIap2LookErr                    -6767    //! Special case that needs to be looked at (e.g. interleaved data).
#define    kIap2SecurityRequiredErr        -6768    //! Security is required for the operation (e.g. must use encryption).
#define    kIap2OrderErr                    -6769    //! Order is incorrect.
#define    kIap2UpgradeErr                -6770    //! Must upgrade.
#define kIap2AsyncNoErr                -6771    //! Async operation successfully started and is now in progress.
#define kIap2DeprecatedErr            -6772    //! Operation or data is deprecated.
#define kIap2PermissionErr            -6773    //! Permission denied.

#define kIap2GenericErrorEnd            -6779    //! Last generic error code (inclusive)


typedef struct iAp2AppPrivate* iAp2AppRef;

//===========================================================================================================================
//    Declarations
//===========================================================================================================================

#ifdef __cplusplus
}
#endif

#endif /* IAP2APPFRAMEWORKCOMMON_H_ */

