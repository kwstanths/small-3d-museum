#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <string>
#include <exception>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "Scene.h"
#include "PLYReader.h"
#include "ConfigurationFile.hpp"


Scene::Scene()
{
}

Scene::~Scene()
{
    /* TODO Delete map */
}

void Scene::init()
{
	initShaders();

    /* Initialize config file */
    ConfigurationFile& c = ConfigurationFile::instance();

    PLYReader reader;

    /* Create a cube, use it as a wall */
    TriangleMesh * cube = new TriangleMesh(0);
    cube->buildCube();
    cube->ComputeAddInfo();
    cube->sendToOpenGL(basicProgram);
    
    /* Create a tile, use it as a floor */
    TriangleMesh * tile = new TriangleMesh(0);
    tile->buildTile();
    tile->ComputeAddInfo();
    tile->sendToOpenGL(basicProgram);
    /* Scale the floor */
    floor_ = new GraphicsObject(tile, tile, tile, tile);
    floor_->SetScale(glm::vec3(200, 200, 200));
    floor_->SetPosition(glm::vec3(0, -1, 0));

    std::cout << "Reading the bunny..." << std::endl;
    TriangleMesh * bunny = new TriangleMesh(0);
    bool bSuccess = reader.readMesh("models_srgge/bunny.ply", *bunny);
    bunny->ComputeAddInfo();
    if (bSuccess) bunny->sendToOpenGL(basicProgram);
    std::cout << "Clustering the bunny..." << std::endl;
    std::vector<TriangleMesh *> bunny_lods = bunny->VertexClusteringOctree({ 8,7,6 }, TriangleMesh::Metric::QUADRIC_ERROR);
    bunny_lods[0]->sendToOpenGL(basicProgram);
    bunny_lods[1]->sendToOpenGL(basicProgram);
    bunny_lods[2]->sendToOpenGL(basicProgram);

    std::cout << "Reading the horse..." << std::endl;
    TriangleMesh * horse = new TriangleMesh(0);
    bSuccess = reader.readMesh("models_srgge/horse.ply", *horse);
    horse->ComputeAddInfo();
    if (bSuccess) horse->sendToOpenGL(basicProgram);
    std::cout << "Clustering the horse..." << std::endl;
    std::vector<TriangleMesh *> horse_lods = horse->VertexClusteringOctree({ 8,7,6 }, TriangleMesh::Metric::QUADRIC_ERROR);
    horse_lods[0]->sendToOpenGL(basicProgram);
    horse_lods[1]->sendToOpenGL(basicProgram);
    horse_lods[2]->sendToOpenGL(basicProgram);

    std::cout << "Reading the armadillo..." << std::endl;
    TriangleMesh * armadillo = new TriangleMesh(0);
    bSuccess = reader.readMesh("models_srgge/armadillo.ply", *armadillo);
    armadillo->ComputeAddInfo();
    if (bSuccess) armadillo->sendToOpenGL(basicProgram);
    std::cout << "Clustering the armadillo with mean..." << std::endl;
    std::vector<TriangleMesh *> armadillo_mean_lods = armadillo->VertexClusteringOctree({ 8,7,6 }, TriangleMesh::Metric::MEAN);
    armadillo_mean_lods[0]->sendToOpenGL(basicProgram);
    armadillo_mean_lods[1]->sendToOpenGL(basicProgram);
    armadillo_mean_lods[2]->sendToOpenGL(basicProgram);
    std::cout << "Clustering the armadillo with qem..." << std::endl;
    std::vector<TriangleMesh *> armadillo_qem_lods = armadillo->VertexClusteringOctree({ 8,7,6 }, TriangleMesh::Metric::QUADRIC_ERROR);
    armadillo_qem_lods[0]->sendToOpenGL(basicProgram);
    armadillo_qem_lods[1]->sendToOpenGL(basicProgram);
    armadillo_qem_lods[2]->sendToOpenGL(basicProgram);

    std::cout << "Reading the frog..." << std::endl;
    TriangleMesh * frog = new TriangleMesh(0);
    bSuccess = reader.readMesh("models_srgge/frog.ply", *frog);
    frog->ComputeAddInfo();
    if (bSuccess) frog->sendToOpenGL(basicProgram);
    std::cout << "Clustering the frog..." << std::endl;
    std::vector<TriangleMesh *> frog_lods = frog->VertexClusteringOctree({ 8,7,6 }, TriangleMesh::Metric::QUADRIC_ERROR);
    frog_lods[0]->sendToOpenGL(basicProgram);
    frog_lods[1]->sendToOpenGL(basicProgram);
    frog_lods[2]->sendToOpenGL(basicProgram);

    std::cout << "Reading the max planck..." << std::endl;
    TriangleMesh * planck = new TriangleMesh(0);
    bSuccess = reader.readMesh("models_srgge/maxplanck.ply", *planck);
    planck->ComputeAddInfo();
    if (bSuccess) planck->sendToOpenGL(basicProgram);
    std::cout << "Clustering the max planck..." << std::endl;
    std::vector<TriangleMesh *> planck_lods = planck->VertexClusteringOctree({ 8,7,6 }, TriangleMesh::Metric::QUADRIC_ERROR);
    planck_lods[0]->sendToOpenGL(basicProgram);
    planck_lods[1]->sendToOpenGL(basicProgram);
    planck_lods[2]->sendToOpenGL(basicProgram);

    std::cout << "Reading the moai..." << std::endl;
    TriangleMesh * moai = new TriangleMesh(0);
    bSuccess = reader.readMesh("models_srgge/moai.ply", *moai);
    moai->ComputeAddInfo();
    if (bSuccess) moai->sendToOpenGL(basicProgram);
    std::cout << "Clustering the moai with QEM..." << std::endl;
    std::vector<TriangleMesh *> moai_qem_lods = moai->VertexClusteringOctree({ 8,7,6 }, TriangleMesh::Metric::QUADRIC_ERROR);
    moai_qem_lods[0]->sendToOpenGL(basicProgram);
    moai_qem_lods[1]->sendToOpenGL(basicProgram);
    moai_qem_lods[2]->sendToOpenGL(basicProgram);
    std::cout << "Clustering the moai with mean..." << std::endl;
    std::vector<TriangleMesh *> moai_mean_lods = moai->VertexClusteringOctree({ 8,7,6 }, TriangleMesh::Metric::MEAN);
    moai_mean_lods[0]->sendToOpenGL(basicProgram);
    moai_mean_lods[1]->sendToOpenGL(basicProgram);
    moai_mean_lods[2]->sendToOpenGL(basicProgram);

    std::cout << "Reading the happy..." << std::endl;
    TriangleMesh * happy = new TriangleMesh(0);
    bSuccess = reader.readMesh("models_srgge/happy.ply", *happy);
    happy->ComputeAddInfo();
    if (bSuccess) happy->sendToOpenGL(basicProgram);
    std::cout << "Clustering the happy..." << std::endl;
    TriangleMesh * happy_lod_0 = happy->VertexClusteringGrid(128, TriangleMesh::Metric::MEAN);
    TriangleMesh * happy_lod_1 = happy->VertexClusteringGrid(64, TriangleMesh::Metric::MEAN);
    TriangleMesh * happy_lod_2 = happy->VertexClusteringGrid(32, TriangleMesh::Metric::MEAN);
    happy_lod_0->sendToOpenGL(basicProgram);
    happy_lod_1->sendToOpenGL(basicProgram);
    happy_lod_2->sendToOpenGL(basicProgram);

    /* Should I use the dragon or not? */
    bool use_the_dragon = ConfigurationFile::instance().UseDragon();

    TriangleMesh * dragon = nullptr;
    std::vector<TriangleMesh *> dragon_lods;
    if (use_the_dragon) {
        std::cout << "Reading the dragon..." << std::endl;
        dragon = new TriangleMesh(0);
        bSuccess = reader.readMesh("models_srgge/dragon.ply", *dragon);
        dragon->ComputeAddInfo();
        if (bSuccess) dragon->sendToOpenGL(basicProgram);
        std::cout << "Clustering the dragon with QEM..." << std::endl;
        dragon_lods = dragon->VertexClusteringOctree({ 8,7,6 }, TriangleMesh::Metric::QUADRIC_ERROR);
        dragon_lods[0]->sendToOpenGL(basicProgram);
        dragon_lods[1]->sendToOpenGL(basicProgram);
        dragon_lods[2]->sendToOpenGL(basicProgram);
    }

    /* Read the map */
    museum_map_ = ReadMapFromFile("map.txt");
    map_ = std::vector<GraphicsObject *>(MAP_SIZE_X*MAP_SIZE_Y, nullptr);
    for (size_t i = 0; i < MAP_SIZE_X; i++) {
        for (size_t j = 0; j < MAP_SIZE_Y; j++) {
            /*
                1 = wall
                2 = bunny
                ...
            */
            int index = i * MAP_SIZE_Y + j;
            if (museum_map_[i][j] == 1) {
                map_[index] = new GraphicsObject(cube, cube, cube, cube);
                map_[index]->SetScale(glm::vec3(map_scale, map_scale, map_scale));
            }
            if (museum_map_[i][j] == 2) {
                map_[index] = new GraphicsObject(bunny, bunny_lods[0], bunny_lods[1], bunny_lods[2]);
            }
            if (museum_map_[i][j] == 3) {
                map_[index] = new GraphicsObject(horse, horse_lods[0], horse_lods[1], horse_lods[2]);
            }
            if (museum_map_[i][j] == 4) {
                map_[index] = new GraphicsObject(frog, frog_lods[0], frog_lods[1], frog_lods[2]);
            }
            if (museum_map_[i][j] == 5) {
                map_[index] = new GraphicsObject(planck, planck_lods[0], planck_lods[1], planck_lods[2]);
            }
            if (museum_map_[i][j] == 6) {
                map_[index] = new GraphicsObject(moai, moai_qem_lods[0], moai_qem_lods[1], moai_qem_lods[2]);
            }
            if (museum_map_[i][j] == 7) {
                map_[index] = new GraphicsObject(moai, moai_mean_lods[0], moai_mean_lods[1], moai_mean_lods[2]);
            }
            if (museum_map_[i][j] == 8) {
                map_[index] = new GraphicsObject(armadillo, armadillo_qem_lods[0], armadillo_qem_lods[1], armadillo_qem_lods[2]);
            }
            if (museum_map_[i][j] == 9) {
                map_[index] = new GraphicsObject(armadillo, armadillo_mean_lods[0], armadillo_mean_lods[1], armadillo_mean_lods[2]);
            }
            if (museum_map_[i][j] == 10) {
                map_[index] = new GraphicsObject(happy, happy_lod_0, happy_lod_1, happy_lod_2);
            }
            if (museum_map_[i][j] == 11) {
                if (use_the_dragon) map_[index] = new GraphicsObject(dragon, dragon_lods[0], dragon_lods[1], dragon_lods[2]);
                else map_[index] = new GraphicsObject(bunny, bunny_lods[0], bunny_lods[1], bunny_lods[2]);
            }
            
            /* Position is at the center of the cell */
            if (map_[index] != nullptr) map_[index]->SetPosition(glm::vec3(map_scale*i + ((float)map_scale / 2), 0, map_scale*j + ((float)map_scale / 2)));
        }
    }

    /* Read visibility */
    ReadVisibility();

	currentTime = 0.0f;
	
	camera.init(2.0f);
	
	bPolygonFill = true;
	
	// Select which font you want to use
	if(!text.init("fonts/OpenSans-Regular.ttf"))
	//if(!text.init("fonts/OpenSans-Bold.ttf"))
	//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
}

bool Scene::loadMesh(const char *filename)
{
	//PLYReader reader;

	//object_->mesh_->free();
 //   bool bSuccess = reader.readMesh(filename, *(object_->mesh_));
	//if(bSuccess)
	//  object_->mesh_->sendToOpenGL(basicProgram);
	//

    std::cout << "Scene::loadMesh(): Code removed" << std::endl;

	return false;
}

void Scene::update(int deltaTime)
{
    frame_time_ = deltaTime;
	currentTime += deltaTime;
}

void Scene::render()
{
    /* Clear time critical rendering objects */
    tcr_.ClearObjects();

    /* Calculate camera position */
    glm::vec3 camera_position = camera.getPosition();
    int camera_cell_row = camera_position.x / map_scale;
    int camera_cell_column = camera_position.z / map_scale;

    /* Add to time critical rendering all the visible objects from that cell */
    for (size_t i = 0; i < visibility_[camera_cell_row*MAP_SIZE_Y + camera_cell_column].size(); i++) {
        int index = visibility_[camera_cell_row*MAP_SIZE_Y + camera_cell_column][i];
        int row = index / MAP_SIZE_Y;
        int col = index - row * MAP_SIZE_Y;

        /* If it's not null, and it's not a wall, add it for time critical rendering */
        if (map_[index] != nullptr && museum_map_[row][col] != 1) tcr_.AddObject(map_[index]);
    }

    /* 
        Perform the lod calculation, pass the camera position and the rendering time
        Rendering time can be used for real time TPS calculation
    */
    tcr_.Step(camera.getPosition(), rendering_time_);

    basicProgram.use();
    basicProgram.setUniformMatrix4f("projection", camera.getProjectionMatrix());
    basicProgram.setUniformMatrix4f("view", camera.getViewMatrix());
    basicProgram.setUniform1i("bLighting", bPolygonFill ? 1 : 0);
    glm::mat3 normalMatrix;

    /* Draw the floor */
    glm::mat4 model = floor_->GetModel();
    basicProgram.setUniformMatrix4f("model", model);
    normalMatrix = glm::inverseTranspose(model * camera.getViewMatrix());
    basicProgram.setUniformMatrix3f("normalMatrix", normalMatrix);
    basicProgram.setUniform4f("color", 0.95f, 0.75f, 0.8f, 1.0f);
    floor_->GetChosenLodMesh()->render();
    
    /* Render visible objects */
    for (size_t i = 0; i < visibility_[camera_cell_row*MAP_SIZE_Y + camera_cell_column].size(); i++) {
        int index = visibility_[camera_cell_row*MAP_SIZE_Y + camera_cell_column][i];
        if (map_[index] != nullptr) {

            model = map_[index]->GetModel();

            basicProgram.setUniformMatrix4f("model", model);
            normalMatrix = glm::inverseTranspose(model * camera.getViewMatrix());
            basicProgram.setUniformMatrix3f("normalMatrix", normalMatrix);
            
            /* Get LOD color, and draw */
            GraphicsObject::LodLevel mesh_lod_level = map_[index]->GetChosenLodLevel();
            renderPolygonMesh(map_[index]->GetChosenLodMesh(), LodColors[mesh_lod_level]);
        }
    }
    
    text.render(std::to_string(fps_), glm::vec2(20, 20), 16, glm::vec4(0, 0, 0, 1));
}

void Scene::renderPolygonMesh(TriangleMesh * mesh, glm::vec4 color) {
    if (bPolygonFill) {
        basicProgram.setUniform4f("color", color.r, color.g, color.b, color.a);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else
    {
        basicProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(0.5f, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        mesh->render();

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_POLYGON_OFFSET_FILL);
        basicProgram.setUniform4f("color", 0.0f, 0.0f, 0.0f, 1.0f);
    }
    mesh->render();
}

Camera &Scene::getCamera()
{
  return camera;
}

void Scene::switchPolygonMode()
{
  bPolygonFill = !bPolygonFill;
}

void Scene::DisplayFps(size_t fps)
{
    fps_ = fps;
}

void Scene::SetRenderingTime(double secs) {
    rendering_time_ = secs;
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/basic.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/basic.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	basicProgram.init();
	basicProgram.addShader(vShader);
	basicProgram.addShader(fShader);
	basicProgram.link();
	if(!basicProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << basicProgram.log() << endl << endl;
	}
	basicProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::ReadVisibility() {
    std::ifstream infile("visibility.txt");
    
    visibility_ = std::vector<std::vector<int>>(MAP_SIZE_X*MAP_SIZE_Y, std::vector<int>());

    size_t cell = 0;
    std::string line;
    while (std::getline(infile, line)) {
        
        std::vector<std::string> line_split = split(line, " ");
        for (size_t str = 0; str < line_split.size(); str++) {
            try {
                visibility_[cell].push_back(std::stoi(line_split[str]));
            } catch (...) {
                /* linux doesn't agree with text files created in Windows environments... */
            }
        }
        cell++;
    }

}

