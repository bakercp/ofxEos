//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include "ofMesh.h"
#include <Eigen/Dense>
#include "eos/core/Landmark.hpp"
#include "eos/core/LandmarkMapper.hpp"
#include "eos/core/Mesh.hpp"
#include "eos/core/read_pts_landmarks.hpp"
#include "eos/fitting/fitting.hpp"
#include "eos/morphablemodel/MorphableModel.hpp"


namespace ofx {
namespace Eos {
    
    
inline eos::core::LandmarkCollection<::Eigen::Vector2f> loadLandmarks(const std::string& path)
{
    eos::core::LandmarkCollection<::Eigen::Vector2f> landmarks;

    try
    {
        landmarks = eos::core::read_pts_landmarks(ofToDataPath(path, true));
    }
    catch (const std::runtime_error& e)
    {
        ofLogError("loadLandmarks") << "Error reading the landmarks: " << e.what();
    }
    
    return landmarks;
}

    
inline eos::morphablemodel::MorphableModel loadModel(const std::string& path)
{
    eos::morphablemodel::MorphableModel morphable_model;
    
    try
    {
        morphable_model = eos::morphablemodel::load_model(ofToDataPath(path, true));
    }
    catch (const std::runtime_error& e)
    {
        ofLogError("loadModel") << "Error loading the Morphable Model: " << e.what();
    }

    return morphable_model;
}

    
inline eos::core::LandmarkMapper loadLandmarkMapper(const std::string& path)
{
    eos::core::LandmarkMapper landmark_mapper;
    
    try
    {
        landmark_mapper = eos::core::LandmarkMapper(ofToDataPath(path, true));
    }
    catch (const std::exception& e)
    {
        ofLogError("loadLandmarkMapper") << "Error loading the landmark mappings: " << e.what();
    }
    
    return landmark_mapper;
}

inline std::vector<eos::morphablemodel::Blendshape> loadBlendshapes(const std::string& path)
{
    std::vector<eos::morphablemodel::Blendshape> blendshapes;
    
    try
    {
        blendshapes = eos::morphablemodel::load_blendshapes(ofToDataPath(path, true));
    }
    catch (const std::exception& e)
    {
        ofLogError("loadBlendshapes") << "Error loading the blendshapes: " << e.what();
    }
    
    return blendshapes;
}

inline eos::fitting::ModelContour loadModelContour(const std::string& path)
{
    eos::fitting::ModelContour modelContour;
    
    try
    {
        modelContour = eos::fitting::ModelContour::load(ofToDataPath(path, true));
    }
    catch (const std::exception& e)
    {
        ofLogError("loadModelContour") << "Error loading the model contour: " << e.what();
    }
    
    return modelContour;
}

    
    
inline eos::fitting::ContourLandmarks loadContourLandmarks(const std::string& path)
{
    eos::fitting::ContourLandmarks contourLandmarks;
    
    try
    {
        contourLandmarks = eos::fitting::ContourLandmarks::load(ofToDataPath(path, true));
    }
    catch (const std::exception& e)
    {
        ofLogError("loadContourLandmarks") << "Error loading the model contour: " << e.what();
    }
    
    return contourLandmarks;
}

    
    
inline eos::morphablemodel::EdgeTopology loadEdgeTopology(const std::string& path)
{
    eos::morphablemodel::EdgeTopology edge_topology;

    try
    {
        edge_topology = eos::morphablemodel::load_edge_topology(ofToDataPath(path, true));
    }
    catch (const std::exception& e)
    {
        ofLogError("loadEdgeTopology") << "Error loading the edge topology: " << e.what();
    }

    return edge_topology;
}
    
    
    

    

//    struct Mesh
//    {
//        std::vector<Eigen::Vector3f> vertices;  ///< 3D vertex positions.
//        std::vector<Eigen::Vector3f> colors;    ///< Colour information for each vertex. Expected to be in RGB order.
//        std::vector<Eigen::Vector2f> texcoords; ///< Texture coordinates for each vertex.
//
//        std::vector<std::array<int, 3>> tvi;    ///< Triangle vertex indices
//        std::vector<std::array<int, 3>> tci;    ///< Triangle color indices
//    };

    
    
    
    
inline ofMesh toOf(const eos::core::Mesh& _mesh)
{
    ofMesh mesh;
    
    for (const auto& v: _mesh.vertices)
        mesh.addVertex({ v[0], v[1], v[2] });
    
//    for (const auto& c: _mesh.colors)
//        mesh.addColor(ofFloatColor(c[0], c[1], c[2]));
//
//    for (const auto& t: _mesh.texcoords)
//        mesh.addTexCoord({ t[0], t[1] });
//
//    for (const auto& i: _mesh.tvi)
//        for (const auto& j: i)
//            mesh.addIndex(j);
    
//    eos_mesh.
    
    
    
    
    
    return mesh;
    
};
    
    
    
} } // namespace ofx::Eos
