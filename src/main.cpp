#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <math.h>
#include <iostream>
#include <vector>
#include "level.cpp"

using namespace sf;
Texture texture;
Font font;
Level level(64, 16, 64);
RenderWindow window;
bool running = true;
bool mouseGrab = true;
bool hasBreakBlock = false;
int blockRenderRadius = 18;
int selectedBlock = 1;
float x = 5;
float y = 5;
float z = 5;
float yaw = 0;
float pitch = 0;
float rotateSpeed = 0.05;
float moveSpeed = 0.005;
float mousedx, mousedy = 0;
float fov = 2;
Vector3i placeBlock(0, 0, 0);
Vector3i breakBlock(0, 0, 0);
float oneTile = 1.f/16.f;
std::vector<FloatRect> textureRects = {FloatRect(oneTile, 0, oneTile, oneTile), //Камень
	FloatRect(0, 0, oneTile, oneTile), //Трава
	FloatRect(oneTile*2, 0, oneTile, oneTile), //Земля
	FloatRect(0, oneTile, oneTile, oneTile), //Булыжник
	FloatRect(oneTile*4, 0, oneTile, oneTile), //Доски
	FloatRect(oneTile*4, oneTile, oneTile, oneTile), //Бревно
	FloatRect(oneTile*6, 0, oneTile, oneTile), //Отшлифованный камень
	FloatRect(oneTile*7, 0, oneTile, oneTile), //Кирпичи
	FloatRect(oneTile*5, oneTile*2, oneTile, oneTile), //Обсидиан
	FloatRect(oneTile*4, oneTile*3, oneTile, oneTile), //Каменный кирпич
	FloatRect(oneTile, oneTile, oneTile, oneTile), //Бедрок
	FloatRect(oneTile*2, oneTile, oneTile, oneTile), //Песок
	FloatRect(oneTile*3, oneTile, oneTile, oneTile) //Гравий
	};

std::vector<string> debugInfo = {"FPS:", "X Y Z: 0 0 0", "Yaw Pitch:", "FOV:"};
bool showDebug = false;

Sprite selectedBlockSprite;

void gluPerspective(float fovy, float aspect, float near, float far) {
	float bottom = -near * (float) tan(fovy / 2);
	float top = -bottom;
	float left = aspect * bottom;
	float right = -left;
	glFrustum(left, right, bottom, top, near, far);
}

void renderCube(int x=-0.5, int y=-0.5, int z=-0.5, bool front = true, bool back = true, bool left = true, bool right = true, bool up = true, bool down = true, FloatRect blockTexture = FloatRect()){
	//Texture::bind(&texture);
	float l = blockTexture.left;
	float t = blockTexture.top;
	float w = blockTexture.width;
	float h = blockTexture.height;
	glBegin(GL_QUADS);
	//Front
	glColor3f(0.9f, 0.9f, 0.9f);
	if(front){
	glTexCoord2f(l, t); glVertex3f(x, y+1, z+1);
	glTexCoord2f(l, t+h); glVertex3f(x, y, z+1);
	glTexCoord2f(l+w, t+h); glVertex3f(x+1, y, z+1);
	glTexCoord2f(l+w, t); glVertex3f(x+1, y+1, z+1);}
	//Back
	if(back){
	glTexCoord2f(l, t); glVertex3f(x+1, y+1, z);
	glTexCoord2f(l, t+h); glVertex3f(x+1, y, z);
	glTexCoord2f(l+w, t+h); glVertex3f(x, y, z);
	glTexCoord2f(l+w, t); glVertex3f(x, y+1, z);}
	//Left
	if(left){
	glTexCoord2f(l, t); glVertex3f(x, y+1, z);
	glTexCoord2f(l, t+h); glVertex3f(x, y, z);
	glTexCoord2f(l+w, t+h); glVertex3f(x, y, z+1);
	glTexCoord2f(l+w, t); glVertex3f(x, y+1, z+1);}
	//Right
	if(right){
	glTexCoord2f(l, t); glVertex3f(x+1, y+1, z+1);
	glTexCoord2f(l, t+h); glVertex3f(x+1, y, z+1);
	glTexCoord2f(l+w, t+h); glVertex3f(x+1, y, z);
	glTexCoord2f(l+w, t); glVertex3f(x+1, y+1, z);}
	//Up
	if(up){
	glTexCoord2f(l, t); glVertex3f(x, y+1, z);
	glTexCoord2f(l, t+h); glVertex3f(x, y+1, z+1);
	glTexCoord2f(l+w, t+h); glVertex3f(x+1, y+1, z+1);
	glTexCoord2f(l+w, t); glVertex3f(x+1, y+1, z);}
	//Down
	if(down){
	glTexCoord2f(l, t); glVertex3f(x+1, y, z+1);
	glTexCoord2f(l, t+h); glVertex3f(x, y, z+1);
	glTexCoord2f(l+w, t+h); glVertex3f(x, y, z);
	glTexCoord2f(l+w, t); glVertex3f(x+1, y, z);}

	glEnd();
	//Texture::bind(NULL);
}

void drawSelectedBlock(){
	glPushMatrix();
	glScalef(1.01, 1.01, 1.01);
	glTranslatef(-0.5, -0.5, -0.5);
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	//Нижняя часть
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);

	glVertex3f(1, 0, 0);
	glVertex3f(1, 0, 1);

	glVertex3f(1, 0, 1);
	glVertex3f(0, 0, 1);

	glVertex3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	//Верхняя часть
	glVertex3f(0, 1, 0);
	glVertex3f(1, 1, 0);

	glVertex3f(1, 1, 0);
	glVertex3f(1, 1, 1);

	glVertex3f(1, 1, 1);
	glVertex3f(0, 1, 1);

	glVertex3f(0, 1, 1);
	glVertex3f(0, 1, 0);
	//Боковые части
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);

	glVertex3f(1, 0, 0);
	glVertex3f(1, 1, 0);

	glVertex3f(0, 0, 1);
	glVertex3f(0, 1, 1);

	glVertex3f(1, 0, 1);
	glVertex3f(1, 1, 1);
	glEnd();
	glPopMatrix();
}

void getPlaceAndBreakBlock(){
	float startX = x;
	float startY = y;
	float startZ = z;
	int blockX, blockY, blockZ, oldX, oldY, oldZ;
	int distance = 0;
	oldX = (int) floor(startX);
	oldY = (int) floor(startY);
	oldZ = (int) floor(startZ);
	while(distance < 140) {
		startX += 0.025*sin(yaw/180*M_PI)*cos(pitch/180*M_PI); blockX = (int) floor(startX);
		startY -= 0.025*sin(pitch/180*M_PI); blockY = (int) floor(startY);
		startZ -= 0.025*cos(yaw/180*M_PI)*cos(pitch/180*M_PI); blockZ = (int) floor(startZ);
		if(level.getBlock(blockX, blockY, blockZ)){
			breakBlock.x = blockX;
			breakBlock.y = blockY;
			breakBlock.z = blockZ;
			placeBlock.x = oldX;
			placeBlock.y = oldY;
			placeBlock.z = oldZ;
			hasBreakBlock = true;
			return;
		}
		oldX = blockX; oldY = blockY; oldZ = blockZ;
		distance += 1;
	}
	hasBreakBlock = false;
}

void updateSelectedBlockSprite(){
	FloatRect rect = textureRects.at(selectedBlock-1);
	Vector2u size = texture.getSize();
	selectedBlockSprite.setTextureRect(IntRect(size.x*rect.left, size.y*rect.top, size.x*rect.width, size.y*rect.height));
}

void applyFOV(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //Без этой функции gluPerspective даст пустой экран + он должен выполняется перед ним обязательно
	gluPerspective(fov, (float) window.getSize().x / (float) window.getSize().y, 0.001f, 1000);
	glMatrixMode(GL_MODELVIEW);
}

int main() {
	texture.loadFromFile("assets/terrain.png");
	font.loadFromFile("assets/font.ttf");
	ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	window.create(VideoMode(1300, 700), "govnocraft copyright Artur Latipov", Style::Default, settings);
	window.setMouseCursorGrabbed(true);
	window.setMouseCursorVisible(false);
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);
	glViewport(0, 0, window.getSize().x, window.getSize().y);
	applyFOV();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_DENSITY, 0.15f);
	glHint(GL_FOG_HINT, GL_FASTEST);
	glFogf(GL_FOG_START, 1.0f);
	glFogf(GL_FOG_END, 40.0f);
	GLfloat fogColor[4] = {0, 0.5f, 1.f, 1.f};
	glFogfv(GL_FOG_COLOR, fogColor);
	Texture::bind(&texture);
	//Создание платформы
	for(int i = 0; i < level.getWidth(); i++){
		for(int j = 0; j < level.getDepth(); j++){
			level.setBlock(i, 0, j, 2);
		}
	}
	Vector2i lastMousePos = Mouse::getPosition(window);
	CircleShape cross(3);
	cross.setFillColor(Color(0, 0, 0));
	cross.setOrigin(1.5, 1.5);
	cross.setPosition(window.getSize().x/2, window.getSize().y/2);
	selectedBlockSprite.setTexture(texture);
	selectedBlockSprite.setScale(Vector2f(3, 3));
	updateSelectedBlockSprite();
	Text text("", font, 20);
	//text.setStyle(1); //1 - Bold
	Clock clock;
	while(running){
		float dtime = clock.getElapsedTime().asMicroseconds();
		float time = dtime/1000;
		clock.restart();
		//Высчитывание ломающего блока и ставящего
		getPlaceAndBreakBlock();
		Event event;
		while(window.pollEvent(event)){
			if(event.type == Event::Closed){
				running = false;
			}
			else if(event.type == Event::Resized){
				window.setView(View(Vector2f(event.size.width/2, event.size.height/2), Vector2f(event.size.width, event.size.height)));
				glViewport(0, 0, event.size.width, event.size.height);
				applyFOV();
				cross.setPosition(window.getSize().x/2, window.getSize().y/2);
			}
			else if(event.type == Event::KeyPressed){
				if(event.key.code == Keyboard::Q){
					running = false;
				}
				if(event.key.code == Keyboard::Escape){
					mouseGrab = !mouseGrab;
					window.setMouseCursorGrabbed(mouseGrab);
					window.setMouseCursorVisible(!mouseGrab);
				}
				if(event.key.code == Keyboard::Z){
					blockRenderRadius += 1;
				}
				if(event.key.code == Keyboard::X){
					blockRenderRadius -= 1;
				}
				if(event.key.code == Keyboard::F3){
					showDebug = !showDebug;
				}
				if(event.key.code == Keyboard::N){
					fov -= 0.01;
					applyFOV();
				}
				if(event.key.code == Keyboard::M){
					fov += 0.01;
					applyFOV();
				}
			}else if(event.type == Event::MouseButtonPressed){
				if(mouseGrab){
					if(event.mouseButton.button == Mouse::Left){
						if(hasBreakBlock) level.setBlock(breakBlock.x, breakBlock.y, breakBlock.z, 0);
					}
					if(event.mouseButton.button == Mouse::Right){
						if(hasBreakBlock) {
							if(level.hasBlock(placeBlock.x, placeBlock.y, placeBlock.z)) level.setBlock(placeBlock.x, placeBlock.y, placeBlock.z, selectedBlock);
						}
					}
					if(event.mouseButton.button == Mouse::Middle){
						if(hasBreakBlock) {
							selectedBlock = level.getBlock(breakBlock.x, breakBlock.y, breakBlock.z);
							updateSelectedBlockSprite();
						}
					}
				}
			}else if(event.type == Event::MouseWheelScrolled){
				if(mouseGrab){
					if(event.mouseWheelScroll.delta == 1){
						if((int) textureRects.size() > selectedBlock) selectedBlock += 1;
						updateSelectedBlockSprite();
					}
					if(event.mouseWheelScroll.delta == -1){
						if(selectedBlock > 1) selectedBlock -= 1;
						updateSelectedBlockSprite();
					}
				}
			}
		}
		if(mouseGrab) {
			if(Keyboard::isKeyPressed(Keyboard::W)){
				x += sin(yaw/180*M_PI)*moveSpeed*time;
				z -= cos(yaw/180*M_PI)*moveSpeed*time;
			}
			if(Keyboard::isKeyPressed(Keyboard::S)){
				x -= sin(yaw/180*M_PI)*moveSpeed*time;
				z += cos(yaw/180*M_PI)*moveSpeed*time;
			}
			if(Keyboard::isKeyPressed(Keyboard::D)){
				x -= sin((yaw-90)/180*M_PI)*moveSpeed*time;
				z += cos((yaw-90)/180*M_PI)*moveSpeed*time;
			}
			if(Keyboard::isKeyPressed(Keyboard::A)){
				x -= sin((yaw+90)/180*M_PI)*moveSpeed*time;
				z += cos((yaw+90)/180*M_PI)*moveSpeed*time;
			}
			if(Keyboard::isKeyPressed(Keyboard::Space)){
				y += moveSpeed*time;
			}
			if(Keyboard::isKeyPressed(Keyboard::LShift)){
				y -= moveSpeed*time;
			}
			yaw += rotateSpeed*mousedx;
			pitch += rotateSpeed*mousedy;
			if(yaw < 0){
				yaw = 360 + yaw;
			}
			if(yaw > 360){
				yaw = yaw - 360;
			}
			if(pitch > 90){
				pitch = pitch - (pitch - 90);
			}
			if(pitch < -90){
				pitch = pitch - (pitch + 90);
			}
		}
		glClearColor(0, 0.5f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glLoadIdentity();
		glPushMatrix();
		glRotatef(pitch, 1, 0, 0); //Pitch
		glRotatef(yaw, 0, 1, 0); //Yaw
		glTranslatef(-x, -y, -z); //Координаты игрока
		//Texture::bind(&texture);
		for(int i = ((int) floor(x))-blockRenderRadius; i < ((int) floor(x))+blockRenderRadius; i++){
			for(int j = ((int) floor(y))-blockRenderRadius; j < ((int) floor(y))+blockRenderRadius; j++){
				for(int k = ((int) floor(z))-blockRenderRadius; k < ((int) floor(z))+blockRenderRadius; k++){
					u_short blockType = level.getBlock(i, j, k);
					if(blockType){
						//glTranslatef(i+0.5, j+0.5, k+0.5);
						//glTranslatef(i, j, k);
						renderCube(i, j, k,
							!level.getBlock(i, j, k+1), !level.getBlock(i, j, k-1),
							!level.getBlock(i-1, j, k), !level.getBlock(i+1, j, k),
							!level.getBlock(i, j+1, k), !level.getBlock(i, j-1, k), textureRects.at(blockType-1));
						//glTranslatef(-i, -j, -k);
						//glTranslatef(-i-0.5, -j-0.5, -k-0.5);
					}
				}
			}
		}
		//Texture::bind(NULL);
		/*for(int i = 0; i < 50; i++){
			for(int j = 0; j < 50; j++){
				renderCube(i, 0, j, false, false, false, false, true, false);
				glTranslatef(i, 0, j);
				glPushMatrix();
				glBegin(GL_QUADS);
				glColor3f(1, 1, 1);
				glVertex3f(i+0, 0, j+0);
				glVertex3f(i+0, 0, j+1);
				glVertex3f(i+1, 0, j+1);
				glVertex3f(i+1, 0, j);
				glVertex3f(0, 0, 0);
				glVertex3f(0, 0, 1);
				glVertex3f(1, 0, 1);
				//glEnd();
				//glPopMatrix();
				//glTranslatef(-i, 0, -j);
			}
		}*/
		glPushMatrix();
		if(hasBreakBlock) {
			glTranslatef(breakBlock.x+0.5, breakBlock.y+0.5, breakBlock.z+0.5);
			drawSelectedBlock();
			glTranslatef(breakBlock.x-0.5, breakBlock.y-0.5, breakBlock.z-0.5);
		}
		glPopMatrix();
		/*glTranslatef(-1+0.5, -1+0.5, -1+0.5);
		renderCube(true, false, true, false, false, false);
		glTranslatef(-1-0.5, -1-0.5, -1-0.5);*/

		glPopMatrix();
		window.pushGLStates();
		//SFML draws...
		selectedBlockSprite.setPosition(window.getSize().x - selectedBlockSprite.getGlobalBounds().width - 10, 10);
		window.draw(selectedBlockSprite);
		window.draw(cross);
		if(showDebug){
			debugInfo[0] = "FPS: " + std::to_string((float)1/(dtime/(float)1000000));
			debugInfo[1] = "X Y Z: " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
			debugInfo[2] = "Yaw Pitch: " + std::to_string(yaw) + " " + std::to_string(pitch);
			debugInfo[3] = "FOV: " + std::to_string(fov);
			for(int i = 0; i < (int) debugInfo.size(); i++){
				text.setPosition(5, 5+(text.getLocalBounds().height+2)*i);
				text.setString(debugInfo.at(i));
				window.draw(text);
			}
		}
		window.popGLStates();
		Vector2i delta = Mouse::getPosition(window)-lastMousePos;
		mousedx = delta.x;
		mousedy = delta.y;
		if(mouseGrab) Mouse::setPosition(Vector2i(window.getSize().x/2, window.getSize().y/2), window);
		lastMousePos = Mouse::getPosition(window);
		window.display();
	}
	Texture::bind(NULL);
}
