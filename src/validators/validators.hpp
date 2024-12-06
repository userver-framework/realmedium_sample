#pragma once

#include <docs/api/api.hpp>

namespace real_medium::validator {

void validate(const handlers::UserLoginDTO& dto);

void validate(const handlers::UserRegistrationDTO& dto);

void validate(const handlers::UserUpdateDTO& dto);

void validate(const handlers::AddComment& dto);

void validate(const handlers::CreateArticleRequest& dto);

void validate(const handlers::UpdateArticleRequest& dto);

}  // namespace real_medium::validator
