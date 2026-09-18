#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnemyCharacter.generated.h"

class UWeaponCollisionComponent;

UCLASS()
class SOBER_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
    AEnemyCharacter(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void PerformAttack();

protected:
    virtual void PossessedBy(AController* NewController) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    TObjectPtr<UWeaponCollisionComponent> WeaponCollisionComp;

    // Ability chứa animation tấn công đơn giản của enemy — set trong CharacterData hoặc trực tiếp ở đây
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
    TSubclassOf<class UGameplayAbility> AttackAbilityClass;
};
