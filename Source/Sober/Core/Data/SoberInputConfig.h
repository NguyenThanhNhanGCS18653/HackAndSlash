#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "GameplayTagContainer.h"
#include "SoberInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FYourGameInputAction
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> InputAction = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (Categories = "Input"))
    FGameplayTag InputTag;
};


UCLASS(BlueprintType, Const)
class SOBER_API USoberInputConfig : public UDataAsset
{
	GENERATED_BODY()
	

public:
    // Input thuần movement/camera — xử lý trực tiếp trong PlayerCharacter, không qua ASC
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Native")
    TArray<FYourGameInputAction> NativeInputActions;

    // Input liên quan Ability — đi qua ASC->AbilityInputTagPressed/Released
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Ability")
    TArray<FYourGameInputAction> AbilityInputActions;

    UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag) const;
    UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag) const;
};
