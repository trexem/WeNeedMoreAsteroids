#include "CollisionSystem.h"
#include "utils.hpp"
#include "GameStateManager.h"
#include "SpatialGridBuilder.h"

#include <iostream>

void CollisionSystem::update(EntityManager& eMgr, const double& dT) {
    if (GameStateManager::instance().getState() == GameState::Playing) {
        std::unordered_map<std::pair<int, int>, std::vector<uint32_t>, pair_hash> spatialGrid;
        spatialGrid = SpatialGrid::instance().getGrid();
        // std::cout << "SpatialGrid count: " << spatialGrid.size() << std::endl;
        for (auto& cell : spatialGrid) {
            auto& entities = cell.second;

            for (size_t i = 0; i < entities.size(); ++i) {
                uint32_t a = entities[i];
                if (eMgr.isDestroyed(a) || eMgr.isMarkedForDestruction(a)) continue;
                TransformComponent transA = eMgr.getComponentData<TransformComponent>(a);
                CollisionComponent colA = eMgr.getComponentData<CollisionComponent>(a);
                TypeComponent* typeA = eMgr.getComponentDataPtr<TypeComponent>(a);
                if (!typeA) continue;

                for (size_t j = i + 1; j < entities.size(); ++j) {
                    uint32_t b = entities[j];
                    if (eMgr.isDestroyed(b) || eMgr.isMarkedForDestruction(b)) continue;
                    TransformComponent transB = eMgr.getComponentData<TransformComponent>(b);
                    CollisionComponent colB = eMgr.getComponentData<CollisionComponent>(b);
                    TypeComponent* typeB = eMgr.getComponentDataPtr<TypeComponent>(b);
                    if (!typeB) continue;
                    EntityType tA = typeA->type;
                    EntityType tB = typeB->type;
                    if (TypesSet::shouldIgnoreCollision(tA, tB)) {
                        continue;
                    }
                    // std::cout << "Checking Collision for: " << tA << " and " << tB << std::endl;
                    if (checkCollision(transA, colA, transB, colB)) {
                        // std::cout << "Collision detected" << std::endl;
                        if (TypesSet::shouldPlayerPick(tA, tB)) {
                            uint32_t player = tA & EntityType::Player ? a : b;
                            uint32_t pickupID = player == a ? b : a;
                            EntityType pType = pickupID == a ? tA : tB;
                            PickupComponent* pComp = eMgr.getComponentDataPtr<PickupComponent>(pickupID);
                            auto msg = std::make_shared<PickupPickedMessage>(a, pComp->value, pType);
                            eMgr.destroyEntityLater(b);
                            MessageManager::instance().sendMessage(msg);
                            continue;
                        } else if (TypesSet::sameType(EntityType::Asteroid, tA, tB)) {
                            std::vector<uint32_t> ids;
                            ids.push_back(a);
                            ids.push_back(b);
                            auto msg = std::make_shared<AsteroidAsteroidCollisionMessage>(ids);
                            MessageManager::instance().sendMessage(msg);
                            continue;
                        }
                        std::vector<uint32_t> ids;
                        ids.push_back(a);
                        ids.push_back(b);
                        auto msg = std::make_shared<CollisionMessage>(ids);
                        MessageManager::instance().sendMessage(msg);
                    }
                }
            }
        }
    }
}

bool CollisionSystem::checkCollision(
    const TransformComponent& transA, const CollisionComponent& colA,
    const TransformComponent& transB, const CollisionComponent& colB
) {
    // Precise shape-specific checks
    if (colA.shape == Shape::Circle && 
        colB.shape == Shape::Circle) {
        // Circle-circle
        float distSq = getSquaredDistanceBetweenCircles(transA.position, colA.radius, transB.position, colB.radius);
        const float combinedRadius  = colA.radius + colB.radius;
        return distSq <= combinedRadius  * combinedRadius ;
    }
    else if (colA.shape == Shape::Circle || colB.shape == Shape::Circle) {
        if (colA.shape == Shape::Circle) {
            return checkCircleOBB(colA, transA, colB, transB);
        }
        return checkCircleOBB(colB, transB, colA, transA);
    } else {
        auto cornersA = getCorners(transA, colA);
        auto cornersB = getCorners(transB, colB);
        return checkOBBCollision(cornersA, cornersB);
    }
}

bool CollisionSystem::checkOBBCollision(
    const std::array<FPair, 4>& cornersA,
    const std::array<FPair, 4>& cornersB
) {
    std::array<FPair, 4> axes;
    // Axes from box A
    axes[0] = {cornersA[1].x - cornersA[0].x, cornersA[1].y - cornersA[0].y};
    axes[1] = {cornersA[3].x - cornersA[0].x, cornersA[3].y - cornersA[0].y};
    // Axes from box B
    axes[2] = {cornersB[1].x - cornersB[0].x, cornersB[1].y - cornersB[0].y};
    axes[3] = {cornersB[3].x - cornersB[0].x, cornersB[3].y - cornersB[0].y};
    // Normalize all axes
    for (auto& axis : axes) {
        axis.normalize();
    }
    // Test each axis
    for (const auto& axis : axes) {
        // Project all points of A onto axis
        float minA = INFINITY, maxA = -INFINITY;
        for (const auto& corner : cornersA) {
            float projection = corner.x * axis.x + corner.y * axis.y;
            minA = std::min(minA, projection);
            maxA = std::max(maxA, projection);
        }
        
        // Project all points of B onto axis
        float minB = INFINITY, maxB = -INFINITY;
        for (const auto& corner : cornersB) {
            float projection = corner.x * axis.x + corner.y * axis.y;
            minB = std::min(minB, projection);
            maxB = std::max(maxB, projection);
        }
        
        // Check for overlap
        if (maxA < minB || maxB < minA) {
            return false; // Found a separating axis
        }
    }
    
    return true; // No separating axis found
}

bool CollisionSystem::checkCircleOBB(
    const CollisionComponent& circle, const TransformComponent& circleTrans,
    const CollisionComponent& obb, const TransformComponent& obbTrans
) {
    // Get OBB corners
    auto obbCorners = getCorners(obbTrans, obb);
    
    // Circle center in world space
    FPair circleCenter = {
        circleTrans.position.x + circle.position.x + circle.radius,
        circleTrans.position.y + circle.position.y + circle.radius
    };
    
    // For each edge of OBB, project circle center onto edge
    float minDistSq = INFINITY;

    auto squaredDistance = [](const FPair& a, const FPair& b) -> float {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        return dx * dx + dy * dy;
    };

    auto closestPointOnEdge = [](const FPair& a, const FPair& b, const FPair& point) -> FPair {
        FPair ab = {b.x - a.x, b.y - a.y};
        float abLengthSq = ab.x * ab.x + ab.y * ab.y;
        if (abLengthSq == 0.0f) return a;

        FPair ap = {point.x - a.x, point.y - a.y};
        float projection = (ap.x * ab.x + ap.y * ab.y) / abLengthSq;
        projection = std::clamp(projection, 0.0f, 1.0f);

        return {
            a.x + projection * ab.x,
            a.y + projection * ab.y
        };
    };
    
    // Check against each edge of OBB
    for (int i = 0; i < 4; i++) {
        int j = (i + 1) % 4;
        FPair pointOnEdge = closestPointOnEdge(obbCorners[i], obbCorners[j], circleCenter);
        float distSq = squaredDistance(circleCenter, pointOnEdge);
        if (distSq < minDistSq) {
            minDistSq = distSq;
        }
    }
    
    
    return minDistSq <= (circle.radius * circle.radius);
}
