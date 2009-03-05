#ifndef __WORLDLOADER_H
#define __WORLDLOADER_H

#include "yaml.h"

#include "IWorldLoader.hpp"
#include "../io/IFileManager.hpp"

class WorldLoader : public IWorldLoader
{

public:

	WorldLoader( IWorld* world, IFileManager* fileManager )
		: _world( world )
		, _fileManager( fileManager )
	{

	}

	void Load( const std::string& levelPath );

private:

	void LoadEntity( const YAML::Node& node );
	void LoadEntityComponents( const YAML::Node& node, IEntity* entity );

	IWorld* _world;
	IFileManager* _fileManager;

};

#endif