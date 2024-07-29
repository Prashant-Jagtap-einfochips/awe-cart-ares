ModuleControl 0xBEEFF2D8, BaseModule

{

    coeff                   *float     // Coefficent Data
    CONTROL_REQUEST         *uint      // Control Message Request
    CONTROL_RESPONSE        *uint      // Control Message Response
    modPtr                  int        // Points to the module to set
    varPtr                  int        // Points to the variable to set within the module instance structure
}

