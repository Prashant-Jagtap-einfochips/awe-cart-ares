%% TLC code generation file - hand written source
%% INCLUDED BY: RTC_NTF.tlc

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
int %<ModelPrefix()>ServiceRtcNotifications(void* pNtfBuf, uint32_t ntfBufSize)
{
    // Note assumption of single service thread that handles both
    // RTC message requests and this notification handler.

    // This static
    static uint32_t tableIdx = 0;

    int retVal = 0;
    // Calculate the table size - subtract one for the End Sentinel
    uint32_t tableSize = (sizeof(notify_table) / sizeof(%<RtcGetNtfFileName()>_type)) - 1;
    uint32_t entryCount = tableSize;
    while (entryCount--)
    {
        %<RtcGetNtfFileName()>_type* pTableEntry = &notify_table[tableIdx];
        if (++tableIdx >= tableSize)
        {
            tableIdx = 0;
        }

        // Poll for completion if the current request is still pending
        if (*pTableEntry->requested_sequence_ptr != pTableEntry->completed_sequence)
        {
            // Not done last we checked. Check again now:
            pTableEntry->check_complete_func();

            // If the request is done now, capture status
            if (*pTableEntry->is_completed_ptr)
            {
                pTableEntry->completed_sequence = *pTableEntry->requested_sequence_ptr;
                pTableEntry->completed_caller_handle = *pTableEntry->requested_caller_handle_ptr;
            }
        }
        // Attempt to send notification if a completed sequence is waiting to send
        if (pTableEntry->last_sent_sequence != pTableEntry->completed_sequence)
        {
            uint8_t ntfBufOK = (ntfBufSize >= pTableEntry->ntf_payload_bytes) && pNtfBuf;

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
                statusHdr->msgId           = pTableEntry->message_id;
                statusHdr->callerHandle    = pTableEntry->completed_caller_handle;
                statusHdr->statusCode      = ASDSTATUS_RTC_NOTIFICATION;
                retVal = pTableEntry->ntf_payload_bytes;
                pTableEntry->last_sent_sequence = pTableEntry->completed_sequence;
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
