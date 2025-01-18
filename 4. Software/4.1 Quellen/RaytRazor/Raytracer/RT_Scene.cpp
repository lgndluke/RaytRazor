#include "RT_Scene.h"
#include "RT_Triangle.h"
#include "../Import/Importers/Object/Object_Importer.h"
#include "../Import/Resources/Object/Object_Resource.h"



void Scene::savebmp (const char *filename, int w, int h, int dpi, RGBType *data) {
	FILE *f;
	int k = w*h;
	int s = 4*k;
	int filesize = 54 + s;

	double factor = 39.375;
	int m = static_cast<int>(factor);

	int ppm = dpi*m;

	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

	bmpfileheader[ 2] = (unsigned char)(filesize);
	bmpfileheader[ 3] = (unsigned char)(filesize>>8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);

	bmpinfoheader[ 4] = (unsigned char)(w);
	bmpinfoheader[ 5] = (unsigned char)(w>>8);
	bmpinfoheader[ 6] = (unsigned char)(w>>16);
	bmpinfoheader[ 7] = (unsigned char)(w>>24);

	bmpinfoheader[ 8] = (unsigned char)(h);
	bmpinfoheader[ 9] = (unsigned char)(h>>8);
	bmpinfoheader[10] = (unsigned char)(h>>16);
	bmpinfoheader[11] = (unsigned char)(h>>24);

	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s>>8);
	bmpinfoheader[23] = (unsigned char)(s>>16);
	bmpinfoheader[24] = (unsigned char)(s>>24);

	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm>>8);
	bmpinfoheader[27] = (unsigned char)(ppm>>16);
	bmpinfoheader[28] = (unsigned char)(ppm>>24);

	bmpinfoheader[29] = (unsigned char)(ppm);
	bmpinfoheader[30] = (unsigned char)(ppm>>8);
	bmpinfoheader[31] = (unsigned char)(ppm>>16);
	bmpinfoheader[32] = (unsigned char)(ppm>>24);

	errno_t err = fopen_s(&f, filename,"wb");
	if (err != 0) {
		Logger::log(MessageType::SEVERE, "Error opening bmp file!");
	}

	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);

	for (int y = h - 1; y >= 0; y--) {
		for (int x = 0; x < w; x++) {
			int i = y*w + x;
			double red = (data[i].r)*255;
			double green = (data[i].g)*255;
			double blue = (data[i].b)*255;

			unsigned char color[3] = {
				static_cast<unsigned char>(std::floor(blue)),
				static_cast<unsigned char>(std::floor(green)),
				static_cast<unsigned char>(std::floor(red))
			};

			fwrite(color,1,3,f);
		}
	}

	fclose(f);
}

int winningObjectIndex(vector<double> object_intersections) {
	// return the index of the winning intersection
	int index_of_minimum_value;

	// prevent unnessary calculations
	if (object_intersections.size() == 0) {
		// if there are no intersections
		return -1;
	}
	else if (object_intersections.size() == 1) {
		if (object_intersections.at(0) > 0) {
			// if that intersection is greater than zero then its our index of minimum value
			return 0;
		}
		else {
			// otherwise the only intersection value is negative
			return -1;
		}
	}
	else {
		// otherwise there is more than one intersection
		// first find the maximum value

		double max = 0;
		for (int i = 0; i < object_intersections.size(); i++) {
			if (max < object_intersections.at(i)) {
				max = object_intersections.at(i);
			}
		}

		// then starting from the maximum value find the minimum positive value
		if (max > 0) {
			// we only want positive intersections
			for (int index = 0; index < object_intersections.size(); index++) {
				if (object_intersections.at(index) > 0 && object_intersections.at(index) <= max) {
					max = object_intersections.at(index);
					index_of_minimum_value = index;
				}
			}

			return index_of_minimum_value;
		}
		else {
			// all the intersections were negative
			return -1;
		}
	}
}

Color Scene::getColorAt(Vector intersection_position, Vector intersecting_ray_direction, vector<Object*> scene_objects, int index_of_winning_object, vector<RT_LightSource*> light_sources, double accuracy, double ambientlight) {

	Color winning_object_color = scene_objects.at(index_of_winning_object)->getColor();
	Vector winning_object_normal = scene_objects.at(index_of_winning_object)->getNormalAt(intersection_position);

	if (winning_object_color.getAlpha() == 2) {
		// checkered/tile floor pattern

		int square = (int)floor(intersection_position.getX()) + (int)floor(intersection_position.getZ());

		if ((square % 2) == 0) {
			// black tile
			winning_object_color.setRed(0);
			winning_object_color.setGreen(0);
			winning_object_color.setBlue(0);
		}
		else {
			// white tile
			winning_object_color.setRed(1);
			winning_object_color.setGreen(1);
			winning_object_color.setRed(1);
		}
	}

	Color final_color = winning_object_color.colorScalar(ambientlight);

	if (winning_object_color.getAlpha() > 0 && winning_object_color.getAlpha() <= 1) {
		// reflection from objects with specular intensity
		double dot1 = winning_object_normal.dot(intersecting_ray_direction.negative());
		Vector scalar1 = winning_object_normal.multiply(dot1);
		Vector add1 = scalar1.add(intersecting_ray_direction);
		Vector scalar2 = add1.multiply(2);
		Vector add2 = intersecting_ray_direction.negative().add(scalar2);
		Vector reflection_direction = add2.normalize();

		Ray reflection_ray (intersection_position, reflection_direction);

		// determine what the ray intersects with first
		vector<double> reflection_intersections;

		for (int reflection_index = 0; reflection_index < scene_objects.size(); reflection_index++) {
			reflection_intersections.push_back(scene_objects.at(reflection_index)->hit(reflection_ray));
		}

		int index_of_winning_object_with_reflection = winningObjectIndex(reflection_intersections);

		if (index_of_winning_object_with_reflection != -1) {
			// reflection ray missed everthing else
			if (reflection_intersections.at(index_of_winning_object_with_reflection) > accuracy) {
				// determine the position and direction at the point of intersection with the reflection ray
				// the ray only affects the color if it reflected off something

				Vector reflection_intersection_position = intersection_position.add(reflection_direction.multiply(reflection_intersections.at(index_of_winning_object_with_reflection)));
				Vector reflection_intersection_ray_direction = reflection_direction;

				Color reflection_intersection_color = getColorAt(reflection_intersection_position, reflection_intersection_ray_direction, scene_objects, index_of_winning_object_with_reflection, light_sources, accuracy, ambientlight);
				final_color = final_color.addColor(reflection_intersection_color.colorScalar(winning_object_color.getAlpha()));
			}
		}
	}

	for (int light_index = 0; light_index < light_sources.size(); light_index++) {
		Vector light_direction = light_sources.at(light_index)->getLightPosition().add(intersection_position.negative()).normalize();

		float cosine_angle = winning_object_normal.dot(light_direction);

		if (cosine_angle > 0) {
			// test for shadows
			bool shadowed = false;

			Vector distance_to_light = light_sources.at(light_index)->getLightPosition().add(intersection_position.negative()).normalize();
			float distance_to_light_magnitude = distance_to_light.magnitude();

			Ray shadow_ray (intersection_position, light_sources.at(light_index)->getLightPosition().add(intersection_position.negative()).normalize());

			vector<double> secondary_intersections;

			for (int object_index = 0; object_index < scene_objects.size() && shadowed == false; object_index++) {
				secondary_intersections.push_back(scene_objects.at(object_index)->hit(shadow_ray));
			}

			for (int c = 0; c < secondary_intersections.size(); c++) {
				if (secondary_intersections.at(c) > accuracy) {
					if (secondary_intersections.at(c) <= distance_to_light_magnitude) {
						shadowed = true;
					}
					break;
				}
			}

			if (shadowed == false) {
				final_color = final_color.addColor(winning_object_color.multiplyColor(light_sources.at(light_index)->getLightColor()).colorScalar(cosine_angle));

				if (winning_object_color.getAlpha() > 0 && winning_object_color.getAlpha() <= 1) {
					// special [0-1]
					double dot1 = winning_object_normal.dot(intersecting_ray_direction.negative());
					Vector scalar1 = winning_object_normal.multiply(dot1);
					Vector add1 = scalar1.add(intersecting_ray_direction);
					Vector scalar2 = add1.multiply(2);
					Vector add2 = intersecting_ray_direction.negative().add(scalar2);
					Vector reflection_direction = add2.normalize();

					double specular = reflection_direction.dot(light_direction);
					if (specular > 0) {
						specular = pow(specular, 10);
						final_color = final_color.addColor(light_sources.at(light_index)->getLightColor().colorScalar(specular*winning_object_color.getAlpha()));
					}
				}

			}

		}
	}

	return final_color.clip();
}

int thisone;
vector<RT_LightSource*> light_sources;
vector<Object*> objects_scene;
Camera camera_rt;

Scene::Scene() {
    //cubeScene();
    sphereScene();
}

bool Scene::render(Image &output) {
	clock_t t1, t2;
	t1 = clock();

	int dpi = 72;
	int width = 720;
	int height = 720;
	int n = width*height;
	RGBType *pixels = new RGBType[n];

	int aadepth = 1;
	double aathreshold = 0.1;
	double aspectratio = (double)width/(double)height;
	double ambientlight = 0.2;
	double accuracy = 0.00000001;

	Vector O (0,0,0);
	Vector X (1,0,0);
	Vector Y (0,1,0);
	Vector Z (0,0,1);


	//Vector campos (3, 500, 0); // Für Teapot
	Vector campos (3, 1.5, -4);

	//Vector look_at (0, 0, 100); // Für Teapot
	Vector look_at (0, 0, 0);
	Vector diff_btw (campos.getX() - look_at.getX(), campos.getY() - look_at.getY(), campos.getZ() - look_at.getZ());

	Vector camdir = diff_btw.negative().normalize();
	Vector camright = Y.cross(camdir).normalize();
	Vector camdown = camright.cross(camdir);
	camera_rt = Camera (campos, camdir, camright, camdown);


	int thisone, aa_index;
	double xamnt, yamnt;
	double tempRed, tempGreen, tempBlue;
	#pragma omp parallel for collapse(2) private(thisone, aa_index, xamnt, yamnt, tempRed, tempGreen, tempBlue)
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			thisone = y*width + x;

			// start with a blank pixel
			double tempRed[1];
			double tempGreen[1];
			double tempBlue[1];

			for (int aax = 0; aax < aadepth; aax++) {
				for (int aay = 0; aay < aadepth; aay++) {

					aa_index = aay*aadepth + aax;

					srand(time(0));

					// create the ray from the camera to this pixel
					if (aadepth == 1) {

						// start with no anti-aliasing
						if (width > height) {
							// the image is wider than it is tall
							xamnt = ((x+0.5)/width)*aspectratio - (((width-height)/(double)height)/2);
							yamnt = (y + 0.5) / height;
						}
						else if (height > width) {
							// the imager is taller than it is wide
							xamnt = (x + 0.5)/ width;
							yamnt = (((height - y) + 0.5)/height)/aspectratio - (((height - width)/(double)width)/2);
						}
						else {
							// the image is square
							xamnt = (x + 0.5)/width;
							yamnt = (y + 0.5)/height;
						}
					}
					else {
						// anti-aliasing
						if (width > height) {
							// the image is wider than it is tall
							xamnt = ((x + (double)aax/((double)aadepth - 1))/width)*aspectratio - (((width-height)/(double)height)/2);
							yamnt = ((y + 0.5) + (double)aax/((double)aadepth - 1))/height;
						}
						else if (height > width) {
							// the imager is taller than it is wide
							xamnt = (x + (double)aax/((double)aadepth - 1))/ width;
							yamnt = (((height - y) + (double)aax/((double)aadepth - 1))/height)/aspectratio - (((height - width)/(double)width)/2);
						}
						else {
							// the image is square
							xamnt = (x + (double)aax/((double)aadepth - 1))/width;
							yamnt = ((height - y) + (double)aax/((double)aadepth - 1))/height;
						}
					}

					Vector cam_ray_origin = camera_rt.getCamPos();
					Vector cam_ray_direction = camdir.add(camright.multiply(xamnt - 0.5).add(camdown.multiply(yamnt - 0.5))).normalize();

					Ray cam_ray (cam_ray_origin, cam_ray_direction);

					vector<double> intersections;

					for (int index = 0; index < objects_scene.size(); index++) {
						intersections.push_back(objects_scene.at(index)->hit(cam_ray));
					}

					int index_of_winning_object = winningObjectIndex(intersections);

					if (index_of_winning_object == -1) {
						// set the backgroung black
						tempRed[aa_index] = 0;
						tempGreen[aa_index] = 0;
						tempBlue[aa_index] = 0;
					}
					else{
						// index coresponds to an object in our scene
						if (intersections.at(index_of_winning_object) > accuracy) {
							// determine the position and direction vectors at the point of intersection

							Vector intersection_position = cam_ray_origin.add(cam_ray_direction.multiply(intersections.at(index_of_winning_object)));
							Vector intersecting_ray_direction = cam_ray_direction;

							Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction, objects_scene, index_of_winning_object, light_sources, accuracy, ambientlight);

							tempRed[aa_index] = intersection_color.getRed();
							tempGreen[aa_index] = intersection_color.getGreen();
							tempBlue[aa_index] = intersection_color.getBlue();
						}
					}
				}
			}

			// average the pixel color
			double totalRed = 0;
			double totalGreen = 0;
			double totalBlue = 0;

			for (int iRed = 0; iRed < aadepth*aadepth; iRed++) {
				totalRed = totalRed + tempRed[iRed];
			}
			for (int iGreen = 0; iGreen < aadepth*aadepth; iGreen++) {
				totalGreen = totalGreen + tempGreen[iGreen];
			}
			for (int iBlue = 0; iBlue < aadepth*aadepth; iBlue++) {
				totalBlue = totalBlue + tempBlue[iBlue];
			}

			double avgRed = totalRed/(aadepth*aadepth);
			double avgGreen = totalGreen/(aadepth*aadepth);
			double avgBlue = totalBlue/(aadepth*aadepth);

			pixels[thisone].r = avgRed;
			pixels[thisone].g = avgGreen;
			pixels[thisone].b = avgBlue;
			output.SetPixel(x, y, avgRed, avgGreen, avgBlue);
		}
	}

	savebmp("Raytracing_Scene.bmp",width,height,dpi,pixels);

	delete pixels, tempRed, tempGreen, tempBlue;


	t2 = clock();
	float diff = ((float)t2 - (float)t1)/1000;

	Logger::log(MessageType::INFO, "Done in: " + std::to_string(diff) + " seconds!");
	for (Object* obj : objects_scene) { delete obj; }
	objects_scene.clear();

	return true;
}

void Scene::cubeScene() {
	Logger::log(MessageType::INFO, "Rendering the .obj scene...!");
	Vector Y (0,1,0);
	Vector O (0,0,0);

	Color tile_floor (1, 1, 1, 2);
	Color white_light (1.0, 1.0, 1.0, 0);
	Color gray (0.5, 0.5, 0.5, 0.5);
	Color maroon (1.0, 0.0, 0.0, 0.5);

    //std::vector<Vertex> vertices = Object_Importer::fetch_vertices("C:/Uni/S5/SOPR/RaytRazor/5. Modelle/5.1 Beispielmodelle/miscellaneous/miscellaneous/teapot/Teapot.obj");
	std::vector<Vertex> vertices = Object_Importer::fetch_vertices("C:/Uni/S5/SOPR/RaytRazor/5. Modelle/5.1 Beispielmodelle/Test/pyramide.obj");

    for(int i = 0; i <= vertices.size()-3 ; i = i+3) {
    	Triangle* scene_triangle = new Triangle(Vector (vertices[i].position.x,vertices[i].position.y,vertices[i].position.z),
    												Vector(vertices[(i+1)].position.x,vertices[(i+1)].position.y,vertices[(i+1)].position.z),
    												Vector(vertices[(i+2)].position.x,vertices[(i+2)].position.y,vertices[(i+2)].position.z), maroon);
    	objects_scene.push_back(scene_triangle);

    }

    Logger::log(MessageType::DEBUG, "Objects: " + std::to_string(objects_scene.size()));


	Plane* scene_plane = new Plane(Y, -1, gray);
	objects_scene.push_back(scene_plane);

	//Vector light_position (3, -300, 0); // für teapot
	Vector light_position (-7,1,0);
	Light* scene_light = new Light(light_position, white_light);
	light_sources.push_back(scene_light);
}

void Scene::sphereScene() {
	Logger::log(MessageType::INFO, "Rendering the sphere scene...!");
	Vector new_sphere_location (1.75, -0.25, 0);
	Vector new_sphere_location2 (-1.75, 0.25, 0);

	Vector O (0,0,0);
	Vector X (1,0,0);
	Vector Y (0,1,0);
	Vector Z (0,0,1);


	Color white_light (1.0, 1.0, 1.0, 0);
	Color pretty_green (0.5, 1.0, 0.5, 0.3);
	Color maroon (0.5, 0.25, 0.25, 0.3);
	Color orange (1.0, 0.25, 0.25, 0.3);
	Color tile_floor (1, 1, 1, 2);
	Color gray (0.5, 0.5, 0.5, 0.3);
	Color black (0.0, 0.0, 0.0, 0);

	Vector light_position (-7,10,-10);
	Light* scene_light = new Light(light_position, white_light);
	light_sources.push_back(scene_light);

	// scene objects
	Sphere* scene_sphere = new Sphere(O, 1, pretty_green);
	Sphere* scene_sphere2 = new Sphere(new_sphere_location, 0.5, maroon);
	Sphere* scene_sphere3 = new Sphere(new_sphere_location2, 1, gray);
	Triangle* scene_triangle = new Triangle(Vector (5,0,0), Vector(0,3,0), Vector(0,0,3), orange);
	//Triangle scene_triangle2 (Vect (-2,0,0), Vect(0,0,-3), Vect(4,0,0), gray);
	Plane* scene_plane = new Plane(Y, -1, tile_floor);
	objects_scene.push_back(scene_sphere);
	objects_scene.push_back(scene_sphere2);
	objects_scene.push_back(scene_sphere3);
	objects_scene.push_back(scene_plane);
	objects_scene.push_back(scene_triangle);
	//scene_objects.push_back(dynamic_cast<Object*>(&scene_triangle2));
}


