/*
SDLEngine
Matt Hoyle
*/

namespace Vox
{
	template< class VoxelDataType, uint32_t t_blockDimensionVx >
	Model<VoxelDataType, t_blockDimensionVx>::Model()
	{
	}

	template< class VoxelDataType, uint32_t t_blockDimensionVx >
	Model<VoxelDataType, t_blockDimensionVx>::~Model()
	{

	}

	template< class VoxelDataType, uint32_t t_blockDimensionVx >
	void Model<VoxelDataType, t_blockDimensionVx>::SetModelExtents(Math::Box3 maxBounds, glm::vec3 voxelSize)
	{
		m_maxDimensions = maxBounds;
		m_voxelDimensions = voxelSize;
		
		// test it fits, we can probably do this better
		glm::vec3 boundsSize = maxBounds.Size();
		SDE_ASSERT(boundsSize.x > voxelSize.x && boundsSize.y > voxelSize.y && boundsSize.z > voxelSize.z);

		m_blockDimensions = m_voxelDimensions * 2.0f * (float)BlockType::BlockDimensions;
	}

	template< class VoxelDataType, uint32_t t_blockDimensionVx >
	void Model<VoxelDataType, t_blockDimensionVx>::BlockCoordToPageIndices(glm::vec3 coordinate, glm::ivec3& result)
	{
		glm::vec3 diffAbs = coordinate - m_maxDimensions.Min();					// 0 to length(maxDimensions.x)
		glm::vec3 firstBlockAbs = glm::floor(diffAbs / m_blockDimensions);		// 0 to length, rounded down to block containing the point
		result = firstBlockAbs;
	}

	template< class VoxelDataType, uint32_t t_blockDimensionVx >
	void Model<VoxelDataType, t_blockDimensionVx>::BlockRangeIntersectingBounds(Math::Box3 boundsToSearch, glm::ivec3& startRange, glm::ivec3& endRange)
	{
		// Clamp to model extents
		glm::vec3 clampedMinBounds = glm::clamp(boundsToSearch.Min(), m_maxDimensions.Min(), m_maxDimensions.Max());
		glm::vec3 clampedMaxBounds = glm::clamp(boundsToSearch.Max(), m_maxDimensions.Min(), m_maxDimensions.Max());

		BlockCoordToPageIndices(clampedMinBounds, startRange);
		BlockCoordToPageIndices(clampedMaxBounds, endRange);
	}

	template< class VoxelDataType, uint32_t t_blockDimensionVx >
	void Model<VoxelDataType, t_blockDimensionVx>::WriteData(Math::Box3 modelSpaceBounds)
	{
		if (!modelSpaceBounds.Intersects(m_maxDimensions))
		{
			return;
		}

		// Find block indices for iteration
		glm::ivec3 startBlockIndices, endBlockIndices;
		BlockRangeIntersectingBounds(modelSpaceBounds, startBlockIndices, endBlockIndices);

		// For each block that matches, find intersecting clumps
		for (int32_t bx = startBlockIndices.x; bx <= endBlockIndices.x; ++bx)
		{
			for (int32_t by = startBlockIndices.y; by <= endBlockIndices.y; ++by)
			{
				for (int32_t bz = startBlockIndices.z; bz <= endBlockIndices.z; ++bz)
				{
				}
			}
		}
	}
}