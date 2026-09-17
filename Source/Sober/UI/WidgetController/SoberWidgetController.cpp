#include "SoberWidgetController.h"

#include "Sober/Core/SoberAttributeSet.h"
#include "Sober/Characters/BaseCharacter.h"
#include "Sober/Core/SoberAbilitySystemComponent.h"

void USoberWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
    PlayerController = WCParams.PlayerController;
    OwningCharacter = WCParams.OwningCharacter;
    AbilitySystemComponent = WCParams.AbilitySystemComponent;
    AttributeSet = WCParams.AttributeSet;

    SoberASC = Cast<USoberAbilitySystemComponent>(AbilitySystemComponent);
    SoberAttributeSet = Cast<USoberAttributeSet>(AttributeSet);
}
