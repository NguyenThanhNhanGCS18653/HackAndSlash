#include "EnemyAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController()
{
    UAIPerceptionComponent* PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
    SetPerceptionComponent(*PerceptionComp);

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("SightConfig");
    SightConfig->SightRadius = SightRadius;
    SightConfig->LoseSightRadius = SightRadius + 200.f;
    SightConfig->PeripheralVisionAngleDegrees = 180.f; // dumb enemy: gần như thấy 360 độ, không cần né góc nhìn
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

    GetPerceptionComponent()->ConfigureSense(*SightConfig);
    GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
    GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (BehaviorTreeAsset)
    {
        RunBehaviorTree(BehaviorTreeAsset);
        GetBlackboardComponent()->SetValueAsFloat(FName("AttackRange"), AttackRange);
    }
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (!GetBlackboardComponent()) return;

    // Dumb: thấy là chase ngay lập tức, không kiểm tra loại actor, không có "vùng nghi ngờ"
    if (Stimulus.WasSuccessfullySensed())
    {
        GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), Actor);
    }
    else
    {
        GetBlackboardComponent()->ClearValue(FName("TargetActor"));
    }
}
