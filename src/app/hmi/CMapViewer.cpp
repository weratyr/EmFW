#include "CMapViewer.h"

GLfloat wayPoints[] = {0.0, 0.0, -30.0, 0.0, 0.0, -30.0,};

CMapViewer::CMapViewer():
	m_width(WINDOW_WIDTH),
	m_height(WINDOW_HEIGHT),
	m_lRenderCount(0),
	m_CurrentPosition/*(48.99751,8.48220),*/				(49.87,8.63),
	m_iZoomLevel(4),
	m_iResolution(pow(2,10+m_iZoomLevel)),
	 mGpsDCAccessor(static_cast<CGpsDataContainer&>(CContext::getGpsContext().getContainer()))
{
	m_Parser.readAllTiles();
	init();
	}

CMapViewer::~CMapViewer()
{

}


void CMapViewer::init()
{
    initOpenGLParameter();

	m_AspectRatio=1.0/m_CurrentPosition.getAspectRatio();		// rechnet Position in Karte um
}


void CMapViewer::Draw()
{
	m_Parser.resetToFirstWayInTile(m_CurrentPosition);

	CPosition myPosition;

	CBounds boundsVisible;
	GetVisibleArea( boundsVisible );

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

#ifdef QNX
	glScalef(m_iResolution, m_AspectRatio*m_iResolution, 1.0 );
#else //linux glut
    glScaled( (double)m_iResolution,m_AspectRatio* (double)m_iResolution, 1.0 );
#endif

    glTranslatef( -0.5f*(boundsVisible.getEastBound()+boundsVisible.getWestBound()),
   				 -0.5f*(boundsVisible.getNorthBound()+boundsVisible.getSouthBound()), 0.0f );


	float fOffset=m_LineWidthRange[0]+(float)(m_iZoomLevel)*0.05*(m_LineWidthRange[1]-m_LineWidthRange[0]);
	float fStep=0.05*(m_LineWidthRange[1]-m_LineWidthRange[0]);
	fOffset=floor(fOffset/m_LineIncr)*m_LineIncr;
	fStep=ceil(fStep/m_LineIncr)*m_LineIncr;

	glClearColor(0.3, 0.3, 0.3, 1.0);							// Hintergrundfarbe
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// alles loeschen

	glColor4f(1.0,1.0,1.0,1.0);							// vordergrundfarbe weiss
	glLineWidth(fOffset+3.0*fStep );					// Definiert die Liniendicke

#ifdef LINUX
    //#######################################################das hier geht bei OpenGL 2.0
	do
	{
		glBegin(GL_LINE_STRIP);
		while ( m_Parser.GetPosition(myPosition) )
		{
			glVertex3f(myPosition.getLongitude(),myPosition.getLatitude(),-30.0);
		}
		glEnd();
	} while (m_Parser.GetNextWay());

#elif QNX
	glEnableClientState(GL_VERTEX_ARRAY);

	//default is z= -30.0
	wayPoints[2]=-30.0;
	wayPoints[5]=-30.0;

	do
	{
		m_Parser.GetPosition(myPosition);
		wayPoints[0]=myPosition.getLongitude();
		wayPoints[1]=myPosition.getLatitude();

		while ( m_Parser.GetPosition(myPosition) )
		{
			wayPoints[3]=myPosition.getLongitude();
			wayPoints[4]=myPosition.getLatitude();

			glVertexPointer(3, GL_FLOAT, 0, wayPoints);
			glDrawArrays(GL_LINE_STRIP, 0, 2);

			//Points tauschen, letzter Punkt ist nun erster des naechsten Teilwegs
			wayPoints[0]=wayPoints[3];
			wayPoints[1]=wayPoints[4];
		}
	} while (m_Parser.GetNextWay());
	glDisableClientState(GL_VERTEX_ARRAY);

#else
	DEBUG_PRINT("Unable to draw ways");
#endif

	/*
	 * TODO:
	 * Werte aus dem GPS Datencontainer auslesen und
	 * als current position speichern und
	 * als Punkt auf der Karte anzeigen.
	 * Die Werte aus dem Datencontainer können ohne Umrechnung gezeichnet werden.
	 */

}



void CMapViewer::DrawPoint(float x,float y,float z)
{
#ifdef LINUX
  glPointSize(10.0);
  glBegin(GL_POINTS);
    glColor4f(1,0,0, 0.5);//red
    glVertex3f(x,y,z);
  glEnd();

#elif QNX
  glEnableClientState(GL_VERTEX_ARRAY);

  wayPoints[0]=x;
  wayPoints[1]=y;
  wayPoints[2]=z;

  glVertexPointer(3, GL_FLOAT, 0, wayPoints);
  glDrawArrays(GL_LINE_STRIP, 0, 1);

  glDisableClientState(GL_VERTEX_ARRAY);

#else
  DEBUG_PRINT("unable to draw point");
#endif
}

