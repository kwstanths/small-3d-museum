#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>

#include "Camera.h"
#include "ShaderProgram.h"
#include "TriangleMesh.h"
#include "Text.h"
#include "Map.hpp"
#include "GraphicsObject.hpp"
#include "TimeCriticalRendering.hpp"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	bool loadMesh(const char *filename);
	void update(int deltaTime);
	void render();
    void renderPolygonMesh(TriangleMesh * mesh, glm::vec4 color = glm::vec4(0.9f, 0.9f, 0.95f, 1.0f));

  Camera &getCamera();
  
  void switchPolygonMode();

  void DisplayFps(size_t fps);
  void SetRenderingTime(double secs);

private:
	void initShaders();

private:
    Camera camera;
    int ** museum_map_;
    std::vector<GraphicsObject *> map_;

    std::vector<std::vector<int>> visibility_;
    TimeCriticalRendering tcr_;

    GraphicsObject * floor_;

	ShaderProgram basicProgram;
	float currentTime;
    
    int frame_time_;

    double rendering_time_;
	
	bool bPolygonFill;
	
	Text text;

    size_t fps_;

    void ReadVisibility();

};


#endif // _SCENE_INCLUDE

