/*
 *  CPhysics.h
 *  Physics manager using Box2D
 *
 *  Created by Marcelo Cohen on 05/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#ifndef CPHYSICS_H
#define CPHYSICS_H

#include <Box2D/Box2D.h>
#include "CImage.h"

struct BodyData
{
    int id;         // id for the body (used in contacts)
    CImage* image;  // image associated to the body (can be NULL)
    b2Vec2 size;    // size of the collision object (not used)
    b2Vec2 offset;  // offset for drawing
    b2Color color;  // color for drawing collision object (when image == NULL)
};

class CPhysics
{
public:
	
	    b2World* world;
        b2Body* newBoxImage(int id, CImage* image, float density, float friction, float restitution, float linearDamping, float angularDamping, bool staticObj=false);
        b2Body* newBox(int id, float x, float y, float width, float height, float rotation, float density, float friction, float restitution, float linearDamping, float angularDamping, bool staticObj=false);
        b2Body* newCircleImage(int id, CImage* image, float density, float friction, float restitution, float linearDamping, float angularDamping, bool staticObj=false);
        b2Body* newCircle(int id, float x, float y, float radius, float density, float friction, float restitution, float linearDamping, float angularDamping, bool staticObj=false);

        void setImage(b2Body* body, CImage* sprite);
        CImage* getImage(b2Body* body);

        void setColor(b2Body* body, const b2Color& cor);
        b2Color& getColor(b2Body* body);

        void setPosition(b2Body* body, const b2Vec2& pos);
        void setAngle(b2Body* body, float angle);

        void setGravity(b2Vec2& grav);
        b2Vec2 getGravity();
    
        void setId(int id);
        int getId();

        void step();
        void debugDraw();
        bool haveContact(int id1, int id2);

       // void setDrawOffset(float ox, float oy);

        static void setConvFactor(float conv);

        // Implement Singleton Pattern
        static CPhysics* instance()
        {
            return &m_CPhysics;
        }
    protected:
        CPhysics();
        ~CPhysics();
    private:
        static CPhysics m_CPhysics;
        static const int velocityIterations = 10;
        static const int positionIterations = 8;
        static const float timeStep;

        static float CONV; // fator de conversão Box2D -> OpenGL

        b2Vec2 gravity;
       // float offsetX, offsetY; // offset to apply to translation when drawing

       
};

#endif // CPHYSICS_H
