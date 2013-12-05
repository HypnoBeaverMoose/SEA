#pragma once

class PlantGeneratorApp : public App
{	
public:	
	AAssetManager* m_assetManager;
	PlantGeneratorApp();
protected:	
	virtual bool loadImage(png::image<png::rgba_pixel>& image, const char* filename);


};