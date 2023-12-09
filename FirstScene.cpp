#include "FirstScene.h"
#include "Player.h"
#include "Camera.h"
#include "Ground.h"
#include "Obstacle.h"
#include "Skybox.h"
#include "GameText.h"
#include <glut.h>

const int zMin = -100, zMax = 100;
const int xMin = -10, xMax = 10;
const int yPosition = 0; 

FirstScene::FirstScene()
{
	gameObjects["player"] = (new Player())
		->setPosition({ 0, 15, -90 });

	gameObjects["ground"] = (new Ground());

	gameObjects["obstacle1"] = (new Obstacle)
		->setPosition({ 0, 5, 70 });
	gameObjects["obstacle2"] = (new Obstacle)
		->setPosition({ 0, 5, 60 });
	gameObjects["obstacle3"] = (new Obstacle)
		->setPosition({ 0, 5, 40 });
	gameObjects["obstacle4"] = (new Obstacle)
		->setPosition({ 0, 5, 10 });
	gameObjects["obstacle5"] = (new Obstacle)
		->setPosition({ 0, 5, 20 });

	gameObjects["camera"] = (new Camera({ 0, 20, -105 }, { 0, 0, 0 }, { 0, 1, 0 }, 1));


	gameObjects["skybox"] = (new Skybox);

	gameObjects["gameText"] = (new GameText);


    for (int i = 0; i < 7; i++) {

        float xPosition = xMin + (xMax - xMin) / 6 * i;
        float zPosition = zMin + (zMax - zMin) / 6 * i;

        gameObjects["obstacle" + std::to_string(i)] = (new Obstacle())
            ->setPosition({
                xPosition, 
                yPosition,
                zPosition 
            });
    }


	
}

void FirstScene::onIdle()
{
	GameScene::onIdle();

	Player *player = getGameObjectByTag<Player>("player");
	Camera* camera = getGameObjectByTag<Camera>("camera");
	GameText* gameText = getGameObjectByTag<GameText>("gameText");

	if (is3rdPerson) {
		camera->setCenter(player->getPosition() + Vector3f(0, 0, 5));
		camera->setEye(player->getPosition() + Vector3f(0, 5, -10));
	}
	else {
		camera->setCenter(player->getPosition() + Vector3f(0, 5, 5));
		camera->setEye(player->getPosition() + Vector3f(0, 5, 2));
	}

	gameText->setPosition(player->getPosition() + Vector3f(50, 10, 100));
	gameText->setText("Score: " + std::to_string(Game::getInstance()->getScore()));
}



void FirstScene::onSpecialKeyPressed(int key, int x, int y)
{
	GameScene::onSpecialKeyPressed(key, x, y);
	Player *player = getGameObjectByTag<Player>("player");
}

void FirstScene::onKeyPressed(unsigned char key, int x, int y)
{
	GameScene::onKeyPressed(key, x, y);

	if (key == '1') {
		is3rdPerson = !is3rdPerson;
	}
}

void FirstScene::setupLights()
{
	double p = hourOfDay / 48;
	
	//glClearColor(127.0 / 255.0 * p, 207.0 / 255.0 * p, 255.0 / 255.0 * p, 0.0f);

	GLfloat light_position[] = { 0.0, 100.0, 0.0, 1.0 };
	GLfloat light_ambient[] = { p, p, p, 0 };
	GLfloat light_diffuse[] = { 127.0 / 255.0 * p, 207.0 / 255.0 * p, 255.0 / 255.0 * p, 0.0f };
	GLfloat light_specular[] = { p, p, p, 0.0f };
	GLfloat shininess[] = { 0.5 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SHININESS, shininess);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHT0);


	Player* player = getGameObjectByTag<Player>("player");

	// Spotlight
	GLfloat light_position1[] = {
		player->getPosition().getX(),
		player->getPosition().getY() + 25,
		player->getPosition().getZ() + 20,
		1.0};
	GLfloat light_ambient1[] = { 0.1, 0.1, 0.1, 0.0 };
	GLfloat light_diffuse1[] = { 242.0 / 255.0, 235.0 / 255.0, 104.0 / 255.0, 0.0 }; // Increase diffuse intensity
	GLfloat light_specular1[] = { 0.2, 0.2, 0.2, 0.0 }; // Increase specular intensity

	GLfloat spot_direction[] = { 0.0, -2.0, 1 };

	// Set spotlight properties
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT1, GL_SHININESS, shininess);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1);

	// Enable lighting and light1
	glEnable(GL_LIGHT1);
}

void FirstScene::onTimer(int value)
{
	GameScene::onTimer(value);

	hourOfDay += timeDirection;
	if (hourOfDay == 24) {
		timeDirection = -1;
	}
	else if (hourOfDay == 0) {
		timeDirection = 1;
	}
}



