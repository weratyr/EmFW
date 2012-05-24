#ifndef FILEPARSER_H_
#define FILEPARSER_H_

#include "CPosition.h"
#include "CPosition.h"
#include "CBounds.h"

class FileParser
{

public:
	FileParser();
	~FileParser();

	bool readAllTiles();
	void resetToFirstWayInTile(CPosition );
	void GetNextTile();

	bool GetPosition(CPosition& Position);
	bool GetNextWay();
	unsigned int GetElementsPerTile();

	bool Testloop()	;
//	CBounds GetBounds();



private:

	bool m_NewWay;
	unsigned short m_Tile;
	unsigned short m_origTile;
	unsigned int m_ElementCount;


	unsigned int ElementsPerTileCnt[256*256];

	char *m_filename;
	CBounds m_Bounds;
	bool m_boundsSet;	

	static float * TilePtr[256*256];
	static float * TilePosition[256*256];


};

#endif /*FILEPARSER_H_*/
