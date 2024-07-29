classdef EventList
methods
function Crc32
   %  Crc32 Summarizes list content with a CRC32 integer hash value
   %  Usage:
   %    hash = Crc32(this)
end
function GetData
   %  GetData retrieves element of an event at the specified index
   %  Usage:
   %    [data] = GetData(this, index)
end
function CheckTimeHit
   %  CheckTimeHit Search for matching event by name and time
   %  if isHit==true, caller must increment iterator themselves to
   %  continue search. Otherwise (i.e. isHit==false) do not change
   %  iterator as it will self-increment until becoming empty.
   %  Usage:
   %    [isHit,iteratorOut] = CheckTimeHit(this, name, min_time, max_time, iterator)
end
function AddEvent
   %  AddEvent adds a new event to the list.
   %  "bubblesort" the new event into order... because sorting an
   %  already filled out struct array is not directly possible.
   %  Usage:
   %     AddEvent(this, name, time, data)
end
function EventList
   %EventList/EventList is a constructor.
   %    this = EventList(size)
   %
   %    Documentation for EventList/EventList
   %       helpwin EventList
   %
end
end
end
