#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

struct Vertex
{

	Vector3D<GLfloat> position;  // To define vertex x,y,z coordinates
						//float normal[3];    // Orthogonal vector used to calculate light impact on the texture color
						//float colour[4];    // Color used for the vertex. If no other color info is given for the fragment
						// the pixel colors will be interpolated from the vertex colors
	float texCoord[2];  // A graphic image (file) can be used to define the texture of the drawn object.
						// This 2-D vector gives the coordinates in the 2-D image file corresponding to
						// the pixel color to be drawn

	Vertex() {

	}

	Vertex(const Vertex& vert) {
		position = vert.position;
		texCoord[0] = vert.texCoord[0];
		texCoord[1] = vert.texCoord[1];
	}

	Vertex(Vector3D<GLfloat> pos, float coordX, float coordY) {
		position = pos;
		texCoord[0] = coordX;
		texCoord[1] = coordY;
	}

};