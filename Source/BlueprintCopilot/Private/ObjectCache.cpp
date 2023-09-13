// Copyright (c) 2022 Ren Brauge. All rights reserved.

#include "ObjectCache.h"

void ObjectCache::UpsertBlueprint(const BlueprintID& blueprintId, UBlueprint* blueprint) noexcept
{
    _blueprintCache[blueprintId] = blueprint;
}

std::optional<UBlueprint*> ObjectCache::GetBlueprint(const BlueprintID& blueprintId) const noexcept
{
    if (_blueprintCache.find(blueprintId) == _blueprintCache.end())
    {
        return std::nullopt;
    }

    return _blueprintCache.at(blueprintId);
}

void ObjectCache::UpsertNode(const NodeID& nodeId, UK2Node* node) noexcept
{
    _nodeCache[nodeId] = node;
}

std::optional<UK2Node*> ObjectCache::GetNode(const NodeID& nodeId) const noexcept
{
    if (_nodeCache.find(nodeId) == _nodeCache.end())
    {
        return std::nullopt;
    }

    return _nodeCache.at(nodeId);
}

void ObjectCache::UpsertGraph(const GraphID& graphId, UEdGraph* graph) noexcept
{
    _graphCache[graphId] = graph;
}

std::optional<UEdGraph*> ObjectCache::GetGraph(const GraphID& graphId) const noexcept
{
    if (_graphCache.find(graphId) == _graphCache.end())
    {
        return std::nullopt;
    }

    return _graphCache.at(graphId);
}