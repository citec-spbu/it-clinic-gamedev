#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "Hero_Base.generated.h"


//-------------------------------------------------------------------------------------------------------------
UCLASS()
class MUSICGAME_API AHero_Base : public APaperZDCharacter {
	GENERATED_BODY()

public:
	AHero_Base();

protected:
	UFUNCTION(BlueprintCallable, Category = "Hero")
	void Count_Combo();

	UPROPERTY(BlueprintReadWrite)
	double damage;

	UPROPERTY(BlueprintReadWrite)
	double timer;

	UPROPERTY(BlueprintReadWrite)
	TArray<double> timer_array;

	UPROPERTY(BlueprintReadWrite)
	bool is_reinforced = false;

};
//-------------------------------------------------------------------------------------------------------------
