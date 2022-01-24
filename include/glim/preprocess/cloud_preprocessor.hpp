#pragma once

#include <random>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include <glim/preprocess/preprocessed_frame.hpp>

namespace glim {

struct CloudPreprocessorParams {
public:
  CloudPreprocessorParams();
  ~CloudPreprocessorParams();

public:
  Eigen::Isometry3d T_lidar_offset;

  bool use_random_grid_downsampling;
  double distance_near_thresh;
  double distance_far_thresh;
  double downsample_resolution;
  double downsample_rate;
  int k_correspondences;
};

/**
 * @brief Point cloud preprocessor
 *
 */
class CloudPreprocessor {
public:
  using Points = std::vector<Eigen::Vector4d, Eigen::aligned_allocator<Eigen::Vector4d>>;
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  /**
   * @brief Construct a new Cloud Preprocessor object
   */
  CloudPreprocessor(const CloudPreprocessorParams& params = CloudPreprocessorParams());

  /**
   * @brief Destroy the Cloud Preprocessor object
   */
  virtual ~CloudPreprocessor();

  /**
   * @brief Preprocess a raw point cloud
   *
   * @param stamp     Timestamp
   * @param times     Timestamps of input points (w.r.t. the first point)
   * @param points    Points (homogeneous coordinates)
   * @return PreprocessedFrame::Ptr  Preprocessed point cloud
   */
  virtual PreprocessedFrame::Ptr preprocess(double stamp, const std::vector<double>& times, const Points& points) const;

private:
  PreprocessedFrame::Ptr sort_by_time(const std::vector<double>& times, const Points& points) const;
  PreprocessedFrame::Ptr distance_filter(const std::vector<double>& times, const Points& points) const;
  std::vector<int> find_neighbors(const Points& points, int k) const;

private:
  using Params = CloudPreprocessorParams;
  Params params;

  mutable std::mt19937 mt;
};

}  // namespace glim