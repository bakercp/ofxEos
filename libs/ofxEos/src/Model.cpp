//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "ofx/Eos/Model.h"
#include "ofx/Eos/Utils.h"


//#include "eos/core/Image.hpp"
//#include "eos/core/Image_opencv_interop.hpp"
//#include "eos/core/Landmark.hpp"


//#include "eos/core/read_pts_landmarks.hpp"

//#include "eos/render/draw_utils.hpp"
//#include "eos/render/texture_extraction.hpp"
//#include "eos/cpp17/optional.hpp"


//#include "boost/filesystem.hpp"
//#include "boost/program_options.hpp"
//
//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"


namespace ofx {
namespace Eos {


Model::Model()
{
}

    
Model::~Model()
{
}
    
    
Model Model::fromFiles(const std::string& morphableModelPath,
                       const std::string& landmarkMapperPath,
                       const std::string& blendShapesPath,
                       const std::string& modelContourPath,
                       const std::string& modelContourLandmarksPath,
                       const std::string& edgeTopologyPath)
{
    Model model;
    model._morphableModel = loadModel(morphableModelPath);
    model._landmarkMapper = loadLandmarkMapper(landmarkMapperPath);
    model._blendShapes = loadBlendshapes(blendShapesPath);
    model._modelContour = loadModelContour(modelContourPath);
    model._modelContourLandmarks = loadContourLandmarks(modelContourLandmarksPath);
    model._edgeTopology = loadEdgeTopology(edgeTopologyPath);

    // TODO: we are assuming the iBug / dlib 68 point mapping.
    for (std::size_t i = 0; i < 68; ++i)
    {
        model._landmarks.push_back(eos::core::Landmark<Eigen::Vector2f>());
        model._landmarks.back().name = std::to_string(i + 1);
        model._landmarks.back().coordinates[0] = 0;
        model._landmarks.back().coordinates[1] = 0;
    }

    return model;
}


} } // namespace ofx::Eos
