#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class InspectorUpdater {

public:
	void setChangeBroadcasterForUpdate(ChangeBroadcaster * updateInspectorsChangeBroadcaster)
	{
		this->updateInspectorsChangeBroadcaster = updateInspectorsChangeBroadcaster;
	}

protected:
	ChangeBroadcaster * updateInspectorsChangeBroadcaster;
};