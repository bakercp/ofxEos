//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include "ofMesh.h"
#include "eos/core/Mesh.hpp"


namespace ofx {
namespace Eos {

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
