#ifndef Magnum_OvrPlatformIntegration_OvrPlatformIntegration_h_
#define Magnum_OvrPlatformIntegration_OvrPlatformIntegration_h_

#include <Corrade/Interconnect/Emitter.h>
#include <Magnum/Texture.h>
#include <Magnum/Mesh.h>
#include <Magnum/Buffer.h>

#include <OVR_Avatar.h>

#include <tuple>
#include <memory> /* for std::unique_ptr */

namespace Magnum { namespace OvrIntegration { namespace Platform {

/** @brief Initialize the avatar SDK with given app id */
void initialize(const char* appId);

/**
 * @brief Avatar Asset Loader
 *
 * Class which handles asynchroneous loading of avatar specification and assets.
 *
 * @todo Usage
 */
class AvatarAssetLoader: public Corrade::Interconnect::Emitter {
    public:
        /**
        * @brief Initialize avatar SDK and then asynchroneously request avatar specification.
        *
        * Call @ref pollAvatarMessages() for processing the request return.
        * Make sure to connect @ref onLoadMesh and @ref onLoadTexture.
        */
        void requestAvatarSpecification();

        /**
         * @brief Poll for new messages from asnychroneous requests.
         *
         * Emitts signals to @ref meshLoaded and @ref textureLoaded.
         */
        void pollAvatarMessages();

        /**
         * @brief Begin asynchroneously loading all assets for given avatar.
         * @param avatar avatar to load the assets for.
         * @return Number of assets being loaded.
         */
        Int beginLoading(ovrAvatar* avatar);

        /** @brief Signal called when an avatar specification has been retrieved */
        Signal avatarSpecificationLoaded(const ovrAvatarSpecification* avatarSpec) {
            return emit(&AvatarAssetLoader::avatarSpecificationLoaded, avatarSpec);
        }

        /** @brief Signal called when a mesh is loaded */
        Signal meshLoaded(const ovrAvatarMeshAssetData& data) {
            return emit(&AvatarAssetLoader::meshLoaded, data);
        }

        /** @brief Signal called when a texture is loaded */
        Signal textureLoaded(const ovrAvatarTextureAssetData& data) {
            return emit(&AvatarAssetLoader::textureLoaded, data);
        }
};

/** @brief Convert ovrAvatarMeshData into a Magnum::Mesh */
std::tuple<Mesh, std::unique_ptr<Buffer>, std::unique_ptr<Buffer>> toMesh(const ovrAvatarMeshAssetData& data);

/** @brief Convert ovrAvatarTextureData into a Magnum::Texture2D */
std::unique_ptr<Texture2D> toTexture2D(const ovrAvatarTextureAssetData& data);

}}}

#endif
