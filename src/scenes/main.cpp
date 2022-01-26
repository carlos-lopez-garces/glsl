#include "scene.h"
#include "scenerunner.h"

// Shading models.
#include "scenephong.h"
#include "scenediffuse.h"
#include "scenediscard.h"
#include "sceneflat.h"
#include "scenesubroutine.h"
#include "scenetwoside.h"

// Lighting.
#include "scenedirectional.h"
#include "scenefog.h"
#include "scenemultilight.h"
#include "sceneperfragment.h"
#include "scenespot.h"
#include "scenetoon.h"
#include "scenepbr.h"

// Texturing.
#include "scenetexture.h"
#include "scenealphatest.h"
#include "scenemultitex.h"
#include "scenenormalmap.h"
#include "sceneprojtex.h"
#include "scenereflectcube.h"
#include "scenerefractcube.h"
#include "scenerendertotex.h"
#include "scenesamplerobj.h"
#include "scenediffibl.h"
#include "sceneparallax.h"

std::map<std::string, std::string> sceneInfo = {
	{ "phong", "Phong reflection mdoel (per-vertex)." },
	{ "diffuse", "Diffuse shading only" },
	{ "discard", "Example of discarding fragments" },
	{ "flat", "Flat shading" },
	{ "subroutine", "Using a shader subroutine" },
	{ "two-side",  "Two-sided lighting" },
	{ "directional", "Directional light source" },
	{ "fog", "Fog" },
	{ "multi-light", "Multiple light sources" },
	{ "per-frag", "Per-fragment shading" },
	{ "spot", "Spot light" },
	{ "toon", "Toon shading" },
	{ "pbr", "Physically based rendering (PBR) shader" },
	{ "alpha-test", "Discard fragments based on an alpha test." },
	{ "multi-tex", "Multiple textures" },
	{ "normal-map", "Normal map" },
	{ "proj-tex", "Projected texture" },
	{ "reflect-cube", "Reflection with a cube map" },
	{ "refract-cube", "Refraction with a cube map" },
	{ "render-to-tex", "Render to a texture using framebuffer objects" },
	{ "sampler-obj", "Sampler objects " },
	{ "texture", "Basic texture mapping" },
	{ "diff-ibl", "Diffuse image based lighting" },
	{ "parallax", "Parallax mapping" }
};

int main(int argc, char *argv[])
{

	std::string recipe = SceneRunner::parseCLArgs(argc, argv, sceneInfo);

	SceneRunner runner("Scenes" + recipe, 1920, 1440, 4);
	std::unique_ptr<Scene> scene;
	if (recipe == "phong") {
		scene = std::unique_ptr<Scene>(new ScenePhong());
	} else if( recipe == "diffuse") {
		scene = std::unique_ptr<Scene>(new SceneDiffuse());
	} else if( recipe == "discard" ) {
		scene = std::unique_ptr<Scene>(new SceneDiscard());
	} else if( recipe == "flat" ) {
		scene = std::unique_ptr<Scene>(new SceneFlat());
	} else if( recipe == "subroutine") {
		scene = std::unique_ptr<Scene>(new SceneSubroutine());
	} else if( recipe == "two-side" ) {
		scene = std::unique_ptr<Scene>(new SceneTwoSide());
	} else if( recipe == "directional" ) {
		scene = std::unique_ptr<Scene>( new SceneDirectional() );
	} else if( recipe == "fog") {
		scene = std::unique_ptr<Scene>( new SceneFog() );
	} else if( recipe == "multi-light") {
		scene = std::unique_ptr<Scene>( new SceneMultiLight() );
	} else if( recipe == "per-frag" ) {
		scene = std::unique_ptr<Scene>( new ScenePerFragment() );
	} else if( recipe == "spot") {
		scene = std::unique_ptr<Scene>( new SceneSpot() );
	} else if( recipe == "toon" ) {
		scene = std::unique_ptr<Scene>( new SceneToon() );
	} else if (recipe == "pbr") {
		scene = std::unique_ptr<Scene>(new ScenePbr());
	} else if( recipe == "alpha-test" ) {
		scene = std::unique_ptr<Scene>( new SceneAlphaTest() );
	} else if( recipe == "multi-tex") {
		scene = std::unique_ptr<Scene>( new SceneMultiTex() );
	} else if( recipe == "normal-map") {
		scene = std::unique_ptr<Scene>( new SceneNormalMap() );
	} else if( recipe == "proj-tex" ) {
		scene = std::unique_ptr<Scene>( new SceneProjTex() );
	} else if( recipe == "reflect-cube") {
		scene = std::unique_ptr<Scene>( new SceneReflectCube() );
	} else if( recipe == "refract-cube" ) {
		scene = std::unique_ptr<Scene>( new SceneRefractCube() );
	} else if( recipe == "render-to-tex" ) {
		scene = std::unique_ptr<Scene>( new SceneRenderToTex() );
	} else if( recipe == "sampler-obj" ) {
		scene = std::unique_ptr<Scene>( new SceneSamplerObj() );
	} else if( recipe == "texture" ) {
		scene = std::unique_ptr<Scene>( new SceneTexture() );
	} else if (recipe == "diff-ibl") {
		scene = std::unique_ptr<Scene>(new SceneDiffIbl());
	} else if (recipe == "parallax") {
		scene = std::unique_ptr<Scene>(new SceneParallax());
	}
	else {
		printf("Unknown recipe: %s\n", recipe.c_str());
		exit(EXIT_FAILURE);
	}

    return runner.run(std::move(scene));
}
