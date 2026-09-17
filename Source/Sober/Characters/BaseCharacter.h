#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Sober/Core/Data/SoberCharacterDataAsset.h"
#include "BaseCharacter.generated.h"

class USoberAttributeSet;
class USoberAbilitySystemComponent;
class USoberCharacterDataAsset;

UCLASS()
class SOBER_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
    ABaseCharacter(const FObjectInitializer& ObjectInitializer);

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    UFUNCTION(BlueprintPure, Category = "Attributes")
    float GetHealth() const;
    UFUNCTION(BlueprintPure, Category = "Attributes")
    float GetMaxHealth() const;
    UFUNCTION(BlueprintPure, Category = "Attributes")
    float GetStamina() const;
    UFUNCTION(BlueprintPure, Category = "Attributes")
    float GetMaxStamina() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
    TObjectPtr<USoberAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
    TObjectPtr<USoberAttributeSet> AttributeSet;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
    TObjectPtr<USoberCharacterDataAsset> CharacterData;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    int32 CharacterLevel = 1;

protected:
    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

    // Virtual để subclass (Player/Enemy) override init khác nhau nếu cần (ví dụ Enemy dùng level scaling)
    virtual void InitializeAbilitySystem();
    virtual void GiveDefaultAbilities();
    virtual void ApplyDefaultEffects();
    virtual float CalculateScaledValue(float BaseValue, UCurveFloat* ScalingCurve, int32 Level) const;

    UFUNCTION() virtual void HandleDeath(AActor* OwningActor);

private:
    bool bAbilitiesGiven = false;
};
