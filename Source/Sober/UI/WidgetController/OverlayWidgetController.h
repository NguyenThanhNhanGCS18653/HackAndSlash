#pragma once

#include "CoreMinimal.h"
#include "SoberWidgetController.h"
#include "OverlayWidgetController.generated.h"


UCLASS(BlueprintType, Blueprintable)
class SOBER_API UOverlayWidgetController : public USoberWidgetController
{
	GENERATED_BODY()
	
public:
    virtual void BroadcastInitialValues() override;
    virtual void BindCallbacksToDependencies() override;

    // ---- Delegates: Widget (UMG Blueprint) bind trực tiếp vào đây ----
    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedSignature OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedSignature OnMaxHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedSignature OnStaminaChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedSignature OnMaxStaminaChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Combo")
    FOnComboSectionChangedSignature OnComboSectionChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|State")
    FOnDeathSignature OnOwnerDeath;

private:
    UFUNCTION()
    void ComboSectionUpdated(AActor* InstigatorActor, FName SectionName);

    int32 CachedComboCount = 0;
};
