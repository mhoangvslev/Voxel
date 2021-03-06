// Copyright 2017 Phyronnaz

#pragma once

#include "CoreMinimal.h"
#include "VoxelMaterial.h"
#include <list>

class FValueOctree;
class IVoxelWorldGenerator;

/**
 * Class that handle voxel data. Mainly an interface to ValueOctree
 */
class FVoxelData
{
public:
	/**
	 * Constructor
	 * @param	Depth			Depth of this world; Width = 16 * 2^Depth
	 * @param	WorldGenerator	Generator for this world
	 */
	FVoxelData(int Depth, UVoxelWorldGenerator* WorldGenerator, bool bMultiplayer);
	~FVoxelData();

	// Depth of the octree
	const int Depth;
	const bool bMultiplayer;

	UVoxelWorldGenerator* const WorldGenerator;

	// Size = 16 * 2^Depth
	FORCEINLINE int Size() const;

	// TODO: Doc
	void BeginSet();
	void EndSet();

	void BeginGet();
	void EndGet();

	void Reset();

	/**
	* Get value and color at position
	* @param	Position	Position in voxel space
	* @return	Value
	* @return	Color
	*/
	FORCEINLINE void GetValueAndMaterial(int X, int Y, int Z, float& OutValue, FVoxelMaterial& OutMaterial) const;
	FORCEINLINE void GetValueAndMaterial(int X, int Y, int Z, float& OutValue, FVoxelMaterial& OutMaterial, FValueOctree*& LastOctree) const;


	/**
	 * Set value at position
	 * @param	Position	Position in voxel space
	 * @param	Value to set
	 */
	FORCEINLINE void SetValue(int X, int Y, int Z, float Value);
	FORCEINLINE void SetValue(int X, int Y, int Z, float Value, FValueOctree*& LastOctree);
	/**
	 * Set color at position
	 * @param	Position	Position in voxel space
	 * @param	Color to set
	 */
	FORCEINLINE void SetMaterial(int X, int Y, int Z, FVoxelMaterial Material);
	FORCEINLINE void SetMaterial(int X, int Y, int Z, FVoxelMaterial Material, FValueOctree*& LastOctree);

	FORCEINLINE void SetValueAndMaterial(int X, int Y, int Z, float Value, FVoxelMaterial Material, FValueOctree*& LastOctree);

	/**
	 * Is Position in this world?
	 * @param	Position	Position in voxel space
	 * @return	IsInWorld
	 */
	FORCEINLINE bool IsInWorld(int X, int Y, int Z) const;

	FORCEINLINE void ClampToWorld(int& X, int& Y, int& Z) const;

	/**
	 * Get save array of this world
	 * @return SaveArray
	 */
	void GetSave(FVoxelWorldSave& OutSave) const;

	/**
	 * Load this world from save array
	 * @param	SaveArray	Array to load from
	 * @param	World		VoxelWorld
	 * @param	bReset		Reset all chunks?
	 */
	void LoadFromSaveAndGetModifiedPositions(FVoxelWorldSave& Save, std::forward_list<FIntVector>& OutModifiedPositions, bool bReset);

	/**
	 * Get sliced diff arrays to allow network transmission
	 * @param	OutValueDiffPacketsList		Each packet is sorted by Id
	 * @param	OutColorDiffPacketsList		Each packet is sorted by Id
	 */
	void GetDiffLists(std::forward_list<FVoxelValueDiff>& OutValueDiffList, std::forward_list<FVoxelMaterialDiff>& OutMaterialDiffList) const;

	/**
	 * Load values and colors from diff arrays, and queue update of chunks that have changed
	 * @param	ValueDiffArray	First element has lowest Id
	 * @param	ColorDiffArray	First element has lowest Id
	 * @param	World			Voxel world
	 */
	void LoadFromDiffListsAndGetModifiedPositions(std::forward_list<FVoxelValueDiff> ValueDiffList, std::forward_list<FVoxelMaterialDiff> MaterialDiffList, std::forward_list<FIntVector>& OutModifiedPositions);

private:
	FValueOctree* MainOctree;

	FThreadSafeCounter GetCount;
	FEvent* CanGetEvent;
	FEvent* CanSetEvent;
};
