Lauren Frazier
HW 5: Raytracer

My raytracer takes two arguments, the scene file and the raytracer config file. The project is currently set up to read from HW5_scene.txt and raytracer_config_sample.txt, which were used to render the included image (supersweet_raytraced_image.bmp). 

The raytracer includes reflection (depth = 5) and shadows. Bounding spheres were also implemented for extra credit. There is a slight problem displaying meshes; they show up entirely shadowed. This is a bug that arose within several minutes of submission, and there was not enough time to backtrack and restore the code. If shadows are turned off, however, you can see that all the shapes are drawing properly. 

To turn off shadows: edit Raytracer::inShadow() to return false all the time.
To turn off reflection: edit the last line of Raytracer::Raytracer() to say this->depth = 1.

In release mode, this code should run in about 3-10 seconds.

This assignment uses up my second and final extension.