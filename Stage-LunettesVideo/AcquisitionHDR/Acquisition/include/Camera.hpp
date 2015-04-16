#ifndef	__CAMERA_HPP__
#define	__CAMERA_HPP__

#include "Utils.hpp"

typedef struct
{
	float k [ 4 ];
	float A [ 16 ];
}InternalParameters;

typedef struct
{
	float R [ 9 ];
	float T [ 3 ];
}ExternalParameters;

typedef struct
{
	float frustum[ 6 ];
	float direction[ 3 ];
}CameraGL;
namespace LA
{

class Camera
{
	public:
		Camera() {}
		Camera( const char* pInternalParameters, const char* pExternalParameters )
		{
			Create( pInternalParameters, pExternalParameters );
		}
		void Load( const char* pInternalParameters, const char* pExternalParameters )
		{
			FILE* f;
			f = fopen ( pInternalParameters, "r" );
			if ( f != NULL ) {
				char str [ 512 ];
				fgets ( str, 512, f );
				fgets ( str, 512, f ); sscanf ( str, "%f %f %f %f", &( m_pInternalParameters.k [ 0 ] ), &( m_pInternalParameters.k [ 1 ] ), &( m_pInternalParameters.k [ 2 ] ), &( m_pInternalParameters.k [ 3 ] ) );
				fgets ( str, 512, f );
				fgets ( str, 512, f ); sscanf ( str, "%f %f %f", &( m_pInternalParameters.A [ 0 ] ), &( m_pInternalParameters.A [ 1 ] ), &( m_pInternalParameters.A [ 2 ] ) );
				fgets ( str, 512, f ); sscanf ( str, "%f %f %f", &( m_pInternalParameters.A [ 4 ] ), &( m_pInternalParameters.A [ 5 ] ), &( m_pInternalParameters.A [ 6 ] ) );
				fgets ( str, 512, f ); sscanf ( str, "%f %f %f", &( m_pInternalParameters.A [ 8 ] ), &( m_pInternalParameters.A [ 9 ] ), &( m_pInternalParameters.A [ 10 ] ) );
				m_pInternalParameters.A [ 3 ]  = 0.0f;
				m_pInternalParameters.A [ 7 ]  = 0.0f;
				m_pInternalParameters.A [ 11 ]  = 0.0f;
				m_pInternalParameters.A [ 12 ]  = 0.0f;
				m_pInternalParameters.A [ 13 ]  = 0.0f;
				m_pInternalParameters.A [ 14 ]  = 0.0f;
				m_pInternalParameters.A [ 15 ]  = 1.0f;
				fclose ( f );
			}
			else
			{
				ERROR << "Error! Could not access to " << pInternalParameters << " file!" << ENDERROR;
				throw new std::exception;
			}
			f = fopen ( pExternalParameters, "r" );
			if ( f != NULL ) {
				char str [ 512 ];
				fgets ( str, 512, f );
				fgets ( str, 512, f ); sscanf ( str, "%f %f %f", &( m_pExternalParameters.R [ 0 ] ), &( m_pExternalParameters.R [ 1 ] ), &( m_pExternalParameters.R [ 2 ] ) );
				fgets ( str, 512, f ); sscanf ( str, "%f %f %f", &( m_pExternalParameters.R [ 3 ] ), &( m_pExternalParameters.R [ 4 ] ), &( m_pExternalParameters.R [ 5 ] ) );
				fgets ( str, 512, f ); sscanf ( str, "%f %f %f", &( m_pExternalParameters.R [ 6 ] ), &( m_pExternalParameters.R [ 7 ] ), &( m_pExternalParameters.R [ 8 ] ) );
				fgets ( str, 512, f );
				fgets ( str, 512, f ); sscanf ( str, "%f %f %f", &( m_pExternalParameters.T [ 0 ] ), &( m_pExternalParameters.T [ 1 ] ), &( m_pExternalParameters.T [ 2 ] ) );
				fclose ( f );
			}
			else
			{
				ERROR << "Error! Could not access to " << pExternalParameters << " file!" << ENDERROR;
				throw new std::exception;
			}
			MatrixIdentity ( m_pPositionMatrix );
			for ( int l = 0 ; l < 3 ; l ++ )
			{
				for ( int j = 0 ; j < 3 ; j ++)
				{
					m_pPositionMatrix [ l * 4 + j ]	= m_pExternalParameters.R [ j * 3 + l ];
					m_pPositionMatrix [ 3 * 4 + l ]	= m_pExternalParameters.T [ l ];
				}
			}
			/*
			printf( "---------------------------------------------------------------\n" );
			printf( "Position Matrix Loaded:\n" );
			printf( "[ %.5f\t  %.5f\t  %.5f\t  %.5f ]\n", m_pPositionMatrix[0], m_pPositionMatrix[1], m_pPositionMatrix[2], m_pPositionMatrix[3] );
			printf( "[ %.5f\t  %.5f\t  %.5f\t  %.5f ]\n", m_pPositionMatrix[4], m_pPositionMatrix[5], m_pPositionMatrix[6], m_pPositionMatrix[7] );
			printf( "[ %.5f\t  %.5f\t  %.5f\t  %.5f ]\n", m_pPositionMatrix[8], m_pPositionMatrix[9], m_pPositionMatrix[10], m_pPositionMatrix[11] );
			printf( "[ %.5f\t  %.5f\t  %.5f\t  %.5f ]\n", m_pPositionMatrix[12], m_pPositionMatrix[13], m_pPositionMatrix[14], m_pPositionMatrix[15] );
			printf( "---------------------------------------------------------------\n" );//*/

			m_pPositionVector[0] = - ( m_pExternalParameters.R [ 0 ] * m_pExternalParameters.T [ 0 ] + m_pExternalParameters.R [ 3 ] * m_pExternalParameters.T [ 1 ] + m_pExternalParameters.R [ 6 ] * m_pExternalParameters.T [ 2 ] );
			m_pPositionVector[1] = - ( m_pExternalParameters.R [ 1 ] * m_pExternalParameters.T [ 0 ] + m_pExternalParameters.R [ 4 ] * m_pExternalParameters.T [ 1 ] + m_pExternalParameters.R [ 7 ] * m_pExternalParameters.T [ 2 ] );
			m_pPositionVector[2] = - ( m_pExternalParameters.R [ 2 ] * m_pExternalParameters.T [ 0 ] + m_pExternalParameters.R [ 5 ] * m_pExternalParameters.T [ 1 ] + m_pExternalParameters.R [ 8 ] * m_pExternalParameters.T [ 2 ] );

			m_pPositionMatrixInverse[0] = m_pPositionMatrix[ 0 ];
			m_pPositionMatrixInverse[1] = m_pPositionMatrix[ 4 ];
			m_pPositionMatrixInverse[2] = m_pPositionMatrix[ 8 ];
			m_pPositionMatrixInverse[3] = 0.0f;

			m_pPositionMatrixInverse[4] = m_pPositionMatrix[ 1 ];
			m_pPositionMatrixInverse[5] = m_pPositionMatrix[ 5 ];
			m_pPositionMatrixInverse[6] = m_pPositionMatrix[ 9 ];
			m_pPositionMatrixInverse[7] = 0.0f;

			m_pPositionMatrixInverse[8] = m_pPositionMatrix[ 2 ];
			m_pPositionMatrixInverse[9] = m_pPositionMatrix[ 6 ];
			m_pPositionMatrixInverse[10] = m_pPositionMatrix[ 10 ];
			m_pPositionMatrixInverse[11] = 0.0f;

			m_pPositionMatrixInverse[15] = 1.0f;
			m_pPositionMatrixInverse[12] = - ( m_pExternalParameters.R [ 0 ] * m_pExternalParameters.T [ 0 ] + m_pExternalParameters.R [ 3 ] * m_pExternalParameters.T [ 1 ] + m_pExternalParameters.R [ 6 ] * m_pExternalParameters.T [ 2 ] );
			m_pPositionMatrixInverse[13] = - ( m_pExternalParameters.R [ 1 ] * m_pExternalParameters.T [ 0 ] + m_pExternalParameters.R [ 4 ] * m_pExternalParameters.T [ 1 ] + m_pExternalParameters.R [ 7 ] * m_pExternalParameters.T [ 2 ] );
			m_pPositionMatrixInverse[14] = - ( m_pExternalParameters.R [ 2 ] * m_pExternalParameters.T [ 0 ] + m_pExternalParameters.R [ 5 ] * m_pExternalParameters.T [ 1 ] + m_pExternalParameters.R [ 8 ] * m_pExternalParameters.T [ 2 ] );
			m_pPositionMatrixInverse[15] = 1.0f;
			/*
			printf( "---------------------------------------------------------------\n" );
			printf( "Position Matrix Inversed:\n" );
			printf( "[ %.5f\t  %.5f\t  %.5f\t  %.5f ]\n", m_pPositionMatrixInverse[0], m_pPositionMatrixInverse[1], m_pPositionMatrixInverse[2], m_pPositionMatrixInverse[3] );
			printf( "[ %.5f\t  %.5f\t  %.5f\t  %.5f ]\n", m_pPositionMatrixInverse[4], m_pPositionMatrixInverse[5], m_pPositionMatrixInverse[6], m_pPositionMatrixInverse[7] );
			printf( "[ %.5f\t  %.5f\t  %.5f\t  %.5f ]\n", m_pPositionMatrixInverse[8], m_pPositionMatrixInverse[9], m_pPositionMatrixInverse[10], m_pPositionMatrixInverse[11] );
			printf( "[ %.5f\t  %.5f\t  %.5f\t  %.5f ]\n", m_pPositionMatrixInverse[12], m_pPositionMatrixInverse[13], m_pPositionMatrixInverse[14], m_pPositionMatrixInverse[15] );
			printf( "---------------------------------------------------------------\n" );//*/

		}
		/// Erwan code...
		void ComputeFrustum( unsigned int uiImageWidth, unsigned int uiImageHeight )
		{
			/* direction de la camera */
			float xx, yy;
			test ( m_pInternalParameters.A, m_pInternalParameters.k, uiImageWidth / 2, uiImageHeight / 2, xx, yy );
			m_oCameraGL.direction [ 0 ] = xx;
			m_oCameraGL.direction [ 1 ] = yy;
			m_oCameraGL.direction [ 2 ] = 1.0;
			//~ normalize ( m_oCameraGL.direction, m_oCameraGL.direction );
			/* frustum gl */
			float x0, x1, x2, x3, y0, y1, y2, y3;
			test ( m_pInternalParameters.A, m_pInternalParameters.k, uiImageWidth/2, 0, x0, y0 );
			test ( m_pInternalParameters.A, m_pInternalParameters.k, uiImageWidth/2, uiImageHeight, x1, y1 );
			test ( m_pInternalParameters.A, m_pInternalParameters.k, 0, uiImageHeight/2, x2, y2 );
			test ( m_pInternalParameters.A, m_pInternalParameters.k, uiImageWidth, uiImageHeight/2, x3, y3 );
			m_oCameraGL.frustum[0] = - sqrtf ( ( x2 - xx ) * ( x2 - xx ) + ( y2 - yy ) * ( y2 - yy ) );
			m_oCameraGL.frustum[1] = sqrtf ( ( x3 - xx ) * ( x3 - xx ) + ( y3 - yy ) * ( y3 - yy ) );
			m_oCameraGL.frustum[2] = - sqrtf ( ( x0 - xx ) * ( x0 - xx ) + ( y0 - yy ) * ( y0 - yy ) );
			m_oCameraGL.frustum[3] = sqrtf ( ( x1 - xx ) * ( x1 - xx ) + ( y1 - yy ) * ( y1 - yy ) );
			m_oCameraGL.frustum[4]=1.f;
			m_oCameraGL.frustum[5]=1000.0f;// + 10.0 * m_pExternalParameters.T [ 2 ];

			printf( "---------------------------------------------------------------\n" );
			printf( "Frustum Computed:\n" );
			printf( "%.5f\t  %.5f\n", m_oCameraGL.frustum[0], m_oCameraGL.frustum[1] );
			printf( "%.5f\t  %.5f\n", m_oCameraGL.frustum[2], m_oCameraGL.frustum[3] );
			printf( "%.5f\t  %.5f\n", m_oCameraGL.frustum[4], m_oCameraGL.frustum[5] );
			printf( "---------------------------------------------------------------\n" );

		}
		/// Erwan code...
		void test2 ( float a[9], float k[4], unsigned int u, unsigned int v, float& rx, float& ry )
		{
			float u0, v0, fx, fy, _fx, _fy, k1, k2, p1, p2, xx, yy;
			u0 = a[2]; v0 = a[6];
			fx = a[0]; fy = a[5];
			_fx = 1.f/fx; _fy = 1.f/fy;
			k1 = k[0]; k2 = k[1];
			p1 = k[2]; p2 = k[3];

			float y = (v - v0)*_fy;
			float x = (u - u0)*_fx;

			float y2 = y*y;
			float _2p1y = 2*p1*y;
			float _3p1y2 = 3*p1*y2;
			float p2y2 = p2*y2;

			float x2 = x*x;
			float r2 = x2 + y2;
			float d = 1 + (k1 + k2*r2)*r2;

			rx = (x*(d + _2p1y) + p2y2 + (3*p2)*x2);
			ry = (y*(d + (2*p2)*x) + _3p1y2 + p1*x2);
		}
		void test ( float A[16], float K[4], unsigned int u, unsigned int v, float& rx, float& ry )
		{
			float u0, v0, a, b;
			u0 = A[ 2 ];
			v0 = A[ 6 ];
			a = A[ 0 ];
			b = A[ 5 ];

			rx = ( u - u0 ) / a;
			ry = ( v - v0 ) / b;
		}

		virtual void Draw() {}
		virtual void DrawImage() {}
	public: // Accessors
		float* GetRotation() { return m_pExternalParameters.R; }
		float* GetR() { return m_pExternalParameters.R; }
		float* GetTranslation() { return m_pExternalParameters.T; }
		float* GetT() { return m_pExternalParameters.T; }
		float* GetK() { return m_pInternalParameters.k; }
		float* GetA() { return m_pInternalParameters.A; }
		float* GetMatrixA() { return m_pInternalParameters.A; }
		float* GetPositionMatrix() { return m_pPositionMatrix; }
		float* GetPositionMatrixInverse() { return m_pPositionMatrixInverse; }
		float* GetMatrixRT() { return m_pPositionMatrix; }
		const float* GetPositionVector() const { return m_pPositionVector; }
		float* GetFrustum() { return m_oCameraGL.frustum; }
		float* GetDirection() { return m_oCameraGL.direction; }
	private:
		void Create( const char* pInternalParameters, const char* pExternalParameters )
		{
			Load( pInternalParameters, pExternalParameters );
		}
		void div ( float dest[ 3 ], const float v1[ 3 ], const float& v2 )
		{
			dest [ 0 ] = v1 [ 0 ] / v2;
			dest [ 1 ] = v1 [ 1 ] / v2;
			dest [ 2 ] = v1 [ 2 ] / v2;
		}
		float dot ( const float v1[ 3 ], const float v2[ 3 ] ) {
			float d = v1 [ 0 ] * v2 [ 0 ];
			d += v1 [ 1 ] * v2 [ 1 ];
			d += v1 [ 2 ] * v2 [ 2 ];
			return d;
		}
		float norm ( const float src[ 3 ] ) {
			float d = dot ( src, src );
			return sqrtf ( d );
		}
		void normalize ( float dest[ 3 ], const float src[ 3 ] ) {
			float d = norm ( src );
			div ( dest, src, d );
		}
		void mul ( float dest[3], const float M[9], const float v[3] ) {
			dest [ 0 ] = M [ 0 ] * v [ 0 ] + M [ 1 ] * v [ 1 ] + M [ 2 ] * v [ 2 ];
			dest [ 1 ] = M [ 3 ] * v [ 0 ] + M [ 4 ] * v [ 1 ] + M [ 5 ] * v [ 2 ];
			dest [ 2 ] = M [ 6 ] * v [ 0 ] + M [ 7 ] * v [ 1 ] + M [ 8 ] * v [ 2 ];
		}
		void add ( float dest[3], const float v1[3], const float v2[3] ) {
			dest [ 0 ] = v1 [ 0 ] + v2 [ 0 ];
			dest [ 1 ] = v1 [ 1 ] + v2 [ 1 ];
			dest [ 2 ] = v1 [ 2 ] + v2 [ 2 ];
		}
		void add ( float dest[3], const float& v1, const float v2[3] ) {
			dest [ 0 ] = v1 + v2 [ 0 ];
			dest [ 1 ] = v1 + v2 [ 1 ];
			dest [ 2 ] = v1 + v2 [ 2 ];
		}
		void add ( float dest[3], const float v1[3], const float& v2 ) {
			dest [ 0 ] = v1 [ 0 ] + v2;
			dest [ 1 ] = v1 [ 1 ] + v2;
			dest [ 2 ] = v1 [ 2 ] + v2;
		}
		void transpose ( float res[9], const float mat[9] ) {
			for ( uint i = 0 ; i < 3 ; i ++ ) {
				for ( uint j = 0 ; j < 4 ; j ++ ) res [ 3 * i + j ] = mat [ 3 * j + i ];
			}
		}
	private:
	public:
		ExternalParameters	m_pExternalParameters;
		InternalParameters	m_pInternalParameters;
		CameraGL		m_oCameraGL;
		float			m_pPositionMatrix[ 16 ];
		float			m_pPositionMatrixInverse[ 16 ];
		float			m_pPositionVector[ 3 ];
};

}
#endif //__CAMERA_HPP__
