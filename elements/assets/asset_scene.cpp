/*
The MIT License (MIT)
Copyright (c) 2016 Alexey Yegorov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
*/


#include "asset_scene.h"

#include "io/system.h"
#include "io/file.h"
#include "io/path.h"

#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

#include <android/log.h>

#define  LOG_TAG  "Elements"
#define  LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

namespace eps {

struct assimp_io_stream : public Assimp::IOStream
{
    assimp_io_stream(utils::link<io::system> fs, const char * file)
        : fs_(fs)
    {
        if(!fs_.expired())
            stream_ = fs_.lock()->open(file);
    }

    ~assimp_io_stream()
    {
        if(!fs_.expired())
            fs_.lock()->close(stream_);
    }

    size_t FileSize() const final
    {
        return stream_ ? stream_->size() : 0;
    }

    void Flush() final
    {
        if(stream_)
            stream_->flush();
    }

    size_t Read(void * buffer, size_t size, size_t count) final
    {
        return stream_ ? stream_->read(buffer, size, count) : 0;
    }

    aiReturn Seek(size_t offset, aiOrigin origin) final
    {
        if(stream_ && stream_->seek(offset, origin) == 0)
            return aiReturn_SUCCESS;
        return aiReturn_FAILURE;
    }

    size_t Tell() const final
    {
        return stream_ ? stream_->tell() : 0;
    }

    size_t Write(const void *, size_t, size_t) final
    {
        return 0;
    }

private:

    utils::link<io::system> fs_;
    io::file * stream_ = nullptr;
};

struct assimp_io_system : public Assimp::IOSystem
{
    explicit assimp_io_system(utils::link<io::system> fs)
        : fs_(fs)
    {}

    bool Exists(const char *file) const final
    {
        return fs_.expired() ? false : fs_.lock()->exists(file);
    }

    Assimp::IOStream * Open(const char * file, const char * = "rb") final
    {
        return fs_.expired() ? nullptr : new assimp_io_stream(fs_, file);
    }

    void Close(Assimp::IOStream * stream) final
    {
        delete stream;
    }

    char getOsSeparator() const final { return '/'; }

private:

    utils::link<io::system> fs_;
};

struct hierarchy_loader
{
    hierarchy_loader(const aiScene * scene)
        : scene_(scene)
    {}

    utils::pointer<scene::node> load() const
    {
        auto root = scene::make_root("root");
        load(scene_->mRootNode, root);

        return root;
    }

private:

    void load(const aiNode * source, utils::pointer<scene::node> dest) const
    {
        dest->set_local_matrix(aiMatrix4x4_to_mat4(source->mTransformation));
        for(size_t i = 0; i < source->mNumChildren; ++i)
        {
            if(auto pchild = dest->add_node(source->mChildren[i]->mName.data).lock())
            {
                load(source->mChildren[i], pchild);

                for(size_t j = 0; j < source->mChildren[i]->mNumMeshes; ++j)
                    pchild->add_node(source->mChildren[i]->mName.data + std::to_string(j));
            }
        }
    }

    math::mat4 aiMatrix4x4_to_mat4(const aiMatrix4x4 & from) const
    {
        math::mat4 to;

        to[0][0] = from.a1; to[0][1] = from.b1;  to[0][2] = from.c1; to[0][3] = from.d1;
        to[1][0] = from.a2; to[1][1] = from.b2;  to[1][2] = from.c2; to[1][3] = from.d2;
        to[2][0] = from.a3; to[2][1] = from.b3;  to[2][2] = from.c3; to[2][3] = from.d3;
        to[3][0] = from.a4; to[3][1] = from.b4;  to[3][2] = from.c4; to[3][3] = from.d4;

        return to;
    }

private:

    const aiScene * scene_;
};

struct geometry_loader
{
    explicit geometry_loader(const aiScene * scene)
        : scene_(scene)
    {}

    asset_scene::gmap load() const
    {
        asset_scene::gmap result;
        load(scene_->mRootNode, result);
        return result;
    }

    void load(const aiNode * source, asset_scene::gmap & result) const
    {
        for(size_t i = 0; i < source->mNumMeshes; ++i)
        {
            result.insert(
            {
                source->mName.data + std::to_string(i),
                asset_scene::geometry(scene_->mMeshes[source->mMeshes[i]])
            });
        }
        for(size_t i = 0; i < source->mNumChildren; ++i)
            load(source->mChildren[i], result);
    }

private:

    const aiScene * scene_;
};

struct maps_loader
{
    maps_loader(const aiScene * scene, const std::string & path)
        : scene_(scene)
        , path_(path)
    {}

    asset_scene::mmap load() const
    {
        asset_scene::mmap result;
        load(scene_->mRootNode, result);
        return result;
    }

private:

    void load(const aiNode * source, asset_scene::mmap & result) const
    {
        for(size_t i = 0; i < source->mNumMeshes; ++i)
        {
            size_t index = scene_->mMeshes[source->mMeshes[i]]->mMaterialIndex;
            result.insert(
            {
                source->mName.data + std::to_string(i),
                asset_scene::maps(scene_->mMaterials[index], path_)
            });
        }

        for(size_t i = 0; i < source->mNumChildren; ++i)
            load(source->mChildren[i], result);
    }

private:

    const aiScene * scene_;
    std::string path_;
};

struct colors_loader
{
    colors_loader(const aiScene * scene)
        : scene_(scene)
    {}

    asset_scene::cmap load() const
    {
        asset_scene::cmap result;
        load(scene_->mRootNode, result);
        return result;
    }

private:

    void load(const aiNode * source, asset_scene::cmap & result) const
    {
        for(size_t i = 0; i < source->mNumMeshes; ++i)
        {
            size_t index = scene_->mMeshes[source->mMeshes[i]]->mMaterialIndex;
            result.insert(
            {
                source->mName.data + std::to_string(i),
                asset_scene::colors(scene_->mMaterials[index])
            });
        }

        for(size_t i = 0; i < source->mNumChildren; ++i)
            load(source->mChildren[i], result);
    }

private:

    const aiScene * scene_;
};

class light_loader
{
public:

    explicit light_loader(const aiScene * scene)
        : scene_(scene)
    {}

    asset_scene::lmap load() const
    {
        asset_scene::lmap lights;
        load(scene_->mRootNode, lights);
        return lights;
    }

private:

    void load(const aiNode * source, asset_scene::lmap & lights) const
    {
        if(const aiLight * light = find_light(source->mName.data))
            lights.insert({source->mName.data, asset_scene::light(light)});

        for(size_t i = 0; i < source->mNumChildren; ++i)
            load(source->mChildren[i], lights);
    }

    const aiLight * find_light(const std::string & name) const
    {
        for(size_t i = 0; i < scene_->mNumLights; ++i)
        {
            if(name == scene_->mLights[i]->mName.data)
                return scene_->mLights[i];
        }

        return nullptr;
    }

private:

    const aiScene * scene_;
};

asset_scene::geometry::geometry(const aiMesh * mesh)
{
    assert(mesh != nullptr);

    load_vertices(mesh);
    load_faces(mesh);
}

void asset_scene::geometry::load_vertices(const aiMesh * mesh)
{
    vertices_.resize(mesh->mNumVertices);

    if(mesh->HasPositions())
    {
        for(size_t i = 0, end = vertices_.size(); i < end; ++i)
        {
            vertices_[i].position.x = mesh->mVertices[i].x;
            vertices_[i].position.y = mesh->mVertices[i].y;
            vertices_[i].position.z = mesh->mVertices[i].z;
        }
    }

    if(mesh->HasNormals())
    {
        for(size_t i = 0, end = vertices_.size(); i < end; ++i)
        {
            vertices_[i].normal.x = mesh->mNormals[i].x;
            vertices_[i].normal.y = mesh->mNormals[i].y;
            vertices_[i].normal.z = mesh->mNormals[i].z;
        }
    }

    if(mesh->HasTangentsAndBitangents())
    {
        for(size_t i = 0, end = vertices_.size(); i < end; ++i)
        {
            vertices_[i].tangent.x = mesh->mTangents[i].x;
            vertices_[i].tangent.y = mesh->mTangents[i].y;
            vertices_[i].tangent.z = mesh->mTangents[i].z;
        }
    }

    if(mesh->HasTextureCoords(0))
    {
        for(size_t i = 0, end = vertices_.size(); i < end; ++i)
        {
            vertices_[i].tex.x = mesh->mTextureCoords[0][i].x;
            vertices_[i].tex.y = mesh->mTextureCoords[0][i].y;
        }
    }
}

void asset_scene::geometry::load_faces(const aiMesh * mesh)
{
    faces_.resize(mesh->mNumFaces);

    for(size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        assert(mesh->mFaces[i].mNumIndices == 3);

        faces_[i].indices[0] = mesh->mFaces[i].mIndices[0];
        faces_[i].indices[1] = mesh->mFaces[i].mIndices[1];
        faces_[i].indices[2] = mesh->mFaces[i].mIndices[2];
    }
}

asset_scene::maps::maps(const aiMaterial * material, const std::string & path)
{
    if(material)
        load_maps(material, path);
}

void asset_scene::maps::load_maps(const aiMaterial * material, const std::string & path)
{
    aiString name;
    if(material->GetTextureCount(aiTextureType_DIFFUSE) &&
       material->GetTexture(aiTextureType_DIFFUSE, 0, &name) == aiReturn_SUCCESS)
    {
        map_diffuse_ = path + name.C_Str();
    }

    if(material->GetTextureCount(aiTextureType_SPECULAR) &&
       material->GetTexture(aiTextureType_SPECULAR, 0, &name) == aiReturn_SUCCESS)
    {
        map_specular_ = path + name.C_Str();
    }

    if(material->GetTextureCount(aiTextureType_NORMALS) &&
       material->GetTexture(aiTextureType_NORMALS, 0, &name) == aiReturn_SUCCESS)
    {
        map_normal_ = path + name.C_Str();
    }
}

asset_scene::colors::colors(const aiMaterial * material)
{
    if(material)
        load_colors(material);
}

void asset_scene::colors::load_colors(const aiMaterial * material)
{
    aiColor3D color(0.0f, 0.0f, 0.0f);

    if(material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == aiReturn_SUCCESS)
        color_diffuse_ = math::vec3(color.r, color.g, color.b);
    if(material->Get(AI_MATKEY_COLOR_SPECULAR, color) == aiReturn_SUCCESS)
        color_specular_ = math::vec3(color.r, color.g, color.b);
    if(material->Get(AI_MATKEY_COLOR_AMBIENT, color) == aiReturn_SUCCESS)
        color_ambient_ = math::vec3(color.r, color.g, color.b);
}

asset_scene::light::light(const aiLight * light)
{
    load_light(light);
}

void asset_scene::light::load_light(const aiLight * light)
{
    intensity_ = math::vec3(light->mColorDiffuse.r, light->mColorDiffuse.g, light->mColorDiffuse.b);
    attenuation_c_ = light->mAttenuationConstant;
    attenuation_l_ = light->mAttenuationLinear;
    attenuation_q_ = light->mAttenuationQuadratic;
}

utils::pointer<scene::node> asset_scene::get_hierarchy() const
{
    return hierarchy_;
}

const asset_scene::geometry *
    asset_scene::get_node_geometry(const std::string & name) const
{
    auto it = geometry_.find(name);
    return it != geometry_.end() ? &it->second : nullptr;
}

const asset_scene::maps *
    asset_scene::get_node_maps(const std::string & name) const
{
    auto it = maps_.find(name);
    return it != maps_.end() ? &it->second : nullptr;
}

const asset_scene::colors *
    asset_scene::get_node_colors(const std::string & name) const
{
    auto it = colors_.find(name);
    return it != colors_.end() ? &it->second : nullptr;
}

const asset_scene::light *
    asset_scene::get_node_light(const std::string & name) const
{
    auto it = lights_.find(name);
    return it != lights_.end() ? &it->second : nullptr;
}

bool asset_scene::load(utils::link<io::system> fs, const std::string & resource)
{
    Assimp::Importer importer;

    importer.SetIOHandler(new assimp_io_system(fs));
    importer.ReadFile(resource, aiProcess_Triangulate |
                                aiProcess_JoinIdenticalVertices |
                                aiProcess_CalcTangentSpace |
                                aiProcess_FlipUVs);

    const aiScene * scene = importer.GetScene();

    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        return false;

    hierarchy_loader hloader(scene);
    hierarchy_ = hloader.load();

    geometry_loader gloader(scene);
    geometry_ = gloader.load();

    maps_loader mloader(scene, io::parent_path(get_resource()));
    maps_ = mloader.load();

    colors_loader cloader(scene);
    colors_ = cloader.load();

    light_loader lloader(scene);
    lights_ = lloader.load();

    return true;
}

} /* eps */
