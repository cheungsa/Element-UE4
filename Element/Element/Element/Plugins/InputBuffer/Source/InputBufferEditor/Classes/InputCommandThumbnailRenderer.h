// Copyright 2017 Isaac Hsu.

#pragma once

#include "ThumbnailRendering/DefaultSizedThumbnailRenderer.h"

#include "InputCommandThumbnailRenderer.generated.h"

UCLASS()
class INPUTBUFFEREDITOR_API UInputCommandThumbnailRenderer : public UDefaultSizedThumbnailRenderer
{
	GENERATED_BODY()

public:

	UInputCommandThumbnailRenderer();

	// UThumbnailRenderer interface
	virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget*, FCanvas* Canvas) override;
	// End of UThumbnailRenderer interface

protected:

	static const float ELLIPSIS_HEIGHT_RATIO;

	UPROPERTY()
	UTexture2D* DefaultTexture;

protected:

	void DrawTexture(class UTexture2D* Texture, int32 X, int32 Y, uint32 Width, uint32 Height, FCanvas* Canvas);
};
