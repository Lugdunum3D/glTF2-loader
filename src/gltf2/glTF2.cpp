#include <iostream>
#include <fstream>
#include <string>
#include <ext/json.hpp>
#include <gltf2/glTF2.hpp>
#include <gltf2/Exceptions.hpp>

#include <fstream>

namespace gltf2 {

static void loadAsset(Asset& asset, nlohmann::json& json);
static void loadScenes(Asset& asset, nlohmann::json& json);
static void loadMeshes(Asset& asset, nlohmann::json& json);
static void loadNodes(Asset& asset, nlohmann::json& json);
static void loadBuffers(Asset& asset, nlohmann::json& json);
static void loadAccessors(Asset& asset, nlohmann::json& json);
static void loadBufferViews(Asset& asset, nlohmann::json& json);
static void loadBufferData(Asset& asset, Buffer& buffer);
static std::string pathAppend(const std::string& p1, const std::string& p2);
static void loadMaterials(Asset& asset, nlohmann::json& json);
static void loadTextureInfo(Material::Texture& texture, nlohmann::json& json);
static void loadImages(Asset& asset, nlohmann::json& json);

static void loadAsset(Asset& asset, nlohmann::json& json) {
    if (json.find("asset") == json.end()) {
        throw MisformattedExceptionIsRequired("asset");
    }

    // version
    if (json["asset"].find("version") == json["asset"].end()) {
        throw MisformattedExceptionIsRequired("asset[version]");
    }
    if (!json["asset"]["version"].is_string()) {
        throw MisformattedExceptionNotString("version");
    }

    asset.metadata.version = json["asset"]["version"].get<std::string>();

    // copyright
    if (json["asset"].find("copyright") != json["asset"].end()) {
        if (!json["asset"]["copyright"].is_string()) {
            throw MisformattedExceptionNotString("copyright");
        }

        asset.metadata.copyright = json["asset"]["copyright"].get<std::string>();
    }

    // generator
    if (json["asset"].find("generator") != json["asset"].end()) {
        if (!json["asset"]["generator"].is_string()) {
            throw MisformattedExceptionNotString("generator");
        }

        asset.metadata.generator = json["asset"]["generator"].get<std::string>();
    }

    // minVersion
    if (json["asset"].find("minVersion") != json["asset"].end()) {
        if (!json["asset"]["minVersion"].is_string()) {
            throw MisformattedExceptionNotString("minVersion");
        }

        asset.metadata.minVersion = json["asset"]["minVersion"].get<std::string>();
    }
}

static void loadScenes(Asset& asset, nlohmann::json& json) {
    if (json.find("scene") != json.end()) {
        if (!json["scene"].is_number()) {
            throw MisformattedExceptionNotNumber("scene");
        }

        asset.scene = json["scene"].get<int32_t>();
    }

    if (json.find("scenes") == json.end()) {
        return;
    }

    auto& scenes = json["scenes"];
    if (!scenes.is_array()) {
        throw MisformattedExceptionNotArray("scenes");
    }

    if (asset.scene == -1) {
        asset.scene = 0;
    }

    asset.scenes.resize(scenes.size());
    for (uint32_t i = 0; i < scenes.size(); ++i) {
        // name
        if (scenes[i].find("name") != scenes[i].end()) {
            if (!scenes[i]["name"].is_string()) {
                throw MisformattedExceptionNotString("scenes[i][name]");
            }

            asset.scenes[i].name = scenes[i]["name"];
        }

        // nodes
        if (scenes[i].find("nodes") != scenes[i].end()) {
            auto& nodes = scenes[i]["nodes"];
            if (!nodes.is_array()) {
                throw MisformattedExceptionNotArray("scenes[i][nodes]");
            }

            asset.scenes[i].nodes.resize(nodes.size());
            for (uint32_t j = 0; j < nodes.size(); ++j) {
                asset.scenes[i].nodes[j] = nodes[j].get<uint32_t>();
            }
        }
    }
}

static void loadMeshes(Asset& asset, nlohmann::json& json) {
    if (json.find("meshes") == json.end()) {
        return;
    }

    auto& meshes = json["meshes"];

    if (!meshes.is_array()) {
        throw MisformattedExceptionNotArray("meshes");
    }

    asset.meshes.resize(meshes.size());
    for (uint32_t i = 0; i < meshes.size(); ++i) {
        // name
        if (meshes[i].find("name") != meshes[i].end()) {
            if (!meshes[i]["name"].is_string()) {
                throw MisformattedExceptionNotString("meshes[i][name]");
            }

            asset.meshes[i].name = meshes[i]["name"];
        }


        if (meshes[i].find("primitives") == meshes[i].end()) {
            throw MisformattedExceptionIsRequired("meshes[i][primitives]");
        }

        auto& primitives = meshes[i]["primitives"];
        if (!primitives.is_array()) {
            throw MisformattedExceptionNotArray("meshes[i][primitives]");
        }

        asset.meshes[i].primitives.resize(primitives.size());
        for (uint32_t j = 0; j < primitives.size(); ++j) {
            // indices
            if (primitives[j].find("indices") != primitives[j].end()) {
                if (!primitives[j]["indices"].is_number()) {
                    throw MisformattedExceptionNotNumber("meshes[i][primitives][j][indices]");
                }

                asset.meshes[i].primitives[j].indices = primitives[j]["indices"].get<int32_t>();
            }

            // material
            if (primitives[j].find("material") != primitives[j].end()) {
                if (!primitives[j]["material"].is_number()) {
                    throw MisformattedExceptionNotNumber("meshes[i][primitives][j][material]");
                }

                asset.meshes[i].primitives[j].material = primitives[j]["material"].get<int32_t>();
            }

            // mode
            if (primitives[j].find("mode") != primitives[j].end()) {
                if (!primitives[j]["mode"].is_number()) {
                    throw MisformattedExceptionNotNumber("meshes[i][primitives][j][mode]");
                }

                asset.meshes[i].primitives[j].mode = static_cast<Primitive::Mode>(primitives[j]["mode"].get<uint8_t>());
            }

            if (primitives[j].find("attributes") == primitives[j].end()) {
                throw MisformattedExceptionIsRequired("meshes[i][primitives][j][attributes]");
            }

            auto& attributes = primitives[j]["attributes"];
            if (!attributes.is_object()) {
                throw MisformattedExceptionNotObject("meshes[i][primitives][j][attributes]");
            }

            for (nlohmann::json::iterator it = attributes.begin(); it != attributes.end(); ++it) {
                asset.meshes[i].primitives[j].attributes[it.key()] = it.value();
            }

            // TODO: primitives[j]["targets"]
        }

        // TODO: meshes[i]["weights"]
        // TODO: meshes[i]["extensions"]
    }
}

static void loadNodes(Asset& asset, nlohmann::json& json) {
    if (json.find("nodes") == json.end()) {
        return;
    }

    auto& nodes = json["nodes"];
    if (!nodes.is_array()) {
        throw MisformattedExceptionNotArray("nodes");
    }

    asset.nodes.resize(nodes.size());
    for (uint32_t i = 0; i < nodes.size(); ++i) {
        // name
        if (nodes[i].find("name") != nodes[i].end()) {
            if (!nodes[i]["name"].is_string()) {
                throw MisformattedExceptionNotString("nodes[i][name]");
            }

            asset.nodes[i].name = nodes[i]["name"];
        }

        // camera
        if (nodes[i].find("camera") != nodes[i].end()) {
            if (!nodes[i]["camera"].is_number()) {
                throw MisformattedExceptionNotNumber("nodes[i][camera]");
            }

            asset.nodes[i].camera = nodes[i]["camera"].get<int32_t>();
        }

        // children
        if (nodes[i].find("children") != nodes[i].end()) {
            auto& children = nodes[i]["children"];
            if (!children.is_array()) {
                throw MisformattedExceptionNotArray("nodes[i][chidren]");
            }

            asset.nodes[i].children.resize(children.size());
            for (uint32_t j = 0; j < children.size(); ++j) {
                asset.nodes[i].children[j] = children[j].get<uint32_t>();
            }
        }

        // skin
        if (nodes[i].find("skin") != nodes[i].end()) {
            if (!nodes[i]["skin"].is_number()) {
                throw MisformattedExceptionNotNumber("nodes[i][skin]");
            }

            asset.nodes[i].skin = nodes[i]["skin"].get<int32_t>();
        }

        // TODO: nodes[i]["matrix"]

        // mesh
        if (nodes[i].find("mesh") != nodes[i].end()) {
            if (!nodes[i]["mesh"].is_number()) {
                throw MisformattedExceptionNotNumber("nodes[i][mesh]");
            }

            asset.nodes[i].mesh = nodes[i]["mesh"].get<int32_t>();
        }

        // TODO: nodes[i]["rotation"]
        // TODO: nodes[i]["scale"]
        // TODO: nodes[i]["translation"]
        // TODO: nodes[i]["weights"]
    }
}

static void loadBuffers(Asset& asset, nlohmann::json& json) {
    if (json.find("buffers") == json.end()) {
        return;
    }

    auto& buffers = json["buffers"];
    if (!buffers.is_array()) {
        throw MisformattedExceptionNotArray("buffers");
    }

    asset.buffers.resize(buffers.size());
    for (uint32_t i = 0; i < buffers.size(); ++i) {

        // name
        if (buffers[i].find("name") != buffers[i].end()) {
            if (!buffers[i]["name"].is_string()) {
                throw MisformattedExceptionNotString("buffers[i][name]");
            }
            asset.buffers[i].name = buffers[i]["name"];
        }

        // byteLength
        if (buffers[i].find("byteLength") == buffers[i].end()) {
            throw MisformattedExceptionIsRequired("buffers[i][byteLength]");
        } else if (!buffers[i]["byteLength"].is_number()) {
            throw MisformattedExceptionNotNumber("buffers[i][byteLength]");
        }
        asset.buffers[i].byteLength = buffers[i]["byteLength"].get<int32_t>();

        // uri
        if (buffers[i].find("uri") != buffers[i].end()) {
            if (!buffers[i]["uri"].is_string()) {
                throw MisformattedExceptionNotString("buffers[i][uri]");
            }

            asset.buffers[i].uri = buffers[i]["uri"];
        }

        loadBufferData(asset, asset.buffers[i]);
    }
}

static void loadAccessors(Asset& asset, nlohmann::json& json) {
    if (json.find("accessors") == json.end()) {
        return;
    }

    auto& accessors = json["accessors"];

    if (!accessors.is_array()) {
        throw MisformattedExceptionNotArray("accessors");
    }

    asset.accessors.resize(accessors.size());
    for (uint32_t i = 0; i < accessors.size(); ++i) {
        // bufferView
        if (accessors[i].find("bufferView") != accessors[i].end()) {
            if (!accessors[i]["bufferView"].is_number()) {
                throw MisformattedExceptionNotNumber("accessors[i][bufferView]");
            }

            asset.accessors[i].bufferView = accessors[i]["bufferView"].get<int32_t>();
        }

        // bufferView
        if (accessors[i].find("bufferView") != accessors[i].end()) {
            if (!accessors[i]["bufferView"].is_number()) {
                throw MisformattedExceptionNotNumber("accessors[i][bufferView]");
            }

            asset.accessors[i].bufferView = accessors[i]["bufferView"].get<int32_t>();
        }

        // byteOffset
        if (accessors[i].find("byteOffset") != accessors[i].end()) {
            if (!accessors[i]["byteOffset"].is_number()) {
                throw MisformattedExceptionNotNumber("accessors[i][byteOffset]");
            }

            asset.accessors[i].byteOffset = accessors[i]["byteOffset"].get<uint32_t>();
        }

        // componentType
        if (accessors[i].find("componentType") == accessors[i].end()) {
            throw MisformattedExceptionIsRequired("accessors[i][componentType]");
        } else if (!accessors[i]["componentType"].is_number()) {
            throw MisformattedExceptionNotNumber("accessors[i][componentType]");
        }

        asset.accessors[i].componentType = static_cast<Accessor::ComponentType>(accessors[i]["componentType"].get<uint16_t>());

        // normalized
        if (accessors[i].find("normalized") != accessors[i].end()) {
            if (!accessors[i]["normalized"].is_boolean()) {
                throw MisformattedExceptionNotBoolean("accessors[i][normalized]");
            }

            asset.accessors[i].normalized = accessors[i]["normalized"].get<bool>();
        }

        // count
        if (accessors[i].find("count") == accessors[i].end()) {
            throw MisformattedExceptionIsRequired("accessors[i][count]");
        } else if (!accessors[i]["count"].is_number()) {
            throw MisformattedExceptionNotNumber("accessors[i][count]");
        }

        asset.accessors[i].count = accessors[i]["count"].get<uint32_t>();

        // type
        if (accessors[i].find("type") == accessors[i].end()) {
            throw MisformattedExceptionIsRequired("accessors[i][type]");
        } else if (!accessors[i]["type"].is_string()) {
            throw MisformattedExceptionNotString("accessors[i][type]");
        }

        std::string type = accessors[i]["type"].get<std::string>();
        if (type == "SCALAR") {
            asset.accessors[i].type = Accessor::Type::Scalar;
        } else if (type == "VEC2") {
            asset.accessors[i].type = Accessor::Type::Vec2;
        } else if (type == "VEC3") {
            asset.accessors[i].type = Accessor::Type::Vec3;
        } else if (type == "VEC4") {
            asset.accessors[i].type = Accessor::Type::Vec4;
        } else if (type == "MAT2") {
            asset.accessors[i].type = Accessor::Type::Mat2;
        } else if (type == "MAT3") {
            asset.accessors[i].type = Accessor::Type::Mat3;
        } else if (type == "MAT4") {
            asset.accessors[i].type = Accessor::Type::Mat4;
        } else {
            throw MisformattedException("accessors[i][type]", "is not a valid type");
        }

        // TODO: accessors[i]["sparse"]
        // TODO: accessors[i]["extensions"]
        // TODO: accessors[i]["min"]
        // TODO: accessors[i]["max"]
    }
}

static void loadBufferViews(Asset& asset, nlohmann::json& json) {
    if (json.find("bufferViews") == json.end()) {
        return;
    }

    auto& bufferViews = json["bufferViews"];
    if (!bufferViews.is_array()) {
        throw MisformattedExceptionNotArray("bufferViews");
    }

    asset.bufferViews.resize(bufferViews.size());
    for (uint32_t i = 0; i < bufferViews.size(); ++i) {
        // name
        if (bufferViews[i].find("name") != bufferViews[i].end()) {
            if (!bufferViews[i]["name"].is_string()) {
                throw MisformattedExceptionNotString("bufferViews[i][name]");
            }

            asset.bufferViews[i].name = bufferViews[i]["name"];
        }

        // buffer
        if (bufferViews[i].find("buffer") == bufferViews[i].end()) {
            throw MisformattedExceptionIsRequired("bufferViews[i][buffer]");
        }
        if (!bufferViews[i]["buffer"].is_number()) {
            throw MisformattedExceptionNotNumber("bufferViews[i][buffer]");
        }
        asset.bufferViews[i].buffer = bufferViews[i]["buffer"].get<int32_t>();

        // byteOffset
        if (bufferViews[i].find("byteOffset") == bufferViews[i].end()) {
            throw MisformattedExceptionIsRequired("bufferViews[i][byteOffset]");
        }
        if (!bufferViews[i]["byteOffset"].is_number()) {
            throw MisformattedExceptionNotNumber("bufferViews[i][byteOffset]");
        }
        asset.bufferViews[i].byteOffset = bufferViews[i]["byteOffset"].get<int32_t>();

        // byteLength
        if (bufferViews[i].find("byteLength") == bufferViews[i].end()) {
            throw MisformattedExceptionIsRequired("bufferViews[i][byteLength]");
        }
        if (!bufferViews[i]["byteLength"].is_number()) {
            throw MisformattedExceptionNotNumber("bufferViews[i][byteLength]");
        }
        asset.bufferViews[i].byteLength = bufferViews[i]["byteLength"].get<int32_t>();

        // byteStride
        if (bufferViews[i].find("byteStride") != bufferViews[i].end()) {
            if (!bufferViews[i]["byteStride"].is_number()) {
                throw MisformattedExceptionNotNumber("bufferViews[i][byteStride]");
            }

            asset.bufferViews[i].byteStride = bufferViews[i]["byteStride"].get<int32_t>();
        }

        // target
        if (bufferViews[i].find("target") != bufferViews[i].end()) {
            if (!bufferViews[i]["target"].is_number()) {
                throw MisformattedExceptionNotNumber("bufferViews[i][target]");
            }
            asset.bufferViews[i].target = static_cast<BufferView::TargetType>(bufferViews[i]["target"].get<uint16_t>());
        }

        // TODO: bufferViews[i]["extensions"]
        // TODO: bufferViews[i]["extras"]
    }
}

static void loadBufferData(Asset& asset, Buffer& buffer) {
    if (!buffer.uri.size() && buffer.byteLength > 0) {
        throw MisformattedException("buffers[i]", "is not empty but has no uri");
    }

    buffer.data = new char[buffer.byteLength];
    // TODO: load base64 uri
    std::ifstream fileData(pathAppend(asset.dirName, buffer.uri), std::ios::binary);
    if (!fileData.good()) {
        throw MisformattedException("buffers[i].uri", "has not a valid uri (failed to open file)");
    }
    fileData.read(buffer.data, buffer.byteLength);
    fileData.close();
}

static std::string pathAppend(const std::string& p1, const std::string& p2) {
    char sep = '/';
    std::string tmp = p1;

    if (p1[p1.length()] != sep) { // Need to add a
        tmp += sep;                // path separator
        return tmp + p2;
    } else {
        return p1 + p2;
    }
}

static std::string getDirectoryName(const std::string& path) {
    std::size_t found;

    found = path.find_last_of("/");
    if (found == std::string::npos) {
        return "";
    }

    return path.substr(0, found);
}

static void loadMaterials(Asset& asset, nlohmann::json& json) {
    if (json.find("materials") == json.end()) {
        return;
    }

    auto& materials = json["materials"];
    if (!materials.is_array()) {
        throw MisformattedExceptionNotArray("materials");
    }

    asset.materials.resize(materials.size());
    for (uint32_t i = 0; i < materials.size(); ++i) {
        // name
        if (materials[i].find("name") != materials[i].end()) {
            if (!materials[i]["name"].is_string()) {
                throw MisformattedExceptionNotString("materials[i][name]");
            }

            asset.materials[i].name = materials[i]["name"];
        }

        // pbrMetallicRoughness
        if (materials[i].find("pbrMetallicRoughness") != materials[i].end()) {
            if (!materials[i]["pbrMetallicRoughness"].is_object()) {
                throw MisformattedExceptionNotObject("materials[i][pbrMetallicRoughness]");
            }

            // pbrMetallicRoughness.baseColorFactor
            if (materials[i]["pbrMetallicRoughness"].find("baseColorFactor") != materials[i]["pbrMetallicRoughness"].end()) {
                if (!materials[i]["pbrMetallicRoughness"]["baseColorFactor"].is_array()) {
                    throw MisformattedExceptionNotArray("materials[i][pbrMetallicRoughness][baseColorFactor]");
                }

                if (materials[i]["pbrMetallicRoughness"]["baseColorFactor"].size() != 4) {
                    throw MisformattedExceptionNotGoodSizeArray("materials[i][pbrMetallicRoughness][baseColorFactor]");
                }

                for (uint32_t j = 0; j < 4; ++j) {
                    if (!materials[i]["pbrMetallicRoughness"]["baseColorFactor"][j].is_number()) {
                        throw MisformattedExceptionNotNumber("materials[i][pbrMetallicRoughness][baseColorFactor][j]");
                    }

                    asset.materials[i].pbr.baseColorFactor[j] = materials[i]["pbrMetallicRoughness"]["baseColorFactor"][j].get<float>();
                }
            }

            // pbrMetallicRoughness.baseColorTexture
            if (materials[i]["pbrMetallicRoughness"].find("baseColorTexture") != materials[i]["pbrMetallicRoughness"].end()) {
                loadTextureInfo(asset.materials[i].pbr.baseColorTexture, materials[i]["pbrMetallicRoughness"]["baseColorTexture"]);
            }

            // pbrMetallicRoughness.metallicFactor
            if (materials[i]["pbrMetallicRoughness"].find("metallicFactor") != materials[i]["pbrMetallicRoughness"].end()) {
                if (!materials[i]["pbrMetallicRoughness"]["metallicFactor"].is_number()) {
                    throw MisformattedExceptionNotNumber("materials[i][pbrMetallicRoughness][metallicFactor]");
                }

                asset.materials[i].pbr.metallicFactor = materials[i]["pbrMetallicRoughness"]["metallicFactor"].get<float>();
            }

            // pbrMetallicRoughness.roughnessFactor
            if (materials[i]["pbrMetallicRoughness"].find("roughnessFactor") != materials[i]["pbrMetallicRoughness"].end()) {
                if (!materials[i]["pbrMetallicRoughness"]["roughnessFactor"].is_number()) {
                    throw MisformattedExceptionNotNumber("materials[i][pbrMetallicRoughness][roughnessFactor]");
                }

                asset.materials[i].pbr.roughnessFactor = materials[i]["pbrMetallicRoughness"]["roughnessFactor"].get<float>();
            }

            // pbrMetallicRoughness.metallicRoughnessTexture
            if (materials[i]["pbrMetallicRoughness"].find("metallicRoughnessTexture") != materials[i]["pbrMetallicRoughness"].end()) {
                loadTextureInfo(asset.materials[i].pbr.metallicRoughnessTexture, materials[i]["pbrMetallicRoughness"]["metallicRoughnessTexture"]);
            }
        }

        // normalTexture
        if (materials[i].find("normalTexture") != materials[i].end()) {
            loadTextureInfo(asset.materials[i].normalTexture, materials[i]["normalTexture"]);

            // scale
            if (materials[i]["normalTexture"].find("scale") != materials[i]["normalTexture"].end()) {
                if (!materials[i]["normalTexture"]["scale"].is_number()) {
                    throw MisformattedExceptionNotNumber("materials[i][normalTexture][scale]");
                }
            }
        }

        // occlusionTexture
        if (materials[i].find("occlusionTexture") != materials[i].end()) {
            loadTextureInfo(asset.materials[i].occlusionTexture, materials[i]["occlusionTexture"]);

            // scale
            if (materials[i]["occlusionTexture"].find("strength") != materials[i]["occlusionTexture"].end()) {
                if (!materials[i]["occlusionTexture"]["strength"].is_number()) {
                    throw MisformattedExceptionNotNumber("materials[i][occlusionTexture][strength]");
                }
            }
        }

        // emissiveTexture
        if (materials[i].find("emissiveTexture") != materials[i].end()) {
            loadTextureInfo(asset.materials[i].emissiveTexture, materials[i]["emissiveTexture"]);
        }

        // emissiveFactor
        if (materials[i].find("emissiveFactor") != materials[i].end()) {
            if (!materials[i]["emissiveFactor"].is_array()) {
                throw MisformattedExceptionNotArray("materials[i][emissiveFactor]");
            }

            if (materials[i]["emissiveFactor"].size() != 3) {
                throw MisformattedExceptionNotGoodSizeArray("materials[i][emissiveFactor]");
            }

            for (uint32_t j = 0; j < 3; ++j) {
                if (!materials[i]["emissiveFactor"][j].is_number()) {
                    throw MisformattedExceptionNotNumber("materials[i][emissiveFactor][j]");
                }

                asset.materials[i].emissiveFactor[j] = materials[i]["emissiveFactor"][j].get<float>();
            }
        }

        // alphaMode
        if (materials[i].find("alphaMode") != materials[i].end()) {
            if (!materials[i]["alphaMode"].is_string()) {
                throw MisformattedExceptionNotString("materials[i][alphaMode]");
            }

            std::string type = materials[i]["alphaMode"].get<std::string>();
            if (type == "OPAQUE") {
                asset.materials[i].alphaMode = Material::AlphaMode::Opaque;
            } else if (type == "MASK") {
                asset.materials[i].alphaMode = Material::AlphaMode::Mask;
            } else if (type == "BLEND") {
                asset.materials[i].alphaMode = Material::AlphaMode::Blend;
            } else {
                throw MisformattedException("materials[i][alphaMode]", "is not a valid string");
            }
        }

        // alphaCutoff
        if (materials[i].find("alphaCutoff") != materials[i].end()) {
            if (!materials[i]["alphaCutoff"].is_number()) {
                throw MisformattedExceptionNotNumber("materials[i][alphaCutoff]");
            }

            asset.materials[i].alphaCutoff = materials[i]["alphaCutoff"].get<float>();
        }

        // doubleSided
        if (materials[i].find("doubleSided") != materials[i].end()) {
            if (!materials[i]["doubleSided"].is_boolean()) {
                throw MisformattedExceptionNotBoolean("materials[i][doubleSided]");
            }

            asset.materials[i].doubleSided = materials[i]["doubleSided"].get<bool>();
        }

        // TODO: materials[i]["extensions"]
        // TODO: materials[i]["extras"]
    }
}

static void loadTextureInfo(Material::Texture& texture, nlohmann::json& json) {
    if (!json.is_object()) {
        throw MisformattedExceptionNotNumber("textureInfo");
    }

    // index
    if (json.find("index") == json.end()) {
        throw MisformattedExceptionIsRequired("textureInfo[index]");
    }

    if (!json["index"].is_number()) {
        throw MisformattedExceptionNotNumber("textureInfo[index]");
    }

    texture.index = json["index"].get<int32_t>();

    // texCoord
    if (json.find("texCoord") != json.end()) {
        if (!json["texCoord"].is_number()) {
            throw MisformattedExceptionNotNumber("textureInfo[texCoord]");
        }

        texture.texCoord = json["texCoord"].get<int32_t>();
    }

    // TODO: json["extensions"]
    // TODO: json["extras"]
}

static void loadImages(Asset& asset, nlohmann::json& json) {
    if (json.find("images") == json.end()) {
        return;
    }

    auto& images = json["images"];
    if (!images.is_array()) {
        throw MisformattedExceptionNotArray("images");
    }

    asset.images.resize(images.size());
    for (uint32_t i = 0; i < images.size(); ++i) {
        // name
        if (images[i].find("name") != images[i].end()) {
            if (!images[i]["name"].is_string()) {
                throw MisformattedExceptionNotString("images[i][name]");
            }

            asset.images[i].name = images[i]["name"];
        }

        // uri
        // TODO: load base64 uri
        if (images[i].find("uri") != images[i].end()) {
            if (!images[i]["uri"].is_string()) {
                throw MisformattedExceptionNotString("images[i][uri]");
            }

            asset.images[i].uri = pathAppend(asset.dirName, images[i]["uri"]);
        }

        // mimeType
        if (images[i].find("mimeType") != images[i].end()) {
            if (!images[i]["mimeType"].is_string()) {
                throw MisformattedExceptionNotString("images[i][mimeType]");
            }

            asset.images[i].mimeType = images[i]["mimeType"];
        }

        // bufferView
        if (images[i].find("bufferView") != images[i].end()) {
            if (!images[i]["bufferView"].is_number()) {
                throw MisformattedExceptionNotNumber("images[i][bufferView]");
            }

            asset.images[i].bufferView = images[i]["bufferView"].get<int32_t>();
        }

        // TODO: Handle dependencies between mimeType and bufferView
        // TODO: Handle the fact that we want an uri OR a bufferView
        // TODO: images[i]["extensions"]
        // TODO: images[i]["extras"]
    }
}

Asset load(std::string fileName) {
    Asset asset{};

    std::ifstream file(fileName);
    asset.dirName = getDirectoryName(fileName);

    // TODO: Check the extension (.gltf / .glb)

    nlohmann::json json;

    file >> json;

    loadAsset(asset, json);
    loadScenes(asset, json);
    loadMeshes(asset, json);
    loadNodes(asset, json);
    loadBuffers(asset, json);
    loadBufferViews(asset, json);
    loadAccessors(asset, json);
    loadMaterials(asset, json);
    loadImages(asset, json);

    return asset;
}

} // gltf2
