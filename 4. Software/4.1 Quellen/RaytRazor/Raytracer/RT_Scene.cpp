#include "../UI/Scenes/Main/Main_Scene.h"
#include "RT_Scene.h"

#include "../UI/Widget/ComponentAttributes_Widget.h"

RT_Scene::RT_Scene() {
	auto components = Main_Scene::getComponents();
	if (components.empty()) {
		sphereScene();
		components.clear();
	}
	else {
		previewScene();
		components.clear();
	}
}

void RT_Scene::savebmp (const char *filename, const int w, const int h, const int dpi, const RGBType *data) {
	FILE *f;
	const int k = w*h;
	const int s = 4*k;
	const int filesize = 54 + s;

	const double factor = 39.375;
	const int m = static_cast<int>(factor);

	const int ppm = dpi*m;

	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

	bmpfileheader[ 2] = static_cast<unsigned char>(filesize);
	bmpfileheader[ 3] = static_cast<unsigned char>(filesize>>8);
	bmpfileheader[ 4] = static_cast<unsigned char>(filesize>>16);
	bmpfileheader[ 5] = static_cast<unsigned char>(filesize>>24);

	bmpinfoheader[ 4] = static_cast<unsigned char>(w);
	bmpinfoheader[ 5] = static_cast<unsigned char>(w>>8);
	bmpinfoheader[ 6] = static_cast<unsigned char>(w>>16);
	bmpinfoheader[ 7] = static_cast<unsigned char>(w>>24);

	bmpinfoheader[ 8] = static_cast<unsigned char>(h);
	bmpinfoheader[ 9] = static_cast<unsigned char>(h>>8);
	bmpinfoheader[10] = static_cast<unsigned char>(h>>16);
	bmpinfoheader[11] = static_cast<unsigned char>(h>>24);

	bmpinfoheader[21] = static_cast<unsigned char>(s);
	bmpinfoheader[22] = static_cast<unsigned char>(s>>8);
	bmpinfoheader[23] = static_cast<unsigned char>(s>>16);
	bmpinfoheader[24] = static_cast<unsigned char>(s>>24);

	bmpinfoheader[25] = static_cast<unsigned char>(ppm);
	bmpinfoheader[26] = static_cast<unsigned char>(ppm>>8);
	bmpinfoheader[27] = static_cast<unsigned char>(ppm>>16);
	bmpinfoheader[28] = static_cast<unsigned char>(ppm>>24);

	bmpinfoheader[29] = static_cast<unsigned char>(ppm);
	bmpinfoheader[30] = static_cast<unsigned char>(ppm>>8);
	bmpinfoheader[31] = static_cast<unsigned char>(ppm>>16);
	bmpinfoheader[32] = static_cast<unsigned char>(ppm>>24);

	if (const errno_t err = fopen_s(&f, filename,"wb"); err != 0) {
		Logger::log(MessageType::SEVERE, "Error opening bmp file!");
	}

	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);

	for (int y = h - 1; y >= 0; y--) {
		for (int x = 0; x < w; x++) {
			const int i = y*w + x;
			const double red = (data[i].r)*255;
			const double green = (data[i].g)*255;
			const double blue = (data[i].b)*255;

			const unsigned char color[3] = {
				static_cast<unsigned char>(std::floor(blue)),
				static_cast<unsigned char>(std::floor(green)),
				static_cast<unsigned char>(std::floor(red))
			};

			fwrite(color,1,3,f);
		}
	}

	fclose(f);
}

int winningObjectIndex(const vector<double> &object_intersections) {
	// prevent unneeded calculations
	if (object_intersections.empty()) { return -1; }
	if (object_intersections.size() == 1) {
		if (object_intersections.at(0) > 0) { return 0; }
		return -1;
	}

		double max = 0;
		for (const double object_intersection : object_intersections) {
			if (max < object_intersection) { max = object_intersection; }
		}

		if (max > 0) {
			int index_of_minimum_value = 0;
			// only positive intersections
			for (int index = 0; index < object_intersections.size(); index++) {
				if (object_intersections.at(index) > 0 && object_intersections.at(index) <= max) {
					max = object_intersections.at(index);
					index_of_minimum_value = index;
				}
			}
			return index_of_minimum_value;
		}
		return -1;

	}

RT_Color RT_Scene::getColorAt(Vector intersection_position, Vector intersecting_ray_direction, vector<RT_Object*> scene_objects, int index_of_winning_object, vector<RT_LightSource*> light_sources, double accuracy, double ambientlight) {

	RT_Color winning_object_color = scene_objects.at(index_of_winning_object)->getColor();
	Vector winning_object_normal = scene_objects.at(index_of_winning_object)->getNormalAt(intersection_position);

	RT_Color final_color = winning_object_color.colorScalar(ambientlight);

	if (winning_object_color.getAlpha() > 0 && winning_object_color.getAlpha() <= 1) {
		double dot1 = winning_object_normal.dot(intersecting_ray_direction.negative());
		Vector scalar1 = winning_object_normal.multiply(dot1);
		Vector add1 = scalar1.add(intersecting_ray_direction);
		Vector scalar2 = add1.multiply(2);
		Vector add2 = intersecting_ray_direction.negative().add(scalar2);
		Vector reflection_direction = add2.normalize();

		Ray reflection_ray (intersection_position, reflection_direction);

		// determine what the ray intersects with first
		vector<double> reflection_intersections;

		for (auto & scene_object : scene_objects) {
			reflection_intersections.push_back(scene_object->hit(reflection_ray));
		}

		if (int index_of_winning_object_with_reflection = winningObjectIndex(reflection_intersections); index_of_winning_object_with_reflection != -1) {
			// reflection ray missed everthing else
			if (reflection_intersections.at(index_of_winning_object_with_reflection) > accuracy) {
				// determine the position and direction at the point of intersection with the reflection ray
				// the ray only affects the color if it reflected off something

				Vector reflection_intersection_position = intersection_position.add(reflection_direction.multiply(reflection_intersections.at(index_of_winning_object_with_reflection)));
				Vector reflection_intersection_ray_direction = reflection_direction;

				RT_Color reflection_intersection_color = getColorAt(reflection_intersection_position, reflection_intersection_ray_direction, scene_objects, index_of_winning_object_with_reflection, light_sources, accuracy, ambientlight);
				final_color = final_color.addColor(reflection_intersection_color.colorScalar(winning_object_color.getAlpha()));
			}
		}
	}

	for (auto & light_source : light_sources) {
		Vector light_direction = light_source->getLightPosition().add(intersection_position.negative()).normalize();

		if (float cosine_angle = winning_object_normal.dot(light_direction); cosine_angle > 0) {
			// test for shadows
			bool shadowed = false;

			Vector distance_to_light = light_source->getLightPosition().add(intersection_position.negative()).normalize();
			float distance_to_light_magnitude = distance_to_light.magnitude();

			Ray shadow_ray (intersection_position, light_source->getLightPosition().add(intersection_position.negative()).normalize());

			vector<double> secondary_intersections;

			for (auto & scene_object : scene_objects) {
				secondary_intersections.push_back(scene_object->hit(shadow_ray));
			}

			for (double secondary_intersection : secondary_intersections) {
				if (secondary_intersection > accuracy) {
					if (secondary_intersection <= distance_to_light_magnitude) {
						shadowed = true;
						break;
					}
				}
			}

			if (shadowed == false) {
				final_color = final_color.addColor(winning_object_color.multiplyColor(light_source->getLightColor()).colorScalar(cosine_angle));

				if (winning_object_color.getAlpha() > 0 && winning_object_color.getAlpha() <= 1) {
					double dot1 = winning_object_normal.dot(intersecting_ray_direction.negative());
					Vector scalar1 = winning_object_normal.multiply(dot1);
					Vector add1 = scalar1.add(intersecting_ray_direction);
					Vector scalar2 = add1.multiply(2);
					Vector add2 = intersecting_ray_direction.negative().add(scalar2);
					Vector reflection_direction = add2.normalize();

					if (double specular = reflection_direction.dot(light_direction); specular > 0) {
						specular = pow(specular, 10);
						final_color = final_color.addColor(light_source->getLightColor().colorScalar(specular*winning_object_color.getAlpha()));
					}
				}

			}

		}
	}

	return final_color.clip();
}

int thisone;
vector<RT_LightSource*> light_sources;
vector<RT_Object*> objects_scene;
Camera camera_rt;
Vector cam_pos;
Vector look_at;


bool RT_Scene::render(Image &output) {
	clock_t t1, t2;
	t1 = clock();

	int dpi = 72;
	int width = output.getXSize();
	int height = output.getYSize();
	int n = width*height;
	auto *pixels = new RGBType[n];

	double aspectratio = static_cast<double>(width)/static_cast<double>(height);

	Vector Y (0,1,0);


	Vector diff_btw (cam_pos.getX() - look_at.getX(), cam_pos.getY() - look_at.getY(), cam_pos.getZ() - look_at.getZ());

	Vector camdir = diff_btw.negative().normalize();
	Vector camright = Y.cross(camdir).normalize();
	Vector camdown = camright.cross(camdir);
	camera_rt = Camera (cam_pos, camdir, camright, camdown);



	int thisone, aa_index;
	double xamnt, yamnt;
	double tempRed, tempGreen, tempBlue;
	#pragma omp parallel for collapse(2) private(thisone, aa_index, xamnt, yamnt, tempRed, tempGreen, tempBlue)
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			int aadepth = 1;
			thisone = y*width + x;

			double temp_red[1];
			double temp_green[1];
			double temp_blue[1];

			for (int aax = 0; aax < aadepth; aax++) {
				for (int aay = 0; aay < aadepth; aay++) {

					aa_index = aay*aadepth + aax;

					srand(time(nullptr));

					if (width > height) {
						xamnt = ((x+0.5)/width)*aspectratio - (((width-height)/(double)height)/2);
						yamnt = (y + 0.5) / height;
					}
					else if (height > width) {
						xamnt = (x + 0.5)/ width;
						yamnt = ((y + 0.5)/height)/aspectratio - (((height - width)/(double)width)/2);
					}
					else {
						xamnt = (x + 0.5)/width;
						yamnt = (y + 0.5)/height;
					}
					Vector cam_ray_origin = camera_rt.getCamPos();
					Vector cam_ray_direction = camdir.add(camright.multiply(xamnt - 0.5).add(camdown.multiply(yamnt - 0.5))).normalize();

					Ray cam_ray (cam_ray_origin, cam_ray_direction);

					vector<double> intersections;

					for (auto & index : objects_scene) {
						intersections.push_back(index->hit(cam_ray));
					}

					if (int index_of_winning_object = winningObjectIndex(intersections); index_of_winning_object == -1) {
						// set the backgroung black
						temp_red[aa_index] = 0;
						temp_green[aa_index] = 0;
						temp_blue[aa_index] = 0;
					}
					else{
						if (double accuracy = 1e-8; intersections.at(index_of_winning_object) > accuracy) {
							double ambientlight = 0.2;

							Vector intersection_position = cam_ray_origin.add(cam_ray_direction.multiply(intersections.at(index_of_winning_object)));
							Vector intersecting_ray_direction = cam_ray_direction;

							RT_Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction, objects_scene, index_of_winning_object, light_sources, accuracy, ambientlight);

							temp_red[aa_index] = intersection_color.getRed();
							temp_green[aa_index] = intersection_color.getGreen();
							temp_blue[aa_index] = intersection_color.getBlue();
						}
					}
				}
			}

			// average the pixel color
			double totalRed = 0;
			double totalGreen = 0;
			double totalBlue = 0;

			for (int iRed = 0; iRed < aadepth*aadepth; iRed++) {
				totalRed = totalRed + temp_red[iRed];
			}
			for (int iGreen = 0; iGreen < aadepth*aadepth; iGreen++) {
				totalGreen = totalGreen + temp_green[iGreen];
			}
			for (int iBlue = 0; iBlue < aadepth*aadepth; iBlue++) {
				totalBlue = totalBlue + temp_blue[iBlue];
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
	float diff = (static_cast<float>(t2) - static_cast<float>(t1))/1000;

	Logger::log(MessageType::INFO, "Done in: " + std::to_string(diff) + " seconds!");
	for (RT_Object* obj : objects_scene) { delete obj; }
	objects_scene.clear();

	for (RT_LightSource* lght : light_sources) { delete lght; }
	light_sources.clear();

	return true;
}

void RT_Scene::cubeScene() {
	Logger::log(MessageType::INFO, "Rendering the .obj scene...!");
	const Vector Y (0,1,0);
	const RT_Color white_light (1.0, 1.0, 1.0, 0);
	const RT_Color gray (0.5, 0.5, 0.5, 0.5);
	const RT_Color maroon (1.0, 0.0, 0.0, 0.3);
	const RT_Color pink (1.0, 0.0, 1.0, 0.3);

	cam_pos = Vector (3, 1.5, -4);
	look_at = Vector (0, 0, 0);
	std::string abs_path = "";
	std::string filename = "resizedTeapot.obj";
	try {
		path relative_path = "../../../../5. Modelle/5.1 Beispielmodelle/Test/" + filename;
		path absolute_path = absolute(relative_path);
		path canonical_path = canonical(absolute_path);
		if (!exists(canonical_path)) {
			throw std::runtime_error("Path does not exist: " + canonical_path.string());
		}
		abs_path = canonical_path.string();
		std::replace(abs_path.begin(), abs_path.end(), '\\', '/');
	}catch (const filesystem_error& e) {
		throw std::runtime_error("Filesystem error: " + std::string(e.what()));
	} catch (const std::exception& e) {
		throw std::runtime_error("Error: " + std::string(e.what()));
	}
	const std::vector<RT_Vertex> vertices = RT_Object_Importer::fetch_vertices(abs_path);

    for(int i = 0; i <= vertices.size()-3 ; i = i+3) {
    	auto* scene_triangle = new Triangle(Vector (vertices[i].position.x,vertices[i].position.y,vertices[i].position.z),
    												Vector(vertices[(i+1)].position.x,vertices[(i+1)].position.y,vertices[(i+1)].position.z),
    												Vector(vertices[(i+2)].position.x,vertices[(i+2)].position.y,vertices[(i+2)].position.z),
    												pink);
    	objects_scene.push_back(scene_triangle);

    }

    Logger::log(MessageType::DEBUG, "Objects: " + std::to_string(objects_scene.size()));


	auto* scene_plane = new Plane(Y, 0, gray);
	objects_scene.push_back(scene_plane);

	//Vector light_position (3, 300, 0); // für teapot
	Vector light_position (10, 1, 10);
	auto* scene_light = new Light(light_position, white_light);
	light_sources.push_back(scene_light);
}

void RT_Scene::sphereScene() {
	Logger::log(MessageType::INFO, "Rendering the sphere scene...!");
	const Vector new_sphere_location (1.75, 0.5, 0);
	const Vector new_sphere_location2 (-1.75, 1, 0);
	const Vector new_sphere_location3 (0, 1, 0);

	cam_pos = Vector(5, 3, -1.75);
	look_at = Vector (0, 1, 0);

	const Vector Y (0,1,0);
	const RT_Color white_light (1.0, 1.0, 1.0, 0);
	const RT_Color pretty_green (0.5, 1.0, 0.5, 0.3);
	const RT_Color blue (0.1, 0.1, 0.8, 0.3);
	const RT_Color orange (0.7, 0.25, 0.25, 0.3);
	const RT_Color tile_floor (0.5, 0.5, 0.5, 0.5);
	const RT_Color gray (0.5, 0.5, 0.5, 0.3);

	std::string abs_path = "";
	std::string filename = "sceneWalls.obj";
	try {
		path relative_path = "../../../../5. Modelle/5.1 Beispielmodelle/Test/" + filename;
		path absolute_path = absolute(relative_path);
		path canonical_path = canonical(absolute_path);
		if (!exists(canonical_path)) {
			throw std::runtime_error("Path does not exist: " + canonical_path.string());
		}
		abs_path = canonical_path.string();
		std::replace(abs_path.begin(), abs_path.end(), '\\', '/');
	}catch (const filesystem_error& e) {
		throw std::runtime_error("Filesystem error: " + std::string(e.what()));
	} catch (const std::exception& e) {
		throw std::runtime_error("Error: " + std::string(e.what()));
	}
	const std::vector<RT_Vertex> vertices = RT_Object_Importer::fetch_vertices(abs_path);

	int iter = 0;
	RT_Color selectedColor = orange;
	for(int i = 0; i <= vertices.size()-3 ; i = i+3) {
		auto* scene_triangle = new Triangle(Vector (vertices[i].position.x,vertices[i].position.y,vertices[i].position.z),
													Vector(vertices[(i+1)].position.x,vertices[(i+1)].position.y,vertices[(i+1)].position.z),
													Vector(vertices[(i+2)].position.x,vertices[(i+2)].position.y,vertices[(i+2)].position.z),
													selectedColor);
		objects_scene.push_back(scene_triangle);
		iter++;
		if (iter >= 2 ) selectedColor = blue;
		if (iter >= 4 ) selectedColor = gray;
	}


	const Vector light_position (0,4,0);
	auto* scene_light = new Light(light_position, white_light);
	light_sources.push_back(scene_light);

	// scene objects
	auto* scene_sphere = new Sphere(new_sphere_location3, 1, pretty_green);
	auto* scene_sphere2 = new Sphere(new_sphere_location, 0.5, orange);
	auto* scene_sphere3 = new Sphere(new_sphere_location2, 1, gray);
	//Triangle scene_triangle2 (Vect (-2,0,0), Vect(0,0,-3), Vect(4,0,0), gray);
	auto* scene_plane = new Plane(Y, 0, tile_floor);
	objects_scene.push_back(scene_sphere);
	objects_scene.push_back(scene_sphere2);
	objects_scene.push_back(scene_sphere3);
	objects_scene.push_back(scene_plane);
	//scene_objects.push_back(dynamic_cast<Object*>(&scene_triangle2));
}

void RT_Scene::previewScene() {
	Logger::log(MessageType::INFO, "Rendering the previewed scene...!");

	auto resources = Main_Scene::getResources();
	auto components = Main_Scene::getComponents();

	glm::mat4 mvp_matrix, projection_matrix, view_matrix, model_matrix;

	for (auto& pair  : components)
	{
		shared_ptr<Camera_Component> camera = dynamic_pointer_cast<Camera_Component>(pair.second);
		shared_ptr<Light_Component> light = dynamic_pointer_cast<Light_Component>(pair.second);
		shared_ptr<Render_Component> render = dynamic_pointer_cast<Render_Component>(pair.second);
		if(camera)
		{
			std::pair<glm::vec3, glm::vec3> camera_pair = Preview_Canvas::calculateCameraVectors(camera->get_position(), camera->get_rotation());
			view_matrix       = glm::lookAt(camera->get_position(), camera_pair.first, camera_pair.second);
			projection_matrix = glm::perspective(glm::radians(camera->get_fov()), camera->get_aspect_ratio(), camera->get_near_clip(), camera->get_far_clip());
			cam_pos = Vector(camera->get_position().x, camera->get_position().y, -camera->get_position().z);
		}
		else if(light)
		{
			Vector lightpos = Vector(light->get_position().x, light->get_position().y, light->get_position().z);
			RT_Color lightcolr (light->get_color().x/255*light->get_intensity(),
				light->get_color().y/255*light->get_intensity(),
				light->get_color().z/255*light->get_intensity(),
				0);
			light_sources.push_back(new Light(lightpos, lightcolr));
		}
		else if (render)
		{
			auto objIt = resources.find(render->get_object_UUID());
			shared_ptr<Object_Resource> objRes = dynamic_pointer_cast<Object_Resource>(objIt->second);
			const std::vector<Vertex> vertices = objRes->get_vertices();
			const std::vector<Indice> indices = objRes->get_indices();

			model_matrix = Preview_Canvas::extract_Model_Matrix(render);

			for (auto &i : indices) {
					std::vector<int> vertex_idx = i.vertexIndices_v;
					int v1 = vertex_idx[0];
					int v2 = vertex_idx[1];
					int v3 = vertex_idx[2];

					auto* scene_triangle = new Triangle(
				Vector(
						vertices[v1].position.x,
						vertices[v1].position.y,
						vertices[v1].position.z
					).multiply(model_matrix),
					Vector(
							vertices[v2].position.x,
							vertices[v2].position.y,
							vertices[v2].position.z
					).multiply(model_matrix),
				Vector(
						vertices[v3].position.x,
						vertices[v3].position.y,
						vertices[v3].position.z
					).multiply(model_matrix),
					RT_Color(
						vertices.at(v1).color.x,
						vertices.at(v1).color.y,
						vertices.at(v1).color.z,
						0.3
					)
				);
					objects_scene.push_back(scene_triangle);
			}
		}
	}

	look_at = Vector (0, 0, 0);

	const RT_Color plane_color (0.5, 0.5, 0.5, 0.5);
	auto* scene_plane = new Plane(Vector(0, 1, 0), 0, plane_color);
	objects_scene.push_back(scene_plane);
}
