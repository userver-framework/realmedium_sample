#pragma once

#include <optional>
#include <string>
#include <string_view>

#include <userver/components/component_base.hpp>
#include <userver/crypto/signers.hpp>
#include <userver/crypto/verifiers.hpp>

namespace real_medium::auth {

class Jwt final : public userver::components::ComponentBase {
public:
    static constexpr std::string_view kName = "jwt";

    Jwt(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context);

    /// Issue a token with the user id in the "id" claim.
    std::string MakeToken(std::string_view user_id) const;

    /// Return the verified user id, or nullopt for an invalid token. A missing expiration claim is allowed.
    std::optional<std::string> VerifyUserId(std::string_view token) const;

    static userver::yaml_config::Schema GetStaticConfigSchema();

private:
    struct Keys {
        userver::crypto::SignerHs256 signer;
        userver::crypto::VerifierHs256 verifier;
    };

    static Keys ReadKeys(const userver::components::ComponentConfig& config);

    const Keys keys_;
};

}  // namespace real_medium::auth
