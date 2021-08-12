OBJs = main.o TestData.o ServiceImpl.o ServiceBase.o Logger.o Database.o File.o String.o DateTime.o

all : $(OBJs)
	g++ -g -o main $(OBJs) -I/usr/include/mysql -I/usr/local/include/mysql++ -L/usr/local/lib -lmysqlpp

main.o: main.cpp
	g++ -c main.cpp -I/usr/include/mysql -I/usr/local/include/mysql++

TestData.o : TestData.h TestData.cpp
	g++ -c TestData.cpp

ServiceImpl.o : ServiceImpl.h ServiceImpl.cpp
	g++ -c ServiceImpl.cpp -I/usr/include/mysql -I/usr/local/include/mysql++

ServiceBase.o : ServiceBase.h ServiceBase.cpp
	g++ -c ServiceBase.cpp -I/usr/include/mysql -I/usr/local/include/mysql++

Logger.o : Logger.h Logger.cpp
	g++ -c Logger.cpp -I/usr/include/mysql -I/usr/local/include/mysql++

Database.o : Database.h Database.cpp
	g++ -c Database.cpp -I/usr/include/mysql -I/usr/local/include/mysql++

File.o : File.h File.cpp
	g++ -c File.cpp

String.o : String.h String.cpp
	g++ -c String.cpp

DateTime.o :DateTime.h DateTime.cpp
	g++ -c DateTime.cpp

clean :
	rm *.o main
