#include "AnimNotifyState_WeaponTrace.h"

#include "GameFramework/Actor.h"

#include "Sober/Components/WeaponCollisionComponent.h"

void UAnimNotifyState_WeaponTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    if (AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr)
    {
        if (UWeaponCollisionComponent* WeaponComp = Owner->FindComponentByClass<UWeaponCollisionComponent>())
        {
            WeaponComp->StartTrace();
        }
    }
}

void UAnimNotifyState_WeaponTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr)
    {
        if (UWeaponCollisionComponent* WeaponComp = Owner->FindComponentByClass<UWeaponCollisionComponent>())
        {
            WeaponComp->StopTrace();
        }
    }
}
