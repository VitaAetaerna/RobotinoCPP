#include <iostream>
#include <stdlib.h>
#include "rec/robotino/api2/all.h"
#include <iostream>


// DOCUMENTATION: https://doc.openrobotino.org/download/RobotinoAPI2/rec_robotino_api2/annotated.html



//Namespace for Robotino and standard lib
using namespace rec::robotino::api2;
using namespace std;
// define a bool if it should run for a killswtich
bool _run = true;
class MyCom : public Com
{
public:
	MyCom()
	    : Com( "Transportino" )
	{
	}
	//Exceptions
	void errorEvent( const char* errorString )
	{
		std::cerr << "Error: " << errorString << std::endl;
	}

	void connectedEvent()
	{
		std::cout << "Connected." << std::endl;
	}

	void connectionClosedEvent()
	{
		std::cout << "Connection closed." << std::endl;
	}

	void logEvent( const char* message, int level )
	{
		std::cout << message << std::endl;
	}
};

// Bumper definieren
class MyBumper : public Bumper
{
public:
	MyBumper()
		: bumped( false ) // bumped = bool
	{
	}
	//Checke den Wert des Bumper, wenn er Kontakt hat dann wird bumped auf true gesetzt (KILLSWITCH)
	void bumperEvent( bool hasContact )
	{
		bumped |= hasContact;
    if (true == bumped){
      std::cout << "Bumper has " << ( hasContact ? "contact" : "no contact") << std::endl;
    }
	}

	bool bumped;
};
// X und Y Koordinaten
double x_distance = 0;
double y_distance = 0;
// Rotation
int phi_absolute = 0;
// Keine Ahnung man <_<
unsigned int sequence = 0;



// Odometrie definieren um Werte zu setzen und auszulesen
class MyOdometry: public Odometry{
  void readingsEvent(double x,double y, double phi, float vx, float vy, float omega, unsigned int seq ){
	cout << "X: " << x << " Y: " << y << " Richtung: " << phi << " Sequenz: " << seq << endl;  
	// Werte setzen und dann ausgeben
	x_distance = x;
	y_distance = y;
	phi_absolute = phi;
	sequence = seq;
	}
};







// Klassen initialisieren
MyCom com;
MyBumper bumper;
OmniDrive omniDrive;
MyOdometry odo;

// Initialisieren
void init( const std::string& hostname )
{
	
	// Zu bereits gesetzter IP verbinden
	std::cout << "Connecting...";
	com.setAddress( hostname.c_str() );
	com.connectToServer( true );


	// Wenn Verbindung fehl schlägt dann also dann keine Ahnung  Tot und verderben der ganzen Weltgeschichte
	// aso und die API wird runtergefahren und das programm schließt sich
	if( false == com.isConnected() )
	{
		std::cout << std::endl << "Could not connect to " << com.address() << std::endl;
		rec::robotino::api2::shutdown();
		exit( 1 );
	}
	else
	{
		// YIPPPPPPPPPPIEEEEEEEEEEEEE (erfolgreich verbunden)
		std::cout << "success" << std::endl;
	}
}







void drive()
{
  	odo.set(0,0,0,-1);
	

	int d = 0;
	while( com.isConnected() && false == bumper.value() && _run )
	{
	rec::robotino::api2::msleep(100);

	//Verarbeitet Trauma ._. (Aktionen)
	com.processEvents();
	// er macht heia und wenn ich sage heia MEINE ICH DER PENNT, DANN MEINE ICH DAS ER GEFÜHLT TOT IS xD
    rec::robotino::api2::msleep(100); 

  }

}


int main( int argc, char **argv )
{
	// IP setzen
	std::string hostname = "192.168.100.174";
	if( argc > 1 )
	{
		hostname = argv[1];
	}



	//main program routine starts in here
	try
	{
		//connect to your robotino (or try at least)
		init( hostname );
		//drive (this contains a while loop)

		
		drive();
		//exit
		com.disconnectFromServer();

	//  Exceptions
	}
    catch( const rec::robotino::api2::RobotinoException& e )
	{
		std::cerr << "Com Error: " << e.what() << std::endl;
	}
	catch( const std::exception& e )
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	catch( ... )
	{
		std::cerr << "Unknow Error" << std::endl;
	}

	rec::robotino::api2::shutdown();

#ifdef WIN32
	std::cout << "Press any key to exit..." << std::endl;
	rec::robotino::api2::waitForKey();
#endif
}