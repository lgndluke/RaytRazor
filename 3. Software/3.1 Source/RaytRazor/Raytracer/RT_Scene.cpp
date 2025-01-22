#include "../UI/Scenes/Main/Main_Scene.h"
#include "RT_Scene.h"

#include "../UI/Widget/ComponentAttributes_Widget.h"

RT_Scene::RT_Scene() {
	if (auto components = Main_Scene::getComponents(); components.empty()) {
		throw invalid_argument("No scene components were found");
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

	constexpr double factor = 39.375;
	const int m = static_cast<int>(factor);

	const int ppm = dpi*m;

	unsigned char bmp_file_header[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
	unsigned char bmp_info_header[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

	bmp_file_header[ 2] = static_cast<unsigned char>(filesize);
	bmp_file_header[ 3] = static_cast<unsigned char>(filesize>>8);
	bmp_file_header[ 4] = static_cast<unsigned char>(filesize>>16);
	bmp_file_header[ 5] = static_cast<unsigned char>(filesize>>24);

	bmp_info_header[ 4] = static_cast<unsigned char>(w);
	bmp_info_header[ 5] = static_cast<unsigned char>(w>>8);
	bmp_info_header[ 6] = static_cast<unsigned char>(w>>16);
	bmp_info_header[ 7] = static_cast<unsigned char>(w>>24);

	bmp_info_header[ 8] = static_cast<unsigned char>(h);
	bmp_info_header[ 9] = static_cast<unsigned char>(h>>8);
	bmp_info_header[10] = static_cast<unsigned char>(h>>16);
	bmp_info_header[11] = static_cast<unsigned char>(h>>24);

	bmp_info_header[21] = static_cast<unsigned char>(s);
	bmp_info_header[22] = static_cast<unsigned char>(s>>8);
	bmp_info_header[23] = static_cast<unsigned char>(s>>16);
	bmp_info_header[24] = static_cast<unsigned char>(s>>24);

	bmp_info_header[25] = static_cast<unsigned char>(ppm);
	bmp_info_header[26] = static_cast<unsigned char>(ppm>>8);
	bmp_info_header[27] = static_cast<unsigned char>(ppm>>16);
	bmp_info_header[28] = static_cast<unsigned char>(ppm>>24);

	bmp_info_header[29] = static_cast<unsigned char>(ppm);
	bmp_info_header[30] = static_cast<unsigned char>(ppm>>8);
	bmp_info_header[31] = static_cast<unsigned char>(ppm>>16);
	bmp_info_header[32] = static_cast<unsigned char>(ppm>>24);

	if (const errno_t err = fopen_s(&f, filename,"wb"); err != 0) {
		Logger::log(MessageType::SEVERE, "Error opening bmp file!");
	}

	fwrite(bmp_file_header,1,14,f);
	fwrite(bmp_info_header,1,40,f);

	for (int y = h - 1; y >= 0; y--) {
		for (int x = 0; x < w; x++) {
			const int i = y*w + x;
			const double red = data[i].r*255;
			const double green = data[i].g*255;
			const double blue = data[i].b*255;

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
					}
				}
			}

			if (shadowed == false)
			{
				final_color = final_color.addColor(winning_object_color.multiplyColor(light_source->getLightColor()).colorScalar(cosine_angle));

				if (winning_object_color.getAlpha() > 0 && winning_object_color.getAlpha() <= 1)
				{
					double dot1 = winning_object_normal.dot(intersecting_ray_direction.negative());
					Vector scalar1 = winning_object_normal.multiply(dot1);
					Vector add1 = scalar1.add(intersecting_ray_direction);
					Vector scalar2 = add1.multiply(2);
					Vector add2 = intersecting_ray_direction.negative().add(scalar2);
					Vector reflection_direction = add2.normalize();

					if (double specular = reflection_direction.dot(light_direction); specular > 0)
					{
						specular = pow(specular, 10);
						final_color = final_color.addColor(light_source->getLightColor().colorScalar(specular*winning_object_color.getAlpha()));
					}
				}

			}

		}
	}

	return final_color.clip();
}

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

	double aspect_ratio = static_cast<double>(width)/static_cast<double>(height);

	Vector Y (0,1,0);
	Vector diff_btw (cam_pos.getX() - look_at.getX(), cam_pos.getY() - look_at.getY(), cam_pos.getZ() - look_at.getZ());
	Vector camera_direction = diff_btw.negative().normalize();
	Vector camera_right = Y.cross(camera_direction).normalize();
	Vector camera_down = camera_right.cross(camera_direction);
	camera_rt = Camera (cam_pos, camera_direction, camera_right, camera_down);

	int current_pixel, aa_index;
	double xamnt, yamnt;
	double temp_red, temp_green, temp_blue;
	#pragma omp parallel for collapse(2) private(current_pixel, aa_index, xamnt, yamnt, temp_red, temp_green, temp_blue)
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			int aadepth = 1;
			current_pixel = y*width + x;

			double temp_red[1];
			double temp_green[1];
			double temp_blue[1];

			for (int aax = 0; aax < aadepth; aax++)
			{
				for (int aay = 0; aay < aadepth; aay++)
				{

					aa_index = aay*aadepth + aax;

					srand(time(nullptr));

					if (width > height)
					{
						xamnt = ((x+0.5)/width)*aspect_ratio - (((width-height)/(double)height)/2);
						yamnt = (y + 0.5) / height;
					}
					else if (height > width)
					{
						xamnt = (x + 0.5)/ width;
						yamnt = ((y + 0.5)/height)/aspect_ratio - (((height - width)/(double)width)/2);
					}
					else
					{
						xamnt = (x + 0.5)/width;
						yamnt = (y + 0.5)/height;
					}

					Vector cam_ray_origin = camera_rt.getCamPos();
					Vector cam_ray_direction = camera_direction.add(camera_right.multiply(xamnt - 0.5).add(camera_down.multiply(yamnt - 0.5))).normalize();

					Ray cam_ray (cam_ray_origin, cam_ray_direction);

					vector<double> intersections;

					for (auto & index : objects_scene)
					{
						intersections.push_back(index->hit(cam_ray));
					}

					if (int index_of_winning_object = winningObjectIndex(intersections); index_of_winning_object == -1)
					{
						// set the backgroung black
						temp_red[aa_index] = 0;
						temp_green[aa_index] = 0;
						temp_blue[aa_index] = 0;
					}
					else if (double accuracy = 1e-8; intersections.at(index_of_winning_object) > accuracy)
					{
						double ambient_light = 0.2;

						Vector intersection_position = cam_ray_origin.add(cam_ray_direction.multiply(intersections.at(index_of_winning_object)));
						Vector intersecting_ray_direction = cam_ray_direction;

						RT_Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction, objects_scene, index_of_winning_object, light_sources, accuracy, ambient_light);

						temp_red[aa_index] = intersection_color.getRed();
						temp_green[aa_index] = intersection_color.getGreen();
						temp_blue[aa_index] = intersection_color.getBlue();
					}
				}
			}

			double totalRed = 0;
			double totalGreen = 0;
			double totalBlue = 0;

			for (int iRed = 0; iRed < aadepth*aadepth; iRed++)
			{
				totalRed = totalRed + temp_red[iRed];
			}
			for (int iGreen = 0; iGreen < aadepth*aadepth; iGreen++)
			{
				totalGreen = totalGreen + temp_green[iGreen];
			}
			for (int iBlue = 0; iBlue < aadepth*aadepth; iBlue++)
			{
				totalBlue = totalBlue + temp_blue[iBlue];
			}

			double avgRed = totalRed/(aadepth*aadepth);
			double avgGreen = totalGreen/(aadepth*aadepth);
			double avgBlue = totalBlue/(aadepth*aadepth);

			pixels[current_pixel].r = avgRed;
			pixels[current_pixel].g = avgGreen;
			pixels[current_pixel].b = avgBlue;

			output.SetPixel(x, y, avgRed, avgGreen, avgBlue);
		}
	}

	savebmp("Raytracing_Scene.bmp",width,height,dpi,pixels);

	delete pixels, temp_red, temp_green, temp_blue;

	t2 = clock();
	float diff = (static_cast<float>(t2) - static_cast<float>(t1))/1000;

	Logger::log(MessageType::INFO, "Done in: " + std::to_string(diff) + " seconds!");
	for (RT_Object* obj : objects_scene)
	{
		delete obj;
	}
	objects_scene.clear();

	for (RT_LightSource* light : light_sources)
	{
		delete light;
	}
	light_sources.clear();

	return true;
}

void RT_Scene::previewScene()
{
	Logger::log(MessageType::INFO, "Rendering the previewed scene...!");

	auto resources = Main_Scene::getResources();
	auto components = Main_Scene::getComponents();

	glm::mat4 view_matrix, model_matrix;

	for (auto& pair  : components)
	{
		shared_ptr<Camera_Component> camera = dynamic_pointer_cast<Camera_Component>(pair.second);
		shared_ptr<Light_Component> light = dynamic_pointer_cast<Light_Component>(pair.second);
		shared_ptr<Render_Component> render = dynamic_pointer_cast<Render_Component>(pair.second);
		if(camera)
		{
			std::pair<glm::vec3, glm::vec3> camera_pair = Preview_Canvas::calculateCameraVectors(camera->get_position(), camera->get_rotation());
			view_matrix = glm::lookAt(camera->get_position(), camera_pair.first, camera_pair.second);
			look_at = Vector(camera_pair.first.x, camera_pair.first.y, camera_pair.first.z).multiply(view_matrix);
			cam_pos = Vector(camera_pair.first.x, camera_pair.first.y, -camera_pair.first.z);
		}
		else if(light)
		{
			auto light_position = Vector(light->get_position().x, light->get_position().y, light->get_position().z);
			RT_Color light_color (light->get_color().x/255*light->get_intensity(),
				light->get_color().y/255*light->get_intensity(),
				light->get_color().z/255*light->get_intensity(),
				0);
			light_sources.push_back(new Light(light_position, light_color));
		}
		else if (render)
		{
			auto objIt = resources.find(render->get_object_UUID());
			shared_ptr<Object_Resource> objRes = dynamic_pointer_cast<Object_Resource>(objIt->second);
			const std::vector<Vertex> vertices = objRes->get_vertices();
			const std::vector<Indice> indices = objRes->get_indices();

			model_matrix = Preview_Canvas::extract_Model_Matrix(render);

			for (auto &i : indices)
			{
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
					0.5
				));
				objects_scene.push_back(scene_triangle);
			}
		}
	}
}
