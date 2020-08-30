#from poland with dev yolo
Instrukcja obslugi:


1.w kodzie zrodlowym yolo.cpp są stałe którymi konfigurujemy program

string classesFile = "coco.names"; // nazwy klas (pobrane w punkcie 2)

String modelConfiguration = "yolov3.cfg"; // konfiguracje (pobrane w punkcie 2)

String modelWeights = "yolov3.weights";// wagi (pobrane w punkcie 2)

string pattern="Has({person, dog}), No({horse})"; // Tagi do sprawdzania

string DirWithImages = "."; // katalog do przeskanowania

2.program instalujemy poleceniem 'make' w ramach którego są pobierane modele oraz program jest kompilowany

3.program uruchamiany przez ./yolo.out