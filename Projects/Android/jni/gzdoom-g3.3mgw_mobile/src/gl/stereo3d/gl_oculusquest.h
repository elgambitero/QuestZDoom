//
//---------------------------------------------------------------------------
// Copyright(C) 2016-2017 Christopher Bruns
// Oculus Quest changes Copyright(C) 2020 Simon Brown
// All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/
//
//--------------------------------------------------------------------------
//
/*
** gl_oculusquest.cpp
** Stereoscopic virtual reality mode for the Oculus Quest HMD
**
*/

#ifndef _GL_OCULUSQUEST_H
#define _GL_OCULUSQUEST_H

#include "gl_stereo3d.h"
#include "gl_stereo_leftright.h"

/* stereoscopic 3D API */
namespace s3d {

class OculusQuestEyePose : public ShiftedEyePose
{
public:
	friend class OculusQuestMode;

	OculusQuestEyePose(int eye);
	virtual ~OculusQuestEyePose() override;
	virtual VSMatrix GetProjection(FLOATTYPE fov, FLOATTYPE aspectRatio, FLOATTYPE fovRatio) const override;
	void GetViewShift(FLOATTYPE yaw, FLOATTYPE outViewShift[3]) const override;
	virtual void AdjustHud() const override;
	virtual void AdjustBlend() const override;

	void initialize();
	void dispose();
	bool submitFrame() const;

protected:
	VSMatrix projectionMatrix;
	VSMatrix eyeToHeadTransform;
	VSMatrix otherEyeToHeadTransform;

	mutable uint32_t framebuffer;
	int eye;

	VSMatrix getQuadInWorld(
		float distance, 
		float width, 
		bool doFixPitch,
		float pitchOffset
	) const;
};

class OculusQuestMode : public Stereo3DMode
{
public:
	friend class OculusQuestEyePose;
	static const Stereo3DMode& getInstance(); // Might return Mono mode, if no HMD available

	virtual ~OculusQuestMode() override;
	virtual void SetUp() const override; // called immediately before rendering a scene frame
	virtual void TearDown() const override; // called immediately after rendering a scene frame
	virtual void Present() const override;
	virtual void AdjustViewports() const override;
	virtual void AdjustPlayerSprites() const override;
	virtual void UnAdjustPlayerSprites() const override;
	virtual void AdjustCrossHair() const override;
	virtual void UnAdjustCrossHair() const override;

	virtual bool GetHandTransform(int hand, VSMatrix* out) const override;
	virtual bool GetWeaponTransform(VSMatrix* out) const override;
	virtual bool RenderPlayerSpritesCrossed() const { return true; }
	virtual bool RenderPlayerSpritesInScene() const { return true; }

protected:
	OculusQuestMode();
	// void updateDoomViewDirection() const;
	void updateHmdPose(double hmdYawRadians, double hmdPitchRadians, double hmdRollRadians) const;

	OculusQuestEyePose leftEyeView;
	OculusQuestEyePose rightEyeView;

	mutable int cachedScreenBlocks;
	mutable double hmdYaw; // cached latest value in radians
	mutable int cachedViewwidth, cachedViewheight, cachedViewwindowx, cachedViewwindowy;
	mutable F2DDrawer * cached2DDrawer;
	mutable F2DDrawer * crossHairDrawer;

private:
	typedef Stereo3DMode super;
	bool hmdWasFound;
	uint32_t sceneWidth, sceneHeight;
};

} /* namespace st3d */


#endif /* _GL_OCULUSQUEST_H */