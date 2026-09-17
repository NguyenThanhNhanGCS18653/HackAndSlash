#include "SoberGameplayAbility.h"

#include "Sober/Characters/BaseCharacter.h"
#include "Sober/Core/SoberAbilitySystemComponent.h"

USoberGameplayAbility::USoberGameplayAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

USoberAbilitySystemComponent* USoberGameplayAbility::GetSoberASC() const
{
    return Cast<USoberAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
}

ABaseCharacter* USoberGameplayAbility::GetOwningBaseCharacter() const
{
    return Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
}
