#ifndef __TSE_H__
#define __TSE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define TSE_MAX_MEM_BUF_CNT 2   /**< Maximum number of memory buffers provided to Create */
#define TSE_MAX_AUDIO_CNT   4   /**< Maximum number of audio channels    */
#define TSE_MAX_CONFIG_DATA_CNT 3     /**< Maximum number of config data buffers        */
#define TSE_MAX_STRING_SIZE   200     /**< Maximum length of a string in module */
#define TSE_PARAM_SENDEQ_LEN   15     /**< Number of send parametric EQ values (5 nodes * 3 values per node)                */
#define TSE_PARAM_RECVEQ_LEN   15     /**< Number of receive parametric EQ values (5 nodes * 3 values per node)                */

#define ttseDataID            int
/*--------------------------------------------------------------------------------------*/
/*      Define for get/set parameters                                                   */
/*--------------------------------------------------------------------------------------*/

/*
	RO - Read only
	RW - parameter can be written & read anytime
	WO - parameter can only be written after initialization / never read 
	WB - parameter can only be written before initialization / read anytime
	WA - parameter can only be written after initialization / read anytime
*/
#define TSE_GENERAL_START     0
 
#define TSE_TSF_INFO_CAR_MANUFACTURER	1 /* RO TSF file information - car manufacturer        (Int32)*/
#define TSE_TSF_INFO_CAR_MODEL			2 /* RO TSF file information - car model               (Int32)*/
#define TSE_TSF_INFO_TUNING_DATE		3 /* RO TSF file information - car tuning date         (Int32)*/

#define TSE_SEND_IN_SIGNAL_NUM          5 /* WB Number of Send In signals    [MIC IN]            (UInt16) */
#define TSE_RECEIVE_IN_SIGNAL_NUM       6 /* WB Number of Recv In signals    [RECV IN]           (UInt16) */
#define TSE_REF_IN_SIGNAL_NUM           7 /* WB Number of Ref In signals    [Reserved]           (UInt16) */
#define TSE_MICOUT_NUM         8 /* RO count of microphone output channels               (UInt16)*/
#define TSE_RECVOUT_NUM        9 /* RO count of receive signal output channels           (UInt16)*/

#define TSE_OPERATION_MODE     14 /* RW operation mode / enum                             (Int32) */
#define TSE_MODULES           15 /* WB switch modules                                     (Int32)*/

#define TSE_STEREO_TYPE        18 /* WB Stereo Type                                       (UInt16)*/

#define TSE_MICOUT_VOLUME   19 /* WB MIC Out volume                                 (UInt32)*/
#define TSE_MICOUT_MUTE     20 /* WB MIC Out mute                                   (UInt32)*/
//#define TSE_MICPROCESSED_MUTE_SWITCH 27 /*  Mic Processed Mute Switch                  (Float32) */

#define TSE_FRAME_SHIFT        30      /* WB input frame shift                                (UInt16)*/
#define TSE_SAMPLERATE        31      /* WB input sample rate                                (UInt16)*/
#define TSE_SYSTEM_DELAY       32      /* WB input system delay                               (UInt16)*/
#define TSE_INITIAL_FRAME_SKIP  33      /* WB initial skip frame number                        (UInt32)*/


#define TSE_VERSION_NUMBER     34      /* RO TSE Version number                               (Int32[4])*/
#define TSE_TSF_INFO_STR        35      /* RO TSF header string                                (Char[max 64])*/
#define TSE_TSF_COUNT            36      /* RO count of loaded TSF files                        (UInt16)*/
#define TSE_PASS_THROUGH_MODE   37  
#define TSE_TSF_PATH_STR        38      /* RO TSF path str                                     (Char[])*/
#define TSE_FILTER_LOG         39      /* RO Log string from filter                           (Char[])*/
#define TSE_TSF_CHECKSUM       40      /* RO TSF checksum                           (Int32)*/

#define TSE_GENERAL_END     48

#define TSE_WNR_START     49
//[EQ]
#define TSE_SEND_EQ_NODE_FREQ_0			50		//# Hz, < SAMPLE_RATE/2
#define TSE_SEND_EQ_NODE_GAIN_0			51		//# > 1.0
#define TSE_SEND_EQ_NODE_FREQ_1			52
#define TSE_SEND_EQ_NODE_GAIN_1			53
#define TSE_SEND_EQ_NODE_FREQ_2			54
#define TSE_SEND_EQ_NODE_GAIN_2			55
#define TSE_SEND_EQ_NODE_FREQ_3			56
#define TSE_SEND_EQ_NODE_GAIN_3			57
#define TSE_SEND_EQ_NODE_FREQ_4			58
#define TSE_SEND_EQ_NODE_GAIN_4			59
#define TSE_SEND_EQ_NODE_FREQ_5			60
#define TSE_SEND_EQ_NODE_GAIN_5			61
#define TSE_SEND_EQ_NODE_FREQ_6			62
#define TSE_SEND_EQ_NODE_GAIN_6			63
#define TSE_SEND_EQ_NODE_FREQ_7			64
#define TSE_SEND_EQ_NODE_GAIN_7			65
#define TSE_SEND_EQ_NODE_FREQ_8			66
#define TSE_SEND_EQ_NODE_GAIN_8			67
#define TSE_SEND_EQ_NODE_FREQ_9			68
#define TSE_SEND_EQ_NODE_GAIN_9			69

//[WS]
#define TSE_REDUCE_WIND_NOISE 			70		//# ON/OFF
#define TSE_MAX_WIND_NOISE_FREQ			71		//# HZ, < SAMPLE_RATE/2
#define TSE_WIND_NOISE_DETECT_AGG		72		//# 0.00 ~ 1.00
#define TSE_WIND_NOISE_START_THRES		73		//# > 0.00
#define TSE_WIND_NOISE_END_SPEED		74		//# > 0.00
#define TSE_WIND_NOISE_SUPP_AGG			75		//# 0.00 ~ 1.00
#define TSE_RESTORE_LOW_BAND_SPEECH		76		//# ON/OFF
#define TSE_SPEECH_RESTORE_FREQ			77		//# HZ, 100.0 ~ 1000.0
#define TSE_WIND_NOISE_HIGH_THR			78		//# 
#define TSE_WIND_NOISE_LOW_THR			79		//# 
#define TSE_FREQ_DEV_RES_FREQ			80		//# HZ, < SAMPLE_RATE/2
#define TSE_WIND_NOISE_FREQ_DEV_THR		81
#define TSE_WIND_NOISE_TIME_DEV_THR		82
#define TSE_WIND_NOISE_TIME_FREQ_SCALE	83
#define TSE_WIND_NOISE_NORM_SCALE		84
#define TSE_WIND_NOISE_MID_FREQ			85
#define	TSE_WIND_NOISE_MID_BW_FREQ		86
#define	TSE_WIND_NOISE_MID_FREQ_RATIO_THR	87
#define	TSE_WIND_NOISE_MID_SMOOTH		88
#define	TSE_WIND_NOISE_MID_WEIGHT		89
#define	TSE_WIND_NOISE_MFR_HIGH_SCALE	95
#define	TSE_WIND_NOISE_MFR_MID_SCALE	96

//[NS]
#define TSE_NR_OUTPUT_LIMITER			90		//# ON/OFF : Output Limiter before converting float into int16
#define TSE_NR_OUTPUT_LIMITER_AT		91		//# 0.00 ~ 1.00 : Attack Time for Output Limiter
#define TSE_NR_OUTPUT_LIMITER_RT		92		//# 0.00 ~ 1.00 : Release Time for Output Limiter
#define TSE_NR_OUTPUT_LIMITER_THRESHOLD	93		//# 0.00 ~ 1.00 : Threshold for Output Limiter
#define TSE_SUPPRESS_RESIDUAL_ECHO		100		//# ON/Off
#define TSE_RESIDUAL_ECHO_SUPP_AGG		101		//# 0.00 ~ 1.00
#define TSE_HIGHPASS_CUTOFF_FREQ		102		//# HZ, < 1000.0
#define TSE_MIN_VAD_ANALYSIS_FREQ		103		//# HZ, HIghpass_cutoff_freq ~ SAMPLE_RATE/2 - 500.0
#define TSE_MAX_VAD_ANALYSIS_FREQ		104		//# HZ, MIn_VAD_analysis_freq + 500.0 ~ SAMPLE_RATE/2
#define TSE_NOISE_UPDATE_DELAY			105		//# 1, 2, 3, ..., 10 (enum)
#define TSE_TOTAL_VAD_THRES				106		//# > 0.00
#define TSE_SPEECH_END_DELAY			107		//# >= 1 (INt)
#define TSE_SPEECH_ONSET_THRES			108		//# > NOISe_only_frame_thres
#define TSE_SPEECH_END_THRES			109		//# > 0.00
#define TSE_NOISE_ONLY_FRAME_THRES		110		//# > 0.00
#define TSE_ECHO_PRESENCE_SPEC_THRES	111		//# > 0.00
#define TSE_ECHO_ONLY_SPEC_THRES		112		//# > 0.00
#define TSE_ECHO_PRESENCE_FRAME_THRES	113		//# 0.00 ~ 1.00
#define TSE_ECHO_ONLY_FRAME_THRES		114		//# 0.00 ~ 1.00
#define TSE_ECHO_SUPP_AGG_IN_SPEECH_FRAME	115	//# 0.00 ~ 1.00
#define TSE_THRES_SPEECH_IN_NONSPEECH_FRAME 116	//# > 0.00
#define TSE_THRES_NONSPEECH_IN_SPEECH_FRAME 117	//# > 0.00
#define TSE_NOISE_EST_AGG_IN_SPEECH_FRAME	118	//# > 0.00
#define TSE_VAD_RESULT_SMOOTHING		119		//# 0.00 ~ 1.00
#define TSE_BIAS_TO_BASE_NOISE_LEVEL	120		//# > 0.00
#define TSE_NOISE_SMOOTHING				121		//# > 0.00
#define TSE_NOISE_SCALING				122		//# > 0.00
#define TSE_MUSICAL_NOISE_SUPPRESS_AGG	123		//# > 0.00
#define TSE_NR_GAIN_BIAS				124		//# > 0.00
#define TSE_SPEECH_PRESENCE_SPEC_THRES	125		//# > 0.00
#define TSE_STRONG_SPEECH_SPEC_THRES	126		//# > SPeech_presence_spec_thres
#define TSE_SPEECH_PRESENCE_FRAME_THRES 127		//# > 0.00
#define TSE_STRONG_SPEECH_FRAME_THRES	128		//# > SPeech_presence_frame_thres
#define TSE_SPEECH_TRANSITION_SPEED		129		//# 0.10 ~ 1.00
#define TSE_NR_AGG_IN_SPEECH_FRAME		130		//# 0.00 ~ 0.90
#define TSE_NR_AGG_NOISE_ONLY_FRAME		131		//# 0.00 ~ 1.00
#define TSE_NR_GAIN_INC_SPEED			132		//# 0.00 ~ 1.00
#define TSE_NR_GAIN_DEC_SPEED			133		//# 0.00 ~ 1.00
#define TSE_MUSICAL_NOISE_SUPPRESS_FREQ	134		//# HZ, Highpass_cutoff_freq ~ SAMPLE_RATE/2
#define TSE_MUSICAL_NOISE_SUPPRESS_STR	135		//# 0.00 ~ 1.00
#define TSE_FREQ_NR_GAIN_FLOOR_MAX		136		//# HZ, Highpass_cutoff_freq ~ SAMPLE_RATE/2
#define TSE_BW_NR_GAIN_FLOOR_MAX		137		//# HZ, > 35.0
#define TSE_NR_GAIN_FLOOR_MAX			138		//# NR_GAIN_floor ~ 1.00
#define TSE_NR_GAIN_FLOOR_SLOPE_LOW		139		//# > 0.00
#define TSE_NR_GAIN_FLOOR_SLOPE			140		//# > 0.00
#define TSE_NR_GAIN_FLOOR				141		//# 0.00 ~ 1.00
#define TSE_MUSICAL_NOISE_SUPP			142		//# ON/OFF
#define TSE_MUSICAL_NOISE_SUPP_SKIP_FREQ 143	//# 0.0 ~ SAMPLE_RATE/2
#define TSE_MUSICAL_NOISE_SUPP_SCALE	144		//# > 1
#define TSE_MUSICAL_NOISE_SUPP_THRES	145		//# 0.00 ~ 1.00
#define TSE_LOWPASS_CUTOFF_FREQ			146		//# HZ, > 6000.0
#define TSE_ECHO_PRESENCE_SCALE			147		//# > 0.00
#define TSE_ECHO_ONLY_SCALE				148		//# > 0.00
#define TSE_NR_CCS_GAIN_MIN				149		//#
//#define TSE_NR_COMF_NOISE_SPP_THR		94

//[CNI]
#define TSE_COMFORT_NOISE_INSERTION		150		//# ON/Off
#define TSE_COMFORT_NOISE_INC_SPEED		151		//# 0.00 ~ 1.00
#define TSE_COMFORT_NOISE_DEC_SPEED		152		//# 0.0000 ~ 1.0000
#define TSE_NOISE_GEN_AGG				153		//# > 0.0
#define TSE_COMFORT_NOISE_BOOST			154		//# > 0.0
#define TSE_INITIAL_FREQUENCY			155		//# # 0.0 ~ SAMPLE_RATE/2
#define TSE_STEP_FREQUENCY				156		//# 0.0 ~ SAMPLE_RATE/2
#define TSE_INITIAL_MINIMUM_NOISE_DB	157		//# -90 < x < 0.0
#define TSE_DECREASE_NOISE_DB			158		//# -90 < X < 0.0
//#define TSE_COMFORT_SKIP_FREQ			159		//# X > 0  
#define TSE_COMFORT_NOISE_MAX_DB			159		//# x > 0
#define TSE_COMFORT_NR_GAIN				160		//# 

//[AGC] [send]
#define TSE_AUTOMATIC_GAIN_CONTROL		170		//# On/Off
#define TSE_INITIAL_AGC_GAIN			171		//# > 1.0
#define TSE_AGC_GAIN_CAL_START_THRES	172		//# 0.000 ~ 1.000
#define TSE_AGC_GAIN_CAL_END_AGG		173		//# > 0.0
#define TSE_AGC_GAIN_SUPPRESS			174		//# > 0.0
#define TSE_AGC_GAIN_INC_SPEED			175		//# 0.0 ~ 1.0
#define TSE_AGC_GAIN_DEC_SPEED			176		//# 0.0 ~ 1.0
#define TSE_MAX_AGC_GAIN_INC_STEP		177		//# > 1.0000
#define TSE_MAX_AGC_GAIN_DEC_STEP		178		//# 0.0000 ~ 1.0000
#define TSE_MIN_AGC_GAIN				179		//# > 1.0
#define TSE_MAX_AGC_GAIN				180		//# > 1.0
//[AGC] [RECV]
#define TSE_AUTOMATIC_GAIN_CONTROL_RECV			181// off		//# On/Off
#define TSE_SILENCE_TO_SILENCE_FRAME_THRES		182// 64.0f; // 0 ~ 128
#define TSE_SPEECH_TO_SILENCE_FRAME_THRES		183// 25.0f; // 0 ~ 128
#define TSE_SPEECH_LEVEL_UPPER_BOUND_FOR_VAD	184// 35.0f; // 0 ~ 128
#define TSE_SPEECH_LEVEL_VARIANCE_FOR_VAD		185// 125.0f; // 0 ~ 256
#define TSE_NUM_SPEECH_LEVEL_EST_FRAMES			186// 100; // int // 0 ~ 200
#define TSE_THRES_NON_SPEECH_FRAME				187// 0.2f; // 0 ~ 128
#define TSE_MIN_LENGTH_OF_SPEECH				188// 7; //int // 0 ~ 200
#define TSE_THRES_SPEECH_FRAME					189// 0.3f;  // 0 ~ 128
#define TSE_INITIAL_RX_AGC_GAIN					190// 7.0f; // 0 ~ 20
#define TSE_TARGET_SPEECH_LEVEL					191// 75.0f; // 0 ~ 128
#define TSE_MAX_LONG_TERM_AGC_GAIN				192// 12.0f; // 0 ~ 128
#define TSE_MAX_AGC_GAIN_CHANGE					193// 15.0f; // 0 ~ 128
#define TSE_MAX_SHORT_TERM_AGC_GAIN				194// 64.0f; // 0 ~ 128
#define TSE_CLIPPING_DETECT_THRES				195// 0.02f; // 0 ~ 1.0
#define TSE_AGC_GAIN_SUPP_AGAINST_CLIPPING		196// 6.0f; // 0 ~ 20

#define TSE_WNR_END     200


#define TUNE_PARAMETER_SEND_EQ  201
#define TUNE_PARAMETER_RECV_EQ  202
#define TUNE_PARAMETER_SIGNAL_GEN  203

// RECV
#define TSE_RECV_START     300
#define TSE_RECV_DYNAMIC_LVL_COMPENSATION 301    //[Receive] on/off
#define TSE_RECV_NORMAL_NOISE_LVL         302    //[Receive]  min -90 ~ max 0  (dB)
#define TSE_RECV_PEAK_NOISE_LVL           303    //[Receive]  min -90 ~ max 0  (dB)
#define TSE_RECV_MAXIMUM_GAIN             304    //[Receive]  min 0 ~ max 12 (dB)
#define TSE_RECV_SMOOTING_FACTOR          305    //[Receive]  min 0 ~ max 1
#define TSE_RECV_CCS_MASK_ENABLE		  306
#define TSE_RECV_CCS_GUARD_THRESHOLD	  307
#define TSE_RECV_CCS_THLD_PNR0			  310
#define TSE_RECV_CCS_THLD_PNR1			  311
#define TSE_RECV_CCS_THLD_PNR2			  312
#define TSE_RECV_CCS_THLD_PNR3			  313
#define TSE_RECV_CCS_THLD_PNR4			  314
#define TSE_RECV_CCS_THLD_PNR5			  315
#define TSE_RECV_CCS_THLD_PNR6			  316
#define TSE_RECV_CCS_THLD_PPR0			  320
#define TSE_RECV_CCS_THLD_PPR1			  321
#define TSE_RECV_CCS_THLD_PPR2			  322
#define TSE_RECV_CCS_THLD_PPR3			  323
#define TSE_RECV_CCS_THLD_PPR4			  324
#define TSE_RECV_CCS_THLD_PPR5			  325
#define TSE_RECV_CCS_THLD_PPR6			  326
#define TSE_RECV_END     349

// CGM
#define TSE_CGM_START     350
#define TSE_CGM_PREAMP_FIXED_GAIN_RECV        351    //[Receive/Send]  min -12 ~ max 12 (dB)
#define TSE_CGM_PREAMP_FIXED_GAIN_SEND   352    //[Receive/Send]  min -12 ~ max 12 (dB)
#define TSE_CGM_END     399

// AEC
#define TSE_AEC_START     400
#define TSE_REFERENCE_VOLUME_APPLICATION        401    // 0.25F; // 0.0F ~ 1.0f
#define TSE_EC_ADAPTVOLUMEREVERSESCALE			402
#define TSE_NEAR_DECISION_SETTHRESHOLD_MIN		403
#define TSE_NEAR_DECISION_UNSETTHRESHOLD_MIN	404
#define TSE_FAR_DECISION_MINUMVALUE				405
#define TSE_FAR_DECISION_SETSCALE				406
#define TSE_FAR_DECISION_UNSETSCALE				407
#define TSE_NEAR_DECISION_AUXILIARY_HELPER		408
#define TSE_AUXILIARY_HELPER_FAR_ERR_RATIO		409
#define TSE_GAMMA								410
#define TSE_VS_GAMMA							411
#define TSE_DELTA_FAR							412
#define TSE_STEPSIZE_POSTFILT					413
#define TSE_FIRST_FAR_STEPSIZE_POSTFILT_LOW		414
#define TSE_FIRST_FAR_STEPSIZE_POSTFILT_HIGH	415
#define TSE_FIRST_FAROTHER_STEPSIZE_POSTFILT_LOW	416
#define TSE_FIRST_FAROTHER_STEPSIZE_POSTFILT_HIGH	417
#define TSE_FIRST_DT_STEPSIZE_POSTFILT_LOW		418
#define TSE_FIRST_DT_STEPSIZE_POSTFILT_HIGH		419
#define TSE_SECOND_FAR_STEPSIZE_POSTFILT_LOW	420
#define TSE_SECOND_FAR_STEPSIZE_POSTFILT_HIGH	421
#define TSE_STEPSIZE_NOSIG						422
#define TSE_FIRST_FAR_STEPSIZE_POSTFILT_LOW_REDUCTIONCONTROL	423
#define TSE_FIRST_FAR_STEPSIZE_POSTFILT_HIGH_REDUCTIONCONTROL	424
#define TSE_FIRST_FAROTHER_STEPSIZE_POSTFILT_LOW_REDUCTIONCONTROL	425
#define TSE_FIRST_FAROTHER_STEPSIZE_POSTFILT_HIGH_REDUCTIONCONTROL	426
#define TSE_FIRST_DT_STEPSIZE_POSTFILT_LOW_REDUCTIONCONTROL		427
#define TSE_FIRST_DT_STEPSIZE_POSTFILT_HIGH_REDUCTIONCONTROL	428
#define TSE_SECOND_FAR_STEPSIZE_POSTFILT_LOW_REDUCTIONCONTROL	429
#define TSE_SECOND_FAR_STEPSIZE_POSTFILT_HIGH_REDUCTIONCONTROL	430
#define TSE_EC_PRENLP_SCALE										431
#define TSE_FIRST_STEPSIZE_PREFILT_LOWFREQ_LOWBIN				432
#define TSE_FIRST_STEPSIZE_PREFILT_LOWFREQ_HIGHBIN				433
#define TSE_FIRST_STEPSIZE_PREFILT_HIGHFREQ_LOWBIN				434
#define TSE_FIRST_STEPSIZE_PREFILT_HIGHFREQ_HIGHBIN				435
#define TSE_SECOND_STEPSIZE_PREFILT_LOWFREQ_LOWBIN				436
#define TSE_SECOND_STEPSIZE_PREFILT_LOWFREQ_HIGHBIN				437
#define TSE_SECOND_STEPSIZE_PREFILT_HIGHFREQ_LOWBIN				438
#define TSE_SECOND_STEPSIZE_PREFILT_HIGHFREQ_HIGHBIN			439
#define TSE_THIRD_STEPSIZE_PREFILT_LOWBIN						440
#define TSE_THIRD_STEPSIZE_PREFILT_HIGHBIN						441
#define TSE_FIRST_STEPSIZE_PREFILT_LOWFREQ_LOWBIN_REDUCTIONCONTROL		442
#define TSE_FIRST_STEPSIZE_PREFILT_LOWFREQ_HIGHBIN_REDUCTIONCONTROL		443
#define TSE_FIRST_STEPSIZE_PREFILT_HIGHFREQ_LOWBIN_REDUCTIONCONTROL		444
#define TSE_FIRST_STEPSIZE_PREFILT_HIGHFREQ_HIGHBIN_REDUCTIONCONTROL	445
#define TSE_SECOND_STEPSIZE_PREFILT_LOWFREQ_LOWBIN_REDUCTIONCONTROL		446
#define TSE_SECOND_STEPSIZE_PREFILT_LOWFREQ_HIGHBIN_REDUCTIONCONTROL	447
#define TSE_SECOND_STEPSIZE_PREFILT_HIGHFREQ_LOWBIN_REDUCTIONCONTROL	448
#define TSE_SECOND_STEPSIZE_PREFILT_HIGHFREQ_HIGHBIN_REDUCTIONCONTROL	449
#define TSE_THIRD_STEPSIZE_PREFILT_LOWBIN_REDUCTIONCONTROL				450
#define TSE_THIRD_STEPSIZE_PREFILT_HIGHBIN_REDUCTIONCONTROL				451
#define TSE_NLP_SCALE													452
#define TSE_POST_SCALE													453

//#define TSE_Reference_volume_application								489    // on/off
#define TSE_ECHO_CANCELLING_RATIO										490    // 0.25f; // 0.0f ~ 3.0f
#define TSE_ECHO_CANCELLING_RATIO_MAX									491    // 1.00f; // 0.0f ~ 1.0f
#define TSE_SIGMOID_FITTING_CURVE										492    // on/off
#define TSE_SIGMOID_FITTING_SLOPE										493    // -10.0f; // -5.0f ~ -20.0f
#define TSE_SIGMOID_FITTING_OFFSET										494    // 0.05f; // 0.0001f ~ 1.0f
#define TSE_SIGMOID_FITTING_AMPLIFY										495 // default 1 min 0  max 10
#define TSE_SIGMOID_FITTING_SHIFT										496 // default 0.5 min 0  max 1
#define TSE_AEC_END														500



#define VERSION_FILE_NAME "version_info.txt"
#define TSF_CHECKSUM_FILE_NAME "checksum_info.txt"
//typedef struct tseMainStruct ttseMain; 
typedef void ttseMain;

/** Structure used to input configuration information with two-stage initialization */
typedef struct ttseConfigData{
	unsigned short int ConfigBufferCnt;
	char* ConfigBuffer[TSE_MAX_CONFIG_DATA_CNT];
	unsigned int ConfigBufferSize[TSE_MAX_CONFIG_DATA_CNT];
} ttseConfigData;

typedef short int *ttseAudio[TSE_MAX_AUDIO_CNT];

typedef unsigned int ttseVolumeInfo;

typedef struct ttseDebug
{
	unsigned int    InBufferSize;      /**< Size of the debug input buffer in bytes                  */
	unsigned int    InBufferUsedSize;  /**< Number of bytes actually being used in the input buffer  */
	void*           InBuffer;          /**< Pointer to debug input buffer                            */
	unsigned int    OutBufferSize;     /**< Size of debug output buffer in bytes                     */
	unsigned int    OutBufferUsedSize; /**< Number of bytes actually being used in the output buffer */
	void*           OutBuffer;         /**< Pointer to debug output buffer                           */
} ttseDebug;

//tse·Î º¯°æ.
/*-----------------------------------------------------------------------------*/
/*       Function prototypes                                                   */
/*-----------------------------------------------------------------------------*/
int tseCreate(ttseMain **Main);
int tseInitialize(ttseMain *pMain,
					const ttseConfigData *pConfigData);
int tseProcess(ttseMain* pMain,
				const ttseAudio MicIn,
				const ttseAudio RecvIn,
				const ttseAudio RefIn,
				ttseAudio MicOut,
				ttseAudio RecvOut,
				ttseVolumeInfo * pVolumeInfo,
				ttseDebug *pDebugData );
int tseDestroy(ttseMain **pMain );
int tseReset(ttseMain *pMain );

int tseSetData(ttseMain *pMain,
				const ttseDataID DataID,
				const int iChannel,
				const unsigned int iSize,
				const void *pData );

int tseGetData(ttseMain *pMain,
				const ttseDataID DataID,
				const unsigned int iChannel,
				unsigned int *pSize,
				void *pData );

int tseGetErrorMessage(const ttseMain *pMain, 
						char *pErrorMessage);

void tseGetVersion(unsigned int *pArrayLen,
					const int **pVersionArray,
					const char **pVersionString,
					const char **pVersionComment );

int tseLoadTSFtoConfig(ttseConfigData *pConfigData, char *strTSFpath);
int tseCheckTsf(ttseMain* pMain, const ttseConfigData* pConfigData);

#ifdef __cplusplus
}
#endif


#endif
