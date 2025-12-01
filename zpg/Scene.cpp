#include "Scene.h"

Scene::~Scene()
{
    for(auto we : mWorldEntities)
    {
        delete we;
    }
}

void Scene::addEntity(WorldEntity* entity)
{
    mWorldEntities.push_back(entity);

    if (Camera* camera = dynamic_cast<Camera*>(entity)) {
        mActiveCamera = camera;
    }
}


void Scene::render(GLFWwindow* window, float deltaTime, Controls* controls)
{
    if (mSkyBox != nullptr)
    {
        mSkyBox->update(window, deltaTime, controls);
        mSkyBox->draw();
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    selectPointsForBezier(controls);
    spawnBezierEntity(controls);
	setSelectedEntity(controls);
	insertEntity(controls);
	removeEntity(controls);
	removeAndInsertEntity(controls);
    transformEntity(controls); 


    for (auto entity : mWorldEntities)
    {   
        glStencilFunc(GL_ALWAYS, entity->getStencilIndex(), 0xFF);
        entity->update(window, deltaTime, controls);
        entity->draw();
    }

    if (mActiveCamera)
		mActiveCamera->notifyObservers();
}

void Scene::setSelectedEntity(Controls* controls)
{
    if (controls->isMouseButtonTriggered(GLFW_MOUSE_BUTTON_LEFT))
    {
        for (auto entity : mWorldEntities)
        {
            if (entity->getStencilIndex() == controls->getStencilIndex() && controls->getStencilIndex() != 0)
            {
                mEntitityToHandle = entity;
                this->mWorldEntityGenerator->setSampleEntity(entity);
                break;
            }
		}
    }
}

void Scene::removeEntity(Controls* controls)
{
    if (controls->isKeyTriggered(GLFW_KEY_DELETE) && mEntitityToHandle != nullptr)
    {
        auto it = find(mWorldEntities.begin(), mWorldEntities.end(), mEntitityToHandle);
        if (it != mWorldEntities.end())
        {
            delete *it;
            mWorldEntities.erase(it);
            mEntitityToHandle = nullptr;
        }
	}
}

void Scene::removeAndInsertEntity(Controls* controls)
{
    if (controls->isKeyTriggered(GLFW_KEY_ENTER))
    {
        for (auto entity : mWorldEntities)
        {
            if (entity->getStencilIndex() == controls->getStencilIndex() && controls->getStencilIndex() != 0)
            {
                mEntitityToHandle = entity;
                break;
            }
        }

        if (mEntitityToHandle != nullptr)
        {
            auto it = find(mWorldEntities.begin(), mWorldEntities.end(), mEntitityToHandle);
            if (it != mWorldEntities.end())
            {
                delete* it;
                mWorldEntities.erase(it);
                mEntitityToHandle = nullptr;
                this->mWorldEntities.insert(mWorldEntities.begin(), this->getWorldEntityGenerator()->generateEntityRandomly());
            }
        }
    }
}

void Scene::insertEntity(Controls* controls)
{
    if (controls->isKeyTriggered(GLFW_KEY_I) && mEntitityToHandle != nullptr)
    {
        WorldEntity* duplicate = mEntitityToHandle->getCopy();
        duplicate->nullifyTranslation();
        duplicate->addStaticTransform(new Translation(controls->getPosition()));
		this->mWorldEntities.insert(mWorldEntities.begin(), duplicate);
    }
}

void Scene::transformEntity(Controls* controls)
{
    if (mEntitityToHandle != nullptr)
    {
        if (controls->isKeyTriggered(GLFW_KEY_T))
        {
            vec3 mousePosition = controls->getPosition();
            vec3 currentPosition = vec3(mEntitityToHandle->getTransformManager()->getFinalMatrix()[3]);
            vec3 direction = mousePosition - currentPosition;
            mEntitityToHandle->addStaticTransform(new Translation(direction));
        }
        if (controls->isKeyPressed(GLFW_KEY_R))
        {
            if (controls->isKeyPressed(GLFW_KEY_X))
            {
                mEntitityToHandle->addStaticTransform(new Rotation(vec3(1.0f, 0.0f, 0.0f)));
            }
            if (controls->isKeyPressed(GLFW_KEY_Y))
            {
                mEntitityToHandle->addStaticTransform(new Rotation(vec3(0.0f, 0.0f, 1.0f)));
            }
            if (controls->isKeyPressed(GLFW_KEY_Z))
            {
                mEntitityToHandle->addStaticTransform(new Rotation(vec3(0.0f, 1.0f, 0.0f)));
            }
        }
        if(controls->isKeyPressed(GLFW_KEY_UP))
        mEntitityToHandle->addStaticTransform(new Scale(vec3(1.01f, 1.01f, 1.01f)));
        if (controls->isKeyPressed(GLFW_KEY_DOWN))
        mEntitityToHandle->addStaticTransform(new Scale(vec3(0.99f, 0.99f, 0.99f)));
    }
}

void Scene::selectPointsForBezier(Controls* controls)
{
    if (controls->isKeyTriggered(GLFW_KEY_B))
    {
        vec3 point = controls->getPosition();
        mWorldEntityGenerator->addPointForBezier(point);
    }
}

void Scene::spawnBezierEntity(Controls* controls)
{
    if (controls->isKeyTriggered(GLFW_KEY_N))
    {
        WorldEntity* bezierEntity = mWorldEntityGenerator->generateBezierEntity();
        if (bezierEntity != nullptr)
        {
            this->mWorldEntities.insert(mWorldEntities.begin(), bezierEntity);
            this->mWorldEntityGenerator->clearPointsForBezier();
        }
    }
}