#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SoberUserWidget.generated.h"


UCLASS()
class SOBER_API USoberUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category = "WidgetController")
    void SetWidgetController(UObject* InWidgetController);

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<UObject> WidgetController;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetController")
    void WidgetControllerSet();
};
