#include<iostream>
#include<filesystem>
#include<chrono>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;
using namespace std::chrono;

uint8_t sharpen_pixel(uint8_t* input_image, float kernel[3][3], int c, int i, int j, int w, int h) {
	float sum = 0;
	for (int di = -1; di <= 1; di++) {
		for (int dj = -1; dj <= 1; dj++) {
			int index = ((i + di) * w + j + dj) * 3 + c;
			sum += (float)input_image[index] / 255.0 * kernel[1 + di][1 + dj];
		}
	}
	return max(min((int)(sum * 255), 255), 0);
}

void sharpen_img(string in_path, string out_path, float kernel[3][3]) {
	int w, h, comp;
	uint8_t* img = stbi_load(in_path.c_str(), &w, &h, &comp, 0);
	uint8_t* img_out = (uint8_t*)malloc((w - 2) * (h - 2) * comp * sizeof(uint8_t));
	for (int c = 0; c < comp; c++) {
		for (int i = 0; i < h - 2; i++) {
			for (int j = 0; j < w - 2; j++) {
				int index = (i * (w - 2) + j) * comp + c;
				img_out[index] = sharpen_pixel(img, kernel, c, i + 1, j + 1, w, h);
			}
		}
	}
	stbi_write_png(out_path.c_str(), w - 2, h - 2, comp, img_out, (w - 2) * comp);
	cout << "Image writting done to: " << out_path << endl;
	delete[] img;
	delete[] img_out;
}

int main() {

	string input_folder = "input_images";
	string output_folder = "output_images";
	int count = 0;
	float kernel[3][3] = {
		{-1/9, -1/9, -1/9},
		{-1/9, 17/9, -1/9},
		{-1/9, -1/9, -1/9},
	};
	auto start_t = high_resolution_clock::now();
#pragma omp parallel default(shared) num_threads(8)
	for (auto path : filesystem::directory_iterator(input_folder)) {
#pragma omp single
		{
			string input_path = input_folder + "/" + path.path().filename().string();
			string output_path = output_folder + "/" + path.path().filename().string();
			count++;
#pragma omp task
			sharpen_img(input_path, output_path, kernel);
		}
	}
	auto end_t = high_resolution_clock::now();

	cout << "Time: " << duration_cast<milliseconds>(end_t - start_t).count();

	return 0;
}