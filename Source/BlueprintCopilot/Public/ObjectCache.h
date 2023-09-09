// Copyright (c) 2022 Ren Brauge. All rights reserved.
#pragma once

#include "Engine/Blueprint.h"
#include "K2Node.h"

#include <optional>
#include <string>
#include <unordered_map>

using BlueprintID = std::string;
using NodeID      = std::string;

/// @brief Because there could be multiple nodes for a same property, we need to keep an internal cache so that the
/// instructions knows which specific node they want. Internal dynamic data of UE, such as a pointer are not available
/// at the moment of asking GPT
class ObjectCache
{
private:
    // TODO only accept gsl_not_null_ptr
    std::unordered_map<BlueprintID, UBlueprint*> _blueprintCache{};
    std::unordered_map<NodeID, UK2Node*> _nodeCache{};

public:
    void UpsertBlueprint(const BlueprintID& blueprintId, UBlueprint* blueprint) noexcept;
    std::optional<UBlueprint*> GetBlueprint(const BlueprintID& blueprintId) const noexcept;
    void UpsertNode(const NodeID& nodeId, UK2Node* node) noexcept;
    std::optional<UK2Node*> GetNode(const NodeID& nodeId) const noexcept;
};