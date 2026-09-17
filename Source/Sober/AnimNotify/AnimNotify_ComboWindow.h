#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_ComboWindow.generated.h"


UCLASS()
class SOBER_API UAnimNotify_ComboWindow : public UAnimNotify
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, Category = "Combo")
    FGameplayTag EventTag; // Event.Combo.WindowOpen hoặc WindowClose

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
        const FAnimNotifyEventReference& EventReference) override;

    virtual FString GetNotifyName_Implementation() const override;
};
