#include "EnemyCharacter.h"

#include "Sober/Core/SoberGameplayTags.h"
#include "Sober/Core/SoberAbilitySystemComponent.h"
#include "Sober/Components/WeaponCollisionComponent.h"

AEnemyCharacter::AEnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WeaponCollisionComp = CreateDefaultSubobject<UWeaponCollisionComponent>("WeaponCollisionComponent");
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    // AI-controlled -> auto give abilities ngay khi bị possess bởi AIController, không cần chờ PlayerState
    if (HasAuthority() && AttackAbilityClass)
    {
        AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AttackAbilityClass, 1, INDEX_NONE, this));
    }
}

void AEnemyCharacter::PerformAttack()
{
    FGameplayTagContainer Container;
    Container.AddTag(FSoberGameplayTags::Get().Ability_Type_Attack); // hoặc tag riêng cho enemy attack nếu bạn muốn tách biệt
    AbilitySystemComponent->TryActivateAbilitiesByTag(Container);
}