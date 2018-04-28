#include "OvrPlatformIntegration.h"
#include "AvatarShader.h"

#include <OVR_Platform.h>

#include <Magnum/TextureFormat.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/ImageView.h>
#include <Corrade/Containers/ArrayView.h>

namespace Magnum { namespace OvrIntegration { namespace Platform {

std::tuple<Mesh, std::unique_ptr<Buffer>, std::unique_ptr<Buffer>> toMesh(const ovrAvatarMeshAssetData& data) {
    Mesh mesh;
    Buffer vertexBuffer, indexBuffer;

    vertexBuffer.setData(Containers::arrayView(data.vertexBuffer, data.vertexCount), Buffer::Usage::StaticDraw);
    indexBuffer.setData(Containers::arrayView(data.indexBuffer, data.indexCount), Buffer::Usage::StaticDraw);

    mesh.setCount(data.indexCount)
        .setIndexBuffer(indexBuffer, 0, Mesh::IndexType::UnsignedShort)
        .addVertexBuffer(vertexBuffer, 0, AvatarShader::Position{}, AvatarShader::Normal{}, AvatarShader::Tangent{},
                AvatarShader::TextureCoord{}, AvatarShader::BlendIndices{}, AvatarShader::BlendWeights{});

    return std::make_tuple(std::move(mesh), std::make_unique<Buffer>(std::move(vertexBuffer)), std::make_unique<Buffer>(std::move(indexBuffer)));
}

std::unique_ptr<Texture2D> toTexture2D(const ovrAvatarTextureAssetData& data) {
    Texture2D texture;

    texture.setMagnificationFilter(Sampler::Filter::Linear)
        .setMinificationFilter(Sampler::Filter::Linear)
        .setWrapping(Sampler::Wrapping::Repeat);

    switch(data.format) {
    case ovrAvatarTextureFormat_RGB24: {
            UnsignedInt width = data.sizeX;
            UnsignedInt height = data.sizeY;
            const UnsignedByte* dataPtr = data.textureData;
            for(UnsignedInt level = 0; level < data.mipCount; ++level) {
                ImageView2D imgData{PixelFormat::BGR,
                    PixelType::UnsignedByte,
                    {Int(width), Int(height)},
                    Containers::arrayView(dataPtr, width*height*3)};
                texture.setSubImage(level, {}, imgData);

                dataPtr += width * height * 3;
                width /= 2;
                height /= 2;
            }
            break;
    }
    case ovrAvatarTextureFormat_DXT1:
    case ovrAvatarTextureFormat_DXT5: {
            CompressedPixelFormat format;
            CompressedPixelStorage storage;

            if(data.format == ovrAvatarTextureFormat_DXT1) {
                storage.setCompressedBlockDataSize(8);
                format = CompressedPixelFormat::RGBAS3tcDxt1;
            } else {
                storage.setCompressedBlockDataSize(16);
                format = CompressedPixelFormat::RGBAS3tcDxt5;
            }

            UnsignedInt width = data.sizeX;
            UnsignedInt height = data.sizeY;
            const UnsignedByte* dataPtr = data.textureData;

            for(UnsignedInt level = 0; level < data.mipCount; ++level) {
                const UnsignedInt levelSize = (width < 4 || height < 4)
                    ? storage.compressedBlockDataSize()
                    : storage.compressedBlockDataSize() * (width / 4) * (height / 4);
                CompressedImageView2D imgData{storage, format, {Int(width), Int(height)}, Containers::arrayView(dataPtr, levelSize)};
                texture.setCompressedSubImage(level, {}, imgData);

                dataPtr += levelSize;
                width /= 2;
                height /= 2;
            }
            break;
        }
    }

    return std::make_unique<Texture2D>(std::move(texture));
}

void initialize(const char* appId) {
    if(ovr_PlatformInitializeWindows(appId) != ovrPlatformInitialize_Success) {
        Error() << "Failed to initialize the Oculus platform";
        return;
    }
    /* Initialize the avatar module */
    ovrAvatar_Initialize(appId);
}

void AvatarAssetLoader::requestAvatarSpecification() {
    /* Start retrieving the avatar specification */
    const ovrID userID = ovr_GetLoggedInUserID();
    ovrAvatar_RequestAvatarSpecification(userID);
}

Int AvatarAssetLoader::beginLoading(ovrAvatar* avatar) {
    /* Trigger load operations for all of the assets referenced by the avatar */
    const UnsignedInt refCount = ovrAvatar_GetReferencedAssetCount(avatar);
    for(UnsignedInt i = 0; i < refCount; ++i) {
        const ovrAvatarAssetID id = ovrAvatar_GetReferencedAsset(avatar, i);
        ovrAvatarAsset_BeginLoading(id);
    }

    return refCount;
}

void AvatarAssetLoader::pollAvatarMessages() {
    while(ovrAvatarMessage* message = ovrAvatarMessage_Pop()) {
        switch(ovrAvatarMessage_GetType(message)) {
            case ovrAvatarMessageType_AvatarSpecification: {
                const ovrAvatarSpecification* spec = ovrAvatarMessage_GetAvatarSpecification(message)->avatarSpec;
                avatarSpecificationLoaded(spec);
                break;
            }
            case ovrAvatarMessageType_AssetLoaded: {
                const ovrAvatarAsset* asset = ovrAvatarMessage_GetAssetLoaded(message)->asset;
                switch(ovrAvatarAsset_GetType(asset)) {
                    case ovrAvatarAssetType_Mesh:
                        meshLoaded(*ovrAvatarAsset_GetMeshData(asset));
                        break;
                    case ovrAvatarAssetType_Texture:
                        textureLoaded(*ovrAvatarAsset_GetTextureData(asset));
                        break;
                }
                break;
            }
        }
        ovrAvatarMessage_Free(message);
    }
}

}}}
