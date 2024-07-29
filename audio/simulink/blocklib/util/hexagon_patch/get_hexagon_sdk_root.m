function hexagon_sdk_root = get_hexagon_sdk_root()
    if ispc
        hexagon_sdk_root = "C:/Qualcomm/Hexagon_SDK/";
    elseif isunix
        hexagon_sdk_root = "/opt/Qualcomm/Hexagon_SDK/";
    else
        error("PLATFORM:NOT:SUPPORTED","Platform not supported for float types alignment script! Float types alignment will not be executed.\n")
    end
end