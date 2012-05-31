#ifndef CPOSITION_H_
#define CPOSITION_H_

#include <ostream>

#ifndef _GLOBAL_H
#include "Global.h"
#endif

#ifdef QNX
#include <math.h>
#endif

#ifdef LINUX
#include <GeographicLib/Geodesic.hpp>
#endif

//-------------------------------------------------------------

class CPosition
{
public:

	friend std::ostream &operator<< (std::ostream &ostr, const CPosition& pos);


	CPosition();
	CPosition(const double& lat, const double& lon);
	~CPosition();


	CPosition Projection(double degree,double distance);

	double getAspectRatio();

	double Distance(CPosition& pos);

	double DistanceToEdgeSegment(CPosition& edgeSegmentPos1,CPosition& edgeSegmentPos2);//,double& lambda,double& distancePerpendicular)

	double intDistanceToEdgeSegment(CPosition& edgeSegmentPos1,CPosition& edgeSegmentPos2);
	double intDistance(CPosition& pos);


	inline void setshortLatitude(unsigned short Lat)
	{
		m_latitude = (Lat & 0xfffe);
	}

	inline void setshortLongitude(unsigned short Lon)
	{
		m_longitude = (Lon & 0xfffe);
	}


	inline  unsigned short getshortLatitude() const
    {
		return m_latKachel;
    }


   inline  unsigned short getshortLongitude() const
   {
		return m_longitude;
   }


	inline double getLatitude() const
    {
		unsigned int temp = (((unsigned int) (m_latKachel) << 16) + (unsigned int) m_latitude);

		double temp_lat = (((double)(temp>>1))/190644.0 + 31.0);
        return temp_lat;
    }


    inline void setLatitude(double lat)  //hinterstes lat-Bit zum Markieren frei
    {
    	//7fff=32767	delta = 75-31	-> 44	32767*256/delta = 190644
		unsigned int temp = ( (unsigned int) ((lat-31.0)*190644.0)<<1);  // 2 Byte fuer Koord., hinterstes Bit 0
		m_latitude = (unsigned short) temp;
        m_latKachel = (unsigned char) (temp>>16);						// 1 Byte fuer Kachel
    	d_latitude = lat;
	}



	inline  double getLongitude() const
    {
		unsigned int temp = (((unsigned int) (m_lonKachel) << 16) + (unsigned int) m_longitude);

		double temp_lon = (((double)(temp>>1))/108939.0 - 32.0);
        return temp_lon;
    }

    inline void setLongitude(double lon)
    {		// delta = 45--32 =77-> 77		108939
		unsigned int temp = (( unsigned int) ((lon+32.0)*108939.0)<<1);
        m_longitude = (unsigned short) temp;
		m_lonKachel = (unsigned char) (temp>>16);
		d_longitude = lon;
    }

	inline void setKachel(unsigned short K)
	{
		m_Kachel = K;
	}


	inline unsigned short getKachel()
	{
		return m_Kachel;
	}

public:				// to do only for debugging protected:									// 4+2 Byte

	union
	{
		unsigned short m_Kachel;
		struct
		{
			unsigned char m_latKachel;
			unsigned char m_lonKachel;
		};
	};

	union
	{
		unsigned int m_Position;
		struct
		{
			unsigned short m_latitude;		// hinterste Bits lat_lon:   00 = normal; 10 = beg.node;  01=Kachelwechsel 11 beides
			unsigned short m_longitude;
		};

	};
	double	d_latitude;
	double 	d_longitude;

};

#endif /* CPOSITION_H_ */
