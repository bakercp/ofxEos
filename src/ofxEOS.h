//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofxCv.h"
#include "ofxDlib.h"
#include "ofxEigen.h"


// N.B. We don't need to include all of these, but it's a quick way to make sure
// everything from the original eos library compiles. Comment some to speed
// compilation.
#include "eos/core/Image.hpp"
#include "eos/core/Image_opencv_interop.hpp"
#include "eos/core/Landmark.hpp"
#include "eos/core/LandmarkMapper.hpp"
#include "eos/core/Mesh.hpp"
#include "eos/core/read_obj.hpp"
#include "eos/core/read_pts_landmarks.hpp"
#include "eos/pca/pca.hpp"
#include "eos/video/Keyframe.hpp"
#include "eos/video/keyframe_merging.hpp"

#include "eos/fitting/affine_camera_estimation.hpp"
#include "eos/fitting/blendshape_fitting.hpp"
//#include "eos/fitting/ceres_nonlinear.hpp"
#include "eos/fitting/closest_edge_fitting.hpp"
#include "eos/fitting/contour_correspondence.hpp"
#include "eos/fitting/fitting.hpp"
#include "eos/fitting/FittingResult.hpp"
#include "eos/fitting/linear_shape_fitting.hpp"
#include "eos/fitting/multi_image_fitting.hpp"
#include "eos/fitting/nonlinear_camera_estimation.hpp"
#include "eos/fitting/orthographic_camera_estimation_linear.hpp"
#include "eos/fitting/RenderingParameters.hpp"
#include "eos/fitting/detail/glm_cerealisation.hpp"
#include "eos/fitting/detail/nonlinear_camera_estimation_detail.hpp"
#include "eos/fitting/detail/optional_cerealisation.hpp"

#include "eos/morphablemodel/Blendshape.hpp"
#include "eos/morphablemodel/MorphableModel.hpp"
#include "eos/morphablemodel/PcaModel.hpp"
#include "eos/morphablemodel/EdgeTopology.hpp"
#include "eos/morphablemodel/coefficients.hpp"
#include "eos/morphablemodel/io/eigen_cerealisation.hpp"
#include "eos/morphablemodel/io/cvssp.hpp"
#include "eos/morphablemodel/io/mat_cerealisation.hpp"

#include "eos/render/draw_utils.hpp"
#include "eos/render/FragmentShader.hpp"
#include "eos/render/Rasterizer.hpp"
#include "eos/render/Rect.hpp"
#include "eos/render/render_affine.hpp"
#include "eos/render/render.hpp"
#include "eos/render/SoftwareRenderer.hpp"
#include "eos/render/texture_extraction.hpp"
#include "eos/render/Texture.hpp"
#include "eos/render/utils.hpp"
#include "eos/render/VertexShader.hpp"
#include "eos/render/detail/render_affine_detail.hpp"
#include "eos/render/detail/render_detail_utils.hpp"
#include "eos/render/detail/render_detail.hpp"
#include "eos/render/detail/texture_extraction_detail.hpp"
#include "eos/render/detail/texturing.hpp"
#include "eos/render/detail/TriangleToRasterize.hpp"
#include "eos/render/detail/Vertex.hpp"

#include "eos/cpp17/optional_serialization.hpp"
#include "eos/cpp17/optional.hpp"
#include "eos/cpp17/detail/akrzemi1_optional_serialization.hpp"
#include "eos/cpp17/detail/akrzemi1_optional.hpp"


