#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoberWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class USoberAttributeSet;
class USoberAbilitySystemComponent;
class ABaseCharacter;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
    GENERATED_BODY()

    FWidgetControllerParams() {}
    FWidgetControllerParams(APlayerController* PC, ABaseCharacter* InCharacter,
        UAbilitySystemComponent* ASC, UAttributeSet* AS)
        : PlayerController(PC), OwningCharacter(InCharacter)
        , AbilitySystemComponent(ASC), AttributeSet(AS)
    {
    }

    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<APlayerController> PlayerController = nullptr;

    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<ABaseCharacter> OwningCharacter = nullptr;

    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnComboSectionChangedSignature, int32, ComboCount, FName, SectionName);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS(Abstract, BlueprintType)
class SOBER_API USoberWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category = "WidgetController")
    void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

    UFUNCTION(BlueprintCallable, Category = "WidgetController")
    virtual void BroadcastInitialValues() {}

    virtual void BindCallbacksToDependencies() {}

protected:
    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<APlayerController> PlayerController;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<ABaseCharacter> OwningCharacter;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<UAttributeSet> AttributeSet;

    // Cache sẵn dạng cast — mọi Controller con dùng thẳng, không phải Cast lại nhiều lần
    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<USoberAbilitySystemComponent> SoberASC;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<USoberAttributeSet> SoberAttributeSet;
};
