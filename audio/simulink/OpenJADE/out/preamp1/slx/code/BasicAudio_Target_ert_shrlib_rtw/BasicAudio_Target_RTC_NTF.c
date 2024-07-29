/*
 * File: BasicAudio_Target_RTC_NTF.c
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

#include "rtwtypes.h"
#include "BasicAudio_Target.h"
               // for access to signal buffers for sequence, caller handle, etc.
#include "BasicAudio_Target_RTC_IDs.h" // for RTCIDs and NTF payload formats
#include "ASDID.h"        // for AsdIdStatus_type and ASDSTATUS_RTC_NOTIFICATION

// Function pointer type for RTC request completion checking
typedef void (*RTC_NTF_Check)( void );

// Function pointer type for RTC notification message packing
typedef void (*RTC_NTF_Pack)(void* pNtfBuf);

// Struct for notification table. Note some datatypes must match
// variables defined by Simulink-generated code (uint32_T,boolean_T)
typedef struct RTC_NotificationTable_tag
{
    uint32_t message_id;
    RTC_NTF_Check check_complete_func;
    RTC_NTF_Pack pack_notification_func;
    uint32_t ntf_payload_bytes;
    uint32_T* requested_sequence_ptr;
    uint32_T* requested_caller_handle_ptr;
    boolean_T* is_completed_ptr;
    uint32_t completed_sequence;
    uint32_t completed_caller_handle;
    uint32_t last_sent_sequence;
}
BasicAudio_Target_RTC_NTF_type;

// ----- Function prototypes for request completion checking:
extern void RtcNtfCheck_BasicAudioBalanceSet( void );
extern void RtcNtfCheck_BasicAudioBassSet( void );
extern void RtcNtfCheck_BasicAudioMuteSet( void );
extern void RtcNtfCheck_BasicAudioTrebleSet( void );
extern void RtcNtfCheck_BasicAudioVolumeSet( void );

// ----- Function prototypes for notification message packing:
extern void RtcNtfPack_BasicAudioBalanceSet(void* pNtfBuf);
extern void RtcNtfPack_BasicAudioBassSet(void* pNtfBuf);
extern void RtcNtfPack_BasicAudioMuteSet(void* pNtfBuf);
extern void RtcNtfPack_BasicAudioTrebleSet(void* pNtfBuf);
extern void RtcNtfPack_BasicAudioVolumeSet(void* pNtfBuf);

// ----- The NOTIFY TABLE -----:
static BasicAudio_Target_RTC_NTF_type notify_table[] =
{
    // Name: BasicAudioVolumeSet '<S120>/NTF Trigger'
    {
        RTCID_BasicAudioVolumeSet, &RtcNtfCheck_BasicAudioVolumeSet,
        &RtcNtfPack_BasicAudioVolumeSet, sizeof(BasicAudioVolumeSet_NTF_T),/*16 char*/
        &(BasicAudio_TargetBasicAudioVolumeSetRtcOut.REQ_sequence),
        &(BasicAudio_TargetBasicAudioVolumeSetRtcOut.REQ_caller_handle),
        &BasicAudio_Target_B.done,
        0, 0, 0
    },

    // Name: BasicAudioBalanceSet '<S12>/NTF Trigger'
    {
        RTCID_BasicAudioBalanceSet, &RtcNtfCheck_BasicAudioBalanceSet,
        &RtcNtfPack_BasicAudioBalanceSet, sizeof(BasicAudioBalanceSet_NTF_T),/*16 char*/
        &(BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_sequence),
        &(BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_caller_handle),
        &BasicAudio_Target_B.done_p,
        0, 0, 0
    },

    // Name: BasicAudioMuteSet '<S69>/NTF Trigger'
    {
        RTCID_BasicAudioMuteSet, &RtcNtfCheck_BasicAudioMuteSet,
        &RtcNtfPack_BasicAudioMuteSet, sizeof(BasicAudioMuteSet_NTF_T),/*16 char*/
        &(BasicAudio_TargetBasicAudioMuteSetRtcOut.REQ_sequence),
        &(BasicAudio_TargetBasicAudioMuteSetRtcOut.REQ_caller_handle),
        &BasicAudio_Target_B.done_e,
        0, 0, 0
    },

    // Name: BasicAudioBassSet '<S41>/NTF Trigger'
    {
        RTCID_BasicAudioBassSet, &RtcNtfCheck_BasicAudioBassSet,
        &RtcNtfPack_BasicAudioBassSet, sizeof(BasicAudioBassSet_NTF_T),/*16 char*/
        &(BasicAudio_TargetBasicAudioBassSetRtcOut.REQ_sequence),
        &(BasicAudio_TargetBasicAudioBassSetRtcOut.REQ_caller_handle),
        &BasicAudio_Target_B.done_m,
        0, 0, 0
    },

    // Name: BasicAudioTrebleSet '<S93>/NTF Trigger'
    {
        RTCID_BasicAudioTrebleSet, &RtcNtfCheck_BasicAudioTrebleSet,
        &RtcNtfPack_BasicAudioTrebleSet, sizeof(BasicAudioTrebleSet_NTF_T),/*16 char*/
        &(BasicAudio_TargetBasicAudioTrebleSetRtcOut.REQ_sequence),
        &(BasicAudio_TargetBasicAudioTrebleSetRtcOut.REQ_caller_handle),
        &BasicAudio_Target_B.done_l,
        0, 0, 0
    },

    // End Sentinel to prevent empty initializer if no NTF payloads.
    {
        0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0
    }
};

/** Services RTC Notifications - polling for request completion as well as
 * creating notification message payloads upon completion. When no notification
 * payload is returned, it means all notifications have been serviced for the
 * current audio frame. However, if a notification payload *is* returned, then
 * this routine can be called again (repeatedly) until no payload is returned.
 * All notifications have a message ID and caller handle matching their original
 * request and a statusCode of ASDSTATUS_RTC_NOTIFICATION.
 * NOTE: the same thread that services RTC requests must be responsible for
 * servicing notifications as well. The two routines must not operate at the
 * same time or race conditions can lead to faulty operation.
 *  @param pNtfBuf Empty buffer read to receive a notification message.
 *  @param ntfBufSize Size of buffer in bytes.
 *  @returns Number of bytes used in the provided notification buffer (pNtfBuf).
 *          Return Value | Meaning
 *           ----------- | :--------
 *               0       | All done. No use in calling this again until audio processing happens.
 *             > 0       | A notification was returned with this many bytes used in buffer. Line up more buffer space and call again if you can.
 *             < 0       | Error: buffer is too small. This is how many bytes are needed for the buffer (in negative form)
 * @ingroup rtc
 * @see AsdIdStatus_type for format of each notification message's header
 */
int BasicAudio_Target_ServiceRtcNotifications(void* pNtfBuf, uint32_t ntfBufSize)
{
    // Note assumption of single service thread that handles both
    // RTC message requests and this notification handler.

    // This static
    static uint32_t tableIdx = 0;
    int retVal = 0;

    // Calculate the table size - subtract one for the End Sentinel
    uint32_t tableSize = (sizeof(notify_table) / sizeof
                          (BasicAudio_Target_RTC_NTF_type)) - 1;
    uint32_t entryCount = tableSize;
    while (entryCount--)
    {
        BasicAudio_Target_RTC_NTF_type* pTableEntry = &notify_table[tableIdx];
        if (++tableIdx >= tableSize)
        {
            tableIdx = 0;
        }

        // Poll for completion if the current request is still pending
        if (*pTableEntry->requested_sequence_ptr !=
                pTableEntry->completed_sequence)
        {
            // Not done last we checked. Check again now:
            pTableEntry->check_complete_func();

            // If the request is done now, capture status
            if (*pTableEntry->is_completed_ptr)
            {
                pTableEntry->completed_sequence =
                    *pTableEntry->requested_sequence_ptr;
                pTableEntry->completed_caller_handle =
                    *pTableEntry->requested_caller_handle_ptr;
            }
        }

        // Attempt to send notification if a completed sequence is waiting to send
        if (pTableEntry->last_sent_sequence != pTableEntry->completed_sequence)
        {
            uint8_t ntfBufOK = (ntfBufSize >= pTableEntry->ntf_payload_bytes) &&
                pNtfBuf;
            if (ntfBufOK)
            {
                AsdIdStatus_type* statusHdr = (AsdIdStatus_type*)pNtfBuf;
                pTableEntry->pack_notification_func(pNtfBuf);

                // Overwrite status code and caller handle written by the pack function.
                // NTF payloads all have the same "I'm a notification" status which is
                // easier to place here than on the model canvas.
                // Lack of outbound buffer space might queue up NTF sends... so when
                // we *do* send a NTF we use the completed caller handle gathered
                // at the time the NTF check succeeded. Between then and now, a new
                // request might have come in. So- maybe the NTF pack above could grab
                // some overly fresh data, but at least the caller handle below will match
                // the request that finished.
                statusHdr->msgId = pTableEntry->message_id;
                statusHdr->callerHandle = pTableEntry->completed_caller_handle;
                statusHdr->statusCode = ASDSTATUS_RTC_NOTIFICATION;
                retVal = pTableEntry->ntf_payload_bytes;
                pTableEntry->last_sent_sequence =
                    pTableEntry->completed_sequence;
            }
            else
            {
                // indicate how many bytes should have been given - just make it negative
                retVal = (0 - pTableEntry->ntf_payload_bytes);
            }
            break;
        }
    }

    return retVal;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
