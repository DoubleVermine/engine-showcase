Script:include( '/data/entities/scripts/entities/character.lua' );

----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Player = { }

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------

----------------------------------------------------------------
-- Player Functions
----------------------------------------------------------------

function Player.onEvent( eventName, var1, var2 )

	if ( eventName == 'INPUT_MOUSE_PRESSED' ) then
	
		player:fireWeapon( );
	
	end
	
	if ( var1 == Script:getName( ) ) then
	
		if ( eventName == 'ACTOR_HIT' ) then
			
			player:onHit( var2 );
		
		end
		
		if ( eventName == 'ACTOR_DEAD' ) then 
			
			print( 'player just died' );
				
		end
		
	end

end

extend( Player, Character );
player = Player:new( );

Script:registerEventHandler( Player.onEvent );