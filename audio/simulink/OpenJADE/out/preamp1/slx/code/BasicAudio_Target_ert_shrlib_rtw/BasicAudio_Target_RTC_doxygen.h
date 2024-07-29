/*
 * File: BasicAudio_Target_RTC_doxygen.h
 *
 * Code generated for Simulink model 'BasicAudio_Target'.
 *
 * Model version                  : 7.2
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Wed Jul 17 15:46:55 2024
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Intel->x86-32 (Windows32)
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_BasicAudio_Target_RTC_doxygen_h_
#define RTW_HEADER_BasicAudio_Target_RTC_doxygen_h_

// ------------------------------------------------------------
// RUNTIME CONTROL MESSAGING SUMMARY
// ------------------------------------------------------------
/** @defgroup rtc Runtime Control (RTC)
 * The Runtime Control mechanism (RTC) supports communication with DSP algorithms.
 * For a list of the RTC messages, see the @ref rtc_messages.
 * Algorithm developers can define three types of payloads for each message as
 * described below.
 * Requests     (Control >>> DSP):
 * ---------
 *  RTC communication always begins with a request sent to the DSP algorithm
 *  code. RTC requests include a message ID and an accompanying data values
 *  (if any) as defined by the DSP algorithmic code.
 *  Request messages signal one of the following actions:
 *  - Initiate an action to take (e.g. mute the audio)
 *  - Update a state value (e.g. engine speed is now "0")
 *
 * Responses     (DSP >>> Control):
 * ---------
 *  Each RTC message definition has the option to include responses or not.
 *  If responses are defined for a message, then EVERY request receives a response.
 *  Note the following characteristics about response payloads:
 *  - They are generated immediately upon receipt of request
 *  - They contain the same RTCID as their matching request along with any
 *    additional information (if any) as defined for the message.
 *  - If a notification payload is also defined, the response will also
 *    contain a sequence tracking number to be referenced by the notification.
 *
 * Notifications (DSP >>> Control):
 * -------------
 *  Notification definitions are optional and only available if a response
 *  is also defined. A notification usually indicates that a request has finally
 *  been satisfied.
 *  Note the following characteristics about notification payloads:
 *  - They are generated at some time later after request is received
 *  - They reference the same RTCID as their matching request and response
 *  - They also reference the sequence number previously reported by
 *    an earlier response.
 *  - If a second request comes in before the first request generates a
 *    the first request may never generate a notification. In other words,
 *    only the "last request" is guaranteed to receive a notification.
 */

/** Setup for Doxygen RTC Service Table
   @defgroup rtc_messages RTC Message Index
   @ingroup rtc
   | Message | Message ID | Request | Response| Notification |
   |---------|------------|---------|---------|--------------|
   | @ref BasicAudioBalanceSet | 0x10000000 | @ref BasicAudioBalanceSet_REQ | @ref BasicAudioBalanceSet_RSP | @ref BasicAudioBalanceSet_NTF |
   | @ref GetTalariaVersion1 | 0x10f70000 | @ref GetTalariaVersion1_REQ | @ref GetTalariaVersion1_RSP | <CENTER><EM>n/a</CENTER></EM> |
   | @ref GetTopCompatHash1 | 0x10f70001 | @ref GetTopCompatHash1_REQ | @ref GetTopCompatHash1_RSP | <CENTER><EM>n/a</CENTER></EM> |
   | @ref GetTspCompatHash1 | 0x10f70002 | @ref GetTspCompatHash1_REQ | @ref GetTspCompatHash1_RSP | <CENTER><EM>n/a</CENTER></EM> |
   | @ref GetRtcCompatHash1 | 0x10f70003 | @ref GetRtcCompatHash1_REQ | @ref GetRtcCompatHash1_RSP | <CENTER><EM>n/a</CENTER></EM> |
   | @ref BasicAudioBassSet | 0x10000001 | @ref BasicAudioBassSet_REQ | @ref BasicAudioBassSet_RSP | @ref BasicAudioBassSet_NTF |
   | @ref BasicAudioMuteSet | 0x10000002 | @ref BasicAudioMuteSet_REQ | @ref BasicAudioMuteSet_RSP | @ref BasicAudioMuteSet_NTF |
   | @ref BasicAudioTrebleSet | 0x10000003 | @ref BasicAudioTrebleSet_REQ | @ref BasicAudioTrebleSet_RSP | @ref BasicAudioTrebleSet_NTF |
   | @ref BasicAudioVolumeSet | 0x10000004 | @ref BasicAudioVolumeSet_REQ | @ref BasicAudioVolumeSet_RSP | @ref BasicAudioVolumeSet_NTF |
 */

/**
   @defgroup BasicAudioBalanceSet
   @ingroup rtc_messages
   No further description of BasicAudioBalanceSet has been defined yet.
 */
/**
   @defgroup GetTalariaVersion1
   @ingroup rtc_messages
   GetTalariaVersion1
   ==============
   Get Talaria Version String e.g. V2100AE
 */
/**
   @defgroup GetTopCompatHash1
   @ingroup rtc_messages
   GetTopCompatHash1
   ==============
   Get TOP compatibility hash based on module name hash
 */
/**
   @defgroup GetTspCompatHash1
   @ingroup rtc_messages
   GetTspCompatHash1
   ==============
   Get TSP compatibility hash based on module name hash
 */
/**
   @defgroup GetRtcCompatHash1
   @ingroup rtc_messages
   GetRtcCompatHash1
   ==============
   Get RTC compatibility hash based on module name hash
 */
/**
   @defgroup BasicAudioBassSet
   @ingroup rtc_messages
   BasicAudioBassSet
   ==============
   Provide the target value as a 32-bit signed integer index value (to be mapped through a tunable calibration into a normalized float between -1 and 1).

   To use slope-based ramping:
   ----------------------------------
   When db_per_second has a non-zero value,ramp_milliseconds is ignored and db_per_second controls the ramp.

   To use time-based ramping:
   ----------------------------------
   The value of db_per_second must be zero for ramp milliseconds to then control the ramping.
 */
/**
   @defgroup BasicAudioMuteSet
   @ingroup rtc_messages
   BasicAudioMuteSet
   ==============
   Command a mute by specifying whether mute is active (non zero mute_switch) or inactive (mute_switch is "0"). If db_per_second is non-zero, it commands the timing of the mute, otherwise the ramp_milliseconds is used.
 */
/**
   @defgroup BasicAudioTrebleSet
   @ingroup rtc_messages
   BasicAudioTrebleSet
   ==============
   Provide the target value as a 32-bit signed integer index value (to be mapped through a tunable calibration into a normalized float between -1 and 1).

   To use slope-based ramping:
   ----------------------------------
   When db_per_second has a non-zero value,ramp_milliseconds is ignored and db_per_second controls the ramp.

   To use time-based ramping:
   ----------------------------------
   The value of db_per_second must be zero for ramp milliseconds to then control the ramping.
 */
/**
   @defgroup BasicAudioVolumeSet
   @ingroup rtc_messages
   BasicAudioVolumeSet
   ==============
   Sets new target volume.
   Provide the target volume as 8 bit index value to be mapped through a tunable VolumeTable.

   To use slope-based ramping:
   ----------------------------------
   When db_per_second has a non-zero value,ramp_milliseconds is ignored and db_per_second controls the ramp.

   To use time-based ramping:
   ----------------------------------
   The value of db_per_second must be zero for ramp milliseconds to then control the ramping.
 */
#endif                         /* RTW_HEADER_BasicAudio_Target_RTC_doxygen_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
