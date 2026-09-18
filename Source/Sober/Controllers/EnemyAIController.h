#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"


UCLASS()
class SOBER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
    AEnemyAIController();

protected:
    virtual void OnPossess(APawn* InPawn) override;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    class UBehaviorTree* BehaviorTreeAsset;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float AttackRange = 150.f;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float SightRadius = 800.f;

private:
    UPROPERTY() class UAISenseConfig_Sight* SightConfig;

    UFUNCTION() void OnTargetPerceptionUpdated(AActor* Actor, struct FAIStimulus Stimulus);
};
