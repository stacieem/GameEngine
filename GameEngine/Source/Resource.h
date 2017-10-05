#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <map>

class Resource {
public:

	Resource() {
		
	}

	virtual ~Resource() {
		DBG("delete resrouce");
	}

	String getName() {
		return name;
	}

	void setName(String name) {
		this->name = name;
	}
	

private:

	String name;

	
	
	JUCE_LEAK_DETECTOR(Resource)


};
