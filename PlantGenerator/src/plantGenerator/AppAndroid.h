#pragma once
#include "App.h"
class AppAndroid : public App
{
public:
	AppAndroid(void);
	~AppAndroid(void);
	AAssetManager* m_assetManager;
protected:
	virtual bool loadImage(png::image<png::rgba_pixel>& image, const char* filename);
public:
	static void CrateCallback();
	static void RenderCallback();
	static void ResizeCallback(int width, int height);
	static void DestroyCallback();
	static AppAndroid* getInstance();
	static bool LoadImageFromFile();
};