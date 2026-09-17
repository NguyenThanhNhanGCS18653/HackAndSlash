#include "SoberInputConfig.h"

UInputAction* USoberInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
    for (const FYourGameInputAction& Action : NativeInputActions)
    {
        if (Action.InputTag == InputTag && Action.InputAction)
        {
            return Action.InputAction;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("[InputConfig] Không tìm thấy Native InputAction cho tag %s"), *InputTag.ToString());
    return nullptr;
}

UInputAction* USoberInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
    for (const FYourGameInputAction& Action : AbilityInputActions)
    {
        if (Action.InputTag == InputTag && Action.InputAction)
        {
            return Action.InputAction;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("[InputConfig] Không tìm thấy Ability InputAction cho tag %s"), *InputTag.ToString());
    return nullptr;
}
