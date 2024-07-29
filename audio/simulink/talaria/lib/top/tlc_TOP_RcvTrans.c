%% tlc_TOP_RcvTrans.c : hand-written C code included by TOP_MEX.tlc


// CAST_PRINT_STORE performs the cast, print, and store
// Assumed:srcArray,dstArray,numElem,buf,bufSize,srcFix.factor,failDetected
// Note that SRCPREFIX is preprended to source value evaluations
#define CAST_PRINT_STORE(STYPE,DTYPE,FMTSTR,SRCPREFIX)\
        {int index;\
         for (index = 0; index < numElem; index++){\
                STYPE srcVal = ((STYPE *)srcArray)[index];\
                DTYPE dstVal = (srcFix.factor)?\
                               (DTYPE)((double)SRCPREFIX((srcVal)*srcFix.factor)):(DTYPE)(SRCPREFIX srcVal);\
                if (bufSize > 1){\
                    int used = snprintf(buf, bufSize, FMTSTR, dstVal);\
                    if (used <= 0) {used=0;failDetected=1;}\
                    bufSize -= used;\
                    buf += used;\
                }\
                if(dstArray)((DTYPE*)dstArray)[index]=dstVal;\
            }}

// SRC_DST_SWITCH is invoked by STYPE code and to then invoke CAST_PRINT_STORE by dstType id
// Assumed:dstType. Note that boolean type uses "!!" to convert to 0 or 1
#define SRC_DST_SWITCH(STYPE) { \
                switch (dstType) { \
                case TSS_DOUBLE  : {CAST_PRINT_STORE(STYPE,double   ," %.17g",);break;} \
                case TSS_SINGLE  : {CAST_PRINT_STORE(STYPE,float    ," %.9g" ,);break;} \
                case TSS_INT8    : {CAST_PRINT_STORE(STYPE,int8_t   ," %i" ,);break;} \
                case TSS_UINT8   : {CAST_PRINT_STORE(STYPE,uint8_t  ," %u" ,);break;} \
                case TSS_INT16   : {CAST_PRINT_STORE(STYPE,int16_t  ," %i" ,);break;} \
                case TSS_UINT16  : {CAST_PRINT_STORE(STYPE,uint16_t ," %u" ,);break;} \
                case TSS_INT32   : {CAST_PRINT_STORE(STYPE,int32_t  ," %i" ,);break;} \
                case TSS_UINT32  : {CAST_PRINT_STORE(STYPE,uint32_t ," %u" ,);break;} \
                case TSS_BOOLEAN : {CAST_PRINT_STORE(STYPE,int8_t   ," %i" ,!!);break;} \
                default:{dstArray=0;CAST_PRINT_STORE(STYPE,double," %lf",);break;}\
                } }

%% pull in code for FixedInfo and DecodeFixedTypeName(const char* typeName)
%include "fixdtNameParse_c"

        
static TOP_RcvTrans_type* GetTopRecord(TranslateEngineIterator topNumIn)
{
    // Validate input parameters
    if ((topNumIn.model < 0) || (topNumIn.model >= numTranslationRoots))
    {
        return 0; // model out of range
    }
    TranslationRoot* theRoot = translationRoots[topNumIn.model];
    if ((topNumIn.index < 0) || (topNumIn.index >= *theRoot->top_count))
    {
        return 0; // index out of range for specified model
    }
    TOP_RcvTrans_type* theTable = *(TOP_RcvTrans_type**)theRoot->top_table;
    return &theTable[topNumIn.index];
}

// Get the name of a TOP, or NULL if not found
const char* GetTopName(TranslateEngineIterator topNumberIn)
{
    const char* theName = 0;
    
    TOP_RcvTrans_type* recordIn = GetTopRecord(topNumberIn);
    if (recordIn)
    {
        theName = recordIn->top_name;
    }
    
    return theName;
}

// Dump the translated value to provided buffer & updating sightings
// Returns number of chars filled in buffer or negative if buffer too small
int DumpTranslatedValue(TranslateEngineIterator rcvTransNumberIn, char* bufferInOut,
    int bufferSizeIn)
{
    int failDetected = 0;
    TOP_RcvTrans_type* recordIn = GetTopRecord(rcvTransNumberIn);
    if (!recordIn)
    {
        return 0; // Top not found
    }

    // Remember how many time this TOP has been encountered
    recordIn->top_sightings_count++;

    // Setup variables for the nested switch macros:
    int srcType = recordIn->top_in_dtype_id;
    int dstType = recordIn->top_out_dtype_id;
    void * srcArray = recordIn->top_value_src_ptr;
    void * dstArray = recordIn->top_value_dst_ptr;
    int numElem = recordIn->top_element_count * (1 + recordIn->top_is_complex);
    char* buf = bufferInOut;
    int bufSize = bufferSizeIn;

    FixedInfo srcFix = DecodeFixedTypeName(recordIn->top_in_dtype_name);
    if (srcFix.factor)
    {
        // When decoding a fixed point type, set the srcType to the
        // integer format of the data. Then... the srcfix.factor will
        // be used to convert to floating point
        srcType = srcFix.intDTypeId;
    }

    // Start with the TOP name and datatype
    int nameChars = snprintf(buf, bufSize, "%s %s",
        recordIn->top_name, recordIn->top_out_dtype_name);
    if (nameChars <= 0)
    {
        // Older implementations of snprintf only give failure
        // (instead of indicating how many bytes would be needed)
        nameChars = 0;
        failDetected = 1;
    }

    bufSize -= nameChars;
    buf += nameChars;
    switch (srcType)
    {
    case TSS_DOUBLE: SRC_DST_SWITCH(double);  break;
    case TSS_SINGLE: SRC_DST_SWITCH(float);   break;
    case TSS_INT8:   SRC_DST_SWITCH(int8_T);  break;
    case TSS_UINT8:  SRC_DST_SWITCH(uint8_T); break;
    case TSS_INT16:  SRC_DST_SWITCH(int16_T); break;
    case TSS_UINT16: SRC_DST_SWITCH(uint16_T);break;
    case TSS_INT32:  SRC_DST_SWITCH(int32_T); break;
    case TSS_UINT32: SRC_DST_SWITCH(uint32_T);break;
    case TSS_BOOLEAN:SRC_DST_SWITCH(int8_T);  break;
    }

    return failDetected ? (-1) : (bufferSizeIn - bufSize);
}

