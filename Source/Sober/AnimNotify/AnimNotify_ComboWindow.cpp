#include "AnimNotify_ComboWindow.h"

#include "GameFramework/Actor.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UAnimNotify_ComboWindow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
    const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    AActor* OwnerActor = MeshComp ? MeshComp->GetOwner() : nullptr;
    if (!OwnerActor) return;

    UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor);
    if (!ASC) return;

    // Gửi GameplayEvent — Ability đang chạy sẽ nhận qua AbilityTask_WaitGameplayEvent
    FGameplayEventData EventData;
    EventData.EventTag = EventTag;
    EventData.Instigator = OwnerActor;

    ASC->HandleGameplayEvent(EventTag, &EventData);
}

FString UAnimNotify_ComboWindow::GetNotifyName_Implementation() const
{
    return EventTag.IsValid() ? EventTag.ToString() : TEXT("ComboWindow");
}

