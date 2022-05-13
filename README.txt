This program is a shortest path algorithm:
    - It calculates all the paths and returns the shortest one.
    - The program uses the white pixels to guide
      the shortest path between two dots.
    - You type a number 1-3 and returns an example.
    - Close the first window with the clean map and the dots
    and a second window will open with the shortest path

I used pywebio for the web application and cv2 to plot

The program has 3 examples of the shortest route between
two dots on a map or a maze

EXAMPLE 1:
    -It's a practical application of the program
     using a print screen of google maps.
    -Calculates all the possible routes and draws
    the shortest one.

EXAMPLE 2:
    -It's a simple example of the algorithm's application

EXAMPLE 3:
    -It's a complicated maze
    -It might take a few minutes to run and plot the path!

All the code was developed in python 3.7 by Cyro Estevao

If the proper libraries are not previously installed the code might not work
	pip install opencv-python
	pip install matplotlib
	pip3 install -U pywebio