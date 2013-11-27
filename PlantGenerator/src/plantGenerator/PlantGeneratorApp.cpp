#include "Definitions.h"
#include "App.h"
#include<PlantDatabase.h>
#include"PlantGeneratorApp.h"
#include<png.hpp>
#include<sstream>

PlantGeneratorApp::PlantGeneratorApp() : App()
{
}

bool PlantGeneratorApp::loadImage(png::image<png::rgba_pixel>& image, const char* filename)
{
	AAsset* asset = AAssetManager_open(m_assetManager,filename,AASSET_MODE_STREAMING);
	LOGI("LOAD IMAGE:: START");
	if(asset == NULL){
		LOGE("IMAGE LOADING FAIL: FAILED TO LOAD INITAL ASSET");	
		return false;
	}

	off_t start, length;
	int ret = AAsset_openFileDescriptor(asset, &start, &length);
	if(ret < 0){
		LOGE("IMAGE LOADING FAIL: FILE DESCRIPTOR IS NOT OPEN!");	
		AAsset_close(asset);
		return false;
	}
	std::vector<char> buff(length,0);
	ret = AAsset_read(asset,buff.data(),length);
	if(ret != length)
	{
		LOGE("IMAGE LOADING FAIL: READING FAIL!");	
		AAsset_close(asset);
		return false;
	}
	try{
		std::istringstream stream;
		stream.rdbuf()->pubsetbuf(buff.data(),buff.size());
		image.read_stream(stream);
	}
	catch(...)
	{
		LOGE("IMAGE LOADING FAIL: LOADING ERROR!");	
	}
	AAsset_close(asset);

	LOGI("LOAD IMAGE::END");
	return true;
}