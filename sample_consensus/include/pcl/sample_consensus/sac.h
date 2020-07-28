/*
 * Software License Agreement (BSD License)
 *
 *  Point Cloud Library (PCL) - www.pointclouds.org
 *  Copyright (c) 2010-2011, Willow Garage, Inc.
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

#pragma once

#include <pcl/pcl_config.h> // for PCL_NO_PRECOMPILE
#include <pcl/sample_consensus/boost.h>
#include <pcl/sample_consensus/sac_model.h>
#include <pcl/pcl_base.h>

#include <ctime>
#include <memory>
#include <set>

namespace pcl
{
  /** \brief SampleConsensus represents the base class. All sample consensus methods must inherit from this class.
    * \author Radu Bogdan Rusu
    * \ingroup sample_consensus
    */
  template <typename T>
  class SampleConsensus
  {
    using SampleConsensusModelPtr = typename SampleConsensusModel<T>::Ptr;

    private:
      /** \brief Constructor for base SAC. */
      SampleConsensus () {};

    public:
      using Ptr = shared_ptr<SampleConsensus<T> >;
      using ConstPtr = shared_ptr<const SampleConsensus<T> >;


      /** \brief Constructor for base SAC.
        * \param[in] model a Sample Consensus model
        * \param[in] random if true set the random seed to the current time, else set to 12345 (default: false)
        */
      SampleConsensus (const SampleConsensusModelPtr &model, bool random = false) 
        : sac_model_ (model)
        , probability_ (0.99)
        , iterations_ (0)
        , threshold_ (std::numeric_limits<double>::max ())
        , max_iterations_ (1000)
        , threads_ (-1)
        , rng_ (new boost::uniform_01<boost::mt19937> (rng_alg_))
      {
         // Create a random number generator object
         if (random)
           rng_->base ().seed (static_cast<unsigned> (std::time (nullptr)));
         else
           rng_->base ().seed (12345u);
      };

      /** \brief Constructor for base SAC.
        * \param[in] model a Sample Consensus model
        * \param[in] threshold distance to model threshold
        * \param[in] random if true set the random seed to the current time, else set to 12345 (default: false)
        */
      SampleConsensus (const SampleConsensusModelPtr &model, 
                       double threshold, 
                       bool random = false)
        : sac_model_ (model)
        , probability_ (0.99)
        , iterations_ (0)
        , threshold_ (threshold)
        , max_iterations_ (1000)
        , threads_ (-1)
        , rng_ (new boost::uniform_01<boost::mt19937> (rng_alg_))
      {
         // Create a random number generator object
         if (random)
           rng_->base ().seed (static_cast<unsigned> (std::time (nullptr)));
         else
           rng_->base ().seed (12345u);
      };

      /** \brief Set the Sample Consensus model to use.
        * \param[in] model a Sample Consensus model
        */
      void
      setSampleConsensusModel (const SampleConsensusModelPtr &model)
      {
        sac_model_ = model;
      }

      /** \brief Get the Sample Consensus model used. */
      SampleConsensusModelPtr
      getSampleConsensusModel () const
      {
        return (sac_model_);
      }

      /** \brief Destructor for base SAC. */
      virtual ~SampleConsensus () {};

      /** \brief Set the distance to model threshold.
        * \param[in] threshold distance to model threshold
        */
      inline void 
      setDistanceThreshold (double threshold)  { threshold_ = threshold; }

      /** \brief Get the distance to model threshold, as set by the user. */
      inline double 
      getDistanceThreshold () const { return (threshold_); }

      /** \brief Set the maximum number of iterations.
        * \param[in] max_iterations maximum number of iterations
        */
      inline void 
      setMaxIterations (int max_iterations) { max_iterations_ = max_iterations; }

      /** \brief Get the maximum number of iterations, as set by the user. */
      inline int 
      getMaxIterations () const { return (max_iterations_); }

      /** \brief Set the desired probability of choosing at least one sample free from outliers.
        * \param[in] probability the desired probability of choosing at least one sample free from outliers
        * \note internally, the probability is set to 99% (0.99) by default.
        */
      inline void 
      setProbability (double probability) { probability_ = probability; }

      /** \brief Obtain the probability of choosing at least one sample free from outliers, as set by the user. */
      inline double 
      getProbability () const { return (probability_); }

      /** \brief Set the number of threads to use or turn off parallelization.
        * \param[in] nr_threads the number of hardware threads to use (0 sets the value automatically, a negative number turns parallelization off)
        * \note Not all SAC methods have a parallel implementation. Some will ignore this setting.
        */
      inline void
      setNumberOfThreads (const int nr_threads = -1) { threads_ = nr_threads; }

      /** \brief Get the number of threads, as set by the user. */
      inline int
      getNumberOfThreads () const { return (threads_); }

      /** \brief Compute the actual model. Pure virtual. */
      virtual bool 
      computeModel (int debug_verbosity_level = 0) = 0;

      /** \brief Refine the model found.
        * This loops over the model coefficients and optimizes them together
        * with the set of inliers, until the change in the set of inliers is
        * minimal.
        * \param[in] sigma standard deviation multiplier for considering a sample as inlier (Mahalanobis distance) 
        * \param[in] max_iterations the maxim number of iterations to try to refine in case the inliers keep on changing
        */
      virtual bool 
      refineModel (const double sigma = 3.0, const unsigned int max_iterations = 1000);

      /** \brief Get a set of randomly selected indices.
        * \param[in] indices the input indices vector
        * \param[in] nr_samples the desired number of point indices to randomly select
        * \param[out] indices_subset the resultant output set of randomly selected indices
        */
      inline void
      getRandomSamples (const IndicesPtr &indices,
                        std::size_t nr_samples, 
                        std::set<index_t> &indices_subset)
      {
        indices_subset.clear ();
        while (indices_subset.size () < nr_samples)
          //indices_subset.insert ((*indices)[(index_t) (indices->size () * (rand () / (RAND_MAX + 1.0)))]);
          indices_subset.insert ((*indices)[static_cast<index_t> (static_cast<double>(indices->size ()) * rnd ())]);
      }

      /** \brief Return the best model found so far. 
        * \param[out] model the resultant model
        */
      inline void 
      getModel (Indices &model) const { model = model_; }

      /** \brief Return the best set of inliers found so far for this model. 
        * \param[out] inliers the resultant set of inliers
        */
      inline void 
      getInliers (Indices &inliers) const { inliers = inliers_; }

      /** \brief Return the model coefficients of the best model found so far. 
        * \param[out] model_coefficients the resultant model coefficients, as documented in \ref sample_consensus
        */
      inline void 
      getModelCoefficients (Eigen::VectorXf &model_coefficients) const { model_coefficients = model_coefficients_; }

    protected:
      /** \brief The underlying data model used (i.e. what is it that we attempt to search for). */
      SampleConsensusModelPtr sac_model_;

      /** \brief The model found after the last computeModel () as point cloud indices. */
      Indices model_;

      /** \brief The indices of the points that were chosen as inliers after the last computeModel () call. */
      Indices inliers_;

      /** \brief The coefficients of our model computed directly from the model found. */
      Eigen::VectorXf model_coefficients_;

      /** \brief Desired probability of choosing at least one sample free from outliers. */
      double probability_;

      /** \brief Total number of internal loop iterations that we've done so far. */
      int iterations_;
      
      /** \brief Distance to model threshold. */
      double threshold_;
      
      /** \brief Maximum number of iterations before giving up. */
      int max_iterations_;

      /** \brief The number of threads the scheduler should use, or a negative number if no parallelization is wanted. */
      int threads_;

      /** \brief Boost-based random number generator algorithm. */
      boost::mt19937 rng_alg_;

      /** \brief Boost-based random number generator distribution. */
      std::shared_ptr<boost::uniform_01<boost::mt19937> > rng_;

      /** \brief Boost-based random number generator. */
      inline double
      rnd ()
      {
        return ((*rng_) ());
      }
   };
}

#ifdef PCL_NO_PRECOMPILE
#include <pcl/sample_consensus/impl/sac.hpp>
#endif
