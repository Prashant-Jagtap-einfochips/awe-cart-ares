using System;
using System.Collections.Generic;
using System.Runtime.InteropServices; // for the DllImport attributes

/// <summary>
/// The <c>TranslationEngine</c> class wraps access to the Talaria Translation Engine DLL built by Simulink
/// </summary>
static class TranslationEngine
{
    // Set this constant to the location of the translation engine dll file
    // Make sure the DLL and this project are created with same platform target: both 32-bit or both 64-bit
    private const string DLLNAME = @"CODE\Model_TranslationEngine_win64.dll";

    #region Public interface
    /// <summary>
    /// <c>GetTuneVarList</c> returns a list of strings containing the names of all tune variables (aka TuneVars) known by the translation engine.
    /// This list may not be needed by OEM tools other than for purposes of debugging or other interests.
    /// </summary>
    /// <returns>List of TuneVar names</returns>
    public static List<string> GetTuneVarList()
    {
        List<string> theList = new List<string>();
        for (IntPtr name = FirstTuneVar(); name != IntPtr.Zero; name = NextTuneVar())
        {
            theList.Add(Marshal.PtrToStringAnsi(name));
        }

        return theList;
    }

    /// <summary>
    /// <c>SetTuneVar</c> registers a new value for the specified TuneVar. The value is a single string containing numeric values delimited by spaces.
    /// </summary>
    /// <param name="tuneVarNameIn">string name of TuneVar</param>
    /// <param name="spaceDelimitedValuesIn">string containing numeric values delimited by spaces</param>
    /// <returns>returns 0 for OK, 1 for unrecognized TuneVar, 2 for too many elements in the provided value</returns>
    public static int SetTuneVar(string tuneVarNameIn, string spaceDelimitedValuesIn)
    {
        IntPtr name = Marshal.StringToHGlobalAnsi(tuneVarNameIn);
        IntPtr values = Marshal.StringToHGlobalAnsi(spaceDelimitedValuesIn);
        return SetTuneVar(name, values);
    }

    /// <summary>
    /// <c>GetTOPs</c> returns a list of string containing all Target Overlay Parameters (aka TOPs) whose value was calculated based on provided TuneVars.
    /// Only the TOPs directly impacted by previously provided TuneVars are returned.
    /// </summary>
    /// <returns>List of TOP strings, each containing TOP name, TOP datatype, and TOP numeric values all delimited by space</returns>
    public static List<string> GetTOPs()
    {
        List<string> theList = new List<string>();
        for (IntPtr topString = GetFirstTranslatedTOP(); topString != IntPtr.Zero; topString = GetNextTranslatedTOP())
        {
            theList.Add(Marshal.PtrToStringAnsi(topString));
        }

        // Now that all the translated TOPs have been retrieved, clear the flags
        ClearAllTranslationRequiredFlags();

        // TODO: check that the GetTranslationErrorState is still 0. If it's not zero, there has been an error
        // Perhaps an exception could be raised. Unfortunately the only place that receives detail about the error
        // is stdout.
        bool thereWasAnError = 0 != GetTranslationErrorState();

        return theList;
    }
    #endregion

    #region DLL imports
    [DllImport(TranslationEngine.DLLNAME)]
    private static extern void ClearAllTuneSightings();

    [DllImport(TranslationEngine.DLLNAME)]
    private static extern void ClearAllTranslationRequiredFlags();

    [DllImport(TranslationEngine.DLLNAME)]
    private static extern int GetTranslationErrorState();

    [DllImport(TranslationEngine.DLLNAME, CallingConvention = CallingConvention.Cdecl)]
    private static extern int SetTuneVar(IntPtr tuneVarNameIn, IntPtr spaceDelimitedValuesIn);

    [DllImport(TranslationEngine.DLLNAME)]
    private static extern IntPtr FirstTuneVar();

    [DllImport(TranslationEngine.DLLNAME)]
    private static extern IntPtr NextTuneVar();

    [DllImport(TranslationEngine.DLLNAME)]
    private static extern IntPtr GetFirstTranslatedTOP();

    [DllImport(TranslationEngine.DLLNAME)]
    private static extern IntPtr GetNextTranslatedTOP();
    #endregion
}
