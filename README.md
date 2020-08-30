#from poland with dev yolo
Instrukcja obslugi:

1.pobieramy konfiguracje dla sieci Yolo poleceniem ./getModels.sh

2.Czasami nazwy plików z konfuguracjami na koncu zawieraja znak '\r' przez błąd wget'a dlatego trzeba poprawić nazwe pliku (usunąć '\r')

3.w kodzie zrodlowym yolo.cpp są stałe którymi konfigurujemy program

string classesFile = "coco.names"; // nazwy klas (pobrane w punkcie 2)

String modelConfiguration = "yolov3.cfg"; // konfiguracje (pobrane w punkcie 2)

String modelWeights = "yolov3.weights";// wagi (pobrane w punkcie 2)

string pattern="Has({person, dog}), No({horse})"; // Tagi do sprawdzania

string DirWithImages = "."; // katalog do przeskanowania

4.program uruchamiamy ./yolo.out