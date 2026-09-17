#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "SoberAssetManager.generated.h"


UCLASS()
class SOBER_API USoberAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static USoberAssetManager& Get();

	virtual void StartInitialLoading() override;
};
