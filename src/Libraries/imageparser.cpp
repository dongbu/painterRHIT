#pragma once

#include <opencv/cv.h>

#ifdef __APPLE__
#include "kmeansSegment.cpp"
#endif

#include "kmeansSegment.hpp"

#include "drawwindow.cpp"
#include "shapes.cpp"

// ==== HELPER FUNCTIONS =======
//(moved inside of ImageParser (to aid in compilation non-redefinitions)).
// =============================

// Base class that takes an image and returns drawing data
class ImageParser {
protected:
	int use_random_colors; // mostly for debugging
	int debug;
public:
	void useRandomColors(int r) { use_random_colors = r; }
	void setDebug(int d) { debug = d; }

	// given an image, look for single pixels surrounded by another color... replace by neighbor's
	void reduceSpecsInImage(cv::Mat *image, float min_neighbor_percent = .61) {
		cv::Vec3b neighbor_colors[8]; // index lookup of colors, 8=max number of neighbors
		int neighbor_counts[8]; //
		int n_changed = 0; // how many pixels were changed
		int debug = 0;

		for (int i = 0; i < image->cols; i++) {
			for (int j = 0; j < image->rows; j++) {
				//      if (n_changed>10) { debug = 0; }

				cv::Vec3b color = image->at<cv::Vec3b>(cv::Point(i, j));

				// zero out counts of previous pixel test
				for (int n = 0; n < 8; n++) { neighbor_counts[n] = 0; }
				int n_colors_found = 0;

				// count how many of each color of the neighbors of i,j
				int left = std::max(i - 1, 0);
				int right = std::min(i + 1, image->cols - 1);
				int top = std::max(j - 1, 0);
				int bottom = std::min(j + 1, image->rows - 1);
				int pixels = (1 + right - left)*(1 + bottom - top) - 1; // how many neighbor pixels to compare

				if (debug) printf("[%i,%i](%d,%d,%d) (%d-%d x %d-%d)=%d neighbors\n", i, j, color[0], color[1], color[2], left, right, top, bottom, pixels);

				for (int n = left; n <= right; n++) {
					for (int m = top; m <= bottom; m++) {
						if (n != i || m != j) {
							// try to look up the color id of the neighbor
							cv::Vec3b neighbor_color = image->at<cv::Vec3b>(cv::Point(n, m));
							int color_id = -1;
							for (int id = 0; id < n_colors_found; id++) {
								if (neighbor_color[0] == neighbor_colors[id][0] &&
									neighbor_color[1] == neighbor_colors[id][1] &&
									neighbor_color[2] == neighbor_colors[id][2]) {
									color_id = id;
								}
							}
							if (color_id == -1) { // neighbor pixel is a new color
								color_id = n_colors_found;
								neighbor_colors[color_id] = neighbor_color;
								n_colors_found++;
								if (debug) printf(" color[%d,%d] id %d = (%d,%d,%d)\n", n, m, color_id, neighbor_color[0], neighbor_color[1], neighbor_color[2]);
							}
							neighbor_counts[color_id]++;
						}
					}
				}

				// find the max number of a given color
				int max = 0;
				int max_color_id = 0;
				for (int id = 0; id < n_colors_found; id++) {
					if (neighbor_counts[id] > max) {
						max = neighbor_counts[id];
						max_color_id = id;
					}
					if (debug) printf(" id %d appears %d times\n", id, neighbor_counts[id]);
				}

				// if a lot of the neighbors are a different color than i,j, then change the color
				float percent_max = (float)max / pixels;
				if (debug) printf(" max color id %d has %d times (%3.2f percent) = (%d,%d,%d)\n", max_color_id, max, percent_max * 100,
					neighbor_colors[max_color_id][0], neighbor_colors[max_color_id][1], neighbor_colors[max_color_id][2]);

				float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				if (percent_max + r*0.15 > min_neighbor_percent && (color[0] != neighbor_colors[max_color_id][0] ||
					color[1] != neighbor_colors[max_color_id][1] ||
					color[2] != neighbor_colors[max_color_id][2])) {
					if (debug) printf(" - UPDATE color\n");
					// debug
					//neighbor_colors[max_color_id][0]=0;
					//neighbor_colors[max_color_id][1]=255;
					//neighbor_colors[max_color_id][2]=0;
					image->at<cv::Vec3b>(cv::Point(i, j)) = neighbor_colors[max_color_id];
					n_changed++;
				}
			}
		}
		if (debug) printf("Despecked %d pixels\n", n_changed);
	}


	// given an image, return a sorted list of colors (darkest first)
	void sortColorsInImage(cv::Mat *image, std::vector<cv::Vec3b>& sorted_colors) {
		cv::Vec3b colors[2048]; // max number of colors
		long colors_count[2048] = { 0 };
		int n_colors = 0;

		// scan image and count colors
		for (int i = 0; i < image->cols; i++) {
			for (int j = 0; j < image->rows; j++) {
				int found = -1;
				//cv::Vec3b color = kmeans_image.at<cv::Vec3b>(cv::Point(i,j));
				cv::Vec3b color = image->at<cv::Vec3b>(cv::Point(i, j));
				for (int k = 0; k < n_colors; k++) {
					if (color[0] == colors[k][0] && color[1] == colors[k][1] && color[2] == colors[k][2]) {
						found = k;
						colors_count[k]++;
						continue;
					}
				}
				if (found == -1) {
					colors[n_colors] = color;
					n_colors++;
				}
			}
		}

		for (int i = 0; i < n_colors; i++) {
			//printf("%d: [%d,%d,%d] = %ld NORM:%f\n",i,colors[i][0],colors[i][1],colors[i][2],colors_count[i],norm(colors[i]));
		}

		// create a sorted list
		int* used_colors = new int[n_colors]; // don't forget to delete this later
		for (int i = 0; i < n_colors; i++) { used_colors[i] = 0; }

		for (int i = 0; i < n_colors; i++) {
			// find the darkest unused color
			int darkest = 255 * 255 * 255;
			int darkest_index = -1;
			for (int j = 0; j < n_colors; j++) {
				if (!used_colors[j] && norm(colors[j]) <= darkest) {
					darkest = norm(colors[j]);
					darkest_index = j;
				}
			}
			if (darkest_index >= 0) { // should always be true
				used_colors[darkest_index] = 1;
				sorted_colors.push_back(colors[darkest_index]);
			}
		}
		delete[] used_colors;

		for (int i = 0; i < n_colors; i++) {
			//printf("SORTED %d: [%d,%d,%d] = NORM:%f\n",i,sorted_colors[i][0],sorted_colors[i][1],sorted_colors[i][2],norm(sorted_colors[i]));
		}
	}


	int parseImage(cv::Mat image) {
		cv::Size s = image.size();
		printf("parsing image (%i x %i pixels)\n", s.width, s.height);
		return 1;
	}
	ImageParser() { use_random_colors = 0; debug = 0; }
};

// Takes an image and returns contour lines via Canny filter
class ImageParserContours : public ImageParser {
protected:
	int min_contour_length;
	int canny_threshold;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::Mat grey_image;

public:
	void setMinContourLength(int len) { min_contour_length = len; }
	void setCannyThreshold(int val) { canny_threshold = val; }

	int parseImage(cv::Mat image) {
		cvtColor(image, grey_image, CV_BGR2GRAY);
		GaussianBlur(grey_image, grey_image, cv::Size(7, 7), 1.5, 1.5);
		Canny(grey_image, grey_image, canny_threshold, canny_threshold * 2, 3);

		if (debug) {
			/// Show in a window
			cv::namedWindow("Canny", CV_WINDOW_AUTOSIZE);
			cv::moveWindow("Canny", image.cols + 10, 30);
			cv::imshow("Canny", grey_image);
		}

		/// Find contours
		findContours(grey_image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

		// only finds the most external contours
		//findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
		return 1;
	}

	void defineShapes(Shapes *S, int skip_freq = 1) {
		// simplify contours
		std::vector<std::vector<cv::Point> > contours_poly(contours.size());
		for (size_t i = 0; i < contours.size(); i++) {
			approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 1.1, true);
		}

		cv::RNG rng(12345);
		for (size_t i = 0; i< contours.size(); i++) {
			PolyLine *PL = new PolyLine();
			PL->setPenColor(0, 0, 0);
			if (use_random_colors) { PL->setPenColor(rng.uniform(100, 200), rng.uniform(100, 200), rng.uniform(100, 200)); }

			if ((signed)contours_poly[i].size()>min_contour_length) {
				// printf("Contour:%i [%lu points]\n",i,contours_poly[i].size());
				for (size_t j = 0; j < contours_poly[i].size(); j+=skip_freq) {
					//printf("(%i,%i)",contours[i][j].x,contours[i][j].y);
					//if (j!=contours_poly[i].size()-1) { printf(","); }
					PL->addPoint(contours_poly[i][j].x, contours_poly[i][j].y);
				}
				S->addShape(PL);
				//printf("\n");
			}
		}
	}

	void draw() {
		// simplified contours
		std::vector<std::vector<cv::Point> > contours_poly(contours.size());
		for (size_t i = 0; i < contours.size(); i++) {
			approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 1.1, true);
		}
		/// Draw contours
		int num_drawn = 0;
		cv::Mat drawing = cv::Mat::zeros(grey_image.size(), CV_8UC3);
		drawing.setTo(cv::Scalar(200, 200, 200)); // b,g,r); 

		cv::RNG rng(12345);
		for (size_t i = 0; i< contours.size(); i++) {
			cv::Scalar color = cv::Scalar(255, 255, 255);
			if (use_random_colors) { color = cv::Scalar(rng.uniform(100, 200), rng.uniform(100, 200), rng.uniform(100, 200)); }

			if ((signed)contours_poly[i].size()>min_contour_length) {
				num_drawn++;
				drawContours(drawing, contours_poly, i, color, 1, 8, hierarchy, 0, cv::Point());
			}

			// test printing out some contour points
			if (i % 8 == 3) {
			  printf("Contour:%lu [%lu points]\n", i, contours_poly[i].size());
			  for (size_t j = 0; j < contours_poly[i].size(); j++) {
			    printf("(%i,%i)", contours[i][j].x, contours[i][j].y);
			    if (j != contours_poly[i].size() - 1) { printf(","); }
			  }
			  printf("\n");
			}
		}

		//char text[50];
		//sprintf(text,"Lines: drew %i out of %lu",num_drawn,contours.size());
		//putText(drawing, text, cvPoint(30,30), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(200,200,250), 1, CV_AA);
		printf("Lines: drew %i out of %lu", num_drawn, contours.size());

		cv::namedWindow("Contour Lines", CV_WINDOW_AUTOSIZE);
		cv::imshow("Contour Lines", drawing);
		//while (cv::waitKey(33)<0) { }
	}

	ImageParserContours() : ImageParser() {
		min_contour_length = 10;
		canny_threshold = 100;
	}

};


// Takes an image and returns pixel regions
class ImageParserKmeans : public ImageParser {
protected:
	int colors; // number of colors
	int blur_loops; // for kmeans
	int min_region_pixels; // smallest number of pixels to be in a region
	int reduce_specs_loops;
	cv::Mat kmeans_image;

public:
	void setNumColors(int num) { colors = num; }
	void setBlurLoops(int num) { blur_loops = num; }
	void setMinPixelsInRegion(int min = 5) { min_region_pixels = min; }
	void setReduceSpecsLoops(int num) { reduce_specs_loops = num; }
	std::vector<std::vector<cv::Point> > regions; // array of arrays of pixels in a region
	std::vector<cv::Vec3b> region_colors;

	int parseImage(cv::Mat image, int group_same_color_regions = 1) {
		kmeansSegment kmeans(colors);
		// KMEANS_RANDOM_CENTERS Select random initial centers in each attempt.
		// KMEANS_PP_CENTERS Use kmeans++ center initialization by Arthur and Vassilvitskii [Arthur2007].
		// KMEANS_USE_INITIAL_LABELS During the first (and possibly the only) attempt,
		///kmeans.set_criteria(KMEANS_RANDOM_CENTERS);
		//blur( image, image, cv::Size(3,3) );
		kmeans_image = kmeans.segment(image);

		// blur and try again... perhaps will make the transitions more smooth
		//result = adaptiveThreshold(result,255,ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,11,2)
		if (blur_loops) {
			for (int i = 0; i < 2; i++) {
				blur(kmeans_image, kmeans_image, cv::Size(3, 3));
				kmeans_image = kmeans.segment(kmeans_image);
			}
		}
		if (debug) printf("Colors:%i Blur Loops:%i\n", colors, blur_loops);

		std::vector<cv::Vec3b> colors; // uninitialized.  Let sortColorsInImage fill it.
		sortColorsInImage(&kmeans_image, colors); // prints out which colors are in image
		for (size_t i = 0; i < colors.size(); i++) {
			if (debug) printf("Defined color #%lu: [%d,%d,%d] = brightness norm:%f\n", i, colors[i][0], colors[i][1], colors[i][2], norm(colors[i]));
		}

		if (debug) {
			/// Show in a window
			cv::namedWindow("kmeans_window", CV_WINDOW_AUTOSIZE);
			cv::moveWindow("kmeans_window", 100, 100);
			cv::imshow("kmeans_window", kmeans_image);
		}

		for (int i = 0; i < reduce_specs_loops; i++) {
			reduceSpecsInImage(&kmeans_image);
		}

		if (debug) {
			cv::namedWindow("kmeans_window_despeck", CV_WINDOW_AUTOSIZE);
			cv::moveWindow("kmeans_window_despeck", 100, 100);
			cv::imshow("kmeans_window_despeck", kmeans_image);
		}

		if (group_same_color_regions) {
			defineSameColorRegionsInImage(&kmeans_image);
		}
		else {
			defineContiquousRegionsInImage(&kmeans_image);
		}

		if (debug) {
			cv::namedWindow("kmeans_window_regions", CV_WINDOW_AUTOSIZE);
			cv::moveWindow("kmeans_window_regions", 700, 100);
			cv::imshow("kmeans_window_regions", kmeans_image);
		}

		return 1;
	}

	// given an image, defines list of lists of pixels that are the same color
	void defineSameColorRegionsInImage(cv::Mat *image) {
		std::vector<cv::Vec3b> colors; // uninitialized.  Let sortColorsInImage fill it.
		sortColorsInImage(image, colors); // prints out which colors are in image
		for (size_t c = 0; c < colors.size(); c++) {
			std::vector<cv::Point> region; // points in this region (auto expands array if needed)
			for (int i = 0; i < image->cols; i++) {
				for (int j = 0; j < image->rows; j++) {
					cv::Vec3b color = image->at<cv::Vec3b>(cv::Point(i, j));
					if (color[0] == colors[c][0] && color[1] == colors[c][1] && color[2] == colors[c][2]) {
						region.push_back(cv::Point(i, j));
					}
				}
			}
			regions.push_back(region);
			region_colors.push_back(colors[c]);
		}
	}

	// given an image, defines a list of lists of contiguous regions in pixels (e.g., if an image has
	// 3 separate regions of red, each region will be a different list of pixels)
	void defineContiquousRegionsInImage(cv::Mat *image) {
		cv::RNG rng(12345);
		int debug = 0;
		//cv::Mat done_image = image->clone();
		int rows = image->rows;
		int cols = image->cols;
		int* done = new int[rows*cols]; // which pixels are done
		std::vector<cv::Point> wavefront; // will automatically allocate member if needed
		int num_regions = 0;

		for (int i = 0; i < rows*cols; i++) { done[i] = -1; }

		std::vector<cv::Vec3b> colors; // uninitialized.  Let sortColorsInImage fill it.
		sortColorsInImage(image, colors); // prints out which colors are in image
		for (size_t c = 0; c < colors.size(); c++) {

			if (debug) printf("Finding regions of color:(%d,%d,%d)\n", colors[c][0], colors[c][1], colors[c][2]);
			int start_i = 0; // where left off looking for a color

			cv::Vec3b set_color;
			if (debug) {
				set_color[0] = rng.uniform(100, 200);
				set_color[1] = rng.uniform(100, 200);
				set_color[2] = rng.uniform(100, 200);
			}
			else {
				set_color = colors[c];
			}

			int num_regions_for_a_color = 0;
			int found_color = 1;
			while (found_color) {
				found_color = 0;
				std::vector<cv::Point> region; // points in this region (auto expands array if needed)

				// find one pixel of this color that hasn't been done
				for (int i = start_i; i < cols; i++) {
					start_i = i; // for speedup
					for (int j = 0; j < rows; j++) {
						if (done[i*rows + j] == -1) {
							cv::Vec3b color = image->at<cv::Vec3b>(cv::Point(i, j));
							if (color[0] == colors[c][0] && color[1] == colors[c][1] && color[2] == colors[c][2]) {
								if (!found_color) { // *if* added to reduce bugs?
									//printf(" - first pixel found at pixel %d,%d\n",i,j);
									wavefront.push_back(cv::Point(i, j));
									region.push_back(cv::Point(i, j));
									num_regions++;
									num_regions_for_a_color++;
									done[i*rows + j] = num_regions; // 1=is in wavefront
									//printf("X:%d %d to %d %d\n",i,j,region[0].x,region[0].y);
									//printf("X:%d %d to %d %d\n",i,j,wavefront[0].x,wavefront[0].y);
								}

								found_color = 1;
								i = image->cols;
								j = image->rows;
							}
						}
					}
				}

				if (found_color) {

					// expand a wavefront from the found pixel that matches the color
					// wavefront from that pixel and mark all adjacent ones
					while (wavefront.size()) {
						cv::Point p = wavefront[wavefront.size() - 1];
						wavefront.pop_back();

						// count how many of each color of the neighbors of i,j
						int left = std::max(p.x - 1, 0);
						int right = std::min(p.x + 1, cols - 1);
						int top = std::max(p.y - 1, 0);
						int bottom = std::min(p.y + 1, rows - 1);
						//int pixels=(1+right-left)*(1+bottom-top) - 1; // how many neighbor pixels to compare

						for (int n = left; n <= right; n++) {
							for (int m = top; m <= bottom; m++) {
								if (n != p.x && m != p.y && done[n*rows + m] == -1) { // candidate pixel
									cv::Vec3b color = image->at<cv::Vec3b>(cv::Point(n, m));
									if (color[0] == colors[c][0] && color[1] == colors[c][1] && color[2] == colors[c][2]) {
										done[n*rows + m] = num_regions; // in wavefront
										wavefront.push_back(cv::Point(n, m));
										region.push_back(cv::Point(n, m));
									}
								}
							}
						}
					}

					regions.push_back(region);
					region_colors.push_back(colors[c]);

					// print out region
					if (debug) {
						printf("REGION %d.%d [%d,%d](%d,%d,%d): %lu points\n", num_regions, num_regions_for_a_color,
							region[0].x, region[0].y,
							colors[c][0], colors[c][1], colors[c][2], region.size());
					}
					if ((signed)region.size() >= min_region_pixels) {
						for (size_t i = 0; i < region.size(); i++) {
							if (i < 10) {
								if (debug) {
									printf("(%d,%d)", region[i].x, region[i].y);
									if (i != region.size() - 1) { printf(","); }
								}
							}
							image->at<cv::Vec3b>(cv::Point(region[i].x, region[i].y)) = set_color; // change to xxx
						}
						if (debug) printf("\n");
					}
					else {
						cv::Vec3b set_color;
						set_color[0] = 0;
						set_color[1] = 255;
						set_color[2] = 0;
						for (size_t i = 0; i < region.size(); i++) { image->at<cv::Vec3b>(cv::Point(region[i].x, region[i].y)) = set_color; }
						num_regions--;
					}
					while (region.size()) { region.pop_back(); }
				}
			}
		}

		DrawWindow DW = DrawWindow(cols, rows, "Done grid"); // w,h
		DW.clearWindow(255, 255, 255);
		for (int i = 0; i < cols; i++) {
			for (int j = 0; j < rows; j++) {
				int d = std::min(2 * done[i*rows + j], 255);
				DW.setPenColor(d, d, d);
				if (done[i*rows + j] < 0) { DW.setPenColor(0, 0, 255); }
				else if (done[i*rows + j] < min_region_pixels) { DW.setPenColor(255, 0, 0); }
				else if (done[i*rows + j] < 2 * min_region_pixels) { DW.setPenColor(255, 255, 0); }
				else if (done[i*rows + j] < 5 * min_region_pixels) { DW.setPenColor(0, 255, 0); }
				DW.drawPixel(i, j);
			}
		}
		DW.show();

		// move a small region's pixels into a larger neighbor
		//void moveSmallRegionsIntoNeighbors() {
		if (0) {
			int num_moved = 1;
			int max_loops = 10;
			int total_candidate_pixels = 0;
			while (num_moved > 0 && max_loops > 0) {
				num_moved = 0;
				max_loops--;
				for (int i = 0; i < cols; i++) {
					for (int j = 0; j < rows; j++) {
						int region_id = done[i*rows + j];
						int size = regions[region_id].size();
						if (size < min_region_pixels) {
							total_candidate_pixels++;
							int left = std::max(i - 1, 0);
							int right = std::min(i + 1, image->cols - 1);
							int top = std::max(j - 1, 0);
							int bottom = std::min(j + 1, image->rows - 1);
							int largest_pixels = 9999990;
							int largest_region_id = -1;
							for (int n = left; n <= right; n++) {
								for (int m = top; m <= bottom; m++) {
									if (n != i || m != j) {
										int size = regions[done[n*rows + m]].size();
										if (size >= min_region_pixels && size < largest_pixels) {
											largest_pixels = size;
											largest_region_id = done[n*rows + m];
										}
									}
								}
							}
							if (largest_region_id >= 0) { // move this pixel into this region
								num_moved++;
								printf("%i: moved %i,%i from %i(%i pxs) to %i(%i pxs) region\n", total_candidate_pixels, i, j, done[i*rows + j], size, largest_region_id, largest_pixels);
								regions[largest_region_id].push_back(cv::Point(i, j));
								done[i*rows + j] = largest_region_id;

								cv::Vec3b set_color;
								set_color[0] = 0;
								set_color[1] = 0;
								set_color[2] = 255;
								image->at<cv::Vec3b>(j, i) = set_color;

								// remove the point from the original tiny region
								//printf("BEFORE %lu\n",regions[region_id].size());
								//int remove_index=-1;
								for (size_t p = 0; p <= regions[region_id].size(); p++) {
									//printf("x=%i y=%i\n",regions[region_id][p].x,regions[region_id][p].y);
									//if (regions[region_id][p].x==i && regions[region_id][p].y==j) { remove_index=p; }
								}
								//printf("removing %d\n",remove_index);
								//regions[region_id].erase(regions[region_id].begin()+remove_index);
								//continue; //p=regions[region_id].size()+1; // get out of loop so can only remove 1 point from list
								//regions[region_id].erase(std::remove(regions[region_id].begin(), regions[region_id].end(), cv::Point(i,j)), regions[region_id].end());
								//printf("AFTER %lu\n",regions[region_id].size());
							}
						}
					}
				}
			}
			printf("Moved %d pixels to larger regions (loop #%d)\n", num_moved, 10 - max_loops);
		}
		if (debug) printf("Defined %d regions\n", num_regions);
		delete[] done;
	}

	void draw() {
		cv::RNG rng(12345);
		DrawWindow W = DrawWindow(kmeans_image.cols, kmeans_image.rows, "Regions"); // w,h
		W.clearWindow(0, 255, 0);
		for (size_t r = 0; r < regions.size(); r++) {
			int num_pixels = regions[r].size();
			if (num_pixels >= min_region_pixels) {
				W.setPenColor(region_colors[r][2], region_colors[r][1], region_colors[r][0]);
				if (use_random_colors) { W.setPenColor(rng.uniform(100, 200), rng.uniform(100, 200), rng.uniform(100, 200)); }
				if (debug) printf("%lu region: %lu pixels %i,%i,%i\n", r, regions[r].size(), region_colors[r][2], region_colors[r][1], region_colors[r][0]);
				W.drawRegion(regions[r]);
			}
		}
		W.show();
		//while (cv::waitKey(33)<0) { }
	}

	void defineShapes(Shapes *S, int skip_freq = 1) {
		cv::RNG rng(12345);
		for (size_t r = 0; r < regions.size(); r++) {
			int num_pixels = regions[r].size();
			if (num_pixels >= min_region_pixels) {
				PixelRegion *PR = new PixelRegion();
				PR->setPenColor(region_colors[r][0], region_colors[r][1], region_colors[r][2]);
				if (use_random_colors) { PR->setPenColor(rng.uniform(100, 200), rng.uniform(100, 200), rng.uniform(100, 200)); }

				for (size_t p = 0; p < regions[r].size(); p++) {
					PR->addPoint(regions[r][p].x, regions[r][p].y);
				}
				S->addShape(PR);
			}
		}
	}

	ImageParserKmeans() : ImageParser() { colors = 10; min_region_pixels = 10; reduce_specs_loops = 10; }
};

