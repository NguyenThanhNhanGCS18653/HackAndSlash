#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "SoberPlayerCharacter.generated.h"

class UCameraComponent;
class USoberInputConfig;
class USpringArmComponent;
class UInputMappingContext;

UCLASS()
class SOBER_API ASoberPlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
    ASoberPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<UCameraComponent> FollowCamera;

    // Gán trong BP_PlayerCharacter — không hardcode trong C++ để Designer tự đổi input scheme
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<USoberInputConfig> InputConfig;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    int32 MappingContextPriority = 0;

    // ---- Native input handlers ----
    void Input_Move(const FInputActionValue& Value);
    void Input_Look(const FInputActionValue& Value);

    // ---- Generic ability input dispatch ----
    void Input_AbilityInputTagPressed(FGameplayTag InputTag);
    void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	
    // ---- UI ----
    void InitializeOverlay();
};
