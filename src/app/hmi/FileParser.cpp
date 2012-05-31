#include "FileParser.h"

#include "Global.h"

//wg static
float * FileParser::TilePtr[256*256]={0};
float * FileParser::TilePosition[256*256]={0};


FileParser::FileParser()											//:	m_filename("/home/icmuser/Ways/WAYARRAY"),m_boundsSet(false)
{
	m_boundsSet = false;
	m_Tile=0;
}


FileParser::~FileParser(){}

void FileParser::resetToFirstWayInTile(CPosition Position)
{
	// aus Position Tile bestimmen und ersten Way im NodeArray
	m_Tile = Position.m_Kachel;
	m_origTile = m_Tile;
	m_ElementCount  =0;

	TilePosition[m_Tile] = TilePtr[m_Tile];
}


unsigned int FileParser::GetElementsPerTile()
{
	unsigned int ret = ElementsPerTileCnt[m_Tile];
	return ret;
}


bool FileParser::readAllTiles()
{

	unsigned int cnt;
	CPosition myPosition;

	// calculate Name
	char filename[256];
	FILE *file;

	snprintf(filename,255, "%s/ElementsPerTile",SORTEDTILEPATH);  	//set filename
	file = fopen(filename, "r");									// ElementsPerTiles lesen
	if(file==NULL)									// hier werden neue Tiles mit den dazugehoerigen Ways abgespeichert
	{
		printf(" something wrong with Fileopen ElementsPerTile \n");
		return false;
	}
	cnt=fread(ElementsPerTileCnt, 1, sizeof(ElementsPerTileCnt), file); 			// schreibe Tile hinein
	if(cnt == sizeof(ElementsPerTileCnt))
	{
		printf("ElementsPerTile successfully read! \n");
	}
	else
	{
		printf("ElementsPerTile problem reading \n");
	}
	fclose(file);


	for (int tile=0; tile<256*256; tile++)				// loop over 65k Tiles
	{
		if (0==ElementsPerTileCnt[tile])				// falls TIle leer, weiter
			continue;

		snprintf(filename,255, "%s/Tile_%d",SORTEDTILEPATH,tile);  	//set filename
		//open file for writing coordinate

		file = fopen(filename, "r");					// Tiles Dateien fuellen
		if(file==NULL)									// hier werden neue Tiles mit den dazugehoerigen Ways abgespeichert
		{
			printf(" something wrong with Fileopen \n");
			return false;
		}
		else											// hier werden neue Tiles mit den dazugehoerigen Ways abgespeichert
		{
			unsigned int debug = ElementsPerTileCnt[tile];
			TilePtr[tile] = new float[debug];
			TilePosition[tile] = TilePtr[tile];

			cnt=fread(TilePtr[tile], sizeof(float), debug,file); 			// schreibe Tile hinein
			if(cnt == ElementsPerTileCnt[tile])
			{
				//printf("Tile %d successfully read! %d  \n",tile, cnt);
			}
			else
			{
				printf("Tile %d problem reading \n",tile);
			}
			fclose (file);
		}
	}
	printf(" Tile read finished \n");
	return true;
}

bool FileParser::GetPosition(CPosition& Position)		// load next Position from memory for current way, Rueckgabe per Ref
															//parsed Datei nach Tiles
{
	CPosition myPosition;
	float Lat, Lon;

	if (m_ElementCount >= ElementsPerTileCnt[m_Tile])		// Ende der Tile erreicht?
		return false;


	Lat = * TilePosition[m_Tile];
	TilePosition[m_Tile]++;
	m_ElementCount++;

	Lon = * TilePosition[m_Tile];
	TilePosition[m_Tile]++;
	m_ElementCount++;

	if (Lat<0.0)
		printf(" debugg here \n");

	if ((Lat==0.0f) && (Lon== 0.0f))						// Way Ende wenn beide Koordinaten = 0
		return false;
	Position.setLatitude(Lat);
	Position.setLongitude(Lon);

	if (Position.getLongitude()<0.0)
		printf(" debug here \n");

	return true;
}

bool FileParser::GetNextWay()								// Dummy Fkt
{
	if (m_ElementCount >=  ElementsPerTileCnt[m_Tile])		// Ende der Tile erreicht?
		return false;

	return true;											// Weiterschalten reicht schon
}


		bool FileParser::Testloop()						// Dummy Fkt
		{
		   CPosition myPosition;

		// testloop statt mapviewer
			do
			{
				while ( GetPosition(myPosition) )
				{
					printf (" %f %f    %d Koordinate: Lat, Lon, way, Kachel \n", myPosition.getLatitude(), myPosition.getLongitude(), myPosition.getKachel());
				}

			} while (GetNextWay());
			return true;
		}



