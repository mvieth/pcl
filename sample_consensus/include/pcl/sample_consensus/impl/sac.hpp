/*
 * Software License Agreement (BSD License)
 *
 *  Point Cloud Library (PCL) - www.pointclouds.org
 *  Copyright (c) 2009, Willow Garage, Inc.
 *  Copyright (c) 2012-, Open Perception, Inc.
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the copyright holder(s) nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id$
 *
 */

#ifndef PCL_SAMPLE_CONSENSUS_IMPL_SAC_H_
#define PCL_SAMPLE_CONSENSUS_IMPL_SAC_H_

#include <pcl/sample_consensus/sac.h>

template <typename PointT> bool
pcl::SampleConsensus<PointT>::refineModel (const double sigma, const unsigned int max_iterations)
{
  if (!sac_model_)
  {
    PCL_ERROR ("[pcl::SampleConsensus::refineModel] Critical error: NULL model!\n");
    return (false);
  }

  double inlier_distance_threshold_sqr = threshold_ * threshold_,
         error_threshold = threshold_;
  double sigma_sqr = sigma * sigma;
  unsigned int refine_iterations = 0;
  bool inlier_changed = false, oscillating = false;
  Indices new_inliers, prev_inliers = inliers_;
  std::vector<std::size_t> inliers_sizes;
  Eigen::VectorXf new_model_coefficients = model_coefficients_;
  do
  {
    // Optimize the model coefficients
    sac_model_->optimizeModelCoefficients (prev_inliers, new_model_coefficients, new_model_coefficients);
    inliers_sizes.push_back (prev_inliers.size ());

    // Select the new inliers based on the optimized coefficients and new threshold
    sac_model_->selectWithinDistance (new_model_coefficients, error_threshold, new_inliers);
    PCL_DEBUG ("[pcl::SampleConsensus::refineModel] Number of inliers found (before/after): %lu/%lu, with an error threshold of %g.\n", prev_inliers.size (), new_inliers.size (), error_threshold);

    if (new_inliers.empty ())
    {
      refine_iterations++;
      if (refine_iterations >= max_iterations)
        break;
      continue;
      //return (false);
    }

    // Estimate the variance and the new threshold
    double variance = sac_model_->computeVariance ();
    error_threshold = sqrt (std::min (inlier_distance_threshold_sqr, sigma_sqr * variance));

    PCL_DEBUG ("[pcl::SampleConsensus::refineModel] New estimated error threshold: %g on iteration %d out of %d.\n", error_threshold, refine_iterations, max_iterations);
    inlier_changed = false;
    std::swap (prev_inliers, new_inliers);
    // If the number of inliers changed, then we are still optimizing
    if (new_inliers.size () != prev_inliers.size ())
    {
      // Check if the number of inliers is oscillating in between two values
      if (inliers_sizes.size () >= 4)
      {
        if (inliers_sizes[inliers_sizes.size () - 1] == inliers_sizes[inliers_sizes.size () - 3] &&
            inliers_sizes[inliers_sizes.size () - 2] == inliers_sizes[inliers_sizes.size () - 4])
        {
          oscillating = true;
          break;
        }
      }
      inlier_changed = true;
      continue;
    }

    // Check the values of the inlier set
    for (std::size_t i = 0; i < prev_inliers.size (); ++i)
    {
      // If the value of the inliers changed, then we are still optimizing
      if (prev_inliers[i] != new_inliers[i])
      {
        inlier_changed = true;
        break;
      }
    }
  }
  while (inlier_changed && ++refine_iterations < max_iterations);

  // If the new set of inliers is empty, we didn't do a good job refining
  if (new_inliers.empty ())
  {
    PCL_ERROR ("[pcl::SampleConsensus::refineModel] Refinement failed: got an empty set of inliers!\n");
    return (false);
  }

  if (oscillating)
  {
    PCL_DEBUG ("[pcl::SampleConsensus::refineModel] Detected oscillations in the model refinement.\n");
    return (true);
  }

  // If no inliers have been changed anymore, then the refinement was successful
  if (!inlier_changed)
  {
    std::swap (inliers_, new_inliers);
    model_coefficients_ = new_model_coefficients;
    return (true);
  }
  return (false);
}

#define PCL_INSTANTIATE_refineModel(T) template bool pcl::SampleConsensus<T>::refineModel (const double sigma, const unsigned int max_iterations);

#endif // PCL_SAMPLE_CONSENSUS_IMPL_SAC_H_
