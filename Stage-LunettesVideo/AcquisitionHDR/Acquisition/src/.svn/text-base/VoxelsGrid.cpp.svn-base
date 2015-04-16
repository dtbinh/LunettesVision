#include "VoxelsGrid.hpp"

using namespace LA;

VoxelsGrid::VoxelsGrid(uint width, uint height, uint depth, double voxelWidth, double voxelHeight, double voxelDepth)
{
    m_uiWidth = width;
    m_uiHeight = height;
    m_uiDepth = depth;

    m_uiVoxelWidth = voxelWidth;
    m_uiVoxelHeight = voxelHeight;
    m_uiVoxelDepth = voxelDepth;

    pGrid = new uint**[m_uiHeight];
    for (int i = 0; i < m_uiHeight; ++i)
    {

        pGrid[i] = new uint*[m_uiWidth];

        for (int j = 0; j < m_uiWidth; ++j)
            pGrid[i][j] = new uint[m_uiDepth];

    }

    for (int i = 0; i < m_uiHeight; ++i)
            for (int j = 0; j < m_uiWidth; ++j)
                for (int k = 0; k < m_uiDepth; ++k)
                    pGrid[i][j][k] = 0;
}

VoxelsGrid::~VoxelsGrid()
{
    Release();
}

void VoxelsGrid::Release()
{
   for (int i = 0; i < m_uiHeight; ++i)
  {

        for (int j = 0; j < m_uiWidth; ++j)
            delete [] pGrid[i][j];

        delete [] pGrid[i];
  }
  delete [] pGrid;

}

uint VoxelsGrid::GetGridWidth()
{
    return m_uiWidth;
}

uint VoxelsGrid::GetGridHeight()
{
    return m_uiHeight;
}

uint VoxelsGrid::GetGridDepth()
{
    return m_uiDepth;
}

double VoxelsGrid::GetVoxelWidth()
{
    return m_uiVoxelWidth;
}

double VoxelsGrid::GetVoxelHeight()
{
    return m_uiVoxelHeight;
}

double VoxelsGrid::GetVoxelDepth()
{
    return m_uiVoxelDepth;
}


   void VoxelsGrid::SetVoxelSize(double voxelWidth, double voxelHeight, double voxelDepth)
{
    m_uiVoxelWidth = voxelWidth;
    m_uiVoxelHeight = voxelHeight;
    m_uiVoxelDepth = voxelDepth;
}


void VoxelsGrid::Reintialize()
{
     for (int i = 0; i < m_uiHeight; ++i)
            for (int j = 0; j < m_uiWidth; ++j)
                for (int k = 0; k < m_uiDepth; ++k)
                    pGrid[i][j][k] = 0;

}

