/* version restructuree mailto:jean-claude.iehl@liris.cnrs.fr */

/*
 * pbrt source code Copyright(c) 1998-2005 Matt Pharr and Greg Humphreys
 *
 * All Rights Reserved.
 * For educational use only; commercial use expressly forbidden.
 * NO WARRANTY, express or implied, for this software.
 * (See file License.txt for complete license)
 */

#ifndef PBRT_TRANSFORM_H
#define PBRT_TRANSFORM_H

#include <iostream>
#include <cstdio>
#include <cassert>

#include "Geometry.h"

namespace gk {
    
// Matrix4x4 Declarations
//! representation d'une matrice homogene 4x4.
struct Matrix4x4
{
    // Matrix4x4 Public Methods
    //! construit une matrice identite, par defaut.
    Matrix4x4( )
    {
        for(int i = 0; i < 4; ++i)
            for(int j = 0; j < 4; ++j)
                m[i][j] = 0.f;
        
        for(int k= 0; k < 4; k++)
            m[k][k]= 1.f;
    }
    
    //! construit une matrice a partir d'un tableau 2d de reels [ligne][colonne].
    Matrix4x4( const float mat[4][4] );
    
    //! construit une matrice a partir des 16 elements.
    Matrix4x4( 
       float t00, float t01, float t02, float t03,
       float t10, float t11, float t12, float t13,
       float t20, float t21, float t22, float t23,
       float t30, float t31, float t32, float t33 );
    
    //! renvoie la matrice transposee.
    Matrix4x4 Transpose( ) const;
    
    //! affiche la matrice.
    void Print( std::ostream &os ) const 
    {
        os << "[ ";
        for (int i = 0; i < 4; ++i) {
            os << "[ ";
            for (int j = 0; j < 4; ++j)  {
                os << m[i][j];
                if (j != 3) os << ", ";
            }
            os << " ] ";
        }
        os << " ] ";
    }
    
    //! affiche la matrice.
    void print( ) const
    {
        #define M44t(m, r, c) m[r][c]
        
        printf("% -.8f  % -.8f  % -.8f  % -.8f\n", 
            M44t(m, 0, 0), M44t(m, 0, 1), M44t(m, 0, 2), M44t(m, 0, 3));
        printf("% -.8f  % -.8f  % -.8f  % -.8f\n", 
            M44t(m, 1, 0), M44t(m, 1, 1), M44t(m, 1, 2), M44t(m, 1, 3));
        printf("% -.8f  % -.8f  % -.8f  % -.8f\n", 
            M44t(m, 2, 0), M44t(m, 2, 1), M44t(m, 2, 2), M44t(m, 2, 3));
        printf("% -.8f  % -.8f  % -.8f  % -.8f\n", 
            M44t(m, 3, 0), M44t(m, 3, 1), M44t(m, 3, 2), M44t(m, 3, 3));
        printf("\n");
        
        #undef M55
   }
    
    //! \name renvoie un vecteur de la matrice 
    // @{
    void getColumn( const int c, float v[4] ) const
    {
        assert(c >= 0 && c < 4);
        v[0]= m[0][c];
        v[1]= m[1][c];
        v[2]= m[2][c];
        v[3]= m[3][c];
    }
    
    void getRow( const int r, float v[4] ) const
    {
        assert(r >= 0 && r < 4);
        v[0]= m[r][0];
        v[1]= m[r][1];
        v[2]= m[r][2];
        v[3]= m[r][3];
    }
    
    Vector getRotationVector( const int c ) const
    {
        assert(c >= 0 && c < 3);
        return Vector(m[c][0], m[c][1], m[c][2]);
    }
    
    void getRotationVector( const int c, Vector& v ) const
    {
        assert(c >= 0 && c < 3);
        v.x= m[c][0];
        v.y= m[c][1];
        v.z= m[c][2];
    }
    
    Vector getTranslationVector( ) const
    {
        return Vector(-m[0][3], -m[1][3], -m[2][3]);
    }
    
    void getTranslationVector( Vector& v ) const
    {
        v.x= m[0][3];
        v.y= m[1][3];
        v.z= m[2][3];
    }
    // @}
    
    //! produit de 2 matrices : renvoie m1 * m2.
    static 
    Matrix4x4 Mul( const Matrix4x4 &m1, const Matrix4x4 &m2 )
    {
        float r[4][4];
        
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                r[i][j] = 
                    m1.m[i][0] * m2.m[0][j] +
                    m1.m[i][1] * m2.m[1][j] +
                    m1.m[i][2] * m2.m[2][j] +
                    m1.m[i][3] * m2.m[3][j];
        
        return Matrix4x4(r);
    }
    
    //! \name transformation de points, vecteurs, etc. par la matrice.
    
    // @{
    static
    Vector Transform( const Matrix4x4& m, const Vector& v )
    {
        const float x = v.x;
        const float y = v.y;
        const float z = v.z;
        
        return Vector( 
            m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z,
            m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z,
            m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z );        
    }
    
    static
    Point Transform( const Matrix4x4& m, const Point& p )
    {
        const float x = p.x;
        const float y = p.y;
        const float z = p.z;
        
        const float xt = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z + m.m[0][3];
        const float yt = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z + m.m[1][3];
        const float zt = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z + m.m[2][3];
        const float wt = m.m[3][0] * x + m.m[3][1] * y + m.m[3][2] * z + m.m[3][3];
        
        assert( wt != 0 );
        if( wt == 1.f ) 
            return Point( xt, yt, zt );
        else
            return Point( xt, yt, zt ) / wt;        
    }

    static
    HPoint Transform( const Matrix4x4& m, const HPoint& p)
    {
        const float x = p.x;
        const float y = p.y;
        const float z = p.z;
        assert(p.w == 1.f);
        
        const float xt = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z + m.m[0][3];
        const float yt = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z + m.m[1][3];
        const float zt = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z + m.m[2][3];
        const float wt = m.m[3][0] * x + m.m[3][1] * y + m.m[3][2] * z + m.m[3][3];
        //~ assert( wt != 0 );
        
        return HPoint(xt, yt, zt, wt);
    }
    
    static
    Normal Transform( const Matrix4x4& m, const Normal& n )
    {
        const float x = n.x;
        const float y = n.y;
        const float z = n.z;
        
        // utilise la transformation inverse ... pas une transformation directe.
        const float tx = m.m[0][0] * x + m.m[1][0] * y + m.m[2][0] * z;
        const float ty = m.m[0][1] * x + m.m[1][1] * y + m.m[2][1] * z;
        const float tz = m.m[0][2] * x + m.m[1][2] * y + m.m[2][2] * z;
        
        return Normal(tx, ty, tz);
    }
    
    static
    BBox Transform( const Matrix4x4& m, const BBox& b )
    {
    #if 0
        BBox ret( Matrix4x4::Transform( m, Point( b.pMin.x, b.pMin.y, b.pMin.z ) ) );
        ret= Union(ret, Matrix4x4::Transform( m, Point( b.pMax.x, b.pMin.y, b.pMin.z ) ) );
        ret= Union(ret, Matrix4x4::Transform( m, Point( b.pMin.x, b.pMax.y, b.pMin.z ) ) );
        ret= Union(ret, Matrix4x4::Transform( m, Point( b.pMin.x, b.pMin.y, b.pMax.z ) ) );
        ret= Union(ret, Matrix4x4::Transform( m, Point( b.pMin.x, b.pMax.y, b.pMax.z ) ) );
        ret= Union(ret, Matrix4x4::Transform( m, Point( b.pMax.x, b.pMax.y, b.pMin.z ) ) );
        ret= Union(ret, Matrix4x4::Transform( m, Point( b.pMax.x, b.pMin.y, b.pMax.z ) ) );
        ret= Union(ret, Matrix4x4::Transform( m, Point( b.pMax.x, b.pMax.y, b.pMax.z ) ) );
        
    #else
        BBox ret;
        ret.Union( Matrix4x4::Transform( m, Point( b.pMin.x, b.pMin.y, b.pMin.z ) ) );
        ret.Union( Matrix4x4::Transform( m, Point( b.pMax.x, b.pMin.y, b.pMin.z ) ) );
        ret.Union( Matrix4x4::Transform( m, Point( b.pMin.x, b.pMax.y, b.pMin.z ) ) );
        ret.Union( Matrix4x4::Transform( m, Point( b.pMin.x, b.pMin.y, b.pMax.z ) ) );
        ret.Union( Matrix4x4::Transform( m, Point( b.pMin.x, b.pMax.y, b.pMax.z ) ) );
        ret.Union( Matrix4x4::Transform( m, Point( b.pMax.x, b.pMax.y, b.pMin.z ) ) );
        ret.Union( Matrix4x4::Transform( m, Point( b.pMax.x, b.pMin.y, b.pMax.z ) ) );
        ret.Union( Matrix4x4::Transform( m, Point( b.pMax.x, b.pMax.y, b.pMax.z ) ) );        
    #endif
        return ret;
    }
    // @}
    
    //! renvoie l'inverse de la matrice.
    Matrix4x4 Inverse( ) const;
    
    //! conversion en float (*)[4]
    operator float *( )
    {
        return (float *) m;
    }
    
    //! conversion en const float (*)[4]
    operator const float *( ) const
    {
        return (const float *) m;
    }
    
    //! elements de la matrice.
    float m[4][4];
};


// Transform Declarations
//! representation d'une transformation == un changement de repere. 
class Transform
{
public:
    // Transform Public Methods
    //! constructeur par defaut, transformation identite.
    Transform() {}
    
    //! construction a partir d'une matrice representee par un tableau 2d de reels.
    Transform( float mat[4][4] )
    {
        m= Matrix4x4(mat);
        mInv = m.Inverse();
    }
    
    //! construction a partir d'une matrice.
    Transform( const Matrix4x4& mat )
    {
        m = mat;
        mInv = m.Inverse();
    }
    
    //! construction a partir d'une matrice et de son inverse.
    Transform( const Matrix4x4& mat, const Matrix4x4& minv )
    {
        m = mat;
        mInv = minv;
    }
    
    friend std::ostream &operator<<( std::ostream &, const Transform & );
    
    //! affiche la matrice representant la transformation.
    void print() const
    {
        m.print();
    }
    
    //! renvoie la transformation sous forme de matrice openGL, utilisable directement avec glLoadTransposeMatrixf();
    const Matrix4x4& getGLTransposeMatrix( ) const
    {
        return m;
    }
    
    //! renvoie la transformation sous forme de matrice.
    const Matrix4x4& matrix( ) const
    {
        return m;
    }
    
    //! renvoie la transformation inverse sous forme de matrice
    const Matrix4x4& inverseMatrix( ) const
    {
        return mInv;
    }
    
    //! renvoie la transformation inverse.
    Transform getInverse() const
    {
        return Transform( mInv, m );
    }
    
    //! \name transformations de points, vecteurs, normales, rayons, aabox.
    // @{
    inline Point operator()( const Point &p ) const;
    inline void operator()( const Point &p, Point &pt ) const;

    //! renvoie le point homogene transforme.
    inline void operator()( const Point &p, HPoint &pt ) const;
    
    inline Vector operator()( const Vector &v ) const;
    inline void operator()( const Vector &v, Vector &vt ) const;
    inline Normal operator()( const Normal & ) const;
    inline void operator()( const Normal &, Normal &nt ) const;
    inline Ray operator()( const Ray &r ) const;
    inline void operator()( const Ray &r, Ray &rt ) const;
    BBox operator()( const BBox &b ) const;
    // @}

    //! \name transformations inverses de points, vecteurs, normales, rayons, aabox.
    // @{
    inline Point inverse( const Point &p ) const;
    inline void inverse( const Point &p, Point &pt ) const;

    //! renvoie le point homogene transforme.
    inline void inverse( const Point &p, HPoint &pt ) const;
    
    inline Vector inverse( const Vector &v ) const;
    inline void inverse( const Vector &v, Vector &vt ) const;
    inline Normal inverse( const Normal & ) const;
    inline void inverse( const Normal &, Normal &nt ) const;
    inline Ray inverse( const Ray &r ) const;
    inline void inverse( const Ray &r, Ray &rt ) const;
    BBox inverse( const BBox &b ) const;
    // @}
    
    //! composition de 2 transformations.
    Transform operator*( const Transform &t2 ) const;
    
    bool SwapsHandedness() const;

protected:
    // Transform Private Data
    //! les matrices directe et inverse de changement de repere.
    Matrix4x4 m, mInv;
};

/* \todo creer RigidTransform qui ne construit que la matrice directe R.T et le constructeur Transform( const RigidTransform& )= { R.T, -T.R-1 }
    modifier RotateXYZ Scale et Translate pour construire une RigidTransform
    ajouter X= Transform::inverse( const X& ) pour obtenir la transformation inverse au lieu de X= Transform::getInverse()(X)

    deplacer les operateurs de transformations operator( const X& ) (point, vecteur, normale, etc.) dans la classe Matrix4x4 
*/


Transform Viewport( float width, float height );
Transform Perspective( float fov, float aspect, float znear, float zfar );
Transform Orthographic( float znear, float zfar );
Transform Orthographic( const float left, const float right, const float bottom, const float top, const float znear, const float zfar );
Transform LookAt( const Point &pos, const Point &look, const Vector &up );
Transform Rotate( float angle, const Vector &axis );
Transform RotateX( float angle );
Transform RotateY( float angle );
Transform RotateZ( float angle );
Transform Scale( float x, float y, float z );
Transform Scale( float value );
Transform Translate( const Vector &delta );


// Transform Inline Functions
inline 
Point Transform::operator()( const Point &p ) const
{
    const float x = p.x;
    const float y = p.y;
    const float z = p.z;
    
    const float xt = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z + m.m[0][3];
    const float yt = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z + m.m[1][3];
    const float zt = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z + m.m[2][3];
    const float wt = m.m[3][0] * x + m.m[3][1] * y + m.m[3][2] * z + m.m[3][3];
    
    assert( wt != 0 );
    if( wt == 1.f ) 
        return Point( xt, yt, zt );
    else
        return Point( xt, yt, zt ) / wt;
}


inline 
void Transform::operator()( const Point &p, Point &pt ) const
{
    const float x = p.x;
    const float y = p.y;
    const float z = p.z;
    
    pt.x = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z + m.m[0][3];
    pt.y = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z + m.m[1][3];
    pt.z = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z + m.m[2][3];
    
    const float wt = m.m[3][0] * x + m.m[3][1] * y + m.m[3][2] * z + m.m[3][3];
    assert( wt != 0 );
    if( wt != 1.f ) 
        pt /= wt;
}

inline 
void Transform::operator()( const Point &p, HPoint &pt ) const
{
    const float x = p.x;
    const float y = p.y;
    const float z = p.z;
    
    pt.x = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z + m.m[0][3];
    pt.y = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z + m.m[1][3];
    pt.z = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z + m.m[2][3];
    pt.w = m.m[3][0] * x + m.m[3][1] * y + m.m[3][2] * z + m.m[3][3];
}

inline 
Vector Transform::operator()( const Vector &v ) const
{
    const float x = v.x;
    const float y = v.y;
    const float z = v.z;
    
    return Vector( 
        m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z,
        m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z,
        m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z);
}

inline 
void Transform::operator()( const Vector &v, Vector &vt ) const
{
    const float x = v.x;
    const float y = v.y;
    const float z = v.z;
    
    vt.x = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z;
    vt.y = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z;
    vt.z = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z;
}

inline 
Normal Transform::operator()( const Normal &n ) const
{
    const float x = n.x;
    const float y = n.y;
    const float z = n.z;
    
    return Normal( 
        mInv.m[0][0] * x + mInv.m[1][0] * y + mInv.m[2][0] * z,
        mInv.m[0][1] * x + mInv.m[1][1] * y + mInv.m[2][1] * z,
        mInv.m[0][2] * x + mInv.m[1][2] * y + mInv.m[2][2] * z);
}

inline 
void Transform::operator()( const Normal &n, Normal& nt ) const
{
    const float x = n.x;
    const float y = n.y;
    const float z = n.z;
    
    nt.x = mInv.m[0][0] * x + mInv.m[1][0] * y + mInv.m[2][0] * z;
    nt.y = mInv.m[0][1] * x + mInv.m[1][1] * y + mInv.m[2][1] * z;
    nt.z = mInv.m[0][2] * x + mInv.m[1][2] * y + mInv.m[2][2] * z;
}

#if 1
inline
BBox Transform::operator()( const BBox &b ) const
{
    // transformation rigide ...
    const Transform &M = *this;
    
    BBox ret( M( Point( b.pMin.x, b.pMin.y, b.pMin.z ) ) );
    ret = Union( ret, M( Point( b.pMax.x, b.pMin.y, b.pMin.z ) ) );
    ret = Union( ret, M( Point( b.pMin.x, b.pMax.y, b.pMin.z ) ) );
    ret = Union( ret, M( Point( b.pMin.x, b.pMin.y, b.pMax.z ) ) );
    ret = Union( ret, M( Point( b.pMin.x, b.pMax.y, b.pMax.z ) ) );
    ret = Union( ret, M( Point( b.pMax.x, b.pMax.y, b.pMin.z ) ) );
    ret = Union( ret, M( Point( b.pMax.x, b.pMin.y, b.pMax.z ) ) );
    ret = Union( ret, M( Point( b.pMax.x, b.pMax.y, b.pMax.z ) ) );
    
    return ret;
}
#endif

inline 
Ray Transform::operator()( const Ray &r ) const
{
    Ray rt;
    (*this)( r.o, rt.o );
    (*this)( r.d, rt.d );
    rt.tmin = r.tmin;
    rt.tmax = r.tmax;

    return rt;
}

inline 
void Transform::operator()( const Ray &r, Ray &rt ) const
{
    (*this)( r.o, rt.o );
    (*this)( r.d, rt.d );
    rt.tmin = r.tmin;
    rt.tmax = r.tmax;
}


// inverse Transform Inline Functions
inline 
Point Transform::inverse( const Point &p ) const
{
    const float x = p.x;
    const float y = p.y;
    const float z = p.z;
    
    const float xt = mInv.m[0][0] * x + mInv.m[0][1] * y + mInv.m[0][2] * z + mInv.m[0][3];
    const float yt = mInv.m[1][0] * x + mInv.m[1][1] * y + mInv.m[1][2] * z + mInv.m[1][3];
    const float zt = mInv.m[2][0] * x + mInv.m[2][1] * y + mInv.m[2][2] * z + mInv.m[2][3];
    const float wt = mInv.m[3][0] * x + mInv.m[3][1] * y + mInv.m[3][2] * z + mInv.m[3][3];
    
    assert( wt != 0 );
    if( wt == 1.f ) 
        return Point( xt, yt, zt );
    else
        return Point( xt, yt, zt ) / wt;
}


inline 
void Transform::inverse( const Point &p, Point &pt ) const
{
    const float x = p.x;
    const float y = p.y;
    const float z = p.z;
    
    pt.x = mInv.m[0][0] * x + mInv.m[0][1] * y + mInv.m[0][2] * z + mInv.m[0][3];
    pt.y = mInv.m[1][0] * x + mInv.m[1][1] * y + mInv.m[1][2] * z + mInv.m[1][3];
    pt.z = mInv.m[2][0] * x + mInv.m[2][1] * y + mInv.m[2][2] * z + mInv.m[2][3];
    
    const float wt = mInv.m[3][0] * x + mInv.m[3][1] * y + mInv.m[3][2] * z + mInv.m[3][3];
    assert( wt != 0 );
    if( wt != 1.f ) 
        pt /= wt;
}

inline 
void Transform::inverse( const Point &p, HPoint &pt ) const
{
    const float x = p.x;
    const float y = p.y;
    const float z = p.z;
    
    pt.x = mInv.m[0][0] * x + mInv.m[0][1] * y + mInv.m[0][2] * z + mInv.m[0][3];
    pt.y = mInv.m[1][0] * x + mInv.m[1][1] * y + mInv.m[1][2] * z + mInv.m[1][3];
    pt.z = mInv.m[2][0] * x + mInv.m[2][1] * y + mInv.m[2][2] * z + mInv.m[2][3];
    pt.w = mInv.m[3][0] * x + mInv.m[3][1] * y + mInv.m[3][2] * z + mInv.m[3][3];
}

inline 
Vector Transform::inverse( const Vector &v ) const
{
    const float x = v.x;
    const float y = v.y;
    const float z = v.z;
    
    return Vector( 
        mInv.m[0][0] * x + mInv.m[0][1] * y + mInv.m[0][2] * z,
        mInv.m[1][0] * x + mInv.m[1][1] * y + mInv.m[1][2] * z,
        mInv.m[2][0] * x + mInv.m[2][1] * y + mInv.m[2][2] * z);
}

inline 
void Transform::inverse( const Vector &v, Vector &vt ) const
{
    const float x = v.x;
    const float y = v.y;
    const float z = v.z;
    
    vt.x = mInv.m[0][0] * x + mInv.m[0][1] * y + mInv.m[0][2] * z;
    vt.y = mInv.m[1][0] * x + mInv.m[1][1] * y + mInv.m[1][2] * z;
    vt.z = mInv.m[2][0] * x + mInv.m[2][1] * y + mInv.m[2][2] * z;
}

inline 
Normal Transform::inverse( const Normal &n ) const
{
    const float x = n.x;
    const float y = n.y;
    const float z = n.z;
    
    return Normal( 
        m.m[0][0] * x + m.m[1][0] * y + m.m[2][0] * z,
        m.m[0][1] * x + m.m[1][1] * y + m.m[2][1] * z,
        m.m[0][2] * x + m.m[1][2] * y + m.m[2][2] * z);
}

inline 
void Transform::inverse( const Normal &n, Normal& nt ) const
{
    const float x = n.x;
    const float y = n.y;
    const float z = n.z;
    
    nt.x = m.m[0][0] * x + m.m[1][0] * y + m.m[2][0] * z;
    nt.y = m.m[0][1] * x + m.m[1][1] * y + m.m[2][1] * z;
    nt.z = m.m[0][2] * x + m.m[1][2] * y + m.m[2][2] * z;
}

#if 1
inline
BBox Transform::inverse( const BBox &b ) const
{
    // transformation rigide ...
    const Transform &M = *this;
    
    BBox ret( M.inverse( Point( b.pMin.x, b.pMin.y, b.pMin.z ) ) );
    ret = Union( ret, M.inverse( Point( b.pMax.x, b.pMin.y, b.pMin.z ) ) );
    ret = Union( ret, M.inverse( Point( b.pMin.x, b.pMax.y, b.pMin.z ) ) );
    ret = Union( ret, M.inverse( Point( b.pMin.x, b.pMin.y, b.pMax.z ) ) );
    ret = Union( ret, M.inverse( Point( b.pMin.x, b.pMax.y, b.pMax.z ) ) );
    ret = Union( ret, M.inverse( Point( b.pMax.x, b.pMax.y, b.pMin.z ) ) );
    ret = Union( ret, M.inverse( Point( b.pMax.x, b.pMin.y, b.pMax.z ) ) );
    ret = Union( ret, M.inverse( Point( b.pMax.x, b.pMax.y, b.pMax.z ) ) );
    
    return ret;
}
#endif

inline 
Ray Transform::inverse( const Ray &r ) const
{
    Ray rt;
    inverse( r.o, rt.o );
    inverse( r.d, rt.d );
    rt.tmin = r.tmin;
    rt.tmax = r.tmax;

    return rt;
}

inline 
void Transform::inverse( const Ray &r, Ray &rt ) const
{
    inverse( r.o, rt.o );
    inverse( r.d, rt.d );
    rt.tmin = r.tmin;
    rt.tmax = r.tmax;
}

} // namespace

#endif // PBRT_TRANSFORM_H
