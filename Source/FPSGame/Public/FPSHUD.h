// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSHUD.generated.h"

class UTexture2D;

UCLASS()
class AFPSHUD : public AHUD
{
	GENERATED_BODY()

protected:

	/** Crosshair asset pointer */
	UTexture2D* CrosshairTex;

public:

	AFPSHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

};

