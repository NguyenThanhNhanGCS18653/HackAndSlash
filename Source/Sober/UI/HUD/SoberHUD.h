#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SoberHUD.generated.h"

class UAttributeSet;
class ABaseCharacter;
class UAbilitySystemComponent;
class USoberUserWidget;
class UOverlayWidgetController;

struct FWidgetControllerParams;

UCLASS()
class SOBER_API ASoberHUD : public AHUD
{
	GENERATED_BODY()
	
public:
    UFUNCTION()
    void InitOverlay(APlayerController* PC, ABaseCharacter* OwningCharacter,
        UAbilitySystemComponent* ASC, UAttributeSet* AS);

    UFUNCTION()
    UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

    // Chỗ mở rộng sẵn cho sau này — chỉ cần thêm 1 hàm Get tương tự khi có màn hình mới
    // UFUNCTION()
    // UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);

private:
    UPROPERTY()
    TObjectPtr<USoberUserWidget> OverlayWidget;

    UPROPERTY(EditAnywhere, Category = "HUD")
    TSubclassOf<USoberUserWidget> OverlayWidgetClass;

    UPROPERTY()
    TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

    UPROPERTY(EditAnywhere, Category = "HUD")
    TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
