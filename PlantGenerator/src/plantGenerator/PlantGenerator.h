#pragma once

class PlantGeneratorApp;

class PlantGenerator
{
public:
	/// Initializes the Plant Generator, must be called after an EGL context is setup.
	static bool InitGenerator(unsigned int width, unsigned int height);

	///Sets the default values for the combination biases whenever new plants are loaded.
	static void setDefaulBias(float leaves, float stalk, float flowers);

	///Loads selected indices from the database. The combination biases are set
	static void loadPlants(PlantDatabase::PlantData plantOne, PlantDatabase::PlantData plantTwo, PlantDatabase::PlantData plantThree);

	///load a specific plant to a selected index
	static void loadPlant(PlantDatabase::PlantData plant, int index);

	///Combine two plant parts;
	static void setCombination(uint plantPart, int l_index, int r_index, float bias);

	///Render the plant to an image, the width and height might 
	///not allways the ones specified in the fucntion
	static bool RenderPlant(int width, int height);

	/////get the rendered image and the actuall with and height. The caller is responsible for the data
	static unsigned char* getRenderedImage(unsigned int& width, unsigned int& height);

	static void setAssetManager(JNIEnv * env, jobject mgr);

	static PlantGeneratorApp* getInstance();
private:
	static void* m_pContext;
	static void* m_pDisplay;
	static void* m_pSurface;
	static PlantGeneratorApp* instance;
};