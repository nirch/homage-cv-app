#include	<malloc.h>
#include	<stdlib.h>
#include	"Uigp/igp.h"
#include	"Ubox/Box2d.h"
#include	"ImageType/ImageType.h"
#include	"ImageType/PaletteType.h"
#include	"ImageType/PaletteTypeHash.h"



//// *******************************************************************


void fill_hash_3D_ball(palette_type_hash *target, palette_type_hash *source, int minor);

/*********  palette_type_hash_alloc  **********
* @Description	:  allocate a hash table for a palette
* 
* @Param  int colorPartitioningSize :  number of shades of each base color (r,g and b)
* @Return  palette_type_hash*  :  the hash table
*****************************************/
palette_type_hash* palette_type_hash_alloc(int colorPartitioningSize)
{
	palette_type_hash* hash;
	hash = (palette_type_hash* )calloc(1, sizeof(palette_type_hash));
	hash->NumberOfRanges = colorPartitioningSize*colorPartitioningSize*colorPartitioningSize;
	hash->data = (palette_hash_entry**)calloc(hash->NumberOfRanges,sizeof(palette_hash_entry*));
	return hash;
}


palette_hash_entry* palette_hash_entry_alloc(int red, int green, int blue, int minor, int palette_index)
{
	palette_hash_entry* entry = (palette_hash_entry*)malloc(sizeof(palette_hash_entry));
	entry->blue = blue;
	entry->green = green;
	entry->red = red;
	entry->minor = minor;
	entry->nextEntry = NULL;
	entry->palette_index = palette_index;
	return entry;
}



/*********  palette_hash_entry_alloc  **********
* @Description	:  copy allocation, except of the next_entry pointer
* 
* @Param  palette_hash_entry* source :  source entry to copy from
* @Return  palette_hash_entry*  :  allocated entry
*****************************************/
palette_hash_entry* palette_hash_entry_alloc_copy(palette_hash_entry* source)
{
	palette_hash_entry* entry = (palette_hash_entry*)malloc(sizeof(palette_hash_entry));
	*entry = *source;
	entry->nextEntry = NULL;
	return entry;
}


void palette_hash_destroy(palette_type_hash *hash)
{
	int i;

	palette_hash_entry *entry;
	if ( hash == NULL ) 
		return	;
	// get rid of the  artificial entries (
	for ( i = 0 ; i < hash->NumberOfRanges ; i ++)
	{		
		if  (hash->data[i] && hash->data[i]->minor != i)
			hash->data[i] = NULL;
	}
	for ( i = 0 ; i < hash->NumberOfRanges ; i ++)
	{		
		while ( hash->data[i] != NULL)
		{
			entry  = hash->data[i];
			entry->nextEntry = NULL;
			hash->data[i] = hash->data[i]->nextEntry;
			free(entry);
		}
	}
	free (hash->data);
	free (hash);
}



palette_type_hash *palette_hash_create_from_palette(palette_type *pal)
{
palette_type_hash* hash;//, *hash2;
int index = 0, partitioning;
int nextFreeCell = 0;
palette_hash_entry *entry, *selectedEntry, *tmpEntry;
int count = 0;
color_type *color;

// priority queue for palette-entries - it's color distance from center of a cube
int  entriesPriorityQ[256/(1<<BITS_PER_BASE_COLOR)][256]; 

// palette-entries as hash-entries
palette_hash_entry **entriesTmpTable;
//palette_hash_entry * entriesPriorityQ[1<<BITS_PER_BASE_COLOR];//partitioning

int r,g,b,i,j;




	if ( pal == NULL )
		return NULL;
	partitioning = 1<<BITS_PER_BASE_COLOR;

	for (  i = 0 ; i < 256/partitioning ; i ++ )
		for (  j = 0 ; j < 256 ; j ++ )
			entriesPriorityQ[i][j] = 0;

	
	hash = palette_type_hash_alloc(partitioning);

	entriesTmpTable = (palette_hash_entry ** )malloc(pal->nColor * 
													 sizeof(palette_hash_entry *));
	

// run over all entries in the palette and put in the 3D cubic strait forward 
	for (index = 0 ; index < pal->nColor ; index ++)
	{
		int minor = 0;
		color = &pal->data[index];

		minor = RGB2MINOR(color->Red, color->Green, color->Blue);
		
		entry = palette_hash_entry_alloc(color->Red, color->Green, color->Blue,
										minor,index);
		entriesTmpTable	[index] = entry;

/*		
		//insert into the hash strait forward - into the right cell 
		if ( hash->data[minor] == NULL)
		{
			hash->data[minor] = entry;
		}
		else
		{
			entry->nextEntry = hash->data[minor] ;
			hash->data[minor] = entry;
		}		
*/
		
	}//for index - all palette entries



	
	// for each cell in the 3D cubic, search for all palette-entries with color within range
	// of one 3D cubic from the center (kind of a 3D ball)
	// ALSO - put the closest entry to the center (if didn't appear earlier).
	for ( index = 0 ; index < hash->NumberOfRanges; index ++)
	{
		// if this color range didn't have a related color in the palette
		//if ( hash2->data[index] == NULL )
		{
			int dRed,dGreen,dBlue;
			int delta, minDelta;
			int sub_index;//, major;//, minor;
			
			//major = MINOR2MAJOR(index);
			r = MINOR2R_MIDDLE(index);
			g = MINOR2G_MIDDLE(index);
			b = MINOR2B_MIDDLE(index);

			selectedEntry = NULL;			
			minDelta = 0xfff;
			for (sub_index= 0 ; sub_index < pal->nColor; sub_index++ )
			{
				tmpEntry = entriesTmpTable[sub_index];
					
				if( (dRed = tmpEntry->red - r ) < 0 )		
					dRed = -dRed;
				if( (dGreen = tmpEntry->green - g ) < 0 )	
					dGreen = -dGreen;
				if( (dBlue = tmpEntry->blue - b ) < 0 )		
					dBlue = -dBlue;

				delta = MAX(dRed, dGreen);
				delta = MAX(dBlue,delta);		
				
				entriesPriorityQ[delta/partitioning][index] = 1;
				

				if ( delta <= 256/partitioning)// size of one cube
				{
					entry = palette_hash_entry_alloc_copy(tmpEntry);
					entry->minor = index;
					entry->nextEntry = hash->data[index];
					hash->data[index] = entry;
				}

				if  (minDelta > delta )
				{
					minDelta = delta;
					selectedEntry = tmpEntry;
				}
			}
			// take all the palette entries with the same distance level
			if (minDelta > 256/partitioning) 
			{// only if that level is farther than 1 cube
				minDelta= minDelta/partitioning;
				for ( i = 0 ; i < 256 ; i ++ )
				{
					if ( entriesPriorityQ[minDelta][i] != 0 )
					{
						tmpEntry = palette_hash_entry_alloc_copy(entriesTmpTable[i]);
						tmpEntry->minor = index;
						tmpEntry->nextEntry = hash->data[index];
						hash->data[index] = tmpEntry;
					}
					if ( minDelta < 255 && entriesPriorityQ[minDelta + 1][i] != 0 )
					{
						tmpEntry = palette_hash_entry_alloc_copy(entriesTmpTable[i]);
						tmpEntry->minor = index;
						tmpEntry->nextEntry = hash->data[index];
						hash->data[index] = tmpEntry;
					}
					if ( minDelta > 0 && entriesPriorityQ[minDelta - 1][i] != 0 )
					{
						tmpEntry = palette_hash_entry_alloc_copy(entriesTmpTable[i]);
						tmpEntry->minor = index;
						tmpEntry->nextEntry = hash->data[index];
						hash->data[index] = tmpEntry;
					}
				}
			}
			
		}// if minor range is empty
	}// for - all minor ranges
	free (entriesTmpTable);
	return hash;
}



/*********  fill_hash_3D_ball  **********
* @Description	:  Fills in each cell colors from it's neighboughrs
* 
* @Param  palette_type_hash *target :  target hash table
* @Param  palette_type_hash*source :  source hash table 
* @Param  int minor :  minor to fill (not on the edge)
* @Return  void  :  
*****************************************
void fill_hash_3D_ball(palette_type_hash *target, palette_type_hash *source, int minor)
{
	int r,g,b , rMinor, gMinor, bMinor;
	palette_hash_entry *entry, *selectedEntry ;



	bMinor = BLUE_DOWN_1_FLOOR(minor);
	gMinor = bMinor = GREEN_DOWN_1_FLOOR(bMinor );
	gMinor = bMinor = rMinor = RED_DOWN_1_FLOOR(gMinor);
	
	for ( r = 0 ; r < 3 ; r++ )
	{
		for ( g = 0 ; g < 3 ; g++ )
		{
			for ( b = 0 ; b < 3 ; b++ )
			{	
				for ( entry = source->data[bMinor]; entry != NULL ;entry = entry->nextEntry)
				{
					selectedEntry = palette_hash_entry_alloc_copy(entry);					
					selectedEntry->minor = minor;
					selectedEntry->nextEntry = target->data[minor];
					target->data[minor] = selectedEntry;					
				}
				bMinor = BLUE_UP_1_FLOOR(bMinor);
			}
			bMinor = gMinor = GREEN_UP_1_FLOOR(gMinor);
		}
		gMinor = bMinor = rMinor = RED_UP_1_FLOOR(rMinor);
	}
}
*/