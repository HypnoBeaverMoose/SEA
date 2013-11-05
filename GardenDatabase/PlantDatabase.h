
#ifndef _PLANTDATABASE_H
#define _PLANTDATABASE_H

class PlantDatabase
{
	public:
		struct Plant
		{
			Plant( int ident )
				: id(ident) { }

			int id;
		};

		PlantDatabase();
		//~PlantDatabase();

		Plant getPlant( int id, bool result ) const;
		bool addPlant( const Plant &data );

	private:
};

#endif