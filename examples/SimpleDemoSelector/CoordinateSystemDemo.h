#ifndef COORDINATE_SYSTEM_DEMO_H
#define COORDINATE_SYSTEM_DEMO_H

#include "Bullet3AppSupport/BulletDemoInterface.h"
#include "OpenGLWindow/CommonGraphicsApp.h"
#include "Bullet3Common/b3Quaternion.h"
#include "OpenGLWindow/CommonRenderInterface.h"

///quick demo showing the right-handed coordinate system and positive rotations around each axis
class CoordinateSystemDemo : public BulletDemoInterface
{
    CommonGraphicsApp* m_app;
    float m_x;
    float m_y;
    float m_z;
    
public:
    
    CoordinateSystemDemo(CommonGraphicsApp* app)
    :m_app(app),
    m_x(0),
    m_y(0),
	m_z(0)
    {
		m_app->setUpAxis(2);

		 {
            int boxId = m_app->registerCubeShape(0.1,0.1,0.1);
            b3Vector3 pos=b3MakeVector3(0,0,0);
            b3Quaternion orn(0,0,0,1);
            b3Vector4 color=b3MakeVector4(0.3,0.3,0.3,1);
            b3Vector3 scaling=b3MakeVector3(1,1,1);
            m_app->m_renderer->registerGraphicsInstance(boxId,pos,orn,color,scaling);
        }

		 m_app->m_renderer->writeTransforms();
    }
    virtual ~CoordinateSystemDemo()
    {
        m_app->m_renderer->enableBlend(false);
    }
    static BulletDemoInterface*    CreateFunc(CommonGraphicsApp* app,int demoUserId)
    {
        return new CoordinateSystemDemo(app);
    }
    
    virtual void    initPhysics()
    {
    }
    virtual void    exitPhysics()
    {
        
    }
    virtual void	stepSimulation(float deltaTime)
    {
        m_x+=0.01f;
        m_y+=0.01f;
		m_z+=0.01f;

    }
    virtual void	renderScene()
    {
		m_app->m_renderer->renderScene();
    }

	virtual void drawArc(const b3Vector3& center, const b3Vector3& normal, const b3Vector3& axis, b3Scalar radiusA, b3Scalar radiusB, b3Scalar minAngle, b3Scalar maxAngle, 
				const b3Vector3& color, bool drawSect, b3Scalar stepDegrees = b3Scalar(10.f))
	{
		b3Scalar lineWidth = 3;
		const b3Vector3& vx = axis;
		b3Vector3 vy = normal.cross(axis);
		b3Scalar step = stepDegrees * B3_RADS_PER_DEG;
		int nSteps = (int)b3Fabs((maxAngle - minAngle) / step);
		if(!nSteps) nSteps = 1;
		b3Vector3 prev = center + radiusA * vx * b3Cos(minAngle) + radiusB * vy * b3Sin(minAngle);
		if(drawSect)
		{
			m_app->m_renderer->drawLine(center, prev, color,lineWidth);
		}
		for(int i = 1; i <= nSteps; i++)
		{
			b3Scalar angle = minAngle + (maxAngle - minAngle) * b3Scalar(i) / b3Scalar(nSteps);
			b3Vector3 next = center + radiusA * vx * b3Cos(angle) + radiusB * vy * b3Sin(angle);
			m_app->m_renderer->drawLine(prev, next, color,lineWidth);
			prev = next;
		}
		if(drawSect)
		{
			m_app->m_renderer->drawLine(center, prev, color,lineWidth);
		}
	}

    virtual void	physicsDebugDraw()
    {
      
		b3Vector3 xUnit=b3MakeVector3(1,0,0);
		b3Vector3 yUnit=b3MakeVector3(0,1,0);
		b3Vector3 zUnit=b3MakeVector3(0,0,1);

		b3Scalar lineWidth=3;

		b3Quaternion rotAroundX(xUnit,m_x);
		b3Quaternion rotAroundY(yUnit,m_y);
		b3Quaternion rotAroundZ(zUnit,m_z);

		b3Scalar radius=0.5;
		b3Vector3 toX=radius*b3QuatRotate(rotAroundX,yUnit);
		b3Vector3 toY=radius*b3QuatRotate(rotAroundY,xUnit);
		b3Vector3 toZ=radius*b3QuatRotate(rotAroundZ,xUnit);
		
		m_app->m_renderer->drawLine(xUnit+toX+b3QuatRotate(rotAroundX,b3MakeVector3(0,0.1,-0.2)),xUnit+toX,xUnit,lineWidth);
		m_app->m_renderer->drawLine(xUnit+toX+b3QuatRotate(rotAroundX,b3MakeVector3(0,-0.2,-0.2)),xUnit+toX,xUnit,lineWidth);
		//draw the letter 'x' on the x-axis
		//m_app->m_renderer->drawLine(xUnit-0.1*zUnit+0.1*yUnit,xUnit+0.1*zUnit-0.1*yUnit,xUnit,lineWidth);
		//m_app->m_renderer->drawLine(xUnit+0.1*zUnit+0.1*yUnit,xUnit-0.1*zUnit-0.1*yUnit,xUnit,lineWidth);

		m_app->m_renderer->drawLine(xUnit+toX+b3QuatRotate(rotAroundX,b3MakeVector3(0,-0.2,-0.2)),xUnit+toX,xUnit,lineWidth);

		m_app->m_renderer->drawLine(yUnit+toY+b3QuatRotate(rotAroundY,b3MakeVector3(-0.2,0,0.2)),yUnit+toY,yUnit,lineWidth);
		m_app->m_renderer->drawLine(yUnit+toY+b3QuatRotate(rotAroundY,b3MakeVector3(0.1,0,0.2)),yUnit+toY,yUnit,lineWidth);
		m_app->m_renderer->drawLine(zUnit+toZ+b3QuatRotate(rotAroundZ,b3MakeVector3(0.1,-0.2,0)),zUnit+toZ,zUnit,lineWidth);
		m_app->m_renderer->drawLine(zUnit+toZ+b3QuatRotate(rotAroundZ,b3MakeVector3(-0.2,-0.2,0)),zUnit+toZ,zUnit,lineWidth);

        
		drawArc(xUnit,xUnit,toX.normalized(),radius,radius,0.4,B3_2_PI,xUnit,false);
		drawArc(yUnit,yUnit,toY.normalized(),radius,radius,0.4,B3_2_PI,yUnit,false);
		drawArc(zUnit,zUnit,toZ.normalized(),radius,radius,0.4,B3_2_PI,zUnit,false);
    }
    virtual bool	mouseMoveCallback(float x,float y)
    {
		return false;   
    }
    virtual bool	mouseButtonCallback(int button, int state, float x, float y)
    {
        return false;   
    }
    virtual bool	keyboardCallback(int key, int state)
    {
        return false;   
    }
    
};
#endif //COORDINATE_SYSTEM_DEMO_H

