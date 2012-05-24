#ifndef CMAPVIEWER_H_
#define CMAPVIEWER_H_

#ifndef CPOSITION_H_
#include "CPosition.h"
#endif

#ifndef CBOUNDS_H_
#include "CBounds.h"
#endif

#ifndef FILEPARSER_H_
#include "FileParser.h"
#endif

#ifndef GRAPHICSINCLUDES_H_
#include "GraphicsIncludes.h"
#endif

#ifndef CGPSDATACONTAINERADAPTER_H_
	#include "CGpsDataContainerAdapter.h"
#endif

#include "CContext.h"

void display(void);
void reshape(int w, int h);

class CMapViewer
{

public:
	CMapViewer();
	virtual ~CMapViewer();
	
	//IRunnable Interface
	void init(void);
	void run(void){};
	void stop(void){}
	void cleanup(void){};
	void prepareForShutdown(void){}
	void reRun(){}
	void goToStandby(){}
	
	int getWidth()
	{
		return m_width;
	}
	
	int getHeight()
	{
		return m_height;
	}
	
	void setWidth(int width)
	{
		m_width=width;
		
	}
	
	void setHeight(int height)
	{
		m_height=height;
		
	}
	
	
	CPosition getPosition()
	{
		return m_CurrentPosition;
	}
	
	void setPosition(CPosition Position)
	{
		m_CurrentPosition.setLatitude(Position.getLatitude());

		m_CurrentPosition.setLongitude(Position.getLongitude() ) ;

	}

	
	void GetVisibleArea( CBounds &boundsVisible )
	{
	   boundsVisible.setWestBound(m_CurrentPosition.getLongitude()-(m_width/2/(double)m_iResolution));
	   boundsVisible.setEastBound(m_CurrentPosition.getLongitude()+(m_width/2/(double)m_iResolution));
	   boundsVisible.setSouthBound(m_CurrentPosition.getLatitude()-(m_height/2/(m_AspectRatio*(double)m_iResolution)));
	   boundsVisible.setNorthBound(m_CurrentPosition.getLatitude()+(m_height/2/(m_AspectRatio*(double)m_iResolution)));
	}
	
	void initOpenGLParameter()
	{
		m_PointWidthRange[0]=0.5;
		m_PointWidthRange[1]=5.0;
		m_PointIncr=0.5;
		m_LineWidthRange[0]=0.5;
		m_LineWidthRange[1]=10.0;
		m_LineIncr=0.5;
	}

	void Draw();

private:
	friend void display(void);
	friend void reshape(int w, int h);
	
	int m_width;
	int m_height;
	
	long m_lRenderCount;

	FileParser m_Parser;
	
	GLfloat m_PointWidthRange[2];
	GLfloat m_PointIncr;

	GLfloat m_LineWidthRange[2];
	GLfloat m_LineIncr;
	
	CPosition m_CurrentPosition;
	
	double m_AspectRatio;
	
	int m_iZoomLevel;
	int m_iResolution;
	bool m_ShowGrid;
	

	void ZoomIn( )
	{
	   if(m_iZoomLevel<10)m_iZoomLevel++;
		
	   m_iResolution = pow(2,m_iZoomLevel+10);
	}
	
	void ZoomOut( )
	{
	   if(m_iZoomLevel>-10)m_iZoomLevel--;
	   
	   m_iResolution = pow(2,m_iZoomLevel+10);
	}
	
	void ScrollUp( )
	{
		m_CurrentPosition.setLatitude(
			m_CurrentPosition.getLatitude()+(100.0/(double)m_iResolution));
	}
	
	void ScrollDown( )
	{
	   m_CurrentPosition.setLatitude(
	   		m_CurrentPosition.getLatitude()-(100.0/(double)m_iResolution));
	}
	
	void ScrollLeft( )
	{
	   m_CurrentPosition.setLongitude(
	   		m_CurrentPosition.getLongitude()-(100.0/(double)m_iResolution));
	}
	
	void ScrollRight( )
	{
	   m_CurrentPosition.setLongitude(
	   		m_CurrentPosition.getLongitude()+(100.0/(double)m_iResolution));
	}
	

	void DrawTile(int TileID);
	
	void DrawPoint(float x, float y,float z=0.0);
	void DrawCross(CPosition& pos,float z,float size);
	void DrawQuad( int x1, int y1, int x2, int y2 );
	
	CGpsDataContainerAdapter mGpsDCAccessor;

};

#endif /*CMAPVIEWER_H_*/
