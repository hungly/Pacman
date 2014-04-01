SOURCE_FOLDER = src/

all:
	cd $(SOURCE_FOLDER); make

clean:
	cd $(SOURCE_FOLDER); make clean

cleanall:
	cd $(SOURCE_FOLDER); make cleanall