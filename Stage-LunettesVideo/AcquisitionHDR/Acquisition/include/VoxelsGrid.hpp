#ifndef VOXELSGRID_HPP_INCLUDED
#define VOXELSGRID_HPP_INCLUDED

#include "Utils.hpp"

namespace LA
{

/**
 * \class VoxelsGrid
 * \brief VoxelsGrid is class reflecting a grid of voxels.
 */
class VoxelsGrid
{
    private:
        uint m_uiWidth;  ///the width of the grid
        uint m_uiHeight; /// the height of the grid
        uint m_uiDepth; /// the depth of the grid

        // do I need to make it double??
        double  m_uiVoxelWidth; /// the size of the voxel
        double  m_uiVoxelHeight;
        double  m_uiVoxelDepth;

    public:
        uint*** pGrid; ///the grid


    private:
        ///release the used memory
        void Release();

    public:

		/// Constructor.
		VoxelsGrid(uint width, uint height, uint depth, double voxelWidth, double voxelHeight, double voxelDepth);

        /// Destructor
		virtual ~VoxelsGrid();

        /// Gets the grid width
        uint GetGridWidth();

        /// Gets the grid height
        uint GetGridHeight();

        /// Gets the grid depth
        uint GetGridDepth();

        /// Gets the voxel width
        double GetVoxelWidth();

        /// Gets the voxel height
        double GetVoxelHeight();

        /// Gets the voxel depth
        double GetVoxelDepth();

        /// Sets the voxel size
        void SetVoxelSize(double voxelWidth, double voxelHeight, double voxelDepth);

        /// Re-Initialize: fill with zeros
        void Reintialize();

};
}
#endif // VOXELSGRID_HPP_INCLUDED
