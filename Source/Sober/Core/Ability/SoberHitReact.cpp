#include "SoberHitReact.h"

#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include "Sober/Core/SoberGameplayTags.h"

USoberHitReact::USoberHitReact()
{
    FSoberGameplayTags GameplayTags = FSoberGameplayTags::Get();
    AbilityTags.AddTag(GameplayTags.Ability_HitReact);
    ActivationOwnedTags.AddTag(GameplayTags.State_HitReacting);

    // Hit React được phép ngắt combo đang chạy của chính Target (bị đánh thì phải khựng lại)
    // nhưng KHÔNG bị chặn bởi State.HyperArmor của chính ability này (đây là ability của TARGET, không phải attacker)
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USoberHitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    if (!Character || !HitReactMontage)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // ---- Knockback: lấy hướng từ Instigator (kẻ tấn công) tới Target (bản thân) ----
    if (TriggerEventData)
    {
        const FVector InstigatorLocation = TriggerEventData->ContextHandle.GetOrigin();
        const FVector TargetLocation = Character->GetActorLocation();
        FVector KnockbackDirection = (TargetLocation - InstigatorLocation).GetSafeNormal2D();

        const float Force = TriggerEventData->EventMagnitude > 0.f ? TriggerEventData->EventMagnitude : 400.f;
        const FVector LaunchVelocity = KnockbackDirection * Force + FVector(0.f, 0.f, KnockbackZOffset);

        Character->LaunchCharacter(LaunchVelocity, true, false);
    }

    // ---- Phát animation hit react ----
    MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
        this, NAME_None, HitReactMontage, 1.f);
    MontageTask->OnCompleted.AddDynamic(this, &USoberHitReact::OnHitReactFinished);
    MontageTask->OnInterrupted.AddDynamic(this, &USoberHitReact::OnHitReactFinished);
    MontageTask->OnCancelled.AddDynamic(this, &USoberHitReact::OnHitReactFinished);
    MontageTask->ReadyForActivation();
}

void USoberHitReact::OnHitReactFinished()
{
    EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, GetCurrentActivationInfo(), true, false);
}
