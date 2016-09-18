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


#include "asset_model.h"

#include "io/system.h"
#include "io/file.h"
#include "io/path.h"

#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

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
        auto root = scene::make_root(scene_->mRootNode->mName.data);
        load(scene_->mRootNode, root);

        return root;
    }

private:

    void load(const aiNode * source, utils::pointer<scene::node> dest) const
    {
        dest->set_local_matrix(aiMatrix4x4_to_mat4(source->mTransformation));

        for(size_t i = 0; i < source->mNumChildren; ++i)
        {
            auto child = dest->add_node(source->mChildren[i]->mName.data);
            load(source->mChildren[i], child.lock());
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

    asset_model::gmap load() const
    {
        asset_model::gmap geometry;
        load(scene_->mRootNode, geometry);
        return geometry;
    }

    void load(const aiNode * source, asset_model::gmap & geometry) const
    {
        auto & meshes = geometry[source->mName.data];

        for(size_t i = 0; i < source->mNumMeshes; ++i)
            meshes.emplace_back(scene_->mMeshes[source->mMeshes[i]]);

        for(size_t i = 0; i < source->mNumChildren; ++i)
            load(source->mChildren[i], geometry);
    }

private:

    const aiScene * scene_;
};

struct material_loader
{
    material_loader(const aiScene * scene, const std::string & path)
        : scene_(scene)
        , path_(path)
    {}

    asset_model::mmap load() const
    {
        asset_model::mmap material;
        load(scene_->mRootNode, material);
        return material;
    }

private:

    void load(const aiNode * source, asset_model::mmap & material) const
    {
        auto & meshes = material[source->mName.data];

        for(size_t i = 0; i < source->mNumMeshes; ++i)
        {
            size_t index = scene_->mMeshes[source->mMeshes[i]]->mMaterialIndex;
            meshes.emplace_back(scene_->mMaterials[index], path_);
        }

        for(size_t i = 0; i < source->mNumChildren; ++i)
            load(source->mChildren[i], material);
    }

private:

    const aiScene * scene_;
    std::string path_;
};

asset_model::geometry::geometry(const aiMesh * mesh)
{
    assert(mesh != nullptr);

    load_vertices(mesh);
    load_faces(mesh);
}

void asset_model::geometry::load_vertices(const aiMesh * mesh)
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

void asset_model::geometry::load_faces(const aiMesh * mesh)
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

asset_model::material::material(const aiMaterial * material, const std::string & path)
{
    if(material)
        load_material(material, path);
}

void asset_model::material::load_material(const aiMaterial * material, const std::string & path)
{
    static const aiTextureType aiId[] =
    {
        aiTextureType_DIFFUSE,
        aiTextureType_SPECULAR,
        aiTextureType_NORMALS
    };

    for(size_t i = 0; i < utils::to_int(scene::material::type_texture::COUNT); ++i)
    {
        aiString name;
        if(material->GetTextureCount(aiId[i]) &&
           material->GetTexture(aiId[i], 0, &name) == aiReturn_SUCCESS)
            material_.set_texture(scene::material::type_texture(i), path + name.C_Str());
    }

    aiColor3D color(0.0f, 0.0f, 0.0f);

    if(material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == aiReturn_SUCCESS)
        material_.set_color(scene::material::type_color::diffuse, math::vec3(color.r, color.g, color.b));
    if(material->Get(AI_MATKEY_COLOR_SPECULAR, color) == aiReturn_SUCCESS)
        material_.set_color(scene::material::type_color::specular, math::vec3(color.r, color.g, color.b));
    if(material->Get(AI_MATKEY_COLOR_AMBIENT, color) == aiReturn_SUCCESS)
        material_.set_color(scene::material::type_color::ambient, math::vec3(color.r, color.g, color.b));
}

utils::pointer<scene::node> asset_model::get_hierarchy() const
{
    return hierarchy_;
}

const std::vector<asset_model::geometry> &
    asset_model::get_node_geometry(const std::string & name) const
{
    static std::vector<geometry> empty;

    auto it = geometry_.find(name);
    return it != geometry_.end() ? it->second : empty;
}

const std::vector<asset_model::material> &
    asset_model::get_node_material(const std::string & name) const
{
    static std::vector<material> empty;

    auto it = material_.find(name);
    return it != material_.end() ? it->second : empty;
}

bool asset_model::load(utils::link<io::system> fs, const std::string & resource)
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

    material_loader mloader(scene, io::parent_path(get_resource()));
    material_ = mloader.load();

    return true;
}

} /* eps */
