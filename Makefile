
default_target: getmodels all
getmodels:
	./getModels2.sh
	./getModels3.sh
	./getModels1.sh
	
all: 
	g++ -ggdb `pkg-config --cflags --libs opencv4` yolo.cpp -lopencv_dnn -lopencv_imgproc -lopencv_core -lopencv_videoio -lopencv_imgcodecs -lopencv_highgui -o yolo.out

