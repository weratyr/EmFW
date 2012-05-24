#include "CPosition.h"


//--------------------------------------------------------------

CPosition::CPosition() :m_Kachel(0), m_latitude(0),m_longitude(0) {}

CPosition::CPosition(const double& lat, const double& lon)
{
	setLatitude(lat);
	setLongitude(lon);
}

CPosition::~CPosition() {
}


CPosition CPosition::Projection(double degree,double distance)
{
	CPosition pos;

	double lat2=0.0,lon2=0.0,azi2=0.0,m12=0.0;

#ifdef LINUX
	GeographicLib::Geodesic::WGS84.Direct(d_latitude,d_longitude,degree,distance,lat2,lon2,azi2,m12,false);

	pos.setLatitude(lat2);
	pos.setLongitude(lon2);
#else
	DEBUG_PRINT("no GeographicLib available- position is 0:0");
#endif

	return pos;
}

double CPosition::Distance(CPosition& pos)		// ein Punkt in pos, der andere als Attribut. Return ellipt. Distanz zwischen den Punkten
{
	double s12=0.0,azi1=0.0,azi2=0.0,m12=0.0;

#ifdef LINUX
	GeographicLib::Geodesic::WGS84.Inverse(d_latitude,d_longitude,pos.getLatitude(),pos.getLongitude(),s12,azi1,azi2,m12);
#else
	DEBUG_PRINT("no GeographicLib available- distance is 0.0");
#endif

	return s12;
}

double CPosition::getAspectRatio()
{
	return cos(d_latitude*M_PI/180.0);
}

double CPosition::DistanceToEdgeSegment(CPosition& edgeSegmentPos1,CPosition& edgeSegmentPos2)//,double& lambda,double& distancePerpendicular)
{
	double dMin=0.0;

#ifdef LINUX
	double AB_s=0.0,AB_aziA=0.0,AB_aziB=0.0,AB_m=0.0;
	double AP_s=0.0,AP_aziA=0.0,AP_aziP=0.0,AP_m=0.0;

	GeographicLib::Geodesic::WGS84.Inverse(edgeSegmentPos1.getLatitude(),edgeSegmentPos1.getLongitude(),edgeSegmentPos2.getLatitude(),edgeSegmentPos2.getLongitude(),AB_s,AB_aziA,AB_aziB,AB_m);

	GeographicLib::Geodesic::WGS84.Inverse(edgeSegmentPos1.getLatitude(),edgeSegmentPos1.getLongitude(),d_latitude,d_longitude, AP_s,AP_aziA,AP_aziP,AP_m);

	double alpha=AP_aziA-AB_aziA;
	double APnew_azi=AB_aziA-alpha;

	if(APnew_azi<0.0)APnew_azi+=360.0;
	if(APnew_azi>360.0)APnew_azi-=360.0;

	CPosition Pnew=edgeSegmentPos1.Projection(APnew_azi,AP_s);

	double PPnew_s=0.0,PPnew_aziP=0.0,PPnew_aziPnew=0.0,PPnew_m=0.0;
	GeographicLib::Geodesic::WGS84.Inverse(d_latitude,d_longitude,Pnew.getLatitude(),Pnew.getLongitude(),PPnew_s,PPnew_aziP,PPnew_aziPnew,PPnew_m);

   	double PB_s=0.0,PB_aziP=0.0,PB_aziB=0.0,PB_m=0.0;
	GeographicLib::Geodesic::WGS84.Inverse(d_latitude,d_longitude,edgeSegmentPos2.getLatitude(),edgeSegmentPos2.getLongitude(),PB_s,PB_aziP,PB_aziB,PB_m);

	dMin=PPnew_s/2.0;

	double AS_s=0.0,AS_aziA=0.0,AS_aziS=0.0,AS_m=0.0;
	if((alpha>90.0 && alpha<270.0) || (alpha<-90.0 && alpha>-270.0))
	{
		dMin=AP_s;
	}
	else
	{
		if(AP_s>AB_s)
		{
			CPosition S=Projection(PPnew_aziP,dMin);
			GeographicLib::Geodesic::WGS84.Inverse(edgeSegmentPos1.getLatitude(),edgeSegmentPos1.getLongitude(),S.getLatitude(),S.getLongitude(),AS_s,AS_aziA,AS_aziS,AS_m);

			if(AS_s>AB_s)
			{
				dMin=PB_s;
			}
		}
	}
#else
	DEBUG_PRINT("no geographicLib available: distance to edge segment is set to 0.0");
#endif
	return dMin;
}

double CPosition::intDistance(CPosition& pos)
{
	double ax = pos.getLatitude();
	double ay = pos.getLongitude();
	double bx = m_latitude;
	double by = m_longitude;
	return sqrt( (ax-bx)*(ax-bx) + (ay-by)*(ay-by));
}

double CPosition::intDistanceToEdgeSegment(CPosition& edgeSegmentPos1,CPosition& edgeSegmentPos2)//,double& lambda,double& distancePerpendicular)
{
	double ax = edgeSegmentPos1.getLatitude();
	double ay = edgeSegmentPos1.getLongitude();
	double bx = edgeSegmentPos2.getLatitude();
	double by = edgeSegmentPos2.getLongitude();
	double cx = d_latitude;
	double cy = d_longitude;
	double dbax = bx-ax;
	double dbay = by-ay;

	double dx,dy;
	double Nennerx = dbay*dbay + dbax*dbax;

	dx = ((cy-ax)*dbax*dbay + dbax*dbax*cx + dbay*dbay*ax)/Nennerx;
	dy = dbay* (  (cy-ax)*dbay*dbax + dbax*dbax*cx + dbay*dbay*ax)/(Nennerx * dbax);
	return sqrt( (dx-cx)*(dx-cx) + (dy-ay)*(dy-ay));
}

