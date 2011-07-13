/*
 *  CPhysics.h
 *  Physics wrapper using Box2D
 *
 *  Created by Marcelo Cohen on 05/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#include "CPhysics.h"
#include <iostream>
#define _USE_MATH_DEFINES 1
#include <math.h>

using namespace std;

float CPhysics::CONV = 10;
const float CPhysics::timeStep = 1.0f / 30.f;
CPhysics CPhysics::m_CPhysics;

void CPhysics::setConvFactor(float conv) {
	CONV = conv;
	cout << "CPhysics::setConvFactor" << conv<<endl;
}

CPhysics::CPhysics() {
	// Init Box2D world
	b2Vec2 gravity(0,10.0f);
	bool doSleep = true; // bodies can sleep (saves time)
	world = new b2World(gravity, doSleep);
	//Pinho
	//offsetX = offsetY = 0;
}

CPhysics::~CPhysics() {
	cout << "CPhisics: cleanup" << endl;
	delete world;
}

// Add a new box and associate a Image to it
// Pinho: acrescentei dois parâmetros: linearDamping e angularDamping
b2Body* CPhysics::newBoxImage(int id, CImage* image, float density, float friction, float restitution, float linearDamping, float angularDamping, bool staticObj) {
	b2BodyDef bd;
	if(!staticObj)
		bd.type = b2_dynamicBody;
	float scale = image->getScale();
	float width = image->getWidth()*scale/CONV;
	float height = image->getHeight()*scale/CONV;

	cout << "Physics::newBoxImage " << image->getX() << "," << image->getY() << " - " << image->getWidth() << " x " << image->getHeight() << endl;

	b2PolygonShape box;

	box.SetAsBox(width/2,height/2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;

	//Pinho
	b2Vec2 pos;
	pos.x = image->getX() + width*CONV/2;
	pos.y = image->getY() + height*CONV/2;
	bd.position.Set(pos.x/CONV, pos.y/CONV);
	//bd.position.Set(image->getX()/CONV,image->getY()/CONV);

	bd.angle = image->getRotation() * M_PI/180 ; // convert to radian

	//Pinho
	bd.linearDamping = linearDamping;
	bd.angularDamping = angularDamping;

	b2Body* body = world->CreateBody(&bd);

	body->CreateFixture(&fixtureDef);

	BodyData* bodyData = new BodyData;
	bodyData->id = id;
	bodyData->image = image;
	bodyData->color = b2Color(0,0,0);
	bodyData->size = b2Vec2(width*CONV,height*CONV);
	bodyData->offset = b2Vec2(width*CONV/2/scale,height*CONV/2/scale);
	body->SetUserData(bodyData);

	return body;
}

// Add new box to world
// Pinho: acrescentei dois parâmetros: linearDamping e angularDamping
b2Body* CPhysics::newBox(int id, float x, float y, float width, float height, float rotation, float density, float friction, float restitution, float linearDamping, float angularDamping, bool staticObj) {
	b2BodyDef bd;
	if(!staticObj)
		bd.type = b2_dynamicBody;

	cout << "Physics::newBox " << x << "," << y << " - " << width << " x " << height << endl;

	b2PolygonShape box;
	width = width/CONV;
	height = height/CONV;
	//Pinho
	box.SetAsBox(width/2,height/2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;

	bd.position.Set(x/CONV,y/CONV);
	//Pinho
	bd.angle = rotation * M_PI/180 ; // convert to radian
	//Pinho
	bd.linearDamping = linearDamping;
	bd.angularDamping = angularDamping;

	b2Body* body = world->CreateBody(&bd);

	body->CreateFixture(&fixtureDef);

	BodyData* bodyData = new BodyData;
	bodyData->id = id;
	bodyData->image = NULL;
	bodyData->color = b2Color(0,0,0);
	bodyData->size = b2Vec2(width*CONV, height*CONV);
	bodyData->offset = b2Vec2(width*CONV/2, height*CONV/2);
	body->SetUserData(bodyData);

	return body;
}

// Add a new circle and associate a Image to it
b2Body* CPhysics::newCircleImage(int id, CImage* image, float density, float friction, float restitution, float linearDamping, float angularDamping, bool staticObj) {
	b2BodyDef bd;
	if(!staticObj)
		bd.type = b2_dynamicBody;
	float scale = image->getScale();
	float width = image->getWidth()*scale/CONV;
	float height = image->getHeight()*scale/CONV;

	// Assume radius as the longest dimension
	float radius = max(width,height);
	radius/=2;

	cout << "Physics::newCircleImage " << image->getX() << "," << image->getY() << " - " << radius << endl;

	b2CircleShape cs;
	cs.m_radius = radius;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &cs;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;

	//Pinho
	b2Vec2 pos;
	pos.x = image->getX() + radius*CONV/2;
	pos.y = image->getY() + radius*CONV/2;
	bd.position.Set(pos.x/CONV, pos.y/CONV);
	//bd.position.Set(image->getX()/CONV,image->getY()/CONV);

	b2Body* body = world->CreateBody(&bd);

	body->CreateFixture(&fixtureDef);

	BodyData* bodyData = new BodyData;
	bodyData->id = id;
	bodyData->image = image;
	bodyData->color = b2Color(0,0,0);
	bodyData->size = b2Vec2(radius,radius);
	bodyData->offset = b2Vec2(radius*CONV/scale,radius*CONV/scale);
	body->SetUserData(bodyData);

	return body;
}

// Add new circle to world
b2Body* CPhysics::newCircle(int id, float x, float y, float radius, float density, float friction, float restitution, float linearDamping, float angularDamping, bool staticObj) {
	b2BodyDef bd;

	//Pinho
	if(!staticObj)
		bd.type = b2_dynamicBody;

	b2CircleShape cs;
	cs.m_radius = radius/CONV;
	bd.position.Set(x/CONV,y/CONV);
	//cs.m_p.Set(0,0);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &cs;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;

	b2Body* body = world->CreateBody(&bd);
	body->CreateFixture(&fixtureDef);

	BodyData* bodyData = new BodyData;
	bodyData->id = id;
	bodyData->image = NULL;
	bodyData->color = b2Color(0,0,0);
	//bodyData->size = b2Vec2(width*2*CONV, height*2*CONV);
	//bodyData->offset = b2Vec2(width*CONV/2, height*CONV/2);
	body->SetUserData(bodyData);

	return body;
}

void CPhysics::setImage(b2Body* body, CImage* Image) {
	body->SetUserData(Image);
}

CImage* CPhysics::getImage(b2Body* body) {
	return (CImage*) body->GetUserData();
}

void CPhysics::setColor(b2Body* body, const b2Color& cor) {
	BodyData* bd = (BodyData*) body->GetUserData();
	bd->color = cor;
}

b2Color& CPhysics::getColor(b2Body* body) {
	BodyData* bd = (BodyData*) body->GetUserData();
	return bd->color;
}

void CPhysics::step() {
	world->Step(timeStep, velocityIterations, positionIterations);
	world->ClearForces();
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
		BodyData* ptr = (BodyData*) b->GetUserData();
		if(ptr->image != NULL) {
			const b2Vec2& pos = b->GetPosition();
			float rot = b->GetAngle();
			rot = rot * 180/M_PI;

			// Pinho
			float dx = -ptr->offset.x;;
			float dy = -ptr->offset.y;
			//	if(ptr->offset.x != offsetX)
			{
				//cout << ptr->offset.x << " -> " << offsetX << endl;
				//dx = offsetX - ptr->offset.x;
				//dy = offsetY - ptr->offset.y;
				//cout << "DX = " << dx << endl;
			}
			ptr->image->setPosition(pos.x*CONV+dx, pos.y*CONV+dy);
			ptr->image->setRotation(rot);
		}
	}

}

// Process contact list for this time step, and return true if
// there was a contact between body id1 and id2
bool CPhysics::haveContact(int id1, int id2) {
	for (b2Contact* c = world->GetContactList(); c; c = c->GetNext()) {
		// process c
		b2Fixture* a = c->GetFixtureA();
		b2Fixture* b = c->GetFixtureB();
		b2Body* ba = a->GetBody();
		b2Body* bb = b->GetBody();
		BodyData* bda = (BodyData*) ba->GetUserData();
		BodyData* bdb = (BodyData*) bb->GetUserData();
		//   b2Vec2 posa = ba->GetPosition();
		//   b2Vec2 posb = bb->GetPosition();
		//   cout <<  "Contact: " << bda->id << " with " << bdb->id << " -> " << posa.x << "," << posa.y << " with "
		//   << posb.x << "," << posb.y << endl;
		if(bda->id==id1 && bdb->id==id2 || bda->id==id2 && bdb->id==id1)
			return true;
	}
	return false;
}

// Draw boxes and circles, with or without associated image
void CPhysics::debugDraw() {
	glDisable(GL_TEXTURE_2D);
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
		BodyData* ptr = (BodyData*) b->GetUserData();
		if(ptr->image == NULL || 1) {
			const b2Vec2& pos = b->GetPosition();
			float rot = b->GetAngle();
			rot = rot * 180/M_PI;
			//cout << "Box: " << pos.x << " " << pos.y << " - " << rot;
			glColor4f(ptr->color.r, ptr->color.g, ptr->color.b, 0.2);
			// Iterate through all fixtures
			b2Fixture* f = b->GetFixtureList();
			
			float tx = pos.x * CONV;
			float ty = pos.y * CONV;

			// Pinho
			//tx += offsetX;
			//ty += offsetY;
			
			glPushMatrix();
			glColor4f(1,1,1,0.4);
			glTranslatef(tx, ty, 0);
			glRotatef(rot, 0, 0, 1);
//#define NEWAPPROACH
#ifdef NEWAPPROACH
			glBegin(GL_QUADS);
			glVertex2f(-ptr->size.x/2,-ptr->size.y/2);
			glVertex2f(-ptr->size.x/2, ptr->size.y/2);
			glVertex2f( ptr->size.x/2, ptr->size.y/2);
			glVertex2f( ptr->size.x/2,-ptr->size.y/2);
			glEnd();
			glPopMatrix();
#else
			b2PolygonShape* pol;
			b2CircleShape* circ;
			while(f != NULL) {
				switch(f->GetType()) {
				case b2Shape::e_polygon:
					pol = (b2PolygonShape*) f->GetShape();
					glBegin(GL_POLYGON);
					for(int i=0; i<pol->GetVertexCount(); i++) {
						const b2Vec2& v = pol->GetVertex(i);
						//cout << v.x << "," << v.y << " ";
						glVertex2f(v.x*CONV,v.y*CONV);
					}
					//cout << endl;
					glEnd();
					break;
				case b2Shape::e_circle:
					circ = (b2CircleShape*) f->GetShape();
					float radius = circ->m_radius;
					//cout << "drawing circle " << radius << endl;
					glBegin(GL_TRIANGLE_FAN);
					glVertex2f(0,0);
					for(float a=0; a<=2*M_PI; a+=0.6) {
						float px = cos(a) * radius;
						float py = sin(a) * radius;
						//cout << px << " " << py << endl;
						glVertex2f(px*CONV,py*CONV);
					}
					//cout << endl;
					glEnd();
				}
				f = f->GetNext();
				glPopMatrix();
			}
#endif
		}
	}
	glColor3f(1,1,1);
}

void CPhysics::setGravity(b2Vec2& grav) {
	gravity = grav;
	world->SetGravity(grav);
}

b2Vec2 CPhysics::getGravity() {
	return gravity;
}

void CPhysics::setPosition(b2Body* body, const b2Vec2& pos) {
	float angle = body->GetAngle();
	body->SetTransform(pos, angle);
}

void CPhysics::setAngle(b2Body* body, float angle) {
	const b2Vec2& pos = body->GetPosition();
	body->SetTransform(pos, angle*M_PI/180.0);
}
/*
void CPhysics::setDrawOffset(float ox, float oy) {
	offsetX = ox;
	offsetY = oy;
}
*/