//////////////////////////////////////////////////////////////////////
///Copyright (C) 2011-2012 Benjamin Quach
//
//This file is part of the "Lost Horizons" video game demo
//
//"Lost Horizons" is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
///////////////////////////////////////////////////////////////////////


//Explosion code
//Creates initial explosion, is then added to gamemanager to be timed
//then deleted


#include "stdafx.h"
#include "explosion.h"

//Theres alot of code here, but no real complex code. Just setup of nodes and stuff like that
//So I'm not really going to explain 
explosion::explosion(irr::IrrlichtDevice *graphics, irrklang::ISoundEngine *sound, core::vector3df& pos) : CEffect(graphics,sound,pos)
{
	//2 second timer
	//initialize important variables
	this->graphics	=		graphics;

	last_time		=		graphics->getTimer()->getTime()+3000;
	end_time		=		graphics->getTimer()->getTime()+6000;

	setEnd(false);																//this variable is mentioned in the CEffect class
	node			=		graphics->getSceneManager()->addCubeSceneNode();	//for reference purposes
	node->setPosition(pos);
	

	//create particle system
	ps = graphics->getSceneManager()->addParticleSystemSceneNode(false, node);
	scene::IParticleEmitter *em = ps->createSphereEmitter(core::vector3df(0,0,0),50,
		vector3df(0,0,0),100,200,SColor(0,255,255,255),
		SColor(0,255,255,255),3000,3000,0,dimension2df(100,100),dimension2df(500,500));
	ps->setEmitter(em);
	em->drop();
	ps->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/engine_trails.pcx"));
	ps->setMaterialFlag(video::EMF_LIGHTING,false);
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

	//create affector so stuff blows outward
	scene::IParticleAffector *explo = ps->createAttractionAffector(node->getPosition(),-150.F);
	ps->addAffector(explo);
	scene::IParticleAffector *af = ps->createFadeOutParticleAffector(SColor(0,0,0,0),1000);
	ps->addAffector(af);
	debris = graphics->getSceneManager()->addParticleSystemSceneNode(false,node);
	scene::IParticleEmitter *deb = debris->createSphereEmitter(vector3df(0,0,0),50,vector3df(0,0,0),50,100,SColor(0,255,255,255),SColor(0,255,255,255),500,750,0,dimension2df(10,10),dimension2df(40,40));
	debris->setEmitter(deb);
	deb->drop();
	af->drop();
	scene::IParticleAffector *explo2 = debris->createAttractionAffector(node->getPosition(),-70.f);
	
	debris->addAffector(explo2);
	debris->addAffector(af);
	debris->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/fire.png"));
	debris->setMaterialFlag(video::EMF_LIGHTING,false);
	debris->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

	shockwave = graphics->getSceneManager()->addCubeSceneNode(10);
	shockwave->setScale(core::vector3df(10,0,10));
	shockwave->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/shockwave.png"));
	shockwave->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	shockwave->setParent(node);
	shockwave->setMaterialFlag(video::EMF_LIGHTING, false);

}

//This is run by the GameManager object
void explosion::loop()
{
	core::vector3df tmp = shockwave->getScale();
	tmp.X+=2;
	tmp.Z+=2;
	shockwave->setScale(tmp);

	if(last_time<graphics->getTimer()->getTime())
	{	
		shockwave->setVisible(false);
		ps->setEmitter(0);
		debris->setEmitter(0);
	}
	if(end_time<graphics->getTimer()->getTime())
	{
		setEnd(true);
	}
}

//remove everything associated with the stuff
explosion::~explosion()
{
	//ps->setVisible(false);
	//node->setVisible(false);
	shockwave->remove();
	node->remove();
}

void explosion::drop()
{
	delete this;
}
