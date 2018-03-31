// Copyright 2016 Gamemakin LLC. All Rights Reserved.

#pragma once

#include "Engine/StaticMesh.h"
#include "Linters/LinterBase.h"

class FStaticMeshLinter : public FLinterBase
{
public:
	virtual bool Lint(const UObject* InObject) override;
	virtual bool HasValidUVs();

protected:
	const UStaticMesh* StaticMesh;

};