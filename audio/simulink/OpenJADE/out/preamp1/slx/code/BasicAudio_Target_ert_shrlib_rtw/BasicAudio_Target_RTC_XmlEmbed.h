/*
 * File: BasicAudio_Target_RTC_XmlEmbed.h
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

#ifndef RTW_HEADER_BasicAudio_Target_RTC_XmlEmbed_h_
#define RTW_HEADER_BasicAudio_Target_RTC_XmlEmbed_h_

// ------------------------------------------------------------
// EMBEDDED XML FILE OF RTC TOKEN VALUES
// ------------------------------------------------------------
/* [XMLBEGIN]<?xml version="1.0" encoding="utf-8"?>
   <root version="1.0">
   <NTF id="BasicAudioBalanceSet">
   <Signal id="msgId" charOffset="0" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="caller_handle" charOffset="4" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="status_code" charOffset="8" dtypeStr="int32_t" dtypeNum="6" width="1"/>
   <Signal id="gain_index" charOffset="12" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   </NTF>
   <REQ id="BasicAudioBalanceSet">
   <Signal id="msgId" charOffset="0" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="caller_handle" charOffset="4" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="gain_index" charOffset="8" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="ramp_db_per_second" charOffset="12" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="ramp_milliseconds" charOffset="16" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   </REQ>
   <RSP id="BasicAudioBalanceSet">
   <Signal id="msgId" charOffset="0" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="caller_handle" charOffset="4" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="status_code" charOffset="8" dtypeStr="int32_t" dtypeNum="6" width="1"/>
   <Signal id="gain_index" charOffset="12" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   </RSP>
   <RTC id="BasicAudioBalanceSet" msgId="0x10000000" payloadMetaHash="0xB435421A" coreIndex="0" coreTag="">
   <Description>[]</Description>
   <PayloadSizes REQ="20" RSP="16" NTF="16"/>
   </RTC>
   <NTF id="BasicAudioBassSet">
   <Signal id="msgId" charOffset="0" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="caller_handle" charOffset="4" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="status_code" charOffset="8" dtypeStr="int32_t" dtypeNum="6" width="1"/>
   <Signal id="gain_index" charOffset="12" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   </NTF>
   <REQ id="BasicAudioBassSet">
   <Signal id="msgId" charOffset="0" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="caller_handle" charOffset="4" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="gain_index" charOffset="8" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="ramp_db_per_second" charOffset="12" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="ramp_milliseconds" charOffset="16" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   </REQ>
   <RSP id="BasicAudioBassSet">
   <Signal id="msgId" charOffset="0" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="caller_handle" charOffset="4" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="status_code" charOffset="8" dtypeStr="int32_t" dtypeNum="6" width="1"/>
   <Signal id="gain_index" charOffset="12" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   </RSP>
   <RTC id="BasicAudioBassSet" msgId="0x10000001" payloadMetaHash="0xB435421A" coreIndex="0" coreTag="">
   <Description>Provide the target value as a 32-bit signed integer index value (to be mapped through a tunable calibration into a normalized float between -1 and 1).&#13; &#13;To use slope-based ramping:&#13;----------------------------------&#13;When db_per_second has a non-zero value,ramp_milliseconds is ignored and db_per_second controls the ramp.&#13; &#13;To use time-based ramping:&#13;----------------------------------&#13;The value of db_per_second must be zero for ramp milliseconds to then control the ramping.</Description>
   <PayloadSizes REQ="20" RSP="16" NTF="16"/>
   </RTC>
   <NTF id="BasicAudioMuteSet">
   <Signal id="msgId" charOffset="0" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="caller_handle" charOffset="4" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="status_code" charOffset="8" dtypeStr="int32_t" dtypeNum="6" width="1"/>
   <Signal id="mute_switch" charOffset="12" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   </NTF>
   <REQ id="BasicAudioMuteSet">
   <Signal id="msgId" charOffset="0" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="caller_handle" charOffset="4" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="mute_switch" charOffset="8" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="ramp_db_per_second" charOffset="12" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="ramp_milliseconds" charOffset="16" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   </REQ>
   <RSP id="BasicAudioMuteSet">
   <Signal id="msgId" charOffset="0" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="caller_handle" charOffset="4" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="status_code" charOffset="8" dtypeStr="int32_t" dtypeNum="6" width="1"/>
   <Signal id="mute_switch" charOffset="12" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   </RSP>
   <RTC id="BasicAudioMuteSet" msgId="0x10000002" payloadMetaHash="0xF0568ABA" coreIndex="0" coreTag="">
   <Description>Command a mute by specifying whether mute is active (non zero mute_switch) or inactive (mute_switch is "0"). If db_per_second is non-zero, it commands the timing of the mute, otherwise the ramp_milliseconds is used.</Description>
   <PayloadSizes REQ="20" RSP="16" NTF="16"/>
   </RTC>
   <NTF id="BasicAudioTrebleSet">
   <Signal id="msgId" charOffset="0" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="caller_handle" charOffset="4" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="status_code" charOffset="8" dtypeStr="int32_t" dtypeNum="6" width="1"/>
   <Signal id="gain_index" charOffset="12" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   </NTF>
   <REQ id="BasicAudioTrebleSet">
   <Signal id="msgId" charOffset="0" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="caller_handle" charOffset="4" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="gain_index" charOffset="8" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="ramp_db_per_second" charOffset="12" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="ramp_milliseconds" charOffset="16" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   </REQ>
   <RSP id="BasicAudioTrebleSet">
   <Signal id="msgId" charOffset="0" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="caller_handle" charOffset="4" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="status_code" charOffset="8" dtypeStr="int32_t" dtypeNum="6" width="1"/>
   <Signal id="gain_index" charOffset="12" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   </RSP>
   <RTC id="BasicAudioTrebleSet" msgId="0x10000003" payloadMetaHash="0xB435421A" coreIndex="0" coreTag="">
   <Description>Provide the target value as a 32-bit signed integer index value (to be mapped through a tunable calibration into a normalized float between -1 and 1).&#13; &#13;To use slope-based ramping:&#13;----------------------------------&#13;When db_per_second has a non-zero value,ramp_milliseconds is ignored and db_per_second controls the ramp.&#13; &#13;To use time-based ramping:&#13;----------------------------------&#13;The value of db_per_second must be zero for ramp milliseconds to then control the ramping.</Description>
   <PayloadSizes REQ="20" RSP="16" NTF="16"/>
   </RTC>
   <NTF id="BasicAudioVolumeSet">
   <Signal id="msgId" charOffset="0" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="caller_handle" charOffset="4" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="status_code" charOffset="8" dtypeStr="int32_t" dtypeNum="6" width="1"/>
   <Signal id="gain_index" charOffset="12" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   </NTF>
   <REQ id="BasicAudioVolumeSet">
   <Signal id="msgId" charOffset="0" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="caller_handle" charOffset="4" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="gain_index" charOffset="8" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="ramp_db_per_second" charOffset="12" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="ramp_milliseconds" charOffset="16" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="gain_offset" charOffset="20" dtypeStr="int32_t" dtypeNum="6" width="1"/>
   </REQ>
   <RSP id="BasicAudioVolumeSet">
   <Signal id="msgId" charOffset="0" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="caller_handle" charOffset="4" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   <Signal id="status_code" charOffset="8" dtypeStr="int32_t" dtypeNum="6" width="1"/>
   <Signal id="gain_index" charOffset="12" dtypeStr="uint32_t" dtypeNum="7" width="1"/>
   </RSP>
   <RTC id="BasicAudioVolumeSet" msgId="0x10000004" payloadMetaHash="0x3B6D65F2" coreIndex="0" coreTag="">
   <Description>Sets new target volume.&#13;Provide the target volume as 8 bit index value to be mapped through a tunable VolumeTable.&#13; &#13;To use slope-based ramping:&#13;----------------------------------&#13;When db_per_second has a non-zero value,ramp_milliseconds is ignored and db_per_second controls the ramp.&#13; &#13;To use time-based ramping:&#13;----------------------------------&#13;The value of db_per_second must be zero for ramp milliseconds to then control the ramping.</Description>
   <PayloadSizes REQ="24" RSP="16" NTF="16"/>
   </RTC>
   </root>
   [XMLEND] */
#endif                        /* RTW_HEADER_BasicAudio_Target_RTC_XmlEmbed_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
