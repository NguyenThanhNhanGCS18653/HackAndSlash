#include "SoberHUD.h"

#include "Sober/UI/Widget/SoberUserWidget.h"
#include "Sober/UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* ASoberHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
    if (OverlayWidgetController == nullptr)
    {
        OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
        OverlayWidgetController->SetWidgetControllerParams(WCParams);
        OverlayWidgetController->BindCallbacksToDependencies();
    }
    return OverlayWidgetController;
}

void ASoberHUD::InitOverlay(APlayerController* PC, ABaseCharacter* OwningCharacter, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
    checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass chưa gán trong BP_YourGameHUD"));
    checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass chưa gán trong BP_YourGameHUD"));

    UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
    OverlayWidget = Cast<USoberUserWidget>(Widget);

    const FWidgetControllerParams WCParams(PC, OwningCharacter, ASC, AS);
    UOverlayWidgetController* Controller = GetOverlayWidgetController(WCParams);

    OverlayWidget->SetWidgetController(Controller);
    Controller->BroadcastInitialValues();

    Widget->AddToViewport();
}