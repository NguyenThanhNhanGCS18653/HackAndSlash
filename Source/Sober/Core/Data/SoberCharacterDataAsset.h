#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayEffect.h"
#include "GameplayAbilitySpec.h"
#include "SoberCharacterDataAsset.generated.h"

UCLASS(BlueprintType)
class SOBER_API USoberCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
    // GE dạng Instant, set base value bằng Curve Table hoặc SetByCaller -> trỏ tới GE_InitAttributes_BP
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
    TSubclassOf<UGameplayEffect> InitAttributesEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
    float BaseHealth = 100.f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
    float BaseMaxHealth = 100.f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
    float BaseStamina = 100.f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
    float BaseMaxStamina = 100.f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
    float BaseAttackPower = 10.f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
    float BaseArmor = 0.f;

    // Mở rộng sau: Level scaling curve — nhân với BaseX theo Level enemy
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scaling")
    UCurveFloat* HealthScalingByLevel;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
    TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

    // Passive effect luôn active (ví dụ regen stamina)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
    TArray<TSubclassOf<UGameplayEffect>> DefaultPassiveEffects;
};
