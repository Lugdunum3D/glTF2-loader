#include "glTF2.hpp"
#include <gtest/gtest.h>

using namespace ::testing;

TEST(Main, Test0) {
    gltf2::Asset asset = gltf2::load("./models/TriangleWithoutIndices.gltf");

    // Asset
    ASSERT_EQ(asset.metadata.version, "2.0");
    ASSERT_EQ(asset.metadata.generator, "Manually converted to 2.0");


    // Scenes
    ASSERT_EQ(asset.scene, 0);
    ASSERT_EQ(asset.scenes.size(), 1);

    // Scene 0
    ASSERT_EQ(asset.scenes[0].name, "scene0");

    ASSERT_EQ(asset.scenes[0].nodes.size(), 1);
    ASSERT_EQ(asset.scenes[0].nodes[0], 0);


    // Nodes
    ASSERT_EQ(asset.nodes.size(), 1);

    // Node 0
    ASSERT_EQ(asset.nodes[0].name, "node0");

    ASSERT_EQ(asset.nodes[0].camera, -1);
    ASSERT_EQ(asset.nodes[0].children.size(), 0);
    ASSERT_EQ(asset.nodes[0].mesh, 0);
    ASSERT_EQ(asset.nodes[0].skin, -1);


    // Meshes
    ASSERT_EQ(asset.meshes.size(), 1);

    // Mesh 0
    ASSERT_EQ(asset.meshes[0].name, "mesh0");
    ASSERT_EQ(asset.meshes[0].primitives.size(), 1);

    // Primitive 0
    ASSERT_EQ(asset.meshes[0].primitives[0].mode, gltf2::Primitive::Mode::Triangles);
    ASSERT_EQ(asset.meshes[0].primitives[0].indices, -1);
    ASSERT_EQ(asset.meshes[0].primitives[0].material, -1);

    // Primitive 0 attributes
    ASSERT_EQ(asset.meshes[0].primitives[0].attributes.size(), 1);
    ASSERT_EQ(asset.meshes[0].primitives[0].attributes.count("POSITION"), 1);
    ASSERT_EQ(asset.meshes[0].primitives[0].attributes["POSITION"], 0);


    // Accessors
    ASSERT_EQ(asset.accessors.size(), 1);

    // Accessor 0
    ASSERT_EQ(asset.accessors[0].name, "");
    ASSERT_EQ(asset.accessors[0].bufferView, 0);
    ASSERT_EQ(asset.accessors[0].byteOffset, 0);
    ASSERT_EQ(asset.accessors[0].componentType, gltf2::Accessor::ComponentType::Float);
    ASSERT_EQ(asset.accessors[0].count, 3);
    ASSERT_EQ(asset.accessors[0].type, gltf2::Accessor::Type::Vec3);


    // BufferViews
    ASSERT_EQ(asset.bufferViews.size(), 1);

    // BufferView 0
    ASSERT_EQ(asset.bufferViews[0].name, "");
    ASSERT_EQ(asset.bufferViews[0].buffer, 0);
    ASSERT_EQ(asset.bufferViews[0].byteOffset, 0);
    ASSERT_EQ(asset.bufferViews[0].byteLength, 36);
    ASSERT_EQ(asset.bufferViews[0].target, gltf2::BufferView::TargetType::ArrayBuffer);


    // Buffer
    ASSERT_EQ(asset.buffers.size(), 1);

    // Buffer 0
    ASSERT_EQ(asset.buffers[0].byteLength, 36);
    ASSERT_EQ(asset.buffers[0].uri, "triangleWithoutIndices.bin");
    ASSERT_NE(asset.buffers[0].data, nullptr);

    uint8_t data[36] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f,
        0x00, 0x00, 0x00, 0x00
    };
    for (uint32_t i = 0; i < asset.buffers[0].byteLength; ++i) {
        ASSERT_EQ(asset.buffers[0].data[i], static_cast<char>(data[i]));
    }
}
