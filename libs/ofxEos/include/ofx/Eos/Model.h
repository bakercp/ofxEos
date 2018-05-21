//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include "ofMesh.h"
#include "Eigen/Dense"
#include "ofLog.h"
#include "ofVectorMath.h"
#include "eos/core/LandmarkMapper.hpp"
#include "eos/fitting/fitting.hpp"
#include "eos/morphablemodel/Blendshape.hpp"
#include "eos/morphablemodel/MorphableModel.hpp"
//#include "Eigen/Core"


namespace ofx {
namespace Eos {


    
class Model
{
public:
    Model();
    ~Model();
    
    void setup(std::size_t width, std::size_t height)
    {
        _videoWidth = width;
        _videoHeight = height;
    }
    
    template<typename T>
    void update(const std::vector<T>& landmarkPoints)
    {
        if (landmarkPoints.size() != _landmarks.size())
        {
            ofLogError("Model::update") << "Inconsistent number of landmark points. Given: " << landmarkPoints.size() << ", expected: " << _landmarks.size();
            return;
        }
        
        for (std::size_t i = 0; i < landmarkPoints.size(); ++i)
        {
            _landmarks[i].coordinates[0] = landmarkPoints[i][0];
            _landmarks[i].coordinates[1] = landmarkPoints[i][1];
        }
        
        std::tie(_mesh, _rendering_params) = eos::fitting::fit_shape_and_pose(_morphableModel,
                                                                            _blendShapes,
                                                                            _landmarks,
                                                                            _landmarkMapper,
                                                                            _videoWidth,
                                                                            _videoHeight,
                                                                            _edgeTopology,
                                                                            _modelContourLandmarks,
                                                                            _modelContour,
                                                                            5,
                                                                            eos::cpp17::nullopt,
                                                                            30.0f);

        
        
        
        
        _ofmesh.clear();
        _ofmesh.setMode(OF_PRIMITIVE_TRIANGLES);

        
        const auto& viewPort = eos::fitting::get_opencv_viewport(_videoWidth,
                                                                 _videoHeight);

        const auto& modelView = _rendering_params.get_modelview();
        const auto& projection = _rendering_params.get_projection();
        
        for (std::size_t i = 0; i < _mesh.vertices.size(); ++i)
        {
            glm::vec3 modelVertex = {
                _mesh.vertices[i][0],
                _mesh.vertices[i][1],
                _mesh.vertices[i][2]
            };
            
            auto projected = glm::project(modelVertex, modelView, projection, viewPort);
            
           // std::cout << projected << std::endl;
            
            _ofmesh.addVertex(modelVertex);
            
            _ofmesh.addTexCoord(projected);
            
        }
        
        for (std::size_t i = 0; i < _mesh.tvi.size(); ++i)
        {
            _ofmesh.addIndex(_mesh.tvi[i][0]);
            _ofmesh.addIndex(_mesh.tvi[i][1]);
            _ofmesh.addIndex(_mesh.tvi[i][2]);
        }

        
    }
    
    ofMesh& mesh() { return _ofmesh; }
    void draw()
    {
        _ofmesh.draw();
    }
    
    static Model fromFiles(const std::string& morphableModelPath = "models/sfm_3448/sfm_shape_3448.bin",
                           const std::string& landmarkMapperPath = "models/sfm_3448/ibug_to_sfm.txt",
                           const std::string& blendShapesPath = "models/sfm_3448/expression_blendshapes_3448.bin",
                           const std::string& modelContourPath = "models/sfm_3448/sfm_model_contours.json",
                           const std::string& modelContourLandmarksPath = "models/sfm_3448/ibug_to_sfm.txt",
                           const std::string& edgeTopologyPath = "models/sfm_3448/sfm_3448_edge_topology.json");
private:
    std::size_t _videoWidth = 1280;
    std::size_t _videoHeight = 720;
    ofMesh _ofmesh;
    
    eos::core::Mesh _mesh;
    eos::fitting::RenderingParameters _rendering_params;

    
    /// \brief The morphable model.
    eos::morphablemodel::MorphableModel _morphableModel;
    
    /// \brief The mapping from 2D landmark points to the model vertex ids.
    eos::core::LandmarkMapper _landmarkMapper;
    
    /// \brief The model's expression blend shapes.
    std::vector<eos::morphablemodel::Blendshape> _blendShapes;
    
    /// \brief Model contour points.
    eos::fitting::ModelContour _modelContour;
    
    /// \brief Model contour landmarks.
    eos::fitting::ContourLandmarks _modelContourLandmarks;
    
    /// \brief The edge topology used for speeding occluded contour fitting.
    eos::morphablemodel::EdgeTopology _edgeTopology;
    
    /// \brief The landmark collection of the current fit.
    eos::core::LandmarkCollection<::Eigen::Vector2f> _landmarks;
};
    

} } // namespace ofx::Eos
