OBJs = app/link_obj/ServiceMain.o app/link_obj/TestData.o app/link_obj/ServiceImpl.o app/link_obj/ServiceBase.o \
	app/link_obj/Logger.o app/link_obj/Database.o app/link_obj/File.o app/link_obj/String.o app/link_obj/DateTime.o

all : $(OBJs)
	g++ -g -o ServiceMain $(OBJs) -I/usr/include/mysql -I/usr/local/include/mysql++ -L/usr/local/lib -lmysqlpp

app/link_obj/ServiceMain.o: app/ServiceMain.cpp
	g++ -o $@ -c app/ServiceMain.cpp -I/usr/include/mysql -I/usr/local/include/mysql++ -I_include/

app/link_obj/TestData.o : _include/TestData.h app/TestData.cpp
	g++ -o $@ -c app/TestData.cpp -I_include/

app/link_obj/ServiceImpl.o : _include/ServiceImpl.h app/ServiceImpl.cpp
	g++ -o $@ -c app/ServiceImpl.cpp -I/usr/include/mysql -I/usr/local/include/mysql++ -I_include/

app/link_obj/ServiceBase.o : _include/ServiceBase.h app/ServiceBase.cpp
	g++ -o $@ -c app/ServiceBase.cpp -I/usr/include/mysql -I/usr/local/include/mysql++ -I_include/

app/link_obj/Logger.o : _include/Logger.h app/Logger.cpp
	g++ -o $@ -c app/Logger.cpp -I/usr/include/mysql -I/usr/local/include/mysql++ -I_include/

app/link_obj/Database.o : _include/Database.h utils/Database.cpp
	g++ -o $@ -c utils/Database.cpp -I/usr/include/mysql -I/usr/local/include/mysql++ -I_include/

app/link_obj/File.o : _include/File.h utils/File.cpp
	g++ -o $@ -c utils/File.cpp -I_include/

app/link_obj/String.o : _include/String.h utils/String.cpp
	g++ -o $@ -c utils/String.cpp -I_include/

app/link_obj/DateTime.o :_include/DateTime.h utils/DateTime.cpp
	g++ -o $@ -c utils/DateTime.cpp -I_include/

clean :
	rm app/link_obj/* | rm ServiceMain