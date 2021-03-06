// C++ Implementation of Terrain Editing Component

#ifndef TERRAINEDIT_H
#define TERRAINEDIT_H

#include <Urho3D/Urho3D.h>

#include <Urho3D/Container/ArrayPtr.h>
#include <Urho3D/Graphics/Terrain.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Container/Str.h>
#include <Urho3D/Graphics/CustomGeometry.h>

#include "ThirdParty/ANL/VM/vm.h"
#include "ThirdParty/ANL/VM/instruction.h"
#include "ThirdParty/ANL/VM/kernel.h"
#include "ThirdParty/ANL/VM/coordinate.h"
#include "ThirdParty/ANL/templates/tarrays.h"
#include "ThirdParty/ANL/templates/tarray1.h"
#include "ThirdParty/ANL/Imaging/imaging.h"

using namespace Urho3D;
using namespace anl;
	
	Vector2 WorldToNormalized(Image *height, Terrain *terrain, Vector3 world);
	Vector3 NormalizedToWorld(Image *height, Terrain *terrain, Vector2 normalized);
	
	void SetHeightValue(Image *height, int x, int y, float val);
	float GetHeightValue(Image *height, int x, int y);
	
	void ApplyHeightBrush(Terrain *terrain, Image *height, Image *mask, float x, float z, float radius, float max, float power, float hardness, bool usemask, float dt);
	void ApplyBlendBrush(Terrain *terrain, Image *height, Image *blend, Image *mask, float x, float z, float radius, float mx, float power, float hardness, int layer, bool usemask, float dt);
	void ApplyBlendBrush8(Terrain *terrain, Image *height, Image *blend0, Image *blend1, Image *mask, float x, float z, float radius, float mx, float power, float hardness, int layer, bool usemask, float dt);
	void ApplyMaskBrush(Terrain *terrain, Image *height, Image *mask, float x, float z, float radius, float mx, float power, float hardness, float dt);
	void ApplySmoothBrush(Terrain *terrain, Image *height, Image *mask, float x, float z, float radius, float max, float power, float hardness, bool usemask, float dt);
	void ApplySpeckleBrush(Terrain *terrain, Image *height, Image *color, Image *mask, float x, float z, float radius, float mx, float power, float hardness, Color c1, Color c2, bool usemask, float dt);
	
	void SetBrushCursorHeight(Terrain *terrain, CustomGeometry *brush, float groundx, float groundz);
	
	void InvertMask(Image *mask);
	void RenderANLKernelToHeight(Image *height, Image *mask, CKernel *kernel, double lowRange=0, double highRange=1, bool useMask=false, bool invertMask=false);
	
	struct RasterVertex
	{
		float x_, y_;
		float val_;
		
		RasterVertex() : x_(0), y_(0), val_(0){}
		RasterVertex(float x, float y, float val): x_(x), y_(y), val_(val){}
		RasterVertex(const RasterVertex &rhs) : x_(rhs.x_), y_(rhs.y_), val_(rhs.val_){}
		~RasterVertex(){}
		
		void operator=(const RasterVertex &rhs)
		{
			x_=rhs.x_;
			y_=rhs.y_;
			val_=rhs.val_;
		}
		
	};
	
	typedef TArray1D<RasterVertex> RasterVertexList;
	//typedef TArray2D<float> CArray2Dd;
	
	void RasterizeTriangle(CArray2Dd *buffer, RasterVertex v1, RasterVertex v2, RasterVertex v3);
	void RasterizeQuadStrip(CArray2Dd *buffer, RasterVertexList *strip);
	void BlendHeightWithRasterizedBuffer(Image *height, CArray2Dd *buffer, CArray2Dd *blend, Image *mask=0, bool useMask=false, bool invertMask=false);
	void BlendColorWithRasterizedBuffer(Image *img, CArray2Dd *buffer, Color endColor, Image *mask=0, bool useMask=false, bool invertMask=false);
	void BlendRasterizedBuffer8(Image *img, Image *other, CArray2Dd *buffer, int layer, Image *mask=0, bool useMask=false, bool invertMask=false);
	void BlendRasterizedBuffer8Max(Image *img, Image *other, CArray2Dd *buffer, int layer, Image *mask=0, bool useMask=false, bool invertMask=false);
	void TessellateLineList(RasterVertexList *in, RasterVertexList *out, int steps);
	void ApplyBedFunction(CArray2Dd *buffer, float hardness, bool quintic);
	void BuildQuadStrip(RasterVertexList *in, RasterVertexList *out, float width);
	void BuildQuadStripVarying(RasterVertexList *in, RasterVertexList *out, float startwidth, float endwidth);
	void RenderANLKernelToBuffer(CArray2Dd *buffer, CKernel *kernel, float lowrange=0, float highrange=1);
	void SetHeightFromRasterBuffer(Image *height, CArray2Dd *buffer, Image *mask=0, bool useMask=false, bool invertMask=false);
	
	void DistortBuffer(CArray2Dd *buffer, CArray2Dd *xd, CArray2Dd *yd, double power);
	
	void CopyImageInto(Image *dest, Image *src, int x, int y);
	bool IsPowerOfTwo(int n);
	Image* GetNextImageLevel(Image *i);
	void ExtractLayerToBuffer(Image *blend1, Image *blend2, CArray2Dd *buffer, int layer);
	float GetLayerBlend(Image *blend1, Image *blend2, int x, int y, int layer);
	void SetLayerBlend(Image *blend1, Image *blend2, int x, int y, int layer, float v);
	
	bool LoadImage(Context *c, Image *i, const char *fname);

#endif
