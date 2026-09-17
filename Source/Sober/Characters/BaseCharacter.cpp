#include "BaseCharacter.h"
#include "GameFramework/PlayerState.h"

#include "Sober/Core/SoberGameplayTags.h"
#include "Sober/Core/SoberAttributeSet.h"
#include "Sober/Core/SoberAbilitySystemComponent.h"

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    AbilitySystemComponent = CreateDefaultSubobject<USoberAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<USoberAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    InitializeAbilitySystem();
}

void ABaseCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();
    InitializeAbilitySystem();
}

void ABaseCharacter::InitializeAbilitySystem()
{
    if (!AbilitySystemComponent) return;

    AbilitySystemComponent->InitAbilityActorInfo(this, this);
    AbilitySystemComponent->SetupTagListeners();


    ApplyDefaultEffects();

    if (!bAbilitiesGiven && HasAuthority())
    {
        GiveDefaultAbilities();
        bAbilitiesGiven = true;
    }
}

void ABaseCharacter::ApplyDefaultEffects()
{
    if (!CharacterData || !HasAuthority()) return;

    FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
    Context.AddSourceObject(this);

    if (CharacterData->InitAttributesEffect)
    {
        FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
            CharacterData->InitAttributesEffect, static_cast<float>(CharacterLevel), Context);

        if (SpecHandle.IsValid())
        {
            SpecHandle.Data->SetSetByCallerMagnitude(FSoberGameplayTags::Get().SetByCaller_Health, CharacterData->BaseHealth);
            SpecHandle.Data->SetSetByCallerMagnitude(FSoberGameplayTags::Get().SetByCaller_MaxHealth, CharacterData->BaseMaxHealth);
            SpecHandle.Data->SetSetByCallerMagnitude(FSoberGameplayTags::Get().SetByCaller_Stamina, CharacterData->BaseStamina);
            SpecHandle.Data->SetSetByCallerMagnitude(FSoberGameplayTags::Get().SetByCaller_MaxStamina, CharacterData->BaseMaxStamina);
            SpecHandle.Data->SetSetByCallerMagnitude(FSoberGameplayTags::Get().SetByCaller_AttackPower, CharacterData->BaseAttackPower);
            SpecHandle.Data->SetSetByCallerMagnitude(FSoberGameplayTags::Get().SetByCaller_Armor, CharacterData->BaseArmor);

            AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }

    for (const TSubclassOf<UGameplayEffect>& PassiveEffect : CharacterData->DefaultPassiveEffects)
    {
        if (!PassiveEffect) continue;
        FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(PassiveEffect, 1.f, Context);
        if (Spec.IsValid())
        {
            AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
        }
    }
}

float ABaseCharacter::CalculateScaledValue(float BaseValue, UCurveFloat* ScalingCurve, int32 Level) const
{
    if (!ScalingCurve) return BaseValue;
    return BaseValue * ScalingCurve->GetFloatValue(static_cast<float>(Level));
}

void ABaseCharacter::GiveDefaultAbilities()
{
    if (!CharacterData) return;

    for (const TSubclassOf<UGameplayAbility>& AbilityClass : CharacterData->DefaultAbilities)
    {
        if (!AbilityClass) continue;
        AbilitySystemComponent->GiveAbility(
            FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
    }
}

void ABaseCharacter::HandleDeath(AActor* OwningActor)
{
    AbilitySystemComponent->AddLooseGameplayTag(FSoberGameplayTags::Get().State_Dead);
    // Ragdoll / disable input / trigger death montage — làm ở subclass hoặc BlueprintImplementableEvent
}

float ABaseCharacter::GetHealth() const { return AttributeSet ? AttributeSet->GetHealth() : 0.f; }
float ABaseCharacter::GetMaxHealth() const { return AttributeSet ? AttributeSet->GetMaxHealth() : 0.f; }
float ABaseCharacter::GetStamina() const { return AttributeSet ? AttributeSet->GetStamina() : 0.f; }
float ABaseCharacter::GetMaxStamina() const { return AttributeSet ? AttributeSet->GetMaxStamina() : 0.f; }

