#pragma once

class PlantGeneratorApp;

class PlantGenerator
{
public:
	enum PlantPart { Leaves = 0, Stalk, Flowers};

	/// Initializes the Plant Generator, must be called after an EGL context is setup.
	static bool InitGenerator();

	///Sets the default values for the combination biases whenever new plants are loaded.
	static void setDefaulBias(float leaves, float stalk, float flowers);

	///Loads selected indices from the database. The combination biases are set
	static void loadPlants(PlantDatabase::PlantData plantOne, PlantDatabase::PlantData plantTwo, PlantDatabase::PlantData plantThree);

	///Combine two plant parts;
	static void setCombination(PlantPart plantPart, int lhs, int rhs, float bias);

	///Render the plant to an image, the width and height might 
	///not allways the ones specified in the fucntion
	static bool RenderPlant(int width, int height);

	/////get the rendered image and the actuall with and height. The caller is responsible for the data
	static const unsigned char* getRenderedImage(unsigned int& width, unsigned int& height);

	static void setAssetManager(JNIEnv * env, jobject mgr);

	static PlantGeneratorApp* getInstance();
private:
	static PlantGeneratorApp* instance;
};