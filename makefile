#file	: makefile
#proj 	: Configuration
#Author	: Kabir Sohel
#Org	: Bubble Motion
#Date	: 17 Sept 2012


libConfigurationMgr.so :  Configuration.o  ConfigurationEvent.o ConfigurationMgr.o ConfigurationException.o FileMonitor.o Utility.o
		g++ -shared -o libConfigurationMgr.so  Configuration.o  ConfigurationEvent.o ConfigurationMgr.o ConfigurationException.o FileMonitor.o Utility.o

Configuration.o:  Configuration.cpp Configuration.h CommonHeader.h ConfigurationEvent.h ConfigurationException.h FileMonitor.h Utility.h
		g++ -fPIC -c -Wno-deprecated Configuration.cpp

ConfigurationEvent.o: ConfigurationEvent.cpp ConfigurationEvent.h CommonHeader.h
		g++ -fPIC -c -Wno-deprecated ConfigurationEvent.cpp

ConfigurationMgr.o: ConfigurationMgr.cpp ConfigurationMgr.h CommonHeader.h
		g++ -fPIC -c -Wno-deprecated -lpthread  ConfigurationMgr.cpp
	
ConfigurationException.o: ConfigurationException.cpp ConfigurationException.h CommonHeader.h
		g++ -fPIC -c -Wno-deprecated ConfigurationException.cpp

FileMonitor.o: FileMonitor.cpp FileMonitor.h Utility.h CommonHeader.h
		g++ -fPIC -c -Wno-deprecated -lpthread  FileMonitor.cpp
Utility.o: Utility.cpp Utility.h CommonHeader.h
		g++ -fPIC -c -Wno-deprecated Utility.cpp

clean:  
		rm libConfigurationMgr.so Configuration.o ConfigurationEvent.o ConfigurationMgr.o ConfigurationException.o FileMonitor.o Utility.o

# END OF MAKE FILE
