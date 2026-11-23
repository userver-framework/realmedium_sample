#include "jwt.hpp"

#include <stdexcept>
#include <utility>

#include <userver/components/component_config.hpp>
#include <userver/crypto/jwt.hpp>
#include <userver/formats/json/inline.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

namespace real_medium::auth {

Jwt::Jwt(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
    : ComponentBase(config, context), keys_(ReadKeys(config)) {}

Jwt::Keys Jwt::ReadKeys(const userver::components::ComponentConfig& config) {
    auto secret = config["secret"].As<std::string>();
    if (secret.size() < 32) {
        throw std::runtime_error("jwt.secret must contain at least 32 bytes for HS256");
    }
    return {userver::crypto::SignerHs256{secret}, userver::crypto::VerifierHs256{std::move(secret)}};
}

std::string Jwt::MakeToken(std::string_view user_id) const {
    return userver::crypto::jwt::MakeToken(userver::formats::json::MakeObject("id", user_id), keys_.signer);
}

std::optional<std::string> Jwt::VerifyUserId(std::string_view token) const {
    userver::formats::json::Value payload;
    try {
        payload = userver::crypto::jwt::Verify(
            token, keys_.verifier, {.expiration = userver::crypto::jwt::ExpirationPolicy::kOptional}
        ).payload;
    } catch (const userver::crypto::VerificationError&) {
        return std::nullopt;
    }
    const auto id = payload["id"];
    if (!id.IsString()) {
        return std::nullopt;
    }
    return id.As<std::string>();
}

userver::yaml_config::Schema Jwt::GetStaticConfigSchema() {
    return userver::yaml_config::MergeSchemas<userver::components::ComponentBase>(R"(
type: object
description: Issues and verifies user tokens with HS256
additionalProperties: false
required: [secret]
properties:
    secret:
        type: string
        description: Shared HS256 secret of at least 32 bytes
)");
}

}  // namespace real_medium::auth
